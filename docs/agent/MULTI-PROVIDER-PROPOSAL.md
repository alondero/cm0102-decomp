# Proposal: workers across agent providers

Status: design for a future implementation; no launcher or provider configuration
was installed by this investigation (2026-10-23). Existing workflows remain
authoritative. Provider evidence is in [PROVIDER-RESEARCH.md](PROVIDER-RESEARCH.md).

## Recommendation

Keep Sol or Opus as the single integrator. Launch two independent OpenCode
processes using a verified MiniMax provider/model as translators. The integrator
initially reviews their output; add a dedicated reviewer only if review becomes
the bottleneck. MiniMax Code (`mcode`) is a real first-party alternative, distinct
from `mmx`; compare it in a later matched cohort using the same protocol. OpenCode
goes first because it is already installed, not because quality superiority has
been established.

Separate three choices: the orchestrator model, the worker client (OpenCode,
Claude Code, etc.), and the inference provider/model. Cross-provider workers are
external processes with files and receipts, not necessarily native subagents in
the orchestrator's UI. MiniMax's coding subscription is also distinct from a
coding client; verify the account endpoint and entitlement before a paid pilot.

## Reuse what exists

- [PARALLEL.md](PARALLEL.md): one integrator, disjoint claims, one shared checkout,
  worker tests and serial acceptance. Worktrees lack ignored game/analysis assets.
- [PLAYBOOK.md](PLAYBOOK.md): at most three functions per worker, one family or
  subsystem, fresh context at the batch boundary, parent-owned final check.
- [THROUGHPUT.md](THROUGHPUT.md): matched cohorts and `cmre pilot` usage accounting.
- [TASKS.md](TASKS.md): staged candidates for tool-less or untrusted workers.
  Do not turn this into a competing queue for ordinary shell-capable translators.

`cmre/work.py` already serializes claims through an OS lock and atomic writes.
Claims coordinate cooperative writers; they do not sandbox shell access.
The missing component is process supervision and provider-result normalization,
not another scheduler or an autonomous agent framework.

## Small implementation boundary

Propose a local launcher with an OpenCode adapter first. This interface is a
design, not an existing command. Its operations would be launch, status, collect
and cancel. Keep queue selection, review and acceptance in existing `cmre` flows.

The integrator claims a bounded set through `cmre` under a unique worker owner,
then launches that worker with exact IDs. The worker does not independently
select more work. A launch record under ignored `CM0102/Analysis/` would contain:

- Run/owner ID, inherited preflight receipt, exact function IDs and allowed files.
- Target, harness and supplied source/header fingerprints, plus a snapshot of
  assigned-file existence and hashes. This checkout can contain uncommitted work;
  a Git commit alone cannot identify inputs or attribute a worker's changes.
- Client version, requested and observed provider/model, provider session ID,
  process identity, timestamps, timeout and attempt budget.
- Prompt, stdout/stderr paths, exit status, changed-file hashes, validation report
  locations, unresolved evidence questions and measured usage (null if unknown).

The prompt names AGENTS.md, C.md, the exact support header and evidence/brief
paths, representative and member differences. It permits edits only to the
assigned C files, permits their `cmre test --c` calls, and forbids acceptance,
shared-header/harness changes, commits, publishing and recursive delegation.
Thirty assistant messages is the existing bounded-attempt limit; the launcher
also needs a wall-clock timeout because client turn accounting can differ.

Use argument arrays and explicit working directories, no shell-interpolated
prompts or credentials. Launch Windows processes hidden with redirected logs.
Keep credentials in each client's supported credential mechanism. Disable session
sharing and unnecessary plugins. Verify headless permissions before launching;
an interactive permission prompt must become a reported blocked run, not a hang.
Do not assume Claude/Codex hooks cover OpenCode. Client permissions and diff
audits are useful controls, not an OS filesystem sandbox.

## Completion and recovery

A zero process exit or a worker saying "pass" is not acceptance. Collect the
source and actual test evidence, inspect the assigned-file diff against original
instructions, then let the integrator accept serially. No worker can mark itself
recovered. Retain explicit evidence-needed and harness-defect outcomes.

On timeout or cancellation, stop the recorded process tree and verify it has
exited before releasing or reassigning work. Reconcile claims against process
records and source files on restart; lease expiry alone is not proof of exit.
Start with standalone processes; if a persistent OpenCode server is introduced,
use its session-abort mechanism and verify inactivity as well. Killing an attached
client does not establish that server-side work stopped.
Written WIP stays owned/pending review until the integrator explicitly disposes
of it. Never blindly delete WIP or rerun a possibly live worker.

Rate limits should pause dispatch for that provider with bounded backoff. Avoid
automatic model/provider fallback: it changes both the experiment and billing.
Backpressure should stop new dispatch when review cannot keep pace. Harness
revalidation and other exclusive machine jobs pause recovery dispatch entirely.

For an untrusted client, use the existing staged task lane with narrower tools
instead of granting general shell access to the shared checkout. Treat that as a
separate adapter, not an excuse to weaken the direct-worker ownership contract.

## Pilot and rollout

1. After the current exclusive job finishes, verify MiniMax authentication,
   exact model ID, endpoint, account entitlement and noninteractive permissions.
   Run one disposable scoped smoke task to verify event parsing, file limits,
   cancellation, usage capture and absence of session sharing.
2. Measure a finite baseline and a comparable two-worker MiniMax cohort with
   `cmre pilot`, using disjoint functions matched by family, size, ABI and hazards.
   For a model comparison, keep the client, two-worker concurrency and supplied
   context fixed. For a concurrency comparison, keep client/model/context fixed
   and compare one versus two workers. Compare OpenCode versus mcode separately
   with the same MiniMax model and concurrency. If these controls are unavailable,
   report only a combined-configuration result, not a model or client advantage.
   Start with well-understood integer routines. Escalate ambiguous ABI, x87,
   SEH and harness defects to the integrator rather than spending repeated retries.
3. Count all failed attempts and coordinator/reviewer cost. Compare accepted
   bytes/hour, review corrections, elapsed time and known usage/cost. Subscription
   quota consumption and API spend are separate measurements; unknown is not zero.
4. Scale to four workers only if reviewed acceptance throughput improves without
   fidelity/ownership regressions. Add one reviewer if the review backlog grows.
   Keep CPU headroom for acceptance reruns; CPU count is not a provider quota.
5. Add another client adapter only when it offers a measured benefit. Keep the
   orchestrator interchangeable by documenting the same file/result protocol.

Implementation tests should cover duplicate assignment, malformed/truncated
events, exit-without-result, timeout with a surviving child, restart recovery,
out-of-scope edits, input drift and missing usage. Fake processes can exercise
supervision without paying for model calls. A real scoped pilot is still needed.

Report accepted C/legacy bytes, lift-inclusive recovery, composed calls and live
integration separately. More translator output does not establish milestone gates.

## Investigation limits

Local discovery found OpenCode, Claude Code, Codex and mmx on PATH. OpenCode
reported 1.18.3; its installed `run --help` exposes model selection, JSON events,
session IDs and directory selection. No authenticated worker call was made.

The parent start command was refused by an active session runner (PID 66696).
Its receipt recorded doctor/status/pytest exit 0 and an ongoing 16-worker
revalidation. That is evidence of the other session's baseline, not a completed
baseline or closing gate for this investigation. Recovery code was not changed.

## Suggested next-session request

> Implement the first OpenCode/MiniMax worker adapter from
> docs/agent/MULTI-PROVIDER-PROPOSAL.md. Reuse cmre claims and pilot accounting,
> preserve the single-integrator acceptance rule, and test supervision with fake
> processes. After exclusive machine jobs finish and provider configuration is
> verified, run a bounded two-worker pilot with measured review costs. Do not
> scale concurrency until reviewed acceptance throughput justifies it.
