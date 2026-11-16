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

## Local inventory entry revision (recovery contributors only)

The current recovery inventory selects one coherent seeded analysis/map pair
under `CM0102/Analysis/cmre/supply-sol-commandcode-20261115/parallel-virtual-entry/`.
Keep the historical `baseline-c` and `recovery-map-b` inputs and receipts intact.
These generated files, the copied executable, projects and pseudocode remain
local; do not publish them. The selected exports are not game build inputs.

After the reviewed seed-tool bundle and pinned Ghidra/Java prerequisites are
installed, a clean workspace with the user-supplied 3.9.68 executable can
attempt to regenerate the selected pair as follows (the output directories must
not exist; preserve any failed run, and use a fresh workspace or restore the
verified originals rather than overwriting existing evidence):

```powershell
$rev = "CM0102/Analysis/cmre/supply-sol-commandcode-20261115/parallel-virtual-entry"
py -3.13 scripts/analyze_seed_prototype.py --target CM0102/Game/cm0102.exe --ghidra tools/ghidra/ghidra_12.1.4_PUBLIC --output "$rev/current-identity-fresh-f"
py -3.13 scripts/map_seed_prototype.py --baseline "$rev/current-identity-fresh-f" --target CM0102/Game/cm0102.exe --ghidra tools/ghidra/ghidra_12.1.4_PUBLIC --output "$rev/current-identity-map-f" --functions 0051a150 0051c020
```

The map wrapper currently requires Java 21.0.10 at its documented Windows host
path; fresh analysis may require the same observed host symbol/environment
inputs. Both commands check the pinned executable; the seeded analysis also rejects
an unexpected seed result. Run them only when their reviewed scripts are present; neither a
source-only clone nor the commands themselves recreate acceptance evidence.
Before running recovery, verify all three selected hashes in `cmre/env.py`;
matching outputs on this host do not prove cross-host reproducibility. If any
required data, tool or hash is unavailable, report the check unavailable or
failed; do not change pins, fetch game files, or substitute another map. After
an authorized inventory revision use fresh processes, because inventory data
is cached in-process. Release revalidation remains a separate requirement.

## Retaining and restoring withdrawn local C evidence

`py -3.13 -m cmre retract-c fn_X --reason "..." --witness PATH` previews withdrawal
of invalid C validation while preserving the source and validated lift. Applying
the reviewed preview requires `--apply --expect-plan SHA256`. Code, policy,
evidence or current hint changes invalidate that plan, including creation of a
previously absent hint file. Withdrawal never reaccepts C automatically.
Inactive withdrawal history retains the original comparison profile and input
policy, including legacy `v1` behavior, so withdrawal cannot silently select a
newer policy for later tests. Missing or malformed retained policy fails closed;
changing it requires a separate reviewed migration.

Before publishing the ledger change, the command retains an immutable hashed
archive under `CM0102/Analysis/cmre/c-retractions/`. Back up that complete tree
with the corresponding local ledgers, source, lift, hints, work state and retained
reports. Do not clean these archives as scratch: `cmre verify` checks them through
the generated ledger's withdrawal history. Keep partial or failed archives too.

For a local workspace restore or relocation, restore the matching recovery files
and complete archive tree at the same paths relative to the new workspace root.
Preserve bytes and manifest hashes; do not rewrite history or manufacture success
labels. Archive references resolve relative to the current workspace, so the old
machine's absolute paths are not verification dependencies. Run `cmre doctor` and
`cmre verify` after restoring the required user-supplied inputs. Missing or changed
archives are an unavailable or failed evidence check, never a pass.

An archive supplies the exact prior ledger and provenance for investigation and
rollback review. It does not authorize restoring an invalid C certificate. Resolve
the retained mismatch and explicitly retry its work hold before normal testing
and acceptance. If publication of the final operation receipt fails after the
ledger change, `applied-receipt-incomplete` reports that withdrawal occurred;
inspect the ledger and archive before retrying.

These are private/local recovery backups. A source-only clone deliberately excludes
generated ledgers, raw witnesses and these archives, as specified by
[the publication workflow](agent/PUBLICATION.md). It need not carry or publish
them. Restore trusted local recovery evidence separately when recreating an
authoritative recovery workspace; reconstructed source alone is not acceptance
evidence.

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
