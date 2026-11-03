# Coordinate bounded C workers

Scope: tool-less or untrusted model workers only. Agents with shell access use
the direct C loop ([C.md](C.md)) and the delegation rule in the
[playbook](PLAYBOOK.md#delegation-only-when-the-user-authorizes-it).

Use after parent preflight when delegating C translations. The parent selects a
finite subsystem campaign through [CAMPAIGN.md](CAMPAIGN.md); workers receive one
packet and inherit preflight. Begin with two workers for an unmeasured campaign.
After a measured cohort, use the authorized runtime capacity when total accepted
throughput improves; two is a starting point, not a permanent ceiling. Include
coordination and review cost before scaling or changing model tier.

## Coordinator

1. Run `py -3.13 -m cmre task status`. Reuse unfinished packets whose input
   fingerprints still match. Inspect expired leases and recorded failures before
   assigning the same function. Completion: no duplicate live assignment.
   The Python `tasks.status()` API returns full manifests; the latest attempt is
   `manifest["attempts"][-1]`. `tasks.summary()` supplies the CLI summary's
   `latest_attempt` field. Inspect the actual schema before writing a probe.
   `cmre campaign plan MANIFEST --workers N` supplies a bounded queue and review
   backlog. Prepare a small backlog of reviewed ABI contracts so worker completion
   can refill a slot without a new open-ended parent investigation.
   Hard holds and existing unreviewed canonical sources require explicit resolution.
   Live leases across input fingerprints and submissions awaiting review or
   publication block a new claim; retain their records and reconcile them first.
   A crashed validation consumes its reserved attempt, including the fifth.
2. Prepare one accepted lift from the selected campaign:

   ```powershell
   py -3.13 -m cmre task prepare fn_004671b0
   ```

   The ID here illustrates syntax. The initial packet lane accepts functions up
   to 600 body bytes. Read the returned `packet_path`; review instruction evidence
   and ABI uncertainty before assigning to a less-capable model. The
   `simple_leaf_candidate` flag is a routing aid, not proof of a signature.
   Completion: known transformation or a specifically assigned investigation.
   For repeated worker use, retain an analyst ABI contract at
   `recovery/c/contracts/fn_XXXXXXXX.json` before preparing the packet. The schema
   is `cmre-abi-contract/1`: `target_sha256`, `address_convention` (the exact
   pinned convention), `va`, `declaration`, `calling_convention` (cdecl, stdcall,
   thiscall or fastcall), `evidence` and `unknowns`. Each evidence item contains
   `va`, hex `bytes`, and `supports`. Preparation checks those bytes against the
   primary image and fingerprints the contract. It does not prove the analyst's
   interpretation or parse C syntax; review ABI claims before dispatch, compile
   declarations in their actual use, and retain unknowns.
3. Give the worker the packet path, task ID and unique owner name. Have it claim
   the task. Its only editable file is `candidate_path`, under
   `recovery/c/tasks/`; shared headers, hints and canonical source stay fixed.
   Workers may validate independent staged candidates concurrently. Completion:
   each worker has a distinct live lease and fixed output scope.
   Use the task/worktree submission lane for bounded worker attempts. Direct
   canonical `cmre test --c` and `cmre explain --c` are coordinator diagnostics,
   not unattended worker validation: they do not reserve durable task attempts.
   Explicit-source diagnostics are fenced against concurrent claims and reject
   task and bound-worker sources; a held function can still have a separate
   coordinator diagnostic without clearing its hold.
4. Review submitted source against the instructions and the recorded report.
   Use [REVIEW.md](REVIEW.md) to reuse identity-matched evidence and add a targeted
   probe only for an uncovered obligation. Review may overlap independent worker
   translation/testing; publication remains serial and excludes machine writers.
   Publish a reviewed submission serially:

   ```powershell
   py -3.13 -m cmre task publish TASK_ID
   ```

   Publication rechecks identities, uses `cmre accept --c` semantics with a fresh
   rerun, and retains rollback evidence. After an interrupted `publishing` state,
   repeat this command to reconcile the ledger and source before doing new work.
   Completion: `published`, or an explicit rejection with old source preserved.
   If review requires a change after a successful submission, the coordinator
   reopens it with `py -3.13 -m cmre task revise TASK_ID --reason "review finding"`.
   This retains all attempts and reports, enforces the existing attempt limit,
   and clears publication eligibility. The worker claims a fresh lease and
   submits the corrected candidate as the next attempt. Input drift still
   requires a new packet; revision does not bypass frozen-input checks.
5. Record measured model/token/time information in submissions. Unknown usage
   remains null. Count parent/reviewer cost as well as worker cost when evaluating
   cheaper models. Compare matched task cohorts; avoid treating cache warmth or
   repeated exposure to a function as a model improvement. Completion: a measured
   pilot before scaling. Target fewer total tokens per reviewed acceptance and
   shorter wall time without fidelity or ownership violations.
   `py -3.13 -m cmre task metrics` reports known usage and missing measurements
   separately, including worker, coordinator and reviewer tokens. Supply measured
   usage through `--metrics` JSON: `model`, `input_tokens`, `output_tokens`,
   `coordinator_tokens`, `reviewer_tokens`, and `worker_seconds`.
   After review or publication, merge actual costs with `py -3.13 -m cmre task usage TASK_ID --attempt N --metrics JSON`; omitted fields remain unchanged, explicit null means unknown, and corrections retain before/after values in the journal.
6. Run the campaign composition check when connected sources are ready, then the
   parent session check. Completion: observed exits and a short handoff naming the
   campaign, unfinished task IDs, receipts and exact next action.
   Finish submitted and staged work before opening another broad draft. Report
   accepted source-byte changes and observed connected calls alongside total costs.

## Worker

1. Claim the supplied task:

   ```powershell
   py -3.13 -m cmre task claim TASK_ID --owner worker-a --lease-seconds 3600
   ```

   Read the packet once. It contains complete scoped instruction bytes/assembly,
   the header, boundaries, unknowns and fixed validation command. Keep the returned
   lease token for submission. Completion: scope and ABI established from evidence;
   unresolved semantic assumptions go back as `evidence-needed`.
2. Write only `candidate_path`. Preserve instruction widths, order, aliasing,
   calling convention and original bugs. Original callees remain explicit.
   Completion: a candidate, or the smallest specific evidence question.
   For adapters, alias/fault behavior or VM transfers, use the risk table in
   [C.md](C.md) to load the applicable boundary reference.
   Includes are limited to the supplied `recovery/include/` support headers;
   compiler-discovered extra includes are rejected before cache lookup.
3. Submit the candidate for the fixed O0/O2 checks:

   ```powershell
   py -3.13 -m cmre task submit TASK_ID --owner worker-a --token TOKEN --summary "instruction-backed translation"
   ```

   Read the report at `latest_attempt.report_path` on failure; repair the first
   mismatch. Five attempts is a hard limit. Submit `--outcome evidence-needed` or
   `--outcome harness-defect` with the reproducer when those apply. Completed
   experiments remain in the journal. Completion: `submitted` or an escalation;
   a worker never claims acceptance from its own success message.
4. Return task ID, result, report path and any unresolved boundary. The coordinator
   performs publication and the final session gate.

## Enforcement and limits

Command leases exclude unrelated CLI/API writers while allowing a supervised
job's own pool workers. Publication is serialized; staged tests can run in parallel.
Task fingerprints reject changed inputs and reports; expiring ownership prevents
duplicate claims. These are trusted local-process controls, not a filesystem
sandbox against arbitrary shell writes. Give workers the narrowest tool scope the
client supports and review their actual diff.

The guard understands Claude edit/shell and Codex `exec_command`/`apply_patch`
payloads. Claude's hook is wired; Codex hook installation depends on the host's
supported hook schema and is not asserted by this workflow. Core CLI publication
checks do not depend on either hook. A launcher can set `CMRE_AUTHORIZED_PATHS`
to a JSON list of explicitly user-authorized tooling paths for that session; it
never overrides preserved/generated paths. Do not use that variable to authorize
worker harness edits.
