# ADR 0012: Bounded budget extension for recovered C

Status: accepted (2026-11-19) under
[ADR 0011](0011-engineering-decision-rights.md). It amends the 2026-10-20
budget rule in [ADR 0002](0002-semantic-c-profile.md).

## Context

The gate runs the original function and its C translation in the emulator on
the same 600 cases. Each run stops after `budget` instructions, 200,000 by
default. Callees run the original code on both sides.

The budget exists so that code that never finishes, such as a message loop or
a broken translation, cannot hang a test. It is not meant to judge speed.

Both sides have had the same cap. The function's own body is optimised MSVC
code in the original, but in the C build it runs at `-O0` and `-O2`, and
`-O0` takes several times more instructions. For a long, loop-heavy body, the
C can run out of budget on a case where the original finished. ADR 0002
records that as `budget-asymmetry`, which blocks acceptance and can only be
cleared with a hand-written `budget` hint.

On 19 Nov, 20 farm-parked functions (23.7 KB) failed first on this. At
`--budget 800000`, 11 of them pass on every case at both optimisation levels.
For an accepted lift, the hint that would raise the budget is shared with the
lifted tier, and `cmre_hint_apply.py` refuses to change shared hints. C-only
hints may not change budgets. So the only remaining route was a person.

## Decision

The original side's budget is unchanged. If the recovered C run exhausts its
budget on a case where the original returned, faulted or stopped for any
reason other than the budget, that C case runs once more. The re-run has
`RECOVERED_BUDGET_FACTOR` (20) times the budget, and its result is compared as
usual.

- If the C still has not finished after the extension, the case remains a
  `budget-asymmetry` mismatch. The mismatch records the extended budget, so a
  real hang still blocks acceptance.
- If both sides exhaust the budget, the case remains inconclusive. Nothing is
  extended.
- The extension applies to the `abi` and `semantic` C profiles in
  `difftest.run` and `difftest.explain`. The lifted (`exact`) tier and
  composition runs are unchanged.
- A report includes `recovered_budget_factor` and `recovered_budget_extended`
  only when at least one case was extended.

- If an extended run still has not finished, later cases of the same test are
  not extended. The verdict is already a failure, so extending again would
  only make a hanging candidate slower to fail. With this rule, a
  never-returning candidate at the default 600 cases and 200k budget fails in
  about 3 seconds; extending every case cost about 2 hours.
- Composition runs (`composition.py`, `composition_diagnostic.py`) still
  count a C-only run-out as inconclusive and never fail on it. They may show
  inconclusive cases that the single-function gate now compares.
- `native_step_guard` in a report describes the base budget, not an extended
  run.

No per-instruction counting is added, so ordinary cases cost nothing extra.
An extended case costs at most 20 times the budget, and only on the C side.

Independent review (2026-11-19, `cm-evidence-reviewer`) approved the change
with that one required fix. It confirmed that `Machine.run` resets all
per-case state, so a re-run is equivalent to a fresh run. It added the
regression tests for memory and imports, and for mutual exhaustion.

## Which accepted evidence can change

The extension runs only on a case where the C exhausted the budget. Every such
case is counted as `inconclusive`, so a C tier that recorded zero inconclusive
cases at both optimisation levels executes and compares exactly as before.
Its report gains no new keys. Its `result_digest` is also unchanged, because
it is computed from the original side.

Accepted C tiers with inconclusive cases (276 of 3,994 on 19 Nov) can change.
Under the current rule their inconclusive cases were mutual exhaustion. But a
tier accepted before ADR 0002's 2026-10-20 asymmetry rule may hide cases where
only the C ran out. The extension now compares those cases.

The release therefore re-gates exactly those 276 tiers with the new harness
(`scripts/release_0012_check.py`). Any tier whose verdict changes is named in
the release record and resolved through `cmre` before merge, as ADR 0011
requires. Lifted tiers use the `exact` profile and are unaffected. No full
inventory sweep is needed.

Release check (2026-11-19): all 276 tiers pass at -O0 and -O2 under the new
harness, and none needed an extension. Their inconclusive cases were all
mutual exhaustion, so no accepted verdict changes. The local evidence is
`CM0102/Analysis/cmre/release-0012-check.jsonl` and the script beside it.

## Consequences

- Budget-bound functions become ordinary farm work: `retry` returns them, and
  passing candidates are accepted with no hint and no person.
- Existing `budget` hints remain valid and still raise both sides. New hints
  for C speed alone are unnecessary.
- An extended case can take up to 20 times longer on the C side. That cost is
  bounded, and it applies only to cases that would otherwise block acceptance.
