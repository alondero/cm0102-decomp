# ADR 0006: Compiler-inserted runtime helpers in plain C

Status: accepted (2026-11-03, chosen by the project owner). Existing `__ftol`
holds are retried under this decision; each still needs the sensitivity
evidence in condition 4 before acceptance.

Addresses are preferred-image VAs (base `0x00400000`) in the primary binary,
SHA-256 `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.

## Context

MSVC lowers some source constructs to calls into statically linked runtime
helpers. The inventory names `0x00945640` `__ftol` (39 bytes; Ghidra library
match "Visual Studio"). MSVC emits a call to it for every floating-point to
integer cast: the value is in ST(0), the helper truncates it through a 64-bit
`FISTP` under a temporarily chopping control word, restores the control word
and returns the 64-bit result in EDX:EAX.

AGENTS.md keeps callees as explicit boundaries until the standalone gate.
Agents have applied that rule to `__ftol`, and plain C cannot place a value in
ST(0) for an ordinary call, so these functions are held. A static census on
2026-11-03 (scratch scripts, `cmre work list --json`, `cmre shards --ids`) found:

- 127 of the 144 x87-blocked work items (305 of 314 KB) call `0x00945640`;
  27 differently named blocker groups describe this one boundary.
- 123 of the 1,152 unlifted functions (548 KB) call it too.
- Only one of 9,241 inventory functions (`fn_00946012`, `FNSAVE`/`FRSTOR` at
  `0x00946122`) can observe the saved x87 instruction and data pointers.

The semantic gate ([ADR 0002](0002-semantic-c-profile.md)) does not compare the
set of original callees; `original_callees` is recorded in the ledger only. It
compares return value, preserved registers, FPU control word and depth, all
non-stack memory and import calls.

A diagnostic spike wrote plain C for `fn_006fa140`
(155 bytes, held as `live-x87-callee-input`) with the call written as
`(s16)(u32)(long long)t`, where `t` is a `long double` that follows the
original `FILD`, `FMUL m64`, `FADD m64` sequence. It passed the unchanged gate
at O0 and O2 (coverage 0.926, no inconclusive cases). Four deliberately wrong
controls also passed: omitting the `FADD`, using `float` precision, changing
the final `<` to `<=`, and changing 360 to 359. The random corpus executed
those instructions without observing their values. Coverage counts executed
instructions; it does not show that a computed value reached an observed
output.

## Decision

A call to a registered compiler-inserted helper may be written as the source
construct MSVC lowers to it, when all of the following hold:

1. The helper is registered with its VA, body hash, the construct it
   represents and that construct's exact semantics. The first entry is
   `__ftol`: `(long long)x` of the live x87 value, used through its EDX:EAX or
   EAX result. `(int)x` is not equivalent: it saturates where `__ftol`
   truncates a 64-bit result.
2. Intermediate x87 values are `long double` in C, so a compiler spill cannot
   round them; constants are read from their original globals.
3. The source names each original call site in a comment.
4. Acceptance requires evidence that the case corpus observes each helper
   site's value: a mutation of that computation must fail the gate. Where the
   random corpus is insensitive, add evidenced fixtures in `c_hints`; never
   weaken the comparison.
5. The helper remains a separate function to recover. This decision makes no
   standalone or replacement claim.

## Consequences

- Up to 305 KB of held x87 work and 548 KB of unlifted work stop depending on
  an x87 call transport. The unlifted part still needs coverage.
- The registry is the only place this exception can grow. Other candidates
  (`__allmul`, `__alldiv`, `_chkstk`, `__EH_prolog`) need their own entries
  and evidence.
- Condition 4 is a new acceptance obligation. The weakness is not specific to
  x87: one flipped comparison survived the gate in 12 of 52 mutants of accepted
  C (2026-11-03 sample, [NEXT-PHASE-PLAN.md](../agent/archive/NEXT-PHASE-PLAN.md)).

## Mechanized condition 4 (2026-11-24)

Until this date nothing checked condition 4. fn_005ce2d0 passed the gate with
9 of its 13 sites unobserved: five had no non-budget case reaching them, and four
were reached but a mutant survived. 215 accepted C routines (422 KB) call
`0x00945640` in the original.

The C gate report now records the original's call and tail-jump sites of
`0x00945640` (`ftol_sites`). For such a routine, the ADR 0010 operand proof
(`cmre/operand_evidence.py`, receipt schema `cmre-farm-x87-operands/2`)
also covers each `__ftol` site:

- A `-fsyntax-only` probe under the gate's C flags classifies every explicit
  signed 64-bit cast in code by its operand type. `-Wfloat-conversion` rejects
  implicit float-to-integer conversions; floating narrowing (`long double` to
  `double`) stays allowed.
- Under `-mno-sse3` GCC lowers every float-to-integer conversion to an x87
  integer store (`fist`/`fistp`/`fisttp`), whatever the spelling (`(int)t`, a
  typedef, enum, macro, `typeof`, compound literal or inline assembly). With
  every floating `(long long)` cast replaced by an opaque marker call, the
  source's -O0 build under the gate's flags must contain no such store. Every
  conversion in the source is then a mutated cast; padding and folding cannot
  hide a differently spelled site.
- A `(long long)` cast of a floating constant is folded at compile time (by
  MSVC too) and fails closed rather than standing in for a site.
- Each cast of a floating operand gets one mutant with its operand `+ 1.0L`.
  The mutant must show a non-budget differential witness at both O0 and O2,
  under the ADR 0010 condition 3 rule.
- Fewer floating casts than original sites fails closed. Each site must be its
  own cast: a shared macro or helper, a cast inside a preprocessor directive,
  or merged paths cannot show per-site observation. A site that still calls
  the original helper as an explicit boundary call (`cm_fn_00945640()`; a
  declaration does not count) needs no mutant.
- Sites are matched by count, not by address. A source that omits a site's
  conversion entirely, while duplicating another cast, is a translation the
  semantic gate and coverage must catch; this proof only shows that every
  conversion the source does contain is observed.

The proof runs before farm and direct acceptance, as the CIpow proof does.
Accepted routines are audited separately (`scripts/cmre_ftol_audit.py`).
