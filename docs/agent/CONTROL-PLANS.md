# Selected computation controls

`cmre mutate fn_VA --plan PLAN.json --output NEW_DIRECTORY` runs a finite named
list through the existing unchanged semantic C gate (O0 and O2), using the
function's normal effective `Options.cases` (600 by default). Plans cannot
override that count. Identity capture pins it both at `identity.cases` and
`identity.options.cases`; inconsistent pins or drift from current effective
options reject before launching a gate. Each optimization report must match
that exact count, budget, seed and input policy. Numeric witness indices are
bounded by the pinned count, including when resuming retained stages.
`--resume` reuses matching terminal stages after rechecking all input identities
and retained artifact hashes. It never repeats a terminal failure. A pending
stage means execution was interrupted: investigate its retained receipt and use
a new output directory for an explicitly authorized retry. Successful terminal
stages preceding an interruption before the next stage can be reused.

This opt-in lane extends `cmre.mutate`; the existing token-mutant command is
unchanged. The existing work registry remains the scheduling authority. The
controller uses the existing diagnostic claim/binding checks and holds its per-VA
attempt fence throughout baseline, controls and packaging. Private controls do
not record canonical tests, consume the ordinary failure allowance, accept,
change work state, or clear holds. They cannot run over live worker claims or
task leases; use the assigned worker protocol for those candidates.

Plans use strict UTF-8 without BOM, duplicate JSON keys, NULs or non-finite
numbers. Unknown schema fields are rejected. Every path is absolute. Pin the
baseline `source` and canonical `hints` as `{ "path": "...", "sha256": "..." }`;
an absent canonical hint has an explicit null hash. Capture `identity` with
`cmre.mutate.control_identity(va, source_path)` after freezing source/hints and
the execution environment. This API preprocesses compiler dependencies and pins
the target, harness, toolchain, headers, effective options, controller and build
environment. It does not execute a differential test. Identity and all source,
hint, control and proof pins are checked before every launch and after execution.

The exact top-level fields are `schema` (`cmre-control-plan/1`), `va`
(`0x` plus eight lowercase hex digits, preferred-image VA for the pinned target
hash in `identity.execution_identity.target`), `source`, `hints`, `identity`, and
`controls`, plus optional `required_stages`. Supported required stages are
`controls` and `diagnostic-pe`; both run by default. Requests for unsupported
`raw-trace` or `actual-gate-pe` stages fail before any gate launches. Each control
has a unique path-safe `name`, `expect`, and exactly one
of `transform` or `source`. Example control:

```json
{
  "name": "changed-return",
  "transform": {"old": "return value;", "new": "return value + 1;", "count": 1},
  "expect": {"kind": "mismatch", "field": "regs"}
}
```

Transforms replace every exact occurrence only when the original occurrence
count equals the positive declared `count`. A `source` control instead supplies
a separately pinned frozen private source file. No plan contains shell commands.
A mismatch field must be one emitted by the unchanged comparator: `outcome`,
`detail`, `regs`, `events`, or `memory`. Integer return-value mismatches use `regs`.
A required control is killed only by a meaningful mismatch in the declared
field at an integer case index in each optimization. Witness case indices may
differ between optimizations and are retained separately. Build, contract,
missing report and budget-asymmetry errors remain unresolved. Survivors remain
visible. An extra expected-equivalent control uses
`"expect": {"kind": "equivalent", "proof": {"path": "...", "sha256": "..."}}`.
It must pass both builds and remains a separate classification; its proof still
needs independent native review. It is never counted as a kill.

Every stage atomically publishes pending state before execution and terminal
state afterward. Retained files include frozen source bytes, full gate JSON,
gate key, structured API command, API result status (not a subprocess exit),
and separate builds made through `build.build` with PE hashes and compiler/linker
commands. These PE copies are diagnostic rebuilds under the pinned identity;
the current gate deletes its actual temporary PE files. Build failures retain
their errors and available partial files. The immutable artifact index hashes
the plan and stage artifacts. Bundle schema `cmre-control-bundle/2` and stage
schema `cmre-control-stage/1` require exact metadata and a complete artifact
manifest. Resume validates the frozen source against the plan-derived bytes,
report execution identity/profile/VA/options, commands and diagnostic PE
provenance, then recomputes classifications and witnesses from the retained
reports. Terminal labels alone cannot establish completion. Reuse fails before
launch for missing or changed files, malformed evidence, environment drift or
any pending stage. Error stages retain a pinned error report and stay unresolved.

## Full-trace gap and follow-up

The supported gate retains result summaries, and `explain_c` returns bounded
event slices. Neither is a complete serialized `emu.Result`. This first increment
records `trace_missing` and `gate_pe_retained: false`. Every retained PE has
`role: diagnostic-rebuild`; it is not the actual gate PE. Ordinary selected
controls completing successfully produce `status: controls-complete` and CLI
exit 0. Unresolved controls produce `incomplete` and exit 1. Trace absence is
separate from that controls outcome; unsupported explicitly required trace/PE
stages are rejected before execution. Bundles retain `readiness: proposal`,
independent review pending and acceptance pending. This is not an acceptance
substitute or a harness release, and no
inventory revalidation claim follows from its no-game regression tests.

A follow-up needs a reviewed optional collection API at the existing
`difftest.run` boundary. Accept a finite selected case set, run the original
generator/discovery prefix unchanged from case zero (including original global
read discovery and RNG call order), and serialize complete raw original and
recovered `Result` records before normalization for selected cases only. Retain
entry inputs, all raw events, dirty memory, read sites, terminal state, original
and compiled PE identities, optimization and case policy. Do not substitute
direct fixture indexing, a regenerated seed, `explain` slices or a truncated
prefix. Test selected-case identity against the ordinary complete run and a
nearby discovery-dependent case before integrating it as a harness release.
Until that API exists, full traces and acceptance readiness remain incomplete.
