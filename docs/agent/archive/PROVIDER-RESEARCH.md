# External agent runner research

Historical capability research. Current roles, the 150-minute session and its
coordinator-led retro are in [PROVIDER-CAMPAIGN.md](PROVIDER-CAMPAIGN.md); the
implemented adapter is in [PROVIDER-WORKERS.md](PROVIDER-WORKERS.md). The November
2026 pilots exercised both MiniMax and Go; this older assessment is not dispatch
or authentication guidance.

Research checked 2026-10-23. This is a capability assessment, not an enabled
workflow or a claim that these clients have passed this project's harness.
No agent was launched, installed, authenticated, or reconfigured for this note.

## Candidate runners

| Runner | Verified automation capability | Implication for a CM0102 pilot |
| --- | --- | --- |
| MiniMax Code (`mcode`) | First-party coding agent with file edits, terminal tools, and headless `mcode exec`; text, JSON, and streaming JSON output. ACP supports permissions and cancellation. | A direct MiniMax worker candidate; preserve a separate adapter rather than assuming Claude-compatible CLI flags. |
| OpenCode (`opencode`) | `opencode run` accepts a provider/model, agent, files, session ID, and JSON event output. MiniMax documents its Subscription Key integration. | A provider-neutral worker candidate with a comparatively explicit automation interface. |
| Claude Code (`claude`) | `claude -p` supports JSON/streaming JSON, session metadata, schema-constrained output, and continuation. | Can coordinate or serve as a bounded fallback/reviewer through the same job envelope. |
| MiniMax CLI (`mmx`) | Text/media/search API client with JSON output and quota inspection. Its setup command configures other coding agents. | Useful for quota checks or tool-less packet responses; it is distinct from the `mcode` coding-agent runtime. |

Sources: [MiniMax Code features](https://agent.minimax.io/docs/cli/features),
[OpenCode CLI](https://opencode.ai/docs/cli/),
[MiniMax's OpenCode integration](https://platform.minimax.io/docs/m-plan/opencode),
[Claude headless](https://code.claude.com/docs/en/headless),
[MiniMax CLI source README](https://github.com/MiniMax-AI/cli/blob/main/README.md).

## Windows and process control

MiniMax Code publishes a Windows PowerShell installer and Windows launchers.
Its documentation distinguishes interactive session resume from headless execution;
the pilot must inspect the installed `mcode exec --help` and verify exact output,
session-ID, timeout, and exit-code behavior before committing an adapter. ACP
cancellation is documented, but headless process-tree cancellation was not
established by the pages reviewed.
[Quick start](https://agent.minimax.io/docs/cli/quick-start),
[source README](https://github.com/MiniMax-AI/minimax-code/blob/main/README.md),
[ACP capabilities](https://agent.minimax.io/docs/cli/features).

OpenCode runs natively on Windows, although its documentation recommends WSL.
For this Windows-native Python/toolchain workspace, native execution is the
first pilot choice (a project recommendation, not OpenCode's recommendation).
Its documented server API exposes session creation, status, asynchronous prompts,
events, and `POST /session/:id/abort`. A persistent server requires explicit
session abort: terminating a client is not sufficient evidence that its worker
stopped. Verify this experimentally.
[Windows guidance](https://opencode.ai/docs/windows-wsl),
[server API](https://opencode.ai/docs/server/).

Claude Code supports native Windows and WSL. Headless JSON includes a session ID;
streaming output includes a final result. The docs distinguish interrupting a
turn from SIGTERM, which leaves the current turn unfinished. Native Windows
termination and descendant cleanup still need a local integration test.
The documented `--bare` mode skips discovery including CLAUDE.md and subscription
OAuth credentials, so it cannot be added casually to a subscription-backed
worker invocation.
[Windows setup](https://code.claude.com/docs/en/setup),
[headless lifecycle](https://code.claude.com/docs/en/headless).

OpenCode's public documentation has both the current main documentation and a
separate v2 documentation surface with different commands/API conventions. Pin
the installed release and its help/API schema; do not combine examples from
different generations. This is an inference from the two official surfaces.
[Main CLI](https://opencode.ai/docs/cli/),
[v2 CLI](https://dev.opencode.ai/v2/docs/cli/commands/).

## Billing and capacity

MiniMax now documents M Plan terminology; some integration identifiers retain
`coding-plan` or Token Plan naming. A Subscription Key consumes plan usage and
eligible credit packs; a standard API Key charges the pay-as-you-go balance.
They are distinct credentials. MiniMax Code can use membership by signing in;
other tools use the Subscription Key. Several tools may use one subscription
concurrently, but all share its quotas. More processes do not create more quota.
The FAQ describes usage windows, RPM/TPM throttling, and dynamic limits; it does
not establish a guaranteed concurrent-worker allowance for this account.
[MiniMax M Plan FAQ](https://platform.minimax.io/docs/m-plan/faq).

MiniMax's OpenCode guide explicitly selects the `MiniMax Token Plan (minimax.io)`
provider. Its configuration identifier is `minimax-coding-plan`; its
Anthropic-compatible adapter uses `https://api.minimax.io/anthropic/v1`.
Do not silently substitute the generic API provider or an arbitrary model ID.
Discover and pin the model actually available to the selected account.
[Integration guide](https://platform.minimax.io/docs/m-plan/opencode).

## Recommended experiment

Project recommendation: start with two OpenCode/MiniMax workers as specified in
[the implementation proposal](MULTI-PROVIDER-PROPOSAL.md). Later compare MiniMax
Code and OpenCode in separate matched cohorts at equal concurrency. Keep the
task contract, source evidence, harness, and acceptance policy identical. Measure
accepted bytes per elapsed hour, coordinator review time, retry rate, and actual
quota/cost consumption. Neither first-party status nor a model's advertised
context size establishes decompilation quality.

Before scaling, demonstrate structured completion, disallowed-write detection,
timeout/cancellation cleanup, quota-exhaustion handling, and recovery after a
coordinator restart. Use a fresh session per bounded batch and retain provider,
model, CLI version, process ID, provider session ID, logs, and artifact hashes
in the coordinator's local job record. These are proposed acceptance criteria,
not claims about implemented infrastructure.
