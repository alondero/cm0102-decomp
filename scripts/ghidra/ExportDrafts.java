// Export UNVALIDATED Ghidra pseudocode for every function, one file per entry.
// Drafts are translation input only; acceptance comes solely from `cmre accept --c`.
// @category CM0102
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.app.decompiler.parallel.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.data.DataType;
import ghidra.program.model.pcode.HighFunction;
import ghidra.program.model.pcode.HighSymbol;
import ghidra.util.task.TaskMonitor;
import java.nio.file.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class ExportDrafts extends GhidraScript {
    public void run() throws Exception {
        Path root = Paths.get(getScriptArgs()[0]);
        Files.createDirectories(root);
        List<Function> functions = new ArrayList<>();
        for (Function f : currentProgram.getFunctionManager().getFunctions(true)) functions.add(f);
        final Map<String, String> status = Collections.synchronizedMap(new TreeMap<>());
        DecompileConfigurer configurer = d -> {
            DecompileOptions options = new DecompileOptions();
            options.grabFromProgram(currentProgram);
            d.setOptions(options);
            d.toggleCCode(true);
            d.toggleSyntaxTree(true);
            d.setSimplificationStyle("decompile");
        };
        DecompilerCallback<Void> callback = new DecompilerCallback<Void>(currentProgram, configurer) {
            @Override
            public Void process(DecompileResults result, TaskMonitor m) throws Exception {
                Function f = result.getFunction();
                String va = String.format("%08x", f.getEntryPoint().getOffset());
                if (!result.decompileCompleted() || result.getDecompiledFunction() == null) {
                    status.put(va, "failed\t" + String.valueOf(result.getErrorMessage()).replace('\t', ' ').replace('\n', ' '));
                    return null;
                }
                String text = "/* UNVALIDATED GHIDRA 12.1.4 PSEUDOCODE for preferred-image VA 0x" + va
                        + ". Draft input only: verify every statement against `cmre show --asm`. */\n"
                        + result.getDecompiledFunction().getC();
                Files.write(root.resolve("fn_" + va + ".c"), text.getBytes(StandardCharsets.UTF_8));
                // Referenced globals with the decompiler's data types: name, VA, type, byte length.
                StringBuilder globals = new StringBuilder("name\tva\ttype\tlength\n");
                HighFunction high = result.getHighFunction();
                if (high != null) {
                    Iterator<HighSymbol> symbols = high.getGlobalSymbolMap().getSymbols();
                    while (symbols.hasNext()) {
                        HighSymbol s = symbols.next();
                        if (s.getStorage() == null || s.getStorage().getMinAddress() == null) continue;
                        DataType t = s.getDataType();
                        globals.append(s.getName()).append('\t')
                               .append(String.format("%08x", s.getStorage().getMinAddress().getOffset())).append('\t')
                               .append(t == null ? "" : t.getDisplayName()).append('\t')
                               .append(t == null ? 0 : t.getLength()).append('\n');
                    }
                }
                Files.write(root.resolve("fn_" + va + ".globals.tsv"), globals.toString().getBytes(StandardCharsets.UTF_8));
                status.put(va, "ok\t");
                return null;
            }
        };
        callback.setTimeout(120);
        try {
            ParallelDecompiler.decompileFunctions(callback, functions, monitor);
        } finally {
            callback.dispose();
        }
        try (PrintWriter out = new PrintWriter(Files.newBufferedWriter(root.resolve("index.tsv"), StandardCharsets.UTF_8))) {
            out.println("va\tstatus\terror");
            for (Map.Entry<String, String> e : status.entrySet()) out.println(e.getKey() + "\t" + e.getValue());
        }
        println("ExportDrafts: " + status.size() + " of " + functions.size() + " functions processed");
    }
}
