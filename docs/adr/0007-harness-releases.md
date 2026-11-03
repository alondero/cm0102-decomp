# ADR 0007: Harness releases

Status: accepted (2026-11-03, chosen by the project owner).

## Context

Accepted evidence is bound to the harness identity: a SHA-256 over the module
names and bytes in `HARNESS_MODULES` (`cmre/recover.py`). Any change to those
15 modules invalidates every accepted tier unless a compatibility receipt
bounds the impact ([HARNESS.md](../agent/HARNESS.md)).

Measured cost:

- 234 revalidation receipts took about 64 job-hours between 2026-10-14 and
  2026-11-01.
- The 2026-11-03 audit was triggered by two emulator edits made for one
  composition experiment. Its eight-worker resume ran 5,598 tiers in 53 minutes
  of wall time (25,175 validation seconds) with no `MemoryError`. All 11,265
  tiers passed under identity `904e5da7…c734`.
- Recorded per-tier times sum to about 101,000 seconds, so a full audit at eight
  workers takes about 3.5 hours of wall time.

Harness edits arrived one at a time, each with its own sweep. Accepted C
throughput fell from about 125 KB per day (2026-10-22 to 10-27) to under 1 KB
per day (10-31 to 11-03). The ready queue ran out in the same period, so sweeps
are one of two causes, not the only one.

## Decision

Harness modules change only through a release.

1. Harness work happens in a separate worktree. Each change carries focused
   tests and its `cmre revalidate --plan` impact.
2. A release batches the reviewed changes. It is applied at a recovery batch
   boundary, when no writer holds the lease (the guard enforces this for
   `cmre/` edits).
3. Each release runs one revalidation: `--reuse` with compatibility receipts
   where the impact is bounded, otherwise the inventory audit. It finishes with
   one full pytest and `cmre verify`.
4. At most one release per working day. A correctness defect in accepted
   evidence may justify an extra release; a new capability does not.
5. The release record keeps the old and new harness identities, the change
   list, the revalidation receipt and the pytest receipt.

## Consequences

- A harness change costs one scheduled sweep of hours, not a sweep per edit
  and an idle recovery queue.
- Capability work (x87 forms, transports, input generation) waits for the next
  release. Plan it in release-sized batches.
- Recovery lanes never see the harness change under them; the guard rule makes
  this mechanical rather than advisory.
