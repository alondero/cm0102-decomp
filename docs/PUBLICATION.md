# Publication boundary

The private GitHub repository (`origin`, branch `main`) carries reviewed,
authored source and tooling. The supplied game, original instruction streams,
embedded executable bytes, the game's own text, local analysis and generated
evidence stay in the authoritative checkout. Issue #22 tracks the migration.

## Histories

- `github-main` tracks `origin/main`. It is a fresh history: its root is
  `a5fe882`, and it shares no commit with the prior local history.
- `main` is the prior local history. It contains inspection material and must
  never be pushed. The scanner fails any commit that shares ancestry with it.
- The local Git exclude rule hides untracked files, so a clean `git status`
  does not mean local work is published or backed up.

## The rule, as `cmre publication scan` enforces it

| Finding | Blocks when |
|---|---|
| `original-bytes` | 16 or more consecutive non-text bytes match a supplied PE image (exact), or 71 or more match another supplied file (sampled), written raw or as hex text (contiguous, spaced, `\x..`, `0x..` lists, little-endian `0x` words). Runs with fewer than four distinct byte values are ignored. |
| `original-text` | One matched game-text quote of 64 or more characters, or 256 or more matched characters in one file. Matches under 24 characters (coincidental English) and single identifiers (API, DLL and symbol names) do not count. Short identification quotes such as window titles and cited error messages pass. |
| `instruction-listing` | 8 or more lines pairing an original address with an x86 instruction, including lifted-assembly labels on their own line. |
| `asset-copy` | A file identical to any file under `CM0102/Game`, `Original-3.9.60`, `InstallMedia` or `Inspection`. |
| `asset-path` | Paths under `CM0102/`, `tools/` or `recovery/{ledger,lifted,blocked}/`, game/build file types (`.exe`, `.dll`, `.lng`, `.dat`, ...), `NUL`, stack dumps. |
| `binary` | Content that is not UTF-8 text or contains NUL bytes. |
| `credential` | GitHub, AWS, Anthropic, Slack and Google key formats, and private-key headers. |
| `private-history` | The commit shares ancestry with `refs/heads/main`. |
| `accepted-mismatch` | A `recovery/c/fn_*.c` file in the published tree (or staged) whose bytes, after Git's line-ending normalisation, differ from the source its local ledger accepted. |

The index is built from the local supplied files on every run (about 35
seconds). Receipts record its identity, never its content. Short hand-assembled
instruction snippets in tests are authored, and pass because they do not match
the game; the scanner, not a regex, makes that distinction.

## Publishing a batch

1. Check the branch: `git rev-parse --abbrev-ref HEAD`. Other sessions share
   this checkout and may have switched it. Prefer assembling the commit in a
   separate worktree based on `github-main`.
2. Review the batch: provenance, explicit status, no personal paths, and
   nothing that depends on unpublished files without saying so.
3. Stage an explicit file list. Never `git add -f` a whole folder: the
   exclude rule makes that capture every session's untracked work.
4. `py -3.13 -m cmre publication scan --staged` must report 0 findings.
5. Commit with `git commit -F <file>`. The guard reads whole commands, so push
   syntax in an inline message is refused.
6. `py -3.13 -m cmre publication scan --commit github-main` scans every blob
   reachable from the commit, checks that `origin` is private, and writes
   `CM0102/Analysis/cmre/publication/<commit>.json`.
7. Push that ref to `main` on `origin`, in the single form the guard admits
   (`<ref>:main`, no flags). Any other push, or a commit without a passing
   receipt, is denied.
8. `py -3.13 -m cmre publication verify-remote github-main` confirms the
   remote branch and private visibility.

Original-derived test inputs that a test needs are kept out of Git in the
hash-addressed store `local/fixtures/<sha256>` and read with
`cmre.env.local_fixture(sha256)`, which verifies the hash.

## Local-only components

These are intentionally absent from the repository:

| Component | Why |
|---|---|
| `CM0102/Game`, `Original-3.9.60`, `InstallMedia` | Supplied game and media. |
| `CM0102/Inspection` | Baseline manifests and disassembly. |
| `CM0102/Analysis` | Ghidra exports, including the pinned inventory TSVs that `cmre` requires; runs, receipts, claims and leases. |
| `CM0102/Runtime` | Runtime captures and VM state. |
| `tools/` | Third-party tools (Ghidra, unshield). |
| `local/fixtures/` | Retained original-derived test inputs. |
| `recovery/ledger`, `lifted`, `blocked`, `hints` | Generated records holding original instructions and evidence. |
| `recovery/lang`, `date`, `crt`, `rng` ledgers, tests and `cm_rng.dll` | Frozen legacy records with embedded original bytes; their clean sources are published separately. |
| 106 `recovery/c` files with inline original instruction streams | Not authored translation. |

## Clean same-machine setup

A fresh clone holds the tooling but none of the local inputs. Measured on
2026-10-30 by cloning this batch and linking only `CM0102/` and `local/` from
the authoritative checkout: collection succeeds and the scanner, guard and
hermetic harness tests pass, but tests that read live records fail for lack of
`recovery/c` sources, `recovery/registry.json` and `recovery/ledger` records.
Those, with `recovery/lifted`, `blocked`, `hints` and the legacy records, are
the remaining local inputs.

Linking writable records from the authoritative checkout into a clone would
split claims, failure counts and acceptance state. Providing them with pinned
identity checks is the coordinator-mediated bootstrap of issue #1, which also
forbids junctioning the whole recovery directory. Until it lands:

- Run recovery, acceptance and the closing gate in the authoritative checkout.
- A clone may run the publication scanner and harness tests that need only
  `CM0102/` and `local/`. Link those two folders, never `recovery/`.
- The guard compares paths lexically, so `CM0102/` stays protected when it is
  a link in a clone.

## Migration status

| Batch | State |
|---|---|
| Publication scanner, guard push gate, `cmre` harness, tests, scripts, `cmre.h` | Published in this batch. Files with uncommitted work from other sessions were held back and will follow from those sessions. |
| Agent workflow (`AGENTS.md`, `CLAUDE.md`, `docs/agent`, skills and agents) | Next batch. |
| Accepted plain C in `recovery/c` with a status manifest | Pending review; excludes the 106 inline-instruction files and marks held and WIP sources. Six of the seven files published earlier are byte-identical to their accepted sources. `fn_00945956.c` was withdrawn: its accepted bytes end in a stray CRLF that `eol=lf` normalisation changes, so it returns after an LF-clean re-acceptance. |
| Clean legacy sources (`recovery/lang`, `date`, `crt`, `rng`) | Pending review; ledgers, byte-bearing tests and `cm_rng.dll` stay local. |
| Other `docs/` | Pending review against the game-text rule. |
