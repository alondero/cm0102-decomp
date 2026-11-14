# Measure recovery throughput

Use the native C loop, work-state registry and family index. The retired
`tasks/` Markdown queues and standalone planning/farm scripts are archived under
`CM0102/Analysis/workflow-upgrade-20261020/retired/`; they are not a second source
of scheduling truth. `cmre task` remains optional for tool-less workers only.

For authorized provider work, use [PROVIDER-CAMPAIGN.md](PROVIDER-CAMPAIGN.md):
it defines ordinary batch allocation and, when requested, the timed schedule,
closing reserve and coordinator-led retro. The measurement rules here apply
inside that workflow.

## Select comparable work

1. Start from `cmre family -n 10 --json` within the selected subsystem. Families
   group original instructions by mnemonic, operand width, register roles and
   internal branch topology. Constants, addresses and field offsets may differ:
   inspect `cmre family fn_REP --peer fn_MEMBER` and each member's brief. A shape
   match is never evidence of behavioral equivalence. Completion: exact IDs,
   reviewed representative, and named differences/unknowns for every member.
2. Select matched cohorts by size, family, ABI and hazard class. Assign disjoint
   functions; translating the same function twice contaminates the comparison
   with prior knowledge and warm caches. Include hard cases and failures in the
   denominator. Completion: finite cohorts and the variable being tested
   (family context, model tier, or worker lifetime), changing one variable at a time.
3. `py -3.13 -m cmre pilot start NAME --functions fn_ID fn_ID ...` records the
   initial accepted set and harness identity. It creates a measurement receipt
   under ignored analysis storage, not a task plan. Completion: receipt exists
   before work begins; already accepted functions cannot earn new-byte credit.

## Run and account

- Follow [C.md](C.md). Keep each worker to at most three functions in one family
  and renew its context at the batch boundary. Supply only the workflow, exact
  paths, original evidence, representative and differences needed for that batch.
  Retain full artifacts on disk; return a one-line result and first discrepancy.
- The parent reviews every member's behavior, using the representative and
  complete differences to avoid repeating established facts. Escalate unknown
  ABI, SEH, x87, aliasing or entry-register contracts before spending five retries.
- `cmre work list` shows pending review and unresolved reasons. Group common
  blockers with `work set --blocker NAME --retry-when ...`. A manual retry records
  why it is justified and retains cumulative attempt/failure counts. Changed
  fingerprints do not automatically reopen work. `work import-wip` migrates
  existing top-of-file WIP notes without inferring that unmarked sources passed.
- Record each observed provider call once, including coordinator, failed attempts
  and reviewer calls:

  ```powershell
  py -3.13 -m cmre pilot usage NAME --call-id PROVIDER_ID --model OBSERVED_MODEL --input-tokens N --cache-read-tokens N --cache-write-tokens N --output-tokens N --review-corrections N
  ```

  Omit unknown token fields. A lower price is not fewer tokens. Report cached
  input separately from fresh input and output; do not sum unlike billing rates.
  Retain reasoning tokens separately when the client exposes them; the pilot's
  output field alone may omit them. An unfinished exported message's initialized
  zeros are unknown usage. Keep raw exports and an explicit reconciliation note.
- `cmre pilot report NAME` reports measured elapsed time, new accepted bytes,
  test invocations, failures, recorded usage and review corrections. Reconcile
  provider logs before `cmre pilot finish NAME --usage-complete`; otherwise finish
  without that flag. Token efficiency is unknown until usage is complete.
  A changed harness invalidates the rate comparison. Stop timing after review
  and acceptance, not at the worker's first passing test.
  Finishing freezes the measurement; later acceptances cannot earn retroactive credit.

Completion: compare accepted bytes/hour and tokens/accepted byte on matched
cohorts, together with review corrections and any later-discovered defects.
Keep the winning configuration only when fidelity is unchanged. The October 20
afternoon baseline was about 7.5 KB/hour; a 10x claim needs measured comparable
work near 75 KB/hour and one tenth the tokens/byte, not more simultaneous workers.

## Advance the executable goal

Every selected subsystem has a [campaign](CAMPAIGN.md) with a dependency report,
an observed recovered-to-recovered call obligation and a named runtime checkpoint.
At a cohort boundary, inspect that report before selecting more isolated leaves.
Run composition when its selected sources are ready. Keep the primary consumed-input
replay obligation from [RUNTIME-REPLAY.md](../RUNTIME-REPLAY.md) in the handoff until
resolved. Report source bytes, composed calls, and live integration separately.
Source-count growth alone never closes M2, M4, M5 or M6.
