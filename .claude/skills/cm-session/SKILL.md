---
name: cm-session
description: Routes CM0102 faithful recovery and native modern port work after preflight. Use for "continue the decompilation", "make progress", using model credits, or a project work session.
---

# Recovery session

Run `py -3.13 scripts/cmre_session.py start`, then follow `docs/agent/FARM.md`:
start the farm in the background, check `status` about hourly, and spend your
own turns on its supply lane (or on the user's explicit task). The farm
dispatches, tests, accepts and audits; do not coordinate functions by hand.

For any other task, read `docs/agent/PLAYBOOK.md` and only the workflow its
routing table selects. End with `cmre_farm.py stop`, the closing
`scripts/cmre_session.py check`, and the short handoff FARM.md describes.
