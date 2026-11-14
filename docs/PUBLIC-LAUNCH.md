# Public repository and progress tracking

Assessment date: 2026-11-14. Repository: `alondero/cm0102-decomp`.
This is preparation for publication; repository visibility and tracker listings
have not been changed by this work.

## Recommended destination

Host the existing recovery atlas on **GitHub Pages** and use a directory for
discovery. Keep the atlas as the authoritative explanation of progress.

| Service | Fit for this project | Next step |
| --- | --- | --- |
| [Decomp 720](https://decomp720.com/about) | Best metric fit: accepts project-defined progress and distinguishes decompilation, recompilation and ports. | Request a listing linking the public repository and atlas, explicitly defining accepted-source coverage. The inspected pages did not establish a submission API. |
| [unbaked](https://unbaked.games/submit/) | Has a project submission form and an AI-assisted decompilation category. Useful additional discovery, rather than the authoritative numerical tracker. | Submit after the repository is public; the form requires confirmation about content and provenance. |
| [decomp.fun](https://decomp.fun/about/) | Its progress means byte-identical matching code and consumes objdiff reports. That is not the current CM0102 metric. | Do not convert behavioral acceptance percentages into matching percentages. Reconsider only with actual matching-build evidence. |
| [decmp.dev](https://decmp.dev/) | Its homepage describes objdiff-based matching progress. | Same metric incompatibility; do not fabricate an objdiff report. |

The decomp.fun history page reports that the original decomp.dev index retired
in October 2026. Its old reputation is not evidence that it currently accepts
new listings. No independent audience statistics were verified for the successor
sites, so this assessment recommends suitability rather than claiming one is
the most popular.

Suggested listing text:

> Championship Manager 01/02 (Windows PC), official 3.9.68. AI-assisted faithful
> behavioral decompilation in progress. Progress is accepted C/legacy source
> coverage weighted by original function body bytes, not byte-identical matching
> or playable-game completion. Lift-inclusive recovery, composed calls and live
> integration are reported separately. No standalone game build is available
> yet. Users supply their own original game files; the repository distributes
> reconstructed source and tooling, not game content.

Use the atlas's dated snapshot for any numerical figure. Source coverage is
authoritative-workspace accounting and can include accepted files withheld from
publication; `recovery/c/STATUS.tsv` describes the published source subset.

## What belongs in Git

| Include after review and scan | Keep local |
| --- | --- |
| Authored reconstruction, required adapters and headers | Supplied executables, installation media, databases and assets |
| Tests with synthetic inputs, tooling and build instructions | Extracted content, original instruction streams, raw disassembly and dumps |
| Focused docs, target hashes, progress metadata | Acceptance ledgers, lifts, hints, worker packets, witnesses and reference saves |
| Reviewed, self-contained atlas HTML | Credentials, environment files, local endpoints, logs, tool installations and build archives |

`.gitignore` prevents accidental adds; it cannot remove tracked content or
erase Git history. Do not force-add ignored folders. Exact local exclusions for
withheld C sources are additional safeguards, not a portable publication policy.
The comparison scan and provenance review remain required.

## Launch gates

1. Review and scan candidate files and staged changes with the existing
   [publication workflow](agent/PUBLICATION.md). Preserve accepted bytes and local
   evidence. Reuse unchanged path/hash-specific independent source reviews.
2. Scan every remote branch and tag's reachable history, not just `main`.
   Include PR head/merge refs when auditing retained pull requests. Resolve
   excluded content before exposure; deleting a file at the tip is insufficient.
   Do not rewrite or delete remote history without a reviewed migration plan.
3. Review GitHub issue/PR bodies, comments, attachments, Actions logs/artifacts,
   releases, wiki, Pages and any packages separately: the Git scanner does not
   cover them. A visibility change exposes more than the default branch.
4. Apply the [MIT scope](LICENSING.md) selected by the maintainer. Retain
   third-party notices and establish which contributions the grant covers.
5. Run the asset-free CI subset from a clean source checkout. Record missing
   game-dependent checks as unavailable. Run the authoritative parent session
   check separately; it is not replaced by public CI.
6. Review the final commit, intended destination and exact scan receipt before
   changing visibility. A PRIVATE receipt does not authorize a later PUBLIC
   push: rescan after visibility changes. Enable GitHub secret scanning/push
   protection where available and require the asset-free checks for changes.
7. Enable Pages only for the explicit report directory using the manual workflow
   below. Then add the live URL to repository About and submit the directory
   listing. Do not describe preparation or an uploaded artifact as deployment.

Suggested GitHub description: `Faithful behavioral decompilation of Championship
Manager 01/02 3.9.68; reconstructed source, tooling and measured recovery progress.`
Suggested topics: `decompilation`, `reverse-engineering`, `championship-manager`,
`game-preservation`, `c`, `python`. Do not use a matching-decompilation claim.

## Publishing the atlas

At a stable recovery batch boundary, complete the parent session check and use
its refreshed `CM0102/Analysis/cmre/progress/index.html`. Copy only that file to
`public/progress/index.html`, review it, then scan that explicit path. Keep the
full local snapshot/history/evidence in ignored storage. Commit the reviewed
HTML with an explicit file list and the normal staged/history scans.

The included initial HTML is the retained 2026-11-14 10:10 UTC snapshot, with
39.215% accepted C/legacy body-byte coverage and 55.238% lift-inclusive recovery;
composed-call and live-integration coverage are both zero. It is a dated snapshot,
not a claim of a fresh validation run or complete published source coverage.

The manual `progress-pages.yml` workflow deploys only that committed HTML from
`main`. In repository Settings > Pages choose GitHub Actions, then run the
workflow on `main` after publication approval. Configure the `github-pages`
environment for the default branch. Expected project URL after successful
deployment: `https://alondero.github.io/cm0102-decomp/`.
The workflow follows [GitHub's custom Pages workflow guidance](https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages).

The public runner does not access the game or recompute recovery metrics.
Updates require a new locally validated, reviewed snapshot. The report's
timestamp makes stale data visible; there is no fabricated history or percentage.

## Audit evidence

At initial inspection the repository was PRIVATE; Pages, wiki and discussions
were disabled. No releases or Actions artifacts were listed.

The audit scanned all 11 advertised branch tips and their reachable histories,
plus the additional PR #26 head (the other advertised PR heads duplicated
already scanned tips). No tags were advertised. All were independent of the
preserved private-history ref. Reusing the two retained, hash-specific source
reviews, `main` at `6b03b9368cfaecac530c903eb2c7ff38a495cbb8` and both recent
`publish/` branches had zero findings. Eight older branch tips and PR #26 had
35–39 `accepted-mismatch` findings each: their historical C versions differ
from today's accepted source identities. No asset, credential, original-content
or private-history findings remained. These older tips are not current accepted
source snapshots and must not be represented as passing current acceptance.
Resolve their publication disposition before claiming all refs pass; no remote
branches were deleted or rewritten by this preparation.

The scanner also checked 34 issue/PR records, 9 issue comments, 10 PR bodies,
zero inline review comments, and all three retained Actions logs. It reported
zero findings and found no attachment links in those texts. This is a bounded
content audit, not proof that all possible credentials or rights issues are
detectable. GitHub-hosted packages and non-advertised retained objects were
not audited. Refresh this inventory before changing visibility.

A source-only worktree passed all 153 selected publication, guard and atlas
tests without game or local evidence directories; atlas JavaScript syntax also
passed. The authoritative baseline printed 2,979 passing tests and four passing
subtests, but its session receipt exited 2 because declared inputs changed
during pytest. That baseline is inconclusive, not green. See the completion
report for the separate closing gate.

Local audit material is retained under
`CM0102/Analysis/cmre/publication/public-launch-20261114/`; it is not a public
artifact. Existing dirty recovery work belongs to ongoing recovery and is not
implicitly included in this publication-preparation batch.
