# Language five: isolated native candidate and two suffix passes

> Historical regional/candidate evidence. The [full morphology application body](LANG-MORPH-PARENT.md) is now accepted separately (323,545 comparisons per O0/O2 build). Counts, residuals and next steps below describe this earlier frozen stage; its ledger and tested inputs remain unchanged.

Primary input is official 3.9.68 `CM0102/Game/cm0102.exe`, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
The analysis copy `CM0102/Analysis/baseline-c/cm0102.exe` has the same hash.
Addresses are preferred-image VAs, image base `0x00400000`.

Language five comparison is `0x0067C2E5`, body `0x0067C2EE`. A mismatch jumps
to shared composition at `0x00670974`. `morph-five-static-a` pins **14,863 bytes /
4,767 instruction records**, ending before `0x0067FCF4`; region SHA-256 is
`981bc35bb9b0fc7ce37e9842210c64718ce0149fece033604e8cdf5aa53755ec`.
Existing common copy tails are outside that range.

An isolated candidate under `CM0102/Analysis/morph-five-native-candidate-a`
passes **5,126 focused comparisons each at O0/O2**. It changes no maintained
application source or accepted ledger. Seventeen inputs are frozen under
`morph-five-candidate-source-used`; `morph-five-candidate-audit-a/audit.json`
passes source/artifact/tool/static/generation/primary-byte checks. This is finite
candidate evidence. Full combined-corpus acceptance, whole-parent validation,
live integration and a standalone game remain outstanding.

## Instruction-derived behavior

The twelve prefix comparisons retain original order: `upper`, `lower`, `El`,
`{El`, `el`, `la`, `del`, `de la`, `DEL`, `al`, `un`, `Un`. Preserve `{El` as
the actual bytes. Upper converts the signed first local-word byte through
`0x009471CD` and stores AL. Lower calls `0x009465FC` for signed bytes, with no
acronym exceptions; its next-byte read occurs even if a callback returns zero.
These paths continue to suffix capture at `0x0067C907` after callbacks.

Ordinary prefix tables admit signed low property1 values 0..5 using unsigned
`JA 5`; other bytes copy a space. `del` and `de la` use the special dispatch at
`0x0067C7B9`: seven DWORD pointers at `0x00681358`, selected through 101 bytes at
`0x00681374`. Values 0 and 5 choose the same first target, 1..4 choose their
individual targets, 6..99 choose space, and 100 chooses the empty literal.
Negative/out-of-range signed values choose space. **Property 100 measures and
group-copies the empty literal**, unlike nine's direct-zero store. Unknown
nonempty prefixes erase caller bytes through the original empty-literal copy.

The first suffix comparison is `s` at `0x0067C90E`. After the comparison has
computed its result, **`0x0067C942` captures property2's low byte in DL once**.
Subsequent comparisons preserve DL; their signed table indexes use that cached
byte. `s` selects the empty local suffix. Suffix tables use unsigned 0..5 bounds
on signed property2 and a space default. No original table-helper call occurs.

There are two suffix passes. First-pass matches copy through EBX to local
suffix `P+0x18` at `0x0067F490`, then fall through to the second pass beginning
`0x0067F4AD`. First-pass misses clear DWORD `P+0x14` at `0x0067F458` before
entering that second pass. Its matches copy through EDX and reach common
composition. Final miss reaches the conditional guard at
`0x0067FD31..0x0067FD3E`: nonempty caller suffix is erased only when `P+0x14`
is zero. Preserve first-pass copies and their trailing bytes even when a later
copy overwrites the displayed suffix.

Repeated comparison keys include `acaba`, `acepta`, `premiado` and `consigue`.
The native control flow retains their original positions and destinations;
they are not consolidated into one map. `no sel le` is also an original byte
string and is retained. The summary records **145 comparison occurrences**:
twelve prefixes and 133 suffix comparisons, including repeats. Its inherited
`captures_property2` marker recognizes BL, so that field is false for five;
the generator and candidate audit separately verify the actual DL capture.

## Native lowering and evidence

`summarize-morph-five.py` and `inventory-morph-five.py` produce the pinned
summary/dependency reports. The dependency inventory identifies 137 EAX-indexed
six-pointer table sites and 299 distinct targets; the selector report adds its
two separately watched regions. The candidate adds **139 table regions**, for
364 watched regions alongside maintained nine's contract.

`generate-morph-five.py` lowers the verified original flow to **455 native C
basic blocks**: 1,623 unmerged blocks, 1,168 joined single-predecessor continuations.
`morph-five-generated-b` pins all 4,765 body records; the two uncovered records
are the language comparison and conditional jump reproduced in candidate assembly.
Native C expressions, literal-copy primitives and compiled branches execute;
the worker contains no original-instruction interpreter. Original code-pointer
tables remain immutable data, mapped to C labels with unsupported-target traps.
The generator freezes itself and its semantic input hashes alongside the output.

The retained `morph-five-generated-a` diagnostic failed because the initial
adaptation decoded the right bytes with the earlier language-three address base.
The corrected generator uses `0x0067C2E5` consistently. This was a generation
assertion before candidate execution, not a differential mismatch.

`prepare-morph-five-candidate.py` verifies the generation/artifact chain before
creating private C/assembly drivers. `test-morph-five-candidate.py` compiles
freestanding 32-bit x86 with GCC 15.2/binutils 2.45 and warnings as errors.
Focused cases cover each comparison occurrence with eleven selected low
property values and two high-bit patterns, every first byte, repeated-key
combinations, aliases, callback slot/property mutations, return bits, dynamic
language/output replacement, word lengths through 2,000 and 1,000 seeded inputs.
The recovered side permits only the original probe and explicit CRT entries.

The watch includes full EAX/ABI, ordered CRT boundary events, original arguments,
frame `P+0x10..P+0x2813`, shared output, language, tables and the full 16KB caller
arena. The relocated probe return-address DWORD is excluded. General faults,
overflow, all aliases, concurrency, real CRT behavior and Windows guard-page/SEH
behavior are outside this diagnostic contract. The old driver depends on its
frozen maintained-nine inputs; do not rerun it after changing those inputs.

## Next acceptance gate

Adopt five into maintained source only with broad low-byte, independent-property,
alias and repeated-pass coverage. Preserve and verify all prior accepted stages.
The candidate's regional selection totals **64,465 bytes**, comprising the
maintained 49,588, five's 14,863 and the 14-byte shared conditional guard. Audit
nonoverlap, instruction completeness and default-language composition before
considering whole-parent recovery. This count alone establishes no full-function
acceptance. The original stack probe, fixed-address strings/tables and CRT
fixtures remain explicit dependencies. Relocation/data recovery is still needed
for a standalone localization build; M2/M4/M6 remain open.

Independent Standards and Spec reviews verified both focused report/artifact
chains and seventeen current/frozen inputs without a remaining material finding.
Spec also passed 180 additional original/candidate comparisons and checked DL
capture, the special selector, both suffix passes and conditional caller erasure.
