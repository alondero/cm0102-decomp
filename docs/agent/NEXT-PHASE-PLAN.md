# Next-phase plan: measured limits, design and sequence

Current timed provider operations and roles are in
[PROVIDER-CAMPAIGN.md](PROVIDER-CAMPAIGN.md), updated after the November 8 pilots.
Use that workflow for the next 150-minute run. Counts, concurrency estimates and
day plans below are historical measurements; refresh supply before dispatch.
The current provider workflow takes precedence over historical lane counts.

Written 2026-11-03 from live `cmre status`, `shards`, `work list` and job receipts
in the authoritative checkout, plus read-only scratch measurements (scripts and
outputs are listed in section 9). Counts move: re-read them before acting. This
plan changes no rule in [AGENTS.md](../../AGENTS.md); decisions it needs are in
section 8. Two are accepted ADRs ([0006](../adr/0006-compiler-inserted-helpers-in-plain-c.md),
[0007](../adr/0007-harness-releases.md)). Section 3.3 and the day-1 plan were
revised the same evening from the coverage spike's arm A result.

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
| Coverage barrier | 1,152 fns, 2,086 KB | Arm A result below: one input family now, a harness release for the rest |
| Record-table chain (coverage) | not yet counted | Authored `fixtures`, no harness change (arm A passed 4 of 4 tried) |
| Input capacity (coverage) | 10 of the 20 sampled fns | Harness release: arena size, coherent CRT heap, CRT file and import output buffers |
| No blocker class | 125 fns, 67 KB | `cm-triager`; assign each to a family |
| Missing virtual entry | 46 fns, 26 KB | Inventory reconciliation check first (read-only) |
| Import output parameters | 12 + 25 fns, 38 KB | One import-boundary contract; part of the input-capacity release |

**Coverage-barrier spike.** Random and probed inputs plateau below 0.6
(2026-10-21: the original-only input explorer ran on almost every unlifted
function; a 40 times larger exploration budget changed 1 of 9 sampled
functions). The spike had two arms on one fixed, size-stratified sample of 20
unlifted functions: **A**, authored fixtures (no harness change), and **B**,
state captured from the running primary game (extending the read-only runtime
observers in [RUNTIME-REPLAY.md](../RUNTIME-REPLAY.md), which today read the GDI
build). The rule: if arm A brings 5 or more to 0.6 with the comparison unchanged,
input authoring becomes a lane; otherwise arm B starts.

**Arm A result (2026-11-03 evening, unchanged gate, 600 cases).** Evidence:
`CM0102/Analysis/cmre/diagnostics/coverage-spike-20261103/RESULT.md` (local).

- The premise was wrong. `0x00ad4394` is the version string `"3.9.68"`, and
  `0x0090d130` → `0x00944c9f` (`sprintf`, format at `0x009870e8`) →
  `0x005e8290` (`"Error"` box) is the game's error-report sequence. Calling it
  marks a function with error paths; it is not a shared object graph. Only 1 of
  the 10 sampled accessor callers passed; 3 of the 10 non-callers did.
- **4 of 20 pass** (baseline 0 of 20): `fn_008dda90`, `fn_008de150`,
  `fn_00935cb0`, `fn_00561050`. A fifth, `fn_0055efe0`, reaches 0.66 but its
  lifted and original sides diverge. By size band: small 3 of 7, medium 1 of 7,
  large 0 of 6. The strict count is below 5, so arm B is triggered.
- Arm A works on one family: small and medium functions gated by chains of
  global record tables (`0xae23bc`, `0xae23b4`, `0xb63c98`, `0xae2364`) or a
  pointer structure. Each took 1–3 attempts, and evidence carried from one
  function to the next.
- 10 of the 15 still below 0.6 stop on input capacity, which arm B cannot fix
  either (the other 5 need deep object graphs or realistic data):
  - game objects of 0.76–2.4 MB against a `0x40000`-byte arena (`cmre/emu.py:26`);
    6 sampled functions touch such objects, and 2 stop there;
  - import stubs that return independent results, so `HeapAlloc` then
    `HeapSize` faults (3 league constructors);
  - CRT file I/O and import output buffers (`fopen`, save files, CD check),
    5 functions.

  These are harness changes, so they ship as one release
  ([ADR 0007](../adr/0007-harness-releases.md)). Prefer designs whose new
  inputs are opt-in per function (a hint key), so existing tiers keep their
  case corpus and the release can reuse evidence through a compatibility
  receipt instead of a 3.5-hour audit.
- Arm B targets deep object graphs (`fn_008b2880`, `fn_006d1cc0`,
  `fn_0070c040`, `fn_008dc370`). Most captured objects will not fit the current
  arena, so arm B depends on the input-capacity release. `fn_006d1cc0` is 8
  instructions short of 0.6 under authored inputs.

### 3.4 Lanes and providers

| Role | Count | Rule |
| --- | --- | --- |
| Coordinator / family owner | 1 | Codex: original evidence, shared contracts, final review, serial accept, gates, retro and handoff. |
| Translator | Within two external workers total | MiniMax initially; eligible functions with prepared evidence, up to three related IDs per fresh assignment. |
| Preliminary reviewer / second translator | Within the same cap | DeepSeek Go initially; independent evidence review or disjoint translation. Final authority stays with the coordinator. |
| Input proposal author | Within the same cap | Either provider in a bounded trial; private evidenced proposals, coordinator adoption and validation. |

Use [PROVIDER-CAMPAIGN.md](PROVIDER-CAMPAIGN.md) and the implemented single-worker
adapter in [PROVIDER-WORKERS.md](PROVIDER-WORKERS.md). The old multi-provider
proposal and research retain history, not current dispatch instructions. Re-verify
account/model access. Scale translation only with prepared substantial supply
and demonstrated review capacity; an empty `cmre next --c --min-bytes 129`
redirects work to blockers and inputs. No measured result authorizes more workers
than the user's cap.

## 4. Sequence

| Phase | Work | Done when | Gate to next |
| --- | --- | --- | --- |
| 0 (done 2026-11-03) | Audit green; guard lease rule in both checkouts; ADR 0006 and 0007 accepted; mutation sample; coverage spike arm A (4 of 20) | main check green | — |
| 1 (day 1) | Day plan below: accept arm A passes, `__ftol` family, record-table fixture lane, input-capacity release design, arm B feasibility, triage, tooling | Lanes report; tokens per byte measured | Midday and end-of-day reviews |
| 2 (days 2–3) | Input-capacity release (one revalidation); rest of the `__ftol` family; arm B on the released harness; providers if tokens allow | Release applied and revalidated; family accepted or held by name | Ready supply and tokens per byte |
| 3 (days 3–5) | Coverage lanes across the unlifted set by input family; family contracts for the remaining groups | Ready queue above 200 KB | Ready supply |
| 4 | Scale translators and providers against measured supply and tokens | 500 KB/day for 3 days | — |

500 KB per day is reachable only if the coverage lanes open the unlifted set;
without them the reachable remainder is about 0.8 MB. Arm A shows authored
inputs open one family cheaply. Most of the rest waits on the input-capacity
release.

### Day 1 plan: parallel lanes

Only `accept`, `block`, `unblock`, `batch`, `revalidate`, the two `validation-*`
commands, `status --write` and the session `check` take the writer lease.
`brief`, `next`, `test`, `screen` and `work set|retry|claim` use short queue
locks, so every lane except the integrator runs concurrently. No harness release
is applied on day 1: the input-capacity lane designs and tests one, and the
project owner decides on it at the end-of-day review. Any other lane that needs
a `HARNESS_MODULES` change stops and reports it.

Model tiers: **top** (strongest available), **moderate** (cheaper, capable),
**cheap** (mechanical tasks with checkable output). Until tokens are measured,
at most four translators (decision 7).

| Lane | Where | Tier | Owns | Done when |
| --- | --- | --- | --- | --- |
| L1 Integrator | main checkout | top | preflight, claims, review, serial `accept`, hourly `check`, pilots, token log, handoff | every passing source reviewed and accepted or held |
| L2 `__ftol` owner | main checkout | top | representative, sensitivity fixtures, cohort list | representative accepted; cohort of 30 listed by family |
| L3–L5 Translators (3) | main checkout, `next --c --claim` | moderate | up to 3 IDs each | `fn_X pass\|wip` returned, mutation control run |
| L6 Translator (1) | main checkout, `next --c --claim` | cheap | up to 3 small IDs at a time | same; compared with L3–L5 on tokens and corrections |
| L7 Record-table fixtures | main checkout, claimed hints only | moderate | the arm A family: census, fixtures, coverage | each claimed function passes the lifted gate or is held with its stop |
| L7c Census helper | main checkout, read-only | cheap | static census for L7 and L9 | lists written with their scripts |
| L8 Tooling | separate worktree | moderate | `cmre mutate`, pytest split, lease-free acceptance test, blocker families | focused tests green; merged once at end of day |
| L9 Triage | main checkout | moderate | 46 missing-virtual-entry items, then the 125 with no class | each item has a family or a recorded cause |
| L10 Reviewer | main checkout, read-only | top, other model family if available | review `needs-review` sources for L1 | each review has a verdict |
| L11 Input-capacity release | separate worktree | top | proposed ADR 0008, harness changes and tests, `revalidate --plan` impact; `fn_0055efe0` mismatch | ADR and tested branch ready for the owner's decision |
| L12 Arm B feasibility | main checkout, read-only runtime | top | captured state for one or two sample functions | a capture that reaches 0.6, or the named blocker |

Lane details:

- **L1.** Run `py -3.13 scripts/cmre_session.py start` once. First work, in
  this order:
  - The arm A passes. Review and accept `fn_008dda90`, `fn_008de150` and
    `fn_00935cb0`. These are lifted-tier accepts: they move the functions into
    the C queue.
  - `fn_00561050` only after a reviewer rules on its record index 404 against
    the 250-record capacity, as recorded in its hint's `fixture_provenance`.
  - The 23 `needs-review` items (`cmre work list --state needs-review`).

  Start a pilot per cohort (`cmre pilot start`). Record elapsed time, accepted
  bytes and each lane's token use. Run `check` at most hourly, and never while a
  batch or revalidation runs. Accept no `__ftol` source without its mutation
  control (C.md).
- **L2.** Make `fn_006fa140` the representative:
  - Move the spike source from
    `CM0102/Analysis/cmre/diagnostics/scale-plan-20261103/spike/` to
    `recovery/c/`.
  - Add `c_hints` fixtures that make the FADD, precision, `<` and 360 controls
    fail.
  - Run `work retry --changed --reason "ADR 0006"`, then test.

  Then build the cohort from the 127 `__ftol` callers, smallest first. Prefer
  those whose only x87 dependency is `__ftol`; callers of `0x00945ff0` need their
  own registry entry. Group them with `cmre family` and hand translators families
  of up to three. ADR 0006 governs the callers. `0x00945640` itself stays a
  separate function with its own holds.
- **L3–L6.** Follow [C.md](C.md) with the `__ftol` rule. Write only the claimed
  `recovery/c/fn_X.c` and hints for claimed IDs. Stop after five failed tests.
  Before L2's cohort is ready, take the ordinary C queue, including the functions
  L1 lifts. L6 takes functions of 128 bytes or less.
- **L7.** Reuse the arm A tools (`try_fixture.py`, `write_hint.py`, `measure.py`,
  `specs/` in the spike evidence directory).
  - Take candidates from L7c's census: unlifted functions of up to about
    900 bytes that read the record tables (`0xae23bc`, `0xae23b4`, `0xb63c98`,
    `0xae2364`), and that do not use CRT heap or file imports.
  - For each, write fixtures from instruction evidence and measure with the
    unchanged `cmre test` lifted gate.
  - Claim each function before editing its hint, remove `"auto": true` and back
    up the old hint.
  - Stop a function after three evidence-based attempts.
  - Hand passes to L1.
- **L7c.** Two read-only censuses. Each writes its script and its output under
  `CM0102/Analysis/cmre/diagnostics/`.
  - For L7: unlifted functions by the record-table globals they read and by the
    imports or CRT helpers they reach.
  - For L9: the missing-virtual-entry list.
- **L8.** In a fresh worktree from the published head, tests first:
  1. `cmre mutate fn_X`, reporting surviving operator and constant mutants
     through the unchanged gate.
  2. A `workspace` pytest marker on the 13 files that read the real `recovery/`
     tree, a conftest audit hook that fails unmarked tests that touch it, and
     separate reuse keys.
  3. Make `test_lifted_refresh_refuses_to_erase_existing_c_tier` use a temporary
     lease.
  4. A blocker family catalogue and `cmre work families`.

  None of these are harness modules.
- **L9.** Read-only inventory check of the 46 missing-virtual-entry items
  ([INVENTORY-RECONCILIATION.md](../INVENTORY-RECONCILIATION.md)), then
  `cm-triager` on the no-class items. Record each under a family with a retry
  condition.
- **L10.** Review with [REVIEW.md](REVIEW.md), reading surviving mutant sites
  first once L8's `cmre mutate` exists. Return a verdict per function to L1.
  Use a different model family from the translators where one is available.
- **L11.** Follow [HARNESS.md](HARNESS.md) and [ADR 0007](../adr/0007-harness-releases.md).
  - Draft proposed ADR 0008 covering three changes:
    - arena capacity for game objects of up to about 2.4 MB;
    - a coherent CRT heap behind the `HeapAlloc`/`HeapSize`/`malloc` stubs;
    - CRT file I/O and import output buffers.
  - Prefer opt-in per-function inputs, so existing tiers keep their corpus.
  - Implement in a worktree with focused tests, and report the
    `cmre revalidate --plan` impact.
  - For runs against the real binary, use an isolated candidate under
    `CM0102/Analysis/cmre/diagnostics/`, following the
    `composition-fault-location-guard-v2-20261103` precedent. Never edit the
    main checkout's `cmre/`.
  - Separately, find the cause of `fn_0055efe0`'s divergence: start from
    `cmre explain fn_0055efe0 --case 7` (`0x9462bb` destructor tail). If it is
    a lift defect, it joins the release.
- **L12.** Read [RUNTIME-REPLAY.md](../RUNTIME-REPLAY.md). Find whether the
  read-only observers can capture, from the primary 3.9.68 game, the inputs
  `fn_006d1cc0` and `fn_008b2880` read, and whether those inputs fit the current
  arena. Never write to the running game, and keep captures local. Report a
  capture that reaches 0.6 under the unchanged gate, or the blocker.

Leave the coordinator's existing holds alone: `fn_005b1940`, `fn_00441c10`
composition, `fn_00524d10`, `fn_00524d50`, `fn_0068dbc0` (see the live handoff).

Reviews:

- **Midday.**
  - Accepted bytes per hour, review corrections and tokens per accepted byte,
    per lane.
  - L7: passes per attempt.
  - L11: draft ADR.

  Add translators only if the ready list exceeds four families and tokens per
  byte are within the measured range. Add a second L7 lane if L7 passes at least
  one function per two attempts.
- **End of day.**
  - Merge L8 once, run `check`, checkpoint.
  - Report tokens per accepted byte per lane, so the daily cap can be set
    (decision 7).
  - The owner decides on ADR 0008. If accepted, the release is applied at the
    next batch boundary with one revalidation.

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
   Outcome 2026-11-03: 4 of 20, so arm B is triggered. Arm A continues as a
   lane for the record-table family only, and the input-capacity release is
   designed on day 1 for the owner's decision (section 3.3).
7. Tokens are measured on day 1; the daily cap and per-function target are set
   from those numbers. Until then, at most four translators.

## 9. Evidence and caveats

- Measurement scripts and outputs (`audit_timing.py`, `supply.py`, `x87_forms.py`,
  `x87_callees.py`, `fpenv_census.py`, `readset/`, `spike/`, `mutants.py`,
  `mutants-results.json`) are retained locally under
  `CM0102/Analysis/cmre/diagnostics/scale-plan-20261103/` (ignored, not published).
- Coverage spike arm A: `CM0102/Analysis/cmre/diagnostics/coverage-spike-20261103/`
  (`RESULT.md`, `sample.json` with seed 20261103, `specs/`, `hints-before/`, and the
  `try_fixture.py`, `write_hint.py` and `measure.py` tools; local, not published).
  The 10 edited hint files there are uncommitted inputs in the main checkout.
- The x87 and callee censuses are linear disassembly of inventory extents:
  structural evidence, not execution.
- Audit per-tier seconds include tiers reused from the earlier receipt.
- The spike is one function; its pass is weak evidence (section 2, item 6).
- Token figures come from earlier session reviews, not a controlled cohort.
