// image_selftest.cpp -- prove Image.h works on its own.
//
// Throwaway harness for the image loader: load bin/GAME.EXE into the model,
// register the stubs, resolve the imports, then read the IAT slots back out of
// model memory to show they hold non-zero stub VAs rather than zero.
//
// If Win32Stubs.h is not in the tree yet, this file supplies minimal stand-ins
// with the same names so the loader is testable before the stub lane lands.
//
//   g++ -std=c++03 -m32 -Isrc/handwritten -Ibuild/shim
//       -o /tmp/image_selftest build/image_selftest.cpp && /tmp/image_selftest
#include <cstdio>

#include "Image.h"

#ifndef TS_IMAGE_HAVE_WIN32_STUBS
namespace ts {
static unsigned g_calls = 0;
static const char *g_last = "";
// A stub is a `void f()` that pops its __stdcall arguments and sets EAX.
// These have none, so they only drop the return address the caller pushed.
static void selftest_stub() { ++g_calls; ESP.d += 4; EAX.d = 0; }
static void selftest_fallback() { ++g_calls; g_last = "unmapped"; ESP.d += 4; EAX.d = 0; }
static u32 g_stub_va = 0;
static u32 g_fallback_va = 0;
void register_win32_stubs() {
    if (g_stub_va) return;
    g_stub_va = 0x70000000u;
    g_fallback_va = 0x70001000u;
    ts_register(g_stub_va, selftest_stub);
    ts_register(g_fallback_va, selftest_fallback);
}
// Named imports resolve; ordinals miss, so the fallback path is exercised too.
u32 stub_va(const char *dll, const char *fn) {
    (void)dll;
    register_win32_stubs();
    if (fn && fn[0] == 0x23) return 0;   // "#<ordinal>": no named stub
    return g_stub_va;
}
u32 fallback_stub_va() { register_win32_stubs(); return g_fallback_va; }
unsigned stub_calls() { return g_calls; }
const char *stub_last_name() { return g_last; }
}  // namespace ts
#endif

int main(int argc, char **argv) {
    const char *path = argc > 1 ? argv[1] : "bin/GAME.EXE";
    if (!ts::load_image(path)) {
        std::printf("FAILED to load %s\n", path);
        return 1;
    }
    std::printf("load_image: base=0x%08x size_of_image=0x%x sections=%u\n",
                ts::image_base(), ts::image_size(), ts::image_section_count());
    std::printf("load_image: %u bytes mapped (%u header bytes)\n",
                ts::image_bytes_mapped(), ts::image_header_bytes());

    // The mapped bytes must be the real bytes: the first section begins with
    // the PE's first instruction at its real VA, and .rdata's IAT is a table of
    // name RVAs before resolution.
    std::printf("read-back .text[0] = 0x%08x (expect the real opcode bytes)\n",
                ts::ld32(0x00401000u));

    const unsigned total = ts::import_total();
    const unsigned written = ts::resolve_imports();
    std::printf("imports: total=%u written=%u resolved=%u ordinals=%u dlls=%u\n",
                total, written, ts::import_resolved(),
                ts::import_ordinal_total(), ts::import_dlls());

    // Read a few IAT slots back out of model memory.
    static const ts::u32 kSlots[] = {
        0x00698000u,  // ADVAPI32, first thunk in the IAT directory (VA 0x400000+0x298000)
        0x006980b8u,  // KERNEL32 (OriginalFirstThunk == 0: names came from here)
        0x00698358u,  // USER32 (same)
        0x006984f8u,  // WSOCK32, all-ordinal descriptor
    };
    unsigned nonzero = 0, zerocount = 0;
    for (unsigned i = 0; i < sizeof(kSlots) / sizeof(kSlots[0]); ++i) {
        const ts::u32 v = ts::ld32(kSlots[i]);
        std::printf("  IAT 0x%08x -> 0x%08x %s\n", kSlots[i], v,
                    v ? "" : "<-- STILL ZERO");
        if (v) ++nonzero; else ++zerocount;
    }
    // The second call must be idempotent: for KERNEL32/USER32 the names come
    // from the slots being overwritten, so a re-walk must not lose them.
    const unsigned total2 = ts::import_total();
    ts::resolve_imports();
    std::printf("idempotent: total=%u (was %u), resolved=%u\n", total2, total,
                ts::import_resolved());

    // The point of the exercise: `call dword ptr [IAT]` now reaches a stub
    // instead of address zero. ts_call is what a transcribed indirect call does.
    const ts::u32 esp0 = ts::mach().r[4].d;
    const ts::u32 before = ts::stub_calls();
    ts::push32(0xdeadbeefu);
    ts::ts_call(ts::ld32(0x006980b8u));
    std::printf("dispatch through IAT: stub_calls %u -> %u, ESP %s\n",
                before, ts::stub_calls(),
                ts::mach().r[4].d == esp0 ? "balanced" : "UNBALANCED");
    if (ts::stub_calls() == before) { std::printf("FAILED (IAT call did not dispatch)\n"); return 1; }

    if (zerocount || total != written || ts::import_resolved() == 0)
        { std::printf("FAILED\n"); return 1; }
    std::printf("OK: %u/%u IAT slots non-zero, %u real stubs, %u fallbacks\n",
                nonzero, (unsigned)(sizeof(kSlots) / sizeof(kSlots[0])),
                ts::import_resolved(), written - ts::import_resolved());
    return 0;
}
