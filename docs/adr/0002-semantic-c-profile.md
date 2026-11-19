# ADR 0002: Semantic comparison profile for tier-2 C

Status: accepted (2026-10-19, chosen by the project owner).

## Context

ADR 0001's `abi` profile compares, on every random case, the outcome detail
including the faulting instruction (mapped to an original VA) and EAX, EDX,
EBX, ESP, EBP, ESI and EDI at that point. Random inputs fault in most cases
(598 of 600 for `fn_007e6130`, 444 of 600 for `fn_00784100`), often inside an
original callee. Passing therefore required the compiled C to reproduce the
original register allocation and frame layout at the fault. Only naked
functions with one inline-assembly statement per original instruction could do
that. After two weeks: 106 C functions (4.2% of body bytes). The supporting
transport code changed the harness identity repeatedly, forcing 12 full
revalidations of 7,929 lifts (64–146 minutes each) in four days.

The plain translation of `fn_007e6130` (a scalar deleting destructor, 11 lines)
failed `abi` only on register and stack-layout values.

## Decision

New tier-2 C is validated with the `semantic` profile (`cmre/semantic.py`,
`difftest._normalize_semantic`). It compares what a caller or the game can
observe:

- Outcome kind; for a fault, the access kind, data address and size.
- On return: the declared return value, EBX, ESI, EDI, EBP, ESP, FPU control
  word and stack depth; ST(0) for a `float`/`double` return.
- All non-stack memory, and import calls with their arguments, in order.

It does not compare register values at a fault, the faulting instruction,
scratch registers on return, or stack bytes. A test-stack address stored as a
value is canonicalised, because it depends on frame layout (for example, an SEH
registration written to `fs:[0]`).

The declared return type selects the return comparison: void (neither EAX nor
EDX), 8-bit, 16-bit, 32-bit,
64-bit (EAX and EDX), or floating point. A static scan of every direct call
site guards that declaration. If a caller reads EDX before overwriting it, the
source must declare a 64-bit return. A void declaration is refused if any direct
caller reads EAX before overwriting the bits read. If a caller reads EAX wider than a declared
8- or 16-bit return, the source must widen it. Pushing a register whose low part
was rewritten after the call counts as passing a narrow argument, not as a read.
The scan tracks overwritten bits in both register families: writing DX covers
DX, DL and DH reads; writing DL covers only DL. Reading the whole register still
observes the callee's untouched upper bits, except for that narrow push rule.

Accepted `abi` records keep `abi`: `recover.c_profile` returns the profile
stored in the ledger. `cmre accept --c --profile semantic` re-accepts one under
the new profile after its source is rewritten.

## Consequences

The hybrid address-space exception in [ADR 0005](0005-original-addresses-in-the-hybrid-vm.md)
retains native fault details but permits a proved original-unmapped versus
recovered-protected write at the same raw address. It does not merge genuine
original protection faults or broaden this profile's instruction comparison.
Raw data-address identity precedes the established code-location relation;
the page witness and its use remain visible in reports.

As of 2026-10-20, asymmetric budget exhaustion is an acceptance failure tagged
`budget-asymmetry`, not a silently skipped comparison. It means unresolved
execution, not proof that a slower compiled routine is semantically wrong.
Investigate or increase the budget for both sides with retained evidence. Mutual
exhaustion remains inconclusive under the existing aggregate acceptance limit.
The change has real-runner regression tests for a nonterminating candidate and
faithful candidates at both sufficient and insufficient budgets.
Since 2026-11-19 ([ADR 0012](0012-bounded-recovered-budget-extension.md)) a
C-only exhaustion first re-runs that C case at 20 times the budget; only C
that still has not finished is a `budget-asymmetry` mismatch.

- Plain C passes when it preserves observable behaviour. Original bugs, integer
  widths, signedness, call order to imports and every heap or global write stay
  compared. Near misses fail (`tests/test_cmre_semantic.py`).
- Caller-visible register effects outside the declared return are not checked
  beyond the static caller scan. Indirect callers (vtables, function pointers)
  are not scanned. Arguments the function under test passes through an indirect
  call are compared only at an indirect stop (amendment below).
- The wide-EAX scan remains conservative about copies such as `mov edi, eax`
  and `push eax` when later code uses only the low word (`fn_006cb6c0`). Resolving
  that case requires data-flow analysis beyond this straight-line scan.
- A store reordered before a fault still fails, because non-stack memory at the
  fault is compared.
- Transports in `return_transport`, `jump_transport` and `x87_transport` remain
  for existing `abi` records. New C should not need them.
- Composition (`compose-test`) still uses `abi`; it is experimental and
  unchanged by this decision.

## Amendment: indirect stops (2026-10-19, chosen by the project owner)

Object initialisers often store fields and then call a vtable slot. When a
random case's vtable pointer aliases the object, a store can turn the slot into
an address inside an original routine (`fn_00925c50` case 72: `0x00873401`).
Both runs executed it, but the instructions that followed read registers the
compiler chose, so no faithful C could match. A slot holding a non-code value
fetch-faulted instead, and only the target address was compared. Three accepted
sources had passed with a stdcall slot that omitted `this` in ECX.

Under `semantic`, the harness now stops both runs before an indirect CALL or JMP
of the function under test whose target is not a function entry, an import or
the function's own code. The outcome is `interior` (executable original code) or
`bad-target` (anything else). The comparison covers:

- the target, all non-stack memory and import calls up to that point;
- ECX, when the original listing loads it for the call. That means ECX's last
  write before the call is followed by no push, and the call operand does not
  use ECX;
- the dwords the listing pushes for the call. They are counted back to the
  previous transfer, label, `add esp`/`pop` cleanup or prologue, excluding
  saved-register pushes (`semantic.indirect_calls`).

The count can fall short, never over: it omits pushes made before a nested call
or on another path. A count the listing does not show is not compared. Examples
are stored arguments after `sub esp`, or a prologue `push ecx` that reserves a
local. Stack addresses in ECX or the arguments are canonicalised. Calls to real
entries run as before. `exact` and `abi` runs are unchanged.
`tests/test_cmre_semantic.py` and `tests/test_cmre_emu.py` hold the equivalent
cases and the near misses (target, ECX, argument, memory, outcome).
