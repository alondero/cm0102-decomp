# Bounded external provider workers

For explicitly requested native OpenCode subagents, use
[OPENCODE-DIRECT.md](OPENCODE-DIRECT.md). The standalone adapter below remains
the default. For authorized coordination-only native work,
[NATIVE-COORDINATION.md](NATIVE-COORDINATION.md) assigns substantive operations to
workers and acceptance/gates to its expert steward; parent-review/adoption wording
below applies only to ordinary mode. Workers inherit completed preflight.

For durable provider-owned loops and terminal launch, use
[PROVIDER-CONTROLLER.md](PROVIDER-CONTROLLER.md). It invokes this process adapter
for each scoped provider stage and owns scheduling, review and gate dispatch.

For provider roles and allocation in ordinary batches and continuations, use
[provider batch intake](PROVIDER-CAMPAIGN.md#batch-intake). That document also owns
explicitly requested timed campaigns and their coordinator-led retro.

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
   and broker report paths. Allow the exact numbered report paths for the permitted
   attempts; use the actual path returned by the broker. A scope-rule change
   requires a new scoped smoke.
3. Create a JSON spec under ignored analysis storage. Required fields:
   `directory`, `allowed_files` (exact relative paths), `owner`, `executable`,
   `model`, and `prompt_file`. Declare `phase: "smoke"` for readiness or
   `phase: "work"` for the inference task. Both phases require
   `required_text_files` (exact relative path to `"text"` or `"json"`),
   `permission_files` (relative paths to every effective client permission/config
   file), and `smoke_file` (a separate harmless allowed write path). Include that
   smoke path in the same `allowed_files` list used for work. All declared text
   and the prompt must decode as UTF-8 (an optional UTF-8 BOM is accepted), contain
   no NUL and, for `"json"`, parse before any process launches. Required text must
   be nonempty, have no terminal blank numbered lines, and keep each line within
   1800 characters. These checks precede client launch. Write packets with
   an explicit UTF-8 writer; Windows PowerShell redirection can produce UTF-16.
   Also record `functions`, `parent_receipt`,
   `client_version`, `pinned_files` (packets, prompt and parent receipt),
   `timeout_seconds` (default 900), and `max_steps` (default 30). `environment`
   may hold client paths/configuration, never credential values. The optional
   `command` argument array is for local fake-process supervision tests only.
   Real provider runs require the readiness protocol; older specs need migration.
   During smoke, ask the provider to read every required text file completely
   with the read tool, then use the write tool on `smoke_file` only. The adapter
   checks completed tool events and their full file content/line ranges against
   the declared bytes, plus the actual harmless write. Complete coverage may use
   several read ranges. A continuation/truncation flag alone does not invalidate
   a range whose text, bounds and total exactly match the file; clipped content,
   gaps, denied reads and unrecognized evidence fail qualification. The initial
   recognizer is specific to the retained OpenCode 1.18.3 event schema.
   For separate `run` invocations, work specs point `smoke_run` to the successful
   smoke's run directory. The
   adapter rechecks raw smoke evidence and matches workspace, input hashes,
   permission/config hashes, a hash of the complete effective inherited environment
   plus overrides (inherited values are not retained), executable hash, exact model,
   client version and allowed paths before dispatch. Changed scope or inputs
   require a fresh smoke. Keep permission files read-only and list every input
   the task needs; undeclared files have no readiness proof.

   To prepare readable copies, use `packet SOURCES_JSON NEW_PACKET_DIRECTORY`.
   `SOURCES_JSON` is a list of `{ "source": "PATH", "name": "packet.json",
   "kind": "json" }` objects; use `kind: "text"` for other UTF-8 input.
   JSON is pretty-printed; terminal blank lines in text copies are normalized.
   Ambiguous/malformed JSON, empty text and lines over 1800 characters are refused.
   `--max-line-length` sets a stricter bound from 1 through 1800. The new directory's
   manifest pins original and copied bytes; originals stay unchanged. For long
   files, list every named chunk in the prompt or prove contiguous read ranges.
   Long JSON strings need a reversible, source-pinned projection with inverse
   equality checked; indentation alone cannot make them readable. Filenames must
   be distinct plain files, including under Windows rules. Declare the copied
   files in the provider scope and readiness spec; packet creation grants no
   permissions or recovery credit.
4. Run from the coordinator, retaining the controller's output separately:

   ```powershell
   py -3.13 scripts/cmre_provider_worker.py run SPEC_JSON NEW_RUN_DIRECTORY
   py -3.13 scripts/cmre_provider_worker.py pair SMOKE_SPEC_JSON WORK_SPEC_JSON NEW_PAIR_DIRECTORY
   py -3.13 scripts/cmre_provider_worker.py status RUN_DIRECTORY
   py -3.13 scripts/cmre_provider_worker.py cancel RUN_DIRECTORY
   py -3.13 scripts/cmre_provider_worker.py collect RUN_DIRECTORY
   py -3.13 scripts/cmre_provider_worker.py collect RUN_DIRECTORY --recover-export
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

   `pair` runs one smoke and one work stage in the same coordinator process,
   preserving the inherited environment identity between them. It verifies equal
   readiness scope before dispatch, pins both phase inputs and supplies its own
   successful smoke path to work. Failed smoke prevents work; neither stage is
   retried. Inspect or cancel the pair directory, and collect its `smoke` and
   `work` subdirectories separately. The pair receipt and receipts for every
   started stage remain available after failure; work is absent when smoke fails.
   This command does not schedule campaigns or
   validate and accept game source.

   For a physical-call list from the diagnostic observer, `project calls LIST_JSON`
   renders function addresses, ECX and every stack DWORD as exact hexadecimal
   values. It rejects missing or invalid fields and preserves a missing receiver
   as null. Use this projection in review packets instead of converting recorded
   decimal values by hand. It does not infer argument widths or discard high bits.

## Collect and review

Collection refuses live workers or orphaned runs. On restart, inspect process identities
and retained claims rather than relaunching a possibly live worker. Orphaned runs
require manual reconciliation of process exit, candidate/report bytes and claims;
the adapter never releases or reassigns claims. Preserve all attempts.

`collect` writes a new `collections/NNNN/receipt.json` with pinned raw artifacts.
It returns collected calls or explicit unknown usage (exit 1), including for
empty/truncated exports. `--recover-export` retries only an unreadable export,
once, with a bounded timeout (default/maximum 120 seconds). Recovery requires a
dead controller and worker and the executable hash pinned by the new readiness
protocol. Older unpinned clients remain unknown. Session and exact model identity
must match. The versioned recovery retains partial exports and stderr on failure;
original receipts and exports stay intact. It never relaunches inference or
promotes a cancelled, failed or incomplete run to completed. A failed smoke still
needs an explicitly scheduled fresh smoke before work dispatch.

The audit hashes tracked and nonignored untracked worker files and explicit
ignored pins. It detects source-scope changes and input drift; it does not audit
arbitrary paths outside the worker or all ignored scratch files. Tool permissions
and the existing broker remain required. Never treat the audit as containment.

`completed` means the client exited cleanly, emitted a terminal stop, exported the
requested model identity and passed the scope audit. It does not mean source
validation or acceptance. Review actual broker reports and original instructions,
record source/report hashes with `cmre worktree review`, then publish locally.
Broker test output includes authoritative `report_status`, a bounded
`first_failure`, and its numbered report path. Use `report_status: "pass"` as the
worker's stopping condition; task lifecycle and client shell exit metadata alone
do not establish the C result. The coordinator still reviews the full report.
Keep held work and failed provider calls in the experiment denominator.

Before pilot finish, import collection receipts and reviewed adoption counts with
`py -3.13 scripts/cmre_provider_worker.py account MANIFEST.json`. The manifest is:

```json
{
  "schema": "cmre-provider-accounting/1",
  "pilot": "ACTIVE_PILOT_NAME",
  "collections": ["ABSOLUTE_RUN_PATH/collections/0001/receipt.json"],
  "adoptions": "ABSOLUTE_PATH/adoptions.json"
}
```

The adoption receipt has `schema: "cmre-provider-adoptions/1"` and a `calls` list
of `{ "call_id": "EXPORTED_ASSISTANT_ID", "review_corrections": 2 }`. The
coordinator records the actual adoption edit count per call, explicitly zero for
unadopted/unchanged calls. Include every exported call from those collections.
The importer verifies raw export pins, recomputes calls, deduplicates identical
IDs, rejects conflicting values, and updates the active pilot atomically under
its existing lock using `pilot.record_usage`. Repeating the same import is a
no-op; an altered prior call or finished pilot is rejected. It retains import
provenance and reasoning per call; the existing pilot's four-field totals stay
fresh input, cached input, cache writes and output. Reasoning is a separate
exported field, not added to output. Completed length-limited messages remain
in the denominator; unfinished zero placeholders remain null, including cost.
Unknown collections remain explicit in the run index and cannot be imported as
  zero usage. A completed timestamp with an unknown finish still has unknown
  usage; keep the raw export and nullable projection. Importing worker calls does
  not attest complete pilot usage.
A client-reported cost of zero is not a subscription cost measurement. Capture
quota before and after, including window boundaries. Codex coordination/review
tokens remain unknown unless observed independently, so do not attest complete
pilot usage merely because worker usage is available. Finish timing after review
and acceptance, then run one parent closing check.

For review scripts, use the same typed artifact projections via
`py -3.13 scripts/cmre_provider_worker.py project controls BUNDLE.json` or
`project mismatches REPORT.json`. `controls-complete` is a successful selected
control outcome with proposal readiness. A `further mismatches omitted` row is
an omission marker with its case position, never an extra numeric witness.
The projections do not validate full bundles or establish acceptance; use
[CONTROL-PLANS.md](CONTROL-PLANS.md) for those limits.

Fake-process regressions:
`py -3.13 -m pytest scripts/tests/test_cmre_provider_worker.py tests/test_cmre_worktrees.py tests/test_cmre_tasks.py -q`.
The Windows workflow `.github/workflows/provider-tooling.yml` adds the controller,
broker, review and context regression suites to that smoke command. Its test list
is the authoritative asset-free CI invocation. It uses local fake processes and
temporary records, without game content, external provider calls or credentials.
The parent full session check remains required locally; CI is not a
game-dependent acceptance gate.
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

For reasoning-heavy work, scope one guard or ABI question and require an artifact
after the necessary reads. Include the original helper guards whose outcomes the
proposal assumes. A timeout or length stop needs a smaller fresh assignment;
retain the unfinished usage and existing hold. Full instruction and compiler
review remain acceptance obligations after an artifact appears.

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
