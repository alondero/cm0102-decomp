// Export analysis evidence, not validated recovered source.
// @category CM0102
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import java.nio.file.*;
import java.io.*;
import java.nio.charset.StandardCharsets;

public class ExportBaseline extends GhidraScript {
    private PrintWriter writer(Path root, String name) throws Exception {
        return new PrintWriter(Files.newBufferedWriter(root.resolve(name), StandardCharsets.UTF_8));
    }
    public void run() throws Exception {
        Path root = Paths.get(getScriptArgs()[0]);
        Files.createDirectories(root);
        try (PrintWriter out = writer(root, "analysis-options.tsv")) {
            out.println("option\tvalue");
            java.util.Map<String, String> options = getCurrentAnalysisOptionsAndValues(currentProgram);
            for (String key : new java.util.TreeSet<String>(options.keySet()))
                out.println(key + "\t" + options.get(key));
        }
        try (PrintWriter out = writer(root, "program.txt")) {
            out.println("sha256=" + currentProgram.getExecutableSHA256());
            out.println("language=" + currentProgram.getLanguageID());
            out.println("compiler=" + currentProgram.getCompilerSpec().getCompilerSpecID());
            out.println("image_base=" + currentProgram.getImageBase());
            out.println("Addresses in exports are virtual addresses (VA).");
        }
        try (PrintWriter out = writer(root, "memory.tsv")) {
            out.println("name\tstart_va\tend_va\tread\twrite\texecute");
            for (MemoryBlock b : currentProgram.getMemory().getBlocks())
                out.printf("%s\t%s\t%s\t%b\t%b\t%b%n", b.getName(), b.getStart(), b.getEnd(), b.isRead(), b.isWrite(), b.isExecute());
        }
        try (PrintWriter out = writer(root, "functions.tsv")) {
            out.println("entry_va\tname\tbody_bytes\tsignature");
            for (Function f : currentProgram.getFunctionManager().getFunctions(true))
                out.printf("%s\t%s\t%d\t%s%n", f.getEntryPoint(), f.getName(), f.getBody().getNumAddresses(), f.getSignature());
        }
        try (PrintWriter out = writer(root, "imports.tsv")) {
            out.println("external_address\tname");
            SymbolIterator symbols = currentProgram.getSymbolTable().getExternalSymbols();
            while (symbols.hasNext()) {
                Symbol s = symbols.next();
                out.println(s.getAddress() + "\t" + s.getName(true));
            }
        }
        try (PrintWriter out = writer(root, "defined-strings.tsv")) {
            out.println("va\tvalue");
            for (Data d : currentProgram.getListing().getDefinedData(true)) {
                if (d.hasStringValue()) out.println(d.getAddress() + "\t" + d.getDefaultValueRepresentation().replace("\t", "\\t").replace("\n", "\\n").replace("\r", "\\r"));
            }
        }
        long[] entries = {0x947a13L, 0x68d370L, 0x5cc6d0L, 0x5cc6b0L, 0x68d520L, 0x5cc7d0L};
        DecompInterface decompiler = new DecompInterface();
        try {
            if (!decompiler.openProgram(currentProgram)) throw new IOException("Cannot open decompiler");
            try (PrintWriter asm = writer(root, "startup-disassembly.tsv"); PrintWriter refs = writer(root, "startup-references.tsv")) {
                asm.println("function_va\tinstruction_va\tinstruction");
                refs.println("from_va\tto_address\ttype");
                for (long entry : entries) {
                    Function f = currentProgram.getFunctionManager().getFunctionAt(toAddr(entry));
                    if (f == null) throw new IOException("Missing function at " + Long.toHexString(entry));
                    for (Instruction i : currentProgram.getListing().getInstructions(f.getBody(), true)) {
                        asm.println(f.getEntryPoint() + "\t" + i.getAddress() + "\t" + i);
                        for (Reference r : i.getReferencesFrom()) refs.println(r.getFromAddress() + "\t" + r.getToAddress() + "\t" + r.getReferenceType());
                    }
                    DecompileResults result = decompiler.decompileFunction(f, 120, monitor);
                    if (!result.decompileCompleted()) throw new IOException(result.getErrorMessage());
                    try (PrintWriter out = writer(root, "startup-" + Long.toHexString(entry) + ".c")) {
                        out.println("/* UNVALIDATED GHIDRA PSEUDOCODE. VA 0x" + Long.toHexString(entry) + ". Not buildable recovered source. */");
                        out.println(result.getDecompiledFunction().getC());
                    }
                }
            }
        } finally { decompiler.dispose(); }
    }
}
