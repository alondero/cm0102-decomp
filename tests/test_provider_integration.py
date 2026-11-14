"""End-to-end fake-runner integration through the durable campaign core.

The real campaign orchestrator, the real OpenCode actor adapter and the real
runtime Executor are used with an API-shaped fake runner, fake collector and
an asset-free broker.  The test proves an actual input-mode owner probe, an
owner proposal, independent second-model review and hash-backed readiness.
"""
from __future__ import annotations

import gzip
import hashlib
import json
import os
from pathlib import Path
import sys


def test_action_key_contract_agrees_across_actor_controller_and_broker():
    from cmre import provider_opencode, provider_campaign, provider_broker
    for kind, expected in provider_broker._ACTION_KEYS.items():
        required, optional = provider_opencode.ACTION_KEYS[kind]
        assert set(expected) == {'kind', *required, *optional}
        controller_required, controller_optional = provider_campaign.ACTION_SCHEMAS[kind]
        assert set(expected) == {'kind', *controller_required, *controller_optional}
        assert set(required) == set(controller_required)


def test_minimal_evidence_and_hex_addresses_cross_actor_and_controller(tmp_path):
    from cmre import provider_opencode as actor, provider_campaign as controller
    adapter = actor.OpenCodeActor({}, tmp_path / 'parent', runner=lambda *args: None,
                                  collector=lambda *args: None)
    for action in ({'kind': 'evidence'},
                   {'kind': 'evidence', 'function': 0x401000, 'start': '0x401004'},
                   {'kind': 'memory', 'address': '0x00401000', 'size': 4}):
        assert controller._validate_action(action, 'owner', 'input') is None
        adapter._validate_action(action, role='deepseek', phase='owner', mode='input',
                                 context={}, config={})

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from cmre import env, provider_jobs  # noqa: E402
from cmre import provider_campaign as campaign  # noqa: E402
from cmre.provider_opencode import OpenCodeActor  # noqa: E402
from cmre.provider_runtime import Executor, prepare_manifest  # noqa: E402

SECRET = 'sk-integration-secret'


def _snapshot(root):
    result = {}
    for path in sorted(root.rglob('*')):
        relative = path.relative_to(root)
        if '.git' in relative.parts or not path.is_file():
            continue
        result[relative.as_posix()] = hashlib.sha256(path.read_bytes()).hexdigest()
    return result


def _pin(path):
    path = Path(path).resolve()
    return {'path': str(path), 'sha256': hashlib.sha256(path.read_bytes()).hexdigest()}


class ScriptedRunner:
    """Read the actual model context and emit probe, propose then review."""

    def __init__(self):
        self.calls = []
        self.documents = []

    def _document(self, spec):
        root = Path(spec['directory'])
        text = ''.join((root / name).read_text(encoding='utf-8')
                       for name in spec['required_text_files']
                       if name.startswith('context/'))
        return json.loads(text)

    def __call__(self, smoke_path, work_path, run_dir):
        self.calls.append((smoke_path, work_path, run_dir))
        smoke_spec = json.loads(Path(smoke_path).read_text(encoding='utf-8'))
        work_spec = json.loads(Path(work_path).read_text(encoding='utf-8'))
        document = self._document(work_spec)
        self.documents.append(document)
        action = self._action(len(self.calls), document)
        run_dir = Path(run_dir)
        (run_dir / 'smoke').mkdir(parents=True)
        (run_dir / 'work').mkdir(parents=True)
        smoke = self._stage(smoke_spec, run_dir / 'smoke', 'smoke', None)
        work = self._stage(work_spec, run_dir / 'work', 'work', action)
        state = work['state'] if work['state'] != 'completed' else smoke['state']
        record = {'schema': 'cmre-provider-pair/1', 'state': state,
                  'started': 1.0, 'ended': 2.0, 'elapsed_seconds': 1.0,
                  'stages': [{'stage': 'smoke', 'state': smoke['state'],
                              'receipt': str(run_dir / 'smoke' / 'receipt.json')},
                             {'stage': 'work', 'state': work['state'],
                              'receipt': str(run_dir / 'work' / 'receipt.json')}]}
        (run_dir / 'receipt.json').write_text(json.dumps(record, indent=2), encoding='utf-8')
        return record

    @staticmethod
    def _action(number, document):
        if document['phase'] == 'owner':
            probes = [item for item in document['context']['results']
                      if item.get('kind') == 'probe']
            if not probes:
                return {'kind': 'probe', 'request': {
                    'function': '0x4d1600', 'name': 'owner-probe', 'patch': {},
                    'budget': 1000,
                    'shapes': {'0x4d1600': {'ecx': False, 'stack_words': 0}}}}
            return {'kind': 'propose', 'artifacts': [probes[-1]['id']],
                    'summary': 'return path with no imports'}
        bundle = document['bundle']
        return {'kind': 'review', 'verdict': 'pass', 'bundle_sha256': bundle['sha256'],
                'checks': list(document['required_checks']), 'unknowns': [],
                'reason': 'raw-backed original return'}

    def _stage(self, spec, directory, phase, action):
        root = Path(spec['directory'])
        before = _snapshot(root)
        for name in spec['required_text_files']:
            (root / name).read_text(encoding='utf-8')
        if phase == 'smoke':
            smoke = root / spec['smoke_file']
            smoke.parent.mkdir(parents=True, exist_ok=True)
            smoke.write_text('READY-SCOPE', encoding='utf-8')
        else:
            target = root / 'packet' / 'action.json'
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text(json.dumps(action), encoding='utf-8')
        after = _snapshot(root)
        changed = sorted(name for name in set(before) | set(after) if before.get(name) != after.get(name))
        out_of_scope = [name for name in changed if name not in spec['allowed_files']]
        receipt = {
            'schema': 'cmre-provider-worker/1', 'owner': spec['owner'],
            'functions': spec.get('functions', []), 'model_requested': spec['model'],
            'parent_receipt': spec.get('parent_receipt'),
            'client_version': spec.get('client_version'),
            'started': 1.0, 'ended': 2.0, 'elapsed_seconds': 1.0,
            'state': 'completed', 'controller_pid': os.getpid(),
            'controller_identity': 'fake', 'source_before': {}, 'source_after': {},
            'session_ids': ['ses_fake'], 'calls': [
                {'call_id': f'call-{phase}', 'model': spec['model'],
                 'input_tokens': 10, 'output_tokens': 5, 'reasoning_tokens': None,
                 'cache_read_tokens': 0, 'cache_write_tokens': 0, 'cost': 0.01,
                 'finish': 'stop', 'usage_observed': True}],
            'steps': 3, 'finished': True, 'provider_errors': [],
            'changed_files': changed, 'out_of_scope': out_of_scope, 'pin_drift': [],
            'parse_errors': [], 'exit_code': 0, 'models_observed': [spec['model']],
        }
        if phase == 'smoke':
            receipt['readiness'] = {'identity': {'schema': 'cmre-provider-readiness/1'},
                                    'passed': True, 'missing_reads': [],
                                    'write_proven': True, 'errors': []}
        (directory / 'receipt.json').write_text(json.dumps(receipt, indent=2), encoding='utf-8')
        return receipt


class FakeCollector:
    def __init__(self):
        self.run_directories = []

    def __call__(self, run_dir):
        self.run_directories.append(str(run_dir))
        record = json.loads((Path(run_dir) / 'receipt.json').read_text(encoding='utf-8'))
        result = {'schema': 'cmre-provider-collection/1', 'run_state': record['state'],
                  'run_directory': str(run_dir), 'usage_status': 'collected',
                  'calls': record.get('calls'), 'models_observed': record.get('models_observed'),
                  'artifacts': {}, 'recovered_export': False,
                  'export_path': str(Path(run_dir) / 'session-export.json')}
        versions = Path(run_dir) / 'collections' / '0001'
        versions.mkdir(parents=True, exist_ok=True)
        (versions / 'receipt.json').write_text(json.dumps(result, indent=2), encoding='utf-8')
        return result


def _broker(config, action, output, *, prior=None, candidate=None):
    assert action['kind'] == 'probe'
    output = Path(output)
    output.mkdir(parents=True, exist_ok=False)
    artifacts = output / 'artifacts'
    artifacts.mkdir()
    fixture = {'regs': {}, 'stack': []}
    fixture_path = artifacts / 'fixture.json'
    fixture_path.write_text(json.dumps(fixture), encoding='utf-8')
    raw = {
        'outcome': 'return', 'covered_pcs': ['0x4d16ea'], 'import_count': 0,
        'execution_identity': {'target': 'pinned'},
        'instructions': [{'va': '0x4d1600', 'mnemonic': 'mov'},
                         {'va': '0x4d16ea', 'mnemonic': 'ret'}],
        'arrivals': [{'pc': '0x4d1600'}], 'returns': [{'pc': '0x4d16ea'}],
    }
    raw_path = artifacts / 'raw.json.gz'
    raw_path.write_bytes(gzip.compress(json.dumps(raw).encode('utf-8')))
    summary = {'outcome': 'return', 'covered_pcs': ['0x4d16ea'], 'coverage': 1.0,
               'import_count': 0, 'execution_identity': {'target': 'pinned'},
               'returns': [{'pc': '0x4d16ea'}]}
    summary_path = artifacts / 'summary.json'
    summary_path.write_text(json.dumps(summary), encoding='utf-8')
    pins = {path.relative_to(output).as_posix(): _pin(path)
            for path in (fixture_path, raw_path, summary_path)}
    return {'kind': 'probe', 'ok': True,
            'result': {'function': '0x4d1600', 'summary': summary,
                       'provenance': {'target': 'original', 'source': 'pinned-original-image'}},
            'artifacts': pins}


def _profile(root, model):
    executable = root / 'opencode'
    executable.parent.mkdir(parents=True, exist_ok=True)
    executable.write_bytes(b'fake-client')
    return {'executable': str(executable), 'model': model,
            'client_version': '1.18.3', 'environment': {'OPENCODE_API_KEY': SECRET}}


def test_owner_input_probe_propose_independent_review_and_ready(tmp_path, monkeypatch):
    analysis = tmp_path / 'analysis'
    analysis.mkdir()
    monkeypatch.setattr(env, 'ANALYSIS', analysis)
    monkeypatch.setattr(env, 'C_DIR', tmp_path / 'c')
    (tmp_path / 'c').mkdir()

    seed = tmp_path / 'seed.json'
    seed.write_text(json.dumps({'regs': {}, 'stack': []}), encoding='utf-8')
    manifest = {
        'schema': 'cmre-provider-campaign/1', 'duration_seconds': 600,
        'concurrency': 1,
        'roles': {'deepseek': {'model': 'deepseek/model'},
                  'minimax': {'model': 'minimax/model'}},
        'jobs': [{'id': 'compute', 'config': {
            'function': '0x004d1600', 'mode': 'input',
            'goal': 'produce a raw-backed original return witness',
            'owner': 'deepseek', 'reviewer': 'minimax',
            'allowed_functions': ['0x004d1600'],
            'seed_fixture': _pin(seed), 'required_pcs': ['0x004d16ea'],
            'min_coverage': 0.0}}],
    }
    campaign_dir = analysis / 'campaign'
    prepared = prepare_manifest(campaign_dir, manifest)
    identity = {'test': 'identity'}
    campaign.initialize(campaign_dir, prepared, identity)

    parent = tmp_path / 'parent-receipt.json'
    parent.write_text(json.dumps({'schema': 'cmre-provider-parent/1'}), encoding='utf-8')
    profiles = {'deepseek': _profile(tmp_path / 'deepseek', 'deepseek/model'),
                'minimax': _profile(tmp_path / 'minimax', 'minimax/model')}
    runner = ScriptedRunner()
    collector = FakeCollector()
    actor = OpenCodeActor(profiles, parent, runner=runner, collector=collector)
    executor = Executor(campaign_dir, broker=_broker)
    monkeypatch.setattr(executor, '_execution', lambda: {'target': 'pinned'})

    result = campaign.run(campaign_dir, actor=actor, execute=executor,
                          identity=identity, clock=lambda: 100.0,
                          sleep=lambda seconds: None, once=False)
    state = provider_jobs.load(campaign_dir)
    job = state['jobs']['compute']
    assert result['counts']['ready'] == 1, json.dumps(job, indent=2)
    assert job['state'] == 'ready'
    assert job['author_model'] == 'deepseek/model'
    assert job['review_model'] == 'minimax/model'
    assert job['config']['mode'] == 'input'
    assert job['report']['final_state'] == 'ready'

    records = {}
    for action_id in ('a0001', 'a0002', 'a0003'):
        records[action_id] = json.loads(
            (campaign_dir / 'jobs' / 'compute' / f'{action_id}.json').read_text())
    assert records['a0001']['role'] == 'deepseek'
    assert records['a0001']['action']['kind'] == 'probe'
    assert records['a0002']['action']['kind'] == 'bundle'
    assert records['a0003']['role'] == 'minimax'
    assert records['a0001']['receipt']['stages']['work']['calls']
    assert [item['stage'] for item in records['a0001']['receipt']['collections']] == \
        ['smoke', 'work']

    bundle_result = records['a0002']['response']
    assert bundle_result['ready'] is True
    bundle_path = Path(bundle_result['pin']['path'])
    payload = json.loads(bundle_path.read_text())
    assert payload['required_checks'] == bundle_result['required_checks']
    assert payload['function'] == '0x004d1600'
    assert payload['fixtures'][0]['data'] == {'regs': {}, 'stack': []}
    raw_pin = payload['raw'][0]['pin']
    assert hashlib.sha256(Path(raw_pin['path']).read_bytes()).hexdigest() == raw_pin['sha256']
    raw = json.loads(gzip.decompress(Path(raw_pin['path']).read_bytes()))
    assert raw['outcome'] == 'return'
    assert raw['covered_pcs'] == ['0x4d16ea']
    assert payload['observations'][0]['summary']['import_count'] == 0

    # The review stage saw the actual structured core context, not only a
    # legacy "existing" blob, and it saw the broker FILE IDs.
    review_document = runner.documents[-1]
    assert review_document['context']['phase'] == 'review'
    assert review_document['context']['results'][-1]['id'] == 'a0002'
    assert 'a0001/artifacts/raw.json.gz' in review_document['artifact_ids']
    assert review_document['required_checks'] == payload['required_checks']
    assert SECRET not in json.dumps(review_document)


def test_revise_reopens_owner_then_independent_pass_reaches_ready(tmp_path, monkeypatch):
    analysis = tmp_path / 'analysis'
    analysis.mkdir()
    monkeypatch.setattr(env, 'ANALYSIS', analysis)
    monkeypatch.setattr(env, 'C_DIR', tmp_path / 'c')
    (tmp_path / 'c').mkdir()
    seed = tmp_path / 'seed.json'
    seed.write_text('{}', encoding='utf-8')
    manifest = {
        'schema': 'cmre-provider-campaign/1', 'duration_seconds': 600,
        'concurrency': 1,
        'roles': {'deepseek': {'model': 'deepseek/model'},
                  'minimax': {'model': 'minimax/model'}},
        'jobs': [{'id': 'compute', 'config': {
            'function': '0x004d1600', 'mode': 'input',
            'goal': 'produce a raw-backed original return witness',
            'owner': 'deepseek', 'reviewer': 'minimax',
            'allowed_functions': ['0x004d1600'],
            'seed_fixture': _pin(seed), 'required_pcs': ['0x004d16ea']}}],
    }
    campaign_dir = analysis / 'campaign'
    campaign.initialize(campaign_dir, prepare_manifest(campaign_dir, manifest),
                        {'test': 'identity'})
    parent = tmp_path / 'parent.json'
    parent.write_text('{}')
    profiles = {'deepseek': _profile(tmp_path / 'deepseek', 'deepseek/model'),
                'minimax': _profile(tmp_path / 'minimax', 'minimax/model')}

    class ReviseOnceRunner(ScriptedRunner):
        def __init__(self):
            super().__init__()
            self.reviews = 0

        def _action(self, number, document):
            if document['phase'] == 'review':
                self.reviews += 1
                if self.reviews == 1:
                    bundle = document['bundle']
                    return {'kind': 'review', 'verdict': 'revise',
                            'bundle_sha256': bundle['sha256'],
                            'checks': list(document['required_checks']),
                            'unknowns': ['retained raw needs another arrival witness'],
                            'reason': 'unresolved evidence'}
            return super()._action(number, document)

    runner = ReviseOnceRunner()
    actor = OpenCodeActor(profiles, parent, runner=runner, collector=FakeCollector())
    executor = Executor(campaign_dir, broker=_broker)
    monkeypatch.setattr(executor, '_execution', lambda: {'target': 'pinned'})
    campaign.run(campaign_dir, actor=actor, execute=executor,
                 identity={'test': 'identity'}, clock=lambda: 100.0,
                 sleep=lambda seconds: None, once=False)
    state = provider_jobs.load(campaign_dir)
    job = state['jobs']['compute']
    assert job['state'] == 'ready'
    assert runner.reviews == 2
    assert len(runner.documents) >= 4
