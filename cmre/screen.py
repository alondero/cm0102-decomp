"""Original-only scheduling evidence; never a replacement for C validation."""
from __future__ import annotations

import json
from pathlib import Path
import random
import time

from . import acceptance_request, coordination, difftest, emu, env, image, lift, recover, tasks

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
    machine = emu.Machine(img, watch=region,
                          arena_size=opts.hints.get('arena_size', emu.ARENA_SIZE))
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


def current_identity(va: int) -> tuple[dict, difftest.Options]:
    opts = difftest.Options.for_function(va, tier='c')
    opts.inputs = recover.c_inputs(va)
    return _identity(va, opts), opts


def _directory(va: int, identity: dict) -> Path:
    return env.ANALYSIS / f'cmre/screens/fn_{va:08x}' / tasks._hash(identity)


def require_promotion(va: int, gate: dict, binding: dict | None) -> None:
    """Require current original and independently built return evidence."""
    if not isinstance(binding, dict) or not binding.get('receipt') or not binding.get('sha256'):
        raise ValueError('Worktree promotion requires a reviewed return-screen receipt')
    identity, _ = current_identity(va)
    path = (env.ROOT / binding['receipt']).resolve()
    if path.parent != _directory(va, identity).resolve():
        raise ValueError('Stale return-screen receipt; screen and review the current inputs')
    raw = path.read_bytes()
    if env.digest_bytes(raw) != binding['sha256']:
        raise ValueError('Reviewed return-screen receipt changed')
    screened = json.loads(raw)
    if (screened.get('schema') != SCHEMA or screened.get('identity') != identity
            or gate.get('execution_identity') != identity['execution']
            or gate.get('profile') != identity['profile']
            or gate.get('effective_options_sha256') != acceptance_request.digest_json(identity['options'])):
        raise ValueError('Return-screen and C gate must bind the same current inputs')
    builds = gate.get('builds', {})
    if gate.get('status') != 'pass' or set(builds) != set(recover.C_OPT_LEVELS):
        raise ValueError('Worktree promotion requires both passing C builds')

    original_returns = screened.get('outcomes', {}).get('return', 0)
    if type(original_returns) is not int or original_returns < 0:
        raise ValueError('Invalid return count in reviewed screen')
    returns = []
    for opt in recover.C_OPT_LEVELS:
        build = builds[opt]
        count = build.get('difftest', {}).get('outcomes', {}).get('return', 0)
        if build.get('status') != 'pass' or type(count) is not int or count < 0:
            raise ValueError(f'Invalid return evidence for {opt}')
        returns.append(count)
    if binding.get('nonreturning_reason'):
        if original_returns or any(returns) or screened.get('status') != 'evidence-needed':
            raise ValueError('Observed return contradicts the nonreturning review')
    elif (not original_returns or not all(returns)
          or screened.get('status') != 'return-observed'
          or type(screened.get('first_return_case')) is not int
          or not 0 <= screened['first_return_case'] < screened['cases']):
        raise ValueError('Worktree promotion needs a top-level return in the screen and both C builds')


def run(va: int) -> dict:
    identity, opts = current_identity(va)
    if identity['profile'] != 'semantic':
        raise ValueError('Screen supports the semantic C lane only')
    directory = _directory(va, identity)
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
    p = sub.add_parser('screen', help='screen original C cases for returns before worktree promotion')
    p.add_argument('va')
    p.set_defaults(func=command)


def command(args) -> int:
    from .cli import _va
    report = run(_va(args.va))
    print(json.dumps(report, indent=2))
    return 0 if report['status'] == 'return-observed' else 1
