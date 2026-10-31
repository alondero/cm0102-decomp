# Instruction-backed recovery navigation

Primary input: `CM0102/Game/cm0102.exe`, official 3.9.68, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
All addresses are preferred-image VAs, base `0x00400000`.
This is static navigation evidence, not newly compiled or validated engine code.

## Reproduce

```powershell
python scripts/map_recovery.py --output CM0102/Analysis/recovery-map-NEW `
  --functions 005cccd0 0081b890 005b5d10 0054a130 0054c5b0
```

The runner verifies the primary input/copy, pinned Ghidra 12.1.4 archive and
every distributed file, then processes accepted `baseline-c/cm0102.gpr` with
`-readOnly -noanalysis`. No names/comments are written to the project. Paths are
extracted directly from the binary; the community catalog is not an input.
The manifest records commands, Java/Python/OS identity, script/output hashes.
Existing outputs are refused.

`ExportRecoveryMap.java` follows references to source-path strings, retaining
instruction bytes, reference types and enclosing functions. Non-instruction or
unowned references remain in the raw TSV but do not produce ownership candidates.
Python checks exported source-reference and call bytes against the disk image.
Calls come from decoded call instructions, not searches for E8 bytes. Unresolved
register/memory-indirect calls remain explicit. Tail jumps and unrecognized code
need separate investigation. Matching bytes do not prove correct boundaries.

## Results

Two read-only exports, `recovery-map-a` and `recovery-map-b`, agreed byte-for-byte
on eight common artifacts: paths, references, calls, module candidates,
orchestration calls, summary, and both common pseudocode files. This tests export
from the same project, not an independent import; M1 has the separate c/d imports.

| Inventory | Count |
| --- | ---: |
| Embedded source-path strings | 397 |
| Distinct C++ paths | 396 |
| Header paths (`network.h`) | 1 |
| References to source strings | 6,305 |
| Functions with instruction-backed path candidates | 1,925 |
| Single-path candidates | 1,896 |
| Multiple-path candidates | 29 |
| Distinct decoded call sites | 121,510 |
| Call records (some sites have multiple targets) | 121,519 |
| Unresolved indirect call sites | 1,564 |

Two binary paths absent from the community source-path catalog are
`cm3\code\comp\worldcup\wc_south_american_league.cpp` and
`cm3\code\comp\leagues\wel_first.cpp`. Direct-binary counts supersede the earlier
shorthand of 395 C++ modules; a path does not prove complete unit recovery.

Single-path candidate examples: Database.cpp 50, human_manager.cpp 50,
transfer_manager.cpp 42, tactics_screens.cpp 41, formation.cpp 33. No recovery
percentage is claimed: assertions cover only some functions and inlined/header
code can attribute multiple paths to a function.

## Using the artifacts

- `module-candidates.json`: candidate modules with exact referring instructions.
  Do not attribute nearby functions by proximity.
- `calls.tsv`: decoded calls, including unresolved sites; not a complete CFG.
- `selected-disassembly.tsv` / `selected-references.tsv`: selected instruction
  evidence, including bytes.
- `function-*.c`: explicitly unvalidated pseudocode for requested functions only.
- `orchestration-calls.json`: 152 sites, 150 direct and 2 unresolved indirect,
  92 distinct direct targets. See [ORCHESTRATION.md](ORCHESTRATION.md).

Ownership stays **candidate**, even for one path. No semantic renaming,
data-pool reclassification or compilation occurs. The `.text` constant-pool
problem remains open. Existing RNG compiled/validated/not-integrated status is
unchanged.

## Current maintained selection

`scripts/map_crt_length_family.py` extends the accepted probe/search-composed
morphology selection with [bounded-length CRT support](CRT-LENGTH.md). Fresh
`crt-length-family-final-a/b` agree on nine artifacts and eighteen pinned inputs:
**45 application plus six CRT entries**, 16,340 decoded selected calls and three
direct tails. Thirteen ledgers record the five recovery statuses independently.
No selected routine is integrated.

Every previous map, ledger and audit remains unchanged. This maintained selection
is separate from the original source-path navigation inventory.
[Deeper locale recovery](CRT-LOCALE-DISCOVERY.md) has a compiled, finite-corpus
validated candidate ledger only; it adds no maintained entry or implicit adoption.
Its shared exception dependencies now have a separate 433-byte / 158-instruction
static map. An isolated source candidate compiles those instructions at their
original VAs and passes 1,964 synthetic callback cases covering all 158
instructions; Windows exception behavior is still unvalidated and the candidate
is not accepted into the maintained ledger. The [bounded runtime replay](RUNTIME-REPLAY.md)
and [isolated primary-reference VM](RUNTIME-VM.md) do not change recovery counts
or integration status. VM sign-in is now resolved and the unchanged primary
completes new game, save/restart/load, progression and a simulated friendly at
16-bit colour. Repeated-run determinism and consumed-input replay remain open.
