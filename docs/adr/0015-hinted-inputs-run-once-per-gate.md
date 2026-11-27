# ADR 0015: Every hinted input runs once per gate

Status: accepted (2026-11-27) under
[ADR 0011](0011-engineering-decision-rights.md), as a harness release under
[ADR 0007](0007-harness-releases.md). Amends the case derivation that
[ADR 0008](0008-opt-in-input-capacity.md), [ADR 0009](0009-captured-entry-inputs.md)
and [ADR 0013](0013-input-lane-authored-entry-inputs.md) rely on.

## Context

`difftest.make_case` replaced each of the 600 random cases with a hinted input
(fixture or capture) with probability 1/2, choosing which one at random. Roughly
300 draws among N inputs leave about N·e^(-300/N) of them never run: with 465
fixtures, 281 never ran. A fixture that never runs can neither raise coverage
nor expose a mismatch. Input supply therefore stopped paying off near the input
probe's 192-fixture cap, and `fn_004b4850` (19,707 B) stayed below the 0.6
coverage threshold although its best spec reaches 0.79 of its instructions.

## Decision

The gate runs every hinted input once, deterministically, before the random cases.

- Order: the effective tier's `fixtures`, then its `captures`, in hint order.
  Hinted input *i* is case number `cases + i`.
- The random cases follow unchanged. `make_case` and its `Random(seed)` stream
  are unchanged, including the existing 1/2 fixture draw. Random case *k* keeps
  number *k* and the same input it had before this release. Hinted inputs draw
  nothing from the case RNG and take no part in global-input discovery.
- Reports keep `cases` (the random case count) and add `input_cases`. `outcomes`
  sums to `cases + input_cases`.
- Every limit keeps `opts.cases` as its denominator: inconclusive below half,
  code-as-data at most 2%, stack-layout waivers at most 2%. Hinted-input cases
  add to the counts only.
- Each stream keeps its own five-entry mismatch report allowance (hinted, then
  random). The random cases therefore report exactly the entries they reported
  before, so a witness a mutation proof read is still in the report.
- `difftest.explain` accepts case numbers `0 .. cases + input_cases - 1` and
  refuses others. It replays a hinted input directly, building no other case,
  and a random case exactly as before (`gate_cases(..., only=N)`).
  `scripts/cmre_page_writes.py` follows the same replay order.
- A capture missing from the local store now fails every gate of its function,
  not only gates whose random draw picked it.
- `difftest.gate_cases` is the one generator. `run`, `explain`, the return
  screen (`cmre/screen.py`) and the input probe's baseline
  (`scripts/cmre_input_probe.py`, now `input-probe/3`) use it.
- Mutation control plans (`cmre/mutate.py`) accept a witness at any case number
  below `cases` plus the number of hinted inputs pinned in the plan's identity.
  A report must have run exactly that many hinted inputs.
- Unchanged: pointer inference (`difftest.infer`), composition gates
  (`composition.py`, `composition_diagnostic.py`), pipeline screens and the
  corpus-replay diagnostic scripts. They keep the random-only stream, so their
  random case numbers still match the gate's. Composition therefore does not run
  hinted inputs: its coverage can lag the single-function gate (`fn_004b4850`:
  0.348 against 0.809). That fails safe (a composition can only be rejected for
  low coverage it would also have had before), but a composed claim is weaker
  evidence about hinted inputs until composition adopts `gate_cases` in its own
  release. `scripts/cmre_paired_diagnostics.py` now says it observes the random
  corpus only.

## What the previous gate rejects stays rejected, except low coverage

The new case set is a strict superset of the old one: the same 600 random cases
in the same order, plus each hinted input once. `Machine.run` restores a
pristine snapshot for every case, so running inputs first cannot change a later
case. Any mismatch, code-as-data case, inconclusive case or stack-layout excess
the old gate observed still occurs, and every limit compares a count that can
only grow with an unchanged denominator. A hinted input that mismatches fails
the gate (`tests/test_cmre_input_sweep.py::test_a_fixture_only_mismatch_fails_the_gate`).

A low-coverage rejection can turn into a pass: that is the purpose of the change
(`fn_004b4850` below). Hinted cases add coverage under the existing rules, the
same as random cases: a compared, matching case counts its original-side
instructions; so do a code-as-data case and, under the `abi`/`semantic`
profiles, a case where either side ran out of budget (both are limited
separately). Those rules predate this change; they now also apply to every
hinted input.

Operand and `__ftol` mutation kills are preserved for the same reason: a mutant
killed on a random case still mismatches on that same case number, and the
separate report allowance keeps that witness in the report
(`test_hinted_mismatches_cannot_crowd_out_random_case_witnesses`).

Measured on real functions (lifted `exact` gate, identical hints on both sides):
the per-case normalized result digest of all random cases was byte-identical
between the old and the new harness for `fn_005576e0` (600 cases),
`fn_004b4850` (600) and `fn_006181d0` (10,000).

## Measurements (2026-11-27, old harness `9914b5ac…`, new `0060077d…`/`0fae83e7…`)

| Function | Hinted inputs | Never run before → after | Coverage before → after | Status before → after | Gate time | Peak working set |
| --- | --- | --- | --- | --- | --- | --- |
| `fn_005576e0` (canonical hints) | 143 fixtures + 6 captures | 21 + 1 → 0 | 0.8422 → 0.9583 | pass → pass | 43.9 s → 61.2 s | 281 → 271 MB |
| `fn_004b4850` (uncapped probe picks of spec 3358) | 465 fixtures + 96 captures | 281 + 55 → 0 | 0.3480 → 0.8091 | low-coverage → **pass** | 5.0 s → 10.7 s | 146 → 151 MB |
| `fn_004b4850` (192-capped picks of the same spec) | 192 + 96 | 59 + 37 → 0 | 0.3038 → 0.4375 | low-coverage → low-coverage | 4.3 s → 6.4 s | 129 → 132 MB |
| `fn_006181d0` (largest hint set; 10,000 random cases) | 2,982 fixtures | 561 → 0 | 0.8263 → 0.9496 | pass → pass | 38.0 s → 38.8 s | 154 → 156 MB |
| `fn_005576e0` accepted C, `semantic`/v2, -O0 and -O2 | 143 + 6 | not counted | 0.8495 → 0.9583 (both builds) | pass → pass | 82.1 s → 118.1 s (both builds) | 282 → 284 MB |

The lifted rows ran on `0060077d…`. The final identity `0fae83e7…` adds only
the review fixes (separate report allowances, `explain` replay without other
cases); the C row ran on it. Its "before" run reproduced the accepted ledger
record exactly (coverage 0.8495, result digest `46a5bcb9…`), and the random-case
digests were byte-identical at both optimisation levels. Zero mismatches in every run. Times are single runs on a shared, loaded host
(farm running), so they indicate scale, not exact cost. The "before" 0.3038
equals the lane's own recorded best for that spec. Build time (4–20 s) is
excluded. Scripts and JSON: session scratch `sweep/` (`measure.py`, `prepare.py`,
`scope.py`); not retained evidence.

## Invalidated evidence

The harness identity changes from
`9914b5acd2ddb6186d3a4352b7c6e980bbb2ee4c133b349f2ea8be52906a4041` to
`0fae83e7c45b5de207d09b014047f42e03427052a6cd850cd15f76a75d032650`
(`difftest.py` is a harness module). No revalidation was run for this release.

- **All accepted tiers lose automatic reuse**: 8,922 lifted and 4,597 C tiers
  (coordinator ledger, 27 Nov). 4,373 lifted and 4,581 C records carry the
  current identity `9914b5ac…`; the other 4,549 lifted and 16 C records already
  carry older identities, reused through earlier receipts. The existing
  validation-compatibility proof cannot carry any of them, because its guard
  refuses changes to case generation.
- **Tiers whose execution changes**: those whose effective hints list fixtures
  or captures. 2,431 lifted tiers (3,046,895 inventory body bytes; 39,216 hinted
  inputs) and 2,208 C tiers (2,490,753 B; 37,362 hinted inputs). The median is 5
  inputs per tier. 31 functions list at least 192 (30 lifted and 30 C tiers) and
  4 at least 600 (4 lifted and 4 C tiers). These tiers run inputs that may never
  have run before and can now fail.
- **Tiers whose execution is identical by construction**: the other 6,491 lifted
  and 2,389 C tiers. They run the identical case stream; only `input_cases: 0`
  is added to the report, and `result_digest` is unchanged.
- **Gate run caches** (`CM0102/Analysis/cmre/runs`) miss under the new
  execution identity.
- **Other records bound to the harness digest**: composition reports
  (`composition.py`, checked by `validation.composition_impact`), work-queue
  identities (`work.py`), pilot records (`pilot.py`), job receipts (`jobs.py`)
  and the families index (`families.py`, rebuilt automatically). Composition
  execution itself is unchanged, so a composition re-run reproduces its
  previous result under the new identity.
- **Return-screen receipts** (`cmre/screens`): the observer digest of
  `screen.py` changes, so every receipt is stale and is re-observed on demand.
- **Mutation control plans** pin the `mutate.py` controller digest and the
  execution identity; existing plans must be re-planned.
- **Operand and `__ftol` mutation evidence** (ADR 0010, ADR 0006 condition 4)
  is bound to gate reports of the old identity; re-accepting a bridged or
  `__ftol` candidate re-runs its mutants under the new identity.
- **Input lane**: attempts are counted per harness digest, so exhausted rows
  re-open, and the `input-probe/3` version re-gates every held best spec once
  without a model call.

Deployment, not this change, decides how the affected tiers are re-validated:
re-gate exactly the 4,639 affected tiers (as ADR 0012 did for its 276) and add
a compatibility proof for the identical tiers, or run one full sweep (ADR 0007
measured about 3.5 h of wall time at eight workers for 11,265 tiers; there are
now 13,519). That runs at a batch boundary
under the writer lease. Any tier that fails is named in the release record and
resolved through `cmre` (retract, repair or re-hint), never hidden.

## Consequences

- An input's reach no longer depends on a random draw. Inputs the input lane
  appended in earlier gates now all run.
- Gate time grows by one case per hinted input. Measured cost is modest, but a
  hint file holding many large fixtures also costs memory to load:
  `fn_005576e0`'s 143 fixtures are 27.8 MB of JSON.
- The probe's 192-pick cap now bounds gate time rather than reach.
  `fn_004b4850` needs its 465 picks (0.8091) and stays below threshold at 192
  (0.4375). Raising the cap is a separate tooling decision; size it by hint
  bytes as well as count.
- The random stream still replaces about half the random cases with a random
  hinted input when inputs exist. Those cases duplicate inputs that now run
  anyway; ADR 0013's caveat about dropped random cases remains. Removing that
  draw would change global-input discovery and the random cases that follow it,
  so it needs its own release and superset argument.
- Coordinate with the infeasible-code accounting release (coverage
  denominator): both change the harness identity, so batch them under ADR 0007.
