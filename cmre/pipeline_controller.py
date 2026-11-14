"""Continuous supply/adoption service for the ordinary durable campaign runner.

Provider execution stays in provider_campaign and its scoped actor pool. This
service prepares frozen packets and adopts independently reviewed input bundles.
Each operation has a pending receipt before launch and a terminal receipt after;
interrupted or failed operations are held rather than silently retried.
"""
from __future__ import annotations

import copy
import json
import time
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from . import coordination, pipeline_policy


def scale(directory, stage, workers, *, owners=None, execution_workers=None, **buffers):
    from . import provider_jobs
    state = provider_jobs.load(directory)
    if not state['manifest'].get('pipeline'):
        raise ValueError('Stage scaling requires a pipeline-enabled run')
    def change(value):
        value['workers'][stage] = workers
        if execution_workers is not None:
            value['execution_workers'] = execution_workers
        if owners is not None:
            if stage not in value['owners']:
                raise ValueError('Only supply stages assign new owner roles')
            value['owners'][stage] = owners
        if buffers:
            if stage not in value['buffers']:
                raise ValueError('Only supply stages have buffer watermarks')
            value['buffers'][stage].update(buffers)
    return pipeline_policy.edit(directory, state['manifest'], change)


def status(directory):
    from . import pipeline_flow, provider_jobs
    state = provider_jobs.load(directory)
    if not state['manifest'].get('pipeline'):
        return None
    path = Path(directory) / 'pipeline/status.json'
    return {'policy': pipeline_policy.read(directory, state['manifest']),
            'inbox': pipeline_flow.inbox(directory),
            'last_service_snapshot': json.loads(path.read_text()) if path.exists() else None}


def enable(manifest):
    """Opt a fresh manifest into reviewed promotion and separate publication."""
    from . import env, work
    from .pipeline_flow import pin
    result = copy.deepcopy(manifest)
    result['pipeline'] = True
    states = work.read()['functions']
    for definition in result['jobs']:
        config = definition['config']
        config.update(defer_publication=True, defer_execution=True,
                      reserve_task=True, self_service_controls=True, auto_test_after_build=True)
        config.setdefault('pipeline_stage', 'author' if config['mode'] == 'c' else 'intake')
        if config['mode'] == 'input' and 'adoption_guard' not in config:
            va = int(config['function'], 16)
            fid = f'fn_{va:08x}'
            hints, source = env.HINTS_DIR / (fid + '.json'), env.C_DIR / (fid + '.c')
            config['adoption_guard'] = {'work': states.get(fid), 'identity': work.identity(va),
                'hints': pin(hints) if hints.exists() else None,
                'source': pin(source) if source.exists() else None}
            config['goal'] += (
                ' Promotion requires propose.resolution: reason, obligations (exact frozen '
                'primary/additional hold reasons), evidence [{va, bytes, supports}], unknowns []. '
                'Prove every named obligation from original instructions. Returning alone '
                'is insufficient. Reconcile retained source for a fresh C attempt explicitly; '
                'leave unresolved boundaries held. Independent review must verify this resolution.')
        if (config['mode'] == 'input' and config['pipeline_stage'] == 'intake'
                and (config.get('adoption_guard', {}).get('work') or {}).get('state') in work.HARD_HOLDS):
            config['pipeline_stage'] = 'evidence'
    return result


def supply_selection(rows, state, policy, preparing=(), *, excluded=()):
    """Select deficits without repeating a held function or stealing ownership."""
    jobs = list(state['jobs'].values())
    seen = defaultdict(list)
    for job in jobs:
        seen[int(job['config']['function'], 16)].append(job)
    reserved = {int(v, 16) for v in preparing}
    selected, shortage = [], {}
    remaining = max(0, policy['max_jobs'] - len(jobs) - len(reserved))
    for stage in pipeline_policy.SUPPLY_STAGES:
        active = [j for j in jobs if j['config'].get('pipeline_stage',
                    'author' if j['config']['mode'] == 'c' else 'intake') == stage
                  and not j.get('pipeline_hold')
                  and (j['state'] in ('prepared', 'running', 'needs-review', 'reviewed', 'awaiting-execution')
                       or j['state'] == 'ready' and not j.get('adoption_receipt'))]
        count = len(active)
        size = sum(j['config'].get('max_body_bytes', 0) for j in active)
        water = policy['buffers'][stage]
        if count >= water['low_functions'] and size >= water['low_bytes']:
            continue
        candidates = []
        for row in rows:
            va = int(row['va'], 16)
            if pipeline_policy.supply_stage(row) != stage or va in reserved or (row['va'], stage) in excluded:
                continue
            previous = seen.get(va, [])
            # The only automatic second job is C after the exact reviewed
            # input proposal has completed canonical adoption.
            if previous and not (stage == 'author' and all(
                    j['state'] == 'ready' and j.get('adoption_receipt')
                    and not j.get('pipeline_hold') and j['config']['mode'] == 'input' for j in previous)):
                continue
            candidates.append(row)
        # New inventory and reusable medium bodies precede known hard evidence
        # families. Family rotation avoids spending every input slot on one gap.
        candidates.sort(key=lambda r: (bool(r.get('dispatch_hold')),
            not 1024 <= r['body_bytes'] <= 8000, -r['body_bytes'], r['function']))
        groups = defaultdict(list)
        for row in candidates:
            groups[row['blocker']].append(row)
        while groups and count < water['high_functions'] and size < water['high_bytes'] and remaining:
            for family in list(groups):
                row = groups[family].pop(0)
                if not groups[family]:
                    del groups[family]
                selected.append(row)
                count += 1
                size += row['body_bytes']
                remaining -= 1
                if count >= water['high_functions'] or size >= water['high_bytes'] or not remaining:
                    break
        shortage[stage] = {'functions': count, 'body_bytes': size,
                          'low_functions': water['low_functions'], 'low_bytes': water['low_bytes'],
                          'starved': count < water['low_functions'] or size < water['low_bytes']}
    return selected, shortage


def author_buffer_full(state, policy):
    active = [j for j in state['jobs'].values() if j['config']['mode'] == 'c'
              and not j.get('pipeline_hold') and j['state'] in (
                  'prepared', 'running', 'awaiting-execution', 'needs-review', 'reviewed')]
    water = policy['buffers']['author']
    return (len(active) >= water['high_functions']
            or sum(j['config'].get('max_body_bytes', 0) for j in active) >= water['high_bytes'])


class Service:
    def __init__(self, directory, profiles, executor, *, clock=time.time,
                 snapshot=None, prepare=None, adopt=None):
        from . import pipeline_flow, pipeline_adoption, provider_jobs
        self.directory = Path(directory).resolve()
        self.profiles = profiles
        self.executor, self.clock = executor, clock
        self.snapshot = snapshot or pipeline_flow.snapshot
        self.prepare = prepare or pipeline_flow.prepare
        self.adopt = adopt or pipeline_adoption.adopt
        self.root = self.directory / 'pipeline'
        self.root.mkdir(exist_ok=True)
        self.rows = None
        self.last_scan = float('-inf')
        self.last_status = float('-inf')
        self.generation = 0
        self.shortages = {}
        self.futures = {}
        self.reserved = set()
        self.pool = ThreadPoolExecutor(max_workers=16, thread_name_prefix='cmre-pipeline')
        self.operations = {}
        for path in sorted((self.root / 'operations').glob('*.json')):
            value = json.loads(path.read_text())
            self.operations[value['id']] = value
        self._reconciled = False
        state = provider_jobs.load(self.directory)
        if not state['manifest'].get('pipeline'):
            raise ValueError('Continuous service requires a pipeline-enabled manifest')
        pipeline_policy.read(self.directory, state['manifest'])

    def __enter__(self):
        return self

    def __exit__(self, *args):
        self.pool.shutdown(wait=True)
        self._collect()

    def _record(self, value):
        self.operations[value['id']] = value
        coordination.atomic_json(self.root / 'operations' / (value['id'] + '.json'), value)

    def _launch(self, kind, function, call, *, job=None, supply_stage=None, owner=None):
        operation = f'op{len(self.operations)+1:06d}'
        value = {'id': operation, 'kind': kind, 'function': function,
                 'job': job, 'supply_stage': supply_stage, 'owner': owner,
                 'generation': self.generation,
                 'state': 'pending', 'started_at': self.clock()}
        self._record(value)
        output = self.root / operation
        self.futures[self.pool.submit(call, output)] = value
        return value

    def _collect(self):
        from . import provider_campaign, provider_jobs, provider_runtime
        for future, operation in list(self.futures.items()):
            if not future.done():
                continue
            del self.futures[future]
            value = dict(operation, finished_at=self.clock())
            try:
                result = future.result()
                if operation['kind'] == 'scan':
                    if operation.get('generation') == self.generation:
                        self.rows = result
                        self.last_scan = self.clock()
                    else:
                        value['superseded_by_adoption'] = True
                    value['inventory'] = dict(Counter(r['stage'] for r in result))
                elif operation['kind'] == 'prepare':
                    manifest = json.loads(Path(result['manifest']).read_text())
                    value['supply'] = str(Path(result['manifest']).parent / 'supply.json')
                    admitted = []
                    for definition in manifest['jobs']:
                        definition['id'] = definition['id'] + '-' + operation['id']
                        # Enqueue only after all immutable evidence and candidate
                        # seeds exist. Each appended batch keeps its own marker.
                        prepared = provider_runtime.prepare_manifest(self.directory,
                            dict(manifest, jobs=[definition]))
                        seed_root = self.directory / 'seed-batches' / operation['id']
                        seed_root.mkdir(parents=True, exist_ok=True)
                        provider_runtime.seed_candidates(self.directory, prepared,
                                                         marker=seed_root / 'seed-candidates.json')
                        job = provider_campaign.enqueue(self.directory, prepared['jobs'][0])
                        admitted.append(job['id'])
                    value['jobs'] = admitted
                    if not admitted:
                        raise ValueError('No packet passed preparation; inspect retained packet-holds.json')
                elif operation['kind'] == 'adoption':
                    self.generation += 1
                    self.rows = None
                    self.last_scan = float('-inf')
                value['state'] = 'finished'
            except Exception as error:
                value.update(state='held', reason=f'{type(error).__name__}: {error}')
                if operation['job']:
                    state = provider_jobs.load(self.directory)
                    job = state['jobs'][operation['job']]
                    provider_jobs.update(self.directory, job['id'],
                        {'pipeline_hold': {'operation': operation['id'], 'reason': value['reason']}},
                        expected_revision=job['revision'])
            self._record(value)
            self.reserved.discard(operation['function'])

    def __call__(self, state, deadline):
        from . import provider_jobs, provider_runtime
        # The campaign owns run.lock before invoking this hook. Construction
        # and a duplicate run attempt must never relabel a live operation.
        if not self._reconciled:
            for operation in list(self.operations.values()):
                failure = None
                if operation['state'] == 'pending':
                    failure = 'Interrupted pipeline operation; inspect retained inputs and side effects before explicit reconciliation'
                elif operation['state'] == 'finished' and operation['kind'] == 'prepare':
                    try:
                        definitions = [{'id': name, 'config': state['jobs'][name]['config']}
                                       for name in operation['jobs']]
                        provider_runtime.verify_seeded_candidates(self.directory, {'jobs': definitions},
                            marker=self.directory / 'seed-batches' / operation['id'] / 'seed-candidates.json')
                    except (OSError, ValueError, KeyError) as error:
                        failure = f'Appended candidate initialization is incomplete: {error}'
                if failure is None:
                    continue
                operation.update(state='held', reason=failure)
                self._record(operation)
                for job in state['jobs'].values():
                    if job['id'] == operation['job'] or job['id'].endswith('-' + operation['id']):
                        changes = {'pipeline_hold': {'operation': operation['id'], 'reason': operation['reason']}}
                        if job['state'] not in provider_jobs.TERMINAL_STATES:
                            changes['state'] = 'held'
                        provider_jobs.update(self.directory, job['id'], changes,
                                             expected_revision=job['revision'])
            self._reconciled = True
        self._collect()
        state = provider_jobs.load(self.directory)
        current = pipeline_policy.read(self.directory, state['manifest'])
        busy = Counter(v['kind'] for v in self.futures.values())
        active_jobs = {v['job'] for v in self.futures.values()}
        if self.clock() < deadline:
            for job in state['jobs'].values():
                if (busy['adoption'] >= current['workers']['adoption']
                        or author_buffer_full(state, current)):
                    break
                if (job['state'] != 'ready' or job['config']['mode'] != 'input'
                        or job.get('adoption_receipt') or job.get('pipeline_hold')
                        or job['id'] in active_jobs):
                    continue
                # Retained failed/interrupted operations never restart just
                # because the campaign process was resumed.
                if any(v['job'] == job['id'] for v in self.operations.values()):
                    continue
                self._launch('adoption', job['config']['function'],
                    lambda output, job=job: self.adopt(self.directory, job['id'], self.executor,
                        output, deadline=deadline, clock=self.clock), job=job['id'])
                busy['adoption'] += 1
                self.last_scan = float('-inf')
            prepare_capacity = current['workers']['prepare']
            if (prepare_capacity and not busy['scan'] and not busy['prepare']
                    and self.clock() - self.last_scan >= current['scan_seconds']):
                self._launch('scan', None, lambda output: self.snapshot())
                busy['scan'] += 1
            if self.rows is not None and not busy['scan']:
                excluded = {(v['function'], v.get('supply_stage')) for v in self.operations.values()
                            if v['kind'] == 'prepare'}
                selected, self.shortages = supply_selection(self.rows, state, current, self.reserved,
                                                           excluded=excluded)
                for row in selected:
                    if busy['prepare'] >= prepare_capacity:
                        break
                    # A terminal packet-preparation failure remains retained;
                    # context oversize never causes a paid dispatch or loop.
                    if any(v['function'] == row['va'] and v['kind'] == 'prepare'
                           and v.get('supply_stage') == pipeline_policy.supply_stage(row)
                           for v in self.operations.values()):
                        continue
                    self.reserved.add(row['va'])
                    lane = pipeline_policy.supply_stage(row)
                    owners = current['owners'][lane]
                    assigned = Counter(j['config']['owner'] for j in state['jobs'].values()
                        if j['config'].get('pipeline_stage') == lane
                        and j['state'] in ('prepared', 'running', 'needs-review', 'reviewed', 'awaiting-execution'))
                    assigned.update(v['owner'] for v in self.futures.values()
                                    if v['kind'] == 'prepare' and v['supply_stage'] == lane)
                    owner = min(owners, key=lambda role: assigned[role])
                    self._launch('prepare', row['va'], lambda output, row=row, owner=owner: self.prepare(
                        output, [row], self.profiles,
                        slots=max(state['manifest']['role_capacity'].values()),
                        duration_seconds=state['manifest']['duration_seconds'],
                        limit=1, buffer_bytes=10000000, scalable=True, owner_override=owner),
                        supply_stage=lane, owner=owner)
                    busy['prepare'] += 1
        if self.clock() - self.last_status >= 2 or not self.futures:
            coordination.atomic_json(self.root / 'status.json', self.projection(state, current))
            self.last_status = self.clock()
        return {'pending': bool(self.futures)}

    def projection(self, state, current):
        counts = Counter()
        sizes = Counter()
        for job in state['jobs'].values():
            stage = ('adoption-held' if job.get('pipeline_hold') else
                     'adopted' if job.get('adoption_receipt') else
                     'adoption' if job['state'] == 'ready' and job['config']['mode'] == 'input' else
                     job['state'] if job['state'] in ('held', 'accepted', 'exhausted', 'cancelled') else
                     pipeline_policy.stage(job))
            counts[stage] += 1
            sizes[stage] += job['config'].get('max_body_bytes', 0)
        families = defaultdict(lambda: {'functions': 0, 'body_bytes': 0, 'retry_when': ''})
        for row in self.rows or []:
            if row['stage'] != 'capability':
                continue
            family = families[row['blocker']]
            family['functions'] += 1
            family['body_bytes'] += row['body_bytes']
            family['retry_when'] = row.get('retry_when', '')
        return {'schema': 'cmre-pipeline-service/1', 'queues': dict(counts),
                'queue_candidate_bytes': dict(sizes),
                'adoption_backpressure': 'author-buffer-full' if author_buffer_full(state, current) else None,
                'active_operations': dict(Counter(v['kind'] for v in self.futures.values())),
                'operations': dict(Counter(v['state'] for v in self.operations.values())),
                'shortages': self.shortages, 'policy': current,
                'capability_families': dict(families), 'snapshot_at': self.clock()}
