# Publish reconstructed source and retain local evidence

Use for commits, Git pushes, release-content review, or a publication scanner
finding. Adam's standing authorization (AGENTS.md) covers committing and pushing
reviewed source-only batches as work progresses: the parent session commits and
pushes accepted sources and reviewed tooling at every handoff, and at batch
boundaries of a long run, without waiting for a separate request. A scan receipt
is the content prerequisite for each push. Recovery acceptance via
`cmre task publish` is independent of Git publication.

Paths that cannot be published yet stay in the working tree: unaccepted drafts
stay untracked, and accepted sources awaiting a listing review or remediation
are listed by exact path in `.git/info/exclude` with a dated comment. Report the
count left local in the handoff.

## Eligible content and local inputs

Review reconstructed source (including necessary assembly), headers, authored
tests/tools, build scripts and focused documentation by content and provenance.
Keep supplied executables/media, assets/databases, extracted original content,
credentials, raw disassembly/byte dumps and local recovery evidence outside
published history and packages. This applies to public and private remotes.
Hashes, function IDs, status summaries and documented reconstruction conclusions
can be published when they contain no excluded payload.

The current scanner covers source publication. It deliberately rejects build
binaries and archives. A future engine-binary release needs its own reproducible
build/package-content verification; passing this source scan alone does not
approve a binary package.

## Review and scan a batch

1. Inspect existing dirty work and identify explicit paths for the requested
   batch. Review authored changes and accepted-source identities. Preserve
   unrelated work and original accepted bytes, including line endings.
2. Run `py -3.13 -m cmre publication scan PATH...` on the candidate paths. Retain
   findings; remediate actual copied content locally. A `.c` or `.S` suffix is
   not proof of authorship. Do not strip labels or alter accepted source merely
   to avoid detection; a behavior/source change requires normal review and
   acceptance before publishing that new revision. Farm and manual acceptance
   surface instruction-listing findings as publication alerts. Resolve them by
   exact-hash review or withhold the candidate from Git publication.
3. The address-keyed listing detector can also flag authored executable assembly.
   An independent reviewer may classify that exact file as authored reconstruction
   after inspecting its provenance and retained build evidence. Record only
   path, source SHA-256, reviewer and specific rationale in an ignored local JSON:

   ```json
   {
     "schema": "cmre-source-reviews/1",
     "reviews": [{
       "path": "src/adapter.S",
       "sha256": "<64 lowercase hex characters for the reviewed file>",
       "reviewer": "reviewer identity",
       "reason": "Specific authorship and emission evidence reviewed locally"
     }]
   }
   ```

   Pass `--source-reviews CM0102/Analysis/cmre/publication/source-reviews.json`
   to scans or manifest generation. This disposition resolves only the listing
   heuristic for matching path/content; changed bytes or another path require
   review again. It never clears asset, original-byte/text, credential, private
   history or accepted-source mismatch findings. Raw disassembly is ineligible.
   Do not create a disposition simply because a file is useful or already accepted.
4. Stage explicit reviewed paths and run `py -3.13 -m cmre publication scan
   --staged`, adding the source-review argument if needed. The staged scan
   compares all tracked C in the index to the authoritative local ledger, so
   inherited mismatches appear before committing. Resolve findings before
   committing a source-publication batch. Local-only history and evidence backups
   remain separate from the publication branch.
5. For a requested push, run `py -3.13 -m cmre publication scan --commit BRANCH`
   against the intended remote, adding the same source-review argument. The scan
    covers every reachable blob, including removed files, and rejects ancestry
    shared with the preserved private history. The default private ref is
   `refs/private/cm0102-pre-publication`; preserve its original commit identity.
   Local `main` tracks `origin/main` in the source-only history. Never merge the
   private history into it or clear/repoint the private baseline to pass a scan.
   A source-only clone lacks that local ref: perform publication scans in the
    authoritative checkout, where the original history and supplied inputs exist.

    `accepted-mismatch` describes current-tree source identity. Content
    findings identify the scanned blob and distinguish a commit-tree finding
    from a history-only one. If a blob is reachable from the local tracking
    ref, the diagnostic names that ref; it does not query the remote or
    weaken the finding. The default destination is `main`; for a same-name
    PR branch, pass `--destination BRANCH`. Editing the tip cannot remove a
    history-only blob.
6. A passing receipt binds the exact commit, push URL and known PUBLIC/PRIVATE
   visibility. Supported guarded commands are `git push origin BRANCH:main` and the
   existing same-name PR form `git push origin BRANCH:BRANCH`; both require that
   identity still to match. PR publication requires an actual local branch, not
   a tag or another namespace. Changed commits, destination URLs
   or visibility require a new scan. Force/mirror/all-ref pushes remain excluded.
   After a main push, run `py -3.13 -m cmre publication verify-remote
   BRANCH` to check the remote main and the scanned destination. For a same-name PR push,
   first obtain the single `git remote get-url --push --all origin` result and
   compare that push URL and visibility with the exact-commit receipt. Compare
   `git ls-remote PUSH_URL refs/heads/BRANCH` with `git rev-parse BRANCH`; the
   fetch URL may identify a different repository.
   A PR branch is not verified by the main-only `verify-remote` command.

Schema-2 receipts replace the older private-only receipts. Rescan a publication
candidate before its next requested push; historical receipts remain evidence of
their original checks and are not upgraded by editing their JSON.

## Retain evidence locally

Ignored storage is not a backup. Preserve acceptance ledgers, hints/contracts,
source/build identities, raw witnesses and rejection controls, session and
composition receipts, task/lease/attempt journals and publication reviews.
Preserve reference saves, environment manifests and observer data required to
reproduce runtime conclusions. Keep a local or private offline backup with a
file/hash manifest and verify restoration before removing the only working copy.
Such backups are not source-publication artifacts, including on private Git remotes.

Scratch outputs may be cleaned only after checking that no retained receipt,
active job, witness or reference state depends on them. Never delete claims or
attempt history to reset ownership or retry limits. Keep authored conclusions
and reproduction instructions in tracked documentation; link local evidence by
identity without copying excluded payload into that documentation.
