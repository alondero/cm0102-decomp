"""Experimental composed-C differential reports, separate from ledger acceptance.

Selected C definitions share a translation unit. Static-name collisions are
build failures. Outside functions/data and import scaffolding remain boundaries.
"""
from __future__ import annotations

import hashlib
import json
import random
import tempfile
from bisect import bisect_left
from itertools import accumulate
from pathlib import Path

import unicorn

from . import build, campaign, coordination, difftest, emu, env, image, jump_transport, lift, recover, return_transport, x87_transport


def _symbols(built, selected: list[int]) -> dict[int, int]:
    symbols = {}
    for name, address in built.symbols.items():
        match = build.DECORATED.match(name)
        if match and int(match.group(1), 16) in selected:
            va = int(match.group(1), 16)
            if not built.image.text.va <= address < built.image.text.end:
                raise ValueError(f"selected function fn_{va:08x} still binds outside recovered code")
            if va in symbols and symbols[va] != address:
                raise ValueError(f"ambiguous selected function fn_{va:08x}")
            symbols[va] = address
    if set(symbols) != set(selected):
        raise ValueError("every selected function needs a compiled definition")
    return symbols


def _instruction_access_index(instructions):
    """Index strict interval overlap, preserving gaps and nested instructions."""
    spans = sorted((ins.va, ins.va + ins.size) for ins in instructions)
    starts = tuple(start for start, _ in spans)
    furthest_ends = tuple(accumulate((end for _, end in spans), max))

    def overlaps(address, size):
        # A matching span starts strictly before the access end. Among every
        # eligible span, one ends after the access start iff this prefix's
        # maximum end does. Prefix maxima retain nested/overlapping spans.
        candidate = bisect_left(starts, address + size) - 1
        return candidate >= 0 and furthest_ends[candidate] > address

    return overlaps


def _run(va, lifted, built, symbols, opts):
    original = emu.Machine(image.primary(), watch=(va, lifted[va].region_end))
    recovered = emu.Machine(image.primary(), built.image)
    # A hard-coded original call/callback to ANY selected member is a failure.
    for item in lifted.values():
        recovered.uc.hook_add(unicorn.UC_HOOK_CODE, recovered._escape,
                              begin=item.va, end=item.region_end - 1)
    visits = {}
    reverse = {address: entry for entry, address in symbols.items()}

    def visit(uc, address, size, data):
        if address in reverse:
            entry = reverse[address]
            visits[entry] = visits.get(entry, 0) + 1

    recovered.uc.hook_add(unicorn.UC_HOOK_CODE, visit,
                          begin=built.image.text.va, end=built.image.text.end - 1)
    amap = emu.AddressMap(built.symbols, {}, built.image.text.va, built.image.text.end)
    rng, templates = random.Random(opts.seed), difftest.arena_templates()
    inputs, covered, outcomes = set(), set(), {}
    instruction_access = _instruction_access_index(lifted[va].insns.values())
    mismatches, inconclusive, code_as_data = [], 0, 0
    mapping_faults = []
    digest = hashlib.sha256()
    compare_fp = bool(getattr(built.image, 'x87_bindings', ()))
    for index in range(opts.cases):
        case = difftest.make_case(rng, opts, sorted(inputs), templates)
        a = original.run(va, case, opts.budget)
        fp_a = x87_transport.snapshot(original.uc) if compare_fp else None
        b = recovered.run(symbols[va], case, opts.budget)
        fp_b = x87_transport.snapshot(recovered.uc) if compare_fp else None
        if index < opts.discovery:
            inputs |= difftest._global_reads(a, image.primary())
        covered |= a.covered
        outcomes[a.outcome] = outcomes.get(a.outcome, 0) + 1
        na, nb = difftest._normalize(a, None, "abi"), difftest._normalize(b, amap, "abi")
        fault_mapping = getattr(b, 'fault_mapping', None)
        if fault_mapping is not None:
            mapping_faults.append({'case': index, 'proof': fault_mapping,
                                   'relation_used': a.outcome == b.outcome == 'fault'
                                   and difftest._mapping_fault_pair(na[1], nb[1], fault_mapping)})
        digest.update(difftest._digest_view(na).encode())
        if compare_fp:
            digest.update(repr(fp_a).encode())
        if "budget" in (a.outcome, b.outcome):
            inconclusive += 1
            continue
        difference = difftest._diff(na, nb, amap, fault_mapping=fault_mapping)
        if compare_fp:
            difference = x87_transport.difference(fp_a, fp_b) or difference
        if difference and len(mismatches) < 5:
            mismatches.append({"case": index, **difference})
        # Embedded tables/alignment are data. Check the whole access so reads
        # starting in data or outside the extent cannot hide instruction bytes.
        if any(kind in ("r", "w") and instruction_access(address, size)
               for kind, address, size, _ in a.events):
            code_as_data += 1
    coverage = len(covered & set(lifted[va].insns)) / len(lifted[va].insns)
    good = (not mismatches and not inconclusive and not code_as_data
            and coverage >= opts.min_coverage and outcomes.get("return", 0) > 0)
    return {"status": "pass" if good else "fail", "cases": opts.cases, "seed": opts.seed,
            "budget": opts.budget, "min_coverage": opts.min_coverage,
            "coverage": coverage, "outcomes": outcomes, "mismatches": mismatches,
            "inconclusive": inconclusive, "code_as_data_cases": code_as_data,
            "result_digest": digest.hexdigest(),
            **({'mapping_faults': mapping_faults} if mapping_faults else {}),
            "fp_registers_compared": list(x87_transport.FP_REGISTERS) if compare_fp else [],
            "selected_entry_visits": {campaign._id(v): n for v, n in sorted(visits.items())},
            "observed_other_selected_entries": [campaign._id(v) for v in sorted(visits) if v != va]}


def test(manifest: dict, overrides: dict | None = None) -> dict:
    """Build and compare every selected C entry twice at O0/O2, without acceptance."""
    manifest = campaign._validate(manifest)
    selected = [campaign._va(v) for v in manifest["functions"]]
    if len(selected) < 2:
        raise ValueError("composition requires at least two selected functions")
    sources = [env.C_DIR / f"fn_{va:08x}.c" for va in selected]
    missing = [str(p) for p in sources if not p.is_file()]
    if missing:
        raise ValueError(f"selected C sources missing: {missing}")
    identities = {p.name: env.digest(p) for p in sources}
    jump_contracts = {campaign._id(va): contract for va in selected
                      if (contract := recover.jump_contract(va)) is not None}
    x87_contracts = {campaign._id(va): contract for va in selected
                     if (contract := recover.x87_contract(va)) is not None}
    execution_identity = recover.execution_identity()
    dependencies = {p.name: recover.c_dependencies(p) for p in sources}
    header = env.ROOT / "recovery/include/cmre.h"
    header_identity = env.digest(header)
    lifted = {va: lift.discover(va) for va in selected}
    if any(item.problems for item in lifted.values()):
        raise ValueError("selected function discovery has unresolved problems")
    if any(any(ins.mnemonic.startswith('f') for ins in item.insns.values())
           and campaign._id(va) not in x87_contracts for va, item in lifted.items()):
        raise ValueError('experimental composition excludes x87 instructions without a dedicated parent contract')
    options = {va: difftest.Options.for_function(va, overrides) for va in selected}
    if any(o.cases <= 0 or o.budget <= 0 or not 0 < o.min_coverage <= 1 for o in options.values()):
        raise ValueError("positive cases/budget and coverage in (0, 1] are required")
    builds = {}
    with tempfile.TemporaryDirectory(prefix="cmre-composition-") as temporary:
        root = Path(temporary)
        combined = root / "composed.c"
        combined.write_text("".join(f'#include "{p.resolve().as_posix()}"\n' for p in sources), encoding="utf-8")
        for opt in ("-O0", "-O2"):
            work = root / opt[1:]
            work.mkdir()
            try:
                built = build.build(combined, lift.symbol(selected[0]), work, opt)
                if any(binding.owner not in selected for binding in built.image.jump_bindings):
                    raise build.BuildError('composition excludes unselected indexed-JMP owners')
                x87_transport.require_complete(built.image.x87_bindings, lifted)
                symbols = _symbols(built, selected)
            except (build.BuildError, ValueError) as exc:
                builds[opt] = {"status": "build-failed", "detail": str(exc),
                               "stderr": getattr(exc, "stderr", "")[-2000:]}
                continue
            entries = {}
            for va in selected:
                first = _run(va, lifted, built, symbols, options[va])
                second = _run(va, lifted, built, symbols, options[va])
                if first != second:
                    first = {**first, "status": "nondeterministic"}
                entries[campaign._id(va)] = first
            builds[opt] = {"status": "pass" if all(r["status"] == "pass" for r in entries.values()) else "fail",
                           "return_transport": return_transport.report(built.image.return_bindings),
                           "jump_transport": jump_transport.report(built.image.jump_bindings),
                           "x87_transport": x87_transport.report(built.image.x87_bindings),
                           "observed_composed_calls": any(r["observed_other_selected_entries"] for r in entries.values()),
                           "compiled_symbols": {campaign._id(v): hex(a) for v, a in sorted(symbols.items())},
                           "entries": entries}
    changed = [p.name for p in sources if env.digest(p) != identities[p.name]]
    if env.digest(header) != header_identity:
        changed.append("recovery/include/cmre.h")
    if recover.execution_identity() != execution_identity:
        changed.append('execution-identity')
    for va, source in zip(selected, sources):
        contract = jump_contracts.get(campaign._id(va))
        if contract and (not (env.ROOT / contract['path']).exists()
                         or env.digest(env.ROOT / contract['path']) != contract['sha256']):
            changed.append(source.name + ':jump-contract')
        if recover.x87_contract(va) != x87_contracts.get(campaign._id(va)):
            changed.append(source.name + ':x87-contract')
        if recover.c_dependencies(source) != dependencies[source.name]:
            changed.append(source.name + ':dependencies')
        if difftest.Options.for_function(va, overrides).__dict__ != options[va].__dict__:
            changed.append(source.name + ':test-inputs')
    return {"schema": "cmre-composition-report/1", "input_sha256": env.TARGET_SHA256,
            "campaign": manifest, "source_sha256": identities,
            "header_sha256": header_identity,
            "source_dependencies": dependencies, "execution_identity": execution_identity,
            "jump_contracts": jump_contracts,
            "x87_contracts": x87_contracts,
            "options": {campaign._id(v): o.__dict__ for v, o in options.items()},
            "harness_sha256": recover.harness_digest(),
            "composition_sha256": env.digest(Path(__file__)), "toolchain": env.toolchain_identity(),
            "status": "pass" if not changed and all(b["status"] == "pass" for b in builds.values()) else "fail",
            "observed_composed_calls_at_both_optimizations": all(b.get("observed_composed_calls", False) for b in builds.values()),
            "changed_inputs": changed, "builds": builds, "milestone_accepted": False,
            "limitations": ["Experimental composed C corpus; no ledger acceptance or live integration.",
                            "Outside callees and original image data remain dependencies; imports are scaffolding.",
                            "Shared translation unit may reject static-name collisions; resolve source organization explicitly.",
                            "A pass requires a normal return but may contain matching faults; inspect outcomes and selected-entry visits.",
                            "Compiler-selected includes are bounded to each source and recovery/include."]}


def _command(args) -> int:
    try:
        overrides = {"cases": args.cases} if args.cases is not None else None
        result = test(campaign.load(args.manifest), overrides)
        identity = env.digest_bytes(json.dumps(result, sort_keys=True).encode())
        path = env.ANALYSIS / 'cmre/compositions' / (identity + '.json')
        coordination.atomic_json(path, result)
        response = result if args.json else {'status': result['status'], 'report_path': env.rel(path),
                  'observed_composed_calls_at_both_optimizations': result['observed_composed_calls_at_both_optimizations'],
                  'milestone_accepted': False, 'limitations': result['limitations']}
        print(json.dumps(response, indent=2))
        return 0 if result["status"] == "pass" else 1
    except (ValueError, OSError) as exc:
        print(json.dumps({"status": "error", "detail": str(exc)}))
        return 1


def add_commands(sub) -> None:
    parser = sub.add_parser("compose-test", help="experimental selected-C composition differential report")
    parser.add_argument("manifest")
    parser.add_argument("--cases", type=int)
    parser.add_argument('--json', action='store_true', help='print full report as well as saving it')
    parser.set_defaults(func=_command)
