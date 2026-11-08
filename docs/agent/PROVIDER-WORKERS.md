# Bounded external provider workers

For campaign selection, provider roles and the 150-minute budget with an explicit
coordinator-led retro, use [PROVIDER-CAMPAIGN.md](PROVIDER-CAMPAIGN.md).

Use `scripts/cmre_provider_worker.py` to supervise one standalone OpenCode process
per isolated worker. This is a process adapter, not a scheduler. The coordinator
still selects, screens and claims exact functions through [WORKTREES.md](WORKTREES.md),
records measurement through [THROUGHPUT.md](THROUGHPUT.md), reviews independently and
accepts serially. Follow those workflows first. Keep execution inputs frozen.

## Prepare and run

1. Require the parent green receipt and completed required inventory gates.
   Bootstrap a fresh external worker and claim its screened function. Copy only
   the necessary original evidence and current instructions into ignored worker
   storage. The writable source is `recovery/c/fn_X.c` in that worker; the broker
   handles task staging. A draft or existing callee C is not proof of its ABI.
   Tell the worker to stop after the broker pass and return the report path and
   available source hash. The coordinator computes the report hash for review;
   the broker response does not supply it. Do not require a worker to discover a
   hash while denying it every hashing command. Limit inspection to the supplied
   evidence/header; an unavailable evidence question is a hold, not a reason to
   explore the harness implementation.
2. Verify the installed client's version, configuration and authenticated model
   discovery. Pin the subscription provider and exact model; never use automatic
   provider fallback. Keep credentials in the client's credential store, outside
   Git, and out of prompts, specs and captured command lines. Disable sharing,
   plugins, delegation and unnecessary tools. Set headless permissions explicitly;
   allow only the assigned source edits and exact broker test command. These are
   cooperative controls, not an OS sandbox.
   Smoke the actual required reads as well as a harmless write: a write-only
   smoke does not establish packet/header access. OpenCode 1.18.3 required exact
   **workspace-relative** read allow rules in the pilots; absolute paths alone
   were denied. Keep relative rules for context, packet, header, assigned source
   and broker report paths. A scope-rule change requires a new scoped smoke.
3. Create a JSON spec under ignored analysis storage. Required fields:
   `directory`, `allowed_files` (exact relative paths), `owner`, `executable`,
   `model`, and `prompt_file`. Also record `functions`, `parent_receipt`,
   `client_version`, `pinned_files` (packets, prompt and parent receipt),
   `timeout_seconds` (default 900), and `max_steps` (default 30). `environment`
   may hold client paths/configuration, never credential values. The optional
   `command` argument array is for local fake-process supervision tests.
4. Run from the coordinator, retaining the controller's output separately:

   ```powershell
   py -3.13 scripts/cmre_provider_worker.py run SPEC_JSON NEW_RUN_DIRECTORY
   py -3.13 scripts/cmre_provider_worker.py status RUN_DIRECTORY
   py -3.13 scripts/cmre_provider_worker.py cancel RUN_DIRECTORY
   py -3.13 scripts/cmre_provider_worker.py collect RUN_DIRECTORY
   ```

   `run` stays attached until terminal; the other operations may run in separate
   processes. Existing run directories are refused. Each run retains spec, prompt
   pins, before/after hashes, PID identity, raw events, stderr, session export and
   a receipt. Windows processes are hidden and assigned a kill-on-close job;
   cancellation, timeout and controller termination stop descendants. The
   controller polls locally; ordinary scheduling needs no model calls. On Windows
   the worker starts suspended, joins the job and then resumes its primary thread,
   closing the launch interval in which descendants could otherwise escape.
   See Microsoft's [process creation flags](https://learn.microsoft.com/en-us/windows/win32/procthread/process-creation-flags)
   and [ResumeThread contract](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-resumethread).

## Collect and review

Collection refuses live or orphaned runs. On restart, inspect process identities
and retained claims rather than relaunching a possibly live worker. Orphaned runs
require manual reconciliation of process exit, candidate/report bytes and claims;
the adapter never releases or reassigns claims. Preserve all attempts.

The audit hashes tracked and nonignored untracked worker files and explicit
ignored pins. It detects source-scope changes and input drift; it does not audit
arbitrary paths outside the worker or all ignored scratch files. Tool permissions
and the existing broker remain required. Never treat the audit as containment.

`completed` means the client exited cleanly, emitted a terminal stop, exported the
requested model identity and passed the scope audit. It does not mean source
validation or acceptance. Review actual broker reports and original instructions,
record source/report hashes with `cmre worktree review`, then publish locally.
Keep held work and failed provider calls in the experiment denominator.

Record every exported assistant call once with `cmre pilot usage`. Fresh input,
cached input, cache writes and output remain separate; missing usage stays unknown.
A client-reported cost of zero is not a subscription cost measurement. Capture
quota before and after, including window boundaries. Codex coordination/review
tokens remain unknown unless observed independently, so do not attest complete
pilot usage merely because worker usage is available. Finish timing after review
and acceptance, then run one parent closing check.

Fake-process regressions:
`py -3.13 -m pytest scripts/tests/test_cmre_provider_worker.py -q`.
Client configuration and event formats remain version-specific; the initial
adapter was exercised with OpenCode 1.18.3 and the subscription model IDs
`minimax-coding-plan/MiniMax-M3.1-Flash` and `opencode-go/deepseek-v4.1-flash`.
Re-discover account access before use; these names do not establish entitlement.

## Role scopes and readiness lessons

The C-worker path above permits only assigned C edits and broker tests. A
preliminary reviewer gets read-only source/evidence access and returns discrepancies;
it does not submit or approve its own source. An input author gets separate private
proposal paths under the selected triage/control workflow. It cannot adopt hints,
change acceptance inputs or use this C broker for an ineligible function. The
coordinator owns adoption, final evidence review and serial acceptance.

Keep credentials in the selected client's supported local store. An expired Go
key needs replacement through Console's account menu: Account > Personal API keys
> Add API key; choose the existing Go workspace. If permissions are offered,
Restricted > Models > Request permits inference. Never request a key in chat.
Local setup paths and live credential locations belong in the ignored handoff,
not tracked provider configuration. The November 2026 pilots used isolated XDG
state because the normal OpenCode database failed before model discovery; preserve
that database rather than resetting it. Treat this as a local workaround.

When a Windows PowerShell helper uses `File.Replace` without a backup, pass
`[NullString]::Value`: the tested Windows PowerShell binding converted `$null` to
an invalid empty backup path. Exercise the same persistence operation on harmless
files before asking for a secret. Keep any failure diagnostics free of values.

These are observed setup failures, not engine logic or automatic permission to
change global account, privacy, billing or filesystem settings. Current Console
key instructions: [API guide](https://opencode.ai/docs/console/api/).
