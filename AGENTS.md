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

## How to work (read this, then the playbook)

1. Run `py -3.13 scripts/cmre_session.py start` once per parent session: doctor,
   status, baseline pytest, with an exit-code receipt. Resumes reuse that receipt;
   workers inherit it. Use `py -3.13` for all Python commands.
2. Route the user's task through [docs/agent/PLAYBOOK.md](docs/agent/PLAYBOOK.md).
   Read the selected workflow only. Explicit tasks take priority over queued work.
3. Run `py -3.13 scripts/cmre_session.py check`. Green means **both pytest and
   cmre verify exit 0**. Report baseline failures as failures; name-only `RED`
   tests are ordinary failing tests.

Use live `cmre status` for counts. For continuation, check
[docs/NEXT-SESSION-PROMPT.md](docs/NEXT-SESSION-PROMPT.md) (at most 40 lines)
against the baseline, then run the [C loop](docs/agent/C.md): `cmre next --c`,
`cmre brief`, write C, `cmre test --c`, `cmre accept --c`, repeat. Use `cmre`
commands rather than ad-hoc probe scripts. Report accepted C/legacy bytes,
lift-inclusive recovery, composed calls and live integration separately.

## Fidelity rules (non-negotiable)

- Recover behaviour from original instructions only. Never invent engine logic,
  plausible simulation rules, Windows API behaviour, safe defaults or
  missing initialization. Unknown stays explicitly unknown or calls original code.
- Preserve integer widths, signedness, overflow, layout, calling conventions,
  floating point, RNG state and call order, and observable original bugs.
- Track identified / decompiled / compiled / validated / integrated separately.
  Only `cmre accept` marks a function recovered. New C is compared under the
  `semantic` profile ([ADR 0002](docs/adr/0002-semantic-c-profile.md)); Ghidra
  drafts are input, never evidence. Lift coverage is not readable
  source coverage or proof of a standalone game.
- Callees and imports remain explicit boundaries until the standalone gate.
  Import stubs inside tests are scaffolding, never evidence.
- Address claims state the convention (VA, RVA or file offset) and the binary hash.

## Preservation rules

- Never modify `CM0102/Game/`, `CM0102/Original-3.9.60/`, `CM0102/InstallMedia/`
  or `CM0102/Inspection/` manifests. Copy binaries before experimenting.
- Publish only reviewed authored source and tooling, through the fresh
  `github-main` history to the private `origin`; never push the prior local
  `main`. Stage explicit paths, and scan before committing and pushing with
  `cmre publication scan`; the guard admits no unscanned push
  ([docs/PUBLICATION.md](docs/PUBLICATION.md)). The supplied game, analysis and
  generated records stay local. `cmre task publish` is the coordinator's local
  acceptance operation.
- Legacy hand-built records (`recovery/{date,lang,crt,rng}/`, `recovery/registry.json`)
  are frozen. Generated records (`recovery/ledger|lifted|blocked/`, `docs/STATUS.md`)
  are written only by `cmre`.
- Agent work goes in `recovery/c/` and `recovery/hints/`; claim edited hints by
  removing `"auto": true`. Workflow details belong in `docs/agent/`.
- `CM0102/Analysis/` and `CM0102/Runtime/` are disposable local work areas (ignored).
- Do not apply community patches; community offsets are hypotheses until verified
  against the pinned bytes ([docs/COMMUNITY-SOURCES.md](docs/COMMUNITY-SOURCES.md)).
