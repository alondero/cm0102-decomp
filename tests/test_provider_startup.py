"""Regression witnesses for retained candidates and prior submissions."""
import json
from pathlib import Path

import pytest

from cmre import env, provider_campaign, provider_jobs, provider_runtime as runtime, work, worktrees


def seeded_manifest(tmp_path, monkeypatch):
    analysis = tmp_path / 'analysis'
    canonical = tmp_path / 'c'
    canonical.mkdir()
    monkeypatch.setattr(env, 'ANALYSIS', analysis)
    monkeypatch.setattr(env, 'C_DIR', canonical)
    monkeypatch.setattr(env, 'ROOT', tmp_path)
    monkeypatch.setattr(work, 'read', lambda: {'functions': {'fn_00401000': {'state': 'ready'}}})
    header = tmp_path / 'recovery/include/cmre.h'
    header.parent.mkdir(parents=True)
    header.write_text('/* support */\n')
    source = canonical / 'fn_00401000.c'
    source.write_text('int cm_fn_00401000(void) { return 1; }\n')
    fixture = tmp_path / 'fixture.json'
    fixture.write_text('{}')
    manifest = {'jobs': [{'id': 'repair', 'config': {
        'function': '0x00401000', 'mode': 'c',
        'seed_fixture': runtime._pin(fixture),
        'candidate_seed': runtime._pin(source)}}]}
    return analysis / 'campaign', manifest, source


def test_retained_seed_is_private_and_does_not_modify_canonical(tmp_path, monkeypatch):
    directory, manifest, canonical = seeded_manifest(tmp_path, monkeypatch)
    original = canonical.read_bytes()
    prepared = runtime.prepare_manifest(directory, manifest)
    assert not directory.exists()
    runtime.seed_candidates(directory, prepared)
    candidate = Path(prepared['jobs'][0]['config']['candidate'])
    assert candidate.is_relative_to(directory)
    assert candidate.read_bytes() == original
    candidate.write_text('private repair\n')
    runtime.verify_seeded_candidates(directory, prepared)
    assert canonical.read_bytes() == original
    with pytest.raises(ValueError, match='already exists'):
        runtime.seed_candidates(directory, prepared)
    assert candidate.read_text() == 'private repair\n'


def test_seed_hash_drift_and_unreconciled_canonical_are_refused(tmp_path, monkeypatch):
    directory, manifest, canonical = seeded_manifest(tmp_path, monkeypatch)
    prepared = runtime.prepare_manifest(directory, manifest)
    canonical.write_text('changed after planning\n')
    with pytest.raises(ValueError, match='hash changed'):
        runtime.seed_candidates(directory, prepared)
    assert not directory.exists()
    with pytest.raises(ValueError, match='hash changed'):
        runtime.prepare_manifest(directory, manifest)
    retained = tmp_path / 'retained.c'
    retained.write_text('different retained source\n')
    manifest['jobs'][0]['config']['candidate_seed'] = runtime._pin(retained)
    with pytest.raises(ValueError, match='canonical'):
        runtime.prepare_manifest(directory, manifest)


@pytest.mark.parametrize('contents', [b'\x00', b'\xff'])
def test_invalid_seed_bytes_never_create_a_private_candidate(tmp_path, monkeypatch, contents):
    directory, manifest, canonical = seeded_manifest(tmp_path, monkeypatch)
    canonical.write_bytes(contents)
    manifest['jobs'][0]['config']['candidate_seed'] = runtime._pin(canonical)
    with pytest.raises(ValueError):
        runtime.prepare_manifest(directory, manifest)
    assert not directory.exists()


def test_input_jobs_and_escaping_candidates_cannot_use_seed_copy(tmp_path, monkeypatch):
    directory, manifest, canonical = seeded_manifest(tmp_path, monkeypatch)
    prepared = runtime.prepare_manifest(directory, manifest)
    prepared['jobs'][0]['config']['candidate'] = str(canonical)
    original = canonical.read_bytes()
    with pytest.raises(ValueError, match='escapes'):
        runtime.seed_candidates(directory, prepared)
    assert canonical.read_bytes() == original and not directory.exists()
    manifest['jobs'][0]['config']['mode'] = 'input'
    with pytest.raises(ValueError, match='only supported for C'):
        runtime.prepare_manifest(directory, manifest)


@pytest.mark.parametrize('state', ['translation-defect', 'evidence-needed'])
def test_c_dispatch_hold_is_reported_before_initialization(tmp_path, monkeypatch, state):
    directory, manifest, _ = seeded_manifest(tmp_path, monkeypatch)
    monkeypatch.setattr(work, 'read', lambda: {'functions': {'fn_00401000': {
        'state': state, 'retry_when': 'Inspect the original witness first'}}})
    with pytest.raises(ValueError, match='not dispatch-ready.*cmre work retry'):
        runtime.prepare_manifest(directory, manifest)
    assert not directory.exists()


@pytest.mark.parametrize('fail_marker', [False, True])
def test_partially_initialized_queue_cannot_dispatch(tmp_path, monkeypatch, fail_marker):
    directory, manifest, _ = seeded_manifest(tmp_path, monkeypatch)
    manifest.update(schema=provider_campaign.SCHEMA, duration_seconds=60, concurrency=1,
                    roles={'minimax': {'model': 'm'}, 'deepseek': {'model': 'd'}})
    manifest['jobs'][0]['config']['goal'] = 'Repair retained source'
    prepared = runtime.prepare_manifest(directory, manifest)
    provider_campaign.initialize(directory, prepared, {})
    candidate = Path(prepared['jobs'][0]['config']['candidate'])
    with monkeypatch.context() as patch:
        if fail_marker:
            original_write = runtime.coordination.atomic_json
            def fail_completion(path, value):
                if Path(path).name == 'seed-candidates.json':
                    raise OSError('simulated completion write failure')
                return original_write(path, value)
            patch.setattr(runtime.coordination, 'atomic_json', fail_completion)
        else:
            original_open = Path.open
            def fail_copy(path, mode='r', *args, **kwargs):
                if path == candidate and mode == 'xb':
                    raise OSError('simulated copy failure')
                return original_open(path, mode, *args, **kwargs)
            patch.setattr(Path, 'open', fail_copy)
        with pytest.raises(OSError):
            runtime.seed_candidates(directory, prepared)
    assert provider_jobs.load(directory)['jobs']['repair']['state'] == 'prepared'
    assert candidate.exists() == fail_marker
    with pytest.raises(ValueError, match='initialization is incomplete'):
        runtime.verify_seeded_candidates(directory, prepared)


@pytest.mark.parametrize('existing_lease', [False, True])
@pytest.mark.parametrize('prior_owner', ['same', 'foreign'])
@pytest.mark.parametrize('refusal', [None, 'Task attempts exhausted',
                                     'Revision requires a validated submission'])
def test_prior_submission_reopens_once_without_resetting_attempts(
        tmp_path, monkeypatch, existing_lease, refusal, prior_owner):
    campaign = tmp_path / 'campaign'
    root = campaign / 'jobs/repair'
    root.mkdir(parents=True)
    source = root / 'fn_00401000.c'
    source.write_text('int cm_fn_00401000(void) { return 1; }\n')
    monkeypatch.setattr(env, 'ROOT', tmp_path)
    monkeypatch.setattr(worktrees, '_record_attempts', lambda *args: None)
    monkeypatch.setattr(worktrees, '_report_feedback', lambda report: {})
    calls = []
    state = {'task_id': 'task-retained', 'status': 'submitted', 'lease': None,
             'input_fingerprint': 'fp', 'attempts': [{'number': 1, 'status': 'pass',
                 'owner': runtime._task_owner(campaign if prior_owner == 'same' else
                                              tmp_path / 'different' / 'campaign', 'repair')}]}

    class API:
        def status(self, task_id):
            assert task_id == state['task_id']
            return state

        def prepare(self, va):
            calls.append('prepare')
            return state

        def revise(self, task_id, reason):
            calls.append('revise')
            assert len(state['attempts']) == 1
            if refusal:
                raise ValueError(refusal)
            state['status'] = 'prepared'
            return state

        def claim(self, task_id, owner, seconds):
            calls.append('claim')
            assert state['status'] == 'prepared'
            state.update(status='claimed', lease={'token': 'token', 'expires_at': 10**12})
            return state

        def _candidate(self, task_id, va):
            return root / 'staged.c'

        def submit(self, task_id, owner, token, **kwargs):
            calls.append('submit')
            assert len(state['attempts']) == 1
            report = tmp_path / 'report.json'
            report.write_text(json.dumps({'status': 'pass'}))
            state['attempts'].append({'number': 2, 'status': 'pass',
                'owner': owner,
                'candidate_sha256': runtime._hash(source), 'report_path': 'report.json',
                'report_sha256': runtime._hash(report)})
            state['status'] = 'submitted'
            return state

    if existing_lease:
        (root / 'task-lease.json').write_text(json.dumps({
            'task_id': state['task_id'], 'owner': 'prior', 'token': 'expired'}))
    executor = runtime.Executor(campaign, task_api=API())
    config = {'directory': str(root), 'function': '0x00401000', 'mode': 'c',
              'candidate': str(source)}
    job = {'id': 'repair', 'config': config}
    foreign_owner = prior_owner != 'same'
    if refusal or foreign_owner:
        with pytest.raises(ValueError, match='task-retained.*submitted'):
            executor._task_action(job, config, root, {'kind': 'test'}, root / 'output')
        assert 'claim' not in calls and 'submit' not in calls
        assert len(state['attempts']) == 1
    else:
        result = executor._task_action(job, config, root, {'kind': 'test'}, root / 'output')
        assert result['ok'] and result['attempt'] == 2
        assert calls[-3:] == ['revise', 'claim', 'submit']
    assert calls.count('revise') == int(not foreign_owner)


def test_task_owners_separate_equal_basenames_and_fit_the_task_limit(tmp_path):
    first = runtime._task_owner(tmp_path / 'first/campaign', 'x' * 64)
    second = runtime._task_owner(tmp_path / 'second/campaign', 'x' * 64)
    assert first != second
    assert len(first) <= 128
