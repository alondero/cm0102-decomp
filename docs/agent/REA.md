# REA binary investigation

[REA](https://github.com/morluto/rea) supplements the existing static exports
with interactive disassembly, pseudocode, callers, callees and references.
Use it for targeted questions about the pinned executable. Recovery and
acceptance still follow the [playbook](PLAYBOOK.md) and `cmre` gates.

## Local setup

The installed CLI is `rea-agents` 6.4.0. The project launcher
[`scripts/rea.ps1`](../../scripts/rea.ps1) selects the existing Ghidra 12.1.4
installation under `tools/ghidra/` and JDK 21.0.10 under
`C:/Program Files/Java/jdk-21.0.10`. It restores the caller's environment after
each invocation. No additional analysis engine or Ghidra extension is needed.

The ignored `.codex/config.toml` registers the same launcher as the project's
`rea` MCP server. Its existing configuration was backed up beside it before
the registration was added. Claude Code has a local-scope `rea` entry for this
workspace in `<your user profile>/.claude.json`, added on 2026-10-14 after backing up
that file. Both override the global REA launcher, whose provider settings belong
to another workspace. Global entries and the existing REA skill are preserved.
After changing a registration, reconnect/restart the client and verify its live
server identity; a config file alone does not establish a connection.

```powershell
# Prepare or verify the analysis copy; prints its absolute path.
.\scripts\rea.ps1 prepare

# Check the provider actually used here; require exit 0 and healthy: true.
.\scripts\rea.ps1 doctor --provider ghidra --json

# Show Codex's effective project registration.
codex mcp list --json

# Show Claude's local registration and connection health.
claude mcp get rea
```

Doctor's `environment_healthy` includes other agents and optional providers.
For this workflow, assess the explicitly requested Ghidra scope and its exit
code. Registration checks alone do not prove that a live MCP server connects.

On another Windows machine, install `rea-agents@6.4.0` with npm if it is absent,
then provision the pinned Ghidra/JDK using [analysis setup](../ANALYSIS-SETUP.md).
Adjust the launcher's JDK location and append this registration to the existing
project configuration, using that machine's absolute workspace path:

```toml
[mcp_servers.rea]
command = 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
args = ['-NoLogo', '-NoProfile', '-NonInteractive', '-File', '<repository>\scripts\rea.ps1', 'mcp']
startup_timeout_sec = 60
tool_timeout_sec = 1200
```

For Claude Code, back up its configuration and register this same launcher in
local scope, substituting the machine's workspace path:

```powershell
claude mcp add --scope local rea -- C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe -NoLogo -NoProfile -NonInteractive -File <repository>\scripts\rea.ps1 mcp
```

Local scope overrides a user entry for this workspace; it does not change other
projects. See [Claude scope precedence](https://code.claude.com/docs/en/mcp#scope-hierarchy-and-precedence).
The current package release is newer than the installed CLI. Upgrade only for a
needed capability/fix, after reviewing the [tool decision](../DECOMPILATION-STRATEGY.md)
and preserving existing Evidence and client configuration.

The launcher allows 900 seconds for cold Ghidra startup. The first native query
can import and analyze the whole executable. Keep one MCP session open for
related queries; separate CLI commands can each start a fresh Ghidra import.
These REA projects are separate from the accepted baseline exports.

## Reuse evidence before new analysis

The later recovery sessions have canonical bundles and selected native extracts
under `CM0102/Analysis/rea/`. Inspect their target hash, provider/profile and
limitations before adopting a result. A bundle stores Evidence; a snapshot
replays eligible observations with an exactly matching target/profile. Neither
is a persistent Ghidra project or proof of complete behavioral coverage.

The reviewed 2026-10-14 bundle is `session80-20261014-with846920.json`.
To inspect its existing function dossier without starting Ghidra, save this
request as UTF-8 JSON **without BOM** in ignored analysis storage:

```json
{
  "path": "<repository>/CM0102/Analysis/rea/session80-20261014-with846920.json",
  "detail": "summary",
  "filters": {
    "operation": "analyze_function",
    "target_sha256": "0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86",
    "procedure_address": "0x846920"
  }
}
```

Then run `./scripts/rea.ps1 inspect-evidence-bundle <absolute-request-json>`.
Use `Path.write_text(..., encoding="utf-8")` from `py -3.13` or .NET
`[IO.File]::WriteAllText($path, $json, [Text.UTF8Encoding]::new($false))` to create
the request. Windows PowerShell's `Out-File -Encoding utf8` adds a BOM that the
installed CLI input parser rejects.
The installed CLI validates the whole bundle before returning a small summary;
allow for its memory/time cost. The address selector is REA's canonical spelling
of preferred-image VA `0x00846920`, image base `0x00400000`, for the pinned hash.
Use `inspect-analysis-view` or existing `session80-fn00846920-*.json` facet extracts
for the specific unanswered question, retaining their parent dossier identity.

That dossier retains complete records but reports truncated high-pcode with
known omitted edges. Complete retention does not remove analysis limitations.
Preserve non-contiguous body ranges too: an address span is not body-byte coverage.

## Session ownership and handoff

One owner controls the active native MCP target and lifecycle. When workers are
authorized, give them exported Evidence for independent reading; they do not
open competing targets or close the parent's session. Parallelizing source
reading is distinct from sharing a mutable native-analysis session.

A reviewable investigation receipt records target/copy hashes, VA convention
and module base, REA/server version, Ghidra/profile identity, bundle path/hash,
parent Evidence IDs, exact queries/view selectors, body gaps/truncation, and
remaining unknowns. Record compile, differential-validation, acceptance and
integration results separately. REA's `verify_reconstruction` does not establish
the project's recovery or milestone gates.

## Bind the preserved target

The launcher checks the original and prepares
`CM0102/Analysis/rea/input/cm0102.exe`. Both must have SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
It refuses an existing copy with different bytes. Preserve a mismatched copy
for diagnosis before preparing a replacement. All REA outputs belong under
the ignored `CM0102/Analysis/rea/` directory.

For a connected MCP client:

1. Inspect `binary_session` with `{}` when the live target or connection is
   unknown. Check its actual tool availability and server identity.
2. Open the copied target with `open_binary`:

   ```json
   {"path":"<repository>/CM0102/Analysis/rea/input/cm0102.exe","provider_id":"ghidra"}
   ```

3. Use the connected server's advertised schemas. For example,
   `analyze_function` takes `{"procedure":"0x00946A04"}`. This is the LCG
   helper documented in [RNG evidence](../RNG-INIT.md), addressed by
   **preferred-image VA**, image base `0x00400000`, for the hash above.
4. Retain complete Evidence, including IDs and limitations. For a large
   dossier, use `inspect_analysis_view` with the returned Evidence reference.
5. Close the session with `close_binary`. To retain eligible query results,
   supply a new absolute `snapshot_path` under `CM0102/Analysis/rea/`.
   Loading that path in a later `open_binary` reuses matching observations;
   a snapshot is a result cache, not a persistent Ghidra database.

Windows Ghidra supports a read-only analysis boundary. Availability varies by
operation; consult `binary_session` before depending on a capability.
Pseudocode is an analysis hypothesis. Check original instruction bytes,
integer widths, calling conventions and gaps before translating to C, then
validate through the appropriate `cmre` workflow. Only `cmre accept` records
recovery.

## Terminal queries

```powershell
$reaTarget = .\scripts\rea.ps1 prepare
.\scripts\rea.ps1 instructions $reaTarget 0x00946A04 --provider ghidra --json
.\scripts\rea.ps1 function $reaTarget 0x00946A04 --provider ghidra --json
.\scripts\rea.ps1 decompile $reaTarget 0x00946A04 --provider ghidra --json
```

Use `--snapshot` with an absolute local JSON path to retain eligible CLI
results. Save JSON as UTF-8 without BOM, and inspect `$LASTEXITCODE` after the
producer. CLI JSON input files reject a BOM; use the write methods above rather
than Windows PowerShell's `Out-File -Encoding utf8` for reusable inputs.
The upstream [CLI guide](https://github.com/morluto/rea/blob/main/docs/cli.md)
and [Windows provider guide](https://github.com/morluto/rea/blob/main/docs/windows-ghidra-p0.md)
describe the supported boundaries. Prefer the installed version's help and
schemas when repository main differs.

## Setup verification

The 2026-10-11 setup exercised the project MCP launcher, advertised tools,
target binding and `analyze_function` on the example VA above. Ghidra 12.1.4
returned seven assembly instructions, pseudocode and caller references for
the 30-byte function. The cold query took about 12 minutes on this host.
The original and copied executable hashes remained unchanged, and closing
the session removed its private runtime directory.

Local receipts and complete Evidence are under `CM0102/Analysis/rea/`:
`smoke-receipt.json`, `analyze_function.json` and `smoke-snapshot.json`.
The matching result can be reused from the terminal:

```powershell
$reaTarget = .\scripts\rea.ps1 prepare
$reaSnapshot = (Resolve-Path 'CM0102/Analysis/rea/smoke-snapshot.json').Path
.\scripts\rea.ps1 function $reaTarget 0x00946A04 --provider ghidra --snapshot $reaSnapshot --json
```

This verifies the setup and one native query; other investigations still need
their own evidence and the project's recovery gates.

The 2026-10-14 infrastructure review found the live Codex server on REA 6.4.0
with the expected Ghidra provider and matching skill version 37. The scoped
Ghidra doctor passed, the retained-bundle query above exited 0, and Claude's
new local override reported connected. No cold import, package upgrade or
additional engine installation was needed. Repeat a bounded native smoke query
after a provider/profile change or when the requested binary question requires
new analysis, rather than repeating the 12-minute import for every session.
