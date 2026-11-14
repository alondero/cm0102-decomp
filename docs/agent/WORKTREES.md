# Same-machine isolated C recovery workers

The [farm](FARM.md) does not use worktrees: its workers write scratch sources
and test with `cmre test --source`. This page is for isolated by-hand workers.

One authoritative checkout owns the local game, pinned inventory, hints, queue,
task leases and attempts, review receipts and acceptance ledger. Separate Git
worktrees own worker C edits and ignored evidence/report snapshots. The coordinator
brokers access to local assets; game files are neither copied into worker trees
nor added to Git. No directory junctions are needed or supported.

This lane uses the existing bounded C task gate: one accepted lifted function,
no accepted C tier, at most 600 inventory body bytes, unchanged semantic comparison
at O0/O2. Larger functions and harness work continue in the authoritative checkout.
It coordinates trusted same-user local processes, not hostile filesystem access.
The integrator owns the parent preflight and closing check; workers inherit that
receipt and do not start independent sessions.

## Workspace lifecycle

Keep one authoritative checkout. Its `main` tracks `origin/main`; private history
is retained under `refs/private/cm0102-pre-publication` and in local backups.
Project exclusions live in `.gitignore`. Keep `.git/info/exclude` specific to
machine-only paths; a blanket exclusion hides unfinished source from review.

Create workers on demand in one external container, such as
`F:/worktrees/cm0102/`, with a unique batch/owner name. The broker rejects workers
nested inside the coordinator. Start with two translation workers and an independent reviewer;
increase concurrency only from measured throughput. Harness experiments use a
separate tree in the same container. Workers inherit the parent receipt and use
the coordinator's broker, never their own copied game or acceptance ledger.

At the batch boundary, stop workers and reconcile live claims through the normal
work protocol. Preserve candidate source, reports, bindings and attempts. Archive
any unique or pending files with a hash manifest outside the disposable tree and
verify restoration before removing it with `git worktree remove`. Retain a commit
reference for unmerged tooling. Clean trees may be removed normally; force removal
requires a verified archive of every ignored and untracked file as well as edits.
Old binding paths remain historical evidence, not reusable worker directories.
Use a fresh path and bootstrap for the next batch; never erase claim/attempt
history to recycle a directory. Local backup locations and pending candidates
belong in `CLAUDE.local.md` and the ignored handoff.

## Bootstrap and claim (coordinator shell)

Bootstrap requires byte-identical `cmre/`
Python modules and `recovery/include/` files. It pins each tree's HEAD and tooling,
the common Git directory, the official target SHA-256 and all inventory hashes.
Different branch histories are allowed when the tooling is identical. Keep these
inputs frozen until workers finish. Normalizing line endings changes their hashes;
use the repository's existing line-ending configuration consistently.

For a fresh, unbound worker from an older revision, use
`py -3.13 -m cmre worktree bootstrap WORKER --align`. It checks pins, Git identity
and every differing tooling/header destination before copying exact coordinator
bytes. Edited, staged, deleted, untracked, ignored, hidden-index and extra worker
files require manual reconciliation; unrelated candidate C stays in place.
Interrupted copies can be retried. Valid bindings are reused automatically. Stale
or orphan bindings require a fresh worker or explicit reconciliation; alignment
preserves their claims and attempts.

```powershell
git worktree add -b recovery-BATCH-a F:/worktrees/cm0102/BATCH-a HEAD
git worktree add -b recovery-BATCH-b F:/worktrees/cm0102/BATCH-b HEAD
py -3.13 -m cmre worktree bootstrap F:/worktrees/cm0102/BATCH-a --align
py -3.13 -m cmre worktree bootstrap F:/worktrees/cm0102/BATCH-b --align
py -3.13 -m cmre screen fn_X
py -3.13 -m cmre screen fn_Y
py -3.13 -m cmre worktree claim F:/worktrees/cm0102/BATCH-a fn_X --owner worker-a --seconds 3600
py -3.13 -m cmre worktree claim F:/worktrees/cm0102/BATCH-b fn_Y --owner worker-b --seconds 3600
```

Use distinct functions and owner names. Claims share `cmre work claims` with
same-checkout workers; duplicates are rejected. Each claim also has a task token
retained only by the coordinator. Its packet, including the complete original
instruction evidence, is copied to the worker's ignored
`CM0102/Analysis/cmre/worktree/fn_X/packet.json`. Read that packet, the inherited
contract and the C workflow. Never stage packets or report snapshots.

## Screen before dispatch

Run `cmre screen fn_X` before reserving worker/review capacity. It executes every
case in the effective semantic C corpus, including original-side discovery,
fixtures and indirect boundaries, without a candidate build, task attempt or
acceptance. Receipts record return counts, first returning case, instruction
coverage, own/callee fault sites and execution seconds. Matching target, inventory,
harness, headers, input policy, hints and observer identities allow receipt reuse.

Exit 1 (`evidence-needed`) means no return was observed. Prefer another function;
keep a named returning-input obligation for the held candidate. Exit 0
(`return-observed`) is a scheduling signal: review the uncovered instructions,
ABI, stack aliases and callee continuation obligations before accepting. A single
return cannot establish path completeness. Keep all fault cases and the unchanged
O0/O2 differential gate. Functions that intentionally never return require a
specific instruction-evidence review rather than invented returning fixtures.

Before claiming a likely register/stack adapter, check the ABI and availability of
the required raw stack-alias and late-fault evidence in [C-BOUNDARIES.md](C-BOUNDARIES.md).
If those inputs cannot be exercised through the current workflow, prepare that
evidence first. Emitted-effect identity is useful structural evidence; record it
separately from raw witnesses actually executed. The 2026-11-02 pilot reached
returns but held both exact-stack adapters for this missing evidence; scaling
translation alone would not have improved reviewed acceptance.

For a small pilot, use `cmre pilot start NAME --functions fn_X fn_Y` before
dispatch, retain task `validation_seconds`, and finish with `cmre pilot finish
NAME`. Compare accepted functions/bytes, holds and elapsed time with the retained
baseline. Record review/coordinator time and actual token usage only when observed;
different cohorts do not establish a concurrency speedup.

Worker bindings and a hash-pinned authoritative packet live under ignored analysis
storage. Submission and review check the worker packet against that retained copy.
Bootstrap
retries retain the existing binding and claims. A changed HEAD or tooling snapshot
requires a fresh worker tree after existing workers have stopped and outstanding
claims have expired or been reconciled; bootstrap never resets attempts.

## Translate and validate (worker shell)

Write only `recovery/c/fn_X.c`, preserving behaviour from the packet's original
instructions, then run:

```powershell
py -3.13 -m cmre worktree test fn_X
```

This invokes the bound coordinator's Python tooling with separate process state
and without inherited writer credentials or lock-path overrides. It checks the
snapshot and live claim, imports only the fixed C file into task staging, reserves
an attempt durably, and runs the original C differential gate. A JSON result names
the ignored worker report and task ID. Failure exits nonzero. Ordinary `cmre test`,
`accept` and machine jobs are not worker commands; workers have no local game or
authoritative recovery state.

Disjoint worker processes can test concurrently. Metadata updates use short queue
locks; each function and each worker tree excludes overlapping operations. The
five-attempt ceiling includes interrupted attempts and previous task fingerprints,
owners and worker roots. Existing queue failures and task attempts are combined
using durable event identities, so completed task failures are counted once.
Changing hints, restarting a process or using a fresh tree cannot erase that budget.
Ownership is checked again after execution, while the queue lock covers the short
submission commit. Expired or replaced claims cannot produce a passing submission.

## Review and serial acceptance (coordinator shell)

Review the worker C source against its packet's original listing and its matching
report. The reviewer returns the exact source/report SHA-256 values and a concrete
instruction-evidence justification. Record those values, then publish locally:

```powershell
py -3.13 -m cmre worktree review F:/worktrees/cm0102/BATCH-a fn_X --reviewer reviewer-a --reason "Instruction evidence and ABI reviewed" --source-sha256 SOURCE_HASH --report-sha256 REPORT_HASH
py -3.13 -m cmre worktree publish F:/worktrees/cm0102/BATCH-a fn_X
```

Review checks both worker copies and authoritative staged evidence. Publication
checks the review's hashes and input fingerprint again, imports reviewed source
into the canonical `recovery/c/fn_X.c`, then calls the existing `cmre accept --c`
gate under the single OS-owned writer lease. Concurrent acceptance and machine
writers fail with lock contention. Worker reports alone never establish recovery.
Other functions' reviewed acceptances do not invalidate a worker's function inputs.

To request an edit after a passing submission:

```powershell
py -3.13 -m cmre worktree revise F:/worktrees/cm0102/BATCH-a fn_X --reason "Describe the instruction or ABI discrepancy"
py -3.13 -m cmre worktree claim F:/worktrees/cm0102/BATCH-a fn_X --owner worker-a --seconds 3600
```

Revision invalidates review and retains all attempts. The worker edits and tests
again; a new review is required. A successful publication releases its queue claim.
`publish` is local acceptance, not a Git push. Only canonical reviewed authored
source may enter the normal publication scan/commit workflow. Task staging under
`recovery/c/tasks/` is ignored, as are evidence, bindings, reports and generated
records. Stage explicit reviewed paths and run `cmre publication scan` as usual.

## Interruption and retry

An OS lock is released when its owning process terminates. Stop the old worker
before retrying, wait for its lease to expire, then claim the same function again.
The coordinator changes the token, marks a reserved unfinished task attempt
interrupted and retains the report history and budget. The old token and stale
worker snapshots cannot submit. A live claim is never stolen.

If validation completed before report export was interrupted, repeat the worker's
`worktree test fn_X`: it reconciles the existing successful task without another
execution or attempt, including a fifth-attempt success. To explicitly restore
missing or damaged packet/report snapshots from their pinned authoritative copies,
run `py -3.13 -m cmre worktree sync WORKER fn_X` in the coordinator. Ordinary test
and review commands reject altered snapshots. Sync never changes worker C source,
the successful candidate hash, its review requirements or the attempt budget.

If publication was interrupted, rerun the same `worktree publish`. The underlying
task publication journal reconciles accepted source against the ledger or restores
the retained previous source. Conflicting external edits require investigation.
Do not delete task directories, bindings, claims or reports to retry. After five
attempts, stop translation and investigate the recorded evidence in the coordinator.

## Validation evidence

`tests/test_cmre_worktrees.py` exercises shared-checkout collisions, concurrent
disjoint actual Git worktrees in separate processes, killed-process lock release,
reserved-attempt recovery, expiry, stale source/report/tooling/input identities,
path redirection, pin mismatches, revision and five-attempt persistence.
Its small synthetic gates test coordination, not game semantics.
`tests/test_cmre_worktrees_integration.py` creates a real Git worker and isolated
local coordinator, validates against the pinned original at preferred-image VA
`0x004671b0`, records review and performs real O0/O2 C acceptance. The target is
SHA-256 `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
That test leaves the production ledger and recovery counts unchanged and checks
that no game or generated analysis paths are tracked by Git.
