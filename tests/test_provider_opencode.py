"""Scoped fake-runner tests for the OpenCode actor adapter."""
from __future__ import annotations

import hashlib
import json
import os
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from cmre.provider_opencode import (  # noqa: E402
    OpenCodeActor, OpenCodeError, decode_context, encode_context)

SECRET = 'sk-secret-value-42'


@pytest.mark.parametrize('phase,mode,forbidden', [
    ('owner', 'input', 'test'), ('review', 'c', 'build'), ('owner', 'c', 'review')])
def test_protocol_advertises_only_current_mode_actions(tmp_path, phase, mode, forbidden):
    from cmre.provider_probe import _NAME_RE
    actor = OpenCodeActor({}, tmp_path / 'parent', runner=lambda *args: None,
                          collector=lambda *args: None)
    protocol = actor._protocol_text('deepseek', phase, mode)
    allowed = next(line for line in protocol.splitlines() if line.startswith('- Allowed action kinds for this stage:'))
    assert forbidden not in allowed.split(':', 1)[1].strip().rstrip('.').split(', ')
    assert f'"kind": "{forbidden}"' not in protocol
    assert _NAME_RE.pattern in protocol
    assert 'C controls and control-plan require a passing full test for the current source' in protocol


def test_review_context_deduplicates_accounting_and_preserves_full_source(tmp_path):
    actor = OpenCodeActor({}, tmp_path / 'parent', runner=lambda *args: None,
                          collector=lambda *args: None)
    bundle = {'sha256': 'f' * 64, 'payload': {
        'source': {'text': 'complete source'}, 'emitted': {'O0': 'all O0', 'O2': 'all O2'},
        'records': [{'id': 'a0001', 'action': {'kind': 'probe'},
                     'actor_response': {'billing': 'x' * 100000}}]}}
    original = json.dumps(bundle)
    context = {'phase': 'review', 'bundle': bundle,
               'results': [{'id': 'a0001', 'kind': 'bundle', 'payload': bundle['payload']}]}
    document = actor._context_document({'function': '0x00401000'}, context,
                                       'minimax', 'review', 'c')
    assert len(json.dumps(document)) < 5000
    assert document['bundle']['payload']['source'] == bundle['payload']['source']
    assert document['bundle']['payload']['emitted'] == bundle['payload']['emitted']
    assert json.dumps(bundle) == original


def test_effective_client_permissions_deny_every_undeclared_capability(tmp_path):
    actor = OpenCodeActor({}, tmp_path / 'parent', runner=lambda *args: None,
                          collector=lambda *args: None)
    effective = actor._opencode_config(['packet/input.txt'], ['packet/action.json'])
    permissions = effective['permission']
    assert permissions['*'] == 'deny'
    assert permissions['read'] == {'*': 'deny', 'packet/input.txt': 'allow'}
    assert permissions['edit'] == {'*': 'deny', 'packet/action.json': 'allow'}


def test_explicit_effort_reaches_both_scoped_worker_specs(tmp_path):
    runner, collector = FakeRunner(), FakeCollector()
    actor = _actor(tmp_path, runner=runner, collector=collector)
    actor.profiles['deepseek']['effort'] = 'low'
    _call(actor, _job(tmp_path))
    for spec in (runner.smoke_spec, runner.work_spec):
        cfg = json.loads(spec['environment']['OPENCODE_CONFIG_CONTENT'])
        assert cfg['provider']['deepseek']['models']['deepseek-chat']['options'] == {
            'effort': 'low', 'reasoningEffort': 'low'}
        assert cfg['permission']['*'] == 'deny'
        assert cfg['permission']['bash'] == 'deny'


@pytest.mark.parametrize('timeout', [.5, float('inf'), float('nan'), True])
def test_provider_timeout_requires_finite_whole_second_budget(timeout):
    from cmre import provider_opencode as adapter
    with pytest.raises(OpenCodeError, match='provider_timeout_seconds'):
        adapter._budget({'provider_timeout_seconds': timeout}, 1000)


def test_subsecond_remaining_work_is_refused_before_dispatch():
    from cmre import provider_opencode as adapter
    with pytest.raises(OpenCodeError, match='Insufficient remaining'):
        adapter._budget({'provider_timeout_seconds': 60}, 90.5)


def test_nullable_context_containers_are_controlled(tmp_path):
    actor = OpenCodeActor({}, tmp_path / 'parent', runner=lambda *args: None,
                          collector=lambda *args: None)
    document = actor._context_document({'function': '0x00401000'}, {'results': None},
                                       'deepseek', 'owner', 'input')
    assert document['context']['results'] == []
    assert actor._artifact_ids({}, {'results': [{'artifacts': None}]}) == []
    with pytest.raises(OpenCodeError, match='bundle'):
        actor._context_document({'function': '0x00401000'}, {'bundle': None},
                                'minimax', 'review', 'input')
SOURCE = 'recovery/c/fn_VA.c'


def _snapshot(root):
    result = {}
    for path in sorted(root.rglob('*')):
        relative = path.relative_to(root)
        if '.git' in relative.parts or not path.is_file():
            continue
        result[relative.as_posix()] = hashlib.sha256(path.read_bytes()).hexdigest()
    return result


class FakeRunner:
    """Emits receipts shaped like scripts.cmre_provider_worker.pair."""

    def __init__(self, *, smoke_state='completed', work_state='completed', models_observed=None,
                 action=None, action_text=None, exit_code=0, readiness_passed=True,
                 smoke_content='READY-SCOPE', edit_source=None, drop_action=False):
        self.smoke_state = smoke_state
        self.work_state = work_state
        self.models_observed = models_observed
        self.action = action
        self.action_text = action_text
        self.exit_code = exit_code
        self.readiness_passed = readiness_passed
        self.smoke_content = smoke_content
        self.edit_source = edit_source
        self.drop_action = drop_action
        self.calls = []
        self.smoke_spec = None
        self.work_spec = None

    def __call__(self, smoke_path, work_path, run_dir):
        self.calls.append((smoke_path, work_path, run_dir))
        self.smoke_spec = json.loads(Path(smoke_path).read_text(encoding='utf-8'))
        self.work_spec = json.loads(Path(work_path).read_text(encoding='utf-8'))
        run_dir = Path(run_dir)
        (run_dir / 'smoke').mkdir(parents=True)
        (run_dir / 'work').mkdir(parents=True)
        smoke = self._stage(self.smoke_spec, run_dir / 'smoke', 'smoke')
        work = self._stage(self.work_spec, run_dir / 'work', 'work')
        state = work['state'] if work['state'] != 'completed' else smoke['state']
        record = {'schema': 'cmre-provider-pair/1', 'state': state,
                  'started': 1.0, 'ended': 2.0, 'elapsed_seconds': 1.0,
                  'stages': [{'stage': 'smoke', 'state': smoke['state'],
                              'receipt': str(run_dir / 'smoke' / 'receipt.json')},
                             {'stage': 'work', 'state': work['state'],
                              'receipt': str(run_dir / 'work' / 'receipt.json')}]}
        (run_dir / 'receipt.json').write_text(json.dumps(record, indent=2), encoding='utf-8')
        return record

    def _stage(self, spec, directory, phase):
        root = Path(spec['directory'])
        before = _snapshot(root)
        for name in spec['required_text_files']:
            (root / name).read_text(encoding='utf-8')
        if phase == 'smoke':
            smoke = root / spec['smoke_file']
            smoke.parent.mkdir(parents=True, exist_ok=True)
            smoke.write_text(self.smoke_content, encoding='utf-8')
        else:
            if self.edit_source:
                target = root / self.edit_source
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_text('int fn_VA(void) { return 2; }\n', encoding='utf-8')
            if not self.drop_action:
                action = root / 'packet' / 'action.json'
                action.parent.mkdir(parents=True, exist_ok=True)
                if self.action_text is not None:
                    action.write_text(self.action_text, encoding='utf-8')
                else:
                    action.write_text(json.dumps(
                        self.action if self.action is not None
                        else {'kind': 'hold', 'reason': 'nothing observed'}, indent=2), encoding='utf-8')
        after = _snapshot(root)
        changed = sorted(name for name in set(before) | set(after) if before.get(name) != after.get(name))
        out_of_scope = [name for name in changed if name not in spec['allowed_files']]
        observed = self.models_observed
        if observed is None:
            observed = [spec['model']]
        receipt = {
            'schema': 'cmre-provider-worker/1',
            'owner': spec['owner'],
            'functions': spec.get('functions', []),
            'model_requested': spec['model'],
            'parent_receipt': spec.get('parent_receipt'),
            'client_version': spec.get('client_version'),
            'started': 1.0, 'ended': 2.0, 'elapsed_seconds': 1.0,
            'state': self.smoke_state if phase == 'smoke' else self.work_state,
            'controller_pid': os.getpid(), 'controller_identity': 'fake',
            'source_before': before, 'source_after': after,
            'session_ids': ['ses_fake'],
            'calls': [{'call_id': 'call_fake', 'model': spec['model'], 'input_tokens': 12,
                       'output_tokens': 7, 'reasoning_tokens': None, 'cache_read_tokens': 0,
                       'cache_write_tokens': 0, 'cost': 0.01, 'finish': 'stop',
                       'usage_observed': True}],
            'steps': 4, 'finished': True, 'provider_errors': [],
            'changed_files': changed, 'out_of_scope': out_of_scope, 'pin_drift': [],
            'parse_errors': [], 'exit_code': self.exit_code,
            'models_observed': observed,
        }
        if phase == 'smoke':
            receipt['readiness'] = {'identity': {'schema': 'cmre-provider-readiness/1'},
                                    'passed': self.readiness_passed, 'missing_reads': [],
                                    'write_proven': True, 'errors': []}
        (directory / 'receipt.json').write_text(json.dumps(receipt, indent=2), encoding='utf-8')
        return receipt


class FakeCollector:
    """Emits a collection receipt shaped like scripts.cmre_provider_worker.collect."""

    def __init__(self, *, usage_status='collected'):
        self.usage_status = usage_status
        self.run_directories = []

    def __call__(self, run_dir):
        self.run_directories.append(str(run_dir))
        record = json.loads((Path(run_dir) / 'receipt.json').read_text(encoding='utf-8'))
        result = {'schema': 'cmre-provider-collection/1', 'run_state': record['state'],
                  'run_directory': str(run_dir), 'usage_status': self.usage_status,
                  'calls': record.get('calls') if self.usage_status == 'collected' else None,
                  'models_observed': record.get('models_observed'),
                  'artifacts': {}, 'recovered_export': False,
                  'export_path': str(Path(run_dir) / 'session-export.json')}
        versions = Path(run_dir) / 'collections' / '0001'
        versions.mkdir(parents=True, exist_ok=True)
        (versions / 'receipt.json').write_text(json.dumps(result, indent=2), encoding='utf-8')
        return result


def _profile(tmp_path, model, secret=SECRET):
    executable = tmp_path / 'bin' / 'opencode'
    executable.parent.mkdir(parents=True, exist_ok=True)
    executable.write_bytes(b'fake client\n')
    return {'executable': str(executable), 'model': model, 'client_version': '1.18.3',
            'environment': {'OPENCODE_API_KEY': secret, 'OPENCODE_LOG_LEVEL': 'warn'}}


def _job(tmp_path, *, mode='c', create_source=True, **overrides):
    job_root = tmp_path / 'job'
    (job_root / 'recovery' / 'c').mkdir(parents=True, exist_ok=True)
    source = job_root / 'recovery' / 'c' / 'fn_VA.c'
    if create_source:
        source.write_text('int fn_VA(void) { return 1; }\n', encoding='utf-8')
    config = {'goal': 'recover fn_VA', 'function': '0x004d1600',
              'allowed_functions': ['0x004d1600'],
              'candidate': str(source), 'directory': str(job_root), 'mode': mode,
              'owner': 'deepseek', 'reviewer': 'minimax'}
    config.update(overrides)
    return {'config': config, 'directory': str(job_root)}


def _parent(tmp_path):
    tmp_path.mkdir(parents=True, exist_ok=True)
    receipt = tmp_path / 'parent-receipt.json'
    receipt.write_text(json.dumps({'schema': 'cmre-provider-parent/1'}), encoding='utf-8')
    return receipt


def _actor(tmp_path, *, deepseek_model='deepseek/deepseek-chat',
           minimax_model='minimax/minimax-m2', runner=None, collector=None):
    profiles = {'deepseek': _profile(tmp_path / 'deepseek', deepseek_model),
                'minimax': _profile(tmp_path / 'minimax', minimax_model)}
    return OpenCodeActor(profiles, _parent(tmp_path), runner=runner, collector=collector)


def test_captured_smoke_proof_survives_an_allowed_work_marker_update(tmp_path):
    class MarkerUpdatedRunner(FakeRunner):
        def _stage(self, spec, stage_dir, phase):
            receipt = super()._stage(spec, stage_dir, phase)
            if phase == 'work':
                marker = Path(spec['directory']) / 'packet/smoke.txt'
                marker.write_text('work stage note', encoding='utf-8')
                receipt['source_after']['packet/smoke.txt'] = hashlib.sha256(marker.read_bytes()).hexdigest()
                receipt['changed_files'].append('packet/smoke.txt')
                (stage_dir / 'receipt.json').write_text(json.dumps(receipt), encoding='utf-8')
            return receipt
    runner = MarkerUpdatedRunner(action={'kind': 'screen'})
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    assert _call(actor, _job(tmp_path, mode='input'))['action'] == {'kind': 'screen'}


def test_smoke_marker_requires_exact_text_when_captured_hash_is_not_known(tmp_path):
    actor = _actor(tmp_path, runner=FakeRunner(smoke_content='READY-SCOPE-FAILED',
                                              action={'kind': 'screen'}),
                   collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='READY-SCOPE'):
        _call(actor, _job(tmp_path, mode='input'))


def _context(phase='owner', **overrides):
    context = {'remaining_seconds': 300.0, 'phase': phase}
    context.update(overrides)
    return context


def _call(actor, job, role='deepseek', context=None, stage='stage-1'):
    stage_path = actor.parent_receipt.parent / stage
    return actor(job, role, context or _context(), stage_path)


def _probe_action():
    return {'kind': 'probe', 'request': {
        'function': '0x4d1600', 'name': 'owner-probe', 'patch': {},
        'budget': 1000,
        'shapes': {'0x4d1600': {'ecx': False, 'stack_words': 0}}}}


def test_provider_roles_and_phase_select_the_profile(tmp_path):
    runner, collector = FakeRunner(), FakeCollector()
    actor = _actor(tmp_path, runner=runner, collector=collector)
    result = _call(actor, _job(tmp_path), role='minimax',
                   context=_context('review'))
    assert runner.work_spec['owner'] == 'minimax'
    assert runner.work_spec['model'] == 'minimax/minimax-m2'
    assert runner.smoke_spec['model'] == 'minimax/minimax-m2'
    assert result['model'] == 'minimax/minimax-m2'
    assert result['role'] == 'minimax'
    assert result['phase'] == 'review'
    assert result['action']['kind'] == 'hold'


def test_observed_model_mismatch_is_rejected_and_all_stages_are_collected(tmp_path):
    runner = FakeRunner(models_observed=['other/model'])
    collector = FakeCollector()
    actor = _actor(tmp_path, runner=runner, collector=collector)
    with pytest.raises(OpenCodeError, match='observed another model') as caught:
        _call(actor, _job(tmp_path))
    assert caught.value.retryable is False
    assert [item['stage'] for item in caught.value.retained['collections']] == ['smoke', 'work']
    assert len(collector.run_directories) == 2


def test_incomplete_clean_stage_is_retryable_and_collected(tmp_path):
    runner = FakeRunner(work_state='incomplete', exit_code=0)
    collector = FakeCollector()
    actor = _actor(tmp_path, runner=runner, collector=collector)
    with pytest.raises(OpenCodeError, match='did not complete') as caught:
        _call(actor, _job(tmp_path))
    assert caught.value.retryable is True
    assert [item['stage'] for item in caught.value.retained['collections']] == ['smoke', 'work']


def test_scope_violation_is_not_retryable(tmp_path):
    runner = FakeRunner(edit_source=SOURCE)
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='outside the scope') as caught:
        _call(actor, _job(tmp_path), role='minimax', context=_context('review'))
    assert caught.value.retryable is False


def test_readiness_failure_is_rejected(tmp_path):
    runner = FakeRunner(readiness_passed=False)
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='readiness'):
        _call(actor, _job(tmp_path))


def test_missing_action_is_retryable_and_collected(tmp_path):
    runner = FakeRunner(drop_action=True)
    collector = FakeCollector()
    actor = _actor(tmp_path, runner=runner, collector=collector)
    with pytest.raises(OpenCodeError, match='no action') as caught:
        _call(actor, _job(tmp_path))
    assert caught.value.retryable is True
    assert len(caught.value.retained['collections']) == 2


def test_duplicate_json_keys_are_rejected(tmp_path):
    runner = FakeRunner(action_text='{"kind": "hold", "reason": "a", "reason": "b"}')
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='Duplicate JSON key'):
        _call(actor, _job(tmp_path))


def test_nonfinite_numbers_and_multiple_actions_are_rejected(tmp_path):
    runner = FakeRunner(action_text='{"kind": "hold", "reason": NaN}')
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError):
        _call(actor, _job(tmp_path))
    runner = FakeRunner(action_text='{"kind": "hold", "reason": "a"}{"kind": "hold", "reason": "b"}')
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='Invalid JSON'):
        _call(actor, _job(tmp_path), stage='stage-two-actions')


def test_action_keys_are_exact_and_notes_is_not_advertised(tmp_path):
    runner = FakeRunner(action={'kind': 'hold', 'reason': 'a', 'notes': 'extra'})
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='Unknown action keys'):
        _call(actor, _job(tmp_path))
    protocol = ''.join(
        path.read_text(encoding='utf-8')
        for path in sorted(Path(actor.parent_receipt.parent, 'stage-1', 'workspace', 'protocol').glob('*')))
    assert '"notes"' not in protocol


def test_reviewer_cannot_edit_source_and_candidate_is_readonly(tmp_path):
    runner = FakeRunner()
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    result = _call(actor, _job(tmp_path), role='minimax', context=_context('review'))
    assert SOURCE not in runner.work_spec['allowed_files']
    permissions = json.loads(Path(result['workspace'], 'packet', 'permissions.json').read_text())
    assert SOURCE not in permissions['writable']
    assert SOURCE in permissions['readable']
    assert result['source']['copied_back'] is False


def test_reviewer_requires_an_existing_candidate(tmp_path):
    job = _job(tmp_path, create_source=False)
    actor = _actor(tmp_path, runner=FakeRunner(), collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='existing candidate'):
        _call(actor, job, role='minimax', context=_context('review'))


def test_owner_source_is_copied_back_only_when_completed_and_valid(tmp_path):
    runner = FakeRunner(edit_source=SOURCE)
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    job = _job(tmp_path)
    result = _call(actor, job)
    assert result['source']['copied_back'] is True
    assert result['source']['relative'] == SOURCE
    assert (Path(job['directory']) / SOURCE).read_text() == 'int fn_VA(void) { return 2; }\n'


def test_owner_may_create_a_new_private_candidate(tmp_path):
    runner = FakeRunner(edit_source=SOURCE)
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    job = _job(tmp_path, create_source=False)
    result = _call(actor, job)
    assert result['source']['copied_back'] is True
    assert (Path(job['directory']) / SOURCE).read_text() == 'int fn_VA(void) { return 2; }\n'


def test_failed_work_does_not_copy_the_source_back(tmp_path):
    runner = FakeRunner(work_state='incomplete', edit_source=SOURCE)
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    job = _job(tmp_path)
    with pytest.raises(OpenCodeError):
        _call(actor, job)
    assert (Path(job['directory']) / SOURCE).read_text() == \
        'int fn_VA(void) { return 1; }\n'


def test_source_with_nul_or_long_lines_is_not_copied_back(tmp_path):
    runner = FakeRunner()
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    job = _job(tmp_path)
    original = runner._stage

    def edit_with_nul(spec, directory, phase):
        receipt = original(spec, directory, phase)
        if phase == 'work':
            (Path(spec['directory']) / SOURCE).write_bytes(b'int x;\x00\n')
        return receipt

    runner._stage = edit_with_nul
    with pytest.raises(OpenCodeError, match='NUL'):
        _call(actor, job)
    assert (Path(job['directory']) / SOURCE).read_text() == \
        'int fn_VA(void) { return 1; }\n'


def test_stale_context_hash_is_rejected(tmp_path):
    evidence = tmp_path / 'evidence.txt'
    evidence.write_text('pinned evidence\n', encoding='utf-8')
    job = _job(tmp_path, context_files=[{'path': str(evidence), 'sha256': '0' * 64}])
    actor = _actor(tmp_path, runner=FakeRunner(), collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='Stale'):
        _call(actor, job)


def test_actual_context_bundle_results_and_feedback_are_readable(tmp_path):
    evidence = tmp_path / 'evidence.txt'
    evidence.write_text('pinned evidence\n', encoding='utf-8')
    sha = hashlib.sha256(evidence.read_bytes()).hexdigest()
    job = _job(tmp_path, context_files=[{'path': str(evidence), 'sha256': sha}])
    actor = _actor(tmp_path, runner=FakeRunner(), collector=FakeCollector())
    context = _context(
        results=[{'id': 'a0001', 'kind': 'probe', 'summary': 'retained',
                  'path': 'jobs/job/a0001.json', 'sha256': 'a' * 64}],
        feedback=['name the unresolved witness'],
        bundle={'sha256': 'b' * 64, 'required_checks': ['original-return']},
        required_checks=['original-return'])
    result = _call(actor, job, context=context)
    workspace = Path(result['workspace'])
    body = ''.join(part.read_text(encoding='utf-8')
                   for part in sorted((workspace / 'context').glob('part-*.json')))
    assert 'pinned evidence' in body
    for marker in ('results', 'feedback', 'bundle', 'required_checks'):
        assert marker in body
    assert SECRET not in body
    assert 'OPENCODE_API_KEY' not in body


def test_candidate_must_be_absolute_and_stay_inside_the_job(tmp_path):
    actor = _actor(tmp_path, runner=FakeRunner(), collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='absolute'):
        _call(actor, _job(tmp_path, candidate=SOURCE))
    outside = tmp_path / 'outside.c'
    outside.write_text('int outside(void) { return 0; }\n', encoding='utf-8')
    with pytest.raises(OpenCodeError, match='escapes'):
        _call(actor, _job(tmp_path, candidate=str(outside)))


def test_long_context_parts_are_bounded_nonempty_and_have_no_terminal_blank(tmp_path):
    long_text = 'L' * 6000
    multiline = 'first line\nsecond line\nthird line'
    existing = {'note': multiline}
    for index in range(120):
        existing[f'field-{index:03d}'] = f'value {index} ' + 'x' * 200
    job = _job(tmp_path, goal=long_text)
    runner = FakeRunner()
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    result = _call(actor, job, context=_context(existing=existing))
    workspace = Path(result['workspace'])
    parts = sorted((workspace / 'context').glob('part-*.json'))
    assert len(parts) > 1
    for part in parts:
        lines = part.read_text(encoding='utf-8').splitlines()
        assert lines
        assert lines[-1].strip()
        assert len(lines) <= 100
        assert all(len(line) <= 1800 for line in lines)
    prompt = Path(runner.work_spec['prompt_file']).read_text(encoding='utf-8')
    for part in parts:
        assert part.relative_to(workspace).as_posix() in prompt
    assert encode_context(multiline)['__cmre_multiline__'] == ['first line', 'second line', 'third line']
    assert decode_context(encode_context(multiline)) == multiline
    assert decode_context(encode_context(long_text)) == long_text
    assert decode_context(encode_context({'a': [multiline, long_text]})) == {'a': [multiline, long_text]}


@pytest.mark.parametrize('limit', ['parts', 'bytes'])
def test_oversized_context_stops_before_any_paid_stage_and_preserves_evidence(tmp_path, limit):
    runner = FakeRunner()
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    existing = {'trace': ['x'] * 7000} if limit=='parts' else {'blob': ['x'*1400] * 200}
    original = json.dumps(existing)
    with pytest.raises(OpenCodeError, match='context budget') as caught:
        _call(actor, _job(tmp_path), context=_context(existing=existing))
    assert caught.value.retryable is False and runner.calls == []
    receipt = json.loads((tmp_path/'stage-1/context-preflight.json').read_text())
    assert receipt['state']=='held' and receipt['exceeded']==[limit]
    assert receipt['provider_dispatched'] is False
    parts = sorted((tmp_path/'stage-1/workspace/context').glob('part-*.json'))
    document = decode_context(json.loads(''.join(p.read_text(encoding='utf-8') for p in parts)))
    assert document['existing_context'] == existing and json.dumps(existing) == original
    assert 'existing' not in document['context']


def test_no_secrets_in_workspace_or_return(tmp_path):
    runner = FakeRunner()
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    result = _call(actor, _job(tmp_path))
    workspace = Path(result['workspace'])
    for path in workspace.rglob('*'):
        if path.is_file():
            assert SECRET not in path.read_text(encoding='utf-8', errors='ignore')
    assert SECRET not in json.dumps(result)
    assert 'OPENCODE_API_KEY' not in json.dumps(result)


def test_time_caps_are_applied(tmp_path):
    runner = FakeRunner()
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    job = _job(tmp_path, provider_timeout_seconds=45)
    _call(actor, job, context=_context(remaining_seconds=300.0))
    assert runner.smoke_spec['timeout_seconds'] == 45
    assert runner.work_spec['timeout_seconds'] == 45
    second_runner = FakeRunner()
    actor = _actor(tmp_path / 'second', runner=second_runner, collector=FakeCollector())
    job = _job(tmp_path / 'second')
    _call(actor, job, context=_context(remaining_seconds=100.0))
    assert second_runner.smoke_spec['timeout_seconds'] == 60
    assert second_runner.work_spec['timeout_seconds'] == 10


def test_insufficient_remaining_time_fails_before_dispatch(tmp_path):
    runner = FakeRunner()
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='Insufficient'):
        _call(actor, _job(tmp_path), context=_context(remaining_seconds=10.0))
    with pytest.raises(OpenCodeError):
        _call(actor, _job(tmp_path), context=_context(remaining_seconds=60.0))
    assert runner.calls == []


def test_git_failure_stops_before_runner_dispatch(tmp_path, monkeypatch):
    import cmre.provider_opencode as module
    def broken(*args, **kwargs):
        raise OSError('no git')
    monkeypatch.setattr(module.subprocess, 'run', broken)
    runner = FakeRunner()
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='git init failed'):
        _call(actor, _job(tmp_path))
    assert runner.calls == []


def test_arbitrary_capabilities_are_denied(tmp_path):
    runner = FakeRunner()
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    result = _call(actor, _job(tmp_path))
    config = json.loads(runner.work_spec['environment']['OPENCODE_CONFIG_CONTENT'])
    assert config['permission']['bash'] == 'deny'
    assert config['permission']['webfetch'] == 'deny'
    assert config['permission']['edit']['*'] == 'deny'
    assert config['permission']['read']['*'] == 'deny'
    assert config['plugin'] == []
    assert config['snapshot'] is False
    assert config['autoupdate'] is False
    assert config['share'] == 'disabled'
    assert config['tools'] == {'task': False, 'skill': False, 'webfetch': False,
                               'question': False, 'todowrite': False}
    permissions = json.loads(Path(result['workspace'], 'packet', 'permissions.json').read_text())
    for capability in ('network', 'shell', 'delegation', 'glob', 'grep'):
        assert capability in permissions['denied']
    assert permissions['canonical_writes'] == 'denied'


def test_no_canonical_modifications(tmp_path):
    job = _job(tmp_path)
    job_root = Path(job['directory'])
    before = _snapshot(job_root)
    runner = FakeRunner()
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    result = _call(actor, job, role='minimax', context=_context('review'))
    assert _snapshot(job_root) == before
    assert not (job_root / 'packet').exists()
    assert not (job_root / 'context').exists()
    assert Path(result['workspace']).is_relative_to(actor.parent_receipt.parent)


def test_unknown_role_and_missing_profile_are_rejected(tmp_path):
    actor = _actor(tmp_path, runner=FakeRunner(), collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='Unknown role'):
        _call(actor, _job(tmp_path), role='analyst')
    empty = OpenCodeActor({}, _parent(tmp_path), runner=FakeRunner(), collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='No worker profile'):
        empty(_job(tmp_path), 'deepseek', _context(), tmp_path / 'stage-empty')


def test_reviewer_must_use_an_independent_model(tmp_path):
    profiles = {'deepseek': _profile(tmp_path / 'deepseek', 'same/model'),
                'minimax': _profile(tmp_path / 'minimax', 'same/model')}
    actor = OpenCodeActor(profiles, _parent(tmp_path), runner=FakeRunner(), collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='must differ'):
        _call(actor, _job(tmp_path), role='minimax', context=_context('review'))


def test_review_must_carry_the_exact_bundle_and_all_pass_checks(tmp_path):
    runner = FakeRunner(action={'kind': 'review', 'verdict': 'pass', 'bundle_sha256': 'a' * 64,
                                'checks': ['build'], 'unknowns': []})
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='frozen bundle'):
        _call(actor, _job(tmp_path), role='minimax',
              context=_context('review', bundle={'sha256': 'b' * 64},
                               required_checks=['build']), stage='stage-bad-bundle')
    runner = FakeRunner(action={'kind': 'review', 'verdict': 'pass', 'bundle_sha256': 'b' * 64,
                                'checks': ['build'], 'unknowns': []})
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    result = _call(actor, _job(tmp_path), role='minimax',
                   context=_context('review', bundle={'sha256': 'b' * 64},
                                    required_checks=['build']), stage='stage-exact')
    assert result['action']['verdict'] == 'pass'
    runner = FakeRunner(action={'kind': 'review', 'verdict': 'pass', 'bundle_sha256': 'b' * 64,
                                'checks': ['build'], 'unknowns': []})
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='misses required checks'):
        _call(actor, _job(tmp_path), role='minimax',
              context=_context('review', bundle={'sha256': 'b' * 64},
                               required_checks=['build', 'screen']), stage='stage-checks')


def test_revise_and_hold_allow_subset_checks_and_unknowns(tmp_path):
    for verdict in ('revise', 'hold'):
        runner = FakeRunner(action={'kind': 'review', 'verdict': verdict,
                                    'bundle_sha256': 'b' * 64, 'checks': [],
                                    'unknowns': ['unresolved evidence a0002'],
                                    'reason': 'need more witnesses'})
        actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
        result = _call(actor, _job(tmp_path), role='minimax',
                       context=_context('review', bundle={'sha256': 'b' * 64},
                                        required_checks=['build', 'screen']),
                       stage=f'stage-{verdict}')
        assert result['action']['verdict'] == verdict


def test_owner_proposal_uses_retained_action_ids_not_workspace_names(tmp_path):
    runner = FakeRunner(action={'kind': 'propose', 'artifacts': ['a0002'], 'summary': 'ok'})
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='not retained'):
        _call(actor, _job(tmp_path), stage='stage-unretained',
              context=_context(results=[{'id': 'a0001', 'kind': 'probe', 'summary': 'x',
                                         'path': 'jobs/job/a0001.json'}]))
    runner = FakeRunner(action={'kind': 'propose', 'artifacts': ['a0001'], 'summary': 'ok'})
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    result = _call(actor, _job(tmp_path),
                   context=_context(results=[{'id': 'a0001', 'kind': 'probe', 'summary': 'x',
                                              'path': 'jobs/job/a0001.json'}]))
    assert result['action']['artifacts'] == ['a0001']
    runner = FakeRunner(action={'kind': 'propose', 'artifacts': [SOURCE], 'summary': 'ok'})
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='action IDs'):
        _call(actor, _job(tmp_path), stage='stage-filename',
              context=_context(results=[{'id': 'a0001'}]))


def test_input_mode_permits_only_original_queries_and_proposals(tmp_path):
    runner = FakeRunner(action=_probe_action())
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    result = _call(actor, _job(tmp_path, mode='input', create_source=False))
    assert result['action']['kind'] == 'probe'
    permissions = json.loads(Path(result['workspace'], 'packet', 'permissions.json').read_text())
    assert 'recovery' not in json.dumps(permissions['readable'])
    assert 'recovery' not in json.dumps(permissions['writable'])


def test_input_mode_rejects_source_and_build_style_actions(tmp_path):
    for action in ({'kind': 'build'}, {'kind': 'test'}, {'kind': 'controls'}):
        runner = FakeRunner(action=action)
        actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
        with pytest.raises(OpenCodeError, match='not permitted'):
            _call(actor, _job(tmp_path, mode='input', create_source=False),
                  stage='stage-' + action['kind'])


def test_c_mode_permits_build_test_and_controls(tmp_path):
    for index, action in enumerate(({'kind': 'build'}, {'kind': 'test'}, {'kind': 'controls'})):
        runner = FakeRunner(action=action)
        actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
        result = _call(actor, _job(tmp_path), stage=f'stage-c-{index}')
        assert result['action']['kind'] == action['kind']


def test_probe_accepts_broker_address_shapes_and_rejects_permissive_forms(tmp_path):
    runner = FakeRunner(action=_probe_action())
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    result = _call(actor, _job(tmp_path, mode='input', create_source=False))
    assert result['action']['request']['function'] == '0x4d1600'
    bad = _probe_action()
    bad['request']['shapes'] = {}
    runner = FakeRunner(action=bad)
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='root shape'):
        _call(actor, _job(tmp_path, mode='input', create_source=False), stage='stage-no-shape')


def test_memory_address_accepts_hex_but_probe_base_rejects_paths(tmp_path):
    runner = FakeRunner(action={'kind': 'memory', 'address': '0x10', 'size': 4})
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    result = _call(actor, _job(tmp_path, mode='input', create_source=False))
    assert result['action']['address'] == '0x10'
    bad = _probe_action()
    bad['base'] = 'jobs/job/a0001.json'
    runner = FakeRunner(action=bad)
    actor = _actor(tmp_path, runner=runner, collector=FakeCollector())
    with pytest.raises(OpenCodeError, match='file ID'):
        _call(actor, _job(tmp_path, mode='input', create_source=False), stage='stage-base')


def test_each_stage_uses_a_fresh_workspace(tmp_path):
    actor = _actor(tmp_path, runner=FakeRunner(), collector=FakeCollector())
    job = _job(tmp_path)
    stage = actor.parent_receipt.parent / 'stage-1'
    actor(job, 'deepseek', _context(), stage)
    with pytest.raises(OpenCodeError, match='fresh stage path'):
        actor(job, 'deepseek', _context(), stage)


def test_collections_and_actor_receipt_retain_per_call_accounting(tmp_path):
    collector = FakeCollector()
    actor = _actor(tmp_path, runner=FakeRunner(), collector=collector)
    result = _call(actor, _job(tmp_path))
    assert [item['stage'] for item in result['collections']] == ['smoke', 'work']
    assert all(item['usage_status'] == 'collected' for item in result['collections'])
    assert all(item['receipt_sha256'] for item in result['collections'])
    assert result['calls'] and result['calls'][0]['model'] == 'deepseek/deepseek-chat'
    assert result['receipt']['stages']['smoke']['calls']
    assert result['receipt']['stages']['work']['calls']
    assert result['receipt']['collections'] == result['collections']
    assert SECRET not in json.dumps(result['receipt'])


def test_failed_collection_keeps_usage_unknown(tmp_path):
    collector = FakeCollector(usage_status='unknown')
    actor = _actor(tmp_path, runner=FakeRunner(), collector=collector)
    result = _call(actor, _job(tmp_path))
    assert all(item['calls'] is None for item in result['collections'])
    assert all(item['usage_status'] == 'unknown' for item in result['collections'])


def test_runner_defaults_to_the_existing_worker_pair():
    worker = pytest.importorskip('scripts.cmre_provider_worker')
    actor = OpenCodeActor({'deepseek': {'executable': __file__, 'model': 'a/b',
                                        'client_version': '1.18.3', 'environment': {}}},
                          Path(__file__))
    assert actor.runner is worker.pair
    assert actor.collector is worker.collect
