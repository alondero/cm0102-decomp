---
name: cm-triage
description: Triage one blocked CM0102 function (fn_XXXXXXXX). Use when asked to diagnose or unblock a specific function, or when a recovery session selects triage.
---

# Triage one function

Read `docs/agent/TRIAGE.md` and execute its procedure for the assigned function.
If unassigned, use its selection step. A worker inherits the parent's preflight;
a standalone invocation also follows `docs/agent/PLAYBOOK.md` preflight and closure.

Done: return the workflow's accepted, blocked, or harness-issue result.
