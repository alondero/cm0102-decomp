# Contributing

This project recovers Championship Manager 01/02 version 3.9.68 behavior from
the original executable. It is not yet a standalone game. Start with
[README.md](README.md), [BUILDING.md](docs/BUILDING.md) and
[the project contract](AGENTS.md).

Useful contributions include tooling fixes, documentation, synthetic regression
tests and evidence-based reconstruction. Open an issue describing the intended
change before substantial recovery work so the maintainer can coordinate it
with active workers. Describe AI assistance and the evidence used; generated
code alone is not validation.

Do not upload game executables, installers, databases, graphics, sounds, saves,
raw disassembly, original instruction bytes, local evidence or credentials in
commits, issues, pull requests, attachments or Actions artifacts. Use function
IDs, hashes and concise authored explanations. Keep supplied installations
unchanged. Do not obtain replacement game content for tests.

Follow the setup and asset-free checks in [BUILDING.md](docs/BUILDING.md).
State the checks actually run and any missing local inputs. A public CI pass
does not establish behavioral fidelity. Recovery changes need the maintainer's
local differential tests and `cmre accept`; do not edit generated ledgers or
claim acceptance based on source presence.

Keep changes focused and stage explicit paths. Include the reason for the
change, evidence, test results and known limitations in the pull request.
The maintainer scans candidate content and its reachable Git history using
[the publication workflow](docs/agent/PUBLICATION.md) before publishing.

Contribute only material you have the right to submit under the project's
[MIT License](LICENSE). Preserve third-party notices and identify any separate
terms. The project license does not grant rights in original game content;
see [license scope](docs/LICENSING.md).
