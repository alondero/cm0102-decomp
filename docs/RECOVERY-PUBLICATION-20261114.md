# Recovery publication review — 14 November 2026

## Scope and evidence

This batch publishes 13 reconstructed C routines from the stopped recovery
farm, alongside the separately committed scheduling fixes. Addresses below
are preferred-image VAs, base `0x00400000`, for CM0102 3.9.68, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.

The source review was performed in the parent session without new subagents.
Existing independent audits were reused where applicable. Each published C
file must match its current acceptance ledger, dependency hashes and retained
passing report at both `-O0` and `-O2`. Local evidence is retained under
`CM0102/Analysis/cmre/publication/tidy-20261114/`; it is not release content.

## Fidelity review

| Routine | Reviewed obligation |
| --- | --- |
| `fn_0051e870` | Signed record indices, row-pointer reloads, ordered field swaps and virtual-call arguments; prior independent audit retained. |
| `fn_00539490` | Original scratch layout, marker/index sign extension, signed byte comparisons, stored pointer slot and adjustment-loop back edge. Corrected during publication review. |
| `fn_005896d0` | Guard order, byte-based `+0x33` accesses, object/SEH adjacency and 12 ordered setter calls. |
| `fn_005a0050` | Stack-probe frame/argument mapping, guarded dispatch tables, variadic arguments and `RET 12`. Native guard-page delivery is not proved. |
| `fn_00669bd0` | SEH layout, record copies, unconditional stores before `00669c6b`, and full helper EAX transport before the later AX write. Corrected during publication review. |
| `fn_00683210` | Callback reread and second call, caller cleanup, signed comparisons, original dispatch table and fallback differences. Non-null callback execution remains an explicit boundary, not a proved subsystem. |
| `fn_006f0630` | Incoming EDX on the null early exit, partial-register results and ordered object updates; prior independent audit retained. |
| `fn_00775cd0` | Scratch widths, diagnostic vararg spill slots, call order and the zero-kind branch from `00775f2c` to `00775f68`. Corrected during publication review. |
| `fn_007953d0` | Scratch offsets, copy extents, SEH state widths, helper return transport and `RET 20`. |
| `fn_007bdee0` | Argument map, overlapping row storage, byte counters and formatting arguments; prior independent audit retained. |
| `fn_007e7ff0` | Post-fix condition slots: `007e8027` feeds the `007e8270` gate; `007e803f` feeds `007e824d`. The historical frame-slot finding is fixed. |
| `fn_008adbb0` | Low-byte result with upper EAX preservation, incoming EDX transport and caller-cleaned arguments; prior independent audit retained. |
| `fn_009233a0` | Three adjacent uninitialized buffers, default-arm destinations, variadic calls and forward string-copy order under DF=0; prior independent audit retained. DF=1 equivalence is not claimed. |

Two focused regression witnesses compare original instructions, the retained
pre-fix C, and the corrected C at both optimization levels:

- `fn_00669bd0`: with the object flag equal to `-1`, the original still stores
  the record byte and word. The previous C skipped them and lost the helper
  result's upper EAX bits. The corrected C matches the observed stores and
  value origin. The witness stops at the next helper entry.
- `fn_00775cd0`: kind zero selects `007701f0`, not `00775f90`. The old C selects
  the latter; the corrected C selects the former. Helper return values in this
  witness are explicit scaffolding shared by both runs.

These witnesses address named gaps in the finite acceptance corpus. They are
not substitutes for the unchanged acceptance gates or proof of helper behavior.

## Publication and remaining boundaries

The 10 source-version scan findings arose because the publication tree still
contained earlier drafts while the local ledger accepted newer source. This
batch replaces those drafts with reviewed, exact accepted bytes; it does not
change the scanner or rewrite historical commits.

The listing heuristics for unchanged `fn_00690d80.c` and `fn_0094ab27.c` have
existing independent path/SHA-specific authored-reconstruction dispositions.
Reuse requires exact hashes. The latter uses owned VM call bridges, not a
standalone native implementation.

Unreviewed candidates, interrupted drafts, captures, ledgers, raw witnesses and
provider logs remain local. In particular, `fn_006d4330` still needs evidence
for its uninitialized-output obligation; `fn_006eecb0` retains its unresolved
power-call boundary. The preexisting `fn_006be710.c` edit is outside this batch.
No supplied game files, assets or private history are publication inputs.

The recovery session added 14 accepted functions and 10,580 inventory body
bytes. This is local recovery progress, not the number of source files newly
published here. Lift-inclusive recovery and live integration did not increase.
Finite differential checks do not establish subsystem parity, native exception
delivery, a standalone game or completion of the modern-port gate.
