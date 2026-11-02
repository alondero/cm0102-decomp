# CM 01/02 faithful decompilation — agent contract

Goal: decompile Championship Manager 01/02 **3.9.68** (`CM0102/Game/cm0102.exe`,
SHA-256 `0c78e7da…f86`, preferred image base `0x00400000`) into buildable source
that preserves original behaviour and bugs, then develop a native modern port.
The sequence is recovered routines → validated subsystems → standalone faithful
game → modern port. Modernization starts after the M6 standalone gate; port
planning may document boundaries earlier. For milestone work, read the
**Milestones and acceptance gates** table in [docs/ROADMAP.md](docs/ROADMAP.md),
then the relevant gate. Method and tool decisions:
[docs/DECOMPILATION-STRATEGY.md](docs/DECOMPILATION-STRATEGY.md).

Users supply their own supported CM0102 installation. Publish the reconstructed
engine and tooling; import original assets and databases locally. At M6, game
logic runs from rebuilt source without executing or emulating original code.
Original files may remain inputs to local extraction and differential tests.

## How to work (read this, then the playbook)

1. Run `py -3.13 scripts/cmre_session.py start` once per parent session: doctor,
   status, baseline pytest, with an exit-code receipt. Resumes reuse that receipt;
   workers inherit it. Use `py -3.13` for all Python commands.
2. Route the user's task through [docs/agent/PLAYBOOK.md](docs/agent/PLAYBOOK.md).
   Read the selected workflow only. Explicit tasks take priority over queued work.
3. Run `py -3.13 scripts/cmre_session.py check`. Green means **both pytest and
   cmre verify exit 0**. Report baseline failures as failures; name-only `RED`
   tests are ordinary failing tests.

Use live `cmre status` for counts. For continuation, assess the local
[docs/NEXT-SESSION-PROMPT.md](docs/NEXT-SESSION-PROMPT.md) against the baseline when
present. This handoff stays local; use the campaign workflow when it is absent.
For session reviews, consult relevant sections of `docs/HISTORY.md` and local
transcripts; routine recovery uses `cmre show`, `explain`, and `next`.

For project continuations, prioritize accepted C and dependency closure for one
observable game behavior through [docs/agent/CAMPAIGN.md](docs/agent/CAMPAIGN.md).
Report accepted C/legacy bytes, lift-inclusive recovery, composed calls and live
integration separately. Reuse unchanged evidence; add checks for a named gap.

## Fidelity rules (non-negotiable)

- Recover behaviour from original instructions only. Never invent engine logic,
  plausible simulation rules, Windows API behaviour, safe defaults or
  missing initialization. Unknown stays explicitly unknown or calls original code.
- Preserve integer widths, signedness, overflow, layout, calling conventions,
  floating point, RNG state and call order, and observable original bugs.
- Track identified / decompiled / compiled / validated / integrated separately.
  Only `cmre accept` marks a function recovered. Lift coverage is not readable
  source coverage or proof of a standalone game.
- Callees and imports remain explicit boundaries until the standalone gate.
  Import stubs inside tests are scaffolding, never evidence.
- Address claims state the convention (VA, RVA or file offset) and the binary hash.

## Preservation rules

- Never modify `CM0102/Game/`, `CM0102/Original-3.9.60/`, `CM0102/InstallMedia/`
  or `CM0102/Inspection/` manifests. Copy binaries before experimenting.
- Legacy hand-built records (`recovery/{date,lang,crt,rng}/`, `recovery/registry.json`)
  are frozen. Generated records (`recovery/ledger|lifted|blocked/`, `docs/STATUS.md`)
  are written only by `cmre`.
- Agent work goes in `recovery/c/` and `recovery/hints/`; claim edited hints by
  removing `"auto": true`. Workflow details belong in `docs/agent/`.
- `CM0102/Analysis/` and `CM0102/Runtime/` are ignored local work areas. Retain and
  back up acceptance evidence, raw witnesses, receipts and required reference
  states before cleaning scratch files; ignored does not mean disposable.
- Do not apply community patches; community offsets are hypotheses until verified
  against the pinned bytes ([docs/COMMUNITY-SOURCES.md](docs/COMMUNITY-SOURCES.md)).

## Publication and reproducibility

- Reviewed reconstructed source, necessary assembly, tests, tooling and focused
  documentation are eligible for publication. Review content and provenance;
  a source extension does not turn a raw dump into authored reconstruction.
- Supplied executables/media, original or extracted game content, credentials
  and raw analysis stay local, outside published history and packages.
- Commit reviewed batches using explicit paths when requested. Push only when
  requested, after the publication workflow scans the exact commit and reachable
  history for the intended remote. Reuse authorization for that batch.
  `cmre task publish` remains local acceptance, separate from Git publication.
- Preserve accepted source bytes and evidence while resolving scanner findings.
  For source reviews, scans, evidence retention and pushes, follow
  [docs/agent/PUBLICATION.md](docs/agent/PUBLICATION.md).
- Keep a documented clean-clone setup using user-supplied files. Validate supported
  versions, leave supplied installations unchanged, regenerate local inputs and
  report missing checks explicitly. Distinguish current recovery setup from the
  future game build in [docs/BUILDING.md](docs/BUILDING.md). Missing game-dependent
  checks are unavailable, never passing. Do not fetch replacement game content.
