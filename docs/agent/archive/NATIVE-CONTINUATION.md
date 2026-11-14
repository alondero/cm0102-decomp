# Native parent queue: bounded continuation

This helper persists **decisions and events**, not an autonomous launcher.
Only the parent executes its actually available native subagent tool and receives
automatic terminal notifications. Python has no native-tool access. No HTTP,
SDK, CLI daemon, broker, polling or sleeping is needed. No tool signature is
invented here. Ready actions are declarative mappings to the parent's real tool.

## Adoption status and published commands

The sole canonical writer/test owner is the assigned Sol steward, not this
author. Preserve native-flow and its earlier failed proposal/history. Reuse its
corrected positional-only event pattern and supply/headroom policy, **not** its
external transport. The reported prior nine FAKE passes are inherited context,
not tests of this candidate and not evidence of actual native dispatch.

The reviewed candidate was sealed, tested, `git apply --check`ed, frozen with
its receipts and pins and given a fresh independent
`opencode-go/deepseek-v4.1-flash` review before steward adoption; changes
invalidate that review. This queue never invokes tests, adopts source, accepts
a function or waives a gate.

The private proposal copy, its private test module and its private patch are
retained local provenance only. They are not part of this published tree, they
are not commands to run here, and the nine reported FAKE passes above are not
a public test result. The steward retains test stdout/exit code and submission
hash in its own authorized receipt paths and preserves existing files before
requesting any subsequent candidate revision.

Published helper and tracked tests, from the repository root:

```powershell
py -3.13 -m pytest -q tests/test_native_queue.py
py -3.13 scripts/cmre_native_queue.py RUN init --input CONFIG.json
```

`tests/test_native_queue.py` holds the 23 tracked cases of this queue. The
isolated candidate validation ran four focused files (165 cases); that is not
the full suite.

## Checkpoint workflow key

`scripts/cmre_session.py` accepts a fixed set of workflow keys, and
`NATIVE-COORDINATION.md` is the accepted native key. A native checkpoint must
name exactly `NATIVE-COORDINATION.md` in its `--workflow` value, for example
`--workflow NATIVE-COORDINATION.md`. This document is the queue protocol and
the default native continuation route; it is not an accepted checkpoint
workflow key, and no workflow key is added or changed by this document.

## Frozen configuration and supply

All queue commands and technical pin checks belong to the steward. A run is
finite: do not initialize another queue to renew failures, time or dispatches.
Use a fresh explicitly authorized private runtime directory `RUN`; its metadata
is separate from the one canonical writer/test lease. Inputs below are steward-
prepared JSON files in the steward's authorized private space.

These inputs are created locally by the steward and are not committed files:
the runtime directory `RUN`, `CONFIG.json`, `PREPARE*.json`, `EVENT.json` and
`HEADROOM.json`, together with the parent and validated-plan pins. A clean
clone contains the helper and its tracked tests, not a prebuilt queue.

`CONFIG.json` keys (no implied defaults):

```json
{
  "parent_owner": "native-parent",
  "steward": "repair-steward",
  "steward_model": "openai/gpt-6.1-sol",
  "parent_receipt": {"path": "PINNED_PARENT_PATH", "sha256": "ACTUAL_SHA256"},
  "validated_plan": {"path": "PINNED_VALID_PLAN_PATH", "sha256": "ACTUAL_SHA256"},
  "private_root": "ABSOLUTE_AUTHORIZED_PRIVATE_ROOT",
  "deadline": 0,
  "dispatch_budget": 8,
  "supply_limit": 4,
  "buffer_limit": 4,
  "ready_limit": 2,
  "author_cap": 2,
  "reviewer_cap": 2,
  "reserve_gib": 4,
  "worker_gib": 1
}
```

Replace deadline 0 with the original authorized finite UTC epoch deadline, all
placeholder pins with actual pins. The `parent_receipt` pin must name a current
completed valid preflight receipt: create one per AGENTS.md
(`py -3.13 scripts/cmre_session.py start`) or inherit the applicable one. The
historical receipt
`CM0102/Analysis/cmre/sessions/20261112T065701289204Z-50652-start/receipt.json`,
SHA256 `5d0a15d23918768468251580acf822de50807720a0c262f60bcc62b76227392c`,
is retained local provenance only; a clean clone has no such file and must not
treat it as a required input. Steward must validate native plan/lint, lifetime
failure allowance, original authorization, private containment and frozen
ownership before initialization. Hash checks do not establish lint success or
authorization. No start/check by workers; the existing gate owner retains all
canonical gates.

`PREPARE-A.json` event has `kind: prepare`, unique `event_id`, and `task` with:

- `id`, distinct `author`, `reviewer` (neither is steward); exact authorized
  `author_model`, `reviewer_model`;
- `max_messages` (1..30), `max_seconds` (1..1200), `remaining_failures` (0..5),
  `required_checks` (nonempty list); zero failures permits evidence-only work;
- `packets: {author: {path,sha256}, reviewer: {path,sha256}}` of complete pinned
  assignment texts; `outputs: {author: [exact private filenames], reviewer:
  [exact private verdict filename]}`; all outputs distinct and nonoverlapping;
- author output files include source/change manifest, hints/no-hint-change and
  report; those are three pinned files, not an assertion that source was accepted.

Packets must contain ROLE=WORKER, inherited preflight, exact work/private scope,
requested identity, finite message/time/failure bounds, hard-stop obligation,
no canonical commands, no tests without steward scheduling, no delegation,
no engine invention, preserved original bugs, and explicit unknowns. Private
author/review writes need no canonical slot. The validated native plan must own
these workers/tasks/paths and the exact model-class routing. Expert author needs
a distinct expert steward; reviewers independently inspect the entire bundle.

In the published tree the exact CLI is:

```powershell
py -3.13 scripts/cmre_native_queue.py RUN init --input CONFIG.json
py -3.13 scripts/cmre_native_queue.py RUN event --input PREPARE-A.json
py -3.13 scripts/cmre_native_queue.py RUN event --input PREPARE-B.json
py -3.13 scripts/cmre_native_queue.py RUN reserve --input HEADROOM.json
py -3.13 scripts/cmre_native_queue.py RUN status
```

`HEADROOM.json` is `{owner: "native-parent", available_gib: NUMBER,
sampled_at: UTC_EPOCH_SECONDS}`. Sampler source/accuracy is external evidence;
older than five seconds denies launch. This is available headroom, not total RAM;
existing workers are already reflected. One reservation batch spends that
headroom once. Parent completes/routs that batch's admissions before requesting
another fresh sample/reservation, avoiding two batches spending one measurement.

## Exact event-driven parent protocol

1. **Steward short command:** reserve emits `native_actions`, optional
   `steward_action`, and metrics. Each native action already has durable monotonic
   `id`, task/phase/owner, exact requested `model`, worker, pinned packet,
   private outputs, parent/plan pins and bounds/deadline. It is an **intent**,
   never proof of launch. Lock is released before the JSON response is returned.
2. **Parent native call:** for each newly returned intent, map its exact model,
   worker identity, packet text, private paths, inherited receipt and bounds to
   the actual available native tool. Reviewer mapping additionally includes
   `bundle` and `bundle_sha256` plus the reviewer packet; these are mandatory
   frozen review inputs, not an optional summary. Parent does no technical
   inspection. Never launch from `status`, never replay a pending intent.
   Do not call after stop/deadline or if its authorization has changed; reconcile
   rather than relaunch. No queue/global writer lease may span this native call.
3. **Admission:** parent routes actual returned subagent ID to steward, who runs
   `event --input ADMIT.json`: `{kind:"admit", event_id:UNIQUE,
   request:INTENT_ID, subagent_id:ACTUAL_RETURNED_ID}`. Do not synthesize IDs.
   Unknown observed model/usage remains JSON null; requested is not observed.
4. **Ambiguous launch:** steward records `{kind:"launch-uncertain",
   event_id:UNIQUE, request:INTENT_ID, reason:NAMED_REASON}`. Intent stays owned
   and consumes capacity; it is never automatically retried. Recover the actual
   returned ID and admit it, or have the named steward append
   `{kind:"launch-not-created", event_id:UNIQUE, request:INTENT_ID,
   steward:STEWARD_ID, evidence:{path,sha256}}` only after affirmative evidence
   **no worker was created**. That task is held; its dispatch debit is retained.
   A crash between intent and parent call is equally ambiguous; status retains it.
5. **Automatic terminal trigger:** parent routes native notification/ID and
   bounded output to steward immediately; no poll/sleep loop. Steward validates
   artifacts/pins and appends `terminal` with unique/stable native notification
   `event_id`, `request`, actual `subagent_id`, `outcome` (`succeeded` or failure),
   `observed_model` and `usage` (null if unknown). Author success includes
   `bundle:{source:{path,sha256},hints:{path,sha256},report:{path,sha256}}`.
   Reviewer success includes `verdict:{path,sha256}`. Distinct event IDs carrying
   identical terminal data are also deduplicated. Conflicting replays fail closed.
6. **Review file:** schema `cmre-native-queue-review/1`, task, author, reviewer,
   bundle_sha256, exact `pins` equal to author bundle, verdict `approve|hold`,
   `checks` equal required_checks, `unknowns` list. Only approve with no unresolved
   unknowns and exact bindings becomes ready. Changed pins reject the event;
   steward records a failure terminal/local hold, never edits the frozen bundle.
   Queue approval checks structural bindings, not truth of expert review.
7. **Replenish/drain:** after an event steward reserves again with a fresh headroom
   sample. Reviews have first claim on headroom and independent reviewer cap;
   authors fill remaining capacity while total pending/review/ready debt is below
   buffer_limit. Reviewer reservations cannot overflow ready_limit. Oldest queued
   work wins within each phase; queue ages and drain rate remain visible. Named
   task-local holds do not stop unrelated prepared work. Add frozen unrelated
   supply using prepare events within the original supply/dispatch/time budgets.
8. **One steward slot:** `steward_action` names the sole steward and frozen review
   and bundle. It reserves only a metadata obligation, not the command lease.
   Steward separately acquires the existing canonical writer/test slot for actual
   permitted operations and releases it when done. Other private work continues.
   Record `{kind:"steward-receipt", event_id:UNIQUE, task:TASK, steward:ID,
   outcome:"completed"|"held", bundle_sha256:HASH, receipt:{path,sha256}}`.
   Receipt arrival releases that slot. `completed` means external receipt, **not**
   cmre accept. Acceptance counts stay null; steward receipt rate is separate.
   Restart uses `status.steward_slot`/task pins to recover this obligation, not a
   second canonical writer or repeated gate.
9. **Stop:** event `{kind:"stop",event_id:UNIQUE}` blocks new native reservations,
   as do finite deadline/budget/cap/headroom limits. Terminal events still drain.
   Live/uncertain/overdue obligations retain IDs and are never relabelled terminal.
   Parent routes bounded cancellation/reconciliation through available native
   capabilities if necessary; queue performs none. Stop never resets failures.

Every event above is applied using the same command form:

```powershell
py -3.13 scripts/cmre_native_queue.py RUN event --input EVENT.json
```

Local held work uses `{kind:"hold",event_id:UNIQUE,task:TASK,reason:REASON,
retry_when:CONDITION}` only after live ownership is terminal. A correction uses
fresh separately authorized task/pins/review, never clears a historical hold.

## Required real native probe (parent after review; NOT performed here)

Steward prepares two independent bounded synthetic private tasks A/B, no game
logic, inherited parent and validated plan. Parent actually dispatches both
authors from persisted actions, routes returned IDs, routes automatic terminal
notifications, then actually dispatches both independent reviewers from next
actions. Steward records exact output/verdict pins, requests one ready slot at
a time and writes synthetic steward receipts. Include one local held synthetic
task and demonstrate unrelated replacement within the same finite budget.
Retain intent/admission/terminal timestamps, actual IDs, requested identities,
observed identities/usage or null, queue-age/headroom/backpressure metrics and
all holds. Real probe evidence is distinct from fake test output. No adoption or
native autoscaling claim before those actual calls/events and independent review.

That probe evidence (two synthetic tasks, four actually returned native IDs) is
separate from runtime generation, integration and acceptance evidence, and a
synthetic probe is not acceptance. This documentation repair requires no new
demo.

Missing seams are deliberately external: actual native tool execution and
automatic notifications, trusted measured headroom, native-plan semantic/lint
validation, identity/usage observation, lifetime failure accounting, substantive
independent review and actual canonical gates. This helper grants none of them.
