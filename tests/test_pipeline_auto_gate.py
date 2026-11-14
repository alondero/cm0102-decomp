"""A successful owner build queues the normal gate without another paid turn."""
from cmre import pipeline_policy, provider_campaign as pc, provider_jobs


def test_replenishment_history_does_not_admit_fresh_jobs_as_active_owners():
    active = [{'id': f'active-{i}', 'state': 'prepared', 'attempts': 1,
               'history': [{}] * 8, 'config': {'mode': 'c', 'owner': 'minimax'}} for i in range(8)]
    enqueued = [{'id': f'fresh-{i}', 'state': 'prepared',
                 'history': [{'state': 'prepared', 'fields': ['state']}],
                 'config': {'mode': 'c', 'owner': 'minimax'}} for i in range(20)]
    chosen = pc._dispatch_batch(active + enqueued, 4,
        role_capacity={'minimax': 4, 'deepseek': 4}, owner_wip={'minimax': 8},
        stage_capacity={'author': 8})
    assert all(job['id'].startswith('active-') for job in chosen)


def test_cancelled_jobs_are_not_reported_as_author_supply(tmp_path):
    from cmre.pipeline_flow import inbox
    directory = tmp_path / 'run'
    provider_jobs.create(directory, {'jobs': [{'id': 'unused', 'config': {
        'mode': 'c', 'function': '0x00401000'}}]}, {})
    provider_jobs.update(directory, 'unused', {'state': 'cancelled'}, expected_revision=0)
    assert inbox(directory)['counts'] == {'cancelled': 1}


def setup_run(tmp_path, *, test_ok=True):
    spec = {'schema': pc.SCHEMA, 'duration_seconds': 600, 'concurrency': 2,
            'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
            'jobs': [{'id': 'candidate', 'config': {'function': '0x00401000',
                'mode': 'c', 'owner': 'minimax', 'goal': 'Recover the pinned body',
                'auto_bundle': True, 'auto_test_after_build': True,
                'defer_execution': True, 'defer_publication': True}}]}
    directory = tmp_path / 'run'
    pc.initialize(directory, spec, {})
    settings = pipeline_policy.defaults(spec)
    paid, executed = [], []

    def actor(job, role, context, stage):
        paid.append(context['phase'])
        if context['phase'] == 'review':
            action = {'kind': 'review', 'verdict': 'pass',
                      'bundle_sha256': context['bundle']['sha256'],
                      'checks': context['required_checks'], 'unknowns': []}
        elif len(paid) == 1:
            action = {'kind': 'build'}
        else:
            action = {'kind': 'hold', 'reason': 'Full test needs source repair'}
        return {'model': spec['roles'][role]['model'], 'action': action}

    def execute(job, action, output, **kwargs):
        executed.append(action['kind'])
        if action['kind'] == 'bundle':
            return {'ready': True, 'sha256': 'exact', 'required_checks': ['source']}
        if action['kind'] == 'finalize':
            return {'state': 'accepted'}
        return {'kind': action['kind'], 'ok': test_ok if action['kind'] == 'test' else True}

    def run():
        return pc.run(directory, actor=actor, execute=execute,
                      service=lambda *args: {'pending': False}, limits=lambda state: settings)

    return directory, settings, paid, executed, run


def test_build_to_gate_obeys_test_pause_without_paid_planning(tmp_path):
    directory, settings, paid, executed, run = setup_run(tmp_path)
    settings['workers']['test'] = 0
    first = run()
    assert executed == ['build']
    assert paid == ['owner']
    job = provider_jobs.load(directory)['jobs']['candidate']
    assert pipeline_policy.stage(job) == 'test'
    assert job['pending_step']['automatic']
    settings['workers']['test'] = 1
    second = run()
    assert executed == ['build', 'test', 'build', 'bundle', 'finalize']
    assert paid == ['owner', 'review']
    assert second['counts']['accepted'] == 1
    assert first['deadline'] == second['deadline']


def test_failed_automatic_gate_returns_to_owner_without_review_or_accept(tmp_path):
    directory, settings, paid, executed, run = setup_run(tmp_path, test_ok=False)
    result = run()
    assert executed == ['build', 'test']
    assert paid == ['owner', 'owner']
    assert result['counts']['held'] == 1
    assert result['counts']['accepted'] == 0
    job = provider_jobs.load(directory)['jobs']['candidate']
    assert len([ref for ref in job['results'] if ref['kind'] == 'test']) == 1
    assert 'test' in job['feedback'][-1].lower()


def test_candidate_change_while_gate_paused_is_held(tmp_path):
    # Configure a private candidate before initialization, then pause exactly at
    # the automatic gate so the existing execution-time pin check is exercised.
    source = tmp_path / 'run/jobs/candidate/fn_00401000.c'
    spec = {'schema': pc.SCHEMA, 'duration_seconds': 600, 'concurrency': 2,
            'roles': {'deepseek': {'model': 'd'}, 'minimax': {'model': 'm'}},
            'jobs': [{'id': 'candidate', 'config': {'function': '0x00401000',
                'mode': 'c', 'goal': 'Recover', 'candidate': str(source),
                'auto_bundle': True, 'auto_test_after_build': True,
                'defer_execution': True}}]}
    directory = tmp_path / 'run'
    pc.initialize(directory, spec, {})
    source.parent.mkdir(parents=True)
    source.write_text('before')
    settings = pipeline_policy.defaults(spec)
    settings['workers']['test'] = 0
    executed = []

    def actor(job, role, context, stage):
        return {'model': 'm', 'action': {'kind': 'build'}}

    def execute(job, action, output, **kwargs):
        executed.append(action['kind'])
        return {'kind': action['kind'], 'ok': True}

    def run():
        return pc.run(directory, actor=actor, execute=execute,
                      service=lambda *args: {'pending': False}, limits=lambda state: settings)

    run()
    source.write_text('after')
    settings['workers']['test'] = 1
    result = run()
    assert executed == ['build']
    assert result['counts']['held'] == 1
    assert 'hash' in provider_jobs.load(directory)['jobs']['candidate']['hold_reason'].lower()
