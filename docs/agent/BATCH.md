# Machine recovery

Start after a green parent preflight. Use one session runner for machine jobs;
its lock excludes other runner jobs and its receipt records the child PID,
UTF-8 log, and exit code. Direct CLI writers share a command lease with the runner
and are excluded while it runs. Freeze harness code for the job's lifetime.

1. Inspect `py -3.13 -m cmre next -n 5 --max-bytes 600`. Completion: a nonempty
   frontier, or an empty result that routes to triage. Queue ordering belongs
   to `cmre/queue.py`; use `cmre --help` for optional filters.
2. Run a bounded batch:

   ```powershell
   py -3.13 scripts/cmre_session.py run batch -n 500 --max-bytes 600 --jobs 16
   ```

   Completion: the runner receipt says `finished` and records the batch exit.
   A batch can exit 0 while blocking functions; read the ACCEPT/BLOCK summary.
   For a diagnosed set, add `--functions fn_XXXXXXXX fn_YYYYYYYY`. This filters
   the ordinary frontier before `-n`; accepted and blocked IDs remain excluded,
   and size/module filters and ordering still apply. It changes selection only.
3. Inspect `cmre status` and `cmre next --blocked -n 10`. Compare before/after
   counts and block reasons. After a harness-error, mismatch, or a repeated
   low-coverage shape, diagnose one representative before increasing batch size.
   With useful progress and no new defect, another bounded batch is appropriate.
4. For input-specific blocks, follow [TRIAGE.md](TRIAGE.md). For a shared
   mechanism or failing test, follow [HARNESS.md](HARNESS.md). For requested
   readable source, follow [C.md](C.md).

After interruption, read the receipt and check its recorded PID before restarting.
An existing lock file alone does not mean a live job: the OS releases the lock
when its owner exits. Re-run preflight to check interrupted writes. A receipt
still marked `running` is unfinished evidence, never an acceptance result.

Batch and revalidation also print an item-receipt path under
`CM0102/Analysis/cmre/jobs/`. After verifying the old process tree has ended,
repeat the original supervised command with `--resume RECEIPT_PATH` and the same
selection options. Completed items are reused only when global and per-function
input/output fingerprints still match. Changed inputs require a new job. The
receipt's sibling directory holds atomic per-item completions; final totals are
written to the receipt on completion. A process exit alone is not an item result.
