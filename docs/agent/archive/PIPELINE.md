# Replenishing recovery pipeline

Use this workflow for session capacity and throughput work. Optimize reviewed
accepted C and connected behavior. Saturday's rate is a comparison point, not a
ceiling. Supply counts describe candidates, not promised accepted bytes.

## Start a continuous run

Complete PLAYBOOK.md preflight/allocation and HARNESS.md recovery intake. Keep
execution, headers and tooling fixed while running. Provision isolated provider
profiles through `cmre_provider_slots.py`: four pairs supply eight model stages;
up to eight pairs are supported. Reuse qualified explicit low effort/65536 output
cap profiles. Profile isolation and actual model identity remain required.

Prepare finite initial supply, then initialize with continuous service enabled:

```powershell
py -3.13 -m cmre pipeline replenish --output PREP --profiles PROFILES.json --slots 4 --limit 64 --buffer-kb 128 --window-minutes 180 --scalable
py -3.13 scripts/cmre_provider_campaign.py init RUN --manifest PREP/manifest.json --profiles PROFILES.json --slot-profiles SLOTS.json --parent PARENT_RECEIPT --pipeline
py -3.13 scripts/cmre_provider_campaign.py run RUN --profiles PROFILES.json --slot-profiles SLOTS.json --parent PARENT_RECEIPT --steps 128
py -3.13 -m cmre pipeline report RUN
py -3.13 scripts/cmre_provider_campaign.py status RUN
```

Replace uppercase paths with the retained local pack. Optional init
`--policy POLICY.json` sets initial stage limits, owner roles, buffers and finite
job budget. Initialization starts no clock; choose the manifest duration for the
authorized recovery window and closing reserve. Run/resume preserves its original
deadline. `--steps` bounds dispatch stages, including paid calls, broker work and
publication, and drains started work. Omit it for continuous operation within the
same deadline. The controller owns the command writer lease; parent checks run
after draining. Retain the actual controller PID for a live process.

Use a small stage limit for initial qualification. A steady run needs enough
stages to keep free slots filled while long turns finish: the limit includes
broker and publication steps, not just model turns. Inspect the bounded `report`
for C/input states, normal C-test outcomes, recent holds, stage time and usage.
Worker seconds overlap; missing exports remain unknown. Accepted bytes come from
inventory, independently of packet-size limits and input adoption.

At the parent batch boundary, pause new model and service dispatch with `scale`,
let the controller exit, then run the parent check. Restore the recorded limits
before resuming the same run and deadline. A smaller `--steps` limit also drains,
but a long final turn can leave the other slots idle after its quota is spent.

Preparation retains complete root instructions, every indexed caller's bounded
window, complete small callees and explicit large-callee excerpts. Oversized
packets stay retained before paid dispatch. Seeds preserve observed fixture
values/address meanings; fallback core-generator samples are diagnostics, not
invented game initialization. Original evidence remains local.

## Scale individual parts

| Stage | Work | Pool |
| --- | --- | --- |
| `intake` | Fresh inventory and input preparation | Model |
| `evidence` | Known held input obligations, spread across families | Model |
| `repair`, `reconcile` | Existing source/input investigation | Model |
| `author` | Eligible C source and source repair | Model |
| `controls` | Propose named source-site sensitivity controls | Model |
| `review` | Other actual model reviews the exact bundle | Model |
| `probe` | Original probes, evidence, inspect and screen | Broker |
| `test` | Normal full C gate | Broker |
| `build` | Retained builds and complete emitted code | Broker |
| `sensitivity` | Existing control-plan gates | Broker |
| `package` | Validate and freeze a bundle | Broker |
| `prepare` | Background inventory/packet preparation | Local service |
| `adoption` | Reviewed input checks, fixtures and native gate | Local service |
| `publish` | Normal finalization and fresh C acceptance | Serial acceptance |

Change a stage while others run, or before resuming a drained run:

```powershell
py -3.13 -m cmre pipeline scale RUN --stage intake --workers 6
py -3.13 -m cmre pipeline scale RUN --stage evidence --workers 1
py -3.13 -m cmre pipeline scale RUN --stage test --workers 4 --execution-workers 8
py -3.13 -m cmre pipeline scale RUN --stage author --workers 6 --owners minimax,deepseek --low-functions 12 --high-functions 32 --low-bytes 32000 --high-bytes 128000
```

Zero workers pauses new dispatch; running work drains. Automatic build/package
follow-ups acquire their own stage limits too. Broker work releases model slots
before execution, so models can author while tests run. `execution_workers`
limits total broker/publication concurrency independently of frozen model/role
capacity. Stage limits are ceilings, not reserved clients; free slots refill
immediately. Owner changes affect newly prepared jobs; frozen assignments remain.

Supply stages have independent low/high byte and function watermarks. Preparation
stops at either high watermark and exposes a byte shortfall when only tiny
functions remain. The finite `max_jobs` policy bounds total admissions.
Adoption waits when the active author buffer reaches a high watermark; reviewed
inputs then accumulate to the input-ready limit and pause new input owners.
This backpressure keeps a paused downstream stage from consuming all upstream
quota. Additional physical profiles or different models require a fresh frozen
run; a policy edit
cannot exceed provisioned capacity. Every revision is retained and pinned; use
`scale` rather than editing the active pointer or historical files.

Native adoption and C publication share one acceptance lock. More adoption
workers parallelize preliminary checks, not canonical acceptance. Full C tests
and evidence work continue independently. Scale execution within host resources;
the playbook's eight-process precedent is not a memory guarantee for all fixtures.

## Automatic promotion and replenishment

Owner windows count actual actor attempts. An enqueue event records preparation;
it does not make an unpaid job active. New pipeline jobs automatically queue the
normal full C test after a successful owner build, under the test stage limit.
Failures return to the author; passing tests still need retained emitted code,
source-site controls where required, independent review and fresh acceptance.

Automatic C publication binds return provenance to the current source's exact
authoritative report. A `void` entry with no indexed direct callers is held:
indirect EAX use remains unknown. Preserve the original return or obtain a
separate original-byte proof and review outside automatic publication. A
destructor-shaped body and a passing void comparison do not prove a dead return.
New pipeline C containing inline assembly is also held before publication; a passing
semantic comparison does not convert an ABI adapter into plain-C recovery.

For a drained older run with excess unpaid C supply, inspect
`cmre pipeline cancel-unstarted RUN --reason "CONCRETE REASON"`. Add `--apply`
only for the intended scope change. It excludes paid/owned work, service holds
and input jobs; retains every packet and candidate; and records hash-chained
cancellations plus an audit receipt. Cancelled jobs stay terminal in that run.
This preserves work and lifetime attempts; it does not retry a held operation.

The supervisor scans in the background, fills separate buffers and appends frozen
jobs to the hash-chained store. Existing configs, attempts, genesis and deadline
stay intact. The only automatic second job for a function is C after reviewed
input adoption. Held attempts/oversized packets do not receive automatic retries.
Shared claims and live/submitted external tasks remain owned. C authors reserve
the normal task before a paid turn and retain ownership while tests are queued.

Input proposals include retained probe IDs and `resolution`: a reason, exact
primary/additional hold `obligations`, original-byte `evidence` entries
(`va`, `bytes`, `supports`), and `unknowns: []`. Returning alone is insufficient.
The frozen bundle pins source/hints/work identity. The other model independently
checks named resolution, fixture-prefix preservation and retained-source
reconciliation. Byte matching establishes provenance; review evaluates claims.

Adoption rechecks exact review/artifacts/ownership and frozen guards under the
writer lease and per-function fence. It preserves fixture prefixes, ordinary
random cases, execution policies and failure counts. Existing native baselines
receive additions under `c_hints`; shared inputs stay intact. Unlifted work
receives shared fixtures. Normal native acceptance runs, refreshing existing
lift evidence. Failure retains the gate/proposed inputs, restores prior hints
and clears no hold. Success resolves only the exact reviewed work record, pins
current hints/native ledger through `pipeline adopted`, then replenishes C.
Source still needs independent review and the unchanged full O0/O2 acceptance.

Registered `__ftol` sites retain ADR0006 obligations. After a matching source
test/build, missing controls enter the model `controls` stage. A typed C-only
`control-plan` supplies finite named exact substitutions, original site VAs and
expected observable mismatches. The broker pins source/hints/identity and runs
CONTROL-PLANS.md unchanged in `sensitivity`. Every helper site needs a control.
Bundle validation rechecks controls against current source and inputs; survivors
and errors remain unresolved. Independent review sees mutations and witnesses.
`controls-complete` alone establishes neither C acceptance nor full-trace proof.

Harness-capability holds stay in a separate family queue with function/byte
counts and retry conditions. Resolve them through a reviewed HARNESS.md release,
then prepare fresh eligible work. They never become generic paid retries.

## Operate and close

For final closure, pause upstream preparation and fresh owners first. Keep
review, broker and serial publication capacity available for already paid work
while the original deadline and closing reserve permit it. Drain those stages
before the final parent check and before changing frozen controller modules.
Otherwise completed reviews can be stranded behind zero publication workers,
requiring separate coordinator reconciliation and another parent gate. Retain
unresolved queues when their remaining gates do not fit; never renew the deadline.

Status includes live policy/inbox and the last timestamped service snapshot:
queue counts/candidate bytes, active operations, buffer shortages and capability
families. A held adoption stops that function, not the whole intake buffer.
Preparation/adoption publish pending receipts before launch. Restart holds
uncertain operations under run ownership; inspect side effects before explicit
reconciliation. Completed evidence remains retained; uncertain work is not replayed.

Use normal full function gates and PLAYBOOK.md parent batch checks. Report
accepted C/legacy bytes, lift-inclusive recovery, input/adoption supply, composed
calls and live integration separately. Fake-runner end-to-end regressions prove
transitions, pause/resume and isolation. Sustained throughput needs a completed
live cohort; quotas and provisioned slots are not a measured rate.
