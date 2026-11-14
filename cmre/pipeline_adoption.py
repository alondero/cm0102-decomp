"""Reviewed, pinned input promotion through the unchanged native gate."""
from __future__ import annotations

import copy
import json
import re
import time
from pathlib import Path

from . import coordination


def obligations(record):
    if not record:
        return []
    rows = [record, *record.get('additional_holds', [])]
    return [row['reason'] for row in rows if row.get('state') in ('evidence-needed', 'harness-blocker')]


def validate_resolution(config, resolution):
    """Check pins and claim structure. Independent review decides semantics."""
    from . import image
    guard = config.get('adoption_guard')
    if not isinstance(guard, dict):
        raise ValueError('No frozen adoption guard; explicit coordinator adoption is required')
    record = guard.get('work') or {}
    if any(row.get('state') == 'harness-blocker' for row in
           [record, *record.get('additional_holds', [])]):
        raise ValueError('Harness capability holds require a separate reviewed release')
    if (not isinstance(resolution, dict) or set(resolution) != {
            'reason', 'obligations', 'evidence', 'unknowns'}
            or not isinstance(resolution['reason'], str)
            or not 1 <= len(resolution['reason'].strip()) <= 4000
            or resolution['unknowns'] != []
            or resolution['obligations'] != obligations(record)):
        raise ValueError('Promotion requires explicit resolution of every frozen obligation and no unknowns')
    evidence = resolution['evidence']
    if not isinstance(evidence, list) or not 1 <= len(evidence) <= 64:
        raise ValueError('Resolution needs 1..64 pinned original-byte evidence entries')
    for row in evidence:
        if (not isinstance(row, dict) or set(row) != {'va', 'bytes', 'supports'}
                or not isinstance(row['va'], str) or not re.fullmatch('0x[0-9a-fA-F]{8}', row['va'])
                or not isinstance(row['bytes'], str) or not re.fullmatch('(?:[0-9a-fA-F]{2}){1,256}', row['bytes'])
                or not isinstance(row['supports'], str) or not 1 <= len(row['supports'].strip()) <= 2000):
            raise ValueError('Malformed original-byte resolution evidence')
        expected = bytes.fromhex(row['bytes'])
        if image.primary().read(int(row['va'], 16), len(expected)) != expected:
            raise ValueError('Resolution evidence differs from the pinned original bytes')


def merge_fixtures(hints, fixtures, *, c_only=False):
    """Append observed inputs; preserve both tier prefixes and every setting."""
    result = copy.deepcopy(hints)
    result.pop('auto', None)
    keys = [('fixtures',)]
    if c_only:
        overrides = result.setdefault('c_hints', {})
        if not isinstance(overrides, dict):
            raise ValueError('Existing C input overrides are not an object')
        overrides.setdefault('fixtures', copy.deepcopy(result.get('fixtures', [])))
        keys = [('c_hints', 'fixtures')]
    elif isinstance(result.get('c_hints'), dict) and 'fixtures' in result['c_hints']:
        keys.append(('c_hints', 'fixtures'))
    for keyspec in keys:
        node = result
        for key in keyspec[:-1]:
            node = node[key]
        previous = node.setdefault(keyspec[-1], [])
        if not isinstance(previous, list):
            raise ValueError('Existing fixture prefix is not a list')
        for fixture in fixtures:
            if fixture not in previous:
                previous.append(copy.deepcopy(fixture))
    return result


def _guard_current(va, guard):
    from . import env, ledger, tasks, work
    from .pipeline_flow import pin
    if work.identity(va) != guard['identity']:
        raise ValueError('Function inputs changed after the proposal was prepared')
    if work.read()['functions'].get(f'fn_{va:08x}') != guard['work']:
        raise ValueError('Work obligations changed after the proposal was prepared')
    hints = env.HINTS_DIR / f'fn_{va:08x}.json'
    source = env.C_DIR / f'fn_{va:08x}.c'
    if ((pin(hints) if hints.exists() else None) != guard['hints']
            or (pin(source) if source.exists() else None) != guard['source']):
        raise ValueError('Canonical source or hints changed since preparation')
    path = ledger.record_path(va)
    if path.exists() and json.loads(path.read_text()).get('c'):
        raise ValueError('Function already has accepted C; input promotion would invalidate it')
    if f'fn_{va:08x}' in work.claims():
        raise ValueError('Another worker has a live shared claim')
    for task in tasks.status():
        if int(task['va'], 16) != va:
            continue
        if (task['status'] in ('submitted', 'publishing')
                or any(a.get('status') == 'testing' for a in task.get('attempts', []))
                or (task.get('lease') or {}).get('expires_at', 0) > time.time()):
            raise ValueError('Function has a live or submitted C task; reconcile ownership first')


def adopt(directory, job_id, executor, output, *, deadline, clock=time.time):
    """One durable adoption attempt. Interrupted attempts require reconciliation.

    Native acceptance and C publication share the executor's serialization lock.
    Failures retain the proposed/previous hints and full gate response; a failed
    native gate restores the previous hints and never clears a work hold.
    """
    from . import env, ledger, provider_jobs, recover, work
    from .pipeline_flow import acknowledge, pin
    from .provider_runtime import _json, _verified
    directory, output = Path(directory).resolve(), Path(output).resolve()
    if not output.is_relative_to(directory):
        raise ValueError('Adoption evidence escapes its campaign')
    state = provider_jobs.load(directory)
    job = state['jobs'][job_id]
    config = job['config']
    if job['state'] != 'ready' or config['mode'] != 'input' or job.get('adoption_receipt'):
        raise ValueError('Only unadopted, independently reviewed input jobs may promote')
    if clock() >= deadline:
        raise ValueError('Run deadline reached before adoption')
    root = directory / 'jobs' / job_id
    bundle = job['bundle']
    payload = _json(_verified(bundle['pin'], root))
    if (payload.get('adoption_guard') != config.get('adoption_guard')
            or not {'named-input-obligation-resolution', 'fixture-prefix-preservation'}
            <= set(bundle['required_checks'])):
        raise ValueError('Bundle lacks reviewed promotion requirements')
    resolution = payload.get('resolution')
    validate_resolution(config, resolution)
    output.mkdir(parents=True, exist_ok=False)
    receipt_path = output / 'receipt.json'
    receipt = {'schema': 'cmre-pipeline-adoption-attempt/1', 'state': 'pending',
               'job': job_id, 'function': config['function'], 'bundle_sha256': bundle['sha256'],
               'deadline': deadline}
    coordination.atomic_json(receipt_path, receipt)
    try:
        # Reuse the normal finalizer to recheck frozen artifacts, model
        # independence, exact review checks and the execution identity.
        executor._finalize(job, config, root, {'kind': 'finalize', 'bundle': bundle,
            'review': job['review'], 'review_model': job['review_model']}, output / 'review-check')
        fixtures = []
        for entry in payload.get('fixtures', []):
            fixture = _json(_verified(entry['pin'], root))
            if fixture != entry['data']:
                raise ValueError('Reviewed fixture data differs from its retained file')
            fixtures.append(fixture)
        if not fixtures:
            raise ValueError('No complete observed fixtures to adopt')
        va = int(config['function'], 16)
        hint_path = env.HINTS_DIR / f'fn_{va:08x}.json'
        with executor._publication_serial, coordination.writer(), work.attempt_fence(va):
            _guard_current(va, config['adoption_guard'])
            if clock() >= deadline:
                raise ValueError('Run deadline reached before canonical mutation')
            previous = hint_path.read_bytes() if hint_path.exists() else None
            if previous is not None:
                coordination.atomic_bytes(output / 'previous-hints.json', previous)
            existing_native = ledger.record_path(va).exists()
            proposed = merge_fixtures(json.loads(previous) if previous else {}, fixtures,
                                      c_only=existing_native)
            coordination.atomic_json(output / 'proposed-hints.json', proposed)
            receipt['proposed_hints'] = pin(output / 'proposed-hints.json')
            coordination.atomic_json(receipt_path, receipt)
            coordination.atomic_json(hint_path, proposed)
            try:
                # Refresh an existing lift rather than trusting its old fixture
                # identity. Both original acceptance executions remain required.
                ok, result = recover.accept(va, refresh=existing_native)
                coordination.atomic_json(output / 'native-gate.json', result)
                if not ok:
                    raise ValueError(f"Native acceptance failed: {result.get('status', 'unknown')}")
            except BaseException:
                if previous is None:
                    hint_path.unlink(missing_ok=True)
                else:
                    coordination.atomic_bytes(hint_path, previous)
                raise
            # Exact expected work state prevents this resolution from erasing a
            # hold recorded while the native gate was executing.
            work.update(va, 'ready', f"Reviewed input adoption {bundle['sha256']}: {resolution['reason']}",
                        expected_previous=config['adoption_guard']['work'],
                        _remaining_holds=[], _resolution=str(receipt_path))
            adopted = {'schema': 'cmre-pipeline-adoption/1', 'function': config['function'],
                       'bundle_sha256': bundle['sha256'], 'canonical_hints': pin(hint_path),
                       'native_gate': pin(ledger.record_path(va))}
            coordination.atomic_json(output / 'adopted.json', adopted)
            result = acknowledge(directory, job_id, output / 'adopted.json')
        receipt.update(state='finished', result=result)
        coordination.atomic_json(receipt_path, receipt)
        return receipt
    except BaseException as error:
        receipt.update(state='held', reason=f'{type(error).__name__}: {error}')
        coordination.atomic_json(receipt_path, receipt)
        raise
