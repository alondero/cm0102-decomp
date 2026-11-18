"""The monitor distinguishes observations, queued stages and current work."""
from concurrent.futures import Future
import json
import os
import time

from scripts import cmre_farm, cmre_farm_dashboard as dashboard


def test_farm_observation_projects_each_stage_without_mutating_inputs(tmp_path, monkeypatch):
    monkeypatch.setenv('CMRE_CLAIM_OWNER', 'test')
    config = cmre_farm.FarmConfig(4, ['test/model'], 2, 'test/review', 10, None, 0, None, False, 'test')
    farm = cmre_farm.Farm(tmp_path, tmp_path, None, None, config, reconcile=False)
    farm.monitor = {'poll_seconds': 10, 'verifier_slots': 2}
    farm.frontier = [cmre_farm.Item('fn_00401000', 90, False, 1, 'x.c')]
    farm.last_scan = farm.clock()
    farm.running = [(dict(n=1, fn='fn_00402000', kind='audit', model='test/review', confirm=True),
                     None, farm.clock())]
    waiting, gating = Future(), Future()
    gating.set_running_or_notify_cancel()

    def job(n):
        return dict(n=n, fn=f'fn_{0x00403000 + n:08x}', kind='translate', model='test/model')

    farm.verifying = [(job(2), waiting), (job(3), gating)]
    farm.lease_waits.add(4)
    farm.accept_queue = [job(4), job(5)]
    farm.observe(accepting=job(5), acceptance_queue=[job(4)])
    snap = json.loads((tmp_path / 'live.json').read_bytes())
    assert [(row['n'], row['stage']) for row in snap['jobs']] == [
        (1, 'model'), (2, 'gate queue'), (3, 'gating'), (4, 'awaiting writer'), (5, 'accepting')]
    assert snap['jobs'][0]['confirm'] is True
    assert snap['scheduler']['frontier'] == 1
    assert snap['scheduler']['frontier_bytes'] == 90
    assert snap['settings']['workers'] == 4
    assert farm.accept_queue == [job(4), job(5)]
    assert not (tmp_path / 'state.json').exists()


def test_observer_follows_lifecycle_and_expires_reported_supply(tmp_path, monkeypatch):
    now = 4_800_000
    monkeypatch.setattr(dashboard.time, 'time', lambda: now)
    monkeypatch.setattr(dashboard, 'process_alive', lambda pid: pid == 7)
    directory = tmp_path / 'farm' / 'current'
    directory.mkdir(parents=True)
    reports = tmp_path / 'reports'
    reports.mkdir()
    for name, expires in [('live', now + 60), ('expired', now)]:
        (reports / f'{name}.json').write_text(json.dumps({'id': name, 'expires_at': expires}))
    journal = directory / 'events.jsonl'

    def emit(name, at, **fields):
        with journal.open('a', encoding='utf-8') as stream:
            stream.write(json.dumps({'event': name, 'at': at, **fields}) + '\n')

    emit('farm-start', now - 30, pid=7, workers=4, audit_workers=2)
    fn = 'fn_00402000'
    emit('accepted', now - 20, fn=fn, kind='translate', bytes=150)
    emit('accepted', now - 19, fn=fn, kind='fix', bytes=150)
    emit('config-changed', now - 10, changes={'workers': [4, 8]})
    snap = {'schema': 'cmre-farm-live/1', 'at': now, 'pid': 7, 'stopping': False,
            'settings': {'workers': 8}, 'jobs': [{'fn': fn, 'stage': 'gating'}], 'counts': {},
            'scheduler': {'frontier': 0}, 'verifier_slots': 2}
    (directory / 'live.json').write_text(json.dumps(snap))
    (directory / 'control.json').write_text(json.dumps({'workers': 12, 'add_hours': []}))
    observer = dashboard.Dashboard(tmp_path / 'farm', reports)
    live = observer.overview()
    assert live['run']['status'] == 'live'
    assert live['run']['settings_changed_at'] == now - 10
    assert live['run']['settings']['workers'] == 8
    assert live['run']['requested_settings'] == {'workers': 12}
    assert live['run']['jobs'] == snap['jobs']
    assert sum(bucket['bytes'] for bucket in live['run']['throughput']) == 150
    assert [row['id'] for row in live['supply']] == ['live']
    (directory / 'STOP').touch()
    assert observer.overview()['run']['status'] == 'draining'
    now += 80
    assert observer.overview()['run']['status'] == 'stale'
    assert observer.overview()['supply'] == []
    emit('farm-exit', now)
    stopped = observer.overview()['run']
    assert stopped['status'] == 'stopped'
    assert stopped['jobs'] == []
    assert stopped['counts']['accepted_functions'] == 1
    emit('farm-start', now + 1, pid=8, workers=10)
    interrupted = observer.overview()['run']
    assert interrupted['status'] == 'interrupted'
    assert not interrupted['telemetry']
    assert interrupted['settings']['workers'] == 10


def test_legacy_history_reads_state_and_removes_unfinished_jobs(tmp_path):
    directory = tmp_path / 'farm' / 'old'
    directory.mkdir(parents=True)
    fn = 'fn_00402000'
    now = time.time()
    rows = [
        {'event': 'farm-start', 'at': now - 60, 'pid': os.getpid(), 'workers': 4},
        {'event': 'dispatch', 'at': now - 30, 'fn': fn, 'kind': 'translate', 'model': 'old/model', 'n': 1},
        {'event': 'farm-exit', 'at': now},
    ]
    (directory / 'events.jsonl').write_text('\n'.join(map(json.dumps, rows)) + '\n')
    (directory / 'state.json').write_text(json.dumps(
        {'accepted': {fn: {'bytes': 80}}, 'parked': {}, 'defects': {}, 'input_needed': {}}))
    run = dashboard.Dashboard(tmp_path / 'farm', tmp_path / 'reports').overview()['run']
    assert (run['status'], run['jobs'], run['counts']['accepted_bytes']) == ('stopped', [], 80)
