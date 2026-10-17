// DecompileAt -- decompile the function(s) containing the given addresses.
//
// Run against the already-analysed TS project so a query costs seconds rather
// than a re-analysis:
//
//   analyzeHeadless ghidra-proj TS -process GAME.EXE -noanalysis \
//       -scriptPath re/tools/ghidra_scripts \
//       -postScript DecompileAt.java 0x5e7294 0x674be0
//
// The point is semantic questions. objdump tells you an instruction; this tells
// you what the function is FOR, which is the slow part of every diagnosis here.

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class DecompileAt extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) {
            println("usage: DecompileAt <hex-address> [more...]");
            return;
        }
        DecompInterface ifc = new DecompInterface();
        ifc.toggleCCode(true);
        ifc.toggleSyntaxTree(true);
        if (!ifc.openProgram(currentProgram)) {
            println("could not open program");
            return;
        }
        for (String a : args) {
            Address addr;
            try {
                addr = toAddr(a);
            } catch (Exception e) {
                println("bad address: " + a);
                continue;
            }
            Function f = getFunctionContaining(addr);
            if (f == null) {
                println("=== no function contains " + a + " ===");
                continue;
            }
            println("=== " + f.getName() + " @ " + f.getEntryPoint()
                    + "  (asked " + a + ") ===");
            DecompileResults res = ifc.decompileFunction(f, 90, monitor);
            if (res != null && res.decompileCompleted()) {
                println(res.getDecompiledFunction().getC());
            } else {
                println("decompile failed: "
                        + (res == null ? "null" : res.getErrorMessage()));
            }
        }
        ifc.dispose();
    }
}
