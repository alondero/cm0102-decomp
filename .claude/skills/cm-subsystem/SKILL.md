---
name: cm-subsystem
description: Recovers a bounded CM0102 subsystem as connected faithful source. Use when a named subsystem or coherent source slice is requested.
---

# Recover a subsystem slice

Read `docs/agent/SUBSYSTEM.md` and execute its scope, packet, layout, recovery,
review and gate steps. A standalone invocation follows `PLAYBOOK.md` preflight
and closure; a worker inherits the parent's preflight.

Example: for a date/localization slice, identify exact function IDs and residual
CRT/API/data dependencies before claiming subsystem closure. Return the claimed
gate, acceptance receipts, remaining unknowns and the next bounded experiment.
For delegated source work, use the campaign plan and task packets. Report the
change in accepted source bytes and observed composed calls; portability and
live integration remain separate claims.
