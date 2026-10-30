// Export instruction-backed candidates. Does not rename or modify the program.
// @category CM0102
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;
import java.io.*;
import java.util.*;

public class ExportRecoveryMap extends GhidraScript {
    private PrintWriter writer(Path root, String name) throws IOException {
        return new PrintWriter(Files.newBufferedWriter(root.resolve(name), StandardCharsets.UTF_8));
    }
    private String bytes(Instruction i) throws Exception {
        StringBuilder s = new StringBuilder();
        for (byte b : i.getBytes()) s.append(String.format("%02x", b & 255));
        return s.toString();
    }
    public void run() throws Exception {
        if (!currentProgram.getExecutableSHA256().equalsIgnoreCase(
                "0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86"))
            throw new IOException("Wrong executable");
        Path root = Paths.get(getScriptArgs()[0]);
        try (PrintWriter out = writer(root, "source-references.tsv")) {
            out.println("string_va\tpath\tfrom_va\treference_type\tinstruction\tbytes\tfunction_va");
            for (String line : Files.readAllLines(root.resolve("source-paths.tsv"), StandardCharsets.UTF_8)) {
                if (line.startsWith("va\t")) continue;
                String[] fields = line.split("\t", 2);
                ReferenceIterator refs = currentProgram.getReferenceManager().getReferencesTo(toAddr(Long.parseLong(fields[0], 16)));
                while (refs.hasNext()) {
                    Reference r = refs.next();
                    Instruction i = currentProgram.getListing().getInstructionAt(r.getFromAddress());
                    Function f = currentProgram.getFunctionManager().getFunctionContaining(r.getFromAddress());
                    out.printf("%s\t%s\t%s\t%s\t%s\t%s\t%s%n", fields[0], fields[1],
                        r.getFromAddress(), r.getReferenceType(), i == null ? "" : i,
                        i == null ? "" : bytes(i), f == null ? "" : f.getEntryPoint());
                }
            }
        }
        try (PrintWriter out = writer(root, "calls.tsv")) {
            out.println("function_va\tsite_va\tbytes\tinstruction\ttarget_address\treference_type");
            for (Instruction i : currentProgram.getListing().getInstructions(true)) {
                if (!i.getFlowType().isCall()) continue;
                Function f = currentProgram.getFunctionManager().getFunctionContaining(i.getAddress());
                boolean emitted = false;
                for (Reference r : i.getReferencesFrom()) {
                    if (!r.getReferenceType().isCall()) continue;
                    out.printf("%s\t%s\t%s\t%s\t%s\t%s%n", f == null ? "" : f.getEntryPoint(),
                        i.getAddress(), bytes(i), i, r.getToAddress(), r.getReferenceType());
                    emitted = true;
                }
                if (!emitted) out.printf("%s\t%s\t%s\t%s\t\tUNRESOLVED%n",
                    f == null ? "" : f.getEntryPoint(), i.getAddress(), bytes(i), i);
            }
        }
        DecompInterface decompiler = new DecompInterface();
        try {
            if (!decompiler.openProgram(currentProgram)) throw new IOException("Cannot open decompiler");
            try (PrintWriter asm = writer(root, "selected-disassembly.tsv");
                 PrintWriter refs = writer(root, "selected-references.tsv")) {
                asm.println("function_va\tinstruction_va\tbytes\tinstruction");
                refs.println("function_va\tfrom_va\tto_address\ttype");
                for (int n = 1; n < getScriptArgs().length; n++) {
                    long entry = Long.parseLong(getScriptArgs()[n], 16);
                    Function f = currentProgram.getFunctionManager().getFunctionAt(toAddr(entry));
                    if (f == null) throw new IOException("Missing function: " + Long.toHexString(entry));
                    for (Instruction i : currentProgram.getListing().getInstructions(f.getBody(), true)) {
                        asm.printf("%s\t%s\t%s\t%s%n", f.getEntryPoint(), i.getAddress(), bytes(i), i);
                        for (Reference r : i.getReferencesFrom()) refs.printf("%s\t%s\t%s\t%s%n",
                            f.getEntryPoint(), r.getFromAddress(), r.getToAddress(), r.getReferenceType());
                    }
                    DecompileResults result = decompiler.decompileFunction(f, 120, monitor);
                    if (!result.decompileCompleted()) throw new IOException(result.getErrorMessage());
                    try (PrintWriter out = writer(root, "function-" + Long.toHexString(entry) + ".c")) {
                        out.println("/* UNVALIDATED GHIDRA PSEUDOCODE. Preferred-image VA. Not recovered source. */");
                        out.println(result.getDecompiledFunction().getC());
                    }
                }
            }
        } finally { decompiler.dispose(); }
        Files.writeString(root.resolve("export-complete.txt"), "Complete\n", StandardCharsets.UTF_8);
    }
}
