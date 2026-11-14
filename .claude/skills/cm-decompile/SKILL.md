---
name: cm-decompile
description: Translate CM0102 lifted functions to plain C by hand and accept them (brief, write, test, accept loop). Use for a named fn_XXXXXXXX or a by-hand batch; bulk continuation uses the farm via cm-session.
---

# Translate functions to C

Read `docs/agent/C.md` and follow its family/subsystem selection when no IDs are
assigned. Run its loop over the bounded IDs. Keep going to the next ID
after each accept. A standalone invocation also follows `docs/agent/PLAYBOOK.md`
preflight and closure; a worker inherits the parent's preflight and does not accept.

Done: one line per ID, `fn_X c-accepted|pass|wip: <detail>`, plus the closing
check's `Progress since` line for a standalone session.
