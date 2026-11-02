# Builds with user-supplied CM0102 files

The intended distribution contains reconstructed engine source, support code,
tests and tools. Each user supplies a supported original installation for game
assets and databases. Local extraction may use the original executable; the
completed engine runs rebuilt source and has no original-code fallback.

## Current availability

This repository is a recovery workspace, not yet a buildable complete game or
modern port. [STATUS.md](STATUS.md) reports local recovery; it does not establish
M6. There is currently no complete clean-clone bootstrap that recreates the
authoritative recovery workspace, and no end-user game build command. Do not
present the analysis commands below as either capability.

A source-only checkout intentionally lacks supplied content, raw analysis,
generated ledgers, lifted assembly, local hints/fixtures and reference saves.
Published `recovery/c/STATUS.tsv`, when present, identifies source hashes and
accepted/WIP status; it is not a replacement for acceptance evidence.

## Recovery contributor setup

1. Obtain the Python and MinGW dependencies described in [README.md](../README.md).
   Use Python 3.13 for the current recovery tooling. Full analysis also needs the
   pinned Ghidra/Java setup in [ANALYSIS-SETUP.md](ANALYSIS-SETUP.md).
2. Supply an official 3.9.68 installation locally under `CM0102/Game/`, preserving
   the source installation. Verify `cm0102.exe` SHA-256 against
   `0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.
   Other revisions are unsupported until separately identified and validated.
   Do not download substitute game files or patch unsupported inputs automatically.
3. Follow [ANALYSIS-SETUP.md](ANALYSIS-SETUP.md) for fresh analysis copies and
   [RECOVERY-MAP.md](RECOVERY-MAP.md) for call-map reproduction. These tools refuse
   existing output directories; retain failed attempts. Their outputs must match
   the current pinned inventory before they can drive acceptance. Renaming a new
   directory to an expected baseline path does not establish compatibility.
4. Restore retained local acceptance evidence from a verified backup, or perform
   the actual recovery/validation steps needed to regenerate it. Accepted ledgers
   cannot be inferred from source presence or reconstructed by assigning old
   success labels. Publication scans also need the supplied comparison files.
5. With all required local inputs present, run `py -3.13 -m cmre doctor`, then use
   the parent session workflow in [agent/PLAYBOOK.md](agent/PLAYBOOK.md).

Existing scripts assume workspace paths and some historical verification commands
also require original media/inspection manifests. They do not yet provide a
general installation selector. A source-only clone alone cannot run the full
recovery acceptance suite; report absent prerequisites rather than inventing them.

## Checks without game files

The publication/guard tests use synthetic files and temporary repositories:

```powershell
py -3.13 -m pytest tests/test_cmre_publication.py tests/test_cmre_guard.py
```

Optional configured-host hook checks report skips when the ignored local Codex
configuration is absent. These skips are unavailable host integration checks,
not passing hook-installation evidence; the synthetic guard checks still run.

The full session suite also exercises game-dependent recovery and local evidence.
List the commands actually run and explicitly identify unavailable or skipped
checks. A passing synthetic subset is not a green recovery session, successful
game build or fidelity result. Public CI must use an explicitly asset-free subset;
game-dependent CI requires separately provisioned local/private inputs.

## Acceptance contract for the future game bootstrap

A clean-clone smoke test must demonstrate: user-selected installation; version
and required-file validation; unchanged supplied files; import/extraction into
ignored local directories; regeneration of required build inputs; documented
toolchain build; and startup of the source-built engine using those assets.
Missing/unsupported files must produce actionable errors. The build/import flow
must not fetch or bundle original game content. Local extraction outputs remain
local, including content embedded in the original executable.

Game-building prerequisites and contributor analysis prerequisites must be
documented separately so players do not need private analysis sessions. M6 still
requires the whole-game parity gates in [ROADMAP.md](ROADMAP.md); setup success
alone does not establish engine completeness.
