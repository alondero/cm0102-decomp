"""Live stage limits bounded by a campaign's frozen physical capacity."""
from __future__ import annotations

import copy
import hashlib
import json
from pathlib import Path

from . import coordination

SCHEMA = 'cmre-pipeline-policy/1'
PROVIDER_STAGES = ('intake', 'evidence', 'repair', 'reconcile', 'author', 'controls', 'review')
MACHINE_STAGES = ('probe', 'test', 'build', 'sensitivity', 'package', 'publish')
STAGES = (*PROVIDER_STAGES, *MACHINE_STAGES, 'prepare', 'adoption')
SUPPLY_STAGES = ('author', 'intake', 'evidence', 'repair', 'reconcile')


def supply_stage(row):
    return 'evidence' if row['stage'] == 'intake' and row.get('dispatch_hold') else row['stage']


def defaults(manifest):
    capacity = manifest['concurrency']
    value = {'schema': SCHEMA, 'workers': {
        **{stage: capacity for stage in PROVIDER_STAGES},
        'prepare': 2, 'adoption': 2, 'publish': 1,
        'probe': 4, 'test': 4, 'build': 2, 'sensitivity': 2, 'package': 2},
        'buffers': {stage: {'low_functions': 8, 'high_functions': 24,
                           'low_bytes': 16000, 'high_bytes': 64000}
                    for stage in SUPPLY_STAGES},
        'owners': {stage: ['minimax'] if stage == 'author' else ['deepseek', 'minimax']
                   for stage in SUPPLY_STAGES},
        'execution_workers': 8,
        'max_jobs': 512, 'scan_seconds': 30, 'input_ready_limit': capacity * 2}
    value['workers']['evidence'] = min(2, capacity)
    value['buffers']['evidence'] = {'low_functions': 2, 'high_functions': 8,
                                   'low_bytes': 4000, 'high_bytes': 32000}
    return value


def validate(policy, manifest):
    expected = {'schema', 'workers', 'buffers', 'owners', 'execution_workers',
                'max_jobs', 'scan_seconds', 'input_ready_limit'}
    if not isinstance(policy, dict) or set(policy) != expected or policy['schema'] != SCHEMA:
        raise ValueError('Invalid pipeline policy schema or fields')
    if not isinstance(policy['workers'], dict) or set(policy['workers']) != set(STAGES):
        raise ValueError('Policy must name every stage capacity')
    for stage, value in policy['workers'].items():
        maximum = (manifest['concurrency'] if stage in PROVIDER_STAGES else
                   1 if stage == 'publish' else 8)
        if type(value) is not int or not 0 <= value <= maximum:
            raise ValueError(f'{stage} workers must be 0..{maximum}')
    if not isinstance(policy['buffers'], dict) or set(policy['buffers']) != set(SUPPLY_STAGES):
        raise ValueError('Policy must name every supply buffer')
    if (not isinstance(policy['owners'], dict) or set(policy['owners']) != set(SUPPLY_STAGES)
            or any(not isinstance(v, list) or not 1 <= len(v) <= 2
                   or any(role not in ('deepseek', 'minimax') for role in v)
                   or len(set(v)) != len(v) for v in policy['owners'].values())):
        raise ValueError('Each supply stage needs one or two distinct provider roles')
    for values in policy['buffers'].values():
        if (not isinstance(values, dict) or set(values) != {
                'low_functions', 'high_functions', 'low_bytes', 'high_bytes'}
                or any(type(v) is not int or v < 1 for v in values.values())
                or values['low_functions'] > values['high_functions']
                or values['low_bytes'] > values['high_bytes']
                or values['high_functions'] > 512 or values['high_bytes'] > 10000000):
            raise ValueError('Invalid low/high buffer watermarks')
    for key, low, high in [('max_jobs', 1, 4096), ('scan_seconds', 1, 3600), ('execution_workers', 1, 16),
                           ('input_ready_limit', 1, 512)]:
        if type(policy[key]) is not int or not low <= policy[key] <= high:
            raise ValueError(f'{key} must be {low}..{high}')
    if policy['max_jobs'] < len(manifest.get('jobs', [])):
        raise ValueError('max_jobs cannot be smaller than the initial frozen queue')
    return copy.deepcopy(policy)


def write(directory, policy, manifest):
    directory = Path(directory)
    policy = validate(policy, manifest)
    with coordination.lock(directory / 'policy.lock'):
        return _write_locked(directory, policy, manifest)


def edit(directory, manifest, change):
    """Apply one edit to the current revision under the policy writer lock."""
    directory = Path(directory)
    with coordination.lock(directory / 'policy.lock'):
        value = read(directory, manifest)
        change(value)
        return _write_locked(directory, validate(value, manifest), manifest)


def _write_locked(directory, policy, manifest):
    pointer = directory / 'pipeline-policy.json'
    old = json.loads(pointer.read_text()) if pointer.exists() else None
    if old:
        read(directory, manifest)
    revision = old['revision'] + 1 if old else 1
    envelope = {'revision': revision, 'previous': old, 'policy': policy}
    path = directory / 'policies' / f'{revision:08d}.json'
    coordination.atomic_json(path, envelope)
    pin = {'revision': revision, 'path': str(path.resolve()),
           'sha256': hashlib.sha256(path.read_bytes()).hexdigest()}
    coordination.atomic_json(pointer, pin)
    return {'revision': revision, 'policy': policy}


def read(directory, manifest):
    directory = Path(directory).resolve()
    pin = json.loads((directory / 'pipeline-policy.json').read_text())
    path = Path(pin['path']).resolve(strict=True)
    if (path != directory / 'policies' / f"{pin['revision']:08d}.json"
            or hashlib.sha256(path.read_bytes()).hexdigest() != pin['sha256']):
        raise ValueError('Pipeline policy pin changed')
    envelope = json.loads(path.read_text())
    if envelope['revision'] != pin['revision']:
        raise ValueError('Pipeline policy revision differs')
    return validate(envelope['policy'], manifest)


def stage(job):
    if job.get('state') == 'awaiting-execution' or job.get('state') == 'running' and job.get('phase') == 'execute':
        kind = job['pending_step']['action']['kind']
        return {'test': 'test', 'build': 'build', 'controls': 'sensitivity',
                'control-plan': 'sensitivity', 'propose': 'package', 'bundle': 'package'}.get(kind, 'probe')
    if job.get('state') == 'reviewed' or job.get('state') == 'running' and job.get('phase') == 'publish':
        return 'publish'
    if job.get('state') == 'needs-review' or job.get('phase') == 'review' and job.get('state') == 'running':
        return 'review'
    if job.get('next_stage') == 'controls':
        return 'controls'
    return job.get('config', {}).get('pipeline_stage') or (
        'author' if job.get('config', {}).get('mode') == 'c' else 'intake')
