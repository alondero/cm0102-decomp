# Tooling and harness changes

## Recovery intake

The coordinator uses this gate before starting a recovery batch or resuming
candidate dispatch. Workers inherit its receipt and frozen shared inputs.

1. Inspect any required validation receipt in the handoff and its actual process
   state. The validation budget and impact classification below determine when
   an inventory gate is required; a new session alone does not request one. Reuse
   matching completed evidence, preserve live jobs and resume interrupted jobs
   through their supported receipts instead of restarting the selection.
2. For a required inventory gate, require its complete selected scope to finish
   with zero failed, tier-inconclusive or pending tiers and no
   identity/compatibility drift. Check
   ledger integrity separately and run the parent check at the boundary defined
   in the [validation budget](PLAYBOOK.md#validation-budget).
3. Record the batch's harness, headers, execution policies and controller identity.
   Keep them fixed while recovery owners run. Prepare tooling in an isolated
   lane and bring reviewed changes back at a batch boundary under the procedure below.

Completion: required evidence matches the frozen execution inputs and dispatch
has no pending mandatory inventory gate. An accepted-evidence correctness defect
still takes priority over throughput.

Retain stage receipts and the recorded child execution environment so a failed
planning stage can resume without repeating successful work. Diagnose environment
drift explicitly; restore a recorded environment or validate the changed inputs.
Keep every declared input of a live proof fixed, including documentation when its
regression receipt covers those files. Stage updates outside that input set until
finalization finishes.

A separate Git worktree does not isolate the Python interpreter or toolchain.
While a full-suite receipt is running, do not install, remove or upgrade shared
packages or tools from any lane. Prepare new dependencies in a worktree-local
virtual environment, and keep the recovery interpreter and its environment
fixed. Package-only drift invalidates exact-input reuse just like file drift.

During a required gate, authorized input workers may prepare private proposals
and bounded original-only diagnostics when their selected protocol supports those
operations. Keep the VM, pinned original code and all declared live inputs fixed;
vary initial data only, retain receipts and leave resource headroom. These probes
inform inputs; C validation and acceptance wait for the required gate.

## Private inventory comparisons

For a named function, compare the canonical pins and a candidate export
without accepting or modifying either inventory:

```powershell
py -3.13 scripts/cmre_inventory_compare.py fn_XXXXXXXX --candidate EXPORT/delta.json --output NEW_LOCAL_DIRECTORY
```

The manifest carries `candidate_sha256` for `functions.tsv`, `calls.tsv`,
and `module-candidates.json` beside it. Add `--source PATH` to compare a C
candidate at both optimization levels. The two fresh processes retain both
pin identities, entry deltas and gate reports. The command can complete
successfully even when one or both gates fail; read the two statuses. This
diagnostic cannot authorize a pin change or replace inventory revalidation.

## Tooling changes

1. Bound the requested change and inspect existing worktree changes. For a defect,
   capture a smallest reproducer and a failing regression test before fixing it.
   For instructions, identify a session failure that the wording must prevent.
   Apply [validation scope](#validation-scope) before starting differential work.

   The exact host read-buffer refactor has a separate, fail-closed proof path:

   ```powershell
   py -3.13 -m cmre validation-compatibility --before PRIOR_RECEIPT --after PLAN --regressions PYTEST_RECEIPT --observer-before OLD_EMU_SOURCE --comparison REAL_CASE_REPORT --reviewer REVIEWER --reason REASON --output PROOF_RECEIPT
   py -3.13 -m cmre revalidate --refresh --plan --reuse PRIOR_RECEIPT --compatibility PROOF_RECEIPT
   ```

   The proof reverses only the recognized storage transformation and checks the
   remaining complete module AST, native public-read contract, old/new real-case
   results, current pytest evidence and unchanged environment. Each tier must
   still match every execution dependency. Acceptance metadata is retained as
   provenance; it does not force execution when those inputs are identical.
   Changed or unknown tier inputs remain pending. Observer source, proof,
   checker, Python regression code or environment drift rejects the proof.
   C source and fixture changes are evaluated per tier. Historical results keep their original
   identities and origins; this does not refresh acceptance records or establish
   new composition or live-integration evidence. Other observer changes do not
   inherit this exception. Subsequent plans based on a proof-backed receipt
   inherit and reverify its proof automatically, including at finalization.

   Completion: observed failure and an explicit target behaviour, with files in scope.
2. Make the change and run its focused check. Harness edits need no prompt
   (ADR 0011); while a recovery writer holds the lease, edit in a worktree.
   The guard denies every `cmre/` edit while a recovery writer holds the command
   lease, because a running revalidation or batch would load the change mid-run.
   Edit in a separate worktree while a run is active, then merge one batch
   ([validation budget](PLAYBOOK.md#validation-budget)).
   Changes to `HARNESS_MODULES` ship only as a release
   ([ADR 0007](../adr/0007-harness-releases.md)): batched in a worktree, applied at
   a recovery batch boundary, one inventory gate per evidence-changing release,
   at most one release per working day unless accepted evidence has a correctness
   defect.
   A worker reporting a harness issue returns evidence to its parent.
3. Classify the changed files using `HARNESS_MODULES` in `cmre/recover.py`:

   | Change | Required validation |
   | --- | --- |
   | Instructions, skills, session runner, guard, CLI routing, queue, tests | Focused checks and the parent session gate |
   | Module in `HARNESS_MODULES` | Focused regression, full pytest and a dependency-aware plan. Apply the scope decision below to select execution and reuse. |
   | Acceptance or ledger semantics, or C build inputs outside that list | Exercise the changed path and identify affected records. Use the plan when prior inventory evidence may change. |

   Behaviour-changing code stays fixed during revalidation. A comparison can
   only be relaxed with a test proving the equivalent case and a near miss that
   still fails; see `tests/test_cmre_normalize.py`.
   Freeze build and acceptance inputs during a source campaign. Bundle necessary
   behavior-module fixes and their regressions before one complete revalidation;
   retain every intermediate failure. Tooling/reporting changes follow their
   own row above and do not trigger inventory refresh merely because they are
   Python files. Never bypass a required revalidation to improve throughput.
4. For an inventory gate, batch necessary fixes, freeze execution, build and
   acceptance inputs, and save the plan for the intended run:

   ```powershell
   py -3.13 -m cmre revalidate --plan --json --output CM0102/Analysis/cmre/validation-plan.json
   ```

   Use the same scope, reuse and compatibility options for planning and
   execution. Include `--refresh` in both when the release requires refreshing
   acceptance records.
   Completion: every accepted lifted and C tier has current executed or explicitly
   reusable evidence, with zero failed, inconclusive or pending tiers. A sampled
   `-n` run is never the inventory gate. The plan lists effective options, profiles,
   optimizations, compiler-selected dependencies, invalidation reasons, previous
   failures, measured item costs and affected recorded compositions. Missing
   dependency coverage selects execution and remains inconclusive if unresolved.
   Revalidate affected compositions separately with `compose-test`; single-function
   results confer no composed-call or live-integration claim. `cmre verify` remains
   an independent ledger-integrity check.
   When a support-header change makes doctor reject existing C dependencies,
   first revalidate and re-accept the **unchanged** canonical C sources with
   `recover.accept(va, 'c', refresh=True)`. Preserve their previous receipts and
   source hashes; never remove C records to bypass doctor. This refresh enables
   the supervised run and does not replace the whole inventory gate. The refresh
   job independently validates each C tier and preserves its lifted baseline.
   Default revalidation reuses the latest dependency-aware receipt. Use `--reuse
   RECEIPT` to choose one explicitly. `--resume RECEIPT` executes pending or
   invalidated successful tiers and retains completed failures without retrying
   them; the command remains failing while any retained failure is present.
   Add `--retry-failed` to explicitly execute failed tiers while retaining matching
   successes and original receipts. `--audit` explicitly reruns the whole selection,
   including previous failures, so it is refused with `--retry-failed`: to retry
   an audit sweep's failures, repeat its command without `--audit`. Schema-2
   receipts retain their original strict `--resume` contract. Migration can reuse
   an accepted tier's current run report only after recomputing its exact key
   from execution identity, effective options, source and selected build inputs.
   An unchanged repeat performs no new differential executions. C-only hints
   preserve unchanged lifted evidence; shared hints invalidate both tiers.
   Header dependencies are compiler-selected at both C optimizations; new include
   availability conservatively invalidates C coverage. Old results keep their
   original identities and receipt locations. Refresh never silently relabels them.
   Plans store hashes of effective fixture payloads and acceptance evidence,
   retaining original hint/report paths instead of copying large corpora into
   every item. Schema-1 prototype validation receipts remain local history;
   schema-2 can migrate exact current accepted run reports without executing them.
   Plans and final receipts include numeric whole-workspace `counts` for accepted
   C/legacy functions and bytes, lifted/inclusive recovery and live integration.
   These are independent of the selected validation tiers. Final receipts retain
   `counts_before` and a fresh closing `counts` snapshot. Historical receipts that
   lack counts report unknown values rather than inventing zeroes.
   Compiler environment and workspace changes invalidate reuse. Volatile macros,
   computed macro names and external include-availability tests are preprocessed
   again; unchanged source hashes alone do not prove their generated inputs.

5. Only after revalidation finishes successfully, run
   `py -3.13 -m cmre unblock --batch` to requeue machine-made blocks. Agent blocks
   remain. Then return to [BATCH.md](BATCH.md) if recovery is the requested task.

Docs-only work can preserve an observed failing baseline and report it; it
cannot label that session green. Keep failing contracts intact. Finish through
the parent [playbook](PLAYBOOK.md#3-close-the-session).

## Page-write diagnostics

When `cmre explain` reports a memory-page difference, retain the bounded guest
write observations for that case:

```powershell
py -3.13 scripts/cmre_page_writes.py fn_XXXXXXXX --c --case N --page 0x00ae2000
```

Use the same candidate and replay options as `explain` (`--source`, `--opt`,
`--profile`, `--budget`, `--cases`, `--window`). The probe records instruction
addresses and values for at most 256 guest write attempts per side, including
stores that fault. It reports only the final attempt if the recovered run
retries with a higher budget. Host writes, including seeded import outputs, are
not observed. The report is local diagnostic evidence, not an acceptance result.

## Validation scope

1. Name the failure the check should catch and the decision it will support.
   Trace the changed path. Use targeted checks for acceptance, publication,
   composition and integration behavior. For example, an operand-proof change
   needs the acceptance path with a valid case and a case for each independently
   broken operand. The ordinary inventory audit reruns the differential cases
   and checks accepted source identity. It bypasses the operand-proof gate in
   `accept`. See [the recorded operand experiment](FARM.md#supply-lane-codex--gpt).
2. When prior inventory evidence may change, inspect the complete
   `cmre revalidate --plan` and the exact diff. In the release record include the
   prior receipt, affected functions or execution paths, the number of tiers to
   execute and reuse, and the estimated wall time or why it is unknown.
   A global hash invalidation reports the loss of automatic reuse. Use the diff
   and dependency evidence to determine the reach of the change.
3. For bounded changes, run the focused regressions and use dependency-aware
   reuse for matching tiers. For a changed global identity, carry forward results
   only through a verified compatibility receipt. Use the supported proof for
   [input dispatch](#bounded-input-dispatch-compatibility) or the exact host
   read-buffer refactor above. When a new proof is needed, batch the change while
   preparing it or record why a sweep is the better use of time for this release.
   Record the targeted result alongside any inventory receipt.
4. Run one full sweep for a named milestone or when a frozen release can
   change results broadly or its affected scope remains unresolved. Global
   changes to emulation, imports, ABI, compilation, input generation, or
   comparisons commonly meet this trigger. Record the reason before launch.
   Reuse matching completed evidence for the same release and resume an
   interrupted job from its receipt.

## Bounded input-dispatch compatibility

Keep a complete dependency-aware receipt from before the change and save a complete
after-change `--plan --output` file. Add focused positive and negative regressions
for the actual dispatch change, including changed seeds/policies/builds/fixtures
and a comparison near miss. Run full pytest through the session runner, then:

```powershell
py -3.13 -m cmre validation-compatibility --before BEFORE_RECEIPT --after AFTER_PLAN --regressions SESSION_RECEIPT --reviewer REVIEWER --reason "Reviewed scope and equivalence argument" --output CM0102/Analysis/cmre/compatibility.json
py -3.13 -m cmre revalidate --refresh --plan --compatibility CM0102/Analysis/cmre/compatibility.json --reuse BEFORE_RECEIPT --json
```

Review the resulting scope before using the same options in the supervised run.
The compatibility receipt pins both harness versions, all global identities,
complete selection, original evidence, test receipt and justification. Creation
rechecks every proposed unaffected tier's effective inputs and compiler-selected
build. The AST guard permits only hint selection/validation and tier dispatch;
changed comparison, RNG generation, thresholds, emulation or other execution code
rejects compatibility. Changed effective inputs/builds remain selected for execution.
Unknown transitions use a full sweep. Retain both original receipts.
The allowed AST transformations intentionally describe this proven dispatch lane;
extending the grammar requires its own equivalence evidence and regressions.
Generalizing arbitrary AST rewrites would broaden the evidence claim without proof.

## Reusing full pytest

The runner records declared inputs before and after full pytest, including code,
tests, authored files, recovery inputs, local fixtures, target/inventory, packages,
Python, tool binaries and relevant environment. A later start, supervised run or
closing check reuses only a completed successful full-suite step with equal inputs
and an intact log. New/changed/deleted dependencies invalidate reuse; old receipts
without coverage and focused runs cannot seed it. Doctor/status and `cmre verify`
still run independently. Keep full pytest, differential validation and integrity
as separate claims in the final report.

## Measuring the work saved

`cmre validation-benchmark fn_005ab510 fn_00666720` copies the selected accepted
sources, hints, headers and ledgers into retained ignored analysis storage. It
executes a cold control, an unchanged repeat and a C-only fixture edit; remaining
seed/source/header/global-scope scenarios measure impact selection only. Canonical
inputs, acceptance and recovery counts are unchanged. JSON receipts record elapsed
time, differential executions, reuse rate and validation/recovery time separately.

The issue #24 measurement on those two #23 accepted functions used six differential
executions in 16.89 s cold, zero in 0.10 s unchanged (100% reuse), and two in 5.12 s
after a C-only fixture edit (75% tier reuse). Both optimizations passed; lifted
evidence remained reusable. Impact-only controls selected two tiers for a shared
seed, one for C source, two for the shared header and all four for a global-scope
audit. These small-corpus timings are measurements, not whole-inventory forecasts.
The retained local report is linked by `CM0102/Analysis/cmre/issue-24-benchmark.log`.

## Separate lifted and C input evidence

Issue #23 exposed fixture edits made for C that changed the lifted random stream
and invalidated four accepted lifts. Top-level hints remain shared; `c_hints`
overrides input shapes and fixture metadata for C only. It cannot override test
policy or original-instruction bindings. Run caches hash the effective tier
inputs, and whole-inventory job receipts hash the complete hint file. Changing
C-only hints therefore cannot reuse stale C evidence or disturb lifted inputs.

C acceptance refreshes the record's hints path too. Previously it inherited that
path from the lifted record, so a legacy `hints: null` does not by itself prove
that C ran without fixtures. Compare retained hint identities and per-tier run
evidence instead.

The local resolution keeps the original C fixtures intact under `c_hints`:

| Function | Lifted inputs | C inputs and status |
| --- | --- | --- |
| `fn_005a90b0` | Original `ecx: ptr`, without C fixtures | Eight fixtures; unaccepted coverage-limited candidate |
| `fn_005ab510` | Default random inputs | Six fixtures; revalidate its accepted C tier |
| `fn_00666720` | Default random inputs | One fixture; revalidate its accepted C tier |
| `fn_005e5df0` | Original first 17 fixtures | All 40 fixtures; unaccepted self-interior-call candidate |

Original hint files are retained under `CM0102/Analysis/cmre/issue-23/` locally.
The latter candidate's C case 19 (lifted case 151 with all 40 fixtures) sets a
virtual slot to preferred-image VA `0x005e5df1`, inside the entry instruction at
VA `0x005e5df0`, for binary SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`. The original executes
there and faults; the recovered VM stops at its watched-region escape guard.
ADR 0002 currently exempts the function's own code from semantic indirect stops.
Splitting inputs preserves this failing C case; it does not validate that candidate
or change the indirect-stop policy. Resolve that boundary before C acceptance.

Session lifecycle and evidence-packet reporting live in `scripts/cmre_session.py`
and `cmre/evidence.py`. Their focused regressions are
`py -3.13 -m pytest scripts/tests/test_cmre_session.py tests/test_session_runner.py tests/test_cmre_evidence.py tests/test_cmre_cli.py`.
These reporting changes leave `HARNESS_MODULES`, comparison profiles and
acceptance inputs unchanged; they do not require inventory revalidation.

## Observable addresses in the hybrid VM

[ADR 0005](../adr/0005-original-addresses-in-the-hybrid-vm.md) records the original
CALL-word and recovered-page contracts. A parent may declare selected lifted
direct CALLs with `lift_original_calls` in that function's hints:

```json
{"lift_original_calls": {"0x005230c3": {"bytes": "e838020000", "evidence": "Original callee array read exposes the caller's return bytes."}}}
```

Keys are preferred-image VAs for the pinned binary. The builder checks the
original E8 bytes, owner, target and continuation against the unchanged generated
CALL and labels. Task workers cannot change these bindings. Declaration does not
change generated source, case generation, seed, comparison profile or thresholds.
The VM executes the original CALL and permits only its owned near-RET dispatch;
it does not repair return words or arbitrary data. Its executed-instruction
budget and finite native-step guard are recorded. C-only counting is unchanged.

Data-access addresses retain their raw value until paired comparison. A native
write-protected fault retains its class. An original write-unmapped fault can
match it only at the same raw address and width, with the profile's existing
instruction relation and a complete read-only recovered-only page witness.
`mapping_faults` records candidate evidence and whether the relation was used.
Do not fold fault classes generally, remove inputs, or change the link base to
hide a collision. Readable/executable page collisions are not solved by this rule.

The semantic profile treats a stack-probe fault below the test stack as one
location: a data fault whose instruction lies in the pinned original probe
(`0x00945610..0x0094563E`) and whose address lies in the page below the stack
(`semantic.GUARD_START`; the return push faults in its last dword). The offset in that
page is the caller's frame depth. Access class, width and every other address
still compare raw.

`cmre explain --window N` shows raw event context and read sites. Semantic and
ABI comparisons filter import events, so `event_context` records their separate
raw indices rather than treating a filtered index as a raw trace position.

### Dead-stack reads in C comparisons

The semantic profile treats the stack as compiler layout. A random case can still
aim a data pointer or loop at the live stack (an unaligned dword composed from two
arena words can name a stack address), so the original's callee reads frames that
earlier or deeper calls left below ESP, and passes those leftovers to an import.
The C wrapper's frame holds different saved registers, so the import arguments
differ although the function is right. Cases run without tracking. A semantic-profile
case that differs is re-run once on the original machine with tracking, which
records each stack read wholly below the live ESP (`Result.dead_stack_reads`); a
re-run that does not reproduce the first run exactly is ignored. The rule:

- For an event difference, a dead read qualifies only if it precedes the original's
  raw event for the first differing import call.
- For a difference in the outcome, fault detail, registers or memory, any dead read
  qualifies.
- Reads by the CRT `__chkstk` probe (`STACK_PROBE_CODE`) never qualify.

A qualifying case is `inconclusive`, is listed under `stack_layout_cases` /
`stack_layout_examples` (kept in the C acceptance record), and contributes no
instruction coverage. Above `STACK_LAYOUT_LIMIT` (2% of cases) the differences stay
mismatches (`kind: stack-layout-excess`). Cases without a difference are
unaffected, and lifted and `abi` runs do not use the rule.

## Codex preservation hook

`.codex/hooks.json` runs `cmre/guard.py --codex` for canonical `Bash` and
`apply_patch` events. Codex sends both payloads in `tool_input.command`.
The Windows command preserves exit code 2 through PowerShell and resolves the
repository root from Git so sessions launched in subdirectories use the same guard.
The JSON Windows override is `commandWindows` (`command_windows` is the TOML spelling).
See the [official hook schema](https://learn.chatgpt.com/docs/hooks).

Codex does not support the Claude `permissionDecision: "ask"` output: it reports
an error and continues the tool. The `--codex` mode therefore blocks unapproved
harness edits with exit 2. A launcher may pass the session's already-authorized
path scopes as a JSON list in `CMRE_AUTHORIZED_PATHS`; these scopes never override
protected game files or generated records. Do not request authorization again
when the user's existing task already supplies it.

Project hooks require review and trust through Codex `/hooks` before they run;
changing the definition requires another trust review. A passing guard subprocess
test does not establish that the current interactive session has trusted the hook.
Run `py -3.13 -m pytest tests/test_cmre_guard.py -q` for the payload and configured
Windows command checks. This guard-only change does not require inventory revalidation.
