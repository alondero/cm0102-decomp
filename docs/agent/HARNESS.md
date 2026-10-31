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
   | Module in `HARNESS_MODULES` | Focused regression, full pytest, and complete revalidation below |
   | Acceptance/ledger semantics or C build inputs outside that list | Determine and validate affected records explicitly; file-list exclusion does not prove unchanged evidence |

   Behaviour-changing code stays fixed during revalidation. A comparison can
   only be relaxed with a test proving the equivalent case and a near miss that
   still fails; see `tests/test_cmre_normalize.py`.
   Freeze build and acceptance inputs during a source campaign. Bundle necessary
   behavior-module fixes and their regressions before one complete revalidation;
   retain every intermediate failure. Tooling/reporting changes follow their
   own row above and do not trigger inventory refresh merely because they are
   Python files. Never bypass a required revalidation to improve throughput.
4. For a behaviour-module change, after full pytest passes:

   ```powershell
   py -3.13 scripts/cmre_session.py run revalidate --refresh --jobs 16
   ```

   Completion: the **whole** accepted inventory passes, not a sampled `-n` run.
   `DRIFT` or `SKIP refresh` needs resolution before continuing. C records need
   their C tier checked and re-accepted too. `cmre verify` checks ledger integrity;
   it does not check the current harness identity or substitute for revalidation.
   When a support-header change makes doctor reject existing C dependencies,
   first revalidate and re-accept the **unchanged** canonical C sources with
   `recover.accept(va, 'c', refresh=True)`. Preserve their previous receipts and
   source hashes; never remove C records to bypass doctor. This refresh enables
   the supervised run and does not replace the whole inventory gate. The refresh
   job independently validates each existing C tier, even if its lifted hash is
   current, and preserves the original lifted baseline.
   After a complete pass with failures, resolve them and run the same supervised
   command with `--resume RECEIPT --retry-failed`. This creates a new whole-inventory
   receipt retaining the original failures. Successful items are reused only
   when their source, hints, ledger, build and harness identities still match;
   changed successful items are rerun too, with their previous receipts retained.
   A changed harness or global execution identity requires a new full pass.
5. Only after revalidation finishes successfully, run
   `py -3.13 -m cmre unblock --batch` to requeue machine-made blocks. Agent blocks
   remain. Then return to [BATCH.md](BATCH.md) if recovery is the requested task.

Docs-only work can preserve an observed failing baseline and report it; it
cannot label that session green. Keep failing contracts intact. Finish through
the parent [playbook](PLAYBOOK.md#3-close-the-session).

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
