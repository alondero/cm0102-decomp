# Tooling and harness changes

1. Bound the requested change and inspect existing worktree changes. For a defect,
   capture a smallest reproducer and a failing regression test before fixing it.
   For instructions, identify a session failure that the wording must prevent.
   Completion: observed failure and an explicit target behaviour, with files in scope.
2. Make the change and run its focused check. User authorization for this task
   covers necessary edits; honor it if a runtime guard asks about harness files.
   A worker reporting a harness issue returns evidence to its parent.
3. Classify the changed files using `HARNESS_MODULES` in `cmre/recover.py`:

   | Change | Required validation |
   | --- | --- |
   | Instructions, skills, session runner, guard, CLI routing, queue, tests | Focused checks and parent session gate; no blanket revalidation for these files alone |
   | Module in `HARNESS_MODULES` | Focused regression, full pytest, and dependency-aware inventory gate below; global/unknown changes select everything |
   | Acceptance/ledger semantics or C build inputs outside that list | Determine and validate affected records explicitly; file-list exclusion does not prove unchanged evidence |

   Behaviour-changing code stays fixed during revalidation. A comparison can
   only be relaxed with a test proving the equivalent case and a near miss that
   still fails; see `tests/test_cmre_normalize.py`.
   Freeze build and acceptance inputs during a source campaign. Bundle necessary
   behavior-module fixes and their regressions before one complete revalidation;
   retain every intermediate failure. Tooling/reporting changes follow their
   own row above and do not trigger inventory refresh merely because they are
   Python files. Never bypass a required revalidation to improve throughput.
4. Inspect impact before launching differential work. Batch necessary fixes,
   then freeze execution/build/acceptance inputs for the final run:

   ```powershell
   py -3.13 -m cmre revalidate --refresh --plan --json --output CM0102/Analysis/cmre/validation-plan.json
   py -3.13 scripts/cmre_session.py run revalidate --refresh --jobs 16
   ```

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
   including previous failures. Schema-2
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

   Global emulation, comparison, imports, ABI, compiler or execution-policy changes
   require a full pass. Use `--audit` at milestone/integration boundaries to execute
   every selected tier regardless of reuse. The only bounded compatibility lane is
   reviewed input-dispatch/validation changes; see the procedure below. File-list
   exclusion or equal hints alone cannot establish compatibility.
5. Only after revalidation finishes successfully, run
   `py -3.13 -m cmre unblock --batch` to requeue machine-made blocks. Agent blocks
   remain. Then return to [BATCH.md](BATCH.md) if recovery is the requested task.

Docs-only work can preserve an observed failing baseline and report it; it
cannot label that session green. Keep failing contracts intact. Finish through
the parent [playbook](PLAYBOOK.md#3-close-the-session).

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

`cmre explain --window N` shows raw event context and read sites. Semantic and
ABI comparisons filter import events, so `event_context` records their separate
raw indices rather than treating a filtered index as a raw trace position.

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
