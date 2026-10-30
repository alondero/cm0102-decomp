"""Resumable item receipts for machine jobs; process exit is not acceptance."""
from __future__ import annotations

from datetime import datetime, timezone
import json
import os
from pathlib import Path
import time
import uuid

from . import coordination, env, recover


def item_identity(va: int) -> dict:
    """Include mutable per-function inputs and the evidence being refreshed."""
    paths = {'hints': env.HINTS_DIR / f'fn_{va:08x}.json',
             'lifted': env.LIFTED_DIR / f'fn_{va:08x}.S',
             'c': env.C_DIR / f'fn_{va:08x}.c',
             'ledger': env.LEDGER_DIR / f'fn_{va:08x}.json',
             'blocked': env.BLOCKED_DIR / f'fn_{va:08x}.json'}
    return {name: env.digest(path) if path.is_file() else None for name, path in paths.items()}


class Job:
    def __init__(self, kind: str, functions: list[int], options: dict, resume: str | None = None,
                 retry_failed: bool = False):
        identity = {'harness': recover.harness_digest(), 'execution': recover.execution_identity()}
        if retry_failed and (not resume or kind != 'revalidate'):
            raise ValueError('Retry failed requires a completed revalidation receipt')
        if resume:
            self.path = Path(resume).resolve()
            if not self.path.is_relative_to((env.ANALYSIS / 'cmre/jobs').resolve()):
                raise ValueError('Resume receipt must be under CM0102/Analysis/cmre/jobs')
            self.data = json.loads(self.path.read_text(encoding='utf-8'))
            if self.data.get('schema') != 'cmre-job/2' or self.data['kind'] != kind:
                raise ValueError('Wrong job receipt kind/schema')
            if self.data['identity'] != identity or self.data['options'] != options:
                raise ValueError('Job inputs changed; start a new job instead of resuming stale evidence')
            if retry_failed and (self.data.get('state') != 'finished'
                                 or self.data['functions'] != [f'{va:08x}' for va in functions]):
                raise ValueError('Retry requires a finished receipt with the complete current selection')
            # Per-item completion files avoid rewriting the entire inventory on
            # every completion. The final manifest also retains the summary.
            for completed in self.path.with_suffix('').glob('*.json'):
                if completed.stem not in self.data['functions']:
                    raise ValueError('Unexpected item in job receipt')
                self.data['results'][completed.stem] = json.loads(completed.read_text(encoding='utf-8'))
            changed = []
            for va in self.data['functions']:
                if retry_failed and not self.data['results'].get(va, {}).get('ok', False):
                    continue
                previous = self.data['results'].get(va, {}).get('output_identity', self.data['item_inputs'][va])
                current = item_identity(int(va, 16))
                if current != previous:
                    if (options.get('refresh') and va not in self.data['results']
                            and {k for k in current if current[k] != previous.get(k)} == {'ledger'}):
                        # An interrupted refresh can publish an in-flight item without its
                        # receipt. It is still pending, so it reruns in full; nothing is reused.
                        continue
                    if retry_failed:
                        changed.append(va)
                    else:
                        raise ValueError(f'Job inputs changed for fn_{va}; start a new job')
            if retry_failed:
                previous_path = self.path
                failed = {k: v for k, v in self.data['results'].items() if not v['ok']}
                if (not failed and not changed) or len(self.data['results']) != len(self.data['functions']):
                    raise ValueError('Retry requires a complete receipt containing failures or changed items')
                stamp = datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S')
                self.path = previous_path.parent / f'{stamp}-{kind}-{uuid.uuid4().hex[:8]}.json'
                self.data = {**self.data, 'started': time.time(), 'retry_of': str(previous_path),
                             'previous_failures': failed,
                             'previous_changed_successes': {k: self.data['results'][k] for k in changed},
                             'results': {k: v for k, v in self.data['results'].items() if v['ok'] and k not in changed},
                             'item_inputs': {k: item_identity(int(k, 16)) for k in self.data['functions']}}
                self.data.pop('ended', None)
        else:
            stamp = datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S')
            self.path = env.ANALYSIS / 'cmre/jobs' / f'{stamp}-{kind}-{uuid.uuid4().hex[:8]}.json'
            self.data = {'schema': 'cmre-job/2', 'kind': kind, 'identity': identity,
                         'options': options, 'started': time.time(), 'state': 'running',
                         'functions': [f'{va:08x}' for va in functions], 'results': {},
                         'item_inputs': {f'{va:08x}': item_identity(va) for va in functions}}
        self.data['state'] = 'running'
        self.data['pid'] = os.getpid()
        self.save()

    @property
    def pending(self) -> list[int]:
        return [int(va, 16) for va in self.data['functions'] if va not in self.data['results']]

    def save(self):
        coordination.atomic_json(self.path, self.data)

    def record(self, va: int, ok: bool, detail: str):
        key = f'{va:08x}'
        result = {'ok': bool(ok), 'detail': detail, 'ended': time.time(),
                  'output_identity': item_identity(va)}
        coordination.atomic_json(self.path.with_suffix('') / f'{key}.json', result)
        self.data['results'][key] = result

    def finish(self):
        self.data.update(state='finished' if not self.pending else 'incomplete', ended=time.time())
        self.save()

    @property
    def failures(self):
        return sum(not result['ok'] for result in self.data['results'].values())


def describe(receipt: str | None = None) -> dict:
    root = env.ANALYSIS / 'cmre/jobs'
    if receipt is None:
        paths = sorted(root.glob('*.json'), key=lambda p: p.stat().st_mtime_ns)
        if not paths:
            return {'status': 'no-jobs'}
        path = paths[-1]
    else:
        path = Path(receipt).resolve()
        if not path.is_relative_to(root.resolve()):
            raise ValueError('Job receipt must be under CM0102/Analysis/cmre/jobs')
    data = json.loads(path.read_text(encoding='utf-8'))
    results = dict(data['results'])
    for item in path.with_suffix('').glob('*.json'):
        results[item.stem] = json.loads(item.read_text(encoding='utf-8'))
    failures = [{'function': 'fn_' + va, 'detail': result['detail']}
                for va, result in sorted(results.items()) if not result['ok']]
    return {'receipt': env.rel(path), 'kind': data['kind'], 'recorded_state': data['state'],
            'pid': data.get('pid'), 'selected': len(data['functions']), 'completed': len(results),
            'passed': len(results) - len(failures), 'failed': len(failures),
            'remaining': len(data['functions']) - len(results), 'first_failures': failures[:5],
            'note': 'Recorded running state does not prove process liveness; inspect the supervisor receipt.'}
