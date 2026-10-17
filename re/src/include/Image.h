// Image.h -- map the target PE into the model's memory and resolve its imports.
//
// The machine model (Machine.h) has an address space but nothing in it, so a
// transcribed `call dword ptr [0x40xxxx]` -- an IAT slot -- reads zero and the
// call goes to 0x00000000. This header puts the real image at its real VAs and
// then writes a stub VA into every IAT slot, so an import call dispatches to a
// stub (Win32Stubs.h) instead of to address zero.
//
// The PE is parsed in C++ at run time from the file the caller names; there is
// no Python tool and no generated table in the path. `load_image` followed by
// `resolve_imports` is the whole dependency.
//
// What the target file actually required, all read out of bin/GAME.EXE:
//
//   * 4 sections, ImageBase 0x400000, SizeOfImage 0x440000, no relocation
//     directory (IMAGE_FILE_RELOCS_STRIPPED), so the image is mapped at its
//     preferred base and no fixups are applied -- and the absolute addresses
//     the transcriptions embed as immediates stay valid.
//   * no bound-import directory, so every IAT entry in the file is still the
//     RVA of an IMAGE_IMPORT_BY_NAME (or a set-high-bit ordinal) and not a
//     pre-resolved DLL address.
//   * 13 import descriptors / 341 thunks: 309 by name, 32 by ordinal
//     (OLEAUT32 11, WSOCK32 19, DSOUND 1, COMCTL32 1). The ordinals carry no
//     name string, so they are passed to stub_va as "#<ordinal>".
//   * KERNEL32 and USER32 have OriginalFirstThunk == 0, which means their name
//     table *is* their IAT. The names therefore have to be read before the
//     slots are overwritten, and a second walk would read stub VAs where it
//     expects name RVAs; the parsed table is cached for that reason.
//   * the import name strings live at RVA 0x43b000, inside .rsrc's raw extent.
//   * .data has VirtualSize 0x17b294 > SizeOfRawData 0x55000. The 0x122294-byte
//     tail is .bss and is left zero, which is what the model already reads for
//     a page nothing has written.
//
// Everything is `inline` so the header can be included by any translation unit
// (the pipeline compiles every header standalone and also concatenates them).
#ifndef TS_IMAGE_H
#define TS_IMAGE_H

#include <cstdio>
#include <cstring>
#include <vector>

#include "Machine.h"

// Win32Stubs.h is a separate lane's file. Include it when it is present so the
// two headers compose with one instance of the stub table; otherwise declare
// the same names here so Image.h compiles and is testable on its own.
#if defined(__has_include)
#  if __has_include("Win32Stubs.h")
#    include "Win32Stubs.h"
#    define TS_IMAGE_HAVE_WIN32_STUBS 1
#  endif
#endif
#ifndef TS_IMAGE_HAVE_WIN32_STUBS
// Minimal stand-in stub table, compiled only while Win32Stubs.h is absent, so
// this header is self-sufficient: it compiles and it links. Every import maps
// to one stub, which pops the return address and leaves EAX = 0 -- enough to
// prove a resolved IAT slot dispatches somewhere instead of to 0x00000000.
//
// It does not pop the __stdcall argument words, because without a per-name
// table the arity is not known, so ESP drifts by four bytes per argument. And
// stub_last_name() cannot name a function for the same reason. Both are what
// the real table exists to fix; neither is needed to test the loader.
namespace ts {
inline unsigned &image_stub_calls() { static unsigned n = 0; return n; }
inline const char *&image_stub_last() { static const char *p = ""; return p; }
inline void image_stub_body() {
    ++image_stub_calls();
    image_stub_last() = "no stub table (Win32Stubs.h absent)";
    ESP.d += 4;   // the return address ts_call's caller pushed
    EAX.d = 0;
}
inline u32 fallback_stub_va() {
    static u32 va = 0;
    if (va == 0) { va = 0x70000000u; ts_register(va, image_stub_body); }
    return va;
}
inline void register_win32_stubs() { fallback_stub_va(); }
inline u32 stub_va(const char *dll, const char *fn) {
    (void)dll; (void)fn;
    return fallback_stub_va();
}
inline unsigned stub_calls() { return image_stub_calls(); }
inline const char *stub_last_name() { return image_stub_last(); }
}  // namespace ts
#endif

namespace ts {
namespace pe {

inline u16 rd16(const u8 *p) { return (u16)((u16)p[0] | ((u16)p[1] << 8)); }
inline u32 rd32(const u8 *p) {
    return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

struct Section {
    u32 va;
    u32 vsize;
    u32 raw;
    u32 rawsize;
};

struct Import {
    u32 slot;        // VA of the IAT entry to overwrite
    bool ordinal;
    char dll[64];
    char fn[96];     // exported name, or "#<ordinal>"
};

struct ImageState {
    bool loaded;
    bool imports_parsed;
    u32 base;
    u32 size_of_image;
    unsigned bytes_mapped;
    unsigned header_bytes;
    unsigned section_count;
    unsigned import_dir_rva;
    unsigned descriptors;
    unsigned ordinal_total;
    unsigned resolved;
    std::vector<Section> sections;
    std::vector<Import> imports;
    ImageState()
        : loaded(false), imports_parsed(false), base(0), size_of_image(0),
          bytes_mapped(0), header_bytes(0), section_count(0),
          import_dir_rva(0), descriptors(0), ordinal_total(0), resolved(0) {}
};

inline ImageState &image() { static ImageState s; return s; }

// Copy `len` bytes of `src` to VA `dst`. The model hands out one independent
// allocation per 4 KiB page, so a single memcpy into page()'s pointer would run
// off the end of the first page's block; the copy is therefore page-clipped.
inline void copy_to_model(u32 dst, const u8 *src, u32 len) {
    u32 done = 0;
    while (done < len) {
        u32 off = (dst + done) & 0xfffu;
        u32 chunk = 4096u - off;
        if (chunk > len - done) chunk = len - done;
        std::memcpy(page(dst + done), src + done, chunk);
        done += chunk;
    }
}

// Is [rva, rva+len) inside a mapped section? Guards every RVA the import walk
// follows, so a malformed or unexpected descriptor reads zero rather than a
// page the model allocated for something else.
inline bool in_image(u32 rva, u32 len) {
    ImageState &s = image();
    for (size_t i = 0; i < s.sections.size(); ++i) {
        const Section &sec = s.sections[i];
        if (rva >= sec.va && rva + len <= sec.va + sec.vsize) return true;
    }
    return false;
}

// NUL-terminated string at VA `va`, copied into `out` (always terminated).
inline bool read_cstr(u32 va, char *out, unsigned cap) {
    if (cap == 0) return false;
    unsigned i = 0;
    for (; i + 1 < cap; ++i) {
        u8 c = ld8(va + i);
        out[i] = (char)c;
        if (c == 0) return true;
    }
    out[cap - 1] = 0;
    return true;
}

// Walk the import directory once and cache it. Cached because a descriptor
// with OriginalFirstThunk == 0 names its functions through the IAT itself, so a
// second walk would read stub VAs where it expects name RVAs.
inline void parse_imports() {
    ImageState &s = image();
    if (s.imports_parsed || !s.loaded) return;
    s.imports_parsed = true;
    if (s.import_dir_rva == 0) return;

    const u32 desc = s.base + s.import_dir_rva;
    for (unsigned i = 0; i < 512; ++i) {
        const u32 d = desc + 20 * i;
        const u32 oft = ld32(d);
        const u32 name_rva = ld32(d + 12);
        const u32 first = ld32(d + 16);
        if (oft == 0 && name_rva == 0 && first == 0) break;
        if (!in_image(name_rva, 1) || !in_image(first, 4)) continue;

        char dll[64];
        if (!read_cstr(s.base + name_rva, dll, sizeof dll)) continue;
        ++s.descriptors;

        // Without an OriginalFirstThunk the IAT is the name table. Both are
        // the same array then, so names are read from whichever exists and the
        // stub VA is always written to FirstThunk.
        const u32 names = s.base + (oft ? oft : first);
        for (unsigned k = 0; k < 4096; ++k) {
            const u32 v = ld32(names + 4 * k);
            if (v == 0) break;
            Import im;
            std::memset(&im, 0, sizeof im);
            im.slot = s.base + first + 4 * k;
            im.ordinal = (v & 0x80000000u) != 0;
            std::strncpy(im.dll, dll, sizeof im.dll - 1);
            if (im.ordinal) {
                std::sprintf(im.fn, "#%u", (unsigned)(v & 0xffffu));
                ++s.ordinal_total;
            } else {
                read_cstr(s.base + v + 2, im.fn, sizeof im.fn);
            }
            s.imports.push_back(im);
        }
    }
}

}  // namespace pe

// -- the shared interface ---------------------------------------------------

// Map the image at `path` into the model's memory at its real VAs. Sections are
// copied at VirtualAddress; a VirtualSize beyond SizeOfRawData is the zeroed
// tail (.bss) and is left alone. False on any structural surprise, with the
// reason on stderr. Bytes mapped are reported by image_bytes_mapped().
inline bool load_image(const char *path) {
    pe::ImageState &s = pe::image();
    s.loaded = false;
    s.imports_parsed = false;
    s.bytes_mapped = 0;
    s.header_bytes = 0;
    s.section_count = 0;
    s.descriptors = 0;
    s.ordinal_total = 0;
    s.resolved = 0;
    s.sections.clear();
    s.imports.clear();

    std::FILE *f = std::fopen(path, "rb");
    if (!f) { std::fprintf(stderr, "load_image: cannot open %s\n", path); return false; }
    std::fseek(f, 0, SEEK_END);
    const long sz = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    if (sz < 0x200) { std::fclose(f); std::fprintf(stderr, "load_image: too short\n"); return false; }
    std::vector<u8> buf((size_t)sz);
    const size_t got = std::fread(&buf[0], 1, (size_t)sz, f);
    std::fclose(f);
    if (got != (size_t)sz) { std::fprintf(stderr, "load_image: short read\n"); return false; }
    const u8 *d = &buf[0];
    const u32 filelen = (u32)sz;

    if (d[0] != 'M' || d[1] != 'Z') { std::fprintf(stderr, "load_image: not MZ\n"); return false; }
    const u32 peoff = pe::rd32(d + 0x3c);
    if (peoff + 0x78 > filelen || pe::rd32(d + peoff) != 0x00004550u) {
        std::fprintf(stderr, "load_image: no PE signature\n");
        return false;
    }
    const u8 *coff = d + peoff + 4;
    const unsigned nsec = pe::rd16(coff + 2);
    const unsigned optsz = pe::rd16(coff + 16);
    const u8 *opt = coff + 20;
    if (pe::rd16(opt) != 0x10b) { std::fprintf(stderr, "load_image: not PE32\n"); return false; }

    s.base = pe::rd32(opt + 28);
    s.size_of_image = pe::rd32(opt + 56);
    const u32 size_of_headers = pe::rd32(opt + 60);
    const u8 *dd = opt + 96;
    s.import_dir_rva = pe::rd32(dd + 8 * 1);
    const u32 reloc_rva = pe::rd32(dd + 8 * 5);
    const u32 bound_rva = pe::rd32(dd + 8 * 11);

    if (reloc_rva == 0)
        std::fprintf(stderr, "load_image: no .reloc; mapping at preferred base "
                             "0x%08x, no fixups\n", s.base);
    if (bound_rva != 0)
        std::fprintf(stderr, "load_image: WARNING bound-import directory "
                             "present; IAT entries may be pre-resolved\n");

    // Headers, so the image is readable from VA 0 rather than only from the
    // first section.
    if (size_of_headers && size_of_headers <= filelen) {
        pe::copy_to_model(s.base, d, size_of_headers);
        s.header_bytes = size_of_headers;
        s.bytes_mapped += size_of_headers;
    }

    const u8 *st = opt + optsz;
    for (unsigned i = 0; i < nsec; ++i) {
        const u8 *e = st + 40 * i;
        pe::Section sec;
        sec.vsize = pe::rd32(e + 8);
        sec.va = pe::rd32(e + 12);
        sec.rawsize = pe::rd32(e + 16);
        sec.raw = pe::rd32(e + 20);
        if (sec.va == 0) continue;
        u32 len = sec.rawsize < sec.vsize ? sec.rawsize : sec.vsize;
        if (len == 0) { s.sections.push_back(sec); continue; }  // pure .bss
        if (sec.raw + len > filelen) len = filelen - sec.raw;
        pe::copy_to_model(s.base + sec.va, d + sec.raw, len);
        s.bytes_mapped += len;
        s.sections.push_back(sec);
        ++s.section_count;
    }

    s.loaded = true;
    return true;
}

// Write a stub VA into every IAT slot. Returns the number of slots written;
// import_resolved() counts the ones that got a real stub rather than the
// logging fallback. Both are non-zero VAs, so no `call dword ptr [IAT]` reads
// address zero afterwards.
inline unsigned resolve_imports() {
    pe::ImageState &s = pe::image();
    if (!s.loaded) return 0;
    pe::parse_imports();
    register_win32_stubs();   // idempotent: ts_register overwrites by VA
    s.resolved = 0;
    for (size_t i = 0; i < s.imports.size(); ++i) {
        const pe::Import &im = s.imports[i];
        u32 va = stub_va(im.dll, im.fn);
        if (va) ++s.resolved;
        else va = fallback_stub_va();
        st32(im.slot, va);
    }
    return (unsigned)s.imports.size();
}

// -- accessors (additive; the four names above are the shared interface) -----

inline unsigned import_total() {
    if (pe::image().loaded) pe::parse_imports();
    return (unsigned)pe::image().imports.size();
}
inline unsigned import_resolved() { return pe::image().resolved; }
inline unsigned import_ordinal_total() {
    if (pe::image().loaded) pe::parse_imports();
    return pe::image().ordinal_total;
}
inline unsigned import_dlls() { return pe::image().descriptors; }
inline unsigned image_bytes_mapped() { return pe::image().bytes_mapped; }
inline unsigned image_header_bytes() { return pe::image().header_bytes; }
inline unsigned image_section_count() { return pe::image().section_count; }
inline u32 image_base() { return pe::image().base; }
inline u32 image_size() { return pe::image().size_of_image; }
inline bool image_loaded() { return pe::image().loaded; }

}  // namespace ts

#endif  // TS_IMAGE_H
