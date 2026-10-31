# Reproduce the initial analysis

Later-session status: [RECOVERY-MAP.md](RECOVERY-MAP.md) adds read-only source
attribution and decoded calls. [RUNTIME.md](RUNTIME.md) records the first completed
GDI new game/save/load/matches. The initial-session results below are historical;
the original accepted M1 exports remain unchanged.

Read [the roadmap](ROADMAP.md) and [the startup evidence](STARTUP.md). This is
analysis infrastructure; generated C files are unvalidated pseudocode, not a
buildable source port.

## Tool and input identity

- Input: `CM0102/Game/cm0102.exe`, official 3.9.68, SHA-256
  `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
- [Ghidra 12.1.4 official release](https://github.com/NationalSecurityAgency/ghidra/releases/tag/Ghidra_12.1.4_build),
  archive `ghidra_12.1.4_PUBLIC_20260921.zip`, SHA-256
  `ddac49f903da9d5bac833e5cc79395098b9c33cfd3279be5f31bd00387d2d4db`.
  The digest was obtained from the official GitHub release API and checked locally.
- Java: existing JDK 21.0.10 at `C:/Program Files/Java/jdk-21.0.10`; exact Java,
  Python and OS versions are in each `run.json`.
- Language `x86:LE:32:default`, compiler specification `windows` (Ghidra's Visual
  Studio ABI model; not a claim that the original compiler version is known).
- Ghidra's release-default PE loader and analyzers. The loader consults this
  host's SysWOW64 DLLs and cached export symbols but saves no DLL programs to the
  project. Effective analysis options are exported to `analysis-options.tsv`.

Installation is portable under ignored `tools/`. No persistent PATH or Java
settings are changed. Ghidra may create its usual per-user caches/preferences.
The runner verifies every distributed file against the pinned archive before
execution. Use a clean extracted distribution without added extensions; the
verification covers archive members, not arbitrary added plugins.

The initial reproducibility check uses separate fresh projects on this host.
It does not establish identical output across Windows releases or user profiles.
The logs record consulted DLLs, export caches and loaded preferences. Preserve
those dependencies when reproducing on another machine, or explicitly disable
external-symbol lookup and establish a new compared baseline. Do not assume the
release archive alone fixes every environmental input.

## Commands

From the workspace root, for a fresh machine with Java 21 installed:

```powershell
# Once, only when this portable installation does not already exist:
.\scripts\setup_ghidra.ps1

py -3.13 scripts/verify_game.py
py -3.13 scripts/analyze_baseline.py --output CM0102/Analysis/repro-a
py -3.13 scripts/analyze_baseline.py --output CM0102/Analysis/repro-b
py -3.13 scripts/compare_analysis.py CM0102/Analysis/repro-a CM0102/Analysis/repro-b
py -3.13 scripts/verify_game.py
```

Pass `--java-home` for another JDK location and `--ghidra` for another extraction
of this exact release, retaining its `ghidra_12.1.4_PUBLIC` directory name.
Existing output directories are always refused. A failed
run stays available for diagnosis; use a new output path on retry. The runner
only copies and analyzes the executable; it does not run the game.

To inspect a successful project interactively, launch
`tools/ghidra/ghidra_12.1.4_PUBLIC/ghidraRun.bat`, then open the run directory's
`cm0102.gpr` and its `cm0102.exe` program. Annotations made interactively belong
in scripts/notes before they can be reproduced in another fresh project.

## Outputs

Each run contains an identical executable copy, Ghidra project, `headless.log`,
`run.json`, and `exports/`. The run manifest records the exact command, script
hashes, tool identity, input/copy/post-analysis input hashes, and export hashes.
Keep logs: analyzer warnings and missing external DLLs affect the interpretation.

| Export | Meaning |
| --- | --- |
| `pe.json` | Disk sections, raw ranges, preferred image base, entry RVA/VA |
| `memory.tsv` | Ghidra's loaded blocks and permissions |
| `program.txt` | Imported hash and language/compiler identity |
| `analysis-options.tsv` | Effective analysis option values |
| `functions.tsv` | Detected function entry points, names, signatures, body byte counts; not recovery coverage |
| `imports.tsv` | Ghidra external symbols; EXTERNAL addresses are synthetic symbol-space addresses, not VAs |
| `raw-strings.tsv` | Printable ASCII/ASCII-subset UTF-16LE runs of at least five characters, with file offset and mapped RVA/VA |
| `defined-strings.tsv` | Strings recognized by Ghidra, with VAs |
| `startup-disassembly.tsv` | Instructions in the six selected startup functions, with VAs |
| `startup-references.tsv` | References identified by Ghidra; unresolved indirect calls remain visible in `startup-disassembly.tsv` |
| `startup-*.c` | Six unvalidated decompiler outputs; comments explicitly mark their status |

Raw strings are candidates: accidental printable data, duplicate encodings and
unrecognized non-ASCII text are expected. PE section conversion uses
`RVA = section.VirtualAddress + file_offset - section.PointerToRawData`, then
`VA = image_base + RVA`. It does not map zero-filled virtual tails to disk bytes.
Unmapped raw strings have blank RVA/VA columns. Ghidra can misidentify code/data,
types, function boundaries and indirect calls. Preserve those uncertainties.

The comparison requires complete exports and successful manifests with matching
input/tool/script identities. It compares export bytes exactly; project database
files, paths, timestamps and logs are not compared. Matching exports demonstrate
reproducible static analysis, not correctness of the decompiler or engine parity.

`CM0102/Analysis/` is local and ignored because projects, copies, strings and
pseudocode are derived game artifacts. Scripts and focused notes are the durable
handoff. Nothing was published and no Git repository was created.

## Session results: 2026-09-27

**M1 achieved for fresh projects on this host.** The successful compared runs are
`CM0102/Analysis/baseline-c/` and `CM0102/Analysis/baseline-d/`. Open either
`cm0102.gpr`. Both completed headless analysis and all six decompilations. All
**16 exported files match byte-for-byte**, including effective analysis options,
inventories, startup instructions/references, and pseudocode.

The inventory contains four PE sections, 9,241 detected functions, 145 external
import symbols, 17,398 Ghidra-defined strings, and 41,229 raw string candidates.
The six startup functions contribute 370 exported instructions. These counts
describe this analyzer run, not verified recovery coverage or all possible text.

Before and after this work, `scripts/verify_game.py` passed all 1,558 baseline
files and all 46 official patch payloads. The eleven previously recorded runtime
files remain additional files. No game executable, data file or manifest was
changed. New-game completion, matches and save/load remain unverified.

Validation also covered Python compilation and rejection of an incorrect input,
self-comparison, incomplete export sets, and a renamed Ghidra extraction root.
The final directory-name guard was added after the two imports, then checked
directly; it does not change their default command or analysis. Exact script
versions used for those runs are preserved in `CM0102/Analysis/runner-used/`
and match their recorded script hashes. Future runs record the current versions.

Warnings were retained, not hidden: Windows library export/resource parsing
warnings; Ghidra no-return-name normalization warnings; no PDB metadata; and
external DLLs not saved to the project. `environment-observed.json` under
`CM0102/Analysis/` records post-analysis hashes of consulted DLLs, export caches,
and preferences named in the log. This is an observation, not a pre-run lockfile.
Cross-host reproducibility remains to be established.

`baseline-a/` is an abandoned attempt started before extraction finished;
`baseline-b/` is an exploratory successful import using an earlier runner.
Neither is the accepted comparison pair. They remain ignored for diagnosis.

Review results:

| Axis | Findings and resolution |
| --- | --- |
| Standards | Baselines preserved; extracted-tool verification, complete-run comparison guards and extraction-root validation added; imprecise export descriptions corrected |
| Spec | Fidelity contract and seven milestone gates match the request; no invented engine logic introduced; static results explicitly separated from runtime/source equivalence |

The next work is M2's runtime reference plus investigation of the M3 candidate
documented in [STARTUP.md](STARTUP.md). The roadmap, reproduction tooling and
startup evidence are complete; no claim of recovered runnable application code
is made.
