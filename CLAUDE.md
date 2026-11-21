@AGENTS.md

## Claude Code specifics

- `cm-session` routes continuation; `cm-decompile` runs the C loop
  (`docs/agent/C.md`); `cm-triage`, `cm-subsystem`, `cm-review` and `cm-port`
  select the other workflows. Procedures live in `docs/agent/`; skills are
  adapters. Codex's `cm0102` adapter in `.agents/skills/` uses the same playbook.
- When delegation is authorized, give each `cm-decompiler` worker distinct
  function IDs. Workers write `recovery/c/fn_X.c` and run `cmre test --c`; the
  parent accepts serially (accept holds an exclusive lease) and owns preflight,
  machine jobs and the final gate.
- A PreToolUse guard (`cmre/guard.py`, wired in `.claude/settings.json`) blocks
  edits to protected or generated paths, redirection into them, bare `python`,
  pushes without a passing publication receipt, and name-based process kills.
  Source-only pushes (standing authorization in AGENTS.md: push reviewed batches
  at each handoff) follow `docs/agent/PUBLICATION.md`; receipts bind the exact
  commit and current push URL/visibility.
  It reads whole command text, so keep push syntax out of commit messages and
  heredocs (use `git commit -F <file>`). Edits under `cmre/` and `tests/` need
  no prompt (ADR 0011); `cmre/` edits are refused while a recovery writer holds
  the command lease, so make harness changes in a worktree during farm runs.
- A refusal of a protected write is a hard stop. For a false positive, keep the
  command and refusal as evidence and fix the guard with a regression test
  through the tooling workflow.
