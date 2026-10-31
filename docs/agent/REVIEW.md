# Review recovery evidence and claims

Use for a recovered routine, subsystem packet, or milestone assertion. A review
checks the claim against evidence; it does not accept code or change generated
records. A standalone review follows [PLAYBOOK.md](PLAYBOOK.md) preflight and
closure; a delegated reviewer inherits the parent's baseline.

1. Pin the review scope: exact function IDs/files or a concrete milestone claim,
   intended behavior, and the supplied evidence. For a dirty worktree, compare
   with a captured pre-task copy/diff so unrelated edits do not become findings
   against this task. Completion: a named claim and bounded input set.
2. Verify target identity, address conventions, tool/profile provenance, and
   original byte/body boundaries. Inspect instructions and relevant callers;
   pseudocode alone does not establish the contract. Completion: every behavior
   claim in scope is observed, inferred, or explicitly unknown.
3. Apply both axes below. Run the smallest relevant check needed to resolve a
   finding; use existing receipts for already completed checks of unchanged
   inputs. Completion: each finding cites its requirement, evidence and impact.
   Reuse a passing report only when its source, hints, build/header/toolchain and
   harness identities match the claimed check. State the uncovered obligation
   before adding another corpus or review pass. Examples: stack aliases excluded
   by ABI normalization, a compiler-specific adapter's emitted effects, an untested
   indirect target, or an observed late fault. Required acceptance, composition
   and runtime gates still run; supplemental checks answer the named gap and stop
   when it is resolved. A new reviewer can audit existing evidence without
   regenerating the same experiments.
4. Return the verdict for the **claimed gate**, remaining unknowns, and the next
   check that would resolve each blocker. Preserve a failed baseline as failed.
   Completion: no required claim is silently omitted or promoted beyond evidence.

| Axis | Checks |
| --- | --- |
| Fidelity | Original behavior only; complete control flow; caller/callee ABI and cleanup; integer widths/signedness/overflow; aliasing and memory effects; uninitialized storage; x87 rounding/intermediates; RNG calls/state/order; indirect calls, jump tables and shared exits; observed bugs retained |
| Evidence and scope | Pinned bytes and convention; original compiler facts versus Ghidra assumptions; genuine `cmre accept` record and matching source/report identity; finite-corpus limits and API stubs named; original-code/data dependencies listed; no unsupported runtime or milestone claim; owned files and preservation contract respected |

For x86, check stack cleanup at both call and return, `this`/fastcall registers,
callee-saved registers, stack probing, float-to-integer helpers and SEH lifetime
updates where relevant. A compiler idiom is a hypothesis until its instructions
and uses establish the effect. Matching output or a tool's hypothesis label is
not a substitute for the project's behavioral gate.

Report findings as `blocking|nonblocking — file/function — requirement — evidence
— impact — smallest next check`. Report the two axes separately. A clean review
means no findings against this bounded claim, not universal engine parity.
Only `cmre accept` promotes routine recovery; M2/M4/M5/M6 need their own runtime
and dependency evidence. Instructions-only changes need route/link/schema checks
and the parent session gate, not blanket recovery revalidation.
