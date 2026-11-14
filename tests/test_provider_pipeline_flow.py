"""Production scheduling regressions: supply, fairness and isolated capacity."""
from threading import Event
from pathlib import Path
from time import sleep

from cmre import pipeline, provider_campaign as pc
import pytest


def test_held_inventory_cannot_size_an_author_pool():
    state = pipeline.build(
        [(0x1000, 12000, False, True), (0x2000, 3000, False, True)],
        {'fn_00001000': {'state': 'evidence-needed', 'blocker': 'x87-live-stack'}},
        set(), {0x3000: 90000})
    plan = pipeline.plan(state, 10000, 180)
    assert plan['selected_bytes'] == 3000
    assert plan['shortfall_bytes'] == 7000
    assert state.bands['lifted'].body_bytes == 15000


def test_dispatch_prioritizes_review_and_rotates_owner_jobs():
    jobs = [
        {'id': 'old', 'state': 'prepared', 'config': {'owner': 'deepseek'},
         'history': [1, 2, 3]},
        {'id': 'new', 'state': 'prepared', 'config': {'owner': 'deepseek'}, 'history': []},
        {'id': 'review', 'state': 'needs-review', 'config': {'reviewer': 'deepseek'}},
    ]
    assert pc._dispatch_batch(jobs, 1)[0]['id'] == 'review'
    assert pc._dispatch_batch(jobs[:2], 1)[0]['id'] == 'new'


def test_pooled_dispatch_respects_each_provider_capacity():
    jobs = [{'id': str(i), 'state': 'prepared', 'config': {'owner': role}}
            for i, role in enumerate(['deepseek'] * 4 + ['minimax'] * 4)]
    chosen = pc._dispatch_batch(jobs, 6, role_capacity={'deepseek': 2, 'minimax': 3})
    assert len(chosen) == 5
    assert sum(j['config']['owner'] == 'deepseek' for j in chosen) == 2


def test_fast_worker_refills_before_slow_worker_finishes(tmp_path, monkeypatch):
    manifest = {'schema': pc.SCHEMA, 'duration_seconds': 30, 'concurrency': 2,
                'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
                'jobs': [{'id': name, 'config': {'function': hex(0x401000+i*16),
                         'mode': 'input', 'owner': role, 'goal': name}}
                         for i, (name, role) in enumerate([
                             ('slow', 'deepseek'), ('fast', 'minimax'), ('next', 'minimax')])]}
    pc.initialize(tmp_path / 'run', manifest, {})
    slow_started, refilled = Event(), Event()

    def step(directory, job, *args):
        if job['id'] == 'slow':
            slow_started.set()
            assert refilled.wait(3), 'fast slot sat idle behind slow stage'
        elif job['id'] == 'fast':
            assert slow_started.wait(3)
        else:
            refilled.set()
        # Real workers contend on the state lock; retries must yield to its owner.
        pc._update(directory, job, {'state': 'held'}, sleep)
        return True

    monkeypatch.setattr(pc, '_step_job', step)
    result = pc.run(tmp_path / 'run', actor=lambda: None, execute=lambda: None)
    assert refilled.is_set()
    assert result['executed'] == 3


def test_owner_work_in_progress_is_bounded():
    jobs = [{'id': str(i), 'state': 'prepared', 'config': {'owner': 'deepseek'},
             'history': [1]} for i in range(3)]
    jobs += [{'id': 'fresh', 'state': 'prepared', 'config': {'owner': 'deepseek'}}]
    assert pc._dispatch_batch(jobs, 1, owner_wip={'deepseek': 3})[0]['id'] != 'fresh'
    assert pc._dispatch_batch(jobs, 0) == []


def test_pool_rejects_shared_state_and_returns_a_failed_lease(tmp_path):
    from cmre.provider_pool import ActorPool, XDG, validate
    pairs = [{role: {'model': role, 'environment': {
        key: str(tmp_path / str(slot) / role / key) for key in XDG}}
              for role in ('deepseek', 'minimax')} for slot in range(2)]
    assert len(validate(pairs)) == 2
    import copy
    duplicate = copy.deepcopy(pairs)
    duplicate[1]['deepseek']['environment'] = duplicate[0]['deepseek']['environment']
    with pytest.raises(ValueError, match='disjoint'):
        validate(duplicate)

    class FailedActor:
        def __init__(self, *_): pass
        def __call__(self, *_): raise RuntimeError('retained failure')

    pool = ActorPool(pairs, 'parent', factory=FailedActor)
    for _ in range(3):
        with pytest.raises(RuntimeError, match='retained failure'):
            pool({}, 'deepseek', {}, 'stage')


def test_replenishment_spreads_blockers_and_preserves_stage_buffers():
    from cmre.pipeline_flow import classify, select
    assert classify(lifted=True, source=False, record={'state': 'evidence-needed'}) == 'intake'
    assert classify(lifted=True, source=True, record={}) == 'reconcile'
    rows = [{'function': f'fn_{i:08x}', 'stage': stage, 'blocker': blocker, 'body_bytes': size}
            for i, (stage, blocker, size) in enumerate([
                ('intake', 'x87', 4000), ('intake', 'x87', 3000),
                ('intake', 'files', 2000), ('author', 'none', 2500),
                ('repair', 'wrong-math', 1000)])]
    picked = select(rows, limit=4, buffer_bytes=10000)
    assert {r['stage'] for r in picked} == {'intake', 'author', 'repair'}
    assert {r['blocker'] for r in picked if r['stage'] == 'intake'} == {'x87', 'files'}
    many = [dict(rows[3], function=f'author{i}', blocker=f'author{i}') for i in range(4)]
    many += [rows[0], rows[4]]
    picked = select(many, limit=3, buffer_bytes=10000)
    assert {r['stage'] for r in picked} == {'intake', 'author', 'repair'}


def test_cli_requires_isolation_for_wider_capacity():
    from scripts.cmre_provider_campaign import _check_worker_cap
    manifest = {'concurrency': 8, 'role_capacity': {'deepseek': 4, 'minimax': 4}}
    with pytest.raises(ValueError):
        _check_worker_cap(manifest)
    _check_worker_cap(manifest, [{}, {}, {}, {}])
    with pytest.raises(ValueError):
        _check_worker_cap(manifest, [{}])


def test_replenish_uses_original_seed_and_never_c_dispatches_held_source(tmp_path, monkeypatch):
    from cmre import env, pipeline_flow as flow
    import json
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    hint = tmp_path / 'hint.json'
    fixture = {'regs': {'ecx': 123}, 'stack': [4], 'arena_writes': {}, 'globals': {}}
    hint.write_text(json.dumps({'fixtures': [fixture]}))
    source = tmp_path / 'candidate.c'
    source.write_text('int cm_fn_00401000(void) { return 0; }')
    row = {'function': 'fn_00401000', 'va': '0x00401000', 'body_bytes': 4000,
           'stage': 'intake', 'state': 'evidence-needed', 'blocker': 'x87',
           'reason': 'old transport hold', 'retry_when': 'named proof',
           'hints': str(hint), 'source': str(source), 'dispatch_hold': {'reason': 'held'}}

    def evidence(va, path):
        path.parent.mkdir(parents=True)
        path.write_text(f'original {va:x}')
        return flow.pin(path)

    report = flow.prepare(tmp_path / 'prep', [row],
        {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}}, evidence=evidence)
    manifest = json.loads(Path(report['manifest']).read_text())
    config = manifest['jobs'][0]['config']
    assert config['mode'] == 'input'
    assert json.loads(Path(config['seed_fixture']['path']).read_text()) == fixture
    assert config['context_files'][0]['sha256']
    assert source.read_text() == 'int cm_fn_00401000(void) { return 0; }'
    with pytest.raises(ValueError, match='fresh'):
        flow.prepare(tmp_path / 'prep', [row], {}, evidence=evidence)


def test_adoption_queue_does_not_embed_large_evidence(tmp_path):
    from cmre.pipeline_flow import inbox
    directory = tmp_path / 'run'
    manifest = {'schema': pc.SCHEMA, 'duration_seconds': 30, 'concurrency': 2,
                'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
                'jobs': [{'id': 'input', 'config': {'function': '0x401000',
                          'mode': 'input', 'goal': 'witness'}}]}
    pc.initialize(directory, manifest, {})
    from cmre import provider_jobs
    job = provider_jobs.load(directory)['jobs']['input']
    pc._update(directory, job, {'state': 'ready', 'bundle': {
        'sha256': 'a'*64, 'pin': {'path': 'retained'},
        'payload': {'raw': 'x'*100000}}}, lambda _: None)
    report = inbox(directory)
    assert report['counts']['adoption'] == 1
    assert 'payload' not in report['queues']['adoption'][0]['bundle']


@pytest.mark.parametrize('mode,kind,ready,expected,controls', [
    ('c', 'test', True, ['test', 'build', 'bundle'], False),
    ('input', 'probe', True, ['probe', 'bundle'], False),
    ('input', 'probe', False, ['probe', 'bundle'], False),
    ('c', 'test', False, ['test', 'build', 'bundle'], True),
    ('c', 'probe', False, ['probe', 'bundle'], True),
])
def test_supervisor_freezes_without_a_paid_propose_turn(tmp_path, mode, kind, ready, expected, controls):
    from cmre import provider_jobs
    manifest = {'schema': pc.SCHEMA, 'duration_seconds': 30,
                'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
                'jobs': [{'id': 'candidate', 'config': {'function': '0x401000',
                    'mode': mode, 'goal': 'evidence', 'auto_bundle': True,
                    'require_controls': controls}}]}
    directory = tmp_path / 'run'
    pc.initialize(directory, manifest, {})
    job = provider_jobs.load(directory)['jobs']['candidate']
    job = pc._update(directory, job, {'action_count': 1, 'round_actions': 1}, lambda _: None)
    step = {'action_id': 'a0001', 'action': {'kind': kind}, 'phase': 'owner',
            'role': job['config']['owner'], 'model': 'm' if mode == 'c' else 'd',
            'output_rel': 'jobs/candidate/rounds/1/a0001-output'}
    actions = []

    def execute(job, action, output, *, prior):
        actions.append(action['kind'])
        if action['kind'] == 'bundle':
            assert action['artifacts'] == [r['id'] for r in prior]
            return {'ready': ready, 'sha256': 'a'*64, 'required_checks': ['full-gate'],
                    'blocker': 'controls-required' if controls and kind == 'test' else None,
                    'feedback': 'original returning witness still missing'}
        return {'kind': action['kind'], 'ok': True, 'result': {}}

    pc._owner_action(directory, job, step, execute, lambda _: None, {})
    after = provider_jobs.load(directory)['jobs']['candidate']
    assert actions == expected
    control_hold = controls and kind == 'test'
    assert after['state'] == ('held' if control_hold else 'needs-review' if ready else 'prepared')
    assert len(after['results']) == len(expected)
    assert after['state'] != 'accepted'
    if control_hold:
        from cmre.pipeline_flow import inbox
        assert inbox(directory)['counts']['controls'] == 1
    elif not ready:
        assert 'original returning witness still missing' in after['feedback']


@pytest.mark.parametrize('failed_kind', ['build', 'bundle'])
def test_automatic_execution_failure_retains_action_provenance(tmp_path, failed_kind):
    from cmre import provider_jobs
    manifest = {'schema': pc.SCHEMA, 'duration_seconds': 30,
                'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
                'jobs': [{'id': 'candidate', 'config': {'function': '0x401000',
                    'mode': 'c', 'goal': 'evidence', 'auto_bundle': True}}]}
    directory = tmp_path / 'run'
    pc.initialize(directory, manifest, {})
    job = provider_jobs.load(directory)['jobs']['candidate']
    job = pc._update(directory, job, {'action_count': 1, 'round_actions': 1}, lambda _: None)
    step = {'action_id': 'a0001', 'action': {'kind': 'test'}, 'phase': 'owner',
            'role': 'minimax', 'model': 'm',
            'output_rel': 'jobs/candidate/rounds/1/a0001-output'}

    def execute(job, action, output, *, prior):
        if action['kind'] == failed_kind:
            raise RuntimeError('exact retained automatic failure')
        return {'kind': action['kind'], 'ok': True}

    pc._owner_action(directory, job, step, execute, lambda _: None, {})
    job = provider_jobs.load(directory)['jobs']['candidate']
    assert job['state'] == 'held'
    refs = job['results']
    import json
    failed = json.loads((directory / refs[-1]['path']).read_text())
    assert failed['action']['kind'] == failed_kind
    assert failed['model'] == 'm' and not failed['ok']
    assert failed['output_path'].endswith('-output')
    assert 'exact retained automatic failure' in failed['error']
    assert refs[-1]['hash'] == failed['hash']


def test_repaired_source_bundle_excludes_earlier_successful_build(tmp_path):
    from cmre import provider_jobs
    manifest = {'schema': pc.SCHEMA, 'duration_seconds': 30,
                'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
                'jobs': [{'id': 'candidate', 'config': {'function': '0x401000',
                    'mode': 'c', 'goal': 'repaired source', 'auto_bundle': True}}]}
    directory = tmp_path / 'run'
    pc.initialize(directory, manifest, {})
    job = provider_jobs.load(directory)['jobs']['candidate']
    prior_step = {'action_id': 'a0001', 'action': {'kind': 'build'}, 'phase': 'owner',
                  'role': 'minimax', 'model': 'm', 'output_rel': 'old-source-build'}
    _, ref = pc._write_record(directory, job, prior_step, {'kind': 'build', 'ok': True})
    job = pc._update(directory, job, {'action_count': 1, 'results': [ref]}, lambda _: None)
    received = []

    def execute(job, action, output, *, prior):
        if action['kind'] == 'bundle':
            received.extend(action['artifacts'])
            return {'ready': True, 'sha256': 'a'*64, 'required_checks': ['source']}
        return {'kind': action['kind'], 'ok': True}

    pc._auto_bundle(directory, job, {'action': {'kind': 'test'}, 'role': 'minimax',
                                   'model': 'm'}, execute, lambda _: None, {})
    assert received == ['a0002']
    assert (directory / 'jobs/candidate/a0001.json').is_file()
    assert provider_jobs.load(directory)['jobs']['candidate']['state'] == 'needs-review'


def test_bounded_live_trial_stops_dispatch_and_drains(tmp_path):
    manifest = {'schema': pc.SCHEMA, 'duration_seconds': 30, 'concurrency': 2,
                'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
                'jobs': [{'id': f'job{i}', 'config': {'function': hex(0x401000+i*16),
                         'mode': 'input', 'owner': role, 'goal': 'bounded trial'}}
                        for i, role in enumerate(['deepseek', 'minimax', 'deepseek'])]}
    directory = tmp_path / 'run'
    pc.initialize(directory, manifest, {})

    def actor(job, role, context, stage):
        return {'model': 'd' if role == 'deepseek' else 'm', 'receipt': {'role': role},
                'action': {'kind': 'hold', 'reason': 'bounded result'}}

    result = pc.run(directory, actor=actor, execute=lambda *_: None, max_steps=2)
    assert result['executed'] == 2 and result['stopped'] == 'step-limit'
    assert pc.status(directory)['counts']['running'] == 0
    assert pc.status(directory)['counts']['prepared'] == 1


def test_adoption_ack_requires_resolved_hold_and_preserves_receipt(tmp_path, monkeypatch):
    import json
    from cmre import env, ledger, provider_jobs, work
    from cmre.pipeline_flow import acknowledge, inbox, pin
    directory = tmp_path / 'run'
    manifest = {'schema': pc.SCHEMA, 'duration_seconds': 30,
                'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
                'jobs': [{'id': 'input', 'config': {'function': '0x401000',
                          'mode': 'input', 'goal': 'witness'}}]}
    pc.initialize(directory, manifest, {})
    job = provider_jobs.load(directory)['jobs']['input']
    pc._update(directory, job, {'state': 'ready', 'bundle': {'sha256': 'a'*64}}, lambda _: None)
    receipt = tmp_path / 'adoption.json'
    receipt.write_text('{}')
    record = tmp_path / 'ledger.json'
    record.write_text(json.dumps({'lifted': {'validation': 'retained'}}))
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path)
    hint = tmp_path / 'fn_00401000.json'
    hint.write_text('{}')
    monkeypatch.setattr(ledger, 'record_path', lambda _: record)
    monkeypatch.setattr(work, 'read', lambda: {'functions': {}})
    monkeypatch.setattr(work, 'dispatch_hold', lambda *_: {'reason': 'unresolved'})
    with pytest.raises(ValueError, match='Resolve'):
        acknowledge(directory, 'input', receipt)
    monkeypatch.setattr(work, 'dispatch_hold', lambda *_: None)
    with pytest.raises(ValueError, match='identify'):
        acknowledge(directory, 'input', receipt)
    payload = {'schema': 'cmre-pipeline-adoption/1', 'function': '0x402000',
               'bundle_sha256': 'a'*64, 'canonical_hints': pin(hint), 'native_gate': pin(record)}
    receipt.write_text(json.dumps(payload))
    with pytest.raises(ValueError, match='identify'):
        acknowledge(directory, 'input', receipt)
    payload['function'] = '0x401000'
    receipt.write_text(json.dumps(payload))
    assert acknowledge(directory, 'input', receipt)['adoption_receipt']['sha256']
    assert inbox(directory)['counts']['adopted'] == 1
    acknowledge(directory, 'input', receipt)
    receipt.write_text('{}')
    with pytest.raises(ValueError, match='differs'):
        acknowledge(directory, 'input', receipt)


def test_native_packet_includes_every_indexed_caller_and_small_callee(tmp_path, monkeypatch):
    from types import SimpleNamespace as S
    from cmre import inventory, lift
    from cmre.pipeline_flow import native_packet
    va, callee = 0x401000, 0x402000
    callers = [0x403000+i*16 for i in range(10)]

    def instruction(pc, target=None):
        return S(va=pc, raw=b'\x90', mnemonic='call' if target else 'nop',
                 op_str=hex(target) if target else '', kind='call' if target else 'plain', target=target)

    decoded = {va: S(insns={va: instruction(va, callee)}, callees={callee}, problems=[]),
               callee: S(insns={callee: instruction(callee)}, callees=set(), problems=[])}
    decoded.update({pc: S(insns={pc: instruction(pc, va)}, callees={va}, problems=[]) for pc in callers})
    monkeypatch.setattr(inventory, 'load', lambda: S(
        functions={pc: object() for pc in decoded}, callees={pc: [va] for pc in callers}))
    monkeypatch.setattr(lift, 'discover', lambda pc: decoded[pc])
    path = tmp_path / 'native.txt'
    native_packet(va, path)
    text = path.read_text()
    for pc in callers:
        assert f'Indexed caller {pc:#010x}:' in text
    assert f'Callee {callee:#010x}: complete decode' in text


def test_separate_pools_cannot_open_the_same_client_tree(tmp_path):
    from concurrent.futures import ThreadPoolExecutor
    from cmre.coordination import BusyError
    from cmre.provider_pool import ActorPool, XDG
    pair = {role: {'model': role, 'environment': {
        key: str(tmp_path / role / key) for key in XDG}}
        for role in ('deepseek', 'minimax')}
    entered, release = Event(), Event()

    class BlockingActor:
        def __init__(self, *_): pass
        def __call__(self, *_):
            entered.set()
            assert release.wait(3)
            return 'done'

    first = ActorPool([pair], 'parent', factory=BlockingActor)
    second = ActorPool([pair], 'parent', factory=BlockingActor)
    with ThreadPoolExecutor(max_workers=1) as workers:
        future = workers.submit(first, {}, 'deepseek', {}, 'stage')
        try:
            assert entered.wait(3)
            with pytest.raises(BusyError):
                second({}, 'deepseek', {}, 'other-stage')
        finally:
            release.set()
        assert future.result() == 'done'


def test_reviewed_helper_input_can_advance_without_waiving_source_controls():
    from cmre.pipeline_flow import needs_helper_intake
    identity = {'target': 'pinned', 'harness': 'frozen', 'hints': 'reviewed-fixtures'}
    assert needs_helper_intake({}, identity)
    record = {'state': 'ready', 'reason': 'Reviewed site/value witness adopted', 'identity': identity}
    assert not needs_helper_intake(record, identity)
    assert needs_helper_intake(record, dict(identity, hints='changed'))
    assert needs_helper_intake(dict(record, state='evidence-needed'), identity)


def test_replenishment_observes_live_claims_and_frozen_submissions():
    from cmre.pipeline_flow import task_queues
    rows = [
        {'va': '0x401000', 'task_id': 'review', 'status': 'submitted'},
        {'va': '0x402000', 'task_id': 'active', 'status': 'claimed',
         'lease': {'expires_at': 200, 'token': 'never-return-this'}},
        {'va': '0x403000', 'task_id': 'expired', 'status': 'claimed', 'lease': {'expires_at': 1}},
    ]
    result = task_queues(rows, 100)
    assert result == {0x401000: {'stage': 'review', 'task_id': 'review'},
                      0x402000: {'stage': 'owned', 'task_id': 'active'}}


def test_generated_seed_passes_the_native_fixture_contract(tmp_path, monkeypatch):
    import json
    from cmre import difftest, env, pipeline_flow as flow
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path / 'hints')

    def evidence(va, path):
        path.parent.mkdir(parents=True)
        path.write_text(f'original {va:x}')
        return flow.pin(path)

    row = {'function': 'fn_00401000', 'va': '0x00401000', 'body_bytes': 4000,
           'stage': 'intake', 'state': 'unrecorded', 'blocker': 'unknown',
           'reason': '', 'retry_when': '', 'hints': None, 'source': None, 'dispatch_hold': None}
    report = flow.prepare(tmp_path / 'prep', [row],
        {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}}, evidence=evidence)
    manifest = json.loads(Path(report['manifest']).read_text())
    fixture = json.loads(Path(manifest['jobs'][0]['config']['seed_fixture']['path']).read_text())
    difftest.validate_hints({'fixtures': [fixture], 'arg_count': len(fixture['stack'])}, worker=True)
    from cmre import provider_probe
    patched = provider_probe.apply_patch(fixture, {'arena_writes': {'0x4': 123}})
    provider_probe.validate_global_writes(patched, None)
    assert patched['arena_writes']['0x4'] == 123


def test_broker_seed_preserves_legacy_hex_addresses(tmp_path, monkeypatch):
    from types import SimpleNamespace as S
    from cmre import difftest, emu, provider_probe
    from cmre.pipeline_flow import broker_seed
    fixture = {'regs': dict.fromkeys(emu.REG_NAMES, 0), 'stack': [], 'arena_template': 0,
               'arena_writes': {'100': 17, 'abcd0': 42},
               'globals': {'401000': 23}, 'import_seed': 0}
    # Larger arena uses the existing core capacity, not a changed VM model.
    templates = [bytes(0x100000)] * 8
    image = S(section_at=lambda _: S(start=0x400000, end=0x500000, writable=True, executable=False))
    monkeypatch.setattr(difftest.image, 'primary', lambda: image)
    normalized = broker_seed(fixture)
    assert '100' in fixture['arena_writes']  # canonical source not mutated
    assert normalized['arena_writes']['0x100'] == 17
    assert difftest._fixture_case(fixture, templates, 0) == difftest._fixture_case(normalized, templates, 0)
    patched = provider_probe.apply_patch(normalized, {'arena_writes': {'0x4': 1}})
    provider_probe.validate_global_writes(patched, image)
    assert patched['globals']['0x401000'] == 23


def test_oversized_prepared_packet_stays_out_of_paid_queue(tmp_path, monkeypatch):
    import json
    from cmre import env, pipeline_flow as flow
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path / 'hints')

    def evidence(va, path):
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text('original instruction\n' * (30000 if va == 0x401000 else 1))
        return flow.pin(path)

    rows = [{'function': f'fn_{va:08x}', 'va': hex(va), 'body_bytes': 4000,
             'stage': 'intake', 'state': 'unrecorded', 'blocker': 'unknown',
             'reason': '', 'retry_when': '', 'hints': None, 'source': None,
             'dispatch_hold': None} for va in (0x401000, 0x401010)]
    report = flow.prepare(tmp_path / 'prep', rows,
        {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}}, evidence=evidence)
    manifest = json.loads(Path(report['manifest']).read_text())
    assert report['dispatch_jobs'] == 1 and report['packet_held_jobs'] == 1
    assert manifest['jobs'][0]['config']['function'] == '0x401010'
    assert (tmp_path / 'prep/native/fn_00401000.txt').read_text().count('original instruction') == 30000


def test_c_publications_queue_while_evidence_work_continues(tmp_path):
    from concurrent.futures import ThreadPoolExecutor
    from cmre.coordination import lock
    from cmre.provider_runtime import Executor
    entered, release, second_called, second_started = Event(), Event(), Event(), Event()
    executor = Executor(tmp_path, broker=lambda *_a, **_k: {'kind': 'evidence', 'ok': True, 'artifacts': {}})
    original_config = executor._config

    def config(job):
        if job['id'] == 'second':
            second_started.set()
        return original_config(job)

    def finalizer(job, *_args):
        if job['id'] == 'second':
            second_called.set()
        with lock(tmp_path / 'publication.lock'):
            entered.set()
            assert release.wait(3)
            return {'state': 'accepted'}

    executor._config = config
    executor._finalize = finalizer
    jobs = [{'id': name, 'config': {'directory': str(tmp_path / 'jobs' / name), 'mode': 'c'}}
            for name in ('first', 'second', 'probe')]
    with ThreadPoolExecutor(max_workers=2) as workers:
        first = workers.submit(executor, jobs[0], {'kind': 'finalize'}, str(tmp_path / 'jobs/first/out'))
        assert entered.wait(3)
        second = workers.submit(executor, jobs[1], {'kind': 'finalize'}, str(tmp_path / 'jobs/second/out'))
        try:
            assert second_started.wait(3)
            assert not second_called.wait(.1), 'publication raced instead of waiting its turn'
            assert executor(jobs[2], {'kind': 'evidence'}, str(tmp_path / 'jobs/probe/out'))['ok']
        finally:
            release.set()
        assert first.result()['state'] == second.result()['state'] == 'accepted'


def test_native_probe_honors_canonical_large_arena(tmp_path, monkeypatch):
    from types import SimpleNamespace as S
    from cmre import difftest, emu, provider_broker
    size = 1376256
    fixture = {'regs': dict.fromkeys(emu.REG_NAMES, 0), 'stack': [], 'arena_template': 0,
               'arena_writes': {'13e99c': 0x12345678}, 'globals': {}, 'import_seed': 0}
    with pytest.raises(ValueError, match='in-range'):
        difftest.validate_hints({'fixtures': [fixture], 'arg_count': 0}, worker=True)
    captured = {}

    class StopBeforeExecution(Exception): pass

    def machine(*args, **kwargs):
        captured.update(kwargs)
        raise StopBeforeExecution

    def make_case(*args):
        case = difftest._fixture_case(*args)
        assert len(case.arena) == size
        assert case.arena[0x13e99c:0x13e9a0] == bytes.fromhex('78563412')
        return case

    modules = {'difftest': S(Options=S(for_function=lambda *_a, **_k: S(hints={'arena_size': size})),
                            validate_hints=difftest.validate_hints, _fixture_case=make_case,
                            arena_templates=difftest.arena_templates),
               'emu': S(ARENA_SIZE=emu.ARENA_SIZE, arena_capacity=emu.arena_capacity, Machine=machine),
               'image': S(primary=lambda: object()),
               'lift': S(discover=lambda va: S(region_end=va+1)), 'recover': S()}
    backend = provider_broker.LocalBackend()
    monkeypatch.setattr(backend, '_verify_target', lambda: 'a'*64)
    monkeypatch.setattr(backend, '_module', lambda name: modules[name])
    monkeypatch.setattr(backend, '_harness', lambda name: S())
    with pytest.raises(StopBeforeExecution):
        backend.probe(0x401000, fixture, 100, {}, tmp_path / 'probe')
    assert captured['arena_size'] == size
