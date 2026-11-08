# Run a 150-minute provider campaign

Use this workflow for an explicitly requested timed campaign using MiniMax and
OpenCode Go. Optimize reviewed accepted C bytes and connected behavior per unit of
coordinator effort. [CAMPAIGN.md](CAMPAIGN.md) owns dependency closure;
[WORKTREES.md](WORKTREES.md) owns isolated C workers;
[THROUGHPUT.md](THROUGHPUT.md) owns measurement. The fidelity rules and validation
budget in [PLAYBOOK.md](PLAYBOOK.md) remain authoritative.

## Roles and authority

| Role | Default assignment | Owns | Returns |
| --- | --- | --- | --- |
| Coordinator / family owner | Codex | Campaign selection, original-instruction contracts, difficult ABI/x87/alias/fault questions, shared inputs, independent final review, serial acceptance, gates and retro | Accepted evidence or a named unresolved obligation |
| Translator | MiniMax initially | Exact eligible functions with prepared evidence; edits only the assigned C files | Broker report path and concise instruction rationale, or an evidence-bound hold |
| Preliminary reviewer / second worker | DeepSeek Go initially | Independent source/evidence review, second opinions, or disjoint translation when supply permits | Exact source/report identities, discrepancies and evidence; never acceptance |
| Input proposal author | Either external provider, as a bounded trial | Private original-derived input/layout proposals for one blocker family | Reproducible evidence and proposed fixtures, with unknowns explicit |

Start with at most **two external provider workers total**, including reviewers
and proposal authors. Role assignments are hypotheses: both providers passed small
translation pilots; neither has established broad input-authoring or review
quality. The coordinator independently checks their evidence before adoption.
Use a different author for review; a worker cannot approve its own candidate.
Increase concurrency only after a measured cohort shows prepared supply and review
capacity, within the user's authorization. Use native helper agents only when
the task or selected skill authorizes them; record their costs when observed.
They do not stand in for the requested provider.

The worktree broker covers accepted lifts without accepted C, at most 600 body
bytes. Larger, held and unlifted functions use the coordinator's selected C,
triage or control workflow. Input authors produce private proposals; they do not
edit canonical hints, change the harness, claim C tasks or run `worktree test` for
ineligible functions. The coordinator adopts reviewed inputs at a boundary and
validates affected evidence. Every writer has explicit ownership and paths.

## Start and freeze the run

1. Resume the inherited green parent receipt, or start once when none applies.
   Inspect live claims, required gates and current source hashes; preserve other
   work. Workers inherit this intake. An active required writer gate pauses
   dispatch. Completion: a valid parent and no unresolved mandatory dispatch gate.
2. Set a **150-minute total wall budget**, including setup, failures, review,
   validation, retro and handoff. Record UTC start/deadline and monotonic elapsed
   time. Setup time does not disappear when a provider or permission check fails.
3. Select one observable campaign and a finite function set. Inspect its dependency
   report and `cmre next --c --min-bytes 129`. When substantial eligible work is
   absent, allocate effort to one shared input/blocker family. Reuse reviewed
   packets and named retry conditions rather than retrying known failures.
   Completion: exact IDs, intended dependency closure and an observable checkpoint.
4. Keep a run index under ignored `CM0102/Analysis/cmre/campaigns/RUN_ID/` linking
   the manifest, parent, frozen identities, role assignments, worker specs/PIDs,
   source/report hashes, attempts, gate receipts and retrospective. This index is
   evidence navigation; `cmre work` and broker claims remain scheduling authority.
   Start finite `cmre pilot` measurement before work on its selected functions.
   Already accepted functions earn no new-byte credit. If selection changes, use
   another finite pilot and retain both, rather than rewriting its baseline.
5. Verify client/version, account and exact model through the selected endpoint.
   Follow [PROVIDER-WORKERS.md](PROVIDER-WORKERS.md) for actual headless read/write
   permission smoke checks and local credentials. Freeze harness, headers,
   comparison policy and declared inputs before recovery. A needed harness change
   becomes a separate release under HARNESS.md; it does not waive revalidation.

## Spend the budget

These are planning checkpoints. Adapt the middle phases to supply while preserving
the closing reserve and finishing each current bounded attempt honestly.
If the campaign finishes or becomes externally blocked earlier, close and run its
retro earlier; the budget does not require idle time or repeated blocked attempts.

| Elapsed minutes | Work | Required result |
| --- | --- | --- |
| 0-15 | Intake, campaign/deadline, provider and permission checks | Frozen scope, working clients, measurement and role assignments |
| 15-35 | Resolve the selected input/ABI/sensitivity obligation | Reviewed improvement or a precise hold; prepared translation supply where possible |
| 35-110 | Bounded assignments, independent review and serial acceptance; composition when ready | Accepted source and dependency progress, all attempts retained |
| 110-130 | Stop dispatch, drain workers, reconcile claims, finish review/accounting and required gate | Terminal worker receipts and honest passed/pending gate states |
| 130-150 | Coordinator invokes the requested retro, then writes the next handoff | Evidence-based improvements and one bounded next task |

Follow the existing batch gate at about 20 acceptances or one hour, whichever
comes first; a 150-minute campaign can contain several batches. Worker completion
does not trigger another full suite. Never run a closing check over live acceptance
or changing declared inputs. Preserve headroom for required final validation.

Dispatch only tasks whose measured cost fits before the drain phase. Bound each
process by the existing 30-step/900-second adapter limits and the remaining work
window; preserve the workflow's cumulative failure ceiling, including any smaller
remaining allowance. At minute 110, stop new dispatch, collect or cancel existing
workers and verify process-tree exit before releasing/reassigning claims. Do not
start a long control sweep or inventory gate merely to fill time.

If an already-running required gate exceeds the reserve, retain its receipt/PID
and unfinished obligations. Write a provisional retro/handoff with that limitation;
do not label the run green or claim a completed gate. No new engine work starts
after the wall budget. A later continuation reconciles the actual terminal result.

## Keep coordinator effort small

- Supply only the selected workflow, complete original evidence, relevant caller
  and callee contracts, required header and exact writable paths. At most three
  related functions per fresh assignment; never refill one context indefinitely.
- Translators stop on the first broker pass and return its path. The coordinator
  hashes reports; report hashing is not an impossible worker-side requirement.
  Unknown ABI or missing witnesses produce a hold, not invented behavior.
- Use preliminary review to focus final instruction review. Passing tests alone,
  model agreement and a clean process audit never establish faithful recovery.
- Move startup, waiting, file hashing, usage ingestion and restart reconciliation
  into deterministic tooling. Preserve failed runs and stop dispatch when review
  backs up or a provider is limited. Change provider only explicitly and record it.
- If work is scarce, use bounded private input proposals or review existing held
  evidence. Resolve a connected campaign's blocker before adding translators.

The implemented `scripts/cmre_provider_worker.py` supervises **one process**. It
does not select/claim work, schedule a 150-minute campaign, adopt hints, accept C,
invoke skills or conduct a retro. Today the coordinator runs this workflow using
existing commands. A future thin batch driver may automate orchestration, reusing
the same registries and adapter; its global deadlines, restart recovery and handoff
must be tested with fake processes before unattended use. Building that driver is
a separate tooling task, not a prerequisite that consumes this whole campaign.

## Measure the whole campaign

Record total wall time, setup/repair, input work, worker execution, coordinator
active time, review, gates and retro separately where observed; overlapping phases
must not be summed as elapsed time. Include failed/held calls and review fixes.
Report accepted C/legacy functions and body bytes, lift-inclusive progress,
newly exercised composed calls and live integration separately. Unblocked supply
and reviewed evidence are intermediate outputs, not accepted source.

Keep fresh input, cached reads/writes, text output and separately reported reasoning
distinct. Preserve raw exports; unfinished message zero placeholders are unknown.
Record quota windows and remaining usage when available. Client-estimated cost is
not a cash charge. Missing Codex/provider usage remains null; do not attest
`--usage-complete` when any included usage is unknown. Report accepted bytes per
coordinator minute only when coordinator active time was actually measured.

Finish pilots after review/acceptance and usage reconciliation, before the closing
gate; retain whole-campaign timing as well. Different functions, prompts, fixtures,
review rules or validation costs yield configuration comparisons, not model
speedup claims. Evaluate Go by additional accepted progress or coordinator effort
saved, including its useful capacity when MiniMax is constrained.

## End with the user-requested retro

The **coordinator** applies the installed `retro` skill after recovery stops; the
user explicitly requested this end-of-run step. Discover its `SKILL.md` rather
than assuming a worker has a skill tool. Known local locations are
`~/.claude/skills/retro/SKILL.md` and `~/.agents/skills/retro/SKILL.md`. Read it and
its writing guide, then use the run's primary receipts/logs. Its manual-invocation
metadata is not permission for external workers to self-invoke it or edit policy.
If unavailable, retain the finding and report retro incomplete; do not invent it.

Apply its categories: navigation, automated checks, reviewer standards, steering
size/no-ops, tool economy and information access. Check existing guardrails first.
For each ranked finding retain the evidence, cause/hypothesis, proposed change,
expected benefit, owner, validation and success criterion for the next run.
Prefer a deterministic check for a mechanical failure over another prose rule.
Choose at most three concrete improvements; distinguish measured facts from ideas.

Save `retro.md` and its cited artifact paths beside the run index. Keep proposals
outside frozen inputs. Post-gate edits to any declared input, including docs,
require their normal validation, so schedule them as bounded next work rather
than silently invalidating the completed gate. Follow the skill's presentation
step in the final response. Replace the local next-session prompt with at most
40 lines linking this pack, latest gate/PIDs, source hashes, unresolved failures
and one next task. Completion: retained required independent reviews, truthful gate status,
pilot/accounting records, retro findings and a resumable handoff.
