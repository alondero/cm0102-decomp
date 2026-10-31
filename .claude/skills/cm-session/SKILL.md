---
name: cm-session
description: Routes CM0102 faithful recovery and native modern port work after preflight. Use for "continue the decompilation", "make progress", or a project work session.
---

# Recovery session

Read `docs/agent/PLAYBOOK.md` and execute its preflight, routing and closure.
Read only the selected workflow. The default continuation is the C loop in
`docs/agent/C.md`: keep translating and accepting functions until the budget is
used; do not open new tooling or investigation work unless the user asks.

When the user authorizes delegation, give each `cm-decompiler` worker distinct
function IDs selected by C.md's family/subsystem queue. Bound worker contexts as
specified in the playbook. Workers return `fn_X pass|wip`; accept reviewed passing
IDs yourself, serially, with `py -3.13 -m cmre accept fn_X --c`. Own preflight,
machine jobs, harness issues and the final `scripts/cmre_session.py check`.
