# C translation boundary references

Applies to C accepted or maintained under the legacy `abi` profile, whose
register-exact comparison required these adapters. New C under the `semantic`
profile ([ADR 0002](../adr/0002-semantic-c-profile.md)) is plain C and does not
need them. Reached from [C.md](C.md) when the original instructions or chosen adapter use
one of these features. These obligations and retained reproducers supplement the
fixed acceptance corpus; their existing scope and limitations still apply.

## Stack aliases and faults

Review adapter stack writes separately from the fixed corpus. An extra C-helper
push can overwrite an aliased input; helper scratch can also become live when an
original callee descends into that stack space. Retain raw stack-alias and late
fault reproducers alongside the source-identified reports. ABI normalization
discards dead stack bytes, so a fixed-corpus pass alone does not cover these cases.

For the two submitted 2026-11-02 pilot adapters, run the source-specific
supplemental probes with a new output directory:

```powershell
py -3.13 scripts/cmre_adapter_witness.py --output CM0102/Analysis/cmre/diagnostics/adapter-witness-NEW
```

The driver checks current task and execution identities, compares the linked
straight-line instruction streams, and retains raw paired VM states and artifact
hashes. It covers only the two explicitly named submissions and their selected
inputs; it does not approve a task, establish comprehensive memory or path
coverage, or prove native continuation. Review the linked code, all applicable
boundary evidence, and the task separately before local acceptance. Rejection
controls are in `tests/test_cmre_adapter_witness.py`.

Include aliases between a slot and its table-pointer global across recovered
calls. In the descriptor slice, VA0094FD73 can OR that shared dword to -1;
caller0094AE85 then rereads the pointer and faults on its next byte access.
Preserve the reread and late fault, alongside saved-register-slot aliases.
Raw inputs and both stacks are retained under
`CM0102/Analysis/cmre/diagnostics/descriptor-slots-20261015/aliases1/`.

## Compiler-specific adapters

For a compiler-specific bridge, inspect and retain the actual emitted instructions
and compiler identity. GCC does not support mixed C/extended assembly in naked
functions ([GCC 15.2 documentation](https://gcc.gnu.org/onlinedocs/gcc-15.2.0/gcc/x86-Function-Attributes.html)).
Passing evidence applies to the inspected build, not a portable compiler contract.
A helper with an explicit optimization attribute uses that setting under both
outer build modes; report this limitation and re-inspect after compiler changes.

Inspect compiler transformations around labelled adapter loops. In the larger
allocator draft retained under
`CM0102/Analysis/cmre/diagnostics/extended-indirect-contract-20261015/`, GCC copied
two loop headers and emitted duplicate access labels. Function-local
`no-tree-ch` prevented that observed copy. Loop alignment also inserted a NOP;
local `align-loops=1`, `align-jumps=1`, and `align-labels=1` removed added alignment
instructions in the inspected builds. These attributes belong to that source's
compiler contract. Verify every reachable effect and both logical successors,
including reordered cold blocks, rather than assuming an attribute preserves
the original instruction sequence. The first failed build and pre-alignment
executable remain in that evidence directory.

Inspect instructions from the actual compiled entry, including code before the
first labelled access. In the initializer and tail-thunk drafts at preferred-image
VAs 0048c580 and 007e6340/007e6370, unused fixed-register outputs caused GCC to
write caller stack slots. Binding only the state needed by those leaf adapters
removed the spills in the inspected builds. Choose constraints from original
live state, then verify all remaining registers, flags and memory accesses in
emission and raw alias/fault probes. The retained first drafts are under
CM0102/Analysis/cmre/diagnostics/cleanup-indirect-contract-20261015/.

Inspect all reachable blocks, including cold branches placed after the first RET.
In the constructor at preferred-image VA007e61c0, GCC split three indexed C loads
into LEA/MOV pairs, changing registers at a failing read. Narrow original MOV
adapters preserved those fault states. Other branches may invert conditions or
fall through after block reordering; verify both logical successors rather than
requiring identical branch spelling. Retained constructor source, first emission
defect and full control-flow proof are under
CM0102/Analysis/cmre/tasks/c-007e61c0-1d47f92345259992/.

Inspect typed byte stores after original saved-register POPs. At preferred-image
VA004AFFFA (base `0x00400000`, primary SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`),
a fixed-register C store caused GCC15.2 to spill into saved words and
the caller's return slot; an ordinary return then fetched the overwritten word.
The narrow original MOV preserves the late fault state while retaining the
caller's sixteen typed reads. Failed emission, complete instruction comparison,
and raw saved-slot/argument/CALL-word aliases are retained under
`CM0102/Analysis/cmre/diagnostics/exception-reset-20261017/case-fpcw/staged-slot-callers/`.
This evidence applies to the pinned primary hash and inspected build; it does
not establish a general mixed-naked compiler contract.
The paired reader at preferred-image VA004B002F also spilled a typed dword
store into live saved slots. Retain the original MOV stores there; the inspected
reader keeps sixteen typed loads and arithmetic operations. Its first failed
draft and corrected emission/fault evidence are in the same directory under
`getter-004b0010/`.

## CALL return words and VM bindings

Before combining recovered definitions, compare their extern declarations with
the recovered callee declaration when one exists. The accepted formatter at
preferred-image VA00944C9F (primary hash below) has two fixed words and variadic
arguments. Fixed-three externs passed isolated numeric-CALL validation but failed
the actual shared translation unit with that definition. Preserve the original
PUSH order; use the compatible variadic declaration. Retain the compile rejection
and prove declaration-only edits preserve emitted bytes, anchors and transports,
then validate and reaccept changed canonical C. Keep active inventory inputs
frozen until its gate finishes. The twelve confirmed existing-source conflicts,
corrected staged callers and transfer proofs are retained under
`CM0102/Analysis/cmre/diagnostics/slot-delivery-20261018/formatter-existing-frontier/`
and its sibling `formatter-declaration-before/` and `formatter-declaration-after/`.

Check whether original CALL return words are also application data. An unchecked
counter at entryESP-0x21c in the record serializers reads the numeric return VA
on subsequent fields; relocated native calls can change indexes and diagnostics.
Source pointers may also read original continuation bytes as record data. Retain
both reproducers before choosing an execution binding. An owned VM continuation
prototype must preserve those bytes and raw writes, reject unowned original
entries, and report its platform seam explicitly. Production harness support
requires [HARNESS.md](HARNESS.md) regression and inventory revalidation; prototype
probes alone do not accept source or establish native execution.

For a verified five-byte near CALL, `CM_ORIGINAL_CALL(callva, returnva, calleeva)`
in `cmre.h` emits the original numeric return word plus a declared native JMP.
Use eight lowercase hex digits without `0x`. The builder checks original bytes,
ownership and the actual original/compiled target. The VM dispatches only an
observed 32-bit near RET from the active slot; original bytes remain readable.
Existing access/fault labels must retain their instruction locations. Avoid
duplicate `v_<return>` labels; an equivalent explicit marker/byte adapter may
retain existing labels after review. Validation reports and C ledgers record the
VM seam. Native continuation binding and non-RET/SEH unwinds remain boundaries.

For pinned two-byte `CALL [EAX]`, `CALL [EDX]`, `CALL EAX`, `CALL ESI`
or `CALL EBP`, or six-byte
`CALL [absolute32]` (FF15), or the exact three-byte `CALL [EDX+44h]`
(`FF5244`), use
`CM_ORIGINAL_INDIRECT_CALL(callva, returnva)`. The builder checks original
instruction ownership and a five-byte native JMP into that exact original CALL.
The CPU reads the target before writing the return slot, so the target table may
alias that slot. The VM observes the actual target/slot and redirects selected
callee entries by EIP only; original pointers and bytes stay intact. Direct and
indirect calls share one owned near-RET stack. Actual cleanup is observed; never
assume RET4 from the caller's argument PUSH. Other indirect forms, original
interior targets and exception/non-RET unwinds require separate evidence. (Plain
semantic C does not run interior targets: ADR 0002's indirect stop classifies them.) Retain
target/return-slot aliases, target-read and return-write faults, and ownership
rejection controls alongside ordinary corpus checks.

The `FF5244` admission is pinned to the original bytes used at preferred-image
VA0051970C, returning to0051970F, in fn00519690 (base00400000, primary SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`).
Observe the target at `(EDX+44h) mod 2^32` before the CPU writes the return word;
do not restore EDX or infer cleanup from caller pushes. Other displacement
encodings remain rejected. Original-only synthetic inputs exercise callback
00519890 and its actual RET18h; they establish neither runtime object type nor
native callback/SEH behavior. The retained scaffold rejection, isolated prototype,
reviews and regression evidence are under
`CM0102/Analysis/cmre/diagnostics/slot-delivery-20261018/`.

## Flags and returned registers

Track the specific flag consumed across instructions. In the recovered resize
routine at preferred-image VA00949399, CMP index,20h feeds JAE across INC CL and
a memory write: INC changes other flags while preserving that carry. Its
arithmetic SAR/DEC bin calculations use unsigned JBE clamps. Preserve the actual
branch conditions and argument-slot byte writes; signed-looking calculations do
not justify signed clamps. Canonical CFG and raw counter-wrap, link-alias and
late-fault evidence are under
`CM0102/Analysis/cmre/diagnostics/resize-recovery-20261015/`.

For call boundaries, retain actual callee-return register values even when a
declaration suggests preservation. The caller contract at VA00945667 forwards
actual EDI after00949CB3 to00949987; recomputing ESI>>4 would erase an observable
callee effect under malformed or aliased state. A known argument derivation before
the call does not establish the register's value after it.

## Indexed jumps

An indexed JMP performs an ordered memory read as well as selecting a successor.
A C switch over its index alone omits that read. For VA0094F0F0, all sixteen
dispatches have retained original-only protected-read witnesses under
`CM0102/Analysis/cmre/diagnostics/copy-abi-preparation-20261015/`.
Keep actual table addresses, read widths, pre-fault registers and observed flags
as explicit translation obligations. Four witnesses change arithmetic flag bits
between the pre-instruction hook and reported VM fault; the cause and native
exception semantics remain unknown. Do not infer a production flag write or
comparison normalization from those diagnostic controls. Lift table relocation
and an ordinary fixed-corpus pass do not establish these C fault boundaries.

For the pinned seven-byte absolute indexed scale-four JMP form, an explicit
`cmre_jump_<eight lowercase original-site hex digits>` marker may name a
byte-identical native instruction. The builder requires the parent's bounded
table declarations and native target instruction anchors. The CPU still reads
the original dword slot; only a source-PC-qualified observed read can own the
subsequent EIP-only target dispatch. Effective addresses wrap at 32 bits, so
index aliases separated by 2^30 name the same admitted physical slot. Metadata
resets on every run, and normal completion cannot bypass a pending transfer.
CALL/RET arrival overlap and overlapping native transfer instructions are outside
this contract; shared physical table slots are intentional. Single-function C
validation rejects extra jump owners; composition admits only selected owners
and fingerprints their parent declarations independently of marker spelling.
Retain raw read/fetch faults, lifecycle and rejection controls, plus the complete
emitted CFG. The failed original-JMP-entry alternative and passing compiled-site
prototype are under diagnostics/indexed-jump-prototype-20261015/ in Analysis/cmre;
production admission evidence is under indexed-jump-admission-20261015/.
The binding remains a VM boundary, not a native continuation implementation.

A newly reviewed table declaration can expose individual DWORD labels in an
already accepted lift. Ordinary refresh must still reject that source drift.
Use the existing `py -3.13 -m cmre lift-migration apply fn_XXXXXXXX
--hints <reviewed-proposal-path> --expected-old-sha256 <accepted-source-hash>
--expected-new-sha256 <proposed-source-hash>` after active inventory jobs finish.
It rejects C/WIP sources and changed original instructions, boundaries or
validation policy, validates the proposal deterministically, and journals old
and new source, hints and ledger before publication. For a label-only change,
retain the separate proof that every instruction/data line and compiled section
is unchanged. After an interruption, inspect the journal and use `cmre
lift-migration reconcile <journal-directory>`; do not edit generated records.
Validate the affected record and run the parent session gate. Migration provides
no additional source recovery or native execution credit.

## Original stack-probe return slots

For primary SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`,
the original helper at preferred-image VA00945610 (image base 0x00400000) moves its return word
while probing EAX bytes of stack. Its 47 instruction bytes establish a RET slot
of `(entryESP - incomingEAX) mod 2^32`; the actual near RET is VA0094563E.
The return transport records those incoming values only for a direct binding to
that unchanged original helper. It checks the full helper bytes before entry and
dispatch, the exact RET site, zero cleanup, actual word, computed slot and final
ESP. Every probe read, register change and copied-word write executes on the CPU,
including overlapping writes for small EAX counts. The host redirects only EIP.

All other bindings retain the entry-slot rule. A compiled imitation of the helper
does not acquire this migration exception. Changed helper bytes, a wrong slot,
non-RET arrival and an unowned continuation remain rejection cases. Keep the
original helper as an explicit boundary until its compiled execution binding has
its own reviewed contract; accepting its source would not establish that binding.

The original same-slot failure, rejection controls and full caller-stack/fault
evidence are retained under
`CM0102/Analysis/cmre/diagnostics/fe81-stack-probe-20261017/`. The admission is an
owned VM seam; normal acceptance, composition and harness inventory gates remain
required. It establishes no native Windows stack-growth or exception semantics.

## Flags across POP

Keep flag-producing tests before the actual saved-register restore when the
original Jcc follows a POP. A branch over the restored register can consume a
different value. The integer-converter evidence under
`CM0102/Analysis/cmre/diagnostics/flag-converters-20261015/` retains complete
relevant-bit grids and protected late-POP faults, including pre-fault flags,
actual saved words and full stacks. C control flow must preserve that dependency.

## Integer exception probes

With Unicorn 2.1.4, repeated integer divide errors on a reused original Machine
can eventually stop at the DIV with a budget outcome, while fresh original
Machines report `UC_ERR_EXCEPTION` for the same inputs. The original-only
reproducer and first failed supplement are retained under
`CM0102/Analysis/cmre/diagnostics/text-format-conversions-20261017/`:
`division-exception-exploration.json`, `shared-exception-boundary.json`, and
`0094dac0/before-linkage-alias/initial-shared-machine-raw.jsonl.gz`.

The conversion supplements isolate every zero-divisor case with a fresh original
and recovered Machine. They retain all inputs, raw comparisons and actual DIV
errors. State this execution policy with those reports; it does not repair
production Machine reuse or establish native exception behavior. Never discard
the budget case or normalize it to a divide error. A production fix belongs to
[HARNESS.md](HARNESS.md), including its regression and revalidation gates.

The subsequent Machine fix saves the initialized CPU context and restores it
before each case's memory and registers. Repeated original divide errors then
match fresh Machines; normal returns and real budget stops remain distinct.
The historical isolated supplements do not replace current-harness inventory
refresh or establish native exception delivery.

Captured input fixtures can specify an optional unsigned 16-bit `fpcw`.
Omitting it keeps the initialized Machine control word. Supply an evidenced
value as an input on both sides; preserve cases without it. The original-only
two-case control under diagnostics/exception-reset-20261017/failures/006ec010/
shows why inherited x87 state can change coverage and error paths. Its observed
0x007f is a diagnostic input, not evidence of native game initialization.

## Saved x87 instruction pointers

Relocating an x87 instruction changes its saved FIP. For the pinned primary
SHA-256 `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`
(preferred-image VAs, base `0x00400000`), root 00784100's FSTP at 007847F4 left
FIP=007847F4 in the original and FIP=6E5D1811 in its inspected O0 C build.
All other sampled state and full stacks matched that case. The ordinary C corpus
passed while this strict saved-FIP discrepancy remained. Preserve the failure;
do not discard or normalize that register to claim raw equivalence.

Evidence is under
`CM0102/Analysis/cmre/diagnostics/association-root-20261018/`:
`raw/first-failure.json`, `raw/raw.jsonl.gz`, and `final-checkpoint.json`.
An analysis-only prototype executes one pinned original x87 instruction at a time,
redirecting EIP alone and checking its exact successor. Its passing raw and
control reports remain prototype evidence. Production admission, native execution,
SEH and standalone recovery are separate obligations. These reports do not
establish C acceptance.

The production candidate in `cmre/x87_transport.py` uses explicit paired
`cmre_x87_<site>` and `cmre_x87_return_<site>_<original-next>` symbols. Parent-owned
`original_x87` hints pin each site's original opcode bytes and evidence. Supported
forms are currently FILD m32, FSTP m64, FMUL m64, FCOMP m64 and FNSTSW AX only.
Binding checks original and native instruction boundaries, owner, continuation
bytes and CALL/JMP ownership. Runtime redirects EIP only: the original instruction
executes on the CPU. Changed bytes, unowned arrivals, unfinished transfers and
overlapping pending CALL/JMP transfers reject; idle bindings and active call
frames are separately tested. This retains an original-instruction dependency.

For an admitted C source, validation compares actual FP0..7, FPCW, FPSW, FPTAG,
FIP, FDP and MXCSR, including their contribution to deterministic result digests.
No FP register or numeric FP address maps to a native substitute. An FP mismatch
cannot be exempted as code-as-data. Older unmarked sources keep their existing
comparison contract; they do not acquire a full FP equivalence claim. Native
exception delivery, XMM state and standalone execution remain separate.

The candidate composer requires parent declarations and bindings for every x87
instruction in the selected bodies, and excludes extra owners. The prototype's
idle-JMP restriction does not apply to this reviewed production protocol. Do not
claim root-plus-formatter composition from protocol unit tests: retain a fresh
selected composition with actual entry visits. Follow [HARNESS.md](HARNESS.md)
before production acceptance; focused tests, full pytest and the complete frozen
accepted-inventory/C refresh are mandatory.

Fresh production root evidence is under
`CM0102/Analysis/cmre/diagnostics/x87-admission-20261018/`: `emission-proof.json`
reuses unchanged compiled sections and all prior anchors; `root-c-test.json`
records the strict FP C corpus; `raw/raw-proof.json` records 2070 raw pairs.
This remains bounded evidence (403 instructions and 277 memory sites unexercised),
independent of the pending inventory refresh and later source acceptance.
