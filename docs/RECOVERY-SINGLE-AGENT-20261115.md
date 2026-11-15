# Single-agent recovery publication, 15 November 2026

## Scope and provenance

This batch publishes 19 reviewed C routines accepted during a six-hour
single-agent recovery experiment. All addresses are preferred-image VAs,
base `0x00400000`, for CM0102 3.9.68, SHA-256
`0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86`.

The parent performed source, original-instruction/caller and compiler-emission
self-reviews. No independent review, subagents, farm, provider model calls or
usage probes were used. The 19 source files match their accepted ledger and
dependency hashes byte for byte; both `-O0` and `-O2` passing records are retained
locally. Published source identities are recorded in `recovery/c/STATUS.tsv`. Its regeneration
also refreshes stale status/hash rows for 13 routines already published in the
previous batch; that metadata refresh adds no experiment recovery bytes.

| Routine | Inventory body bytes | Coverage at both levels | Inconclusive at each level | Reviewed obligation |
| --- | ---: | ---: | ---: | --- |
| `fn_004e0180` | 86 | 0.9655 | 0 | Full argument words and EAX/EDX/ECX helper inputs; inherited draft amended. |
| `fn_0076c120` | 4,873 | 0.6055 | 0 | Object/scratch/SEH layout, partial-register paths and wide date-helper results. |
| `fn_0094f5b0` | 22 | 1.0000 | 0 | Actual caller homes, second-word mask and control-word helper boundary. |
| `fn_0094a698` | 18 | 1.0000 | 0 | Fixed arguments and full result from the original control-word wrapper. |
| `fn_009470b8` | 22 | 1.0000 | 0 | Actual variadic-home pointer and its physical EAX input. |
| `fn_0094fa5d` | 22 | 1.0000 | 0 | Caller-home load order and fixed configuration argument. |
| `fn_0094f564` | 23 | 1.0000 | 0 | Terminal call order; original-derived signal-slot fixture. |
| `fn_00944180` | 28 | 1.0000 | 0 | Cookie store and explicit inputs to original context transfer. |
| `fn_00943960` | 35 | 0.9375 | 0 | Incoming EAX on null path; owner reload after the first release. |
| `fn_00947b2e` | 35 | 1.0000 | 0 | Diagnostic home read after optional callback; terminal import boundary. |
| `fn_0094aab3` | 35 | 0.6250 | 0 | Caller-home reads around the byte flag store and final read/clear. |
| `fn_0094da93` | 36 | 1.0000 | 0 | Stream-home reload before each counter increment; full reader result. |
| `fn_0094a6e8` | 41 | 0.9231 | 0 | Ordered symbol lookup, indirect-call cleanup and fallback result. |
| `fn_009473a7` | 67 | 0.7391 | 0 | Uninitialized directory buffer, byte guards and original helper boundary. |
| `fn_0094f425` | 70 | 0.7500 | 0 | Short-circuit checks, callback reload and prevention of a tail-call home write. |
| `fn_00945487` | 78 | 0.8205 | 15 | Caller-home updates/reloads, unsigned pointer guards and signed comparison. |
| `fn_00945161` | 123 | 0.9804 | 0 | Signed search limits, wrapped addressing and ordered output writes. |
| `fn_0083b6e0` | 1,520 | 0.7474 | 54 | Three numerical conversion sites, alias-sensitive pointer reloads and call order. |
| `fn_009445d0` | 206 | 0.6981 | 0 | Original erroneous null store, full EDX:EAX and native transfer inputs; inherited source amended. |

The gain is 7,340 inventory body bytes: 17 fresh translations and two inherited
draft amendments. No accepted routines were withdrawn, and no corrections to
previously accepted routines were counted as new recovery.

## Validation and numerical evidence

The unchanged function gates passed at both optimization levels; each acceptance
also performed its supported fresh determinism check. The closing parent gate
passed 2,991 tests plus 4 subtests, `cmre verify`, and status generation. No
harness, comparison policy, compiler header, inventory or binding was changed.

`fn_0083b6e0` was accepted only after all three single-site conversion-result
`+1` mutations rejected at both levels against the final source/input pair.
An original-derived alias fixture makes the third conversion observable in a
later fee store. It is a derived test input, not an observed game state. The
five earlier capture references and matching-source baseline remain retained;
adding a sixth capture changes the sampled 600-case mixture. The .7474 coverage
and 54 inconclusive cases remain limitations, as do the 15 inconclusive cases
for `fn_00945487`. Finite passing gates do not prove every path faithful.

Source reviews corrected result-width/register inputs in `fn_0076c120`, the
variadic EAX input in `fn_009470b8`, and a caller-home tail-call write in
`fn_0094f425` before final acceptance. The amended `fn_009445d0` also prevents
that tail-call write and preserves the physical wide result. No accepted source
was altered merely to satisfy a publication heuristic.

## Throughput experiment

| Measure | Single parent | Historical farm | Parent/farm |
| --- | ---: | ---: | ---: |
| Accepted functions / body bytes | 19 / 7,340 | 14 / 10,580 | Different candidate mix |
| Work/scheduler duration | 6.007125 h | 0.905672 h | |
| Work/scheduler body bytes per hour | 1,221.88 | 11,682 | 10.46% |
| Operational duration through green closure | 6.307184 h | 1.849469 h | |
| Operational body bytes per hour | 1,163.75 | 5,720.56 | 20.34% |

The work window ran from 01:25:26.387856Z to 07:25:52.039087Z on 15 November,
including 25.651 seconds of overrun. Six hourly checkpoints were retained.
Setup/preflight took 536.124 seconds; post-work closing overhead took 544.087
seconds. Report writing and this later publication work are outside those
denominators. Work time includes failed attempts, diagnostics, review and waits.

The parent selected 52 unique candidates in 54 visits. It made 41 explicit
canonical test invocations (28 passing, 13 failing), 19 successful acceptance
invocations, and 24 scratch mutation controls (13 rejected, 11 survived). Internal
acceptance rechecks are not counted again as explicit test invocations. There
was one corrected syntax-only preflight failure. Drafts and surviving controls
contributed no accepted bytes.

The historical farm used 24 translator slots, 4 auditors and 8 gate threads.
Its scheduler interval was 3,260.419 seconds; the operational interval ran from
19:52:14.101848Z to 21:43:12.189771Z on 14 November. It inherited 13 hash-bound
drafts, worked held candidates and exhausted its MiniMax/DeepSeek providers.
Scheduler time excludes setup/interruption/full checks; operational time includes
an interrupted launch, tool repairs and an intermediate gate, but excludes later
publication review. That later review corrected three accepted routines without
byte gain. See [the prior publication review](RECOVERY-PUBLICATION-20261114.md).

Models, queue and draft availability differ. These rates describe the workflows;
they do not establish a controlled causal ranking, defect-free recovery rate or
standalone-game throughput. The parent was Codex based on GPT-6; exact deployment
and cost are unknown. No paid-provider or billing probes were made.

## Retained local work and boundaries

Thirty-three selected candidates remain unaccepted: 23 need evidence and 10 have
harness blockers. In particular, `fn_0085e960` still has a surviving numerical
control because the fixed semantic gate stops at an unindexed native indirect
target before the conversion. `fn_008e5d60` lacks required numerical observability
across its 28 conversion sites. `fn_0092b680` remains unchanged after a diagnostic
proved its child array scans live native stack contents. `fn_009455b2` remains an
unaccepted FNCLEX prototype; environment/tag equivalence is unproved.

These drafts, all 54 earlier untracked sources, and the preexisting modified
`fn_006be710` stay outside this publication. Original files, extracted content,
raw captures/witnesses, compiler artifacts, acceptance ledgers, local hints and
receipts remain local. Their retained identities and detailed retry conditions
are in the ignored experiment directory
`CM0102/Analysis/cmre/experiments/20261115T011630Z-single-agent/`.

Accepted C/legacy recovery is now 3,725 functions and 1,721,178 inventory body
bytes. Lift-inclusive coverage stays at 8,225 functions and 2,391,066 bytes. No
new composed-call evidence was added; integrated functions remain zero. Original
callees/imports remain explicit boundaries. Isolated root prototypes require
reviewed adapters before composition; this is not a complete game build or
modern-port milestone. [BUILDING.md](BUILDING.md) describes the current setup.
