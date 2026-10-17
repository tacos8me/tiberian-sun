// image_selftest.cpp -- prove Image.h works on its own.
//
// Load bin/GAME.EXE into the model, register the stubs, resolve the imports,
// then read IAT slots back out of model memory to show they hold non-zero stub
// VAs rather than zero, and that a call through one reaches a stub.
//
// It uses whatever stub table is in the tree: Win32Stubs.h when present,
// otherwise the stand-in Image.h compiles in so the loader is testable alone.
//
// Standalone (no generated tree):
//
//   g++ -std=c++03 -m32 -Isrc/handwritten -Ibuild/shim \
//       -o /tmp/image_selftest build/image_selftest.cpp && /tmp/image_selftest
//
// With the recovered tree linked, it also runs the real PE entry point, which
// is the measurement that matters: resolving the IAT is what stops the 16,383
// `call dword ptr [IAT]` instructions from reading address zero.
//
//   g++ -std=c++03 -O0 -m32 -DTS_SELFTEST_WITH_TREE -Isrc/include \
//       -Isrc/generated -Ibuild/shim -o /tmp/image_entry_test \
//       build/image_selftest.cpp src/generated/code/*.cpp && /tmp/image_entry_test
#include <cstdio>

#include "Image.h"

#ifdef TS_SELFTEST_WITH_TREE
#include "code/recovered.h"
#endif

static int fail(const char *why) { std::printf("FAILED: %s\n", why); return 1; }

int main(int argc, char **argv) {
    const char *path = argc > 1 ? argv[1] : "bin/GAME.EXE";

#ifdef TS_SELFTEST_WITH_TREE
    ts::register_all_recovered();
#endif
    if (!ts::load_image(path)) return fail("load_image");

    std::printf("load_image: base=0x%08x size_of_image=0x%x sections=%u\n",
                ts::image_base(), ts::image_size(), ts::image_section_count());
    std::printf("load_image: %u bytes mapped (%u header bytes)\n",
                ts::image_bytes_mapped(), ts::image_header_bytes());

    // The mapped bytes must be the real bytes, at their real VAs.
    std::printf("read-back .text[0] = 0x%08x (real opcode bytes, not zero)\n",
                ts::ld32(0x00401000u));

    const unsigned total = ts::import_total();
    const unsigned written = ts::resolve_imports();
    std::printf("imports: total=%u written=%u resolved=%u ordinals=%u dlls=%u\n",
                total, written, ts::import_resolved(),
                ts::import_ordinal_total(), ts::import_dlls());
    if (written != total) return fail("not every IAT slot was written");

    // Show the parsed table really names the DLLs and functions.
    {
        const std::vector<ts::pe::Import> &im = ts::pe::image().imports;
        std::printf("first 3 parsed: ");
        for (size_t i = 0; i < 3 && i < im.size(); ++i)
            std::printf("%s!%s@0x%08x ", im[i].dll, im[i].fn, im[i].slot);
        std::printf("\nlast 2 parsed:  ");
        for (size_t i = im.size() >= 2 ? im.size() - 2 : 0; i < im.size(); ++i)
            std::printf("%s!%s@0x%08x ", im[i].dll, im[i].fn, im[i].slot);
        std::printf("\n");
    }

    // Read IAT slots back out of model memory: VAs, not zero.
    static const ts::u32 kSlots[] = {
        0x00698000u,  // ADVAPI32, first thunk in the IAT directory
        0x006980b8u,  // KERNEL32 (OriginalFirstThunk == 0: names came from here)
        0x00698358u,  // USER32 (same)
        0x006984f8u,  // WSOCK32, all-ordinal descriptor
    };
    const unsigned nslots = (unsigned)(sizeof(kSlots) / sizeof(kSlots[0]));
    unsigned nonzero = 0;
    for (unsigned i = 0; i < nslots; ++i) {
        const ts::u32 v = ts::ld32(kSlots[i]);
        std::printf("  IAT 0x%08x -> 0x%08x %s\n", kSlots[i], v,
                    v ? "" : "<-- STILL ZERO");
        if (v) ++nonzero;
    }
    if (nonzero != nslots) return fail("an IAT slot still reads zero");

    // The point of the exercise: `call dword ptr [IAT]` reaches a stub. ts_call
    // is what a transcribed indirect call does.
    const ts::u32 esp0 = ts::mach().r[4].d;
    const unsigned before = ts::stub_calls();
    ts::push32(0xdeadbeefu);
    ts::ts_call(ts::ld32(0x006980b8u));
    const bool balanced = ts::mach().r[4].d == esp0;
    std::printf("dispatch through IAT: stub_calls %u -> %u, ESP %s\n", before,
                ts::stub_calls(), balanced ? "balanced" : "UNBALANCED");
    if (ts::stub_calls() == before) return fail("IAT call did not dispatch");

    // A second pass must be idempotent: for KERNEL32/USER32 the names come from
    // the very slots the first pass overwrote, so the walk must not lose them.
    const unsigned total2 = ts::import_total();
    ts::resolve_imports();
    std::printf("idempotent: total=%u (was %u), resolved=%u\n", total2, total,
                ts::import_resolved());
    if (total2 != total) return fail("second pass lost imports");

#ifdef TS_SELFTEST_WITH_TREE
    // The payoff: the original PE entry point, run against the model with the
    // image loaded. Without resolve_imports it reports 16,383 unmapped calls,
    // all at 0x00000000.
    ts::Machine &m = ts::mach();
    m.r[4].d = 0x00300000u;   // ESP, as build/ts_link_test.cpp sets it
    m.unmapped_calls = 0;
    m.depth_exceeded = 0;
    m.unmapped_sample_n = 0;
    ts::ts_call(0x006854e3u);
    std::printf("entry 0x006854e3 ran; EAX=0x%08x unmapped_calls=%lu "
                "depth_limit_hits=%lu\n",
                m.r[0].d, m.unmapped_calls, m.depth_exceeded);
    if (m.unmapped_sample_n) {
        std::printf("first unmapped targets:");
        for (unsigned i = 0; i < m.unmapped_sample_n; ++i)
            std::printf(" 0x%08x", m.unmapped_sample[i]);
        std::printf("\n");
    }
    std::printf("stub calls: %u, last=%s\n", ts::stub_calls(),
                ts::stub_last_name());
#endif

    std::printf("OK: %u/%u IAT slots non-zero, %u/%u imports given a stub VA\n",
                nonzero, nslots, ts::import_resolved(), total);
    return 0;
}
