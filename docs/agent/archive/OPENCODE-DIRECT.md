# Native OpenCode recovery workers

Use only when the user explicitly requests direct harness subagents. Otherwise
use [PROVIDER-WORKERS.md](PROVIDER-WORKERS.md)'s standalone adapter. This branch
changes dispatch, not fidelity, recovery ownership or acceptance gates.

For the authorized coordination-only mode, use
[NATIVE-COORDINATION.md](NATIVE-COORDINATION.md) for roles, capacity, worker
inheritance and gate execution. Steps below describe ordinary native mode where
they assign parent technical work or a two-worker starting limit; those assignments
are replaced, not combined, in coordination-only mode.

1. Inherit the parent's completed preflight and complete
   [provider intake](PROVIDER-CAMPAIGN.md#batch-intake). In OpenCode, launch long
   session gates with the shell tool's background mode and retain its terminal
   notification; a short foreground timeout can terminate pytest mid-run.
   Keep declared inputs fixed while a gate is live. A launch/session ID is not a
   completed worker or green gate.
2. Resolve user-named models through the harness catalog, filtering your own
   provider first, then the requested name. Record the returned exact provider/model
   ID; preserve the authorized provider instead of silently falling back. Keep
   discovery and validation output bounded: use the non-JSON revalidation summary
   with `--output` for the complete artifact, not a full inventory in parent context.
3. In ordinary native mode, start with at most two workers, including reviewers. Record each native
   session ID, model requested, role, exact function IDs, immutable evidence,
   writable paths, parent receipt and terminal artifact in the ignored run index.
   Mark each dispatch `ROLE=WORKER`: its completed preflight is inherited;
   session start/check/checkpoint commands remain with the parent. A duplicate
   start occurred despite cooperative scope instructions in the first native trial.
   Give a fresh worker at most three functions from one family/subsystem. Include
   complete original function instructions, relevant call contracts and `cmre.h`;
   a draft or accepted callee declaration alone is not ABI evidence.
4. First require full assigned packet/header reads and a harmless scoped write.
   Authors own private C and input proposals plus numbered full function reports;
   reviewers get read-only source/evidence and a separate verdict path. Assign
   explicit attempt/context/time bounds through [C.md](C.md). Native prompt bounds
   and path restrictions are cooperative unless the harness actually enforces
   them; they do not inherit the standalone adapter's process audit or containment.
5. Authors run the unchanged function gate against their assigned private source,
   stop at the first pass or evidence-bound hold and return source/report hashes
   and all failed invocations. Give one shared-workspace worker the testing slot
   at a time; the parent queues acceptance until that owner returns. A private
    source path does not exempt a CLI test from that cooperative scheduling slot.
    Ordinary CLI `test` does not itself take the command writer lease. Before testing new full hint JSON, use
   `py -3.13 scripts/cmre_hint_check.py PATH`. The schema check executes no engine
   code and supplies no input or acceptance evidence. Private `--source` failures
   may lack canonical work-registry counters: retain them in the assignment receipt.
   Store each original probe attempt at a new numbered path before correcting
   inputs; retain the prior raw result, not only its quoted terminal outcome.
6. Use a different author for independent review of the exact source/report/input
   bundle. Make disagreements falsifiable against original instructions; passing
   tests and reviewer agreement are not proof. The parent owns canonical adoption,
   work holds and serial `cmre accept --c`; defer these while a writer gate is live.
   Follow the existing [validation budget](PLAYBOOK.md#validation-budget), not a
   fresh full suite per worker. Keep receipt/checkpoint publication outside live gates.
7. At a batch boundary, collect terminal native results and reconcile assignments.
   Retain tool logs when available. Without exported observed-model/usage fields,
   report the requested model and unknown usage; do not manufacture adapter receipts,
   token totals, hard timeouts or complete read audits. A timed run uses its original
   deadline and closing reserve in [PROVIDER-CAMPAIGN.md](PROVIDER-CAMPAIGN.md).
   Stop new dispatch at drain time; if cancellation/termination is unavailable,
   retain unfinished session IDs and obligations rather than claiming workers exited.

Close with accepted C/legacy bytes, lift-inclusive recovery, exercised composed
calls and live integration separately. Invoke a user-requested skill through the
current harness's skill tool before falling back to filesystem discovery. Native
dispatch by itself establishes neither greater context capacity nor model speedup;
use the finite pilot and truthful accounting in [THROUGHPUT.md](THROUGHPUT.md).
