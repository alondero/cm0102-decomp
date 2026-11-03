# Next-phase plan: measured limits, design and sequence

Written 2026-11-03 from live `cmre status`, `shards`, `work list` and job receipts
in the authoritative checkout, plus read-only scratch measurements (scripts and
outputs are listed in section 9). Counts move: re-read them before acting. This
plan changes no rule in [AGENTS.md](../../AGENTS.md); decisions it needs are in
section 8, and two are drafted as proposed ADRs ([0006](../adr/0006-compiler-inserted-helpers-in-plain-c.md),
[0007](../adr/0007-harness-releases.md)).

Terms: **C** is readable C source; **lift** is the machine-translated assembly
tier; a **tier** is one validation unit; **x87** is the x86 floating-point unit;
the **harness** is the modules in `HARNESS_MODULES` (`cmre/recover.py`).
Addresses are preferred-image VAs (base `0x00400000`) in the primary binary,
SHA-256 `0c78e7da…f86`.

## 1. Where we are

| Measure | Value |
| --- | --- |
| Accepted C/legacy | 3,332 functions; 1,348,610 of 4,309,884 body bytes (31.3%) |
| Lift-inclusive recovered | 8,035 functions (49.0%); integrated 0 |
| Audit 2026-11-03 | 11,265 tiers passed, 0 failed; harness identity `904e5da7…c734` |
| Unlifted (coverage below 0.6) | 1,152 functions, 2,086 KB |
| Held in the work registry | 702 functions, 702 KB (192 blocker names, 119 used once) |
| Lift-blocked | 23 functions, 50 KB |
| **Ready C queue** | **136 functions, 8 KB** |

## 2. What limits throughput (measured)

1. **Supply, not translation.** 8 KB is ready. Every remaining byte sits behind
   a blocker or the coverage barrier. Adding translators or providers now only
   adds contention.
2. **Per-function validation is cheap.** A whole `cmre test --c` process (O0 and
   O2, both builds) took 2.5 s, 4.9 s and 7.2 s for 100, 348 and 1,204-byte
   functions. Accept reuses that cached result and repeats it once fresh, so an
   accept costs about the same again. Audit timings for 3,281 C tiers (four
   executions each): median 5.2 s, p90 23.6 s; about 40–65 CPU-seconds per KB.
   500 KB a day needs about 7 CPU-hours. The earlier "about 4 minutes per
   accept" figure does not hold for the current tooling.
3. **The batch check holds the lease for about 5.5 minutes** (pytest 328 s plus
   verify). Accept waits behind it. At 20 accepts per batch and 1,235 functions
   a day that is about 5.7 hours of blocked accepts; hourly batches cost about
   2.2 hours.
4. **Harness changes cost a sweep.** A full audit at 8 jobs is about 3.5 hours
   of wall time (5,598 tiers ran in 53 minutes with no `MemoryError`). Sweeps
   per edit, not their size, did the damage.
5. **Tokens.** About 2 million tokens per accepted function historically,
   including overhead. At the target rate that is 2.5 billion a day. This must
   fall before scaling; measure it per cohort (section 7).
6. **Corpus sensitivity is not established.** Coverage counts executed
   instructions, not whether a value reaches an observed output. For
   `fn_006fa140`, four deliberately wrong variants passed the unchanged gate.
   A seeded sample of 30 accepted semantic C functions, with one comparison
   operator flipped per mutant (`<`↔`<=`, `==`↔`!=`, and so on), gave 52 valid
   mutants: **12 survived (23%), in 11 of the 30 functions.** Survivors sit in
   unexecuted code (coverage 0.63–0.74) and at boundary values never hit (three
   at coverage 1.0). Four survivors were read and all change behaviour; the
   other eight are unread, so some may be equivalent. Acceptance today shows
   equivalence on the executed cases, not over the whole function.

## 3. Design

### 3.1 Proof that each function is right, and stays right

Keep everything that exists: the unchanged semantic gate at O0/O2, the fresh
determinism repeat, ledger hashes, dependency-aware revalidation, composition
reports kept separate, and the guard's lease rule. Add two things.

- **Sensitivity evidence.** The sample (section 2, item 6) shows survivors are
  common. Add a `cmre mutate fn_X` diagnostic (not a harness module) that runs
  operator and constant mutants through the unchanged gate and lists survivors.
  Review reads those sites first and adds evidenced fixtures where the original
  can be driven there. Making "no unexplained survivor" an acceptance condition
  is a separate decision (section 8). Already-accepted functions with survivors
  are not wrong by this; their evidence is narrower than their coverage figure
  suggests.
- **Harness releases** ([ADR 0007](../adr/0007-harness-releases.md)). Harness
  edits batch in a worktree and land as one release with one revalidation.
  Recovery lanes never run under a changing harness.

### 3.2 Validation cost

- **Accept stays serial.** At about 8 s per accept one lane exceeds 400
  functions an hour. Per-function locking is not needed for the target; do not
  build it now.
- **Keep the second differential run in accept.** It costs seconds per function
  and is the determinism evidence ADR 0001 relies on.
- **Proposed: batch hourly, not every 20 accepts,** once accepts exceed 20 an
  hour. Until decided, the [PLAYBOOK](PLAYBOOK.md#validation-budget) rule stands.
- **Split pytest by input class.** A read-set measurement (audit hook over the
  full suite) found 13 test files that read the real `recovery/` tree: they open
  all 7,984 ledger records and stat 4,721 `recovery/c` paths, so a key over
  "files read" still changes on every accept. Those 12 files (excluding one that
  takes the real lease) ran in 93 s; the full suite takes 328 s. Build: a
  `workspace` marker for those files; a conftest audit hook that fails any
  unmarked test which touches the real `recovery/` tree (the proof); tooling
  tests reused on a code-only key; workspace tests run every batch. Saves about
  4 minutes per batch.
- **Fix `test_lifted_refresh_refuses_to_erase_existing_c_tier`.** It calls
  `recover.accept`, which takes the real lease. It fails when a writer runs
  without passing its token, and can briefly block a real writer.

### 3.3 Supply program (the throughput program)

Work by blocker family, not by function. One owner per family writes the
contract, its positive and negative regressions and the retry condition; then
translators apply it.

| Family | Size | Lever |
| --- | --- | --- |
| `__ftol` boundary | 127 held fns, 305 KB; plus 123 unlifted, 548 KB | Policy, not harness: [ADR 0006](../adr/0006-compiler-inserted-helpers-in-plain-c.md). Spike `fn_006fa140` passes as plain C; sensitivity fixtures needed |
| Remaining x87 | 17 fns, about 9 KB; 63 distinct forms across the group | Only after ADR 0006; extending the transport form by form unlocks slowly (29 forms reach 92 fns) |
| Coverage barrier | 1,152 fns, 2,086 KB | Spike below |
| No blocker class | 125 fns, 67 KB | `cm-triager`; assign each to a family |
| Missing virtual entry | 46 fns, 26 KB | Inventory reconciliation check first (read-only) |
| Import output parameters | 12 + 25 fns, 38 KB | One import-boundary contract |

**Coverage-barrier spike (time box: one working day).** Random and probed inputs
plateau below 0.6 (2026-10-21: the original-only input explorer ran on almost
every unlifted function; a 40 times larger exploration budget changed 1 of 9
sampled functions). The unlifted set shares a few callees: `0x0090d130`
(`mov eax, 0x00ad4394; ret`, a global-object accessor) is called by 585 of them
(1.2 MB); `0x00944c9f` by 674; `0x005e8290` by 599. The likely cause is
unrealistic state under that object graph. Two arms on one fixed, size-stratified
sample of 20 unlifted functions:

- **A. Authored fixtures** (no harness change): an agent reads the listing and
  writes evidenced `fixtures` hints that set the object pointers and fields the
  branches test, then measures original-side coverage.
- **B. Captured state:** snapshot the object graph under `0x00ad4394` from the
  running primary game (extending the read-only runtime observers in
  [RUNTIME-REPLAY.md](../RUNTIME-REPLAY.md), which today read the GDI build).

Decide on the number of sample functions that reach 0.6 with the comparison
unchanged. If arm A reaches it, it becomes a parallel lane (input authoring)
that needs no harness release.

### 3.4 Lanes and providers

| Role | Count | Rule |
| --- | --- | --- |
| Integrator | 1 | Claims, serial accept, hourly check, handoff. Never delegates acceptance. |
| Family owner | 1–2 | Writes a family contract and its regressions. Strongest model. |
| Input author | from spike result | Writes fixtures for coverage and sensitivity. |
| Translator | ready KB / 20, at most 8 | Cheapest model passing the adapter checks; families of up to three. |
| Reviewer | 1 per 2–4 translators | Different model family; reviews surviving mutants first. |

Providers follow the adapter rule in the 2026-11-03 plan
([MULTI-PROVIDER-PROPOSAL.md](MULTI-PROVIDER-PROPOSAL.md),
[PROVIDER-RESEARCH.md](PROVIDER-RESEARCH.md); re-verify capabilities and quota).
Scale translators only while `cmre next --c --min-bytes 129` is non-empty.

## 4. Sequence

| Phase | Work | Done when | Gate to next |
| --- | --- | --- | --- |
| 0 (done 2026-11-03) | Audit green; guard lease rule in both checkouts; ADR 0006 and 0007 accepted; mutation sample | main check green | — |
| 1 (day 1) | Day plan below: `__ftol` family, coverage spike arm A, triage, tooling, `needs-review` | Lanes report; tokens per byte measured | Midday and end-of-day reviews |
| 2 (days 2–3) | Rest of the `__ftol` family; coverage lane on the arm that worked; providers if tokens allow | Family accepted or held by name | Ready supply and tokens per byte |
| 3 (days 3–5) | Coverage lane across the unlifted set; family contracts for the remaining groups | Ready queue above 200 KB | Ready supply |
| 4 | Scale translators and providers against measured supply and tokens | 500 KB/day for 3 days | — |

500 KB per day is reachable only if the coverage lane opens the unlifted set;
without it the reachable remainder is about 0.8 MB.

### Day 1 plan: parallel lanes

Only `accept`, `block`, `unblock`, `batch`, `revalidate`, the two `validation-*`
commands, `status --write` and the session `check` take the writer lease.
`brief`, `next`, `test`, `screen` and `work set|retry|claim` use short queue
locks, so every lane except the integrator runs concurrently. No harness release
is planned for day 1; a lane that needs a `HARNESS_MODULES` change stops and
reports it.

| Lane | Where | Model | Owns | Done when |
| --- | --- | --- | --- | --- |
| L1 Integrator | main checkout | strongest Claude | preflight, claims, review, serial `accept`, hourly `check`, pilots, token log, handoff | every passing source reviewed and accepted or held |
| L2 `__ftol` owner | main checkout | strong | representative, cohort list, `work retry` of the cohort | representative accepted; cohort of 30 listed by family |
| L3–L6 Translators (4) | main checkout, `next --c --claim` | cheapest that passes | up to 3 IDs each from the L2 list | `fn_X pass|wip` returned, mutation control run |
| L7 Coverage spike, arm A | main checkout, sample hints only | strong | 20-function sample, `fixtures` hints, coverage numbers | number of sample functions at 0.6, by midday and end of day |
| L8 Tooling | separate worktree | strong | `cmre mutate`, pytest split, lease-free acceptance test, blocker families | focused tests green; merged once at end of day |
| L9 Triage | main checkout | medium | 46 missing-virtual-entry items, then the 125 with no class | each item has a family or a recorded cause |

Lane details:

- **L1.** Run `py -3.13 scripts/cmre_session.py start` once. First work: the 23
  `needs-review` items (`cmre work list --state needs-review`): test once,
  review, accept or hold by name. Start a pilot per cohort (`cmre pilot start`),
  record elapsed time, accepted bytes and each lane's token use. Run `check` at
  most hourly and never while a batch or revalidation runs. Accept nothing that
  lacks its mutation control (C.md, `__ftol` rule).
- **L2.** Make `fn_006fa140` the representative: move the spike source
  (`CM0102/Analysis/cmre/diagnostics/scale-plan-20261103/spike/`) to
  `recovery/c/`, add `c_hints` fixtures that make the FADD, precision, `<` and 360
  controls fail, then `work retry --changed --reason "ADR 0006"` and test. Build
  the cohort from the 127 `__ftol` callers, smallest first, preferring those whose
  only x87 dependency is `__ftol` (callers of `0x00945ff0` need their own entry).
  Group them with `cmre family` and hand translators families of up to three.
- **L3–L6.** Follow [C.md](C.md) with the `__ftol` rule. Write only the claimed
  `recovery/c/fn_X.c` and hints for claimed IDs. Stop after five failed tests.
- **L7.** Pick a seeded, size-stratified sample of 20 unlifted functions, at
  least half calling `0x0090d130`. Write fixtures that set the object graph under
  `0x00ad4394` from listing evidence; measure original-side coverage. Report the
  number reaching 0.6. Arm B (captured game state) starts on day 2 only if arm A
  reaches fewer than 5.
- **L8.** In a fresh worktree from the published head: (1) `cmre mutate fn_X`
  reporting surviving operator and constant mutants through the unchanged gate;
  (2) a `workspace` pytest marker on the 13 files that read the real `recovery/`
  tree, a conftest audit hook that fails unmarked tests that touch it, and
  separate reuse keys; (3) make `test_lifted_refresh_refuses_to_erase_existing_c_tier`
  use a temporary lease; (4) a blocker family catalogue and `cmre work families`.
  None of these are harness modules. Tests first.
- **L9.** Read-only inventory check of the 46 missing-virtual-entry items
  ([INVENTORY-RECONCILIATION.md](../INVENTORY-RECONCILIATION.md)), then `cm-triager`
  on the no-class items. Record each under a family with a retry condition.

Leave the coordinator's existing holds alone: `fn_005b1940`, `fn_00441c10`
composition, `fn_00524d10`, `fn_00524d50`, `fn_0068dbc0` (see the live handoff).

Reviews:

- **Midday.** `__ftol` accepted bytes per hour, review corrections, surviving
  mutants, tokens per accepted byte; arm A count. Add translators only if the
  ready list exceeds four families and tokens per byte are within the measured
  range.
- **End of day.** Merge L8 once, run `check`, checkpoint, and report tokens per
  accepted byte per lane so the daily cap can be set (decision 7).

## 5. How agents work autonomously

- Bounded goal per session: a byte target and a stop condition.
- Check long jobs at most hourly with `py -3.13 -m cmre job status RECEIPT`.
- Claim, do, hand back: translators return `fn_X pass|wip: <detail>`.
- Record held work under an existing family name (`cmre work list`); a new name
  needs a reason that no family fits.
- Escalate harness, acceptance-policy, fidelity and provider-terms questions.

## 6. Validation budget

[PLAYBOOK.md](PLAYBOOK.md#validation-budget) is the single source. In short:
function tests per function, one check per batch (hourly at high rate), one
revalidation per harness release, an audit only for a milestone claim.

## 7. Measures

Per lane and cohort: accepted bytes per elapsed hour; review corrections and
later defects; surviving mutants per function; tokens per accepted byte (cached
input separately); validation seconds per accepted byte; accept-lane wait time.

## 8. Decisions (recorded 2026-11-03 by the project owner)

1. ADR 0006 accepted: `__ftol` as a cast, with sensitivity evidence.
2. ADR 0007 accepted: harness releases, at most one a working day.
3. Surviving mutants are a review aid first; whether they become an acceptance
   condition is decided from the day-1 numbers.
4. Build the pytest split (lane L8).
5. No per-function locking; the fresh accept repeat stays.
6. Coverage spike: one working day for arm A, stop rule in section 3.3.
7. Tokens are measured on day 1; the daily cap and per-function target are set
   from those numbers. Until then, at most four translators.

## 9. Evidence and caveats

- Measurement scripts and outputs (`audit_timing.py`, `supply.py`, `x87_forms.py`,
  `x87_callees.py`, `fpenv_census.py`, `readset/`, `spike/`, `mutants.py`,
  `mutants-results.json`) are retained locally under
  `CM0102/Analysis/cmre/diagnostics/scale-plan-20261103/` (ignored, not published).
- The x87 and callee censuses are linear disassembly of inventory extents:
  structural evidence, not execution.
- Audit per-tier seconds include tiers reused from the earlier receipt.
- The spike is one function; its pass is weak evidence (section 2, item 6).
- Token figures come from earlier session reviews, not a controlled cohort.
