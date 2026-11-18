# Championship Manager 01/02 faithful recovery

This project recovers the official 3.9.68 executable into buildable source that preserves its behavior and bugs. A validated standalone game precedes any modern port. The pinned original has SHA-256 `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`; its preferred image base is `0x00400000`.

Users supply their own supported CM0102 installation for assets and databases.
The completed engine runs source-built game logic; original files may be used
for local extraction and validation. The complete game/port and general clean-clone
bootstrap are not available yet. See the [build/setup contract](docs/BUILDING.md)
and [publication workflow](docs/agent/PUBLICATION.md). Recovery tooling currently
requires Python 3.13 with `unicorn`, `capstone`, `pefile`, `numpy`, `pytest`, and
the documented MinGW/Ghidra dependencies.

## Progress and contributing

This is an AI-assisted behavioral decompilation, not a byte-matching project.
Accepted C/legacy coverage is weighted by original function body bytes; it does
not mean that the same percentage of a playable game is finished. Lift-inclusive
recovery, composed calls and live integration are separate measurements.
See [current recovery status](docs/STATUS.md), the
[live progress atlas](https://alondero.github.io/cm0102-decomp/)
([offline copy](public/progress/index.html)), the
[progress atlas and hosting instructions](docs/agent/PROGRESS.md), and the
[public launch and tracker assessment](docs/PUBLIC-LAUNCH.md).

Read [CONTRIBUTING.md](CONTRIBUTING.md) before submitting changes. Game files,
databases, extracted content and raw analysis must remain on your own machine.
Project contributions are offered under the [MIT License](LICENSE), to the
extent contributors hold the relevant rights. Original game content is excluded;
see [license scope and precedents](docs/LICENSING.md).

## Local workspace and repository

The authoritative recovery checkout is on the same machine as the supplied game and local analysis. Its agents follow the local `AGENTS.md` and `docs/agent/PLAYBOOK.md`. Use `py -3.13` for Python. The executable, supplied assets, raw analysis, generated lifts, recovery records, hints, and evidence remain local and are not distributed by this repository.

This source repository contains reviewed recovery tooling, tests, scripts, workflow documentation and a subset of reconstructed C and legacy sources. It is not a complete game or an independently runnable copy of the authoritative recovery workspace: original inputs and full local acceptance records are intentionally absent. `recovery/c/STATUS.tsv` records the published source hashes and accepted/WIP status. [docs/PUBLICATION.md](docs/PUBLICATION.md) defines the publication boundary, the scan every commit and push must pass, the local-only components, and historical migration evidence. A clean Git status does not mean those local-only or unreviewed files are backed up or synchronized. See [issue #22](https://github.com/alondero/cm0102-decomp/issues/22) for the completed source migration.

The local authoritative checkout also uses exact Git exclude rules for withheld recovery sources. Review a file and its contents before explicitly adding it; never publish supplied assets, raw executable bytes, original instruction streams, local analysis, or credentials. The prior local Git history is retained separately and must not be pushed to this repository.

## Parallel work

See the [issues](https://github.com/alondero/cm0102-decomp/issues) and
[recovery farm workflow](docs/agent/FARM.md). The authoritative workspace uses
coordinated workers and acceptance gates. External contributions need maintainer
review and local validation before they count as recovered source.

## Local checks

For a visual overview, run `py -3.13 -m cmre progress` and open
`CM0102/Analysis/cmre/progress/index.html`. The interactive treemap tracks accepted
C/legacy, lifts, composed calls and live integration separately, with search and
history. Successful session checks refresh it automatically. See
[visual progress](docs/agent/PROGRESS.md) for worktree use and sharing.

For live farm activity, run `py -3.13 scripts/cmre_farm_dashboard.py serve` and open
[the local pipeline dashboard](http://127.0.0.1:8042/). See the
[farm dashboard guide](docs/agent/FARM.md#live-pipeline-dashboard) for capacity,
supply, and reporting expert assignments.

From the authoritative checkout, start a parent session with `py -3.13 scripts/cmre_session.py start`. The C workflow uses `py -3.13 -m cmre next --c`, `cmre brief`, `cmre test --c`, and `cmre accept --c`. Close the session with `py -3.13 scripts/cmre_session.py check --parent RECEIPT`; both pytest and `cmre verify` must pass. The local `docs/ROADMAP.md` defines the runtime, integration, subsystem, and standalone parity gates. Accepted C, lifts, composed calls, and live integration are separate measures.
