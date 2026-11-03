# Translate functions to C

Input: function IDs from the parent. Otherwise inspect `py -3.13 -m cmre family -n 5`
for a bounded family in the current subsystem, then use
`py -3.13 -m cmre next --c -n 20 --module NAME` for remaining dependencies.
Without an established subsystem, select a family with an accepted representative
or use the ordinary queue. Existing unaccepted sources and recorded holds are
excluded; `cmre work list` exposes them and `next --c --include-held` is for triage.
When other agents work concurrently, select with `next --c --claim NAME`
([PARALLEL.md](PARALLEL.md)) so no two agents translate the same function.
Bytes are the progress measure: prefer `--min-bytes 129` once small leaves are
done, or `--module NAME` to stay inside one source file. The queue excludes
compiler-generated entries that image metadata proves: C++ unwind funclets,
SEH `__finally` blocks and import thunks. Funclets are regenerated with their
parent's C++/SEH; thunks by the linker. `cmre boundaries` names each parent
([INVENTORY-RECONCILIATION.md](../INVENTORY-RECONCILIATION.md)).

The queue's `ready` label prioritizes routines whose callees already have C.
`waits:N` counts callees without accepted C; it does not prohibit translation.
When fresh ready entries run out, take waiting entries and retain their original
callees as explicit boundaries. C acceptance does not establish composed execution.

## Loop (one function at a time, then the next)

1. `py -3.13 -m cmre brief fn_X`. One block: return/argument evidence, callers,
   callee prototypes, referenced globals with types, the converted Ghidra draft
   path, and the instruction listing. Do not write probe scripts for this.
   For a family, reuse the reviewed representative and run
   `cmre family fn_REP --peer fn_X` to inspect all operand differences. Shape
   similarity is a hypothesis: validate each member's ABI, constants, widths,
   branches, memory order and call contracts. Each member needs its own test
   and acceptance. Use `brief --no-asm` when the listing is already in context.
   Across contexts, use `cmre brief fn_X --packet` and read its returned artifact
   as needed. It retains the full listing, every indexed direct caller's bounded
   continuation window, declaration provenance and an input-hash manifest.
   `cmre brief fn_X --packet --peer fn_REP` shares the representative packet and
   includes every operand delta and the member's own jump tables and embedded
   data for a matching instruction shape. Review the
   representative once, then each member's differences and independent contracts.
   Packets expire when their dependencies change; caller windows stop at control
   transfers and are not transitive return-width proofs. Indirect callers remain
   an explicit review obligation. Packet reuse never establishes acceptance.
2. Write `recovery/c/fn_X.c`. Start from the draft
   (`CM0102/Analysis/cmre/c-drafts/fn_X.c`), check every statement against the
   listing, and fix what it got wrong. Its `cmre draft repairs` comment lists
   inserted type casts and removed unused declarations; a `receiver missing`
   note gives the listing's ECX setup for each call that dropped `this`. Rules:
   - `#include "cmre.h"`; entry `cm_fn_xxxxxxxx` with the original convention
     (`CM_THISCALL`, `CM_STDCALL`, `CM_FASTCALL`, cdecl). `ret N` pops N stack
     bytes; ECX read before written means `this` (thiscall) or fastcall.
   - Callees stay explicit: `extern RET CONV cm_fn_<va>(...);` binds to original code.
   - Globals: `CM_GLOBAL(type, 0xVA)`. Keep widths, signedness, evaluation order
     and the original's bugs. Unknown meaning stays unnamed; never invent logic.
     Use unsigned address arithmetic for wrapping 32-bit addresses; the build
     does not enable signed-overflow wrapping with `-fwrapv`.
   - Keep memory accesses and calls in listing order. Use volatile accesses when
     necessary to preserve widths and fault order. Capture vtable pointers and
     saved SEH predecessors where the listing reads them, even if used later;
     distinguish SEH state writes from other stack locals. Passing tests do not
     justify moving a read or call across stores.
     For volatile pointer DWORD accesses, use `CM_GLOBAL(volatile u32, VA)` and
     cast the value as needed; `volatile u32 *` qualifies the pointee, not the
     pointer object stored at the global address.
     For object pointer fields that DWORD stores can overlap, load a volatile
     `u32` and cast the value. Retain every original reload: a nonvolatile
     pointer-typed field can be cached under C aliasing rules (the overlapping
     record regression for `fn_007c6bf0` demonstrates this at `-O2`).
     When the original dispatch uses byte-map or DWORD target-table reads,
     retain them with the same guards and access order. A C switch alone does
     not represent those memory accesses.
     Check each guard's inclusive bound, index adjustment and table base at its
     own instruction site; adjacent switch families may use different tables.
     For repeated initializer rows, verify every helper argument and field-store
     constant against the original. Passing fixtures can miss incorrect rows.
   - An indirect slot called after `mov ecx, object` is `CM_THISCALL`, with the
     object first. Reconstruct all live pushes in reverse order, including pushes
     made earlier in the routine; callee stack cleanup does not imply stdcall.
     A callee can forward incoming ECX to another original callee without reading
     ECX in its own instructions. Check those calls before removing the receiver;
     preserve the forwarded value as an explicit parameter at the boundary.
   - Declare the real return width (`u8`, `u16`, `u32`, 64-bit, `float`/`double`):
     only that width is compared. `cmre test` refuses a declaration that callers
     contradict (EDX read → 64-bit; wider EAX read → widen). That caller check
     misses a wrapper that returns the callee's EAX unchanged and a caller that
     does `mov ax, ..; push eax`: read each direct caller before choosing `void`.
     With no direct callers, keep the explicit EAX result unless data references
     (vtables, CRT function tables) show EAX is dead after every indirect call.
   - Draft markers `NEEDS-REVIEW` (`extraout_*`, `unaff_*`, `in_*`, pieces like
     `x._1_3_`) must be resolved from the listing before testing.
   - A `call 0x00945640` (`__ftol`) is MSVC's lowering of a floating-point to
     integer cast ([ADR 0006](../adr/0006-compiler-inserted-helpers-in-plain-c.md)).
     Write it as `(long long)t`, where `t` is a `long double` that follows the
     listing's x87 sequence on the original constants (`CM_GLOBAL(double, VA)`);
     narrow the 64-bit result as the listing uses EAX/EDX, never `(int)t`. Name the
     original call VA in a comment. A pass is not enough: break each converted
     value once in a scratch copy (for example drop one FADD) and run
     `cmre test fn_X --c --source COPY`. If that copy also passes, add evidenced
     `c_hints` fixtures that reach the value before accepting.
   - MSVC pushes arguments right to left and may run a nested call between pushes.
     Pushes before `call A` that A's `(void)` signature does not take belong to the
     next call: `push x; push y; call A; push eax; push fmt; call B` is `B(fmt, A(), y, x)`.
     A single `add esp` after several calls is the sign.
     Keep an evidenced callee signature fixed during mismatch diagnosis; revise
     it only when new original-instruction evidence contradicts the earlier map.
   - For a local `lea [esp + displacement]`, name the base immediately after
     `sub esp, N` as `L`: its address is `L + displacement - live saved/pushed
     bytes`. Recalculate after each push, pop and callee cleanup. For example,
     three saved registers plus two pushed arguments make `lea [esp+0x14]` the
     same `L+0` as `lea [esp+0x0c]` before those argument pushes.
     Keep buffers with related original addresses in one scratch allocation
     with explicit offsets. Separate C arrays do not preserve their adjacency.
     Leave scratch bytes uninitialized unless the listing initializes them.
     Do not shift these offsets to match compiled stack addresses. A compiler's
     frame layout, or a pass at only one optimization level, is not source evidence.
   - Reconstruct partial-register arithmetic through the declared return width:
     `setcc al; dec eax; and al, mask; add eax, K` can return a nonzero low byte
     on both branches. A write to a stack argument slot reassigns that local;
     distinguish it from a store through the pointer the slot originally held.
     Trace each backward branch to its exact target: register copies before that
     target stay cached even when their source register changes inside the loop.
   - Plain C. Inline assembly, naked functions and `CM_ORIGINAL_CALL` belong only
     to legacy `abi` records ([C-BOUNDARIES](C-BOUNDARIES.md)).
   - `fs:[off]` is `CM_FS(type, off)`. An SEH frame is one `volatile`
     `{ prev, handler, state }` struct, linked and unlinked in listing order
     ([ADR 0003](../adr/0003-fs-segment-access-in-plain-c.md)).
     A byte write to the original state updates the low byte of `frame.state`.
     Writing the same numeric offset in a separate scratch array does not
     update the linked frame; do not enlarge scratch space to imitate it.
3. `py -3.13 -m cmre test fn_X --c`. On a mismatch,
   `py -3.13 -m cmre explain fn_X --c --case N` shows that case side by side.
   Fix the first mismatch; stop after five failed iterations. Successful test
   runs do not consume that failure allowance.
   Count every failed invocation, including build failures. An ABI correction
   or parent-directed fix does not reset or waive the cap. If a command returns
   a running session ID, poll it to completion; delayed output does not justify
   starting a duplicate test.
   Canonical `test --c` records the report and cumulative invocation/failure counts
   in `cmre work`. A pass becomes `needs-review`; it is not acceptance. Stop early
   on a known common blocker: `cmre work set fn_X --state harness-blocker
   --blocker GROUP --reason "first discrepancy" --retry-when "specific fix"`.
   Use `evidence-needed` for unknown ABI/layout and `translation-defect` for a
   local correction. `work retry fn_X --changed --reason "resolved obligation"`
   requeues changed inputs explicitly; a change alone does not resolve a blocker.
4. `py -3.13 -m cmre accept fn_X --c`, then go back to step 1 for the next ID.
   Accept needs no pytest, `check` or revalidation; those run per batch and per
   harness change ([validation budget](PLAYBOOK.md#validation-budget)).
   A delegated worker stops at a passing test and returns the ID; the parent
   accepts serially (accept holds an exclusive lease and refuses a second writer).
   After review changes, wait for the worker's final pass and finished edits
   before accepting. Any later source edit, including names or comments,
   requires acceptance again.
   `cmre_session.py check` holds the same writer lease throughout pytest and
   verification. Queue pending acceptances until that check exits; do not
   remove its lock or retry acceptance while its receipt is still running.

A draft can be tried without touching `recovery/c/`:
`py -3.13 -m cmre test fn_X --c --source CM0102/Analysis/cmre/c-drafts/fn_X.c`.

## What the gate compares (ADR 0002, `semantic`)

Return value at its declared width, EBX/ESI/EDI/EBP/ESP, FPU control word and
depth, all non-stack memory, import calls with arguments, and a fault's data
address. Register allocation, stack layout and the faulting instruction are not
compared. An indirect call whose target is not a function entry stops both runs
(`interior`/`bad-target`); the target, memory, and the ECX and pushed arguments
the listing sets up are compared there. Coverage must reach 0.6; low coverage is fixed with
`recovery/hints/fn_X.json` input hints, never by weakening the comparison.
Fixture `arena_template` selects a deterministic random arena, including template
0. Explicitly set the pointers and record slots needed for the intended path.
Every fixture requires `regs`, `stack`, `arena_template`, `arena_writes`, `globals`
(use `{}` when empty), and `import_seed`. Argument kinds are `any`, `ptr`, `small`
or `struct`; a rejected fixture or kind still counts as a failed invocation.
Existing `abi` records keep `abi`; to replace one with plain C, rewrite it and run
`cmre accept fn_X --c --profile semantic`.
New C uses input policy `v2` ([ADR 0004](../adr/0004-versioned-c-input-policy.md)):
DF is clear at entry and only globals the function itself reads are randomized.
Accepted records keep their recorded policy. Error-path (`0x00ae2000`) and
string-instruction mismatches are therefore no longer known harness blockers.
Fixture replay follows the recorded input policy too: v2 clears DF after fixture
selection, while v1 replays the captured flags unchanged. Saved fixture bytes stay
intact; policy normalization changes neither the other inputs nor the RNG stream.

Hints at the top level apply to both tiers. Put C-only input shapes, fixtures and
their provenance in `"c_hints": {...}` within the same hint file. These fields
override shared inputs for C tests, explanations, acceptance and composition;
they do not participate in lifted case generation or its RNG stream. Overrides
replace each named field as a whole, including `fixtures` and `globals`. They
cannot change coverage thresholds, case counts, budgets, seeds or lift bindings.
Remove `auto` when claiming the file. After changing shared hints, test both the
lift and any accepted C tier; a C pass alone does not renew lifted evidence.

Asymmetric instruction-budget exhaustion blocks acceptance, including when the
original returns and C exhausts its budget. Investigate the exact case or raise
both execution budgets through evidenced hints; do not treat an inconclusive
case as a match. Both sides exhausting the budget remains inconclusive and the
existing aggregate limit still applies.

## Stop conditions

After five failed iterations, leave `/* WIP: <field> mismatch, case N */` at the
top of the file, report `fn_X wip: <detail>`, and take the next ID. A worker
returns `fn_X c-accepted|wip: <detail>`; the parent owns the session gate.
