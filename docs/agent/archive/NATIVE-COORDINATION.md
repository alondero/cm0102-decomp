# Coordination-only native workflow

Single authority for explicitly authorized **coordination-only** native mode,
including the next session directed by the November 12 handoff. This overrides
parent-execution wording in task/provider workflows, not fidelity, independent
review, existing cmre gates, leases or lifetime failure budgets. The completed
150-minute trial stays closed; this mode starts no new timed campaign.

## Default native continuation route

Authorized native continuations use the durable prepared queue in
[NATIVE-CONTINUATION.md](NATIVE-CONTINUATION.md) by default; that document
holds the helper CLI and the event protocol. The parent dispatches with the
native subagent tool it actually has and receives its automatic terminal
notifications; the Python helper stores decisions and events only, so it starts
no worker, opens no transport and names no tool it does not have.

Flow: frozen preflight and native plan with private packets -> steward short
queue commands -> parent native call -> admission of the actually returned ID
-> automatic terminal routing -> independent review -> one Sol steward ready
action.

- The default route authorizes no recovery campaign and resets no failure
  budget.
- The queue stops new reservations at the run deadline and classifies a late
  completion as a budget breach; admission only binds the actual returned ID.
  A running worker is not killed by it: over budget the parent stops new
  substantive assignment and asks the active worker to stop or freeze
  cooperatively. Report no hard kill and no enforced in-flight timeout. The
  provider campaign deadline fix bounds provider invocations; it is not a
  native in-flight cancel.
- Task-local and source-local holds, applied only after live ownership is
  terminal, return their slot to unrelated eligible supply. Author and reviewer
  caps are bounded separately by measured resources, ready buffer and queue
  age.
- No queue or global writer lease is held while a worker runs; the queue lock
  is released before each response returns.
- The F01 headroom sample-consumption fix is adopted in `cmre/native_queue.py`
  (sha256 dd13878e4b61ef7805cf81a626ce5a851e74d1bcbd0cde9a36db6b40aa5a6244):
  one measured sample funds at most one batch, the durable
  `headroom_consumed_at` watermark rejects a reused or older sample, and a
  fresh sample with a different timestamp requires the parent to admit or
  reconcile the previous batch before measuring again. Its tracked tests pass
  (23 cases in `tests/test_native_queue.py`, sha256
  20ae62d9843c0fde6b83b26be1724c9307b13b37803e4ed038016246d12db44f), and the
  latest full-workspace check is recorded GREEN 2873+4 with `cmre verify` exit
  0 and status 0. Steward adoption and gate receipts are local provenance. No
  global memory controller and no sustained autoscaling is claimed, and no
  native in-flight cancel is claimed.
- An unknown capacity reading stays recorded as unknown; it does not raise a
  blanket global stop. Deadlines stop new reservations for the affected run.
- Review covers substantive artifact and policy changes; an operational breach
  is logged once.
- Manual coordination starved on serialized hand-offs; the real two-task
  native probe shows the queue path end to end, accepted no function and proves
  no autoscaling.

## Authority

The parent communicates with the user, schedules/replenishes bounded assignments,
sets priorities, ownership and budgets, routes escalations, and reads bounded
receipt/verdict metadata. Every substantive operation belongs to a worker:
evidence analysis, source/hint/tooling/docs corrections, input construction,
emitted-code inspection, technical review, acceptance, tests and session gates.
The parent ensures terminal green through receipts, never gate execution or
internal technical adjudication.

| Class / exact authorized model | Work |
| --- | --- |
| Expert: `openai/gpt-6.1-sol` | Architecture, ambiguous ABI, arbitration; independent acceptance/integration steward and session gates |
| Senior: `opencode-go/deepseek-v4.1-flash` | Hard investigation, inputs, senior review, tricky repairs |
| Routine: `minimax-coding-plan/MiniMax-M3.1-Flash` | Bounded routine translation, mechanical repairs, projections, docs chores |

Class is routing, not evidence. Each bundle has distinct author and reviewer;
its expert steward is not its author. Expert-authored bundles need a different
expert steward. A reviewer may steward a bundle they did not author. Disagreement
goes to an expert worker against original evidence, not to the parent. Corrections
invalidate affected review pins and require renewed independent review.

## Dispatch and completion

1. Assign one expert preflight/gate steward. It runs `start` once only when no
   applicable completed parent exists. Every other dispatch says `ROLE=WORKER`,
   inherits the same pinned completed preflight, and runs no start/check/checkpoint.
   Resumes keep that parent. The steward handles receipt publication and final
   gates under the playbook validation budget. Completion: inherited receipt and
   explicit worker role in every assignment.
2. A worker prepares the plan below; the steward runs
   `py -3.13 scripts/cmre_native_plan.py PLAN.json` before dispatch and after changes.
   Invalid plans stop dispatch. Retain plan and bounded result in the ignored run
   index. Record requested exact model IDs; absent observed identity/usage stays
   unknown. Completion: valid declared ownership, bounds, paths and receipt pins.
3. Schedule parallel private author/reviewer lanes against measured supply, review
   backlog and resource headroom. Initial **4 routine + 2 senior + 1 expert steward**
   is a suggestion, not an entitlement, fixed pool or speedup claim. Replenish
   independent review without parent technical inspection. Use exact private paths
   or isolated worktrees and preserve unrelated user work. A live canonical gate
   freezes its declared inputs, not all private investigation. Completion: frozen
   bundles or evidence-bound holds, retaining every attempt and cumulative failure,
   including private-source failures.
   Expert-authored tasks require a second expert worker distinct from the steward;
   otherwise hold them. Stop author dispatch when prepared supply is exhausted or
   review/acceptance backlog exceeds the single steward's measured drain capacity.
   Record held allocation `reason` and `retry_when`; parent never fills the gap by
   authoring and no second canonical writer is added.
   The prepared supply, local holds and reviewer backlog are the queue's durable state in native mode.
4. One exclusive serialized canonical writer/test slot belongs to the expert
   steward. Authors prepare privately; shared-workspace CLI tests still need this
   scheduled slot. Steward schedules/executes tests, adoption, acceptance
   and validation; launch no competing writer or duplicate test. Review pins exact
   source, hints and report; retain reviewer verdict and identity. Inspect full
   emitted code where required. Tests/model agreement alone never approve bundles.
   Changed bundles return to review. Completion: steward's terminal acceptance/hold
   receipt, never author selfapproval.
5. Drain at the authorized boundary. Steward reconciles live worker obligations,
   runs the closing check and returns bounded exit/status/path metadata. Parent
   schedules correction workers for failures and ensures terminal green or reports
   the exact unresolved gate; old green is not current green. The steward updates
   the handoff preserving source/gate/hold pointers and this mode. Completion: terminal
   worker receipts and truthful gate status, no hidden live job. Only the assigned
   steward publishes checkpoints/handoff, after live gate termination and while
   owning the serialized slot: checkpoint takes the command writer lease too.

## Plan and allocation contract

The helper's constants and validation errors define the exact schema. Plans use
`cmre-native-coordination/1`, mode `coordination-only`, authorization reference,
pinned `parent_receipt` (`path`, `sha256`), `workers`, complete `roles`, `steward`,
`writer_slots: 1`, and bounded `tasks`. Workers declare unique ID, class, exact model,
`role: WORKER`, `preflight: inherit` and the same parent pin. Assign every role in
`ROLE_CLASSES`; preflight/integration/acceptance/validation share the expert steward.
Tasks name author, reviewer, steward, difficulty (`routine`, `hard`, `expert`), exact
private writable paths and attempt/context/time bounds. `planned` tasks have empty
pins; `ready` tasks pin source, hints, report and verdict. Ready means pinned for
review/adoption, not accepted. Tooling tasks pin a source change manifest, test
report and explicit no-hint-change artifact. Pins prove bytes, not verdict truth.
Use the same four keys for tooling: `source` pins the change manifest, `hints` the
no-hint-change artifact, `report` the test report, and `verdict` independent review.
The verdict file uses `cmre-native-review/1`, matching `task`, `author`, `reviewer`,
`verdict: approve` and `pins` for those exact source/hints/report files. Lint verifies
that binding, not whether the named reviewer actually performed adequate review.
`bounds` has integer `max_messages` (1..30), `max_seconds` (1..86400), and
`remaining_failures` (0..5). The steward checks remaining allowance against retained
lifetime failures before dispatch; lint cannot verify that assertion or enforce
runtime deadlines. Zero remaining failures permits evidence-only work, not tests.
Private declarations must resolve below `CM0102/Analysis/cmre/` in the checkout
running the checker: canonical paths, escapes, directories and nesting are rejected.
For external worktrees, run that checkout's checker against its own private analysis
paths; canonical adoption is a separate steward operation, not an author path grant.

Reuse `cmre-worker-allocation/1` with `mode: coordination-only` and embedded
`native_plan`; existing bounded assignment receipts remain navigation records.
Checkpoint validation calls this checker and snapshots the plan, not a scheduler.
Empty assignments still require `reason` and `retry_when`. Re-pin explicit replacement
allocation after bundle changes; never relabel old review. Provider/solo semantics
stay unchanged. Generated coordination-only handoffs retain the policy pointer.

## Enforcement limits

Lint rejects incomplete roles, unauthorized declared classes, selfreview and
author-steward conflicts, multiple writer slots, missing/drifted pins, invalid
inherited preflight, overlapping declared private files and missing bounds.
Existing cmre leases/gates retain their enforcement. Native prompts, roles, scope
and time bounds are cooperative: lint enforces no filesystem permissions, actual
caller/model identity, session behavior, containment, semantic independence,
complete evidence or gate result. It neither launches workers nor grants permission.

It enforces no native cancellation either: a running native worker ends when it finishes; asking it to stop is cooperative and not guaranteed.
The canonical `accept` and session `check`/`checkpoint` commands take writer leases.
Ordinary CLI `test`, including private `--source`, does not: its slot ownership is
cooperative scheduling, not an existing lock or native permission boundary.
