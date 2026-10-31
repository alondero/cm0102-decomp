# Locale mapping and deeper classification candidate

Primary input: official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
Addresses are preferred-image VAs, image base `0x00400000`.
No preserved binary, asset, manifest or earlier acceptance record changed.

This is an **isolated candidate**, not an accepted maintained CRT ledger or
case/morphology adoption. Its local ledger separately records identified,
decompiled, compiled and finite-corpus validated true; integrated false.
The maintained thirteen-ledger inventory remains 45 application/six CRT entries.

## Instruction closure

Read-only Ghidra export: `CM0102/Analysis/crt-locale-closure-static-b`.
The failed `static-a` is retained: the shared dispatcher has no Ghidra function
boundary. Capstone 5.0.7 verifies recovered instructions against original bytes,
including local SEH blocks omitted from the normal Ghidra bodies.

| Entry | Complete envelope | Bytes | Ghidra-selected | Extra local SEH |
| --- | --- | ---: | ---: | ---: |
| Mapping `0x0094C0FE` | through `0x0094C321` | 548 | 511 | 37 |
| Text classification `0x0094ED85` | through `0x0094EECD` | 329 | 318 | 11 |
| Fill `0x0094A640` | through `0x0094A697` | 88 | 88 | 0 |

These **965 bytes / 356 instructions** include three filters returning one and
their recovery blocks. Scope tables at `0x00971600` and `0x00971960` verify the
exact entries. Shared dispatcher `0x0094BF5C` remains unrecovered. Region hashes:

- Mapping: `bab06d0092064c4d63aec87fbd452e7232e26baf2fe68ced7027fd8a936c5468`.
- Classification: `94f18fb544eaf0f7aff6ac60a129b69161e31920ddbc4bbb9cfcb544a002f9a2`.
- Fill: `2af9ec6db955aa74b3de79ebe2752b3fa148e02935ac40304eb9ffc13ccf0d2c`.

Mapping probes W/A support when its cached mode is zero. Only positive signed
input lengths call [maintained length support](CRT-LENGTH.md); the result
overwrites the caller length slot. It rereads the cached mode afterward. The
wide path writes a default code page into a zero caller slot, queries conversion
with flags one/nine, allocates with original low-byte alignment arithmetic, and
converts with flags one. Flag `0x400` selects sort-key handling and signed output
capacity checks. Other flags allocate a second temporary and convert wide output
to bytes with flags `0x220`. Capacity zero forces both final API output arguments
to zero. Original size/return distinctions and unchecked arithmetic remain.

Classification has its own cached mode. Its A path substitutes the default
locale for a zero argument. Its W path writes a default code page to the caller
slot, queries conversion, allocates and clears twice the queried count, converts,
then classifies using the **second conversion's returned count**. Fill preserves
alignment byte writes, REP STOSD, incoming DF and caller argument reloads.

## Corrected evidence

`crt-locale-candidate-test-h/report.json` passes **5,757 comparisons**. Candidate
instructions plus maintained probe/length comprise **393 compiled instruction
records; all 393 execute** in the corpus. The audit independently decodes the PE
and checks every instruction and its original VA. The corpus executes 2,484
probe, 1,875 length and 1,710 fill calls.

Imported calls are arbitrary scripted callbacks for `LCMapStringW/A`,
`MultiByteToWideChar`, `WideCharToMultiByte` and `GetStringTypeW/A`. Outcomes
include failure, non-BOOL success, inconsistent sizes, partial/absent output and
mode/default mutations. They do not implement Windows API semantics. Comparisons
cover registers/flags, complete stack/arena bytes, globals, FS chain, ordered CPU
accesses, exact API arguments/pointer snapshots, helper counts and first faults.
The corpus also covers DF/alignment, extreme allocation observations, standalone
fill, 54 direct lexical filter/recovery entries and 1,000 seeded mixed cases.

Two discoveries constrain reproduction:

- Initial `test-a` was invalid: loading only FS left SS in 16-bit stack mode;
  cases agreed on premature push faults. Its report and five frozen inputs are
  preserved. `crt-locale-candidate-audit-a/earlier-run-invalidation.json` rejects
  that pass claim. Corrected tests configure flat 32-bit CS/DS/ES/SS plus FS and
  require ordinary cases to return normally. Failures b–g remain unaccepted.
- An unwritten temporary can expose prior **code-return bytes** to an API.
  Relocating code and normalizing those bytes would conceal observable data.
  Addressed candidate-b and its linker script preserve every actual original
  instruction address; compared bytes are unchanged by code/data normalization.
  Source compiles instructions rather than loading an original executable image.

`crt-locale-candidate-audit-a/audit.json` passes, pins generation/export/tool/test
chains and freezes five final inputs in `crt-locale-candidate-source-used`.
Its `candidate-ledger.json` does not promote maintained recovery. Scripts and
bulk evidence stay local in ignored Analysis; preserve them for reproduction.

```powershell
py -3.13 CM0102/Analysis/test-crt-locale-candidate.py --output CM0102/Analysis/crt-locale-candidate-test-next
```

Use fresh paths. Local preparation/audit scripts intentionally refuse their
existing output names; inspect and choose new paths before rerunning those stages.

## Remaining boundaries

Mode/default initialization and classification table data remain unrecovered.
Six imported API slots, original-address empty strings/scope tables and the shared
exception dispatcher remain explicit. Synthetic FS and direct lexical recovery
do not establish OS unwind or guard-page behavior. Extreme-size/DF observations
do not imply that actual APIs return those scripted values.

Review before promotion; validate case/classification/morphology composition
separately. Recover the dispatcher and relocate data while preserving exposed
prior-stack bytes. M2 replay remains prerequisite to M4; M6 is unmet.

## Shared exception dependency map

`scripts/map_crt_exception_dependencies.py` now reproduces the dispatcher's
lexical dependency closure in fresh `crt-exception-dependencies-a` evidence.
It checks **433 bytes / 158 instructions** against a hash-locked primary copy:
dispatcher `0x0094BF5C`, unwind wrapper `0x009451DC`, collided-unwind callback
`0x009451FC`, local scope walk `0x0094521E`, current-scope test `0x00945286`,
shared notification entries `0x009452A9` / `0x009452B2`, and import thunk
`0x009532A0`. The report records each range's hash, VA and file offset. These
are identified lexical ranges; decompiled, compiled, validated and integrated
statuses remain false in this static report. No maintained count changes.

Original PE descriptors identify thunk IAT VA `0x00967158` as
`KERNEL32.DLL::RtlUnwind`. Its behavior remains external. The wrapper pushes a
registration pointer, continuation VA `0x009451F4`, and two zero arguments.
The dispatcher tests record flags with mask six, walks twelve-byte scope rows,
and distinguishes zero, negative and positive filter returns. Positive filters
reach global and local unwind before calling the scope handler. Local unwind
registers a temporary FS record, updates the enclosing level before invoking
zero-filter callbacks, then restores the previous FS head on its ordinary exit.
Notification code writes three fields in the original data at `0x00AD6BFC`.

This instruction evidence establishes neither OS callback arguments nor
nonlocal-transfer behavior.

## Compiled shared exception closure

The fresh unaccepted candidate at
\`CM0102/Analysis/crt-exception-native-candidate-c\` translates these seven
identified regions to x86 assembly at their original preferred-image VAs. It
preserves one combined section with explicit \`.org\` offsets, avoiding COFF
padding between adjacent entries. The 433 source bytes compile into a PE copy
whose seven regions decode to the same 158 instruction addresses and operations.
Candidate and test scripts are \`scripts/prepare_crt_exception_candidate.py\` and
\`scripts/test_crt_exception_candidate.py\`; source, toolchain hashes, linker
layout, PE, symbol map, per-case results and report are frozen under
\`CM0102/Analysis/crt-exception-candidate-test-c\`.

The differential harness passes **1,964 cases** with all 158 instructions
executed. It compares bounded CPU state, full synthetic stack/arena bytes,
notification memory, FS-chain memory, ordered memory accesses, callback entry
state, and outcomes/faults between original bytes and compiled source at the
same addresses. It scripts arbitrary callback returns or stops for the imported
unwind call and caller filters/handlers. These are explicit fixture contracts,
not Windows API semantics.

Status for this isolated candidate is identified/decompiled/compiled/validated
true under that finite synthetic contract, integrated false, and not in the
maintained ledger. The original RtlUnwind behavior, OS exception dispatch,
nonlocal transfer, nested faults and guard-page growth remain unknown. Do not
promote it or claim SEH parity from this corpus. Actual OS behavior still needs
separate disposable-process experiments with the native executable.
