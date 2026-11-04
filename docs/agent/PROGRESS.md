# Visual recovery progress

The recovery atlas is a self-contained interactive HTML report. It has a byte-weighted
treemap, candidate-module and eight-shard grouping, search, drill-down, a keyboard-accessible
function index, light/dark themes and a history chart. It works from a local file with
no server, package installation, network access or game content embedded in the page.

```powershell
py -3.13 -m cmre progress
Start-Process CM0102/Analysis/cmre/progress/index.html
```

`cmre status --write` also refreshes it. A successful parent closing check already
runs this command, so ordinary recovery sessions produce regular visual updates.
A failed pytest/verify gate does not refresh status or the atlas. Manual exports
are accounting snapshots, not a new verification claim. Reload the page after an export.

The default directory is ignored local analysis storage. It contains `index.html`
(complete embedded data), `snapshot.json` and `history.json`. Retain/back up this
directory with the project's local evidence; history begins at the first export.
Repeated exports with unchanged state do not add points. Changes and regressions
are retained. Inventory revisions remain in history, but chart lines compare only
snapshots with identical target and inventory pins. The chart's x-axis is export time;
no historical progress is inferred from source-file modification dates or Git commits.
The timestamp records capture time; the checkout revision is context, including local
state, rather than a claim that every byte is committed.

## Tooling worktrees

A source-only worktree has no authoritative recovery data. Read the existing
same-machine checkout without copying its game, ledgers or analysis:

```powershell
py -3.13 scripts/cmre_progress.py --workspace F:/src/cm0102-decompiled --output CM0102/Analysis/cmre/progress
```

This runs the exporter from the tooling worktree using the specified checkout's
existing `cmre` accounting modules and local inputs. It does not alter acceptance.
The normal inventory/generated-code caches may be populated by those modules.
Use a stable recovery batch boundary for a coherent snapshot; individual records
are atomic, but a manual export does not freeze concurrent acceptance activity.
Missing/changed pinned exports or a missing acceptance registry fail explicitly;
there is no fabricated zero-percent or demo fallback. Recovery setup remains in
[BUILDING.md](../BUILDING.md).

## Reading the atlas

- Each rectangle represents one pinned inventory function, weighted by its original
  `body_bytes`, with small gutters and group headings. Zero-byte entries remain in
  the index; they have no map area. Source size and ledger extent are never weights.
- Accepted C/legacy is the source measure. Lift-inclusive recovery includes accepted
  lifts without claiming readable C. C-tier records can still contain assembly/adapters.
- Composed-call coverage uses `shards.gather`'s current passing experimental reports,
  including source, header, dependency and execution-identity checks. Live integration
  uses the same recorded flags as `cmre status`. Neither is inferred from acceptance.
- Work-state colors use the existing shard reconciliation rules. Compiler-generated
  functions stay in the inventory denominator. Holds, blocks, lifts and inferred
  compiler-generated dispositions are not accepted-source completion.
- Candidate modules are inventory hypotheses. Multiple distinct candidates go in
  `Ambiguous module`; absent ownership goes in `Unknown module`. Absolute source-path
  prefixes are removed. Shard grouping is the existing CRC32 ownership rule.
- Headline metrics always cover the entire pinned inventory; search/zoom affect
  only the map and function index. Entries outside the inventory are reported as
  exceptions and excluded from byte coverage. Thus function totals may differ from
  `cmre status`, which also counts unmapped accepted records.

Record presence is not fresh revalidation, subsystem parity, portability, a matching
binary or a standalone game. Only `cmre accept` changes recovery status.

## Sharing and hosting

Open or share the generated `index.html` directly. It can also be served as a static
site (including GitHub Pages) after the normal [publication review](PUBLICATION.md).
Publish only reviewed report files; never expose the analysis directory through a
web server. No deployment or remote write is part of export. A source-only public
CI runner cannot regenerate authoritative progress without the retained local inputs.

The export uses an explicit metadata allowlist: IDs/preferred-image VAs, inventory
sizes/pins, sanitized candidate modules, recovery states and aggregate history.
It omits original names/signatures, instructions, disassembly, test inputs, source,
raw witnesses, asset content, credentials and absolute local paths. Review provenance
and report content before publication; metadata does not bypass that requirement.

The [linked gist](https://gist.github.com/marijnvdwerf/771ce3f867a943247bc952a17c3d2613)
is a decomp.dev treemap recoloring userscript, rather than a data exporter.
[Chaos Viewer](https://github.com/tangosdev/chaos-viewer) is a matching-decompilation
atlas with a different data/claims model. This project uses its existing behavioral
acceptance accounting rather than assigning objdiff byte-match percentages to it.

Asset-free checks:

```powershell
py -3.13 -m pytest tests/test_cmre_progress.py tests/test_cmre_progress_atlas.py
node --check scripts/progress/app.js
```
