// ts_link_test.cpp -- link the whole recovered tree into a program and run it.
//
// The generated tree is 41,822 transcriptions of recovered machine functions
// plus 6,892 declaration-shaped forwarders. Compiling each file proves the
// syntax; it proves nothing about whether the tree LINKS, and a forwarder that
// names a transcription nothing defines is exactly the kind of breakage a
// per-file syntax check cannot see.
//
// So this links everything together and then drives it:
//
//   1. register_all_recovered() -- every transcription into the dispatch table;
//   2. a sample lookup, to prove dispatch resolves a VA to a body;
//   3. the target's real PE entry point (0x6854e3), run inside the model.
//
// Step 3 is the interesting one. It is the CRT startup of the original program,
// executed against the modelled machine: modelled memory, modelled stack, and
// every `call` routed through the recovered bodies. Nothing is asserted about
// what it achieves -- the point is that the whole recovered program runs and
// terminates without the model losing its footing. Unmapped calls are counted
// rather than guessed at, so the count is reported as-is.
#include <cstdio>

#include "Machine.h"
#include "code/recovered.h"

// The PE entry point of bin/GAME.EXE (ImageBase 0x400000 + RVA 0x2854e3).
static const ts::u32 kEntryPoint = 0x006854e3u;

// A handful of VAs from across .text, to show dispatch resolves more than the
// first entry in the table.
static const ts::u32 kSample[] = {
    0x00401000u, 0x00405790u, 0x00445640u, 0x006829a5u, 0x006854e3u,
};

int main() {
    std::printf("ts_link_test: linking the recovered tree succeeded\n");

    ts::register_all_recovered();

    unsigned resolved = 0;
    for (unsigned i = 0; i < sizeof(kSample) / sizeof(kSample[0]); ++i) {
        ts::Fn f = ts::ts_lookup(kSample[i]);
        std::printf("  dispatch 0x%08x -> %s\n", kSample[i],
                    f ? "resolved" : "NOT REGISTERED");
        if (f) ++resolved;
    }
    std::printf("ts_link_test: %u of %u sampled VAs resolved\n", resolved,
                (unsigned)(sizeof(kSample) / sizeof(kSample[0])));

    // Run the original entry point against the model.
    //
    // ESP is set to a plausible stack address first. The model starts it at 0,
    // so the first push wraps to 0xfffffffc and every later frame is at a
    // negative address; the pages are lazily allocated so it "works", but the
    // addresses are nonsense and make the run harder to read.
    ts::Machine &m = ts::mach();
    m.r[4].d = 0x00300000u;   // ESP
    m.unmapped_calls = 0;
    m.depth_exceeded = 0;
    m.unmapped_sample_n = 0;
    ts::ts_call(kEntryPoint);

    std::printf("ts_link_test: entry 0x%08x ran; EAX=0x%08x ESP=0x%08x\n",
                kEntryPoint, m.r[0].d, m.r[4].d);
    std::printf("ts_link_test: unmapped calls during entry: %lu\n",
                (unsigned long)m.unmapped_calls);
    std::printf("ts_link_test: unmodelled instructions: %lu\n",
                (unsigned long)m.unmodelled_insns);
    // If the depth guard fired, the entry point was CUT SHORT rather than run
    // to completion, and saying "it ran" without saying so would be dishonest.
    std::printf("ts_link_test: recursion depth limit: %lu; times hit: %lu\n",
                (unsigned long)m.call_depth_limit,
                (unsigned long)m.depth_exceeded);
    if (m.unmapped_sample_n) {
        std::printf("ts_link_test: first unmapped call targets:");
        for (unsigned i = 0; i < m.unmapped_sample_n; ++i)
            std::printf(" 0x%08x", m.unmapped_sample[i]);
        std::printf("\n");
    }
    if (m.depth_exceeded)
        std::printf("ts_link_test: NOTE the entry point was truncated by the "
                    "depth guard, not run to completion\n");

    // The gate is the link and the dispatch table. The entry-point run is a
    // measurement, not an assertion: the CRT startup needs a Windows heap, a
    // file system and kernel32, none of which the machine model has, so it is
    // expected to stop at an unmapped import rather than to finish. What is
    // asserted is that it runs, terminates, and does so without the model
    // losing its footing.
    if (resolved != sizeof(kSample) / sizeof(kSample[0])) {
        std::printf("ts_link_test: FAILED (dispatch incomplete)\n");
        return 1;
    }
    std::printf("ts_link_test: OK\n");
    return 0;
}
