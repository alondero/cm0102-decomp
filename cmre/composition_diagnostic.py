"""Observable-effect diagnostics for linked C; never a composition gate receipt.

The existing ABI composition gate and its reports remain unchanged. This tool
uses the existing semantic normalizer, retains ABI differences for the same
executions, and writes to a separate diagnostic namespace.
"""
from __future__ import annotations

import hashlib
import json
import random
import re
import tempfile
from pathlib import Path

import unicorn

from . import build, campaign, composition, coordination, difftest, emu, env, image, lift, recover


def _inspector_identity():
    tool = Path(env.tool('objdump'))
    version = env.run_tool([str(tool), '--version'])
    if version.returncode or not version.stdout.splitlines():
        raise ValueError('composition diagnostic object inspector is unavailable')
    return {'path': str(tool), 'sha256': env.digest(tool), 'version': version.stdout.splitlines()[0]}


def _contracts(sources, bodies):
    returns = {}
    for va, source in sources.items():
        body = bodies[va]
        if body.problems:
            raise ValueError(f'{campaign._id(va)} has unresolved original discovery')
        if any(i.kind in ('indirect_call', 'indirect_jmp', 'table_jmp')
               or i.mnemonic.startswith('f') for i in body.insns.values()):
            raise ValueError('semantic composition diagnostic excludes selected indirect/x87 instructions')
        text = source.read_text(encoding='utf-8')
        if re.search(r'\b(?:__asm__|__asm|asm|naked|CM_ORIGINAL_CALL)\b', text):
            raise ValueError('semantic composition diagnostic requires plain C sources')
        kind, problems = recover._semantic_contract(va, text)
        if problems:
            raise ValueError(f'{campaign._id(va)} return contract: {"; ".join(problems)}')
        if kind == 'float':
            raise ValueError('semantic composition diagnostic excludes floating return contracts')
        returns[va] = kind
    return returns


def _plain_translations(sources, dependencies, returns):
    """Inspect expansion at both modes; unused macro definitions disappear here."""
    include = (env.ROOT / 'recovery/include').resolve()
    for va, source in sources.items():
        for opt in recover.C_OPT_LEVELS:
            expanded = env.run_tool([env.tool('gcc'), '-m32', '-E', opt,
                '-fno-asynchronous-unwind-tables', '-ffreestanding', '-fno-builtin',
                f'-I{include}', str(source.resolve())])
            if expanded.returncode:
                raise build.BuildError('Diagnostic C preprocessing failed', expanded.stderr)
            # Keep this local inspection tied to the existing dependency contract.
            # A future preprocessing-flag change fails closed instead of drifting.
            if env.digest_bytes(expanded.stdout.encode('utf-8')) != dependencies[source.name]['preprocessed'][opt]:
                raise ValueError('Diagnostic preprocessing differs from the dependency proof')
            if re.search(r'\b(?:__asm__|__asm|asm|naked|CM_ORIGINAL_CALL)\b', expanded.stdout):
                raise ValueError('semantic composition diagnostic requires plain C after preprocessing')
            kind, problems = recover._semantic_contract(va, expanded.stdout)
            if kind == 'float':
                raise ValueError('semantic composition diagnostic excludes floating return contracts')
            if problems or kind != returns[va]:
                raise ValueError('Diagnostic return contract differs after preprocessing')


def _run(va, bodies, built, symbols, opts, returns):
    arena_size = opts.hints.get('arena_size', emu.ARENA_SIZE)
    original = emu.Machine(image.primary(), watch=(va, bodies[va].region_end), arena_size=arena_size)
    recovered = emu.Machine(image.primary(), built.image, arena_size=arena_size)
    for body in bodies.values():
        recovered.uc.hook_add(unicorn.UC_HOOK_CODE, recovered._escape,
                             begin=body.va, end=body.region_end - 1)
    visits = {}
    reverse = {address: entry for entry, address in symbols.items()}

    def visit(_uc, address, _size, _data):
        if address in reverse:
            entry = reverse[address]
            visits[entry] = visits.get(entry, 0) + 1

    recovered.uc.hook_add(unicorn.UC_HOOK_CODE, visit,
                         begin=built.image.text.va, end=built.image.text.end - 1)
    locations = composition._verified_fault_locations(bodies, built, symbols)
    amap = emu.AddressMap(built.symbols, {}, built.image.text.va, built.image.text.end,
                          verified_fault_locations=locations)
    accesses_code = composition._instruction_access_index(
        instruction for body in bodies.values() for instruction in body.insns.values())
    rng, templates = random.Random(opts.seed), difftest.arena_templates()
    inputs, covered, outcomes = set(), set(), {}
    mismatches, abi_mismatches = [], []
    mapping_faults = []
    inconclusive, code_as_data = 0, 0
    digest, abi_digest = hashlib.sha256(), hashlib.sha256()
    for index in range(opts.cases):
        case = difftest.make_case(rng, opts, sorted(inputs), templates)
        own = (difftest._discovery(original, opts, index, (va, bodies[va].region_end))
               if opts.discovery else None)
        a = original.run(va, case, opts.budget)
        fp_a = difftest.semantic_fp(original.uc, returns)
        b = recovered.run(symbols[va], case, opts.budget)
        fp_b = difftest.semantic_fp(recovered.uc, returns)
        if index < opts.discovery:
            inputs |= difftest._global_reads(a, image.primary(), own)
        covered |= a.covered
        outcomes[a.outcome] = outcomes.get(a.outcome, 0) + 1
        na = difftest._normalize(a, None, 'semantic', returns, fp_a)
        nb = difftest._normalize(b, amap, 'semantic', returns, fp_b)
        aa, ab = difftest._normalize(a, None, 'abi'), difftest._normalize(b, amap, 'abi')
        fault_mapping = getattr(b, 'fault_mapping', None)
        if fault_mapping is not None:
            mapping_faults.append({'case': index, 'proof': fault_mapping,
                'original_detail': a.detail, 'recovered_detail': b.detail,
                'relation_used': a.outcome == b.outcome == 'fault'
                    and difftest._mapping_fault_pair(na[1], nb[1], fault_mapping),
                'abi_relation_used': a.outcome == b.outcome == 'fault'
                    and difftest._mapping_fault_pair(aa[1], ab[1], fault_mapping)})
        digest.update(difftest._digest_view(na).encode())
        abi_digest.update(difftest._digest_view(aa).encode())
        if 'budget' in (a.outcome, b.outcome):
            inconclusive += 1
        else:
            for left, right, failures in ((na, nb, mismatches), (aa, ab, abi_mismatches)):
                difference = difftest._diff(left, right, amap,
                    fault_mapping=fault_mapping, strict_data_fault=True)
                if difference and len(failures) < 5:
                    failures.append({'case': index, **difference})
        if any(kind in ('r', 'w') and accesses_code(address, size)
               for result in (a, b) for kind, address, size, _ in result.events):
            code_as_data += 1
    coverage = len(covered & set(bodies[va].insns)) / len(bodies[va].insns)
    complete = (not inconclusive and not code_as_data and coverage >= opts.min_coverage
                and outcomes.get('return', 0) > 0)
    return {'status': 'pass' if complete and not mismatches else 'fail',
            'abi_comparison_status': 'pass' if complete and not abi_mismatches else 'fail',
            'profile': 'semantic', 'returns': returns, 'cases': opts.cases, 'seed': opts.seed,
            'budget': opts.budget, 'coverage': coverage, 'min_coverage': opts.min_coverage,
            'outcomes': outcomes, 'inconclusive': inconclusive, 'code_as_data_cases': code_as_data,
            'mismatches': mismatches, 'abi_first_mismatches': abi_mismatches,
            'mapping_faults': mapping_faults,
            'result_digest': digest.hexdigest(), 'abi_result_digest': abi_digest.hexdigest(),
            'selected_entry_visits': {campaign._id(v): n for v, n in sorted(visits.items())},
            'observed_other_selected_entries': [campaign._id(v) for v in sorted(visits) if v != va]}


def diagnose(manifest, overrides=None):
    manifest = campaign._validate(manifest)
    selected = [campaign._va(value) for value in manifest['functions']]
    if len(selected) < 2:
        raise ValueError('composition diagnostic requires at least two C functions')
    sources = {va: env.C_DIR / f'fn_{va:08x}.c' for va in selected}
    if any(not source.is_file() for source in sources.values()):
        raise ValueError('composition diagnostic requires every selected C source')
    identities = {source.name: env.digest(source) for source in sources.values()}
    bodies = {va: lift.discover(va) for va in selected}
    returns = _contracts(sources, bodies)
    options = {va: composition.composition_c_options(va, overrides) for va in selected}
    if any(o.cases <= 0 or o.budget <= 0 or not 0 < o.min_coverage <= 1 for o in options.values()):
        raise ValueError('positive cases/budget and coverage in (0, 1] are required')
    identity = recover.execution_identity()
    implementations = {'composition.py': env.digest(Path(composition.__file__)),
                       Path(__file__).name: env.digest(Path(__file__))}
    inspector = _inspector_identity()
    dependencies = {source.name: recover.c_dependencies(source) for source in sources.values()}
    _plain_translations(sources, dependencies, returns)
    builds = {}
    with tempfile.TemporaryDirectory(prefix='cmre-composition-diagnostic-') as temporary:
        for opt in recover.C_OPT_LEVELS:
            work = Path(temporary) / opt[1:]
            work.mkdir()
            try:
                built = composition._build_separate(list(sources.values()), lift.symbol(selected[0]), work, opt)
                if any(getattr(built.image, kind, ()) for kind in ('return_bindings','jump_bindings','x87_bindings')):
                    raise ValueError('semantic composition diagnostic excludes ABI transports')
                symbols = composition._symbols(built, selected)
                entries = {}
                for va in selected:
                    first = _run(va, bodies, built, symbols, options[va], returns[va])
                    second = _run(va, bodies, built, symbols, options[va], returns[va])
                    if first != second:
                        first = {**first, 'status': 'nondeterministic'}
                    entries[campaign._id(va)] = first
                builds[opt] = {**built.composition_artifacts,
                    'status': 'pass' if all(e['status'] == 'pass' for e in entries.values()) else 'fail',
                    'compiled_symbols': {campaign._id(v): hex(a) for v, a in sorted(symbols.items())},
                    'entries': entries,
                    'observed_composed_calls': any(e['observed_other_selected_entries'] for e in entries.values())}
            except (ValueError, build.BuildError) as error:
                builds[opt] = {'status': 'build-failed', 'detail': str(error)}
    changed = [source.name for source in sources.values() if env.digest(source) != identities[source.name]]
    for va, source in sources.items():
        if recover.c_dependencies(source) != dependencies[source.name]:
            changed.append(source.name + ':dependencies')
        if composition.composition_c_options(va, overrides).__dict__ != options[va].__dict__:
            changed.append(source.name + ':test-inputs')
    if recover.execution_identity() != identity:
        changed.append('execution-identity')
    try:
        if _inspector_identity() != inspector:
            changed.append('objdump')
    except ValueError:
        changed.append('objdump')
    for name, expected in implementations.items():
        if env.digest(Path(__file__).with_name(name)) != expected:
            changed.append(name)
    return {'schema': 'cmre-semantic-composition-diagnostic/1',
            'diagnostic_only': True, 'comparison_profile': 'semantic', 'milestone_accepted': False,
            'composition_gate_passed': False,
            'status': 'pass' if not changed and all(b['status'] == 'pass' for b in builds.values()) else 'fail',
            'campaign': manifest, 'source_sha256': identities, 'source_dependencies': dependencies,
            'execution_identity': identity, 'implementation_sha256': implementations,
            'objdump': inspector,
            'options': {campaign._id(va): opts.__dict__ for va, opts in options.items()},
            'returns': {campaign._id(va): kind for va, kind in returns.items()},
            'builds': builds, 'changed_inputs': changed,
            'observed_composed_calls_at_both_optimizations': all(b.get('observed_composed_calls', False) for b in builds.values()),
            'limitations': ['Diagnostic only; the existing ABI composition gate remains separate.',
                'No ledger acceptance, native exception parity, milestone or live integration claim.',
                'Original outside callees/data and import scaffolding remain boundaries.',
                'Selected indirect/x87 instructions, floating returns and ABI transports are unsupported.']}


def _command(args):
    try:
        overrides = {'cases': args.cases} if args.cases is not None else None
        result = diagnose(campaign.load(args.manifest), overrides)
        key = env.digest_bytes(json.dumps(result, sort_keys=True).encode())
        path = env.ANALYSIS / 'cmre/composition-diagnostics' / (key + '.json')
        coordination.atomic_json(path, result)
        print(json.dumps(result if args.json else {'status': result['status'], 'report_path': env.rel(path),
            'diagnostic_only': True, 'composition_gate_passed': False, 'milestone_accepted': False,
            'observed_composed_calls_at_both_optimizations': result['observed_composed_calls_at_both_optimizations']}, indent=2))
        return 0 if result['status'] == 'pass' else 1
    except (ValueError, OSError, build.BuildError) as error:
        print(json.dumps({'status': 'error', 'diagnostic_only': True, 'detail': str(error)}))
        return 1


def add_commands(sub):
    parser = sub.add_parser('compose-diagnose', help='semantic linked-C diagnostic; preserves the separate ABI gate')
    parser.add_argument('manifest')
    parser.add_argument('--cases', type=int)
    parser.add_argument('--json', action='store_true')
    parser.set_defaults(func=_command)
