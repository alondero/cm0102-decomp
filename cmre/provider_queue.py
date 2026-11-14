"""Cancel unpaid C preparation at a drained, explicitly bounded run boundary.

Candidates and every event remain retained. This is an operator scope change,
not a retry: cancelled jobs do not reopen or consume inference or C attempts.
"""
from contextlib import ExitStack
import hashlib
import json
from pathlib import Path
import time

from . import coordination, provider_jobs

CONTROLS_PRECONDITION = 'execute error: ValueError: Source-site controls require a passing current-source full test'


def retry_precondition(directory, job_id, *, reason, apply=False, clock=time.time):
    """Explicitly reopen one proven, side-effect-free controls ordering refusal."""
    from . import env, provider_campaign
    if not isinstance(reason, str) or not 1 <= len(reason.strip()) <= 1000:
        raise ValueError('Reconciliation requires a concrete reason')
    directory = Path(directory).resolve()
    with ExitStack() as stack:
        if apply:
            stack.enter_context(coordination.writer())
            stack.enter_context(coordination.lock(directory / 'run.lock'))
        state = provider_jobs.load(directory)
        timing = provider_campaign._validate_run(
            json.loads((directory / 'run.json').read_text(encoding='utf-8')), state)
        if clock() >= timing['deadline']:
            raise ValueError('The original run deadline has expired')
        job = state['jobs'][job_id]
        if (job['state'] != 'held' or job['config']['mode'] != 'c' or job.get('pipeline_hold')
                or job.get('hold_reason') != CONTROLS_PRECONDITION
                or (job.get('escalation') or {}).get('adapter') != 'execute'
                or job.get('round_actions', 0) >= provider_campaign.MAX_ACTIONS_PER_ROUND):
            raise ValueError('Only the exact controls-before-test precondition is reconcilable')
        runtime_pin = state['identity'].get('controller', {}).get('cmre/provider_runtime.py')
        if runtime_pin != env.digest(env.ROOT / 'cmre/provider_runtime.py'):
            raise ValueError('Pinned executor changed; side-effect-free refusal is unproven')
        records = list(provider_campaign._verified_records(directory, job))
        if not records:
            raise ValueError('No retained failed action')
        ref, record = records[-1]
        if (record.get('kind') != 'control-plan' or record.get('ok') is not False
                or record.get('error') != CONTROLS_PRECONDITION or record.get('response') is not None):
            raise ValueError('Latest failed record does not prove the precondition refusal')
        root = (directory / 'jobs' / job_id).resolve()
        source = Path(job['config']['candidate']).resolve(strict=True)
        output = (directory / record['output_path']).resolve()
        if (not source.is_relative_to(root) or not output.is_relative_to(root) or output.exists()
                or hashlib.sha256(source.read_bytes()).hexdigest() !=
                (record.get('actor_response') or {}).get('source', {}).get('sha256')):
            raise ValueError('Source changed or execution output exists; inspect uncertain side effects')
        report = {'schema': 'cmre-precondition-reconciliation/1', 'job': job_id, 'apply': apply,
                  'reason': reason, 'record_hash': ref['hash'], 'deadline': timing['deadline'],
                  'attempts_preserved': job.get('attempts', 0), 'action_count_preserved': job.get('action_count', 0)}
        if apply:
            receipt = directory / 'scope-changes' / (str(time.time_ns()) + '.json')
            report.update(receipt=str(receipt), state='running')
            coordination.atomic_json(receipt, report)
            feedback = list(job.get('feedback') or [])
            feedback.append('Explicit operator reconciliation: the prior controls request was rejected '
                'before output creation or gate execution. Source and all counters are unchanged. '
                'Request the normal full C test before controls; repair its first discrepancy if needed. '
                'This resolves only request ordering, not source fidelity or sensitivity obligations.')
            try:
                provider_jobs.update(directory, job_id, {'state': 'prepared', 'phase': 'owner',
                    'retry_reason': reason, 'hold_reason': None, 'escalation': None, 'checkpoint': None,
                    'pending_step': None, 'next_stage': None, 'feedback': feedback},
                    expected_revision=job['revision'])
            except Exception as error:
                report.update(state='failed', error=f'{type(error).__name__}: {error}')
                coordination.atomic_json(receipt, report)
                raise
            report['state'] = 'finished'
            coordination.atomic_json(receipt, report)
        return report


def cancel_unstarted(directory, *, reason, apply=False):
    if not isinstance(reason, str) or not 1 <= len(reason.strip()) <= 1000:
        raise ValueError('Cancellation requires a concrete reason of at most 1000 characters')
    directory = Path(directory).resolve()
    with ExitStack() as stack:
        if apply:
            stack.enter_context(coordination.writer())
            stack.enter_context(coordination.lock(directory / 'run.lock'))
        state = provider_jobs.load(directory)
        selected = []
        for job in state['jobs'].values():
            if (job['state'] != 'prepared' or job['config']['mode'] != 'c' or job.get('pipeline_hold')
                    or any(job.get(key) for key in ('attempts', 'action_count', 'probes', 'reviews',
                                                    'checkpoint', 'pending_step', 'results'))):
                continue
            root = (directory / 'jobs' / job['id']).resolve()
            if not root.is_relative_to(directory / 'jobs'):
                raise ValueError('Job directory escapes its run')
            if (root / 'task-lease.json').exists():
                continue
            candidate = Path(job['config'].get('candidate', root / 'candidate.c')).resolve()
            pin = None
            if candidate.is_relative_to(root) and candidate.is_file():
                pin = {'path': str(candidate),
                       'sha256': hashlib.sha256(candidate.read_bytes()).hexdigest()}
            selected.append({'job': job['id'], 'function': job['config']['function'],
                             'revision': job['revision'], 'candidate': pin})
        report = {'schema': 'cmre-cancel-unstarted/1', 'apply': apply,
                  'directory': str(directory), 'genesis_hash': state['genesis_hash'],
                  'identity_hash': state['identity_hash'], 'store_revision': state['revision'],
                  'reason': reason, 'selected': selected, 'cancelled': [],
                  'retention': 'All packets, source candidates, work records and lifetime attempts remain intact'}
        if not apply:
            return report
        receipt_root = directory / 'scope-changes'
        receipt_root.mkdir(exist_ok=True)
        receipt = receipt_root / (str(time.time_ns()) + '.json')
        report.update(receipt=str(receipt), state='running')
        coordination.atomic_json(receipt, report)
        try:
            for row in selected:
                provider_jobs.update(directory, row['job'], {
                    'state': 'cancelled', 'cancellation': {'reason': reason, 'receipt': str(receipt)}},
                    expected_revision=row['revision'])
                report['cancelled'].append(row['job'])
                coordination.atomic_json(receipt, report)
        except Exception as error:
            report.update(state='failed', error=f'{type(error).__name__}: {error}')
            coordination.atomic_json(receipt, report)
            raise
        report['state'] = 'finished'
        coordination.atomic_json(receipt, report)
        return report


def command(args):
    if args.action == 'retry-precondition':
        print(json.dumps(retry_precondition(args.directory, args.job, reason=args.reason, apply=args.apply), indent=2))
        return 0
    report = cancel_unstarted(args.directory, reason=args.reason, apply=args.apply)
    print(json.dumps({key: value for key, value in report.items() if key not in ('selected', 'cancelled')}
                     | {'selected_jobs': len(report['selected']), 'cancelled_jobs': len(report['cancelled'])},
                     indent=2))
    return 0
