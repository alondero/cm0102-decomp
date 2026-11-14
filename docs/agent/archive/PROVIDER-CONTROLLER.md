# Provider-owned campaigns

For authorized coordination-only native work, use
[NATIVE-COORDINATION.md](NATIVE-COORDINATION.md) instead of this controller's role
allocation and dispatch procedure. Parent technical work and human/Codex exception
escalations below route to an authorized expert worker, never back to the parent.
The native plan lint does not control or launch this standalone controller.

Use this workflow when DeepSeek/MiniMax should own recovery decisions and repair
loops, or when starting the controller from a terminal. The controller uses
OpenCode headlessly; an interactive Codex or OpenCode session is optional.
For continuous replenishment and independently adjustable model/execution stages,
use [PIPELINE.md](PIPELINE.md). Initialize with `--pipeline`; its reviewed input
promotion, source-site controls and broker queues handle routine handoffs.

## Ownership

| Role | Owner | Responsibility |
| --- | --- | --- |
| Input investigation and campaign decisions | DeepSeek | Read original evidence, choose probes, diagnose results, revise inputs, propose a frozen bundle |
| C author | MiniMax | Recover the assigned routine, request builds and the full gate, repair failures |
| Independent reviewer | Other provider | Review the exact source, emitted code and evidence; request revisions or name a hold |
| Supervisor and broker | Python | Durable queue, scope, model identity, time budget, artifacts, existing CMRE gates and serial acceptance |
| Exception escalation | Human or explicitly selected Codex session | Resolve a named infrastructure defect or evidence question from a retained hold |

The providers receive typed actions and private files. Each action gets a fresh
scoped smoke, an exact observed model and retained receipts. The broker performs
original-only probes and normal builds. C tests and acceptance use the existing
task API. A reviewer uses a different actual model; source and reports are frozen
before review. Reviewed input evidence earns input progress, separately from C
acceptance, composition and live integration.

## Start from PowerShell

1. Complete the parent preflight and required recovery intake. Prepare a local
   manifest and provider profiles under `CM0102/Analysis/`; credentials stay in
   the existing client credential store. Profiles map `deepseek` and `minimax`
   to local worker specs containing executable, exact model, client version and
   environment. Pin the authenticated endpoint/model through provider discovery.
   Completion: green parent, fixed execution identity, exact allowed functions,
   hash-pinned seed fixtures and an evidence-derived goal for each job.
   Have the providers check that the required PCs and coverage threshold can
   describe one invocation. Separate mutually exclusive branch cases. An
   unproven feasibility claim stays unknown during investigation.
2. Initialize the queue. This freezes inputs and starts no experiment clock:

   ```powershell
   py -3.13 scripts/cmre_provider_campaign.py init RUN --manifest MANIFEST.json --profiles PROFILES.json --parent PARENT_RECEIPT
   ```

   Completion: initialized durable jobs and matching model/controller identity.
3. Run the queue. The first run persists its start and deadline; resuming uses
   the same deadline and inputs:

   ```powershell
   py -3.13 scripts/cmre_provider_campaign.py run RUN --profiles PROFILES.json --parent PARENT_RECEIPT
   py -3.13 scripts/cmre_provider_campaign.py status RUN
   ```

   Keep the foreground process alive, or launch it using a hidden Windows
   process with redirected logs. The recorded PID must be the Python controller
   itself: launch `py -3.13` directly rather than through a wrapper process, and
   stop that PID to stop the controller. The worker adapter removes the enclosing
   session's `OPENCODE`, `OPENCODE_CONFIG`, `OPENCODE_CONFIG_CONTENT`,
   `OPENCODE_SESSION_ID` and `OPENCODE_TERMINAL` variables before applying a
   profile environment, so a controller started from inside an OpenCode session
   no longer hands its own client configuration to provider children. `--once`
   executes at most one dispatch stage for inspection. Completion: terminal jobs, a
   retained hold, or the persisted deadline. Without isolated slot profiles, only one stage per provider role
   runs at a time: two same-provider OpenCode clients share one client state
   database and can fail with `database is locked`, so a higher manifest
   concurrency pairs an author with the independent reviewer instead of two
   authors. Interruption retains the uncertain stage and holds that job for
   reconciliation; inspect its receipts before any explicit retry.
   For a known clean, terminal actor failure, prepare a retry without inference:

   ```powershell
   py -3.13 scripts/cmre_provider_campaign.py retry RUN --job JOB --reason "Inspected terminal actor failure" --profiles PROFILES.json --parent PARENT_RECEIPT
   ```

   This preserves the original deadline, counters, evidence and bundle. It
   refuses expired or changed identities, active controllers, uncertain
   interruptions, executor/finalization failures and deliberate reviewer holds.
   It does not renew the automatic retry budget. Run the queue separately after
   preparation. Old experiment queues are expired; a new session creates a new
   reviewed plan after its preflight rather than reopening their history.
4. Drain provider processes and collect usage. Run the parent check at the
   handoff boundary. Report accepted C/legacy bytes, lift-inclusive recovery,
   original input evidence, composition and integration separately. Record
   failed scopes, provider repairs, independent review outcomes and Codex
   interventions. Unknown usage remains unknown. Completion: pytest and verify
   both exit zero, or explicitly reported failures/unavailable checks.

## Manifest

```json
{
  "schema": "cmre-provider-campaign/1",
  "duration_seconds": 10800,
  "concurrency": 2,
  "roles": {
    "deepseek": {"model": "EXACT_DISCOVERED_DEEPSEEK_MODEL"},
    "minimax": {"model": "EXACT_DISCOVERED_MINIMAX_MODEL"}
  },
  "jobs": [{
    "id": "contract-input",
    "config": {
      "function": "0x004d1600",
      "mode": "input",
      "owner": "deepseek",
      "reviewer": "minimax",
      "goal": "Reach the named original formula path using instruction-derived data",
      "allowed_functions": ["0x004d1600"],
      "seed_fixture": {"path": "ABSOLUTE_LOCAL_FIXTURE", "sha256": "FIXTURE_SHA256"},
      "required_pcs": ["0x004d16ea"],
      "require_no_imports": true,
      "smoke_timeout_seconds": 180,
      "max_rounds": 30
    }
  }]
}
```

Choose the duration explicitly for the authorized run. C jobs use `mode: "c"`;
their default owner is MiniMax and reviewer DeepSeek. Existing canonical source
requires separate reconciliation before queueing. An explicit `max_body_bytes`
can widen the default 600-byte task packet lane without changing the full gate
or five-attempt lifetime budget. Accepted lift evidence remains required for C
tasks; original-only input jobs can investigate larger or unlifted functions.
To resume retained C, set `candidate_seed` to a `{ "path": "ABSOLUTE_C_FILE",
"sha256": "SOURCE_SHA256" }` descriptor. Initialization copies the verified
bytes into the private job candidate without changing canonical C. If canonical
C already exists, its hash must equal the seed hash; otherwise reconcile those
different versions first. An existing private candidate is never overwritten.
Seed bytes and C dispatch readiness are checked before queue creation. A failed
copy retains the partial queue; the completion marker must be present before
`run` can dispatch. The marker records initialization, so later private repairs
need not match the original seed hash. Reopen only this exact owner's submitted
tasks automatically. Foreign or older basename-only submissions require explicit
coordinator reconciliation after checking the prior owner has stopped.
Set `smoke_timeout_seconds` to fit complete packet reads (1..180 seconds); the
default is 60. Review packets omit duplicate accounting and retain full source,
emitted code, fixtures and evidence. The complete original bundle stays pinned.
Named sensitivity controls require a hash-pinned `control_plan` and
`require_controls: true` in the finite workflow. Pipeline jobs can propose a
typed `control-plan`; the broker pins and runs it through the same validator.
Unresolved source-site controls remain an explicit obligation.

For OpenCode 1.18.3, cap each provider turn with
`OPENCODE_EXPERIMENTAL_OUTPUT_TOKEN_MAX` in its local profile environment. The
similarly named `...OUTPUT_TOKEN_LIMIT` is absent from the
[version's runtime flags](https://github.com/anomalyco/opencode/blob/v1.18.3/packages/opencode/src/effect/runtime-flags.ts).
Choose a small cap for one-action turns and a separate larger build-authoring
profile. Retain actual reasoning/output usage and length-limit failures.

A terminal `length` finish holds the job without an identical automatic retry.
Inspect the retained task and packet before preparing a new bounded plan. Context
preflight also stops oversized packets before either paid stage; its
`context-preflight.json` records counts, limits and the decision. Preserve the
complete source and emissions, and replace duplicated raw traces with hash-linked
proof projections. Held packet files remain available; no evidence is truncated.

Input readiness requires an original return satisfying the assigned PCs,
coverage and import policy, followed by independent review. It establishes a
local evidence bundle; canonical hints are adopted through the normal input
workflow at a batch boundary. Keep all witnesses, failed attempts and receipts.

## Operational limits

The official CLI defaults to two workers. For a prepared replenishing pipeline,
pass `--slot-profiles SLOTS.json` to init, run and retry. The provisioning report
supplies hash-pinned profiles with disjoint XDG data/state/config/cache trees;
role_capacity in the manifest cannot exceed those actual slots. Four profile
pairs permit concurrency8; up to eight pairs/concurrency16 are supported. The
pool's profiles, executable bytes and controller modules are pinned in identity.
Record user-authorized capacity in the allocation before dispatch. Without slot
profiles the existing two-worker limit remains. See [PIPELINE.md](PIPELINE.md).
The controller refills free workers immediately, drains reviews first, bounds
owner work in progress and pauses input dispatch when adoption backs up.

The persisted deadline stops new dispatch and bounds provider stages. An
in-flight broker build or full task test drains under its own limits and can
finish after that deadline. Reserve closing time; hard broker cancellation is
not implemented. Acceptance still requires the full normal gate.

Use the typed action protocol, including its metadata constraints: omit an
unused evidence `start` instead of writing `null`; keep probe names lowercase
and at most 63 characters. A one-time initial action must be conditional on
current results, not an unconditional instruction in the immutable goal.
Cross-cohort artifact references need verified relocation; they are not portable
action IDs. Queue planning and such transfers remain provider/operator work.

Input evidence is demonstrated separately from a complete C pilot. The controller
retains raw witnesses, full builds and normal task gates, but four reviewed input
cases do not establish new C acceptance, a standalone game or live integration.
