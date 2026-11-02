"""Original-only scheduling evidence; never a replacement for C validation."""
from __future__ import annotations

import json
from pathlib import Path
import random
import time

from . import coordination, difftest, emu, env, image, lift, recover, tasks

SCHEMA = 'cmre-return-screen/1'


def _identity(va: int, opts) -> dict:
    execution = recover.execution_identity()
    if (execution['target'] != env.TARGET_SHA256 or
            execution['inventory'] != {env.rel(p): h for p, h in env.INVENTORY_SHA256.items()}):
        raise ValueError('Screen requires pinned target and inventory')
    hints = env.HINTS_DIR / f'fn_{va:08x}.json'
    return {'schema': SCHEMA, 'va': f'0x{va:08x}', 'execution': execution,
            'options': json.loads(json.dumps(opts.__dict__)),
            'hints': env.digest(hints) if hints.exists() else None,
            'profile': recover.c_profile(va), 'inputs': recover.c_inputs(va),
            'observer': env.digest(Path(__file__))}


def observe(va: int, lifted, opts) -> dict:
    """Replay every C case, including discovery, fixtures and indirect boundaries."""
    img = image.primary()
    region = (lifted.va, lifted.region_end)
    machine = emu.Machine(img, watch=region)
    difftest._stop_indirect(machine, None, region, None, recover._indirect_calls(lifted))
    rng = random.Random(opts.seed)
    templates = difftest.arena_templates()
    globals_in, covered = set(), set()
    outcomes, stops = {}, {}
    first_return = None
    for index in range(opts.cases):
        case = difftest.make_case(rng, opts, sorted(globals_in), templates)
        own = difftest._discovery(machine, opts, index, region)
        result = machine.run(va, case, opts.budget)
        if index < opts.discovery:
            globals_in |= difftest._global_reads(result, img, own)
        covered |= result.covered
        outcomes[result.outcome] = outcomes.get(result.outcome, 0) + 1
        if result.outcome == 'return' and first_return is None:
            first_return = index
        stop = result.outcome
        if result.outcome == 'fault':
            (access, _address, _size), eip = result.detail
            location = 'function' if region[0] <= eip < region[1] else 'callee'
            stop = f'fault in {location} eip={eip:#010x} {difftest.ACCESS.get(access, access)}'
        stops[stop] = stops.get(stop, 0) + 1
    return {'cases': opts.cases, 'outcomes': dict(sorted(outcomes.items())),
            'stops': dict(sorted(stops.items())), 'first_return_case': first_return,
            'coverage': len(covered & set(lifted.insns)) / len(lifted.insns) if lifted.insns else 0.0,
            'uncovered': [f'{x:#010x}' for x in sorted(set(lifted.insns) - covered)],
            'status': 'return-observed' if first_return is not None else 'evidence-needed',
            'note': 'Scheduling evidence only. A return does not establish ABI, path completeness, '
                    'candidate equivalence, dependency closure or acceptance. Retain all fault cases.'}


def run(va: int) -> dict:
    opts = difftest.Options.for_function(va, tier='c')
    opts.inputs = recover.c_inputs(va)
    identity = _identity(va, opts)
    if identity['profile'] != 'semantic':
        raise ValueError('Screen supports the semantic C lane only')
    key = tasks._hash(identity)
    directory = env.ANALYSIS / f'cmre/screens/fn_{va:08x}' / key
    with coordination.lock(directory / '.lock'):
        index = directory / 'latest.json'
        if index.exists():
            try:
                saved = json.loads(index.read_text(encoding='utf-8'))
                name = saved['report']
                if Path(name).name != name or not name.endswith('.json'):
                    raise ValueError('Invalid screen report path')
                path = directory / name
                report = json.loads(path.read_text(encoding='utf-8'))
                if env.digest(path) == saved['sha256'] and report['identity'] == identity:
                    return {**report, 'reused': True, 'receipt': env.rel(path)}
            except (OSError, ValueError, KeyError, TypeError):
                pass  # Keep incomplete/corrupt evidence; execute a new receipt.
        lifted = lift.discover(va)
        if lifted.problems:
            raise ValueError(f'Unresolved original boundaries: {lifted.problems}')
        started = time.perf_counter()
        result = observe(va, lifted, opts)
        elapsed = time.perf_counter() - started
        if _identity(va, opts) != identity:
            raise ValueError('Screen inputs changed during execution')
        report = {'schema': SCHEMA, 'identity': identity, 'validation_seconds': elapsed, **result}
        path = directory / f'{time.time_ns()}.json'
        coordination.atomic_json(path, report)
        coordination.atomic_json(index, {'report': path.name, 'sha256': env.digest(path)})
        return {**report, 'reused': False, 'receipt': env.rel(path)}


def add_commands(sub) -> None:
    p = sub.add_parser('screen', help='screen original C cases for returns before dispatch (advisory)')
    p.add_argument('va')
    p.set_defaults(func=command)


def command(args) -> int:
    from .cli import _va
    report = run(_va(args.va))
    print(json.dumps(report, indent=2))
    return 0 if report['status'] == 'return-observed' else 1
