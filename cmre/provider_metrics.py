"""Read-only, bounded diagnostics from retained pipeline worker receipts.

These measurements describe work and usage, never source approval. Overlapping
worker seconds are reported separately from elapsed wall time. No corpus, source
text or raw provider message is included in the projection.
"""
from collections import Counter, defaultdict
import json
from pathlib import Path
import time

from . import provider_campaign, provider_jobs

TOKEN_FIELDS = ('input_tokens', 'cache_read_tokens', 'cache_write_tokens',
                'output_tokens', 'reasoning_tokens')


def report(directory, *, clock=time.time):
    directory = Path(directory).resolve()
    state = provider_jobs.load(directory)
    now = clock()
    timing_path = directory / 'run.json'
    timing = (provider_campaign._validate_run(
        json.loads(timing_path.read_text(encoding='utf-8')), state) if timing_path.exists() else {})
    started = timing.get('started_at')
    elapsed = max(0, now - started) if isinstance(started, (int, float)) else None
    jobs = list(state['jobs'].values())
    states = Counter(job['state'] for job in jobs)
    accepted = [job for job in jobs if job['state'] == 'accepted' and job['config']['mode'] == 'c']
    accepted_bytes = 0
    diagnostics = []
    if accepted:
        from . import inventory
        try:
            functions = inventory.load().functions
            accepted_bytes = sum(functions[int(job['config']['function'], 16)].body_bytes
                                 for job in accepted)
        except (OSError, ValueError, KeyError):
            accepted_bytes = None
            diagnostics.append('Accepted body bytes unavailable from the local inventory')

    groups = defaultdict(lambda: {'stages': 0, 'running': 0, 'worker_seconds': 0, 'observed_models': set(),
                                  'calls': 0, 'known_tokens': Counter(),
                                  'unknown_token_fields': Counter()})
    seen_calls = {}
    stages = []
    unreadable = 0
    for job in jobs:
        for phase in ('smoke', 'work'):
            for path in (directory / 'jobs' / job['id'] / 'rounds').glob(f'*/*/pair/{phase}/receipt.json'):
                try:
                    receipt = json.loads(path.read_text(encoding='utf-8'))
                    if receipt.get('schema') != 'cmre-provider-worker/1':
                        raise ValueError('Unexpected worker receipt schema')
                except (OSError, ValueError):
                    unreadable += 1
                    continue
                model = receipt.get('model_requested') or 'unknown'
                group = groups[(model, phase)]
                group['observed_models'].update(receipt.get('models_observed') or [])
                group['stages'] += 1
                running = receipt.get('state') == 'running'
                group['running'] += int(running)
                duration = receipt.get('elapsed_seconds')
                if running and isinstance(receipt.get('started'), (int, float)):
                    duration = max(0, now - receipt['started'])
                if isinstance(duration, (int, float)) and duration >= 0:
                    group['worker_seconds'] += duration
                    stages.append({'job': job['id'], 'phase': phase, 'requested_model': model,
                                   'state': receipt.get('state'), 'seconds': round(duration, 3)})
                calls = receipt.get('calls')
                if not isinstance(calls, list) or not calls:
                    group['unknown_token_fields'].update(TOKEN_FIELDS)
                    continue
                for call in calls:
                    if not isinstance(call, dict):
                        group['unknown_token_fields'].update(TOKEN_FIELDS)
                        continue
                    call_id = (call.get('model'), call.get('call_id'))
                    if not all(isinstance(value, str) and value for value in call_id):
                        group['unknown_token_fields'].update(TOKEN_FIELDS)
                        continue
                    if call_id in seen_calls:
                        previous, previous_group = seen_calls[call_id]
                        if previous != call:
                            group['unknown_token_fields'].update(TOKEN_FIELDS)
                            if previous_group is not group:
                                previous_group['unknown_token_fields'].update(TOKEN_FIELDS)
                            diagnostics.append('Conflicting retained usage for one provider call')
                        continue
                    seen_calls[call_id] = (call, group)
                    group['calls'] += 1
                    for field in TOKEN_FIELDS:
                        count = call.get(field)
                        if (call.get('usage_observed') is True and type(count) is int and count >= 0):
                            group['known_tokens'][field] += count
                        else:
                            group['unknown_token_fields'][field] += 1

    workers = []
    for (model, phase), group in sorted(groups.items()):
        group['worker_seconds'] = round(group['worker_seconds'], 3)
        group['tokens'] = {field: None if group['unknown_token_fields'][field]
                           else group['known_tokens'][field] for field in TOKEN_FIELDS}
        group['known_tokens'] = dict(group['known_tokens'])
        group['unknown_token_fields'] = dict(group['unknown_token_fields'])
        group['observed_models'] = sorted(group['observed_models'])
        workers.append(dict(requested_model=model, phase=phase, **group))
    if unreadable:
        diagnostics.append(f'{unreadable} worker receipts unreadable; usage and stage totals are incomplete')
    held = [job for job in jobs if job['state'] == 'held' or job.get('pipeline_hold')]
    held.sort(key=lambda job: (job.get('history') or [{}])[-1].get('sequence', 0), reverse=True)
    hold_categories = Counter()
    for job in held:
        reason = str(job.get('hold_reason') or '')
        category = next((kind for kind in ('actor', 'controller', 'execute')
                         if reason.startswith(kind + ' error:')), 'other')
        hold_categories['service' if job.get('pipeline_hold') else category] += 1
    return {'schema': 'cmre-provider-metrics/1', 'snapshot_at': now,
            'elapsed_seconds': round(elapsed, 3) if elapsed is not None else None,
            'deadline': timing.get('deadline'), 'job_states': dict(states),
            'modes': {mode: dict(Counter(job['state'] for job in jobs if job['config']['mode'] == mode))
                      for mode in ('c', 'input')},
            'c_test_actions': dict(Counter('passed' if ref.get('ok') else 'failed'
                for job in jobs if job['config']['mode'] == 'c'
                for ref in job.get('results', []) if ref.get('kind') == 'test')),
            'accepted_c_functions': len(accepted), 'accepted_c_body_bytes': accepted_bytes,
            'accepted_c_bytes_per_wall_hour': round(accepted_bytes * 3600 / elapsed, 3)
                if accepted_bytes is not None and elapsed else None,
            'adopted_inputs': sum(bool(job.get('adoption_receipt')) for job in jobs),
            'workers': workers, 'slowest_stages': sorted(stages, key=lambda row: -row['seconds'])[:10],
            'hold_categories': dict(hold_categories),
            'holds': [{'job': job['id'], 'reason': str(job.get('hold_reason') or
                       job.get('pipeline_hold', {}).get('reason', ''))[:300]}
                      for job in held[:10]],
            'diagnostics': diagnostics[:10],
            'note': 'Worker seconds overlap. Usage excludes coordinator calls; missing usage remains unknown. '
                    'Counts do not establish composed execution or live integration.'}
