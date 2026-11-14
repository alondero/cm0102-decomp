"""Queue liveness, bounded scaling, durable promotion and fidelity boundaries."""
import copy
import json
from collections import Counter
from pathlib import Path
from threading import Event

import pytest

from cmre import provider_campaign as pc, provider_jobs as jobs
from cmre import pipeline_policy as policy


def manifest():
    return {'schema': pc.SCHEMA, 'duration_seconds': 60, 'concurrency': 2,
            'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
            'jobs': [{'id': 'initial', 'config': {'function': '0x00401000',
                     'mode': 'input', 'goal': 'original input'}}]}


def test_append_replays_without_changing_frozen_genesis_or_deadline(tmp_path):
    pc.initialize(tmp_path / 'run', manifest(), {})
    directory = tmp_path / 'run'
    genesis = (directory / 'genesis.json').read_bytes()
    state = jobs.load(directory)
    definition = {'id': 'next', 'config': dict(state['jobs']['initial']['config'],
                                             function='0x00402000')}
    assert jobs.enqueue(directory, definition)['state'] == 'prepared'
    revision = jobs.load(directory)['revision']
    jobs.enqueue(directory, definition)
    assert jobs.load(directory)['revision'] == revision
    assert (directory / 'genesis.json').read_bytes() == genesis
    assert set(jobs.load(directory)['jobs']) == {'initial', 'next'}
    with pytest.raises(jobs.InvalidChange, match='changed'):
        jobs.enqueue(directory, dict(definition, config={}))
    value = json.loads((directory / 'state.json').read_text())
    value['jobs']['next']['config']['goal'] = 'forged'
    (directory / 'state.json').write_text(json.dumps(value))
    with pytest.raises(jobs.CorruptStore):
        jobs.load(directory)


def test_live_policy_is_bounded_and_retains_each_revision(tmp_path):
    spec = manifest()
    first = policy.defaults(spec)
    policy.write(tmp_path, first, spec)
    scaled = copy.deepcopy(first)
    scaled['workers'].update(author=0, intake=2, adoption=4)
    policy.write(tmp_path, scaled, spec)
    assert policy.read(tmp_path, spec) == scaled
    assert len(list((tmp_path / 'policies').glob('*.json'))) == 2
    for stage, value in [('author', 3), ('publish', 2), ('prepare', -1)]:
        invalid = copy.deepcopy(first)
        invalid['workers'][stage] = value
        with pytest.raises(ValueError):
            policy.write(tmp_path, invalid, spec)
    assert policy.read(tmp_path, spec) == scaled


def test_stage_limits_leave_other_provider_work_runnable():
    candidates = [
        {'id': str(i), 'state': state, 'config': {'mode': mode, 'owner': role,
         'reviewer': role}, 'history': []}
        for i, (state, mode, role) in enumerate([
            ('prepared', 'c', 'minimax'), ('prepared', 'input', 'deepseek'),
            ('needs-review', 'input', 'minimax')])]
    chosen = pc._dispatch_batch(candidates, 2, stage_capacity={'author': 0,
                                'intake': 1, 'review': 1})
    assert [j['id'] for j in chosen] == ['2', '1']


def test_service_projection_keeps_cancelled_supply_out_of_author_queue():
    from cmre.pipeline_controller import Service
    service = object.__new__(Service)
    service.rows, service.futures, service.operations, service.shortages = [], {}, {}, {}
    service.clock = lambda: 100
    spec = manifest()
    state = {'jobs': {'unused': {'id': 'unused', 'state': 'cancelled',
             'config': {'mode': 'c', 'max_body_bytes': 99}}}}
    result = service.projection(state, policy.defaults(spec))
    assert result['queues'] == {'cancelled': 1}
    assert result['queue_candidate_bytes'] == {'cancelled': 99}


def test_scheduler_services_upstream_while_slow_actor_runs(tmp_path, monkeypatch):
    directory = tmp_path / 'run'
    pc.initialize(directory, manifest(), {})
    started, serviced = Event(), Event()

    def step(directory, job, *args):
        started.set()
        assert serviced.wait(3), 'supervisor waited for slow model before servicing queues'
        pc._update(directory, job, {'state': 'held'}, lambda _: None)
        return True

    def service(state, deadline):
        if started.is_set():
            serviced.set()
        return {'pending': False}

    monkeypatch.setattr(pc, '_step_job', step)
    result = pc.run(directory, actor=lambda: None, execute=lambda: None, service=service)
    assert result['executed'] == 1
    assert serviced.is_set()


def test_publication_spare_capacity_cannot_launch_extra_paid_actor():
    candidates = [{'id': str(i), 'state': 'prepared',
                   'config': {'owner': role, 'mode': 'input'}}
                  for i, role in enumerate(['deepseek', 'deepseek', 'minimax'])]
    chosen = pc._dispatch_batch(candidates, 3, role_capacity={'deepseek': 2, 'minimax': 2},
                                provider_capacity=2)
    assert len(chosen) == 2


def row(va, stage='intake', size=4000, blocker='unclassified'):
    return {'va': f'{va:#010x}', 'function': f'fn_{va:08x}', 'stage': stage,
            'body_bytes': size, 'blocker': blocker, 'dispatch_hold': None}


def test_held_packets_do_not_occupy_replenishment_watermarks():
    from cmre.pipeline_controller import supply_selection
    state = {'jobs': {}}
    settings = policy.defaults(manifest())
    rows = [row(0x401000+i*16) for i in range(20)]
    excluded = {(r['va'], 'intake') for r in rows[:16]}
    selected, shortage = supply_selection(rows, state, settings, excluded=excluded)
    assert {r['va'] for r in selected} == {r['va'] for r in rows[16:]}
    assert shortage['intake']['starved']


def test_input_adoption_releases_exactly_one_new_author_job():
    from cmre.pipeline_controller import supply_selection
    state = {'jobs': {'input': {'id': 'input', 'state': 'ready',
                'adoption_receipt': {'path': 'proof', 'sha256': 'pinned'},
                'config': {'function': '0x00401000', 'mode': 'input'}}}}
    selected, _ = supply_selection([row(0x401000, 'author')], state, policy.defaults(manifest()))
    assert len(selected) == 1
    state['jobs']['author'] = {'id': 'author', 'state': 'held',
        'config': {'function': '0x00401000', 'mode': 'c'}}
    selected, _ = supply_selection([row(0x401000, 'author')], state, policy.defaults(manifest()))
    assert selected == []


def test_append_rejects_duplicate_function_until_reviewed_adoption(tmp_path):
    directory = tmp_path / 'run'
    pc.initialize(directory, manifest(), {})
    definition = {'id': 'author', 'config': {'function': '0x00401000', 'mode': 'c', 'goal': 'author'}}
    with pytest.raises(pc.CampaignError, match='retained work'):
        pc.enqueue(directory, definition)
    job = jobs.load(directory)['jobs']['initial']
    jobs.update(directory, job['id'], {'state': 'ready', 'adoption_receipt': {'path': 'gate'}},
                expected_revision=job['revision'])
    assert pc.enqueue(directory, definition)['state'] == 'prepared'


def test_pipeline_promotes_reviewed_input_and_can_pause_only_authors(tmp_path, monkeypatch):
    from cmre import env, work
    from cmre.pipeline_controller import Service
    from cmre.pipeline_flow import pin
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    monkeypatch.setattr(env, 'C_DIR', tmp_path / 'canonical')
    monkeypatch.setattr(work, 'read', lambda: {'functions': {}})
    directory = tmp_path / 'run'
    spec = manifest()
    spec['pipeline'] = True
    spec['jobs'][0]['config'].update(auto_bundle=True, defer_publication=True, defer_execution=True,
                                   pipeline_stage='intake', max_body_bytes=4000)
    pc.initialize(directory, spec, {})
    settings = policy.defaults(spec)
    settings['workers']['author'] = 0
    policy.write(directory, settings, spec)
    promoted = Event()
    model_calls = []
    machine_calls = []

    def actor(job, role, context, stage):
        model_calls.append((job['config']['mode'], context['phase'], role))
        if context['phase'] == 'review':
            action = {'kind': 'review', 'verdict': 'pass', 'unknowns': [],
                      'bundle_sha256': context['bundle']['sha256'], 'checks': context['required_checks']}
        else:
            action = {'kind': 'test'} if job['config']['mode'] == 'c' else {'kind': 'probe', 'request': {}}
        return {'model': spec['roles'][role]['model'], 'action': action}

    def execute(job, action, output, **kwargs):
        machine_calls.append(action['kind'])
        if action['kind'] == 'bundle':
            return {'ready': True, 'sha256': 'exact-' + job['id'], 'required_checks': ['exact']}
        if action['kind'] == 'finalize':
            return {'state': 'accepted' if job['config']['mode'] == 'c' else 'ready'}
        return {'kind': action['kind'], 'ok': True, 'artifacts': {}}

    def adopt(directory, job_id, executor, output, **kwargs):
        current = jobs.load(directory)['jobs'][job_id]
        assert current['state'] == 'ready' and current['review']['verdict'] == 'pass'
        jobs.update(directory, job_id, {'adoption_receipt': {'path': 'native-gate'}},
                    expected_revision=current['revision'])
        promoted.set()
        return {'state': 'finished'}

    def snapshot():
        return [row(0x401000, 'author' if promoted.is_set() else 'intake')]

    def prepare(output, rows, profiles, **kwargs):
        output.mkdir(parents=True)
        seed = output / 'seed.json'
        seed.write_text('{}')
        role = kwargs['owner_override']
        value = manifest()
        value['jobs'] = [{'id': 'author', 'config': {
            'function': rows[0]['va'], 'goal': 'recover exact source', 'mode': 'c',
            'owner': role, 'reviewer': 'minimax' if role == 'deepseek' else 'deepseek',
            'seed_fixture': pin(seed), 'auto_bundle': True, 'defer_publication': True, 'defer_execution': True,
            'pipeline_stage': 'author', 'max_body_bytes': 4000}}]
        (output / 'manifest.json').write_text(json.dumps(value))
        return {'manifest': str(output / 'manifest.json')}

    options = dict(actor=actor, execute=execute,
                   limits=lambda state: policy.read(directory, state['manifest']))
    with Service(directory, {}, execute, snapshot=snapshot, prepare=prepare, adopt=adopt) as service:
        first = pc.run(directory, service=service, **options)
    assert promoted.is_set()
    assert not any(mode == 'c' for mode, phase, role in model_calls)
    assert any(j['config']['mode'] == 'c' for j in jobs.load(directory)['jobs'].values())
    settings['workers']['author'] = 1
    policy.write(directory, settings, spec)
    with Service(directory, {}, execute, snapshot=snapshot, prepare=prepare, adopt=adopt) as service:
        second = pc.run(directory, service=service, **options)
    assert first['deadline'] == second['deadline']
    assert any(j['state'] == 'accepted' for j in jobs.load(directory)['jobs'].values())
    assert model_calls == [('input', 'owner', 'deepseek'), ('input', 'review', 'minimax'),
                           ('c', 'owner', 'minimax'), ('c', 'review', 'deepseek')]
    assert machine_calls.count('finalize') == 2


def test_competing_service_cannot_relabel_live_operations(tmp_path):
    from cmre import coordination
    from cmre.pipeline_controller import Service
    directory = tmp_path / 'run'
    spec = dict(manifest(), pipeline=True)
    pc.initialize(directory, spec, {})
    policy.write(directory, policy.defaults(spec), spec)
    operation = directory / 'pipeline/operations/op000001.json'
    operation.parent.mkdir(parents=True)
    operation.write_text(json.dumps({'id': 'op000001', 'state': 'pending', 'kind': 'scan',
                                    'function': None, 'job': None}))
    before = operation.read_bytes()
    with coordination.lock(directory / 'run.lock'):
        with Service(directory, {}, lambda: None) as service:
            with pytest.raises(coordination.BusyError):
                pc.run(directory, actor=lambda: None, execute=lambda: None, service=service)
    assert operation.read_bytes() == before


def test_feedback_exposes_failed_gate_and_named_control_survivors():
    record = {'id': 'a0001', 'kind': 'test', 'ok': True,
              'response': {'ok': False, 'report_status': 'mismatch',
                           'first_failure': {'kind': 'mismatch', 'case': 17, 'opt': '-O0'}}}
    result = pc._project_record(record, {})
    assert result['ok'] is False
    assert result['first_failure']['case'] == 17
    record.update(kind='control-plan', response={'ok': False, 'result': {
        'status': 'incomplete', 'stages': {'site-401010': {'classification': 'survived'}}}})
    result = pc._project_record(record, {})
    assert result['controls']['site-401010']['classification'] == 'survived'


def test_held_adoptions_do_not_fill_the_upstream_buffer():
    from cmre.pipeline_controller import supply_selection
    state = {'jobs': {'bad': {'id': 'bad', 'state': 'ready',
        'pipeline_hold': {'reason': 'native gate failed'},
        'config': {'function': '0x00401000', 'mode': 'input', 'pipeline_stage': 'intake',
                   'max_body_bytes': 1000000}}}}
    selected, _ = supply_selection([row(0x402000)], state, policy.defaults(manifest()))
    assert [r['va'] for r in selected] == ['0x00402000']


def test_evidence_families_have_separate_capacity_and_watermarks():
    from cmre.pipeline_controller import supply_selection
    fresh, difficult = row(0x401000), row(0x402000)
    difficult['dispatch_hold'] = {'reason': 'work-state-hold'}
    selected, shortage = supply_selection([fresh, difficult], {'jobs': {}}, policy.defaults(manifest()))
    assert set(policy.supply_stage(r) for r in selected) == {'intake', 'evidence'}
    assert shortage['intake']['functions'] == 1
    assert shortage['evidence']['functions'] == 1


def test_source_site_plans_pin_the_normal_gate_and_cover_every_helper(tmp_path, monkeypatch):
    from types import SimpleNamespace
    from cmre import env, lift, mutate
    from cmre.provider_runtime import Executor
    from cmre.pipeline_flow import pin
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path / 'hints')
    root = tmp_path / 'run/jobs/c'
    root.mkdir(parents=True)
    source = root / 'candidate.c'
    source.write_text('int f(int x) { return x + 1; }')
    sites = {0x401004: SimpleNamespace(target=0x945640),
             0x401010: SimpleNamespace(target=0x945640)}
    monkeypatch.setattr(lift, 'discover', lambda va: SimpleNamespace(insns=sites))
    monkeypatch.setattr(mutate, 'control_identity', lambda *a: {'cases': 600,
                        'options': {'cases': 600}, 'hints_path': str(env.HINTS_DIR / 'fn_00401000.json')})
    launches = []

    def run_plan(path, output):
        parsed, texts = mutate._plan(path)
        launches.append(parsed)
        return {'status': 'controls-complete'}

    monkeypatch.setattr(mutate, 'run_plan', run_plan)
    config = {'function': '0x00401000', 'mode': 'c', 'candidate': str(source),
              'self_service_controls': True, 'require_controls': True}
    executor = Executor(root.parent.parent)
    records = {'a0001': {'action': {'kind': 'test'}, 'response': {
        'ok': True, 'source_sha256': pin(source)['sha256']}}}
    controls = [{'site': f'{site:#010x}', 'name': 'site-' + str(i),
                 'transform': {'old': 'return x + 1;', 'new': f'return x + {i+2};', 'count': 1},
                 'expect': {'kind': 'mismatch', 'field': 'regs'}} for i, site in enumerate(sites)]
    with pytest.raises(ValueError, match='Every registered helper'):
        executor._control_plan({}, config, root, {'controls': controls[:1]}, root / 'missing', records)
    assert launches == []
    before = source.read_bytes()
    result = executor._control_plan({}, config, root, {'controls': controls}, root / 'complete', records)
    assert result['ok']
    assert launches[0]['identity']['cases'] == 600
    assert launches[0]['source'] == pin(source)
    assert source.read_bytes() == before


def test_models_refill_while_a_deferred_full_test_is_running(tmp_path):
    spec = manifest()
    spec['concurrency'] = 1
    spec['jobs'] = [{'id': name, 'config': {'function': hex(0x401000+i*16),
        'mode': 'c', 'owner': 'minimax', 'goal': name, 'defer_execution': True}}
        for i, name in enumerate(['a-slow', 'b-next'])]
    directory = tmp_path / 'run'
    pc.initialize(directory, spec, {})
    gate_started, model_refilled = Event(), Event()
    calls = Counter()

    def actor(job, role, context, stage):
        calls[job['id']] += 1
        if job['id'] == 'b-next':
            assert gate_started.wait(3)
            model_refilled.set()
        action = {'kind': 'test'} if job['id'] == 'a-slow' and calls['a-slow'] == 1 else {
            'kind': 'hold', 'reason': 'bounded test complete'}
        return {'model': spec['roles'][role]['model'], 'action': action}

    def execute(job, action, output, **kwargs):
        gate_started.set()
        assert model_refilled.wait(3), 'model slot stayed busy while only the C gate was running'
        return {'kind': 'test', 'ok': False}

    pc.run(directory, actor=actor, execute=execute,
           service=lambda *a: {'pending': False}, limits=lambda state: policy.defaults(state['manifest']))
    assert model_refilled.is_set()


def test_automatic_build_and_package_obey_their_own_pause_limits(tmp_path):
    spec = manifest()
    spec['jobs'][0]['config'].update(mode='c', owner='minimax', reviewer='deepseek',
        auto_bundle=True, defer_execution=True, defer_publication=True)
    directory = tmp_path / 'run'
    pc.initialize(directory, spec, {})
    settings = policy.defaults(spec)
    settings['workers'].update(build=0, package=0)
    executed = []
    paid = []

    def actor(job, role, context, stage):
        paid.append(context['phase'])
        action = {'kind': 'test'} if context['phase'] == 'owner' else {
            'kind': 'review', 'verdict': 'pass', 'bundle_sha256': context['bundle']['sha256'],
            'checks': context['required_checks'], 'unknowns': []}
        return {'model': spec['roles'][role]['model'], 'action': action}

    def execute(job, action, output, **kwargs):
        executed.append(action['kind'])
        if action['kind'] in ('build', 'bundle'):
            current = jobs.load(directory)['jobs'][job['id']]
            checkpoint = current['checkpoint']
            assert checkpoint['action'] + '-output' in output
            assert checkpoint['phase'] == 'execute'
        if action['kind'] == 'bundle':
            return {'ready': True, 'sha256': 'exact', 'required_checks': ['source']}
        if action['kind'] == 'finalize':
            return {'state': 'accepted'}
        return {'kind': action['kind'], 'ok': True}

    def run():
        return pc.run(directory, actor=actor, execute=execute,
            service=lambda *a: {'pending': False}, limits=lambda state: settings)

    first = run()
    assert executed == ['test']
    current = jobs.load(directory)['jobs']['initial']
    assert policy.stage(current) == 'build'
    settings['workers']['build'] = 1
    second = run()
    assert executed == ['test', 'build']
    assert policy.stage(jobs.load(directory)['jobs']['initial']) == 'package'
    settings['workers']['package'] = 1
    third = run()
    assert executed == ['test', 'build', 'bundle', 'finalize']
    assert paid == ['owner', 'review']
    assert first['deadline'] == second['deadline'] == third['deadline']


def test_c_task_is_reserved_before_any_paid_author_stage(tmp_path):
    spec = manifest()
    spec['jobs'][0]['config'].update(mode='c', reserve_task=True)
    directory = tmp_path / 'run'
    pc.initialize(directory, spec, {})
    paid = []

    class Executor:
        def reserve(self, job):
            raise ValueError('foreign central task lease')

        def __call__(self, *args, **kwargs):
            raise AssertionError('no gate should run')

    result = pc.run(directory, actor=lambda *a: paid.append(a), execute=Executor())
    assert paid == []
    assert result['jobs']['initial']['state'] == 'held'


def test_supply_attempt_counts_do_not_reset_or_double_count_failures():
    from cmre.pipeline_flow import attempt_counts
    state = {'fn_00401000': {'failures': 4, 'report_events': {'c-00401000-a:1': 'fail'}}}
    manifests = [{'va': '0x00401000', 'task_id': 'c-00401000-a',
                  'attempts': [{'number': 1}, {'number': 2}]}]
    assert attempt_counts(manifests, state)[0x401000] == 5


def test_full_author_buffer_applies_upstream_adoption_backpressure():
    from cmre.pipeline_controller import author_buffer_full
    settings = policy.defaults(manifest())
    state = {'jobs': {'source': {'state': 'awaiting-execution', 'config': {
        'mode': 'c', 'max_body_bytes': settings['buffers']['author']['high_bytes']}}}}
    assert author_buffer_full(state, settings)
    state['jobs']['source']['state'] = 'accepted'
    assert not author_buffer_full(state, settings)
