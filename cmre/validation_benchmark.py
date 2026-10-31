"""Bounded reproducible validation benchmark in a retained local copy of inputs."""
from __future__ import annotations

from contextlib import contextmanager
import json
from pathlib import Path
import shutil
import time

from . import coordination, env, ledger, validation as v


@contextmanager
def copied_workspace(root: Path, vas: list[int]):
    names = ('ROOT', 'ANALYSIS', 'RUNS', 'C_DIR', 'LIFTED_DIR', 'HINTS_DIR', 'LEDGER_DIR')
    original = {name: getattr(env, name) for name in names}
    root.mkdir(parents=True)
    shutil.copytree(env.ROOT / 'recovery/include', root / 'recovery/include')
    try:
        for name, directory, suffix in [('C_DIR', 'c', '.c'), ('LIFTED_DIR', 'lifted', '.S'),
                                        ('HINTS_DIR', 'hints', '.json'), ('LEDGER_DIR', 'ledger', '.json')]:
            target = root / 'recovery' / directory
            target.mkdir()
            for va in vas:
                source = original[name] / f'fn_{va:08x}{suffix}'
                if source.is_file():
                    shutil.copy2(source, target / source.name)
            setattr(env, name, target)
        env.ROOT = root
        env.ANALYSIS = root / 'analysis'
        env.RUNS = env.ANALYSIS / 'cmre/runs'
        v.worker_identity.cache_clear()
        yield
    finally:
        for name, value in original.items():
            setattr(env, name, value)
        v.worker_identity.cache_clear()


def measure(vas: list[int], label: str, *, execute: bool) -> dict:
    started = time.monotonic()
    plan = v.plan(vas)
    if execute:
        run = v.Run(plan)
        identity = v.digest(plan['identity'])
        for key, item in plan['items'].items():
            if item['action'] == 'execute':
                run.record(key, v.execute(item, identity, False))
        run.finish()
        summary = run.data['summary']
        receipt = str(run.path)
    else:
        summary = v.summary(plan)
        receipt = None
    return {'scenario': label, 'mode': 'executed' if execute else 'impact only',
            'elapsed_seconds': time.monotonic() - started,
            'reuse_rate': summary['reused'] / max(1, summary['selected_tiers']),
            'summary': summary, 'receipt': receipt,
            'selected': [key for key, item in plan['items'].items() if item['action'] == 'execute']}


def command(args) -> int:
    vas = [int(f.lower().removeprefix('fn_'), 16) for f in args.functions]
    if not vas or len(vas) > 8 or len(set(vas)) != len(vas):
        raise ValueError('Select one to eight distinct accepted C functions')
    if any(not v.read(ledger.record_path(va)).get('c') for va in vas):
        raise ValueError('Benchmark requires accepted C tiers')
    directory = env.ANALYSIS / 'cmre/benchmarks' / str(time.time_ns())
    result = {'schema': 'cmre-validation-benchmark/1', 'functions': args.functions,
              'workspace': str(directory / 'workspace'), 'scenarios': [],
              'claims': 'Copied inputs only; no acceptance, source recovery, live integration or canonical input edits.'}
    with copied_workspace(directory / 'workspace', vas):
        rows = result['scenarios']
        rows.append(measure(vas, 'cold copied corpus', execute=True))
        rows.append(measure(vas, 'unchanged repeat', execute=True))
        hint = env.HINTS_DIR / f'fn_{vas[0]:08x}.json'
        original_hint = hint.read_bytes() if hint.exists() else None
        hints = json.loads(original_hint) if original_hint else {}
        overlay = hints.setdefault('c_hints', {})
        fixtures = overlay.get('fixtures', hints.get('fixtures', []))
        if fixtures:
            overlay['fixtures'] = json.loads(json.dumps(fixtures))
            overlay['fixtures'][0]['import_seed'] ^= 1
        else:
            overlay['ecx'] = 'ptr' if overlay.get('ecx', hints.get('ecx')) != 'ptr' else 'small'
        hint.write_text(json.dumps(hints), encoding='utf-8')
        rows.append(measure(vas, 'C-only fixture/input edit (#23 tier isolation)', execute=True))
        # Subsequent changes are impact-only; their snapshots are compared with
        # the retained unchanged baseline, never fabricated passing evidence.
        baseline = rows[1]['receipt']
        if original_hint is None:
            hint.unlink()
        else:
            hint.write_bytes(original_hint)
        hints = json.loads(original_hint) if original_hint else {}
        hints['seed'] = (hints.get('seed', vas[0]) + 1) & 0xffffffff
        hint.write_text(json.dumps(hints), encoding='utf-8')
        # Explicit baseline selection avoids the deliberately changed C receipt.
        def impact(label):
            started = time.monotonic()
            plan = v.plan(vas, reuse=baseline)
            summary = v.summary(plan)
            rows.append({'scenario': label, 'mode': 'impact only', 'elapsed_seconds': time.monotonic() - started,
                         'reuse_rate': summary['reused'] / summary['selected_tiers'], 'summary': summary,
                         'selected': [k for k, item in plan['items'].items() if item['action'] == 'execute']})
        impact('shared seed edit')
        if original_hint is None:
            hint.unlink()
        else:
            hint.write_bytes(original_hint)
        source = env.C_DIR / f'fn_{vas[0]:08x}.c'
        original_source = source.read_bytes()
        source.write_bytes(original_source + b'\n')
        impact('C source edit')
        source.write_bytes(original_source)
        header = env.ROOT / 'recovery/include/cmre.h'
        original_header = header.read_bytes()
        header.write_bytes(original_header + b'\n/* benchmark dependency change */\n')
        impact('shared header edit')
        header.write_bytes(original_header)
        # Audit exercises the same complete selection required by a global
        # change. The global-change negative regressions test the invalidator.
        plan = v.plan(vas, reuse=baseline, audit=True)
        rows.append({'scenario': 'global-scope audit selection', 'mode': 'impact only',
                     'summary': v.summary(plan), 'selected': list(plan['items'])})
    output = directory / 'benchmark.json'
    coordination.atomic_json(output, result)
    print(json.dumps({'report': str(output), **result}, indent=2))
    # A failed mutated corpus is recorded as a failure; it need not mean a
    # benchmark failure. The cold and unchanged controls must be fully green.
    controls = result['scenarios'][:2]
    return 1 if any(r['summary']['failed'] or r['summary']['pending'] for r in controls) else 0
