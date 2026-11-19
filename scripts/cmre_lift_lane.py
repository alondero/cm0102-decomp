"""Lift lane: turn stored captures into accepted lifted functions without an expert session.

One pass (`once`) takes every function that has captures in the local store
(ADR 0009) and no accepted record, and whose capture set changed since its
last pass. It expands the captures (coverage-guided mutation, original side
only), lists the selected captures in the function's hints, runs the unchanged
lifted gate and accepts what passes with `cmre accept`. Lifted functions then
enter the farm's C frontier on its next rescan.

The farm launches a pass every `lift_lane_minutes` (FARM.md "Lift lane"), so
new overnight captures reach the C translators without anyone remembering the
expand/apply/accept steps. Nothing here changes the gate: expansion and
selection are `cmre capture expand/apply`, acceptance is `cmre accept`.
State is local evidence in CM0102/Analysis/cmre/lift-lane/; never publish it.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_STATE = ROOT / 'CM0102/Analysis/cmre/lift-lane/state.json'
MAX_ERRORS = 3          # tool errors (not gate results) retried this many passes, then left for a person


def _fn(va):
    return f'fn_{va:08x}'


def capture_key(names):
    return hashlib.sha256('\n'.join(sorted(names)).encode()).hexdigest()


class CmreOps:
    """The real capture store and cmre commands."""

    def __init__(self, root=ROOT):
        self.root = Path(root)
        self._inventory = None

    def stored(self):
        from cmre import capture_expand
        return capture_expand.stored()

    def recovered(self):
        from cmre import ledger
        return set(ledger.recovered())

    def names_for(self, va):
        from cmre import capture
        return capture.names_for(va)

    def body_bytes(self, va):
        if self._inventory is None:
            from cmre import inventory
            self._inventory = inventory.load()
        entry = self._inventory.functions.get(va)
        return entry.body_bytes if entry else 0

    def expand(self, vas, jobs):
        from cmre import capture_expand
        return capture_expand.run_many('expand', list(vas), jobs)

    def apply(self, vas, jobs):
        from cmre import capture_expand
        return capture_expand.run_many('apply', list(vas), jobs, test=True)

    def accept(self, fn):
        done = subprocess.run([sys.executable, '-m', 'cmre', 'accept', fn], cwd=self.root,
                              capture_output=True, text=True, encoding='utf-8', errors='replace')
        last = (done.stdout or '').strip().splitlines()[-1:] or ['']
        try:
            status = json.loads(last[0]).get('status')
        except (ValueError, AttributeError):
            status = None
        if done.returncode == 0 and status == 'accepted':
            return 'accepted'
        if 'Writer active' in (done.stderr or '') or 'BusyError' in (done.stderr or ''):
            return 'busy'
        return 'refused: ' + ((done.stderr or done.stdout or '').strip().splitlines() or ['no output'])[-1][:200]


def _load(path):
    try:
        return json.loads(Path(path).read_text(encoding='utf-8'))
    except (OSError, ValueError):
        return {'functions': {}}


def _save(path, state):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix('.tmp')
    tmp.write_text(json.dumps(state, indent=1, sort_keys=True) + '\n', encoding='utf-8')
    os.replace(tmp, path)


def plan(ops, rows, recovered, limit=None):
    """Captured, unrecovered functions whose capture set changed since their last pass."""
    todo = []
    for va in ops.stored():
        row = rows.get(_fn(va), {})
        if va in recovered or row.get('errors', 0) >= MAX_ERRORS:
            continue
        if row.get('key') != capture_key(ops.names_for(va)):
            todo.append(va)
    todo.sort(key=lambda va: (-ops.body_bytes(va), va))
    return todo[:limit] if limit else todo


def _accept(ops, fn, row, accepted, waiting):
    outcome = ops.accept(fn)
    row['accept'] = outcome
    if outcome == 'accepted':
        accepted.append(fn)
    elif outcome == 'busy':
        waiting.append(fn)


def run_once(ops, state_path, jobs=4, limit=None, clock=time.time):
    state = _load(state_path)
    rows = state.setdefault('functions', {})
    recovered = ops.recovered()
    accepted, waiting = [], []
    # Passes whose accept met another writer: accept again, no new expansion needed.
    for fn, row in sorted(rows.items()):
        if row.get('accept') == 'busy' and int(fn[3:], 16) not in recovered:
            _accept(ops, fn, row, accepted, waiting)
    todo = plan(ops, rows, recovered, limit)
    statuses = {}
    if todo:
        ops.expand(todo, jobs)
        for result in ops.apply(todo, jobs):
            va = int(result['va'], 16)
            fn, status = _fn(va), str(result.get('status'))
            statuses[status] = statuses.get(status, 0) + 1
            row = {'status': status, 'coverage': result.get('coverage'), 'at': clock(),
                   'bytes': ops.body_bytes(va)}
            if status == 'error':
                row['errors'] = rows.get(fn, {}).get('errors', 0) + 1
                row['error'] = str(result.get('error', ''))[:300]
            else:
                row['key'] = capture_key(ops.names_for(va))   # after expansion: its own mutants are not news
            rows[fn] = row
            if status == 'pass':
                _accept(ops, fn, row, accepted, waiting)
    summary = {'at': clock(), 'processed': len(todo), 'statuses': statuses, 'accepted': accepted,
               'accepted_bytes': sum(ops.body_bytes(int(fn[3:], 16)) for fn in accepted),
               'accept_waiting': waiting}
    state['last'] = summary
    _save(state_path, state)
    return summary


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest='action', required=True)
    once = sub.add_parser('once', help='one expand/apply/accept pass over changed captured functions')
    once.add_argument('--jobs', type=int, default=4)
    once.add_argument('--limit', type=int, help='largest N functions this pass')
    once.add_argument('--state', type=Path, default=DEFAULT_STATE)
    once.add_argument('--output', type=Path, help='also write the pass summary here')
    status = sub.add_parser('status', help='last pass and per-status counts')
    status.add_argument('--state', type=Path, default=DEFAULT_STATE)
    a = parser.parse_args(argv)
    if a.action == 'status':
        state = _load(a.state)
        counts = {}
        for row in state['functions'].values():
            counts[row.get('status')] = counts.get(row.get('status'), 0) + 1
        print(json.dumps({'last': state.get('last'), 'functions': counts}, indent=1))
        return 0
    sys.path.insert(0, str(ROOT))
    summary = run_once(CmreOps(), a.state, jobs=a.jobs, limit=a.limit)
    if a.output:
        _save(a.output, summary)
    print(json.dumps(summary))
    return 0


if __name__ == '__main__':
    sys.exit(main())
