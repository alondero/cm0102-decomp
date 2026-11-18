"""Farm scheduling, verification and accounting with fake cmre and fake workers.

No test starts a paid model or runs the real gate.
"""
import importlib.util
import hashlib
import json
import os
import re
from pathlib import Path
import sys
import time

import pytest

MODULE = Path(__file__).resolve().parents[1] / 'cmre_farm.py'
spec = importlib.util.spec_from_file_location('cmre_farm', MODULE)
farm = importlib.util.module_from_spec(spec)
sys.modules['cmre_farm'] = farm
spec.loader.exec_module(farm)

GOOD = '#include "cmre.h"\nu32 cm_fn_00401000(u32 a) { return a + 1; }\n'


def bound_automatic_fix(root):
    changed = GOOD.replace('a + 1', 'a + 2') + '// PASS fake gate\n'
    f, cmre, _ = make(root, [], writes(changed), workers=1, audit_workers=0)
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    (root / f'recovery/hints/{fn}.json').write_bytes(b'{"arg_count":1}')
    f.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(f._inputs(fn))}
    cmre.brief = lambda fn: 'FAKE original listing'
    origin = f._new_job('audit', fn, 'm/auditor', bytes=200)
    f._launch(origin)
    concerns = [{'detail': 'unresolved fidelity concern', 'evidence': 'FAKE original instruction'}]
    Path(origin['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': concerns}))
    f._audit_result(origin, False)
    f.running = []
    return f, cmre, origin, concerns


def test_automatic_fix_rejects_missing_origin_snapshot_before_dispatch(root):
    f, cmre, origin, concerns = bound_automatic_fix(root)
    prior = f._inputs(origin['fn'])
    (f.run_dir / 'audit/snapshots' / str(origin['n']) / 'source.c').unlink()
    f._dispatch()
    f._reap()
    f._collect()
    f._accept()
    assert cmre.accepted == []
    assert f._inputs(origin['fn']) == prior
    assert f.state['defects'][origin['fn']] == concerns
    assert origin['fn'] in f.audit_queue
    assert not any(job['kind'] == 'fix' for job, _, _ in f.launcher.started)


def test_automatic_fix_rejects_changed_dispatch_bundle_before_install(root):
    f, cmre, origin, concerns = bound_automatic_fix(root)
    prior = f._inputs(origin['fn'])
    f._dispatch()
    fix = next(job for job, _, _ in f.running if job['kind'] == 'fix')
    (root / fix['bundle']).write_text('not the dispatched fix inputs')
    f._reap()
    f._collect()
    f._accept()
    assert cmre.accepted == []
    assert f._inputs(origin['fn']) == prior
    assert f.state['defects'][origin['fn']] == concerns
    assert origin['fn'] in f.audit_queue


@pytest.mark.parametrize('artifact', ['origin-source', 'fix-bundle'])
def test_automatic_fix_rechecks_evidence_before_serial_acceptance(root, artifact):
    f, cmre, origin, concerns = bound_automatic_fix(root)
    prior = f._inputs(origin['fn'])
    f._dispatch()
    fix = next(job for job, _, _ in f.running if job['kind'] == 'fix')
    f._reap()
    f._collect()
    assert f.accept_queue
    path = (f.run_dir / 'audit/snapshots' / str(origin['n']) / 'source.c'
            if artifact == 'origin-source' else root / fix['bundle'])
    path.unlink()
    f._accept()
    assert cmre.accepted == []
    assert f._inputs(origin['fn']) == prior
    assert f.state['defects'][origin['fn']] == concerns
    assert origin['fn'] in f.audit_queue


def test_missing_canonical_source_claim_fails_before_creating_receipt(root, monkeypatch, capsys):
    f, _, job, canonical = prepared_candidate(root)
    canonical.unlink()
    f.state['defects'][job['fn']] = ['unresolved concern']
    f.save()
    monkeypatch.setattr(farm, 'ROOT', root)
    assert farm.main(['review', str(f.run_dir), job['fn'], '--claim', 'missing-c']) == 2
    view = json.loads(capsys.readouterr().out)
    assert view['status'] == 'fresh-review-required'
    assert 'review_receipt' not in view
    assert not (f.run_dir / 'reviews/missing-c').exists()


@pytest.mark.parametrize('action', ['adopt', 'dismiss'])
def test_old_missing_source_receipt_cannot_dismiss_unresolved_concern(root, action):
    f, _, _ = make(root, [], writes(GOOD), workers=0)
    fn = 'fn_00401000'
    f.state['defects'][fn] = ['unresolved missing-source concern']
    f.save()
    directory = f.run_dir / 'reviews/old-missing-c'
    directory.mkdir(parents=True)
    receipt = directory / 'receipt.json'
    receipt.write_text(json.dumps({'schema': 'cmre-farm-review/1', 'fn': fn,
                                   'identity': {'c': None, 'hint': None},
                                   'review': {'open_findings': f.state['defects'][fn]}}))
    source = root / 'hand.c'
    source.write_text(GOOD)
    farm._request(f.run_dir, fn, action, source=str(source), reason='old unreviewable claim',
                  review_receipt=str(receipt), review_receipt_sha256=hashlib.sha256(receipt.read_bytes()).hexdigest(),
                  expected_identity={'c': None, 'hint': None})
    f._requests()
    assert f.state['defects'][fn] == ['unresolved missing-source concern']
    assert list((f.run_dir / 'requests/rejected').glob('*.json'))


@pytest.mark.parametrize('action', ['adopt', 'dismiss'])
def test_offline_quarantine_blocks_only_its_function_not_unrelated_request(root, monkeypatch, action):
    f, cmre, _ = make(root, [], writes(GOOD), workers=0)
    first, second = 'fn_00401000', 'fn_00401100'
    for fn in (first, second):
        (root / f'recovery/c/{fn}.c').write_text(GOOD.replace(first, fn))
        f.state['accepted'][fn] = {'bytes': 200}
        f.state['defects'][fn] = [f'concern {fn}']
    f.save()
    proof = farm.review_request_data(review_receipt(f, first), f.run_dir, root, first)
    blocked = farm._request(f.run_dir, first, 'dismiss', reason='prior review', **proof)
    f.state['quarantined'] = {first: {'reason': 'unresolved acceptance'}}
    f.save()
    prior_first = f._inputs(first)
    hand = root / 'second.c'
    hand.write_text(GOOD.replace(first, second) + '// PASS\n')
    reviewed_request(f, second, action, source=str(hand), reason='fresh second-function review')
    monkeypatch.setattr(farm, 'ROOT', root)
    monkeypatch.setattr(farm, 'CmreAdapter', lambda: cmre)
    result = farm.apply_requests_now(f.run_dir)
    assert blocked.exists()
    assert result._inputs(first) == prior_first
    assert first in result.state['quarantined'] and first in result.state['defects']
    assert second not in result.state['defects']
    assert cmre.accepted == ([second] if action == 'adopt' else [])


@pytest.mark.parametrize('action', ['adopt', 'dismiss'])
def test_quarantined_action_cli_returns_bounded_json_instead_of_global_abort(root, monkeypatch, capsys, action):
    f, cmre, job, canonical = prepared_candidate(root)
    proof = review_receipt(f)
    before = canonical.read_bytes()
    f.state['quarantined'] = {job['fn']: {'reason': 'unresolved acceptance'}}
    f._event('farm-exit', status='done')
    f.save()
    monkeypatch.setattr(farm, 'ROOT', root)
    monkeypatch.setattr(farm, 'CmreAdapter', lambda: cmre)
    args = ['--source', job['source']] if action == 'adopt' else ['--reason', 'old review']
    assert farm.main([action, str(f.run_dir), job['fn'], *args, '--review-receipt', proof]) == 2
    view = json.loads(capsys.readouterr().out)
    assert view['status'] == 'acceptance-quarantined'
    assert canonical.read_bytes() == before
    assert not list((f.run_dir / 'requests').glob('*.json'))


def fix_artifact(f, origin, fix, artifact):
    side, kind = artifact.split('-', 1)
    job = origin if side == 'origin' else fix
    if kind == 'bundle':
        return f.root / job['bundle']
    if kind == 'verdict':
        return Path(job['audit_path'])
    folder = f.run_dir / 'audit/snapshots' / str(origin['n']) if side == 'origin' else Path(fix['_fix_dispatch']['snapshot'])
    return folder / {'source': 'source.c', 'hints': 'hints.json', 'metadata': 'identity.json'}[kind]


@pytest.mark.parametrize('change', ['missing', 'drift'])
@pytest.mark.parametrize('artifact', ['origin-source', 'origin-hints', 'origin-metadata', 'origin-bundle', 'origin-verdict'])
def test_fix_dispatch_requires_complete_origin_evidence(root, artifact, change):
    f, cmre, origin, concerns = bound_automatic_fix(root)
    prior = f._inputs(origin['fn'])
    path = fix_artifact(f, origin, None, artifact)
    path.unlink() if change == 'missing' else path.write_bytes(b'changed origin evidence')
    f._dispatch()
    assert not any(job['kind'] == 'fix' for job, _, _ in f.launcher.started)
    assert f._inputs(origin['fn']) == prior
    assert f.state['defects'][origin['fn']] == concerns
    assert origin['fn'] in f.audit_queue
    assert cmre.accepted == []


@pytest.mark.parametrize('stage', ['before-install', 'before-accept'])
@pytest.mark.parametrize('change', ['missing', 'drift'])
@pytest.mark.parametrize('artifact', [
    'origin-source', 'origin-hints', 'origin-metadata', 'origin-bundle', 'origin-verdict',
    'fix-source', 'fix-hints', 'fix-metadata', 'fix-bundle',
])
def test_fix_consumption_rechecks_all_origin_and_dispatch_artifacts(root, artifact, change, stage):
    f, cmre, origin, concerns = bound_automatic_fix(root)
    prior = f._inputs(origin['fn'])
    f._dispatch()
    fix = next(job for job, _, _ in f.running if job['kind'] == 'fix')
    if stage == 'before-accept':
        f._reap()
        f._collect()
        assert f.accept_queue
    path = fix_artifact(f, origin, fix, artifact)
    path.unlink() if change == 'missing' else path.write_bytes(b'changed bound fix input')
    if stage == 'before-install':
        f._reap()
        f._collect()
    f._accept()
    assert cmre.accepted == []
    assert f._inputs(origin['fn']) == prior
    assert f.state['defects'][origin['fn']] == concerns
    assert origin['fn'] in f.audit_queue


@pytest.mark.parametrize('artifact', ['origin-source', 'fix-bundle'])
def test_fix_evidence_changed_during_private_gate_blocks_canonical_install(root, artifact):
    f, cmre, origin, concerns = bound_automatic_fix(root)
    prior = f._inputs(origin['fn'])
    f._dispatch()
    fix = next(job for job, _, _ in f.running if job['kind'] == 'fix')
    real_test = cmre.test
    def changing_test(fn, source=None):
        result = real_test(fn, source)
        if source is not None:
            fix_artifact(f, origin, fix, artifact).unlink()
        return result
    cmre.test = changing_test
    f._reap()
    f._collect()
    f._accept()
    assert cmre.accepted == []
    assert f._inputs(origin['fn']) == prior
    assert f.state['defects'][origin['fn']] == concerns
    assert all(source is not None for _, source in cmre.tests)


def test_fix_evidence_is_checked_again_after_acceptance_hold_retry(root):
    f, cmre, origin, concerns = bound_automatic_fix(root)
    prior = f._inputs(origin['fn'])
    f._dispatch()
    fix = next(job for job, _, _ in f.running if job['kind'] == 'fix')
    f._reap()
    f._collect()
    cmre.holds[origin['fn']] = {'state': farm.HARD_HOLDS[0]}
    real_retry = cmre.retry
    def changing_retry(fn, reason):
        real_retry(fn, reason)
        fix_artifact(f, origin, fix, 'fix-bundle').unlink()
    cmre.retry = changing_retry
    f._accept()
    assert cmre.accepted == []
    assert f._inputs(origin['fn']) == prior
    assert f.state['defects'][origin['fn']] == concerns


def test_fix_origin_is_rechecked_after_bundle_creation_before_worker_start(root):
    f, cmre, origin, concerns = bound_automatic_fix(root)
    prior = f._inputs(origin['fn'])
    def changing_brief(fn):
        (f.run_dir / 'audit/snapshots' / str(origin['n']) / 'source.c').unlink()
        return 'FAKE listing while origin disappeared'
    cmre.brief = changing_brief
    f._dispatch()
    assert not any(job['kind'] == 'fix' for job, _, _ in f.launcher.started)
    assert f._inputs(origin['fn']) == prior
    assert f.state['defects'][origin['fn']] == concerns
    assert origin['fn'] in f.audit_queue


def test_stale_fix_never_restores_over_newer_canonical_or_concerns(root):
    f, cmre, origin, concerns = bound_automatic_fix(root)
    fn = origin['fn']
    f._dispatch()
    fix = next(job for job, _, _ in f.running if job['kind'] == 'fix')
    f._reap()
    f._collect()
    fix_artifact(f, origin, fix, 'fix-bundle').unlink()
    (root / f'recovery/c/{fn}.c').write_bytes(b'newer unrelated canonical source')
    (root / f'recovery/hints/{fn}.json').write_bytes(b'{"arg_count":99}')
    f.state['defects'][fn].append({'detail': 'new unreviewed concern'})
    current = f._inputs(fn)
    current_concerns = json.loads(json.dumps(f.state['defects'][fn]))
    f._accept()
    assert f._inputs(fn) == current
    assert f.state['defects'][fn] == current_concerns
    assert cmre.accepted == []
    assert fn in f.audit_queue


@pytest.mark.parametrize('newer', [False, True])
def test_interrupted_fix_with_missing_origin_requeues_without_stale_restore(root, newer):
    f, cmre, origin, concerns = bound_automatic_fix(root)
    fn, prior = origin['fn'], f._inputs(origin['fn'])
    f._dispatch()
    f._reap()
    f._collect()
    f.save()
    (f.run_dir / 'audit/snapshots' / str(origin['n']) / 'source.c').unlink()
    if newer:
        (root / f'recovery/c/{fn}.c').write_bytes(b'newer canonical after interruption')
    expected = f._inputs(fn) if newer else prior
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert resumed._inputs(fn) == expected
    assert resumed.state['defects'][fn] == concerns
    assert fn in resumed.audit_queue
    assert cmre.accepted == []


def test_partial_fix_dispatch_identity_is_not_reused_after_restart(root):
    f, cmre, origin, _ = bound_automatic_fix(root)
    fix = f._next_pending()
    def die(*args):
        raise KeyboardInterrupt('after input snapshot, before dispatch event')
    f.launcher.start = die
    with pytest.raises(KeyboardInterrupt):
        f._launch(fix)
    path = Path(fix['_fix_dispatch']['snapshot']) / 'identity.json'
    before = path.read_bytes()
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert resumed._new_job('audit', origin['fn'], 'm/auditor')['n'] > fix['n']
    assert path.read_bytes() == before


def test_unknown_acceptance_during_offline_drain_does_not_abort_other_function(root, monkeypatch):
    f, cmre, _ = make(root, [], writes(GOOD), workers=0)
    first, second = 'fn_00401000', 'fn_00401100'
    for fn in (first, second):
        (root / f'recovery/c/{fn}.c').write_text(GOOD.replace(first, fn))
        f.state['defects'][fn] = [f'concern {fn}']
    f.save()
    requests = {}
    outputs = {}
    for fn in (first, second):
        source = root / f'{fn}-hand.c'
        source.write_text(GOOD.replace(first, fn).replace('a + 1', 'a + 2') + '// PASS\n')
        outputs[fn] = source.read_bytes()
        requests[fn] = reviewed_request(f, fn, 'adopt', source=str(source))
    real_accept = cmre.accept
    def uncertain_accept(fn, *, request):
        if fn == first:
            cmre.accepted.append(fn)
            return 1, 'unknown error after ledger write'
        return real_accept(fn, request=request)
    cmre.accept = uncertain_accept
    cmre.acceptance_identity = lambda fn: None
    monkeypatch.setattr(farm, 'ROOT', root)
    monkeypatch.setattr(farm, 'CmreAdapter', lambda: cmre)
    result = farm.apply_requests_now(f.run_dir)
    assert set(cmre.accepted) == {first, second}
    assert first in result.state['quarantined'] and first in result.state['defects']
    assert second not in result.state['defects']
    assert requests[first].exists() and not requests[second].exists()
    for fn in (first, second):
        assert (root / f'recovery/c/{fn}.c').read_bytes() == outputs[fn]


def test_newly_unknown_target_action_returns_bounded_quarantine_json(root, monkeypatch, capsys):
    f, cmre, job, canonical = prepared_candidate(root)
    proof = review_receipt(f)
    f._event('farm-exit', status='done')
    def uncertain_accept(fn, *, request):
        cmre.accepted.append(fn)
        return 1, 'unknown after ledger commit'
    cmre.accept = uncertain_accept
    cmre.acceptance_identity = lambda fn: None
    monkeypatch.setattr(farm, 'ROOT', root)
    monkeypatch.setattr(farm, 'CmreAdapter', lambda: cmre)
    assert farm.main(['adopt', str(f.run_dir), job['fn'], '--source', job['source'],
                      '--review-receipt', proof]) == 2
    view = json.loads(capsys.readouterr().out)
    assert view['status'] == 'acceptance-quarantined'
    assert canonical.read_bytes() == Path(job['source']).read_bytes()
    assert list((f.run_dir / 'requests').glob('*.json'))


@pytest.mark.parametrize('confirm', [False, True])
@pytest.mark.parametrize('verdict', ['ok', 'defect'])
@pytest.mark.parametrize('artifact', ['source.c', 'hints.json', 'identity.json'])
@pytest.mark.parametrize('change', ['missing', 'drift'])
def test_verdict_admission_requires_retained_dispatch_artifacts(root, confirm, verdict, artifact, change):
    f, _, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1,
                    confirm_model='m/judge' if confirm else None)
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    (root / f'recovery/hints/{fn}.json').write_bytes(b'{"arg_count":1}')
    f.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(f._inputs(fn))}
    concern = [{'detail': 'legacy unresolved concern'}]
    f.state['defects'][fn] = concern.copy()
    f.state['audits'][fn] = {'rounds': 0}
    job = f._new_job('audit', fn, 'm/strong', bytes=200)
    f._launch(job)
    if confirm:
        Path(job['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': concern}))
        f._audit_result(job, False)
        job = f._new_job('audit', fn, 'm/judge', bytes=200, confirm=True)
        f._launch(job)
    snapshot = f.run_dir / 'audit/snapshots' / str(job['n']) / artifact
    if change == 'missing':
        snapshot.unlink()
    elif artifact == 'identity.json':
        metadata = json.loads(snapshot.read_text())
        metadata['open_findings'] = ['metadata was changed after dispatch']
        snapshot.write_text(json.dumps(metadata))
    else:
        snapshot.write_bytes(b'changed snapshot bytes')
    Path(job['audit_path']).write_text(json.dumps({'verdict': verdict, 'findings': ['unbound finding']}))
    f._audit_result(job, False)
    assert f.state['defects'][fn] == concern
    assert not f.pending
    assert fn in f.audit_queue
    assert f.state['audits'][fn].get('verdict') not in ('ok', 'defect')


@pytest.mark.parametrize('case,expected', [
    ('quarantined', 'acceptance-quarantined'),
    ('conflict', 'conflict'),
    ('missing-run', 'fresh-review-required'),
])
def test_claim_cli_returns_bounded_expected_status_json(root, case, expected):
    f, _, job, _ = prepared_candidate(root)
    if case == 'quarantined':
        f.state['quarantined'] = {job['fn']: {'reason': 'unresolved acceptance'}}
        f.save()
    elif case == 'conflict':
        farm.claim_review(f.run_dir, root, job['fn'], 'reviewed-once')
    run = f.run_dir if case != 'missing-run' else root / 'missing-run'
    scripts = root / 'scripts'
    scripts.mkdir()
    script = scripts / 'cmre_farm.py'
    script.write_bytes(MODULE.read_bytes())
    result = farm.subprocess.run(
        [sys.executable, str(script), 'review', str(run), job['fn'], '--claim', 'reviewed-once'],
        capture_output=True, text=True,
        env=dict(os.environ, PYTHONPATH=str(MODULE.parents[1])))
    assert result.returncode == 2
    assert 'Traceback' not in result.stderr
    view = json.loads(result.stdout)
    assert view['status'] == expected
    assert 'review_receipt' not in view
    assert len(view.get('reason', '')) <= 300


def test_coordinator_direct_cmre_permissions_are_read_only():
    from fnmatch import fnmatchcase
    agent = farm.coordinator_agent()
    rules = re.findall(r'\{ action: "shell", resource: "([^"]+)", effect: allow \}', agent)
    for command in ('accept fn_00401000 --c', 'retract-c fn_00401000', 'work retry fn_00401000',
                    'work block fn_00401000', 'task publish task-1', 'test fn_00401000 --c'):
        assert not any(fnmatchcase(f'{farm.PY} -m cmre {command}', rule) for rule in rules)
    for command in ('status', 'pipeline survey', 'work show fn_00401000', 'job status receipt.json'):
        assert any(fnmatchcase(f'{farm.PY} -m cmre {command}', rule) for rule in rules)


def test_queued_adoption_uses_immutable_proposal_not_later_source_path_bytes(root):
    f, cmre, job, canonical = prepared_candidate(root)
    queued_bytes = Path(job['source']).read_bytes()
    reviewed_request(f, job['fn'], 'adopt', source=job['source'])
    Path(job['source']).write_text(GOOD.replace('a + 1', 'a + 2') + '// PASS changed after queue\n')
    f._requests()
    f._collect()
    f._accept()
    assert cmre.accepted == [job['fn']]
    assert canonical.read_bytes() == queued_bytes


@pytest.mark.parametrize('artifact', ['source.c', 'hints.json', 'identity.json', 'verdict'])
@pytest.mark.parametrize('change', ['missing', 'drift'])
def test_confirmation_requires_intact_first_review_artifacts(root, artifact, change):
    f, _, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1, confirm_model='m/judge')
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    (root / f'recovery/hints/{fn}.json').write_bytes(b'{"arg_count":1}')
    f.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(f._inputs(fn))}
    f.state['defects'][fn] = ['legacy concern']
    first = seed_unconfirmed_audit(f, fn, ['legacy concern'])
    confirm = f._new_job('audit', fn, 'm/judge', bytes=200, confirm=True)
    f._launch(confirm)
    path = (Path(first['audit_path']) if artifact == 'verdict'
            else f.run_dir / 'audit/snapshots' / str(first['n']) / artifact)
    if change == 'missing':
        path.unlink()
    else:
        path.write_bytes(b'first review evidence changed')
    Path(confirm['audit_path']).write_text('{"verdict":"ok","findings":[]}')
    f._audit_result(confirm, False)
    assert f.state['defects'][fn] == ['legacy concern']
    assert fn in f.audit_queue
    assert not f.pending
    assert f.state['audits'][fn].get('verdict') != 'ok'


def test_invalid_old_snapshot_cannot_invalidate_newer_bound_review(root):
    f, _, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1)
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(f._inputs(fn))}
    old = f._new_job('audit', fn, 'm/old', bytes=200)
    f._launch(old)
    current = f._new_job('audit', fn, 'm/current', bytes=200)
    f._launch(current)
    Path(current['audit_path']).write_text('{"verdict":"ok","findings":[]}')
    f._audit_result(current, False)
    bound = json.loads(json.dumps(f.state['audits'][fn]))
    (f.run_dir / 'audit/snapshots' / str(old['n']) / 'source.c').unlink()
    Path(old['audit_path']).write_text('{"verdict":"defect","findings":["old stale finding"]}')
    f._audit_result(old, False)
    assert f.state['audits'][fn] == bound
    assert not f.state['defects'] and not f.pending
    assert fn not in f.audit_queue


@pytest.mark.parametrize('when', ['admission', 'verification'])
def test_queued_candidate_copy_hash_is_checked_before_use(root, when):
    f, cmre, job, canonical = prepared_candidate(root)
    request = reviewed_request(f, job['fn'], 'adopt', source=job['source'])
    queued = json.loads(request.read_text())
    if when == 'verification':
        f.verifier = ManualExecutor()
        f._requests()
    Path(queued['source']).write_text(GOOD.replace('a + 1', 'a + 2') + '// PASS tampered queue copy\n')
    if when == 'admission':
        f._requests()
    else:
        f.verifier.run_all()
        f._collect()
    assert canonical.read_bytes() == b'prior canonical'
    assert cmre.tests == [] and cmre.accepted == []
    assert not f.accept_queue


def test_prepared_journal_does_not_own_external_matching_candidate_bytes(root):
    f, cmre, job, canonical = prepared_candidate(root)
    f.verifier = ManualExecutor()
    f._verify(job, False)
    external = Path(job['source']).read_bytes()
    canonical.write_bytes(external)
    f.verifier.run_all()
    f._collect()
    assert canonical.read_bytes() == external
    assert cmre.accepted == []
    assert job['fn'] in f.state['parked']


def test_audit_and_defect_prompts_keep_callee_c_navigation_only(root):
    audit = _job(root, 'audit')
    audit['bytes'] = 200
    for prompt in (farm.audit_prompt(root, audit), farm.confirm_prompt(root, audit), farm.defect_agent()):
        assert 'Callee C is navigation only' in prompt
        assert 'never authority or proof' in prompt
        assert 'original instructions at preferred-image VAs' in prompt
        assert '0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86' in prompt
        assert 'not callee composition or runtime proof' in prompt


def test_generated_authority_uses_complete_target_sha256_pin(root):
    from cmre import env
    assert re.fullmatch(r'[0-9a-f]{64}', env.TARGET_SHA256)
    audit = _job(root, 'audit')
    audit['bytes'] = 200
    translate = _job(root, 'translate')
    translate['bytes'] = 200
    texts = [
        '\n'.join(farm.AUDIT_AUTHORITY),
        farm.audit_prompt(root, audit),
        farm.confirm_prompt(root, audit),
        farm.fix_prompt(root, _job(root, 'fix')),
        farm.translate_prompt(root, translate),
        farm.defect_agent(),
    ]
    for text in texts:
        assert re.findall(r'SHA-256 ([0-9a-f]+)', text) == [env.TARGET_SHA256]


def test_fresh_spec_queued_legacy_adopt_cannot_rebase_onto_new_acceptance(root):
    f, cmre, job, canonical = prepared_candidate(root)
    f._verify(job, False)
    f._collect()
    newer = canonical.read_bytes()
    old_handfix = root / 'handfix.c'
    old_handfix.write_text(GOOD.replace('a + 1', 'a + 2') + '// PASS fake gate\n')
    request = farm._request(f.run_dir, job['fn'], 'adopt', source=str(old_handfix))
    f._requests()
    assert request.exists() and not f.verifying
    f._accept()
    f._requests()
    f._collect()
    f._accept()
    assert canonical.read_bytes() == newer
    assert cmre.accepted == [job['fn']]
    assert any(e['event'] == 'request-failed' for e in events(f.run_dir))


def test_fresh_spec_bundle_changed_after_dispatch_never_binds_or_schedules_fix(root):
    f, cmre, _ = make(root, [], writes(GOOD), workers=0)
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(f._inputs(fn))}
    cmre.brief = lambda fn: 'original listing'
    job = f._new_job('audit', fn, 'm/a', bytes=200)
    f._launch(job)
    (root / job['bundle']).write_text('a different source and listing, not dispatched inputs')
    Path(job['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': ['changed bundle']}))
    f._audit_result(job, False)
    f.save()
    assert not f.pending
    assert farm.review_paths(f.run_dir, root, fn)['status'] == 'fresh-review-required'


def test_fresh_spec_deleted_terminal_verdict_is_reaudited_on_restart(root):
    f, cmre, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1)
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(f._inputs(fn))}
    job = f._new_job('audit', fn, 'm/a', bytes=200)
    f._launch(job)
    Path(job['audit_path']).write_text('{"verdict":"ok"}')
    f._audit_result(job, False)
    f.save()
    Path(job['audit_path']).unlink()
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert fn in resumed.audit_queue
    assert resumed.state['audits'][fn].get('verdict') != 'ok'


def test_queued_review_receipt_cannot_rebase_after_another_acceptance(root):
    f, cmre, job, canonical = prepared_candidate(root)
    receipt = review_receipt(f)  # actual review of the prior canonical inputs
    reviewed = farm.review_request_data(receipt, f.run_dir, root, job['fn'])
    f._verify(job, False)
    f._collect()
    newer = canonical.read_bytes()
    hand = root / 'old-handfix.c'
    hand.write_text(GOOD.replace('a + 1', 'a + 2') + '// PASS\n')
    farm._request(f.run_dir, job['fn'], 'adopt', source=str(hand), **reviewed)
    f._requests()
    f._accept()
    f._requests()
    f._collect()
    f._accept()
    assert canonical.read_bytes() == newer
    assert cmre.accepted == [job['fn']]
    assert list((f.run_dir / 'requests/rejected').glob('*.json'))


@pytest.mark.parametrize('kind', ['adopt', 'dismiss'])
@pytest.mark.parametrize('changed', ['c', 'hint'])
def test_reviewed_request_rejects_source_or_hint_drift_without_mutation(root, kind, changed):
    f, cmre, job, canonical = prepared_candidate(root)
    receipt = review_receipt(f)
    reviewed = farm.review_request_data(receipt, f.run_dir, root, job['fn'])
    if changed == 'c':
        canonical.write_bytes(b'newer user canonical')
    else:
        (root / 'recovery/hints/fn_00401000.json').write_bytes(b'{"arg_count":99}')
    before = f._inputs(job['fn'])
    farm._request(f.run_dir, job['fn'], kind, source=job['source'], reason='old review', **reviewed)
    f._requests()
    assert f._inputs(job['fn']) == before
    assert not f.verifying and not cmre.accepted
    assert list((f.run_dir / 'requests/rejected').glob('*.json'))


@pytest.mark.parametrize('action', ['adopt', 'dismiss'])
def test_offline_single_action_preserves_other_two_legacy_open_defects(root, monkeypatch, action):
    f, cmre, _ = make(root, [], writes(GOOD), workers=0)
    names = ['fn_00401000', 'fn_00401100', 'fn_00401200']
    for index, fn in enumerate(names):
        (root / f'recovery/c/{fn}.c').write_text(GOOD.replace(names[0], fn))
        f.state['accepted'][fn] = {'bytes': 200, 'model': 'old/worker'}
        f.state['audits'][fn] = {'verdict': 'defect', 'findings': [f'legacy-{index}'], 'rounds': 2}
        f.state['defects'][fn] = [f'legacy-{index}']
    f.save()
    before = json.loads(f.state_path.read_text())
    hand = root / 'hand.c'
    hand.write_text(GOOD)
    reviewed_request(f, names[0], action, source=str(hand), reason='fresh review')
    monkeypatch.setattr(farm, 'ROOT', root)
    monkeypatch.setattr(farm, 'CmreAdapter', lambda: cmre)
    result = farm.apply_requests_now(f.run_dir)
    disk = json.loads(f.state_path.read_text())
    for state in (result.state, disk):
        assert set(state['defects']) == set(names[1:])
        for fn in names[1:]:
            assert state['defects'][fn] == before['defects'][fn]
            assert state['audits'][fn] == before['audits'][fn]
            assert state['accepted'][fn] == before['accepted'][fn]


@pytest.mark.parametrize('artifact', ['verdict', 'source_snapshot', 'hints_snapshot', 'bundle'])
@pytest.mark.parametrize('verdict', ['ok', 'defect'])
def test_restart_requeues_terminal_evidence_drift_without_dismissing_concerns(root, artifact, verdict):
    f, cmre, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1)
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(f._inputs(fn))}
    cmre.brief = lambda fn: 'dispatch listing'
    job = f._new_job('audit', fn, 'm/a', bytes=200)
    f._launch(job)
    Path(job['audit_path']).write_text(json.dumps({'verdict': verdict, 'findings': ['concern']}))
    f._audit_result(job, False)
    f.save()
    paths = farm.review_paths(f.run_dir, root, fn)['review']
    path = Path(paths['audit_path' if artifact == 'verdict' else artifact])
    path.write_bytes(b'changed after terminal verdict')
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert fn in resumed.audit_queue
    assert resumed.state['audits'][fn]['fresh_review_required']
    assert not resumed.pending
    if verdict == 'defect':
        assert resumed.state['defects'][fn] == ['concern']


def test_review_claim_cli_is_immutable_and_required_by_adopt(root, monkeypatch, capsys):
    f, _, job, canonical = prepared_candidate(root)
    monkeypatch.setattr(farm, 'ROOT', root)
    assert farm.main(['review', str(f.run_dir), job['fn'], '--claim', 'human-reviewed-1']) == 0
    claimed = json.loads(capsys.readouterr().out)
    receipt = Path(claimed['review_receipt'])
    assert Path(claimed['source']).read_bytes() == b'prior canonical'
    canonical.write_bytes(b'newer canonical')
    assert Path(claimed['source']).read_bytes() == b'prior canonical'
    original = receipt.read_bytes()
    with pytest.raises(FileExistsError):
        farm.claim_review(f.run_dir, root, job['fn'], 'human-reviewed-1')
    assert receipt.read_bytes() == original
    assert farm.main(['adopt', str(f.run_dir), job['fn'], '--source', job['source']]) == 2
    assert not list((f.run_dir / 'requests').glob('*.json'))


def test_documented_lookup_cli_subprocess_reads_and_claims_fake_run(root):
    f, cmre, _ = make(root, [], writes(GOOD), workers=0)
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(f._inputs(fn))}
    cmre.brief = lambda fn: 'FAKE listing for CLI smoke'
    job = f._new_job('audit', fn, 'fake/auditor', bytes=200)
    f._launch(job)
    Path(job['audit_path']).write_text('{"verdict":"ok","findings":[]}')
    f._audit_result(job, False)
    f.save()
    scripts = root / 'scripts'
    scripts.mkdir()
    copied_cli = scripts / 'cmre_farm.py'
    copied_cli.write_bytes(MODULE.read_bytes())  # fake ROOT; never inspect live recovery
    command = [sys.executable, str(copied_cli), 'review', str(f.run_dir), fn]
    plain = farm.subprocess.run(command, check=True, capture_output=True, text=True,
                                env=dict(os.environ, PYTHONPATH=str(MODULE.parents[1])))
    assert json.loads(plain.stdout)['status'] == 'bound'
    claimed = farm.subprocess.run(command + ['--claim', 'documented-cli-smoke'],
                                  check=True, capture_output=True, text=True,
                                env=dict(os.environ, PYTHONPATH=str(MODULE.parents[1])))
    view = json.loads(claimed.stdout)
    assert view['status'] == 'bound'
    assert Path(view['source']).read_text() == GOOD
    assert Path(view['review_receipt']).is_file()
    assert Path(view['review']['bundle']).name == f"{fn}-{job['n']}.md"


@pytest.mark.parametrize('kind', ['translate', 'fix'])
def test_production_private_hint_lane_requires_parent_without_touching_canonical(root, kind):
    canonical = root / 'recovery/c/fn_00401000.c'
    hint = root / 'recovery/hints/fn_00401000.json'
    canonical.write_text(GOOD)
    hint.write_bytes(b'{"arg_count":2}\n')
    before = {'c': canonical.read_bytes(), 'hint': hint.read_bytes()}
    proposal = {'arg_count': 2, 'c_hints': {'fixtures': [hint_fixture(9)]}}
    def worker(job, r):
        Path(job['source']).write_text(GOOD)
        Path(job['hint_source']).write_text(json.dumps(proposal))
    f, cmre, _ = make(root, [], worker, workers=0)
    job = seed_bound_fix(f) if kind == 'fix' else f._new_job(kind, 'fn_00401000', 'm/worker', bytes=200)
    f._launch(job)
    cfg = farm.worker_config(root, job)
    allowed = [name for name, permission in cfg['permission']['edit'].items() if permission == 'allow']
    assert not any('recovery/hints' in name.replace('\\', '/') for name in allowed)
    assert job['hint_source'].replace('\\', '/') in [name.replace('\\', '/') for name in allowed]
    assert 'Canonical recovery/hints is read-only' in f.launcher.started[0][1]
    f._reap()
    f._collect()
    f._accept()
    assert f._inputs(job['fn']) == before
    assert cmre.tests == [] and cmre.accepted == []
    retained = json.loads(Path(f.state['hint_proposals'][job['fn']]['receipt']).read_text())
    assert retained['dispatch_identity'] == farm._identity(before)
    assert json.loads(bytes.fromhex(retained['proposal'])) == proposal
    assert any(e['event'] == 'hint-evidence-needed' for e in events(f.run_dir))


def test_supervised_legacy_reaudit_preserves_open_backlog_in_memory_and_on_disk(root):
    f, cmre, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1)
    names = ['fn_00401000', 'fn_00401100', 'fn_00401200']
    for fn in names:
        f.state['accepted'][fn] = {'bytes': 200}
        f.state['audits'][fn] = {'verdict': 'defect', 'findings': [fn], 'rounds': 2}
        f.state['defects'][fn] = [fn]
    f.save()
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert set(resumed.audit_queue) == set(names)
    assert not resumed.pending
    for state in (resumed.state, json.loads(f.state_path.read_text())):
        assert state['defects'] == {fn: [fn] for fn in names}
        assert all(state['audits'][fn]['fresh_review_required'] for fn in names)


def test_reviewed_adoption_cannot_clear_a_new_concern_added_during_verification(root):
    f, cmre, job, canonical = prepared_candidate(root)
    reviewed_request(f, job['fn'], 'adopt', source=job['source'])
    f._requests()
    f._collect()
    f.state['defects'][job['fn']] = ['new concern not covered by the claim']
    f._accept()
    assert cmre.accepted == []
    assert canonical.read_bytes() == b'prior canonical'
    assert f.state['defects'][job['fn']] == ['new concern not covered by the claim']


def test_audit_ok_for_old_concern_set_never_dismisses_new_open_finding(root):
    f, _, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1)
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(f._inputs(fn))}
    f.state['defects'][fn] = ['legacy concern']
    job = f._new_job('audit', fn, 'm/a', bytes=200)
    f._launch(job)
    assert 'legacy concern' in f.launcher.started[-1][1]
    f.state['defects'][fn].append('new unreviewed concern')
    Path(job['audit_path']).write_text('{"verdict":"ok"}')
    f._audit_result(job, False)
    assert f.state['defects'][fn] == ['legacy concern', 'new unreviewed concern']
    assert fn in f.audit_queue
    assert not f.pending


@pytest.mark.parametrize('had_prior', [True, False])
def test_spec_unknown_post_ledger_acceptance_preserves_inputs_and_quarantines(root, had_prior):
    f, cmre, job, canonical = prepared_candidate(root)
    if not had_prior:
        canonical.unlink()
        job['_snap']['c'] = None
    request = farm._request(f.run_dir, job['fn'], 'adopt', source=job['source'])
    job['_request'] = str(request)
    hint = root / 'recovery/hints/fn_00401000.json'
    hint.write_bytes(b'{"arg_count":1}')
    job['_snap']['hint'] = hint.read_bytes()
    def committed_then_died(fn, *, request):
        cmre.accepted.append(fn)  # the ledger was committed
        raise KeyboardInterrupt('death after ledger commit')
    cmre.accept = committed_then_died
    # Same explicit unavailability as the production adapter.
    cmre.acceptance_identity = farm.CmreAdapter(root).acceptance_identity
    f._verify(job, False)
    f._collect()
    candidate = canonical.read_bytes()
    with pytest.raises(KeyboardInterrupt):
        f._accept()
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert canonical.read_bytes() == candidate
    assert hint.read_bytes() == b'{"arg_count":1}'
    assert job['fn'] in resumed.state['quarantined']
    resumed.tick()
    again = farm.Farm(f.run_dir, root, cmre, None, f.config)
    again.tick()
    assert cmre.accepted == [job['fn']]
    assert request.exists()


def test_spec_unowned_hint_created_before_verify_is_never_deleted(root):
    f, cmre, job, canonical = prepared_candidate(root)
    hint = root / 'recovery/hints/fn_00401000.json'
    hint.write_bytes(b'{"arg_count":99}')  # external writer, not this job
    f._verify(job, False)
    f._collect()
    assert hint.read_bytes() == b'{"arg_count":99}'
    assert canonical.read_bytes() == b'prior canonical'
    assert cmre.tests == [] and cmre.accepted == []


def test_spec_accept_error_without_nonacceptance_proof_never_rolls_back(root):
    f, cmre, job, canonical = prepared_candidate(root)
    cmre.acceptance_identity = farm.CmreAdapter(root).acceptance_identity
    def committed_then_error(fn, *, request):
        cmre.accepted.append(fn)
        return 1, 'error after ledger write'
    cmre.accept = committed_then_error
    f._verify(job, False)
    f._collect()
    before = canonical.read_bytes()
    f._accept()
    assert canonical.read_bytes() == before
    assert job['fn'] in f.state['quarantined']
    assert not f.accept_queue


@pytest.mark.parametrize('output,expected', [('Writer active: lease', 2), ('error after ledger write', 1)])
def test_spec_real_adapter_reserves_busy_code_for_explicit_contention(root, output, expected):
    adapter = farm.CmreAdapter(root)
    adapter._cli = lambda *args: (2, output, '')
    assert adapter.accept('fn_00401000', request=root / 'bound-request.json')[0] == expected


@pytest.mark.parametrize('failure', ['infra', 'limited'])
@pytest.mark.parametrize('kind', ['fix', 'translate', 'audit'])
def test_spec_running_fix_provider_failure_preserves_newer_user_hints(root, failure, kind):
    f, _, _ = make(root, [], writes(GOOD), workers=0)
    hint = root / 'recovery/hints/fn_00401000.json'
    hint.write_bytes(b'{"arg_count":1}')
    if kind == 'fix':
        (root / 'recovery/c/fn_00401000.c').write_text(GOOD)
        job = seed_bound_fix(f)
    else:
        job = f._new_job(kind, 'fn_00401000', 'm/strong', bytes=200)
    f._launch(job)
    hint.write_bytes(b'{"arg_count":99}')  # newer unrelated edit after dispatch
    if failure == 'infra':
        f._infra(job, 'auth failure')
    else:
        f._limited(job, 'rate limit', 60)
    assert hint.read_bytes() == b'{"arg_count":99}'
    assert not f.pending


def test_review_locator_binds_first_and_confirmation_paths(root, monkeypatch, capsys):
    f, _, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1, confirm_model='m/judge')
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(f._inputs(fn))}
    f.cmre.brief = lambda fn: 'BRIEF with listing'
    first = f._new_job('audit', fn, 'm/a', bytes=200)
    f._launch(first)
    Path(first['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': ['first']}))
    f._audit_result(first, False)
    f.save()
    before = farm.review_paths(f.run_dir, root, fn)
    assert before['status'] == 'bound'
    assert Path(before['review']['audit_path']) == Path(first['audit_path'])
    assert Path(before['review']['bundle']).name == f"{fn}-{first['n']}.md"
    failed_confirm = f._new_job('audit', fn, 'm/b', bytes=200, confirm=True)
    f._launch(failed_confirm)
    f._audit_result(failed_confirm, False)  # no verdict; keep the valid first-audit locator
    f.save()
    assert Path(farm.review_paths(f.run_dir, root, fn)['review']['audit_path']) == Path(first['audit_path'])
    confirm = f._new_job('audit', fn, 'm/b', bytes=200, confirm=True)
    f._launch(confirm)
    Path(confirm['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': ['confirmed']}))
    f._audit_result(confirm, False)
    f.save()
    monkeypatch.setattr(farm, 'ROOT', root)
    assert farm.main(['review', str(f.run_dir), fn]) == 0
    located = json.loads(capsys.readouterr().out)
    assert located['status'] == 'bound'
    assert Path(located['review']['audit_path']) == Path(confirm['audit_path'])
    assert Path(located['first_review']['audit_path']) == Path(first['audit_path'])
    assert Path(located['review']['source_snapshot']).read_text() == GOOD
    # A per-job verdict is evidence, not an editable current-verdict alias.
    Path(confirm['audit_path']).write_text('{"verdict":"ok"}')
    assert farm.review_paths(f.run_dir, root, fn)['status'] == 'fresh-review-required'
    (root / f'recovery/c/{fn}.c').write_bytes(b'new canonical source')
    assert farm.review_paths(f.run_dir, root, fn)['status'] == 'fresh-review-required'


def test_status_never_reconciles_or_changes_interrupted_inputs(root, monkeypatch, capsys):
    f, _, job, canonical = prepared_candidate(root)
    f._verify(job, False)
    before = canonical.read_bytes()
    journal = Path(job['_transaction']).read_bytes()
    state = f.state_path.read_bytes()
    monkeypatch.setattr(farm, 'ROOT', root)
    assert farm.main(['status', str(f.run_dir), '--json']) == 0
    assert json.loads(capsys.readouterr().out)['quarantined'] == {}
    assert canonical.read_bytes() == before
    assert Path(job['_transaction']).read_bytes() == journal
    assert f.state_path.read_bytes() == state


def test_private_hint_redispatch_archives_draft_without_claiming_canonical_ownership(root):
    f, _, _ = make(root, [], writes(GOOD), workers=0)
    hint = root / 'recovery/hints/fn_00401000.json'
    hint.write_bytes(b'{"arg_count":1}')
    (root / 'recovery/c/fn_00401000.c').write_text(GOOD)
    job = seed_bound_fix(f)
    f._launch(job)
    private = Path(job['hint_source'])
    private.write_bytes(b'{"arg_count":1,"c_hints":{"notes":"draft"}}')
    # A retry gets a new immutable dispatch and leaves the old private draft in place.
    draft = private.read_bytes()
    old_dispatch = json.loads(json.dumps(job['_fix_dispatch']))
    f._retry_worker(job)
    again = f.pending.pop()
    f._launch(again)
    assert again['n'] != job['n']
    assert private.read_bytes() == draft
    assert Path(again['hint_source']).read_bytes() == hint.read_bytes()
    assert hashlib.sha256((Path(old_dispatch['snapshot']) / 'identity.json').read_bytes()).hexdigest() == old_dispatch['snapshot_metadata_sha256']
    hint.write_bytes(b'{"arg_count":99}')
    f._infra(again, 'auth failure')
    assert hint.read_bytes() == b'{"arg_count":99}'
    assert not f.pending


@pytest.mark.parametrize('changed', ['source_snapshot', 'hints_snapshot', 'bundle', 'canonical_hints'])
def test_review_locator_rejects_changed_artifacts_without_writing_state(root, changed):
    f, _, _ = make(root, [], writes(GOOD), workers=0)
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    f.cmre.brief = lambda fn: 'BRIEF with listing'
    f.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(f._inputs(fn))}
    job = f._new_job('audit', fn, 'm/a', bytes=200)
    f._launch(job)
    Path(job['audit_path']).write_text('{"verdict":"ok","findings":[]}')
    f._audit_result(job, False)
    f.save()
    located = farm.review_paths(f.run_dir, root, fn)
    assert located['status'] == 'bound'
    path = (root / f'recovery/hints/{fn}.json' if changed == 'canonical_hints'
            else Path(located['review'][changed]))
    path.write_bytes(b'changed by another writer')
    state = f.state_path.read_bytes()
    assert farm.review_paths(f.run_dir, root, fn)['status'] == 'fresh-review-required'
    assert f.state_path.read_bytes() == state


def test_documented_and_generated_agents_use_supported_review_lookup():
    docs = (MODULE.parents[1] / 'docs/agent/FARM.md').read_text(encoding='utf-8')
    for prompt in (farm.coordinator_agent(), farm.defect_agent(), docs):
        assert 'scripts/cmre_farm.py review RUN fn_X' in prompt
    assert 'scripts/cmre_farm.py review *' in farm.defect_agent()
    assert 'unbound_legacy_evidence' in docs
    assert 'No journal/snapshot' in docs and 'verified archive' in docs


def test_review_locator_marks_legacy_and_source_drift_unbound(root):
    f, _, _ = make(root, [], writes(GOOD), workers=0)
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    legacy = f.run_dir / f'audit/{fn}.json'
    legacy.parent.mkdir()
    legacy.write_text('{"verdict":"defect"}')
    f.state['accepted'][fn] = {'bytes': 200}
    f.state['audits'][fn] = {'verdict': 'defect', 'findings': ['legacy']}
    f.save()
    located = farm.review_paths(f.run_dir, root, fn)
    assert located['status'] == 'fresh-review-required'
    assert str(legacy.resolve()) in located['unbound_legacy_evidence']
    assert 'review' not in located
    assert 'fresh' in located['instruction']


def prepared_candidate(root):
    f, cmre, _ = make(root, [], writes(GOOD), workers=0)
    canonical = root / 'recovery/c/fn_00401000.c'
    canonical.write_bytes(b'prior canonical')
    job = f._new_job('translate', 'fn_00401000', 'm/cheap', bytes=200)
    source = root / 'candidate.c'
    source.write_text(GOOD)
    job.update(source=str(source), _snap={'c': canonical.read_bytes(), 'hint': None})
    f._register_dispatch(job)  # synthetic fixture models the real pre-launch boundary
    f.save()
    return f, cmre, job, canonical


def review_receipt(f, fn='fn_00401000'):
    name = f"claim-{len(list((f.run_dir / 'reviews').glob('*')))}"
    return farm.claim_review(f.run_dir, f.root, fn, name)['review_receipt']


def reviewed_request(f, fn, kind, **data):
    receipt = review_receipt(f, fn)
    return farm._request(f.run_dir, fn, kind,
                         **farm.review_request_data(receipt, f.run_dir, f.root, fn), **data)


def seed_unconfirmed_audit(f, fn, findings, model='fake/first-auditor'):
    """Build real dispatch artifacts for a fake first verdict, not an unbound state dictionary."""
    launcher = f.launcher
    f.launcher = FakeLauncher(f.root, writes(GOOD))
    first = f._new_job('audit', fn, model, bytes=f.state['accepted'][fn]['bytes'])
    f._launch(first)
    Path(first['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': findings}))
    f._audit_result(first, False)
    f.running = [row for row in f.running if row[0] is not first]
    f.launcher = launcher
    f.save()
    return first


def seed_bound_fix(f, fn='fn_00401000'):
    """Authorize a running fix through retained fake review evidence, not a bare job dictionary."""
    assert f._inputs(fn)['c'] is not None
    assert f.config.confirm_model is None
    f.state['accepted'].setdefault(fn, {'bytes': 200, 'identity': farm._identity(f._inputs(fn))})
    launcher = f.launcher
    f.launcher = FakeLauncher(f.root, writes(GOOD))
    first = f._new_job('audit', fn, 'fake/first-auditor', bytes=200)
    f._launch(first)
    Path(first['audit_path']).write_text(json.dumps({
        'verdict': 'defect', 'findings': [{'detail': 'FAKE reviewed concern', 'evidence': 'FAKE listing'}]}))
    f._audit_result(first, False)
    f.running = [row for row in f.running if row[0] is not first]
    f.launcher = launcher
    fix = next(job for job in f.pending if job['kind'] == 'fix' and job['fn'] == fn)
    f.pending.remove(fix)
    return fix


def test_serial_accept_rechecks_candidate_identity(root):
    f, cmre, job, canonical = prepared_candidate(root)
    f._verify(job, False)
    f._collect()
    canonical.write_bytes(b'new unrelated user work')
    f._accept()
    assert cmre.accepted == []
    assert canonical.read_bytes() == b'new unrelated user work'


@pytest.mark.parametrize('phase', ['installed', 'accepting'])
@pytest.mark.parametrize('newer', [False, True])
def test_interrupted_candidate_restores_only_owned_bytes(root, phase, newer):
    f, cmre, job, canonical = prepared_candidate(root)
    f._verify(job, False)
    if phase == 'accepting':
        def die(fn, *, request):
            raise KeyboardInterrupt('death during acceptance')
        cmre.accept = die
        f._collect()
        with pytest.raises(KeyboardInterrupt):
            f._accept()
    if newer:
        canonical.write_bytes(b'new unrelated user work')
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert canonical.read_bytes() == (b'new unrelated user work' if newer else b'prior canonical')
    assert 'fn_00401000' not in resumed.state['accepted']


def test_death_after_accept_reconciles_explicit_ledger_identity(root):
    f, cmre, job, canonical = prepared_candidate(root)
    identity = {'c': hashlib.sha256(Path(job['source']).read_bytes()).hexdigest(), 'hint': None}
    def accepted_then_died(fn, *, request):
        cmre.acceptance_identity = lambda fn: identity
        raise KeyboardInterrupt('after ledger write, before return')
    cmre.accept = accepted_then_died
    f._verify(job, False)
    f._collect()
    with pytest.raises(KeyboardInterrupt):
        f._accept()
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert canonical.read_text() == GOOD
    assert 'fn_00401000' in resumed.state['accepted']


@pytest.mark.parametrize('failure', ['limited', 'infra', 'defect'])
def test_stale_audit_never_restores_or_fixes_newer_canonical(root, failure):
    f, _, launcher = make(root, [], writes(GOOD), workers=0, audit_workers=1)
    fn = 'fn_00401000'
    canonical = root / f'recovery/c/{fn}.c'
    canonical.write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200}
    job = f._new_job('audit', fn, 'm/strong', bytes=200)
    f._launch(job)
    canonical.write_bytes(b'new unrelated user work')
    if failure == 'limited':
        f._limited(job, 'rate limit', 60)
    elif failure == 'infra':
        f._infra(job, 'auth')
    else:
        Path(job['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': ['old defect']}))
        f._audit_result(job, False)
    assert canonical.read_bytes() == b'new unrelated user work'
    assert not f.pending
    assert fn not in f.state['defects']
    assert fn in f.audit_queue


@pytest.mark.parametrize('identity', [None, {'c': 'old', 'hint': None}])
def test_restart_requeues_terminal_audit_without_current_identity(root, identity):
    f, cmre, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1)
    fn = 'fn_00401000'
    f.state['accepted'][fn] = {'bytes': 200}
    f.state['audits'][fn] = {'verdict': 'ok', 'rounds': 2}
    if identity is not None:
        f.state['audits'][fn]['identity'] = identity
    f.save()
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert fn in resumed.audit_queue


def test_offline_adopt_accepts_without_dispatch_from_persistent_controls(root, monkeypatch):
    f, cmre, job, canonical = prepared_candidate(root)
    cmre.items = [item('fn_00401100')]
    control = f.run_dir / 'control.json'
    control.write_text(json.dumps({'workers': 24, 'audit_workers': 8,
                                   'ladder': ['m/live'], 'audit_model': 'm/audit',
                                   'confirm_model': 'm/confirm'}))
    before = control.read_bytes()
    request = reviewed_request(f, job['fn'], 'adopt', source=job['source'])
    monkeypatch.setattr(farm, 'ROOT', root)
    monkeypatch.setattr(farm, 'CmreAdapter', lambda: cmre)

    # These assertions also show that any dispatch failure comes AFTER acceptance.
    try:
        result = farm.apply_requests_now(f.run_dir)
    finally:
        assert cmre.accepted == [job['fn']]
        assert canonical.read_text() == GOOD
        assert not request.exists()
    assert result.launcher is None
    assert not result.running
    assert len(cmre.tests) >= 2  # candidate and installed-source gates still run
    assert control.read_bytes() == before
    assert json.loads(result.state_path.read_text())['accepted'][job['fn']]
    events, _ = farm._read_events(f.run_dir / 'events.jsonl', 0)
    assert not any(e['event'] == 'dispatch' for e in events)


def test_offline_lock_precedes_state_read(root, monkeypatch):
    f, _, _ = make(root, [], writes(GOOD), workers=0)
    f.save()
    with farm.RunLock(f.run_dir):
        with pytest.raises(RuntimeError, match='supervisor'):
            farm.apply_requests_now(f.run_dir)


def test_offline_without_requests_never_probes_or_claims_supply(root, monkeypatch):
    f, cmre, _ = make(root, [item('fn_00401100')], writes(GOOD), workers=0)
    f.save()
    control = f.run_dir / 'control.json'
    control.write_text(json.dumps({'workers': 24, 'audit_workers': 8, 'ladder': ['m/live'],
                                   'audit_model': 'm/audit', 'confirm_model': 'm/confirm',
                                   'request_only': False}))
    before = control.read_bytes()
    def unexpected(*args, **kwargs):
        pytest.fail('offline requests must not probe models or fetch/claim model supply')
    monkeypatch.setattr(cmre, 'frontier', unexpected)
    monkeypatch.setattr(cmre, 'claim', unexpected)
    monkeypatch.setattr(cmre, 'brief', unexpected, raising=False)
    monkeypatch.setattr(farm, 'probe', unexpected)
    monkeypatch.setattr(farm, 'make_launcher', unexpected)
    monkeypatch.setattr(farm, 'ROOT', root)
    monkeypatch.setattr(farm, 'CmreAdapter', lambda: cmre)

    result = farm.apply_requests_now(f.run_dir)
    # A subsequent control reload also cannot remove the process-local restriction.
    result.tick()
    assert result.launcher is None
    assert not result.running and not result.pending
    assert not cmre.tests and not cmre.accepted
    assert result.config.workers == 24 and result.config.audit_workers == 8
    assert result.config.ladder == ['m/live']
    assert control.read_bytes() == before


@pytest.mark.parametrize('lane', ['translate', 'fix', 'audit', 'confirm'])
def test_request_only_blocks_model_lanes_but_later_live_run_dispatches(root, lane):
    fn = 'fn_00401000'
    if lane == 'fix':
        f, cmre, _, _ = bound_automatic_fix(root)
        launcher = f.launcher
    else:
        f, cmre, launcher = make(root, [item(fn)] if lane == 'translate' else [],
                                 writes(GOOD), workers=0,
                                 confirm_model='m/confirm' if lane == 'confirm' else None)
        if lane in ('audit', 'confirm'):
            (root / f'recovery/c/{fn}.c').write_text(GOOD)
            f.state['accepted'][fn] = {'bytes': 200}
            if lane == 'confirm':
                seed_unconfirmed_audit(f, fn, [{'detail': 'FAKE concern', 'evidence': 'FAKE listing'}])
    f.save()
    launcher.started.clear()
    control = f.run_dir / 'control.json'
    control.write_text(json.dumps({'workers': 24 if lane in ('translate', 'fix') else 0,
                                   'audit_workers': 8 if lane in ('audit', 'confirm') else 0,
                                   'ladder': ['m/live'], 'audit_model': 'm/audit',
                                   'confirm_model': 'm/confirm', 'request_only': False}))
    before = control.read_bytes()
    offline = farm.Farm(f.run_dir, root, cmre, None, f.config)
    offline.pending.extend(f.pending)  # retained in-process fix work is not dispatched either
    pending = list(offline.pending)
    for _ in range(2):
        offline.tick()
    assert not launcher.started and not offline.running
    assert offline.pending == pending
    assert control.read_bytes() == before

    # No offline marker is persisted: the next real supervisor still honors model controls.
    live = farm.Farm(f.run_dir, root, cmre, launcher, f.config)
    live.pending.extend(offline.pending)
    live.tick()
    assert len(launcher.started) == 1
    dispatched = launcher.started[0][0]
    assert dispatched['kind'] == ('audit' if lane == 'confirm' else lane)
    assert bool(dispatched.get('confirm')) == (lane == 'confirm')
    assert dispatched['model'] == ('m/confirm' if lane == 'confirm' else
                                    'm/audit' if lane == 'audit' else 'm/live')
    assert control.read_bytes() == before


def test_offline_drains_second_request_queued_during_verification(root, monkeypatch):
    from concurrent.futures import Future
    f, cmre, job, canonical = prepared_candidate(root)
    cmre.items = [item('fn_00401100')]
    control = f.run_dir / 'control.json'
    control.write_text(json.dumps({'workers': 24, 'audit_workers': 8,
                                   'ladder': ['m/live'], 'audit_model': 'm/audit'}))
    before = control.read_bytes()
    monkeypatch.setattr(farm, 'ROOT', root)
    monkeypatch.setattr(farm, 'CmreAdapter', lambda: cmre)
    reviewed_request(f, job['fn'], 'adopt', source=job['source'])
    calls = []
    queued_second = []
    class Deferred:
        def submit(self, fn, *args):
            future = Future()
            calls.append((future, fn, args))
            return future
    monkeypatch.setattr(farm, 'InlineExecutor', Deferred)
    def progress(_):
        # Competing offline writer must fail even while the first is verifying.
        with pytest.raises(RuntimeError, match='supervisor'):
            farm.apply_requests_now(f.run_dir)
        for future, fn, args in calls:
            if not future.done():
                outcome = fn(*args)
                # Review the installed candidate while its verification is still
                # pending, not the older dispatch baseline that it replaced.
                if len(calls) == 1 and not queued_second:
                    reviewed_request(f, job['fn'], 'adopt', source=job['source'])
                    queued_second.append(True)
                future.set_result(outcome)
                break
    monkeypatch.setattr(farm.time, 'sleep', progress)
    result = farm.apply_requests_now(f.run_dir)
    assert len(cmre.accepted) == 2
    assert not result.verifying and not result.accept_queue
    assert not list((f.run_dir / 'requests').glob('*.json'))
    assert result.launcher is None and not result.running
    assert control.read_bytes() == before


@pytest.mark.parametrize('boundary', ['prepared', 'installing', 'installed', 'accepted', 'complete'])
def test_candidate_journal_crash_boundaries_keep_prior_bytes_and_identity(root, monkeypatch, boundary):
    f, cmre, job, canonical = prepared_candidate(root)
    hints = root / 'recovery/hints/fn_00401000.json'
    before = b'{ "arg_count": 1, "c_hints": {} }\n'
    after = before  # canonical hints are now read-only; proposals have their own lane
    hints.write_bytes(after)
    job['_snap']['hint'] = before
    original_json = farm._durable_json
    def checkpoint(path, data):
        original_json(path, data)
        if Path(path).parent.name == 'transactions' and data['phase'] == boundary:
            # This is exactly the disk record available to a new controller.
            assert bytes.fromhex(data['prior']['c']) == b'prior canonical'
            assert bytes.fromhex(data['prior']['hint']) == before
            assert bytes.fromhex(data['candidate']['hint']) == after
            assert data['identity'] == farm._identity(farm._decode_snapshot(data['candidate']))
            raise KeyboardInterrupt(boundary)
    monkeypatch.setattr(farm, '_durable_json', checkpoint)
    with pytest.raises(KeyboardInterrupt):
        f._verify(job, False)
        f._collect()
        f._accept()
    monkeypatch.setattr(farm, '_durable_json', original_json)
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    accepted = boundary in ('accepted', 'complete')
    assert canonical.read_text() == (GOOD if accepted else 'prior canonical')
    assert hints.read_bytes() == (after if accepted else before)
    assert ('fn_00401000' in resumed.state['accepted']) == accepted
    # Repeated startup must not reapply acceptance or duplicate accounting.
    again = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert again.state['attempts'] == resumed.state['attempts']


@pytest.mark.parametrize('evidence', [None, {'c': 'wrong candidate', 'hint': None}])
def test_accepting_without_matching_ledger_proof_requires_reverification(root, evidence):
    f, cmre, job, canonical = prepared_candidate(root)
    cmre.acceptance_identity = lambda fn: evidence
    def die(fn, *, request):
        raise KeyboardInterrupt()
    cmre.accept = die
    f._verify(job, False)
    f._collect()
    with pytest.raises(KeyboardInterrupt):
        f._accept()
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert canonical.read_text() == GOOD
    assert not resumed.state['accepted']
    assert job['fn'] in resumed.state['quarantined']
    assert any(e['event'] == 'acceptance-quarantined' for e in events(f.run_dir))


def test_resume_collect_pending_is_resumable_with_existing_prefix_and_receipt(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes('bad'), workers=1)
    executor = ManualExecutor()
    f.verifier = executor
    fn = 'fn_00401000'
    canonical = root / f'recovery/c/{fn}.c'
    canonical.write_bytes(b'prior unfinished source prefix')
    retained = f.run_dir / 'retained.c'
    retained.write_text(GOOD)
    receipt = f.run_dir / 'inherited-receipt.json'
    receipt.write_bytes(b'{"green":true,"inherited":"parent"}')
    (f.run_dir / 'resume-candidates.json').write_text(json.dumps({
        'schema': 'cmre-farm-resume-candidates/1',
        'receipt': str(receipt),
        'sources': {fn: {'path': retained.name, 'sha256': hashlib.sha256(retained.read_bytes()).hexdigest(),
                         'collect': True, 'hints_sha256': None}}}))
    f.tick()
    assert f.verifying
    assert fn not in f.state.get('resume_collected', {})
    resumed = farm.Farm(f.run_dir, root, cmre, f.launcher, f.config)
    assert canonical.read_bytes() == b'prior unfinished source prefix'
    assert resumed._resume_seed(fn, collect=True) == retained.read_bytes()
    resumed._dispatch()
    resumed._collect()
    resumed._accept()
    assert cmre.accepted == [fn]
    assert resumed.state['resume_collected'][fn] == hashlib.sha256(retained.read_bytes()).hexdigest()
    assert receipt.read_bytes() == b'{"green":true,"inherited":"parent"}'


@pytest.mark.parametrize('changed', ['c', 'hint', 'findings'])
def test_confirmation_is_bound_to_snapshot_and_first_verdict(root, changed):
    f, _, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1, confirm_model='m/judge')
    fn = 'fn_00401000'
    canonical = root / f'recovery/c/{fn}.c'
    canonical.write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200}
    f.state['audits'][fn] = {'rounds': 1, 'verdict': 'unconfirmed', 'findings': ['first'],
                             'identity': farm._identity(f._inputs(fn))}
    job = f._new_job('audit', fn, 'm/judge', confirm=True, bytes=200)
    f._launch(job)
    if changed == 'c':
        canonical.write_bytes(b'new user source')
    elif changed == 'hint':
        (root / f'recovery/hints/{fn}.json').write_bytes(b'{}')
    else:
        f.state['audits'][fn]['findings'] = ['different first verdict']
    Path(job['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': ['obsolete']}))
    f._audit_result(job, False)
    assert fn not in f.state['defects']
    assert not f.pending
    assert f.state['audit_history'][fn][-1].get('outcome') == 'stale' or any(
        row.get('outcome') == 'stale' for row in f.state['audit_history'][fn])


def test_delayed_fix_does_not_apply_old_findings_to_new_source(root):
    f, _, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1)
    fn = 'fn_00401000'
    canonical = root / f'recovery/c/{fn}.c'
    canonical.write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200}
    f._defect(fn, ['old finding'])
    canonical.write_bytes(b'new user source')
    assert f._next_pending() is None
    assert canonical.read_bytes() == b'new user source'
    assert f.state['defects'][fn] == ['old finding']  # unresolved, not silently dismissed


def test_acceptance_checks_hints_after_hold_retry(root):
    f, cmre, job, canonical = prepared_candidate(root)
    f._verify(job, False)
    f._collect()
    cmre.holds[job['fn']] = {'state': 'hold', 'reason': 'old concern'}
    # Use an actual hard-hold name, rather than assuming its spelling.
    cmre.holds[job['fn']]['state'] = next(iter(farm.HARD_HOLDS))
    original_retry = cmre.retry
    def retry(fn, reason):
        original_retry(fn, reason)
        (root / f'recovery/hints/{fn}.json').write_bytes(b'{"user":"newer"}')
    cmre.retry = retry
    f._accept()
    assert cmre.accepted == []
    assert (root / 'recovery/hints/fn_00401000.json').read_bytes() == b'{"user":"newer"}'
    assert canonical.read_bytes() == b'prior canonical'


def test_audit_snapshots_and_verdict_paths_are_per_job(root):
    f, _, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1)
    fn = 'fn_00401000'
    canonical = root / f'recovery/c/{fn}.c'
    hints = root / f'recovery/hints/{fn}.json'
    canonical.write_bytes(GOOD.encode())
    hints.write_bytes(b'{"c_hints":{}}')
    first = f._new_job('audit', fn, 'm/a', bytes=200)
    f._launch(first)
    Path(first['audit_path']).write_bytes(b'{"verdict":"ok"}')
    canonical.write_bytes(b'newer source')
    hints.write_bytes(b'newer hints')
    second = f._new_job('audit', fn, 'm/b', bytes=200)
    f._launch(second)
    assert (root / first['review_source']).read_bytes() == GOOD.encode()
    assert (root / first['review_hints']).read_bytes() == b'{"c_hints":{}}'
    assert (root / second['review_source']).read_bytes() == b'newer source'
    assert first['audit_path'] != second['audit_path']
    assert Path(first['audit_path']).read_bytes() == b'{"verdict":"ok"}'


def test_conflicted_restart_parks_instead_of_recollecting_over_userwork(root):
    f, cmre, job, canonical = prepared_candidate(root)
    cmre.items = [item(job['fn'])]
    f._verify(job, False)
    canonical.write_bytes(b'newer unrelated user source')
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert job['fn'] in resumed.state['parked']
    assert resumed._next_item() is None
    assert canonical.read_bytes() == b'newer unrelated user source'


def test_corrupt_candidate_identity_stops_reconciliation_without_writes(root):
    f, cmre, job, canonical = prepared_candidate(root)
    f._verify(job, False)
    path = Path(job['_transaction'])
    data = json.loads(path.read_text())
    data['identity']['c'] = 'wrong'
    path.write_text(json.dumps(data))
    before = canonical.read_bytes()
    with pytest.raises(RuntimeError, match='journal identity mismatch'):
        farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert canonical.read_bytes() == before


@pytest.mark.parametrize("include_held", [False, True])
def test_real_adapter_forwards_explicit_held_claim_option(monkeypatch, tmp_path, include_held):
    from cmre import work
    calls = []
    monkeypatch.setattr(work, "claim", lambda *args, **kwargs: calls.append((args, kwargs)))
    farm.CmreAdapter(tmp_path).claim("fn_00401000", "farm-t", 2, include_held=include_held)
    assert calls == [(([0x401000], "farm-t", 2), {"include_held": include_held})]


class FakeCmre:
    """Gate stand-in: a source passes when it contains PASS or is GOOD."""

    def __init__(self, items, holds=None, accept_codes=None):
        self.items = list(items)
        self.holds = dict(holds or {})
        self.accept_codes = list(accept_codes or [])
        self.claimed, self.accepted, self.retried, self.tests = [], [], [], []
        self.root = None

    def frontier(self, limit, min_bytes, max_bytes, include_held, owner):
        out = [i for i in self.items if i.bytes >= min_bytes and (max_bytes is None or i.bytes <= max_bytes)]
        if not include_held:
            out = [i for i in out if i.fn not in self.holds]
        return out[:limit]

    def claim(self, fn, owner, hours, *, include_held):
        if fn == 'fn_badbad00':
            raise ValueError('exhausted its five-attempt budget')
        self.claimed.append(fn)

    def release(self, owner, fns):
        pass

    def test(self, fn, source=None):
        path = Path(source) if source else self.root / 'recovery/c' / f'{fn}.c'
        self.tests.append((fn, source))
        text = path.read_text() if path.exists() else ''
        ok = text == GOOD or 'PASS' in text
        return {'status': 'pass' if ok else 'fail', 'problems': [] if ok else ['mismatch'],
                'builds': {'-O0': {'status': 'pass' if ok else 'mismatch', 'detail': 'case 7 eax'}}}

    def accept(self, fn, *, request):
        code = self.accept_codes.pop(0) if self.accept_codes else 0
        if code == 0:
            self.accepted.append(fn)
        return code, 'ok' if code == 0 else 'busy'

    def acceptance_identity(self, fn):
        # This fake's complete ledger is accepted[]. Absence is authoritative
        # here, unlike an unavailable production ledger query.
        return {'status': 'not-accepted'} if fn not in self.accepted else None

    def current_execution_identity(self):
        return json.loads(json.dumps(X87_EXECUTION_IDENTITY))

    def hold(self, fn):
        return self.holds.get(fn)

    def held(self, fns):
        return {fn for fn in fns if fn in self.holds}

    def retry(self, fn, reason):
        self.retried.append(fn)
        self.holds.pop(fn, None)


class FakeHandle:
    def __init__(self, job, behaviour, root):
        self.job, self.behaviour, self.root, self.done, self.killed = job, behaviour, root, False, False

    def poll(self):
        if not self.done:
            self.done = True
            self.behaviour(self.job, self.root)
        return 0

    def kill(self):
        self.killed = True


class FakeLauncher:
    def __init__(self, root, behaviour):
        self.root, self.behaviour, self.started = root, behaviour, []

    def start(self, job, prompt, config):
        self.started.append((job, prompt, config))
        return FakeHandle(job, self.behaviour, self.root)


def writes(text):
    def behaviour(job, root):
        if job['kind'] == 'audit':
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).write_text(json.dumps({'verdict': 'ok', 'findings': []}))
            return
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(text(job) if callable(text) else text)
    return behaviour


@pytest.fixture(autouse=True)
def _restore_claim_owner(monkeypatch):
    # Farm() exports CMRE_CLAIM_OWNER process-wide (right for the real farm process);
    # restore it so it never leaks into the rest of the suite.
    monkeypatch.setenv('CMRE_CLAIM_OWNER', 'pytest-placeholder')


@pytest.fixture
def root(tmp_path):
    r = tmp_path / 'repo'
    (r / 'recovery/c').mkdir(parents=True)
    (r / 'recovery/hints').mkdir(parents=True)
    return r


def make(root, items, behaviour, holds=None, accept_codes=None, **cfg):
    cmre = FakeCmre(items, holds, accept_codes)
    cmre.root = root
    launcher = FakeLauncher(root, behaviour)
    options = dict(workers=2, ladder=['m/cheap', 'm/strong'], audit_workers=0, audit_model='m/strong',
                   attempt_minutes=30, hours=1, min_bytes=0, max_bytes=None, include_held=False, owner='farm-t')
    options.update(cfg)
    run = root / 'run'
    f = farm.Farm(run, root, cmre, launcher, farm.FarmConfig(**options), clock=lambda: 0.0)
    return f, cmre, launcher


def item(fn, size=200):
    return farm.Item(fn, size, True, 0, 'mod.cpp')


@pytest.mark.parametrize("valid", [False, True])
def test_hash_bound_interrupted_candidate_seeds_private_worker(root, valid):
    f, _, _ = make(root, [], writes(GOOD))
    saved = f.run_dir / "retained.c"
    saved.write_bytes(GOOD.encode())
    (f.run_dir / "resume-candidates.json").write_text(json.dumps({
        "schema": "cmre-farm-resume-candidates/1",
        "sources": {"fn_00401000": {
            "path": saved.name,
            "sha256": hashlib.sha256(saved.read_bytes()).hexdigest() if valid else "0" * 64,
        }},
    }))
    job = f._new_job("translate", "fn_00401000", "m/cheap", bytes=200)
    f._launch(job)
    assert Path(job["source"]).exists() is valid
    if valid:
        assert Path(job["source"]).read_bytes() == saved.read_bytes()
        assert job["seeded"] is True
    assert not (root / "recovery/c/fn_00401000.c").exists()
    assert f.state_path.exists()  # dispatch heartbeat precedes the next tick


def test_interrupted_seed_cannot_escape_run_directory(root):
    f, _, _ = make(root, [], writes(GOOD))
    outside = root / "outside.c"
    outside.write_bytes(GOOD.encode())
    (f.run_dir / "resume-candidates.json").write_text(json.dumps({
        "schema": "cmre-farm-resume-candidates/1",
        "sources": {"fn_00401000": {
            "path": "../outside.c", "sha256": hashlib.sha256(outside.read_bytes()).hexdigest(),
        }},
    }))
    job = f._new_job("translate", "fn_00401000", "m/cheap", bytes=200)
    f._launch(job)
    assert not Path(job["source"]).exists()


def test_run_saves_heartbeat_before_first_tick(root, monkeypatch):
    f, _, _ = make(root, [], writes(GOOD))
    def first_tick():
        assert f.state_path.exists()
        f.frontier = []
    monkeypatch.setattr(f, "tick", first_tick)
    f.run(poll=0)


def test_resume_job_numbers_do_not_overwrite_interrupted_logs(root):
    f, _, _ = make(root, [], writes(GOOD))
    f._event('dispatch', fn='fn_00401000', kind='translate', model='m/cheap', n=21)
    f.save()
    resumed, _, _ = make(root, [], writes(GOOD))
    assert resumed._new_job('translate', 'fn_00401000', 'm/cheap')['n'] == 22


def test_resume_collect_retests_retained_candidate_without_model(root):
    f, cmre, launcher = make(root, [item('fn_00401000')], writes('bad'), workers=1)
    retained = f.run_dir / 'retained.c'
    retained.write_text(GOOD)
    (f.run_dir / 'resume-candidates.json').write_text(json.dumps({
        'schema': 'cmre-farm-resume-candidates/1',
        'sources': {'fn_00401000': {
            'path': retained.name, 'sha256': hashlib.sha256(retained.read_bytes()).hexdigest(),
            'collect': True, 'hints_sha256': None,
        }},
    }))
    drain(f)
    assert launcher.started == []
    assert cmre.accepted == ['fn_00401000']
    assert len(cmre.tests) == 2
    assert retained.read_text() == GOOD


def test_resume_collection_refuses_changed_hints(root):
    f, _, _ = make(root, [], writes(GOOD))
    retained = f.run_dir / 'retained.c'
    retained.write_text(GOOD)
    (f.run_dir / 'resume-candidates.json').write_text(json.dumps({
        'schema': 'cmre-farm-resume-candidates/1',
        'sources': {'fn_00401000': {
            'path': retained.name, 'sha256': hashlib.sha256(retained.read_bytes()).hexdigest(),
            'collect': True, 'hints_sha256': None,
        }},
    }))
    (root / 'recovery/hints/fn_00401000.json').write_text('{}')
    assert f._resume_seed('fn_00401000', collect=True) is None


def test_interrupted_dispatch_is_not_reported_as_a_live_worker(tmp_path):
    path = tmp_path / 'events.jsonl'
    path.write_text('\n'.join(json.dumps(e) for e in [
        {'event': 'dispatch', 'fn': 'fn_00401000', 'kind': 'translate', 'model': 'm/cheap', 'n': 21},
        {'event': 'interrupted', 'fn': 'fn_00401000', 'n': 21},
    ]) + '\n')
    assert farm.running_from_events(path) == []


def test_run_lock_rejects_competing_supervisor_and_releases(tmp_path):
    with farm.RunLock(tmp_path):
        with pytest.raises(RuntimeError, match='supervisor'):
            with farm.RunLock(tmp_path):
                pytest.fail('second supervisor acquired run')
    with farm.RunLock(tmp_path):
        assert (tmp_path / 'supervisor.json').exists()


def drain(f, ticks=20):
    for _ in range(ticks):
        f.tick()


def test_queue_parsing_reads_cmre_next_lines():
    text = 'fn_0055f620    394B waits:5  display.cpp\nfn_0094bc6e     13B ready    \nnoise\n'
    got = farm.parse_queue(text)
    assert [(i.fn, i.bytes, i.ready, i.waits) for i in got] == [('fn_0055f620', 394, False, 5), ('fn_0094bc6e', 13, True, 0)]


def test_passing_worker_source_is_copied_tested_and_accepted(root):
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD))
    drain(f)
    assert cmre.accepted == ['fn_00401000']
    assert (root / 'recovery/c/fn_00401000.c').read_text() == GOOD
    assert f.state['accepted']['fn_00401000']['bytes'] == 200
    assert f.summary()['accepted_bytes'] == 200


def test_failed_attempt_restores_previous_source_and_escalates_model(root):
    (root / 'recovery/c/fn_00401000.c').write_text('/* WIP old */')
    attempts = []

    def behaviour(job, r):
        attempts.append(job['model'])
        text = 'PASS' if job['model'] == 'm/strong' else 'bad'
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD.replace('a + 1', 'a + 1 /*PASS*/') if text == 'PASS' else text)

    f, cmre, launcher = make(root, [item('fn_00401000')], behaviour, workers=1)
    drain(f)
    assert attempts == ['m/cheap', 'm/strong']
    assert cmre.accepted == ['fn_00401000']
    # The second prompt carries the first attempt's failure and its source path.
    second_prompt = launcher.started[1][1]
    assert 'Previous attempt' in second_prompt and 'mismatch' in second_prompt


def test_failure_restores_exact_prior_bytes(root):
    (root / 'recovery/c/fn_00401000.c').write_text('/* WIP old */')
    f, cmre, _ = make(root, [item('fn_00401000')], writes('bad'), ladder=['m/cheap'])
    drain(f)
    assert cmre.accepted == []
    assert (root / 'recovery/c/fn_00401000.c').read_text() == '/* WIP old */'
    assert 'fn_00401000' in f.state['parked']


def test_failure_without_prior_source_removes_candidate(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes('bad'), ladder=['m/cheap'])
    drain(f)
    assert not (root / 'recovery/c/fn_00401000.c').exists()


def hint_fixture(value):
    return {'regs': {name: 0 for name in ('eax', 'ecx', 'edx', 'ebx', 'esp', 'ebp', 'esi', 'edi', 'eflags')},
            'stack': [value, 0], 'arena_template': 0, 'arena_writes': {}, 'globals': {}, 'import_seed': 1}


@pytest.mark.parametrize('field,value', [
    ('cases', 1),
    ('min_coverage', 0.000001),
    ('discovery', 0),
    ('original_x87', {}),
    ('lift_original_calls', {}),
    ('fixtures', [hint_fixture(0)]),
])
def test_worker_shared_hint_change_is_rejected_before_gate_and_restored(root, field, value):
    hint = root / 'recovery/hints/fn_00401000.json'
    shared = {'cases': 800, 'min_coverage': 0.7, 'discovery': 32, 'arg_count': 2,
              'fixtures': [hint_fixture(7)]}
    before = (json.dumps(shared, indent=2).replace('\n', '\r\n') + '\r\n').encode()
    hint.write_bytes(before)

    def behaviour(job, r):
        changed = json.loads(before)
        changed[field] = value
        changed['c_hints'] = {'fixtures': [hint_fixture(9)]}
        Path(job['hint_source']).write_text(json.dumps(changed))
        writes(GOOD)(job, r)

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, ladder=['m/cheap'])
    drain(f)
    assert cmre.tests == []
    assert cmre.accepted == []
    assert hint.read_bytes() == before
    assert not (root / 'recovery/c/fn_00401000.c').exists()
    assert 'hint' in f.state['parked']['fn_00401000']['detail']


@pytest.mark.parametrize('proposal', [
    {'cases': 1},
    {'min_coverage': 0.000001},
    {'discovery': 0},
    {'original_x87': {}},
    {'lift_original_calls': {}},
    {'fixtures': [{'stack': [9]}]},
])
@pytest.mark.parametrize('existing', [False, True])
def test_worker_invalid_c_hints_are_rejected_before_gate_and_restore_snapshot(root, proposal, existing):
    hint = root / 'recovery/hints/fn_00401000.json'
    before = b'{"cases": 800, "arg_count": 2}\r\n' if existing else None
    if existing:
        hint.write_bytes(before)

    def behaviour(job, r):
        changed = json.loads(before) if existing else {}
        changed['c_hints'] = proposal
        Path(job['hint_source']).write_text(json.dumps(changed))
        writes(GOOD)(job, r)

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, ladder=['m/cheap'])
    drain(f)
    assert cmre.tests == []
    assert cmre.accepted == []
    assert (hint.read_bytes() if hint.exists() else None) == before
    assert 'hint' in f.state['parked']['fn_00401000']['detail']


@pytest.mark.parametrize('replacement', [None, b'{', b'[]', b'{}'])
def test_worker_cannot_delete_corrupt_or_remove_shared_hints(root, replacement):
    hint = root / 'recovery/hints/fn_00401000.json'
    before = b'{"cases": 800, "fixtures": []}\r\n'
    hint.write_bytes(before)

    def behaviour(job, r):
        if replacement is None:
            Path(job['hint_source']).unlink()
        else:
            Path(job['hint_source']).write_bytes(replacement)
        writes(GOOD)(job, r)

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, ladder=['m/cheap'])
    drain(f)
    assert cmre.tests == []
    assert cmre.accepted == []
    assert hint.read_bytes() == before


def test_worker_hint_edit_is_reverted_on_failure_and_kept_on_success(root):
    hint = root / 'recovery/hints/fn_00401000.json'
    shared = {'cases': 800, 'min_coverage': 0.7, 'discovery': 32, 'arg_count': 2,
              'original_x87': {}, 'lift_original_calls': {}, 'fixtures': [hint_fixture(7)]}
    hint.write_text(json.dumps(shared))
    edited = {**shared, 'c_hints': {'fixtures': [hint_fixture(9)]}}
    starts = []

    def behaviour(job, r):
        starts.append(json.loads(Path(job['hint_source']).read_text()))
        Path(job['hint_source']).write_text(json.dumps(edited))
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text('bad' if job['model'] == 'm/cheap' else GOOD)

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, workers=1)
    f.tick()  # dispatch cheap
    f.tick()  # reap failure, restore, dispatch strong
    assert json.loads(hint.read_text()) in (shared, edited)
    drain(f)
    assert cmre.accepted == []
    assert starts == [shared]  # a valid private proposal stops for the parent's authorized input lane
    assert json.loads(hint.read_text()) == shared
    receipt = Path(f.state['hint_proposals']['fn_00401000']['receipt'])
    retained = json.loads(receipt.read_text())
    assert json.loads(bytes.fromhex(retained['proposal'])) == edited
    assert retained['dispatch_identity']['hint'] == hashlib.sha256(hint.read_bytes()).hexdigest()


def test_mechanical_rejections_never_reach_the_gate(root):
    asm = GOOD.replace('return a + 1;', '__asm__("nop"); return a + 1;')
    f, cmre, _ = make(root, [item('fn_00401000')], writes(asm), ladder=['m/cheap'])
    drain(f)
    assert cmre.accepted == []
    assert all(source is not None for _, source in cmre.tests) or cmre.tests == []
    assert 'inline assembly' in json.dumps(f.state['parked'])


@pytest.mark.parametrize('text,needle', [
    ('__attribute__((optimize("O2"))) u32 cm_fn_00401000(void){return 0;}', 'optimize attribute'),
    ('u32 cm_fn_00401000(void){ register u32 x = 0; return x; }', 'register'),
    ('u32 cm_fn_00401000(void){ return CM_ORIGINAL_CALL(0); }', 'CM_ORIGINAL_CALL'),
    ('/* WIP: case 3 */ u32 cm_fn_00401000(void){return 0;}', 'WIP'),
    ('u32 cm_fn_00409999(void){return 0;}', 'entry'),
])
def test_mechanical_problems(text, needle):
    problems = farm.mechanical_problems('fn_00401000', text)
    assert any(needle in p for p in problems), problems


def test_comment_mentions_do_not_trip_checks():
    text = '/* no asm here; register pressure note */\nu32 cm_fn_00401000(void){return 0;}\n'
    assert farm.mechanical_problems('fn_00401000', text) == []


@pytest.mark.parametrize('directive', [
    '#pragma GCC diagnostic ignored "-Wreturn-type"',
    '# pragma GCC diagnostic warning "-Wreturn-type"',
    '#pragma clang diagnostic ignored "-Wreturn-type"',
    '#pragma GCC diagnostic /* reason */ ignored "-Wreturn-type"',
    '#pragma GCC diagnostic ignored \\\n "-Wreturn-type"',
    '_Pragma("GCC diagnostic ignored \\"-Wreturn-type\\"")',
    '_Pragma ( /* reason */ "GCC diagnostic warning \\"-Wreturn-type\\"" )',
    '_Pragma(L"clang diagnostic ignored \\"-Wreturn-type\\"")',
])
def test_return_type_diagnostic_weakening_rejected(directive):
    source = (directive + '\n'
              'static u32 __attribute__((noinline)) capture_eax(void) {}\n'
              'u32 cm_fn_00401000(void) { return capture_eax(); }\n')
    assert farm.mechanical_problems('fn_00401000', source) == [
        'return-type diagnostic weakening permits undefined register capture']


def test_return_type_diagnostic_rejection_never_runs_gate(root):
    source = ('#pragma GCC diagnostic ignored "-Wreturn-type"\n'
              'static u32 __attribute__((noinline)) capture_eax(void) {}\n'
              'u32 cm_fn_00401000(void) { return capture_eax(); }\n')
    f, cmre, _ = make(root, [item('fn_00401000')], writes(source), ladder=['m/cheap'])
    drain(f)
    assert cmre.tests == []
    assert cmre.accepted == []
    assert 'return-type diagnostic weakening' in json.dumps(f.state['parked'])


@pytest.mark.parametrize('decoy', [
    '/*\n#pragma GCC diagnostic ignored "-Wreturn-type"\n*/',
    '// _Pragma("GCC diagnostic ignored \\"-Wreturn-type\\"")',
    r'const char *s = "#pragma GCC diagnostic ignored \"-Wreturn-type\"";',
    r'const char *s = "_Pragma(\"GCC diagnostic ignored \\\"-Wreturn-type\\\"\")";',
    '#pragma GCC diagnostic error "-Wreturn-type"',
    '#pragma GCC diagnostic ignored "-Wunused-parameter"',
    '#pragma GCC diagnostic ignored "-Wreturn-type-extra"',
    '_Pragma("GCC diagnostic error \\"-Wreturn-type\\"")',
])
def test_return_type_checker_ignores_decoys_and_nonweakening_pragmas(decoy):
    assert farm.mechanical_problems('fn_00401000', decoy + '\n' + GOOD) == []


@pytest.mark.parametrize('abi', ['regparm(3), stdcall', 'thiscall', 'fastcall'])
def test_explicit_entry_abi_is_not_undefined_register_capture(abi):
    source = (f'u32 __attribute__(({abi})) cm_fn_00401000(u32 eax, u32 edx, u32 ecx, u32 stack) '
              '{ return eax + edx + ecx + stack; }\n')
    assert farm.mechanical_problems('fn_00401000', source) == []


def test_busy_accept_is_retried_not_failed(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD), accept_codes=[2, 0])
    drain(f)
    assert cmre.accepted == ['fn_00401000']


def test_held_items_are_retried_before_accept_and_flagged_for_audit(root):
    holds = {'fn_00401000': {'state': 'evidence-needed', 'reason': 'return width unknown'}}
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), holds=holds,
                             include_held=True, audit_workers=1)
    drain(f)
    assert 'return width unknown' in launcher.started[0][1]
    assert cmre.retried == ['fn_00401000']
    assert cmre.accepted == ['fn_00401000']
    assert f.state['accepted']['fn_00401000']['was_held'] is True
    assert f.state['audits']['fn_00401000']['verdict'] == 'ok'


def test_claim_refusal_skips_item(root):
    f, cmre, launcher = make(root, [item('fn_badbad00'), item('fn_00401000')], writes(GOOD))
    drain(f)
    assert cmre.accepted == ['fn_00401000']
    assert 'fn_badbad00' in f.state['skipped']


def test_largest_ready_first_ordering(root):
    items = [farm.Item('fn_00000001', 100, True, 0, ''), farm.Item('fn_00000002', 900, False, 2, ''),
             farm.Item('fn_00000003', 500, True, 0, '')]
    f, cmre, launcher = make(root, items, writes(GOOD), workers=1)
    f.tick()
    assert launcher.started[0][0]['fn'] == 'fn_00000003'


def test_audit_defect_dispatches_fix_with_finding(root):
    calls = []

    def behaviour(job, r):
        calls.append(job['kind'])
        if job['kind'] == 'audit':
            verdict = 'defect' if calls.count('audit') == 1 else 'ok'
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).write_text(json.dumps(
                {'verdict': verdict, 'findings': [{'line': 3, 'detail': 'byte counter widened', 'evidence': '0x401010'}]}))
            return
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD if job['kind'] == 'translate' else GOOD.replace('a + 1', 'a + 1 /*PASS fixed*/'))

    f, cmre, launcher = make(root, [item('fn_00401000')], behaviour, audit_workers=1)
    drain(f, 40)
    fix_prompts = [p for job, p, _ in launcher.started if job['kind'] == 'fix']
    assert fix_prompts and 'byte counter widened' in fix_prompts[0]
    assert cmre.accepted == ['fn_00401000', 'fn_00401000']
    assert 'PASS fixed' in (root / 'recovery/c/fn_00401000.c').read_text()
    assert f.state['audits']['fn_00401000']['verdict'] == 'ok'


def test_unfixed_defect_restores_accepted_source_and_is_reported(root):
    def behaviour(job, r):
        if job['kind'] == 'audit':
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': [{'detail': 'x'}]}))
            return
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD if job['kind'] == 'translate' else 'broken')

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, audit_workers=1)
    drain(f, 40)
    assert (root / 'recovery/c/fn_00401000.c').read_text() == GOOD
    assert 'fn_00401000' in f.summary()['open_defects']


def test_timeout_kills_worker_and_counts_failure(root):
    now = [0.0]

    class Slow(FakeHandle):
        def poll(self):
            return None

    class SlowLauncher(FakeLauncher):
        def start(self, job, prompt, config):
            h = Slow(job, None, self.root)
            self.started.append((job, prompt, config))
            self.handles.append(h)
            return h

    cmre = FakeCmre([item('fn_00401000')])
    cmre.root = root
    launcher = SlowLauncher(root, None)
    launcher.handles = []
    cfg = farm.FarmConfig(workers=1, ladder=['m/cheap'], audit_workers=0, audit_model=None, attempt_minutes=1,
                          hours=1, min_bytes=0, max_bytes=None, include_held=False, owner='farm-t')
    f = farm.Farm(root / 'run', root, cmre, launcher, cfg, clock=lambda: now[0])
    f.tick()
    now[0] = 120.0
    f.tick()
    assert launcher.handles[0].killed
    assert 'timeout' in json.dumps(f.state['parked'])


def test_deadline_stops_dispatch_and_stop_file_drains(root):
    f, cmre, launcher = make(root, [item('fn_00401000'), item('fn_00401100')], writes(GOOD), workers=1)
    (f.run_dir / 'STOP').write_text('')
    drain(f)
    assert launcher.started == []
    assert f.finished()


def test_worker_config_limits_edits_and_commands(root):
    job = {'fn': 'fn_00401000', 'kind': 'translate', 'source': str(root / 'run/src/fn_00401000.c'),
           'hint_source': str(root / 'run/hints/fn_00401000-1.json')}
    cfg = farm.worker_config(root, job)
    perm = cfg['permission']
    assert perm['edit']['*'] == 'deny'
    assert any(k.endswith('run/src/fn_00401000.c') and v == 'allow' for k, v in perm['edit'].items())
    assert perm['edit'].get('recovery/hints/fn_00401000.json', perm['edit']['*']) == 'deny'
    assert perm['edit']['run/hints/fn_00401000-1.json'] == 'allow'
    assert perm['bash']['*'] == 'deny'
    assert perm['bash']['py -3.13 -m cmre test *'] == 'allow'
    assert 'py -3.13 -m cmre accept *' not in perm['bash']
    assert cfg['tools']['task'] is False


def test_audit_config_cannot_edit_sources(root):
    job = {'fn': 'fn_00401000', 'kind': 'audit', 'audit_path': str(root / 'run/audit/fn_00401000.json')}
    perm = farm.worker_config(root, job)['permission']
    assert 'recovery/hints/fn_00401000.json' not in perm['edit']
    assert any(k.endswith('run/audit/fn_00401000.json') for k in perm['edit'])


def test_state_survives_restart(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD))
    drain(f)
    again, _, _ = make(root, [], writes(GOOD))
    assert again.summary()['accepted_bytes'] == 200


def test_real_adapter_maps_cmre_queue_items(monkeypatch):
    import types
    from cmre import queue
    real = queue.Item(0x401000, 'FUN_00401000', 321, False, 2, 'mod.cpp')
    monkeypatch.setattr(queue, 'c_frontier', lambda *a, **k: [real])
    got = farm.CmreAdapter().frontier(5, 0, None, False, 'farm-t')
    assert got == [farm.Item('fn_00401000', 321, False, 2, 'mod.cpp')]


def test_status_command_reads_state_without_inventing_a_deadline(root, capsys):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD))
    drain(f)
    assert farm.main(['status', str(f.run_dir)]) == 0
    out = capsys.readouterr().out
    assert 'accepted 1 fn' in out and 'STOPPING' not in out
    (f.run_dir / 'STOP').write_text('')
    farm.main(['status', str(f.run_dir)])
    assert 'STOPPING' in capsys.readouterr().out


def test_status_line_reports_rate_and_models(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD))
    drain(f)
    line = farm.status_line(f.summary())
    assert 'accepted 1 fn' in line and '200 B' in line and 'm/cheap' in line


def test_translate_prompt_bounds_exploration(root):
    job = {'fn': 'fn_00401000', 'kind': 'translate', 'bytes': 300, 'source': str(root / 'run/src/fn_00401000.c')}
    prompt = farm.translate_prompt(root, job)
    assert 'recovery/include/cmre.h' in prompt
    assert 'Do not search the repository' in prompt
    assert 'first complete version' in prompt


def test_worker_shell_allowlist_excludes_unbounded_recursive_search(root):
    job = {'fn': 'fn_00401000', 'kind': 'translate', 'source': str(root / 'run/src/fn_00401000.c')}
    bash = farm.worker_config(root, job)['permission']['bash']
    assert bash.get('rg *') == 'allow'
    assert 'grep *' not in bash and 'ls *' not in bash


class ScriptedHandle:
    def __init__(self, code=None, infra=None, activity=0.0):
        self.code, self.infra, self.activity, self.killed = code, infra, activity, False

    def poll(self):
        return self.code

    def kill(self):
        self.killed = True

    def infra_error(self):
        return self.infra

    def last_activity(self):
        return self.activity


class ScriptedLauncher:
    def __init__(self, plan):
        self.plan, self.started, self.handles = plan, [], []

    def start(self, job, prompt, config):
        self.started.append((job, prompt, config))
        h = self.plan(job)
        self.handles.append(h)
        return h


def scripted_farm(root, plan, clock, **cfg):
    cmre = FakeCmre([item('fn_00401000')])
    cmre.root = root
    launcher = ScriptedLauncher(plan)
    options = dict(workers=1, ladder=['m/cheap', 'm/strong'], audit_workers=0, audit_model=None,
                   attempt_minutes=30, hours=1, min_bytes=0, max_bytes=None, include_held=False,
                   owner='farm-t', stall_minutes=5)
    options.update(cfg)
    return farm.Farm(root / 'run', root, cmre, launcher, farm.FarmConfig(**options), clock=clock), cmre, launcher


def test_auth_error_marks_model_unavailable_without_spending_an_attempt(root):
    def plan(job):
        if job['model'] == 'm/cheap':
            return ScriptedHandle(code=1, infra='401 This API key has expired')
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD)
        return ScriptedHandle(code=0)

    f, cmre, launcher = scripted_farm(root, plan, lambda: 0.0)
    drain(f)
    assert [j['model'] for j, _, _ in launcher.started] == ['m/cheap', 'm/strong']
    assert cmre.accepted == ['fn_00401000']
    assert 'm/cheap' in f.summary()['unavailable_models']
    assert all(r['model'] != 'm/cheap' for r in f.state['attempts']['fn_00401000'])
    # A later item never goes to the unavailable model.
    assert farm.status_line(f.summary()).count('UNAVAILABLE') == 1


def test_stalled_worker_is_killed_before_the_attempt_timeout(root):
    now = [0.0]
    f, cmre, launcher = scripted_farm(root, lambda job: ScriptedHandle(code=None, activity=0.0),
                                      lambda: now[0], ladder=['m/cheap'])
    f.tick()
    now[0] = 4 * 60
    f.tick()
    assert not launcher.handles[0].killed
    now[0] = 6 * 60
    f.tick()
    assert launcher.handles[0].killed
    assert 'stalled' in json.dumps(f.state['parked'])


def test_probe_reports_each_model(monkeypatch, capsys):
    import subprocess as sp

    def fake_run(command, **kwargs):
        flag = '-m' if '-m' in command else '--model'
        model = command[command.index(flag) + 1]
        out = ('{"type":"text","part":{"text":"OK"}}' if model == 'm/ok' else
               '{"type":"error","error":{"name":"APIError","data":{"message":"This API key has expired"}}}')
        return sp.CompletedProcess(command, 0, stdout=out, stderr='')

    monkeypatch.setattr(farm.subprocess, 'run', fake_run)
    assert farm.main(['probe', '--ladder', 'm/ok,m/dead']) == 1
    out = capsys.readouterr().out
    assert 'm/ok: OK' in out and 'm/dead: UNAVAILABLE This API key has expired' in out


def test_status_counts_running_workers_from_events(root, capsys):
    f, cmre, launcher = make(root, [item('fn_00401000'), item('fn_00401100')], writes(GOOD), workers=2)
    f._dispatch()
    f.save()
    farm.main(['status', str(f.run_dir)])
    assert 'running 2' in capsys.readouterr().out


def test_quota_limited_dispatch_is_not_still_running(tmp_path):
    path = tmp_path / 'events.jsonl'
    rows = [
        {'event': 'dispatch', 'n': 1, 'fn': 'fn_00401000',
         'kind': 'translate', 'model': 'opencode-go/minimax-m3.1-flash'},
        {'event': 'model-limited', 'n': 1, 'provider': 'opencode-go'},
    ]
    path.write_text(''.join(json.dumps(row) + '\n' for row in rows))
    assert farm.running_from_events(path) == []


def test_clean_farm_exit_clears_historical_unfinished_dispatches(tmp_path):
    path = tmp_path / 'events.jsonl'
    rows = [
        {'event': 'dispatch', 'n': 1, 'fn': 'fn_00401000',
         'kind': 'translate', 'model': 'opencode-go/minimax-m3.1-flash'},
        {'event': 'farm-exit', 'status': 'running 0'},
    ]
    path.write_text(''.join(json.dumps(row) + '\n' for row in rows))
    assert farm.running_from_events(path) == []


def test_worker_config_sets_explicit_reasoning_effort_for_the_job_model(root):
    job = {'fn': 'fn_00401000', 'kind': 'translate', 'model': 'prov/model-x',
           'source': str(root / 'run/src/fn_00401000.c')}
    cfg = farm.worker_config(root, job, effort='low')
    assert cfg['provider'] == {'prov': {'models': {'model-x': {'options': {'effort': 'low', 'reasoningEffort': 'low'}}}}}
    assert 'provider' not in farm.worker_config(root, job, effort=None)


def test_output_cap_is_reported_distinctly(root):
    def plan(job):
        h = ScriptedHandle(code=0)
        h.finish_reason = lambda: 'length'
        return h

    f, cmre, launcher = scripted_farm(root, plan, lambda: 0.0, ladder=['m/cheap'])
    drain(f)
    assert 'output cap' in json.dumps(f.state['parked'])


def test_farm_exports_its_claim_owner_for_workers_and_retests(root, monkeypatch):
    # cmre fences --source diagnostics on a claimed function to the claim owner only.
    monkeypatch.delenv('CMRE_CLAIM_OWNER', raising=False)
    f, cmre, launcher = scripted_farm(root, lambda job: ScriptedHandle(code=0), lambda: 0.0)
    assert os.environ['CMRE_CLAIM_OWNER'] == 'farm-t'


def test_audit_is_bounded_to_brief_source_and_checklist(root):
    # 2026-11-13 smoke: mutate outlived opencode's 120 s shell timeout and the auditor
    # then wandered through run directories, the ledger and its own transcript.
    job = {'fn': 'fn_00401000', 'kind': 'audit', 'model': 'm/x',
           'audit_path': str(root / 'run/audit/fn_00401000.json')}
    prompt = farm.audit_prompt(root, job)
    assert 'mutate' not in prompt
    assert 'Do not read farm run files' in prompt
    assert 'at most 20 tool calls' in prompt  # an unbounded audit wrote no verdict in 19 min
    bash = farm.worker_config(root, job)['permission']['bash']
    assert not any('mutate' in k for k in bash)


def test_errored_audit_is_retried_once_and_reported(root):
    # 2026-11-13 smoke: an auditor wrote no verdict; the error vanished from status.
    audits = []

    def behaviour(job, r):
        if job['kind'] == 'audit':
            audits.append(job['fn'])
            return  # never writes the verdict file
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD)

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, audit_workers=1)
    drain(f, 40)
    assert audits == ['fn_00401000', 'fn_00401000']
    assert f.summary()['audit_errors'] == 1
    assert 'error 1' in farm.status_line(f.summary())


def test_unaudited_accepts_are_requeued_after_restart(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD))  # audit_workers=0: accepted, never audited
    drain(f)
    audits = []

    def behaviour(job, r):
        audits.append(job['fn'])
        Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['audit_path']).write_text(json.dumps({'verdict': 'ok', 'findings': []}))

    again, _, _ = make(root, [], behaviour, audit_workers=1)
    drain(again)
    assert audits == ['fn_00401000']


def test_live_control_uses_the_same_held_policy_for_selection_and_claim(root):
    fn = 'fn_00401000'
    f, cmre, launcher = make(root, [item(fn)], writes(GOOD), workers=1, include_held=False,
                             holds={fn: {'state': 'evidence-needed', 'reason': 'pending'}})
    claims = []
    claim = cmre.claim
    def record_claim(fn, owner, hours, *, include_held):
        claims.append(include_held)
        return claim(fn, owner, hours, include_held=include_held)
    cmre.claim = record_claim

    f.tick()
    assert not claims and not launcher.started
    (f.run_dir / 'control.json').write_text('{"include_held": true}', encoding='utf-8')
    f.tick()
    assert claims == [True]
    assert [job['fn'] for job, _, _ in launcher.started] == [fn]


def test_include_held_dispatches_unheld_items_first(root):
    # An unattended run drains the ready queue before spending attempts on held functions.
    items = [farm.Item('fn_00000001', 900, True, 0, ''), farm.Item('fn_00000002', 100, True, 0, '')]
    holds = {'fn_00000001': {'state': 'harness-blocker', 'reason': 'x87'}}
    f, cmre, launcher = make(root, items, writes(GOOD), holds=holds, include_held=True, workers=1)
    f.tick()
    assert launcher.started[0][0]['fn'] == 'fn_00000002'


def test_frontier_request_covers_the_whole_queue(root):
    # cmre orders its frontier smallest-first; a 500 limit hid most held bytes (691 items on 2026-11-13).
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD))
    limits = []
    real = cmre.frontier
    cmre.frontier = lambda limit, *a: limits.append(limit) or real(limit, *a)
    f.tick()
    assert limits and limits[0] >= 5000


def _job(root, kind='translate'):
    job = {'fn': 'fn_0000beef', 'kind': kind, 'model': 'opencode-go/deepseek-v4.1-flash', 'n': 7}
    if kind == 'audit':
        job['audit_path'] = str(root / 'CM0102/Analysis/cmre/farm/r/audit/fn_0000beef.json')
    else:
        job['source'] = str(root / 'CM0102/Analysis/cmre/farm/r/src/fn_0000beef.c')
        job['hint_source'] = str(root / 'CM0102/Analysis/cmre/farm/r/hints/fn_0000beef-7.json')
    return job


def test_v2_agent_scopes_edit_to_own_files_and_shell_to_the_allowlist(root):
    # OpenCode 2.x takes permissions from an agent definition (verified 2026-11-13:
    # exact relative edit paths and shell globs are enforced; the rest is denied).
    text = farm.v2_agent(root, _job(root))
    assert text.startswith('---\n') and 'mode: primary' in text
    assert '{ action: "*", resource: "*", effect: deny }' in text
    assert '{ action: "edit", resource: "CM0102/Analysis/cmre/farm/r/src/fn_0000beef.c", effect: allow }' in text
    assert '{ action: "edit", resource: "recovery/hints/fn_0000beef.json", effect: allow }' not in text
    assert '{ action: "edit", resource: "CM0102/Analysis/cmre/farm/r/hints/fn_0000beef-7.json", effect: allow }' in text
    assert '{ action: "shell", resource: "py -3.13 -m cmre test *", effect: allow }' in text
    assert '{ action: "shell", resource: "rg *", effect: allow }' in text
    assert 'grep *' not in text and 'ls *' not in text and 'mutate' not in text
    audit = farm.v2_agent(root, _job(root, 'audit'))
    assert 'recovery/hints' not in audit and 'audit/fn_0000beef.json' in audit


def test_v2_launcher_runs_the_job_agent_and_removes_it_when_done(root, monkeypatch):
    started = {}

    class Proc:
        pid, code = 1, None

        def poll(self):
            return self.code

    proc = Proc()

    def popen(command, **kw):
        started.update(command=command, cwd=kw['cwd'], env=kw['env'])
        return proc

    monkeypatch.setattr(farm.subprocess, 'Popen', popen)
    monkeypatch.setattr(farm.subprocess, 'run',
                        lambda command, **kw: farm.subprocess.CompletedProcess(command, 0))  # agent visible
    launcher = farm.OpencodeV2Launcher(root, root / 'run', executable='oc2')
    handle = launcher.start(_job(root), 'PROMPT', {})
    cmd = started['command']
    name = cmd[cmd.index('--agent') + 1]
    agent = root / '.opencode/agents' / f'{name}.md'
    # Shared background service: concurrent --standalone servers hung at startup and mid-step
    # (2026-11-13: 8-minute stalls on both models; the same probe took 5 s on the service).
    assert cmd[:2] == ['oc2', 'run'] and '--standalone' not in cmd and '--pure' not in cmd
    assert cmd[cmd.index('-m') + 1] == 'opencode-go/deepseek-v4.1-flash' and cmd[-1] == 'PROMPT'
    assert started['cwd'] == root and agent.exists()
    assert handle.poll() is None and agent.exists()
    proc.code = 0
    assert handle.poll() == 0 and not agent.exists()


def test_v1_launcher_raises_the_output_cap(root, monkeypatch):
    # opencode 1.18 caps output at 32k unless OPENCODE_EXPERIMENTAL_OUTPUT_TOKEN_MAX is set;
    # 5 of 8 parked production jobs hit that cap.
    seen = {}
    monkeypatch.setattr(farm.subprocess, 'Popen', lambda command, **kw: seen.update(kw) or object())
    farm.OpencodeLauncher(root, root / 'run').start(_job(root), 'P', {})
    assert int(seen['env']['OPENCODE_EXPERIMENTAL_OUTPUT_TOKEN_MAX']) >= 131072


def test_cli_selects_the_opencode_build(root):
    assert isinstance(farm.make_launcher('v2', root, root / 'run'), farm.OpencodeV2Launcher)
    assert isinstance(farm.make_launcher('v1', root, root / 'run'), farm.OpencodeLauncher)
    v2 = farm.probe_command('v2', 'm/x')
    assert v2[1:3] == ['run', '-m'] and '--standalone' not in v2 and v2[-1] == 'Reply with exactly: OK'
    assert farm.probe_command('v1', 'm/x')[:3] == ['opencode', '--pure', 'run']


def test_claim_race_with_another_farm_is_a_refusal_not_a_crash(monkeypatch):
    # 2026-11-13: two farms claimed one function at once; cmre's attempt fence raised
    # BusyError, which killed the second farm before its first dispatch.
    from cmre import coordination, work

    def busy(*a, **k):
        raise coordination.BusyError('Writer active: attempt-fences/fn_0056c570.lock')

    monkeypatch.setattr(work, 'claim', busy)
    with pytest.raises(ValueError, match='Writer active'):
        farm.CmreAdapter().claim('fn_0056c570', 'farm-t', 1, include_held=False)


def test_release_survives_a_busy_work_lock(monkeypatch):
    # Claims expire on their own; a busy queue lock must not crash the farm.
    from cmre import coordination, work
    calls = []

    def busy(owner, vas=None):
        calls.append(owner)
        raise coordination.BusyError('Writer active: work.lock')

    monkeypatch.setattr(work, 'release', busy)
    monkeypatch.setattr(farm.time, 'sleep', lambda s: None)
    farm.CmreAdapter().release('farm-t', ['fn_00401000'])
    assert len(calls) == 3


def test_v2_kill_interrupts_the_session_in_the_shared_service(root, monkeypatch):
    # Killing only the run client leaves the session running in the service.
    log = root / 'w.jsonl'
    log.write_text('{"type":"step_start","sessionID":"ses_abc123","part":{}}\n', encoding='utf-8')
    agent = root / 'agent.md'
    agent.write_text('x')
    calls = []
    monkeypatch.setattr(farm.subprocess, 'run', lambda command, **kw: calls.append(command))

    class Proc:
        pid = 42

        def poll(self):
            return None

    handle = farm.OpencodeV2Handle(Proc(), log, agent, executable='oc2')
    handle.kill()
    assert calls[0] == ['oc2', 'api', 'session.interrupt', '--param', 'sessionID=ses_abc123']
    assert not agent.exists()


def test_v2_launcher_waits_until_the_service_sees_the_new_agent(root, monkeypatch):
    # 2026-11-13: a run launched right after its agent file was written failed with
    # 'Agent not found'; the shared service picks new agent files up after a short delay.
    checks, order = [], []

    def run(command, **kw):
        checks.append(command)
        order.append('check')
        return farm.subprocess.CompletedProcess(command, 0 if len(checks) >= 3 else 1, stdout='', stderr='')

    class Proc:
        pid = 1

        def poll(self):
            return None

    monkeypatch.setattr(farm.subprocess, 'run', run)
    monkeypatch.setattr(farm.subprocess, 'Popen', lambda command, **kw: order.append('launch') or Proc())
    monkeypatch.setattr(farm.time, 'sleep', lambda s: None)
    farm.OpencodeV2Launcher(root, root / 'run', executable='oc2').start(_job(root), 'P', {})
    assert order == ['check', 'check', 'check', 'launch']
    assert checks[0][:3] == ['oc2', 'api', 'agent.get']
    assert f'location[directory]={root}' in checks[0]


def test_default_stall_limit_allows_a_long_silent_thinking_step():
    # opencode 2.x emits nothing while a step thinks; DeepSeek spent 178 s and 20k reasoning
    # tokens in one step, and 8 minutes killed legitimate steps (2026-11-13).
    assert farm.DEFAULT_STALL_MINUTES >= 15


# --- 2026-11-13 evening: throughput and the OpenCode coordinator -------------------

def audit_writer(verdicts):
    """Audit/confirm behaviour from a list of (verdict, findings), consumed in order; sources pass."""
    seen = []

    def behaviour(job, r):
        if job['kind'] == 'audit':
            seen.append(job)
            verdict, findings = verdicts.pop(0) if verdicts else ('ok', [])
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).write_text(json.dumps({'verdict': verdict, 'findings': findings}))
            return
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD if job['kind'] == 'translate' else GOOD.replace('a + 1', 'a + 1 /*PASS*/'))
    behaviour.seen = seen
    return behaviour


A = {'line': 2, 'detail': 'cmp si, cx treated as 32-bit', 'evidence': '0x401004'}
B = {'line': 3, 'detail': 'EAX return dropped', 'evidence': '0x401010 mov eax, ecx'}


def test_x87_runtime_helpers_are_skipped_by_default(root):
    # 7 of 10 production audit defects came from 0x0094xxxx-0x00952xxx runtime helpers whose
    # x87 control-word and caller-stack effects the gate cannot see (2026-11-13).
    assert any(lo <= 0x0094bc15 < hi and lo <= 0x00952f05 < hi for lo, hi in farm.DEFAULT_SKIP_RANGES)
    items = [item('fn_0094bc15', 900), item('fn_00401000', 100)]
    f, cmre, launcher = make(root, items, writes(GOOD), workers=2, skip_ranges=farm.DEFAULT_SKIP_RANGES)
    f.tick()
    assert [j['fn'] for j, _, _ in launcher.started] == ['fn_00401000']
    assert farm.parse_ranges('0x00940000-0x00953000') == [(0x00940000, 0x00953000)]
    assert farm.parse_ranges('') == []


def _docs(root):
    (root / 'docs/agent').mkdir(parents=True, exist_ok=True)
    (root / 'docs/agent/C.md').write_text(
        '# C\n## Loop\n1. brief\n2. Write `recovery/c/fn_X.c`. Rules:\n   - RULE-CMD\n3. `py -3.13 -m cmre test`\n')
    (root / 'docs/agent/FARM.md').write_text(
        '# Farm\n## Worker rules\n- WORKER-RULE\n## Audit checklist\n1. CHECK-ITEM\n## Supply lane\nSUPPLY\n')
    (root / 'recovery/include').mkdir(parents=True, exist_ok=True)
    (root / 'recovery/include/cmre.h').write_text('#define HEADER-TEXT\n')
    (root / 'drafts').mkdir(exist_ok=True)
    (root / 'drafts/fn_00401000.c').write_text('DRAFT-TEXT\n')


def test_translate_job_gets_one_bundle_with_brief_draft_header_and_rules(root):
    # Workers spent 5-8 of ~20 steps re-reading the brief, draft, header and rules (2026-11-13).
    _docs(root)
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), workers=1)
    cmre.brief = lambda fn: f'BRIEF {fn}\ndraft: drafts/{fn}.c  NEEDS-REVIEW\nlisting (VA, instruction):\n'
    f._dispatch()
    job, prompt, _ = launcher.started[0]
    text = (root / 'run/bundle/fn_00401000.md').read_text()
    for needle in ('BRIEF fn_00401000', 'DRAFT-TEXT', 'HEADER-TEXT', 'RULE-CMD', 'WORKER-RULE'):
        assert needle in text
    assert 'SUPPLY' not in text and 'CHECK-ITEM' not in text
    assert 'run/bundle/fn_00401000.md' in prompt
    assert f'cmre brief fn_00401000' not in prompt


def test_audit_bundle_carries_the_checklist_and_accepted_source(root):
    _docs(root)
    (root / 'recovery/c/fn_00401000.c').write_text(GOOD)
    job = {'fn': 'fn_00401000', 'kind': 'audit', 'model': 'm/x', 'n': 1,
           'audit_path': str(root / 'run/audit/fn_00401000.json')}
    text = farm.bundle_text(root, job, 'BRIEF fn_00401000\n')
    assert 'CHECK-ITEM' in text and 'cm_fn_00401000' in text and 'RULE-CMD' not in text


def test_bundle_survives_a_failed_brief(root):
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), workers=1)

    def broken(fn):
        raise RuntimeError('brief crashed')
    cmre.brief = broken
    f._dispatch()
    assert 'cmre brief fn_00401000' in launcher.started[0][1]


def test_a_defect_needs_a_second_auditor_before_any_fix(root):
    # MiniMax M3 flagged 8 of 8 functions including false positives (xaudit-mm-on-ds, 2026-11-13).
    behaviour = audit_writer([('defect', [A]), ('ok', [])])
    f, cmre, launcher = make(root, [item('fn_00401000')], behaviour, audit_workers=1, confirm_model='m/judge')
    drain(f, 40)
    assert 'fix' not in [j['kind'] for j, _, _ in launcher.started]
    confirm = behaviour.seen[1]
    assert confirm['model'] == 'm/judge' and confirm.get('confirm')
    confirm_prompt = [p for j, p, _ in launcher.started if j.get('confirm')][0]
    assert 'cmp si, cx treated as 32-bit' in confirm_prompt and 'false positive' in confirm_prompt
    audit = f.state['audits']['fn_00401000']
    assert audit['verdict'] == 'ok' and audit['disputed'] == [A]
    assert f.summary()['audits_disputed'] == 1 and not f.summary()['open_defects']


def test_a_confirmed_defect_fixes_only_the_confirmed_findings(root):
    behaviour = audit_writer([('defect', [A, B]), ('defect', [B]), ('ok', [])])
    f, cmre, launcher = make(root, [item('fn_00401000')], behaviour, audit_workers=1, confirm_model='m/judge')
    drain(f, 60)
    fix_prompt = [p for j, p, _ in launcher.started if j['kind'] == 'fix'][0]
    assert 'EAX return dropped' in fix_prompt and 'cmp si, cx' not in fix_prompt
    assert cmre.accepted == ['fn_00401000', 'fn_00401000']


def test_unconfirmed_defects_from_earlier_runs_are_confirmed_on_restart(root):
    first, _, _ = make(root, [], writes(GOOD), workers=0, audit_workers=1, confirm_model='m/judge')
    fn = 'fn_00401000'
    (root / f'recovery/c/{fn}.c').write_text(GOOD)
    first.state['accepted'][fn] = {'bytes': 200, 'identity': farm._identity(first._inputs(fn))}
    seed_unconfirmed_audit(first, fn, [A], 'm/x')
    behaviour = audit_writer([('ok', [])])
    f, cmre, launcher = make(root, [], behaviour, audit_workers=1, confirm_model='m/judge')
    drain(f)
    assert [j.get('confirm') for j in behaviour.seen] == [True]


class ManualExecutor:
    """Runs submitted verification only when told to."""

    def __init__(self):
        self.tasks = []

    def submit(self, fn, *args):
        from concurrent.futures import Future
        future = Future()
        self.tasks.append((future, fn, args))
        return future

    def run_all(self):
        for future, fn, args in self.tasks:
            future.set_result(fn(*args))
        self.tasks = []


def test_verification_runs_off_the_main_loop(root):
    # Gate re-tests took a median 48 s per job (smoke, 2026-11-13); run inline they block every
    # dispatch, which caps the farm near 20 workers.
    cmre = FakeCmre([item('fn_00401000'), item('fn_00401100')])
    cmre.root = root
    launcher = FakeLauncher(root, writes(GOOD))
    executor = ManualExecutor()
    cfg = farm.FarmConfig(workers=1, ladder=['m/cheap'], audit_workers=0, audit_model=None, attempt_minutes=30,
                          hours=1, min_bytes=0, max_bytes=None, include_held=False, owner='farm-t')
    f = farm.Farm(root / 'run', root, cmre, launcher, cfg, clock=lambda: 0.0, verifier=executor)
    f.tick()            # dispatch fn_00401000
    f.tick()            # it exits: verification submitted, slot freed, next item dispatched
    assert executor.tasks and cmre.accepted == []
    assert [j['fn'] for j, _, _ in launcher.started] == ['fn_00401000', 'fn_00401100']
    assert not f.finished()
    executor.run_all()
    f.tick()
    assert cmre.accepted == ['fn_00401000']


def events(run):
    path = Path(run) / 'events.jsonl'
    return [json.loads(line) for line in path.read_text().splitlines()] if path.exists() else []


def test_run_records_start_and_exit_and_supply_exhaustion(root, monkeypatch):
    monkeypatch.setattr(farm.time, 'sleep', lambda s: None)
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), workers=1)
    f.run(poll=0)
    names = [e['event'] for e in events(f.run_dir)]
    assert names[0] == 'farm-start' and names[-1] == 'farm-exit'
    assert names.count('supply-empty') == 1


def test_an_unfixable_defect_raises_an_open_defect_event(root):
    def behaviour(job, r):
        if job['kind'] == 'audit':
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': [B]}))
            return
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD if job['kind'] == 'translate' else 'broken')

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, audit_workers=1)
    drain(f, 40)
    assert [e['fn'] for e in events(f.run_dir) if e['event'] == 'open-defect'] == ['fn_00401000']


def test_wait_returns_on_a_notable_event_and_ignores_older_ones(root):
    f, cmre, _ = make(root, [], writes(GOOD))
    f._event('open-defect', fn='fn_00000001')
    f.save()
    now = [0.0]

    def sleep(seconds):
        now[0] += seconds
        if now[0] == 30:
            f._event('accepted', fn='fn_00000002', bytes=1, model='m', kind='translate')
            f._event('open-defect', fn='fn_00000003')
        (f.run_dir / 'state.json').touch()

    reason, seen = farm.wait_for(f.run_dir, minutes=10, poll=15, clock=lambda: now[0], sleep=sleep)
    assert reason == 'event' and [e['fn'] for e in seen] == ['fn_00000003']


def test_wait_times_out_and_detects_a_dead_farm(root):
    f, cmre, _ = make(root, [], writes(GOOD))
    f.save()
    now = [0.0]

    def sleep(seconds):
        now[0] += seconds
        (f.run_dir / 'state.json').touch()

    assert farm.wait_for(f.run_dir, minutes=1, poll=15, clock=lambda: now[0], sleep=sleep)[0] == 'timeout'
    old = time.time() - 3600
    os.utime(f.run_dir / 'state.json', (old, old))
    reason, _ = farm.wait_for(f.run_dir, minutes=1, poll=15, clock=lambda: 0.0, sleep=lambda s: None)
    assert reason == 'farm not running'


def test_start_detaches_a_supervised_farm(root, monkeypatch, capsys):
    seen = {}

    class Proc:
        pid = 4242

    def popen(command, **kw):
        seen['command'], seen['kw'] = command, kw
        return Proc()

    monkeypatch.setattr(farm.subprocess, 'Popen', popen)
    run = root / 'run'
    assert farm.main(['start', str(run), '--workers', '3', '--include-held']) == 0
    command = seen['command']
    assert command[command.index('supervise') + 1:] == [str(run), '--workers', '3', '--include-held']
    assert '4242' in capsys.readouterr().out
    assert seen['kw']['stdin'] is farm.subprocess.DEVNULL
    if os.name == 'nt':
        assert seen["kw"]["creationflags"] & farm.subprocess.CREATE_NO_WINDOW  # a detached farm pops a console per child
        assert seen["kw"]["creationflags"] & farm.subprocess.CREATE_BREAKAWAY_FROM_JOB


@pytest.mark.skipif(os.name != 'nt', reason='job objects are a Windows mechanism')
def test_start_leaves_a_job_that_forbids_breakaway_through_wmi(root, monkeypatch, capsys):
    # A plain child of such a job died silently with the shell or session that started it.
    def popen(command, **kw):
        raise OSError('access denied')

    launched = {}
    monkeypatch.setattr(farm.subprocess, 'Popen', popen)
    monkeypatch.setattr(farm, '_wmi_launch', lambda command, cwd: launched.update(command=command) or 777)
    run = root / 'run'
    assert farm.main(['start', str(run), '--workers', '3']) == 0
    assert launched['command'][launched['command'].index('supervise') + 1:] == [str(run), '--workers', '3']
    assert '777' in capsys.readouterr().out and 'WMI' not in str(launched['command'])


def test_adopt_request_accepts_a_hand_fix_and_reaudits_it(root):
    f, cmre, launcher = make(root, [], audit_writer([('ok', [])]), audit_workers=1, workers=0)
    f.state['accepted']['fn_00401000'] = {'bytes': 200, 'model': 'm/x', 'kind': 'translate', 'at': 0}
    f.state['audits']['fn_00401000'] = {'rounds': 1, 'verdict': 'defect', 'findings': [B]}
    f.state['defects']['fn_00401000'] = [B]
    (root / 'recovery/c/fn_00401000.c').write_text(GOOD)
    f.save()
    hand = root / 'hand.c'
    hand.write_text(GOOD.replace('a + 1', 'a + 1 /*PASS by hand*/'))
    assert farm.main(['adopt', str(f.run_dir), 'fn_00401000', '--source', str(hand),
                      '--note', 'restored EAX', '--review-receipt', review_receipt(f)]) == 0
    drain(f, 10)
    assert cmre.accepted == ['fn_00401000']
    assert 'PASS by hand' in (root / 'recovery/c/fn_00401000.c').read_text()
    assert 'fn_00401000' not in f.state['defects']
    assert f.state['audits']['fn_00401000']['verdict'] == 'ok'   # re-audited after the adopt
    assert not list((f.run_dir / 'requests').glob('*.json'))


def test_failed_adopt_keeps_the_defect_and_the_accepted_source(root):
    f, cmre, launcher = make(root, [], writes(GOOD), workers=0)
    f.state['accepted']['fn_00401000'] = {'bytes': 200, 'model': 'm/x', 'kind': 'translate', 'at': 0}
    f.state['defects']['fn_00401000'] = [B]
    (root / 'recovery/c/fn_00401000.c').write_text(GOOD)
    (root / 'bad.c').write_text('#include "cmre.h"\nu32 cm_fn_00401000(u32 a) { return a; }\n')
    f.save()   # a live farm: fresh state, no exit event
    farm.main(['adopt', str(f.run_dir), 'fn_00401000', '--source', str(root / 'bad.c'),
               '--review-receipt', review_receipt(f)])
    drain(f, 5)
    assert (root / 'recovery/c/fn_00401000.c').read_text() == GOOD
    assert 'fn_00401000' in f.state['defects'] and cmre.accepted == []
    assert any(e['event'] == 'request-failed' for e in events(f.run_dir))


def test_dismiss_request_closes_a_false_positive_with_its_reason(root):
    f, cmre, launcher = make(root, [], writes(GOOD), workers=0)
    f.state['accepted']['fn_00401000'] = {'bytes': 200, 'model': 'm/x', 'kind': 'translate', 'at': 0}
    f.state['audits']['fn_00401000'] = {'rounds': 1, 'verdict': 'defect', 'findings': [A]}
    f.state['defects']['fn_00401000'] = [A]
    (root / 'recovery/c/fn_00401000.c').write_text(GOOD)
    f.save()
    farm.main(['dismiss', str(f.run_dir), 'fn_00401000', '--reason', 'cmp si, cx is a 16-bit compare',
               '--review-receipt', review_receipt(f)])
    f.tick()
    audit = f.state['audits']['fn_00401000']
    assert audit['verdict'] == 'ok' and audit['dismissed'] == 'cmp si, cx is a 16-bit compare'
    assert 'fn_00401000' not in f.state['defects']


def test_hand_test_runs_as_the_farm_claim_owner(root, monkeypatch):
    # The defect subagent runs in the shared OpenCode service, which does not inherit the
    # coordinator shell's CMRE_CLAIM_OWNER; the farm still holds the claim.
    f, cmre, _ = make(root, [], writes(GOOD))
    f.save()
    seen = {}
    monkeypatch.setattr(farm.subprocess, 'call',
                        lambda command, **kw: seen.update(command=command, env=kw['env']) or 0)
    assert farm.main(['test', str(f.run_dir), 'fn_00401000', 'some/path.c']) == 0
    assert seen['env']['CMRE_CLAIM_OWNER'] == 'farm-t'
    assert seen['command'][-5:] == ['test', 'fn_00401000', '--c', '--source', 'some/path.c']


def test_workers_zero_run_ends_when_nothing_is_left(root, monkeypatch):
    monkeypatch.setattr(farm.time, 'sleep', lambda s: None)
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), workers=0)
    f.run(poll=0)
    assert launcher.started == []


def test_install_agents_writes_the_coordinator_and_defect_agents(root):
    paths = farm.install_agents(root)
    coordinator = (root / '.opencode/agents/cm-coordinator.md').read_text()
    defect = (root / '.opencode/agents/cm-defect.md').read_text()
    assert sorted(p.name for p in paths) == ['cm-coordinator.md', 'cm-defect.md']
    assert 'mode: primary' in coordinator and 'docs/agent/FARM.md' in coordinator
    assert '{ action: "subagent", resource: "cm-defect", effect: allow }' in coordinator
    assert '{ action: "shell", resource: "py -3.13 scripts/cmre_farm.py *", effect: allow }' in coordinator
    assert '{ action: "*", resource: "*", effect: ask }' in coordinator
    assert 'mode: subagent' in defect
    assert '{ action: "*", resource: "*", effect: deny }' in defect
    assert '{ action: "edit", resource: "CM0102/Analysis/cmre/farm/*/hand/*.c", effect: allow }' in defect
    assert '{ action: "shell", resource: "py -3.13 scripts/cmre_farm.py test *", effect: allow }' in defect
    assert not any('accept' in line for line in defect.splitlines() if 'shell' in line)


def test_summary_counts_audit_records_that_have_no_verdict_yet(root):
    # An adopted hand fix resets its audit to {'rounds': 0} until the re-audit; status crashed on it.
    f, cmre, _ = make(root, [], writes(GOOD), workers=0)
    f.state['accepted']['fn_00401000'] = {'bytes': 200, 'model': 'hand', 'kind': 'adopt', 'at': 0}
    f.state['audits']['fn_00401000'] = {'rounds': 0, 'adopted': 'restored EAX'}
    assert f.summary()['audits_ok'] == 0
    assert 'audits ok 0' in farm.status_line(f.summary())


def test_model_pass_rate_counts_translation_attempts_not_audits(root):
    # A model that is both an auditor and a ladder rung looked like 0/5 from its audit verdicts,
    # and the "drop under 20%" rule would have removed it (2026-11-13 backlog run).
    behaviour = audit_writer([('defect', [A]), ('ok', [])])
    f, cmre, launcher = make(root, [item('fn_00401000')], behaviour, audit_workers=1, confirm_model='m/strong')
    drain(f, 40)
    assert f.summary()['by_model'] == {'m/cheap': {'attempts': 1, 'passes': 1}}


# --- provider usage limits (2026-11-13: OpenCode Go 5-hour limit, HTTP 429) ---------------

GO_LIMIT_LINE = json.dumps({'type': 'error', 'sessionID': 'ses_x', 'error': {
    'type': 'provider.go-limit', 'status': 429, 'retryAfterMs': 4693000,
    'message': 'OpenCode Go 5 hour usage limit reached. Switch to a free model, or turn on Extra Usage.'}})


def limited_handle(seconds=3600):
    h = ScriptedHandle(code=1)
    h.limit_error = lambda: ('OpenCode Go 5 hour usage limit reached', seconds)
    return h


def test_handle_reads_a_usage_limit_and_its_reset_time(tmp_path):
    log = tmp_path / 'w.jsonl'
    log.write_text(GO_LIMIT_LINE + '\n')
    message, seconds = farm.OpencodeHandle(None, log).limit_error()
    assert 'usage limit' in message and seconds == 4693
    log.write_text(json.dumps({'type': 'error', 'error': {
        'type': 'provider.go-limit', 'status': 429,
        'message': 'OpenCode Go usage limit reached (5-hour, resets in 1h 19m).'}}) + '\n')
    assert farm.OpencodeHandle(None, log).limit_error()[1] == 79 * 60
    log.write_text('{"type":"step_finish","part":{"reason":"stop"}}\n')
    assert farm.OpencodeHandle(None, log).limit_error() is None


def test_usage_limit_pauses_the_provider_and_requeues_without_an_attempt(root, monkeypatch):
    # Four backlog fix attempts were spent on the limit and reported as unchanged sources.
    now = [1000.0]
    monkeypatch.setattr(farm.time, 'time', lambda: now[0])

    def plan(job):
        if job['model'].startswith('go/') and now[0] < 4600:
            return limited_handle()
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD)
        return ScriptedHandle(code=0)

    f, cmre, launcher = scripted_farm(root, plan, lambda: 0.0, ladder=['go/a', 'go/b'])
    drain(f, 5)
    assert [j['model'] for j, _, _ in launcher.started] == ['go/a']   # go/b shares the account limit
    assert 'fn_00401000' not in f.state['attempts'] and not f.state['parked']
    assert 'LIMITED go' in farm.status_line(f.summary())
    assert any(e['event'] == 'model-limited' for e in events(f.run_dir))
    now[0] = 5000.0
    drain(f, 5)
    assert cmre.accepted == ['fn_00401000']
    assert [j['model'] for j, _, _ in launcher.started] == ['go/a', 'go/a']


def test_a_limited_provider_falls_through_to_another_providers_rung(root, monkeypatch):
    monkeypatch.setattr(farm.time, 'time', lambda: 1000.0)

    def plan(job):
        if job['model'].startswith('go/'):
            return limited_handle()
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD)
        return ScriptedHandle(code=0)

    f, cmre, launcher = scripted_farm(root, plan, lambda: 0.0, ladder=['go/a', 'mm/b'])
    drain(f, 5)
    assert [j['model'] for j, _, _ in launcher.started] == ['go/a', 'mm/b']
    assert cmre.accepted == ['fn_00401000']
    assert [r['model'] for r in f.state['attempts']['fn_00401000']] == ['mm/b']


@pytest.mark.parametrize('source, accepted', [(GOOD, True), (GOOD.replace('a + 1', 'a + 2'), False)])
def test_default_terra_lane_works_when_other_providers_are_limited(root, monkeypatch, source, accepted):
    monkeypatch.setattr(farm.time, 'time', lambda: 1000.0)
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(source), workers=1,
                             ladder=list(farm.DEFAULT_LADDER), audit_workers=1,
                             audit_model=farm.DEFAULT_AUDIT_MODEL,
                             confirm_model=farm.DEFAULT_CONFIRM_MODEL)
    f.state['limited_providers'] = {
        provider: {'until': 4600, 'message': 'quota exhausted'}
        for provider in ('opencode-go', 'minimax-coding-plan', 'commandcode')}
    drain(f)
    assert launcher.started and all(j['model'] == 'openai/gpt-6.1-terra'
                                    for j, _, _ in launcher.started)
    assert cmre.accepted == (['fn_00401000'] if accepted else [])
    assert len(cmre.tests) == (2 if accepted else 1)
    if accepted:
        assert f.state['audits']['fn_00401000']['verdict'] == 'ok'
    else:
        assert 'fn_00401000' in f.state['parked']


def test_commandcode_flash_matches_go_effort_tiers_not_fast():
    go = 'opencode-go/deepseek-v4.1-flash'
    commandcode = 'commandcode/deepseek/deepseek-v4.1-flash'
    assert farm.COMMANDCODE_FLASH_MODEL == commandcode
    assert farm.DEFAULT_LADDER[1:-2] == (
        go + '#low', commandcode + '#low', go, commandcode)
    assert not any('flash-fast' in model for model in farm.DEFAULT_LADDER)


def test_default_worker_ladder_includes_free_hy4_and_separate_minimax_plan():
    assert farm.DEFAULT_LADDER[0] == 'minimax-coding-plan/MiniMax-M3.1-Flash'
    assert farm.HY4_FLASH_MODEL == 'opencode-go/hy4-flash-free'
    assert farm.DEFAULT_LADDER[-2:] == (farm.HY4_FLASH_MODEL, farm.TERRA_MODEL)
    assert 'opencode-go/minimax-m3.1-flash' not in farm.DEFAULT_LADDER


@pytest.mark.parametrize('provider, expected', [
    ('opencode-go', 'commandcode/deepseek/deepseek-v4.1-flash#low'),
    ('commandcode', 'opencode-go/deepseek-v4.1-flash#low'),
])
def test_equivalent_flash_provider_remains_available(root, monkeypatch, provider, expected):
    monkeypatch.setattr(farm.time, 'time', lambda: 1000.0)
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), workers=1,
                             ladder=list(farm.DEFAULT_LADDER))
    f.state['limited_providers'] = {
        name: {'until': 4600, 'message': 'quota exhausted'}
        for name in ('minimax-coding-plan', provider)}
    drain(f)
    assert [job['model'] for job, _, _ in launcher.started] == [expected]
    assert cmre.accepted == ['fn_00401000']
    assert len(cmre.tests) == 2


@pytest.mark.parametrize('confirm', [False, True])
@pytest.mark.parametrize('failure', ['limited', 'auth'])
def test_audit_and_confirmation_fall_back_to_terra(root, monkeypatch, confirm, failure):
    monkeypatch.setattr(farm.time, 'time', lambda: 1000.0)
    preferred = 'opencode-go/deepseek-v4.1-flash'
    terra = 'openai/gpt-6.1-terra'

    def plan(job):
        if job['model'] == preferred:
            return ScriptedHandle(code=1, infra='401 API key expired')
        Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['audit_path']).write_text(json.dumps({'verdict': 'ok', 'findings': []}))
        return ScriptedHandle(code=0)

    f, cmre, launcher = scripted_farm(root, plan, lambda: 0.0, workers=0,
                                      ladder=[preferred, terra], audit_workers=1,
                                      audit_model=preferred, confirm_model=preferred)
    fn = 'fn_00401000'
    (root / 'recovery/c' / f'{fn}.c').write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200, 'model': preferred, 'kind': 'translate', 'at': 0,
                               'identity': farm._identity(f._inputs(fn))}
    if confirm:
        seed_unconfirmed_audit(f, fn, [B], preferred)
    prior_attempts = len(f.state['attempts'].get(fn, []))
    if fn not in f.audit_queue:
        f.audit_queue.append(fn)
    if failure == 'limited':
        f.state['limited_providers']['opencode-go'] = {'until': 4600, 'message': 'usage limit'}
    drain(f)
    models = [j['model'] for j, _, _ in launcher.started]
    assert models == ([terra] if failure == 'limited' else [preferred, terra])
    assert bool(launcher.started[-1][0].get('confirm')) == confirm
    assert f.state['audits'][fn]['verdict'] == 'ok'
    assert f.config.audit_model == preferred and f.config.confirm_model == preferred
    assert cmre.accepted == [] and cmre.tests == []
    assert all(row['model'] == terra for row in f.state['attempts'][fn][prior_attempts:])


def test_a_limited_audit_waits_for_the_reset_instead_of_erroring(root, monkeypatch):
    now = [1000.0]
    monkeypatch.setattr(farm.time, 'time', lambda: now[0])
    calls = []

    class Launcher(FakeLauncher):
        def start(self, job, prompt, config):
            self.started.append((job, prompt, config))
            if job['kind'] == 'audit':
                calls.append(now[0])
                if now[0] < 4600:
                    return limited_handle()
            return FakeHandle(job, writes(GOOD), self.root)

    cmre = FakeCmre([item('fn_00401000')])
    cmre.root = root
    cfg = farm.FarmConfig(workers=1, ladder=['go/cheap'], audit_workers=1, audit_model='go/judge',
                          attempt_minutes=30, hours=1, min_bytes=0, max_bytes=None, include_held=False,
                          owner='farm-t')
    f = farm.Farm(root / 'run', root, cmre, Launcher(root, None), cfg, clock=lambda: 0.0)
    drain(f, 5)
    assert calls == [1000.0] and f.state['audits'].get('fn_00401000', {}).get('verdict') is None
    now[0] = 5000.0
    drain(f, 5)
    assert f.state['audits']['fn_00401000']['verdict'] == 'ok'


def test_adopt_and_dismiss_apply_directly_when_the_run_has_no_live_farm(root, monkeypatch):
    # The backlog run's open defects outlive its farm; the coordinator resolves them later.
    f, cmre, launcher = make(root, [], writes(GOOD), workers=0)
    for fn in ('fn_00401000', 'fn_00401100'):
        f.state['accepted'][fn] = {'bytes': 200, 'model': 'm/x', 'kind': 'translate', 'at': 0}
        f.state['audits'][fn] = {'rounds': 1, 'verdict': 'defect', 'findings': [B]}
        f.state['defects'][fn] = [B]
    f._event('farm-start', pid=1)
    f._event('farm-exit', status='done')
    f.save()
    (root / 'recovery/c/fn_00401000.c').write_text(GOOD)
    (root / 'recovery/c/fn_00401100.c').write_text(GOOD.replace('fn_00401000', 'fn_00401100'))
    hand = root / 'hand.c'
    hand.write_text(GOOD.replace('a + 1', 'a + 1 /*PASS by hand*/'))
    monkeypatch.setattr(farm, 'ROOT', root)
    monkeypatch.setattr(farm, 'CmreAdapter', lambda *a, **k: cmre)
    assert farm.main(['adopt', str(f.run_dir), 'fn_00401000', '--source', str(hand),
                      '--review-receipt', review_receipt(f)]) == 0
    assert farm.main(['dismiss', str(f.run_dir), 'fn_00401100', '--reason', 'false positive',
                      '--review-receipt', review_receipt(f, 'fn_00401100')]) == 0
    state = json.loads((f.run_dir / 'state.json').read_text())
    assert cmre.accepted == ['fn_00401000']
    assert state['defects'] == {} and state['audits']['fn_00401100']['dismissed'] == 'false positive'
    assert not list((f.run_dir / 'requests').glob('*.json'))


def test_coordinator_runs_on_a_different_provider_than_the_go_limited_lanes():
    # 2026-11-13: the OpenCode Go 5-hour limit stopped every DeepSeek model at once.
    coordinator = farm.coordinator_agent()
    assert f'model: {farm.COORDINATOR_MODEL}' in coordinator
    assert farm.COORDINATOR_MODEL.split('/')[0] != farm.DEFAULT_CONFIRM_MODEL.split('/')[0]


def test_defects_command_lists_each_open_defect_on_one_line(root, capsys):
    # A MiniMax coordinator read state.json and the farm source to learn this (2026-11-13 probe).
    f, cmre, _ = make(root, [], writes(GOOD), workers=0)
    f.state['accepted']['fn_00401000'] = {'bytes': 321, 'model': 'm/x', 'kind': 'translate', 'at': 0}
    f.state['audits']['fn_00401000'] = {'rounds': 1, 'verdict': 'defect', 'findings': [B],
                                        'confirmed_by': 'go/judge'}
    f.state['defects']['fn_00401000'] = [B]
    f.state['attempts']['fn_00401000'] = [{'model': 'go/judge', 'kind': 'fix', 'outcome': 'fail',
                                           'detail': 'compile failed', 'at': 0}]
    f.save()
    assert farm.main(['defects', str(f.run_dir)]) == 0
    out = capsys.readouterr().out.strip().splitlines()
    assert len(out) == 1
    assert out[0].startswith('fn_00401000 321B') and 'EAX return dropped' in out[0] and 'compile failed' in out[0]


def test_coordinator_agent_names_its_information_sources():
    assert 'status, wait and defects' in farm.coordinator_agent()


def test_every_default_deepseek_model_is_v4_1_flash():
    # Adam's choice (2026-11-13): DeepSeek runs use v4.1 Flash.
    defaults = [*farm.DEFAULT_LADDER, farm.DEFAULT_AUDIT_MODEL, farm.DEFAULT_CONFIRM_MODEL, farm.COORDINATOR_MODEL]
    agents = farm.coordinator_agent() + farm.defect_agent()
    models = defaults + re.findall(r'^model: (\S+)', agents, re.M)
    deepseek = [m for m in models if 'deepseek' in m]
    allowed = {'opencode-go/deepseek-v4.1-flash', 'commandcode/deepseek/deepseek-v4.1-flash'}
    assert deepseek and all(m.split('#')[0] in allowed for m in deepseek), deepseek


# --- continuous operation (2026-11-16) -----------------------------------------
# Review of the 14-16 Nov orchestrator sessions: the farm was stopped and drained eight times in
# one 19-hour session (floor change, model switch, hint/fix application, parent checks) and
# exited for good on supply-empty at 21:27, after which nothing ran for nine hours.

def passing(job):
    return GOOD.replace('fn_00401000', job['fn']) + '// PASS fake gate\n'


def test_continuous_farm_idles_on_empty_supply_and_takes_work_that_arrives_later(root):
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(passing), workers=1,
                             continuous=True, rescan_seconds=0)
    drain(f)
    assert cmre.accepted == ['fn_00401000']
    assert not f.drained()                      # idle, not finished: only STOP or the deadline ends it
    cmre.items.append(item('fn_00401100'))
    drain(f)
    assert cmre.accepted == ['fn_00401000', 'fn_00401100']
    names = [e['event'] for e in events(f.run_dir)]
    assert names.count('supply-empty') == 2     # one per dry spell, re-armed by new supply
    assert names.count('supply-resumed') == 1
    (f.run_dir / 'STOP').write_text('')
    assert f.drained()


def test_run_once_farm_still_exits_when_supply_is_exhausted(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD), workers=1)
    drain(f)
    assert f.drained()


def test_empty_supply_rescans_are_throttled(root):
    now = [0.0]
    cmre = FakeCmre([])
    cmre.root = root
    calls = []
    original = cmre.frontier
    cmre.frontier = lambda *args: calls.append(now[0]) or original(*args)
    cfg = farm.FarmConfig(workers=2, ladder=['m/cheap'], audit_workers=0, audit_model=None, attempt_minutes=30,
                          hours=None, min_bytes=0, max_bytes=None, include_held=False, owner='farm-t',
                          continuous=True, rescan_seconds=120)
    f = farm.Farm(root / 'run', root, cmre, FakeLauncher(root, writes(GOOD)), cfg, clock=lambda: now[0])
    for second in (0, 10, 20, 119, 120, 130):
        now[0] = float(second)
        f.tick()
    assert calls == [0.0, 120.0]


def test_control_file_changes_the_size_floor_and_slots_without_a_restart(root):
    f, cmre, launcher = make(root, [item('fn_00401000', 200), item('fn_00401100', 700)], writes(passing),
                             workers=1, min_bytes=600, continuous=True, rescan_seconds=0)
    drain(f)
    assert cmre.accepted == ['fn_00401100']
    assert farm.main(['set', str(f.run_dir), '--min-bytes', '100', '--workers', '3']) == 0
    drain(f)
    assert cmre.accepted == ['fn_00401100', 'fn_00401000']
    assert f.config.min_bytes == 100 and f.config.workers == 3
    changed = [e for e in events(f.run_dir) if e['event'] == 'config-changed']
    assert changed and changed[0]['changes']['min_bytes'] == [600, 100]


def test_control_file_switches_models_for_new_jobs(root):
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), workers=1, continuous=True)
    assert farm.main(['set', str(f.run_dir), '--ladder', 'm/new,m/strong', '--audit-model', 'm/judge']) == 0
    drain(f)
    assert launcher.started[0][0]['model'] == 'm/new'
    assert f.config.audit_model == 'm/judge'


def test_control_survives_a_farm_restart(root):
    f, cmre, _ = make(root, [], writes(GOOD), workers=1, min_bytes=600)
    assert farm.main(['set', str(f.run_dir), '--min-bytes', '100']) == 0
    f2, _, _ = make(root, [], writes(GOOD), workers=1, min_bytes=600)
    f2.tick()
    assert f2.config.min_bytes == 100


def test_set_extends_the_deadline_of_a_running_farm(root):
    now = [0.0]
    f, cmre, _ = make(root, [], writes(GOOD), workers=1, hours=1, continuous=True)
    f.clock = lambda: now[0]
    f.deadline = 3600.0
    now[0] = 3500.0
    assert farm.main(['set', str(f.run_dir), '--add-hours', '2']) == 0
    f.tick()
    now[0] = 3700.0
    assert not f.stopping()
    f.tick()                                    # an applied extension is not applied twice
    assert f.deadline == 3600.0 + 7200.0


def test_set_rejects_an_empty_change(root, capsys):
    assert farm.main(['set', str(root / 'run')]) == 2


def input_bound_report():
    diff = {'cases': 600, 'inconclusive': 0, 'mismatches': [], 'coverage': 0.3669, 'code_as_data_cases': 0,
            'uncovered': ['0x004ae359', '0x004ae35b']}
    return {'status': 'fail', 'problems': [],
            'builds': {'-O0': {'status': 'fail', 'difftest': dict(diff)},
                       '-O2': {'status': 'fail', 'difftest': dict(diff)}}}


def test_failure_detail_reports_coverage_and_the_first_mismatch():
    # 254 of 300 parked functions (run 20261115T1250Z) carried 'fail | -O0 fail:  | -O2 fail: '.
    assert 'coverage 0.3669' in farm.report_detail(input_bound_report())
    report = input_bound_report()
    report['builds']['-O2']['difftest'] = {'cases': 600, 'inconclusive': 3, 'coverage': 0.81,
                                           'mismatches': [{'case': 17, 'field': 'eax', 'original': 1, 'recovered': 2},
                                                          {'case': 40, 'field': 'mem'}]}
    detail = farm.report_detail(report)
    assert '2 mismatches' in detail and '"case": 17' in detail and 'inconclusive 3/600' in detail


def test_input_bound_failure_is_recognised_only_without_mismatches_or_budget_exhaustion():
    assert farm.input_bound(input_bound_report())
    mismatch = input_bound_report()
    mismatch['builds']['-O2']['difftest']['mismatches'] = [{'case': 1}]
    assert not farm.input_bound(mismatch)
    looping = input_bound_report()               # C that never terminates shows only as inconclusive
    looping['builds']['-O0']['difftest']['inconclusive'] = 300
    assert not farm.input_bound(looping)
    broken = input_bound_report()
    broken['builds']['-O0'] = {'status': 'build-failed', 'detail': 'x'}
    assert not farm.input_bound(broken)
    assert not farm.input_bound({'status': 'fail', 'problems': ['contract'], 'builds': input_bound_report()['builds']})


def test_input_bound_failure_parks_without_spending_the_stronger_model(root):
    f, cmre, launcher = make(root, [item('fn_00401000')], writes('u32 cm_fn_00401000(u32 a) { return a; }\n'),
                             workers=1)
    cmre.test = lambda fn, source=None: input_bound_report()
    drain(f)
    assert [j['model'] for j, _, _ in launcher.started] == ['m/cheap']
    assert f.state['parked']['fn_00401000']['detail'].startswith('input-bound')
    needed = [e for e in events(f.run_dir) if e['event'] == 'input-needed']
    assert needed and needed[0]['fn'] == 'fn_00401000' and needed[0]['coverage'] == 0.3669


def park_input_bound(root):
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(passing), workers=1,
                             continuous=True, rescan_seconds=0)
    (root / 'recovery/hints/fn_00401000.json').write_text('{"arg_count": 1}')
    cmre.test = lambda fn, source=None: input_bound_report()
    drain(f)
    assert 'fn_00401000' in f.state['parked'] and len(launcher.started) == 1
    del cmre.test                                   # the gate now passes a PASS-marked source
    return f, cmre, launcher


def test_new_inputs_regate_the_retained_candidate_without_a_model(root):
    f, cmre, launcher = park_input_bound(root)
    (root / 'recovery/hints/fn_00401000.json').write_text('{"arg_count": 1, "fixtures": [{}]}')
    drain(f)
    assert cmre.accepted == ['fn_00401000']
    assert len(launcher.started) == 1               # the supply lane's inputs were the only missing piece
    names = [e['event'] for e in events(f.run_dir)]
    assert 'input-arrived' in names and 'recheck' in names
    assert 'fn_00401000' not in f.state['input_needed']


def test_unchanged_inputs_leave_an_input_bound_candidate_parked(root):
    f, cmre, launcher = park_input_bound(root)
    tests = len(cmre.tests)
    drain(f)
    assert cmre.accepted == [] and len(cmre.tests) == tests and len(launcher.started) == 1


def test_resumed_recheck_resolves_legacy_relative_source(root, monkeypatch):
    f, cmre, launcher = park_input_bound(root)
    fn = 'fn_00401000'
    row = f.state['input_needed'][fn]
    row['source'] = str(Path(row['source']).relative_to(root))
    row.pop('source_sha256', None)  # historical state
    f.save()
    monkeypatch.chdir(root.parent)
    (root / f'recovery/hints/{fn}.json').write_text('{"arg_count": 1, "fixtures": [{}]}')
    resumed = farm.Farm(f.run_dir, root, cmre, launcher, f.config, clock=f.clock)
    drain(resumed)
    assert cmre.accepted == [fn]
    assert len(launcher.started) == 1
    assert fn not in resumed.state['parked']


def test_new_inputs_regate_while_every_model_is_unavailable(root):
    f, cmre, launcher = park_input_bound(root)
    fn = 'fn_00401000'
    f.state['unavailable_models'].update(dict.fromkeys(f.config.ladder, 'unavailable'))
    (root / f'recovery/hints/{fn}.json').write_text('{"arg_count": 1, "fixtures": [{}]}')
    drain(f)
    assert cmre.accepted == [fn]
    assert len(launcher.started) == 1


def test_recheck_claim_contention_survives_restart(root):
    f, cmre, launcher = park_input_bound(root)
    fn = 'fn_00401000'
    claim = cmre.claim
    def claimed_elsewhere(*args, **kwargs):
        raise ValueError('claimed by another worker')
    cmre.claim = claimed_elsewhere
    (root / f'recovery/hints/{fn}.json').write_text('{"arg_count": 1, "fixtures": [{}]}')
    f.tick()
    assert fn in f.state['parked']
    assert cmre.accepted == []
    cmre.claim = claim
    resumed = farm.Farm(f.run_dir, root, cmre, launcher, f.config, clock=f.clock)
    drain(resumed)
    assert cmre.accepted == [fn]
    assert len(launcher.started) == 1


def test_changed_retained_source_requires_explicit_retry(root):
    f, cmre, launcher = park_input_bound(root)
    fn = 'fn_00401000'
    path = Path(f.state['input_needed'][fn]['source'])
    path.write_bytes(path.read_bytes() + b'\n/* drift */\n')
    (root / f'recovery/hints/{fn}.json').write_text('{"arg_count": 1, "fixtures": [{}]}')
    drain(f)
    assert cmre.accepted == []
    assert len(launcher.started) == 1
    assert 'retained candidate' in f.state['parked'][fn]['detail']


def test_recheck_mismatch_starts_with_cheapest_rung_and_is_not_rechecked_again(root):
    f, cmre, launcher = park_input_bound(root)
    fn = 'fn_00401000'
    cmre.test = lambda fn, source=None: {'status': 'fail', 'problems': ['mismatch']}
    launcher.behaviour = writes(lambda job: f'u32 cm_fn_00401000(u32 a) {{ return a + {job["n"]}; }}\n')
    (root / f'recovery/hints/{fn}.json').write_text('{"arg_count": 1, "fixtures": [{}]}')
    drain(f)
    assert [job['model'] for job, _, _ in launcher.started] == ['m/cheap', 'm/cheap', 'm/strong']
    assert [e['fn'] for e in events(f.run_dir) if e['event'] == 'recheck'] == [fn]
    assert fn in f.state['parked']


def test_new_inputs_that_still_fall_short_park_again_without_looping(root):
    f, cmre, launcher = park_input_bound(root)
    cmre.test = lambda fn, source=None: input_bound_report()
    (root / 'recovery/hints/fn_00401000.json').write_text('{"arg_count": 1, "fixtures": [{}]}')
    drain(f)
    rechecks = [e for e in events(f.run_dir) if e['event'] == 'recheck']
    assert len(rechecks) == 1 and 'fn_00401000' in f.state['parked'] and len(launcher.started) == 1


def test_retry_request_returns_a_parked_function_to_the_running_farm(root):
    wrong = writes(lambda job: f"u32 cm_fn_00401000(u32 a) {{ return a * {job['n']} + 7; }}\n")
    f, cmre, launcher = make(root, [item('fn_00401000')], wrong, workers=1, continuous=True, rescan_seconds=0)
    drain(f)
    assert 'fn_00401000' in f.state['parked'] and len(launcher.started) == 2   # both rungs failed
    assert farm.main(['retry', str(f.run_dir), 'fn_00401000', '--reason', 'harness release cleared its family']) == 0
    launcher.behaviour = writes(passing)
    drain(f)
    assert cmre.accepted == ['fn_00401000']
    retried = [e for e in events(f.run_dir) if e['event'] == 'retry']
    assert retried and retried[0]['reason'] == 'harness release cleared its family'
    again = launcher.started[2][0]                  # the retry starts from the last candidate and its failure
    assert 'case 7 eax' in again['prior']['detail'] and again['prior']['model'] == 'm/strong'


def test_reclassify_registers_old_parked_candidates_for_the_input_lane(root):
    # Runs before 2026-11-16 parked 254 of 300 functions with an empty reason and no input record.
    names = ['fn_00401000', 'fn_00401100', 'fn_00401200', 'fn_00401300']
    f, cmre, launcher = make(root, [item(fn, 300) for fn in names], writes(passing), workers=0)
    reports = {'fn_00401000': input_bound_report(), 'fn_00401100': {'status': 'pass', 'builds': {}},
               'fn_00401200': {'status': 'fail', 'problems': ['mismatch'], 'builds': {}}}
    for fn in names:
        f.state['parked'][fn] = {'detail': 'fail | -O0 fail:  | -O2 fail: ', 'models': ['m/cheap']}
        f.state['attempts'][fn] = [{'model': 'm/cheap', 'kind': 'translate', 'outcome': 'fail', 'detail': '', 'at': 0}]
    for fn in names[:3]:                             # the fourth has no retained candidate
        (f.run_dir / 'src').mkdir(exist_ok=True)
        (f.run_dir / 'src' / f'{fn}.c').write_text(passing({'fn': fn}))
        (root / f'recovery/hints/{fn}.json').write_text('{"arg_count": 1}')
    f.save()
    cmre.test = lambda fn, source=None: reports[fn]
    counts = farm.reclassify(f.run_dir, root, cmre)
    assert counts == {'input-bound': 1, 'passes-now': 1, 'failed': 1, 'no-candidate': 1}
    state = json.loads(f.state_path.read_text())
    assert state['input_needed']['fn_00401000']['coverage'] == 0.3669
    assert state['input_needed']['fn_00401000']['bytes'] == 300
    assert set(state['parked']) == set(names)        # nothing is unparked or accepted here

    del cmre.test
    f2, cmre2, launcher2 = make(root, [item(fn, 300) for fn in names], writes(passing), workers=1,
                                continuous=True, rescan_seconds=0)
    drain(f2)
    assert cmre2.accepted == ['fn_00401100']         # the candidate that passes now is re-gated by the farm
    assert launcher2.started == []                   # and no parked function costs a model attempt


def test_wait_ignores_supply_events_when_the_farm_keeps_running(root):
    # supply-empty used to wake the coordinator, which then stopped the farm (2026-11-15 21:27).
    assert 'supply-empty' not in farm.NOTABLE_EVENTS and 'supply-resumed' not in farm.NOTABLE_EVENTS


def test_status_line_shows_idle_slots_and_the_dry_spell(root):
    f, cmre, _ = make(root, [], writes(GOOD), workers=4, continuous=True)
    f.tick()
    line = farm.status_line(f.summary())
    assert 'SUPPLY EMPTY' in line and 'idle 4' in line


def test_supervisor_restarts_a_farm_that_dies_and_stops_on_request(root):
    run = root / 'run'
    run.mkdir()
    codes, launched = [1, 1, 0], []

    def launch(command):
        launched.append(command)
        if len(launched) == 3:
            (run / 'STOP').write_text('')
        return codes[len(launched) - 1]

    assert farm.supervise(run, ['--workers', '2'], launch=launch, sleep=lambda s: None, clock=lambda: 0.0) == 0
    assert len(launched) == 3 and all(c[-2:] == ['--workers', '2'] for c in launched)
    names = [e['event'] for e in events(run)]
    assert names.count('farm-restart') == 2


def test_supervisor_gives_up_on_a_crash_loop(root):
    run = root / 'run'
    run.mkdir()
    launched = []
    code = farm.supervise(run, [], launch=lambda command: launched.append(command) or 1,
                          sleep=lambda s: None, clock=lambda: 0.0, max_restarts=3)
    assert code == 1 and len(launched) == 4
    assert [e['event'] for e in events(run)][-1] == 'farm-crash-loop'


def test_supervisor_does_not_restart_after_the_deadline(root):
    run = root / 'run'
    run.mkdir()
    (run / 'state.json').write_text(json.dumps({'deadline_at': time.time() - 5}))
    launched = []
    assert farm.supervise(run, [], launch=lambda command: launched.append(command) or 0,
                          sleep=lambda s: None, clock=lambda: 0.0) == 0
    assert len(launched) == 1


# --- pre-accept safeguards: self-declared candidates and ADR 0010 condition 3 ---

BRIDGED = ('#include "cmre.h"\n'
           '#include "cmre_x87.h"\n'
           'long double cm_fn_00401000(long double base, long double exponent)\n'
           '{ return cm_x87_CIpow(base, exponent); } // PASS\n')


def x87_mutant_report(outcome):
    """Synthetic gate reports for an operand mutant."""
    if outcome == 'survivor':
        return {'status': 'pass', 'problems': [],
                'builds': {opt: {'status': 'pass', 'difftest': {'cases': 8, 'inconclusive': 0,
                                                               'mismatches': []}}
                           for opt in ('-O0', '-O2')}}
    if outcome == 'build-failed':
        return {'status': 'fail', 'problems': [],
                'builds': {opt: {'status': 'build-failed', 'detail': 'compile error'}
                           for opt in ('-O0', '-O2')}}
    if outcome == 'inconclusive':
        return {'status': 'fail', 'problems': [],
                'builds': {opt: {'status': 'fail', 'difftest': {'cases': 8, 'inconclusive': 1,
                                                               'mismatches': [{'case': 0, 'field': 'regs'}]}}
                           for opt in ('-O0', '-O2')}}
    if outcome == 'no-inconclusive':
        report = x87_mutant_report('killed')
        for build in report['builds'].values():
            build['difftest'].pop('inconclusive')
        return report
    return {'status': 'fail', 'problems': [],
            'builds': {opt: {'status': 'fail', 'difftest': {'cases': 8, 'inconclusive': 0,
                                                           'mismatches': [{'case': 1, 'field': 'regs'}]}}
                       for opt in ('-O0', '-O2')}}


X87_EXECUTION_IDENTITY = {
    'target': 'a' * 64, 'harness': 'b' * 64, 'python': '3.13',
    'inventory': {'inventory.json': 'c' * 64},
    'headers': {'recovery/include/cmre.h': 'd' * 64},
    'dependencies': {name: '1.0' for name in ('capstone', 'unicorn', 'pefile')},
    'toolchain': {name: {'version': 'fake 1.0', 'sha256': 'e' * 64}
                  for name in ('gcc', 'as', 'ld', 'nm')},
}


def bind_x87_report(report, path, fn):
    """Give a synthetic gate report the real report identity fields the farm binds."""
    report = json.loads(json.dumps(report))
    digest = hashlib.sha256(Path(path).read_bytes()).hexdigest()
    report.update({'va': '0x' + fn[3:], 'tier': 'c', 'profile': 'semantic',
                   'source_sha256': digest, 'execution_identity': X87_EXECUTION_IDENTITY,
                   'key': digest[:16]})
    return report





@pytest.mark.parametrize('marker', ['UNVALIDATED CANDIDATE', 'UNVALIDATED candidate',
                                    'BLOCKER: unresolved x87 operand boundary',
                                    'must not be accepted', 'MUST NOT be accepted'])
def test_candidate_self_disclaimer_is_refused_before_the_gate(root, marker):
    source = GOOD.replace('return a + 1;', f'return a + 1; /* {marker} */')
    f, cmre, _ = make(root, [item('fn_00401000')], writes(source), workers=1, ladder=['m/cheap'])
    drain(f)
    assert cmre.accepted == []
    assert cmre.tests == []
    assert not (root / 'recovery/c/fn_00401000.c').exists()
    detail = f.state['parked']['fn_00401000']['detail']
    assert detail.startswith('BLOCKER:')
    assert 'candidate self-declares' in detail


def x87_gate(root, cmre, outcome, *, drift=None):
    original = cmre.test

    def gate(fn, source=None):
        path = Path(source) if source else root / 'recovery/c' / f'{fn}.c'
        text = path.read_text() if path.exists() else ''
        if '+ 1.0L' in text:
            if drift == 'hints':
                (root / f'recovery/hints/{fn}.json').write_text('{"arg_count": 3}')
            elif drift == 'source':
                (root / f'run/src/{fn}.c').write_text(text.replace('return', 'return /* drift */ return'))
            report = x87_mutant_report(outcome)
        else:
            report = original(fn, source)
        return bind_x87_report(report, path, fn)
    cmre.test = gate
    return gate


def test_x87_operand_mutants_must_each_fail_and_their_evidence_is_retained(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(BRIDGED), workers=1, ladder=['m/cheap'])
    x87_gate(root, cmre, 'killed')
    drain(f)
    assert cmre.accepted == ['fn_00401000']
    summaries = list((f.run_dir / 'x87-mutants').glob('*/summary.json'))
    assert len(summaries) == 1
    summary = json.loads(summaries[0].read_text())
    assert summary['status'] == 'killed'
    assert [row['operand'] for row in summary['mutants']] == ['base', 'exp']
    assert [row['outcome'] for row in summary['mutants']] == ['killed', 'killed']
    directory = summaries[0].parent
    accepted_bytes = (root / 'recovery/c/fn_00401000.c').read_bytes()
    assert summary['candidate_sha256'] == hashlib.sha256(accepted_bytes).hexdigest()
    baseline = summary['baseline']
    assert (directory / baseline['source']).read_bytes() == accepted_bytes
    assert baseline['source_sha256'] == summary['candidate_sha256']
    baseline_report = directory / baseline['report']
    assert hashlib.sha256(baseline_report.read_bytes()).hexdigest() == baseline['report_sha256']
    assert json.loads(baseline_report.read_text())['status'] == 'pass'
    for row in summary['mutants']:
        mutant = directory / row['source']
        report = directory / row['report']
        assert mutant.is_file() and report.is_file()
        assert hashlib.sha256(mutant.read_bytes()).hexdigest() == row['source_sha256']
        assert hashlib.sha256(report.read_bytes()).hexdigest() == row['report_sha256']
        assert '+ 1.0L' in mutant.read_text()
        assert json.loads(report.read_text())['source_sha256'] == row['source_sha256']
    assert summary['mutants'][0]['line'] == 4


@pytest.mark.parametrize('outcome,needle', [
    ('survivor', 'survivor'),
    ('build-failed', 'build-failed'),
    ('inconclusive', 'inconclusive'),
    ('no-inconclusive', 'inconclusive'),
])
def test_x87_operand_mutant_without_a_differential_kill_blocks_acceptance(root, outcome, needle):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(BRIDGED), workers=1, ladder=['m/cheap'])
    x87_gate(root, cmre, outcome)
    drain(f)
    assert cmre.accepted == []
    assert not (root / 'recovery/c/fn_00401000.c').exists()
    detail = f.state['parked']['fn_00401000']['detail']
    assert detail.startswith('BLOCKER:')
    assert needle in detail
    summary = json.loads(next((f.run_dir / 'x87-mutants').glob('*/summary.json')).read_text())
    assert summary['status'] == 'blocked'


def test_x87_operand_parsing_fails_closed_on_an_unsupported_call_shape(root):
    source = ('#include "cmre.h"\n'
              'long double cm_fn_00401000(long double a, long double b)\n'
              '{ return cm_x87_CIpow(a, b, 0); } // PASS\n')
    f, cmre, _ = make(root, [item('fn_00401000')], writes(source), workers=1, ladder=['m/cheap'])
    drain(f)
    assert cmre.accepted == []
    detail = f.state['parked']['fn_00401000']['detail']
    assert detail.startswith('BLOCKER:')
    assert 'unsupported' in detail
    summary = json.loads(next((f.run_dir / 'x87-mutants').glob('*/summary.json')).read_text())
    assert summary['status'] == 'blocked'
    assert summary['mutants'] == []


@pytest.mark.parametrize('drift', ['source', 'hints'])
def test_x87_operand_mutant_evidence_rejects_candidate_or_hints_drift(root, drift):
    (root / 'recovery/hints/fn_00401000.json').write_text('{"arg_count": 2}')
    f, cmre, _ = make(root, [item('fn_00401000')], writes(BRIDGED), workers=1, ladder=['m/cheap'])
    x87_gate(root, cmre, 'killed', drift=drift)
    drain(f)
    assert cmre.accepted == []
    detail = f.state['parked']['fn_00401000']['detail']
    assert 'BLOCKER:' in detail
    assert 'drift' in detail.lower()

@pytest.mark.parametrize('change', ['missing', 'changed'])
def test_x87_acceptance_rechecks_retained_mutant_evidence(root, change):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(BRIDGED), workers=1, ladder=['m/cheap'])
    executor = ManualExecutor()
    f.verifier = executor
    x87_gate(root, cmre, 'killed')
    f.tick()                       # dispatch
    f.tick()                       # reap and submit verification
    executor.run_all()
    f._collect()
    assert f.accept_queue
    summary = next((f.run_dir / 'x87-mutants').glob('*/summary.json'))
    if change == 'missing':
        summary.unlink()
    else:
        mutant = summary.parent / json.loads(summary.read_text())['mutants'][0]['source']
        mutant.write_bytes(mutant.read_bytes() + b'// changed after verification\n')
    f._accept()
    assert cmre.accepted == []
    assert not (root / 'recovery/c/fn_00401000.c').exists()
    detail = f.state['parked']['fn_00401000']['detail']
    assert detail.startswith('BLOCKER:')
    assert 'x87 operand evidence' in detail


def test_x87_bridge_parser_ignores_comments_and_strings_and_blocks_macro_aliases():
    text = ('/* cm_x87_CIpow(a, b) */\n'
            'const char *s = "cm_x87_CIpow(a, b)";\n'
            'long double f(long double a, long double b) { return cm_x87_CIpow(a, b); }\n')
    assert len(farm.bridge_call_sites(text)) == 1
    with pytest.raises(ValueError, match='preprocessor directive'):
        farm.bridge_call_sites('#define alias(a, b) cm_x87_CIpow(a, b)\n')


def test_x87_bridge_mutation_preserves_a_multiline_operand_expression():
    text = ('long double f(long double a, long double b)\n'
            '{ return cm_x87_CIpow((long double)(a + 1),\n'
            '                      (long double)b); } // PASS\n')
    rows = farm.bridge_mutant_sources(text)
    assert [row['operand'] for row in rows] == ['base', 'exp']
    assert '((long double)(a + 1)) + 1.0L' in rows[0]['source']
    assert '((long double)b) + 1.0L' in rows[1]['source']


def x87_pending_accept(root, outcome='killed'):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(BRIDGED), workers=1, ladder=['m/cheap'])
    executor = ManualExecutor()
    f.verifier = executor
    x87_gate(root, cmre, outcome)
    f.tick()
    f.tick()
    executor.run_all()
    f._collect()
    assert f.accept_queue
    return f, cmre, next((f.run_dir / 'x87-mutants').glob('*/summary.json'))


@pytest.mark.parametrize('restart', [False, True])
@pytest.mark.parametrize('change', ['harness', 'headers', 'target', 'toolchain', 'python'])
def test_x87_current_execution_identity_drift_blocks_acceptance(root, restart, change):
    f, cmre, summary_path = x87_pending_accept(root)
    original_evidence = summary_path.read_bytes()
    current = cmre.current_execution_identity()
    if change == 'headers':
        current['headers']['recovery/include/cmre.h'] = 'f' * 64
    elif change == 'toolchain':
        current['toolchain']['gcc']['sha256'] = 'f' * 64
    else:
        current[change] = 'f' * 64
    cmre.current_execution_identity = lambda: current
    if restart:
        f.save()
        f = farm.Farm(f.run_dir, root, cmre, None, f.config)
    f._accept()
    assert cmre.accepted == []
    assert 'fn_00401000' not in f.state['accepted']
    assert not (root / 'recovery/c/fn_00401000.c').exists()
    detail = f.state['parked']['fn_00401000']['detail']
    assert detail.startswith('BLOCKER:')
    assert 'current execution identity' in detail
    assert summary_path.read_bytes() == original_evidence


@pytest.mark.parametrize('restart', [False, True])
@pytest.mark.parametrize('current', [
    None, {}, [], {'harness': 'b' * 64}, 'unavailable', 'system-exit', 'missing-method',
])
def test_x87_current_execution_identity_unavailable_or_malformed_blocks(root, restart, current):
    f, cmre, _ = x87_pending_accept(root)

    def identity():
        if current == 'unavailable':
            raise OSError('identity inputs unavailable')
        if current == 'system-exit':
            raise SystemExit('identity provider stopped')
        return current

    cmre.current_execution_identity = identity
    if current == 'missing-method':
        cmre.current_execution_identity = None
    if restart:
        f.save()
        f = farm.Farm(f.run_dir, root, cmre, None, f.config)
    f._accept()
    assert cmre.accepted == []
    detail = f.state['parked']['fn_00401000']['detail']
    assert detail.startswith('BLOCKER:')
    assert 'current execution identity' in detail


@pytest.mark.parametrize('field', ['target', 'harness', 'python', 'inventory', 'headers',
                                   'dependencies', 'toolchain'])
def test_x87_matching_malformed_execution_identities_are_not_proof(root, field):
    f, cmre, summary_path = x87_pending_accept(root)
    malformed = cmre.current_execution_identity()
    malformed[field] = None
    cmre.current_execution_identity = lambda: malformed
    summary = json.loads(summary_path.read_bytes())
    for row in [summary['baseline'], *summary['mutants']]:
        report_path = summary_path.parent / row['report']
        report = json.loads(report_path.read_bytes())
        report['execution_identity'] = malformed
        report_path.write_text(json.dumps(report))
        row['report_sha256'] = hashlib.sha256(report_path.read_bytes()).hexdigest()
    summary_path.write_text(json.dumps(summary))
    f._accept()
    assert cmre.accepted == []
    detail = f.state['parked']['fn_00401000']['detail']
    assert 'current execution identity is malformed' in detail


def test_no_bridge_does_not_require_current_execution_identity(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD), workers=1, ladder=['m/cheap'])

    def unavailable():
        raise AssertionError('nonbridge source must not query execution identity')

    cmre.current_execution_identity = unavailable
    drain(f)
    assert cmre.accepted == ['fn_00401000']


def test_real_adapter_current_execution_identity_uses_live_local_provider(monkeypatch):
    from cmre import recover

    calls = []
    current = json.loads(json.dumps(X87_EXECUTION_IDENTITY))

    def identity():
        calls.append(True)
        return current

    monkeypatch.setattr(recover, 'execution_identity', identity)
    adapter = farm.CmreAdapter()
    assert adapter.current_execution_identity() == current
    current['harness'] = 'f' * 64
    assert adapter.current_execution_identity()['harness'] == 'f' * 64
    assert len(calls) == 2


@pytest.mark.parametrize('change', [
    'summary-source', 'duplicate-row', 'duplicate-only', 'malformed-row', 'missing-row',
    'missing-report', 'changed-report', 'bad-report-json', 'changed-baseline',
])
def test_x87_acceptance_rejects_tampered_or_malformed_evidence(root, change):
    f, cmre, summary_path = x87_pending_accept(root)
    summary = json.loads(summary_path.read_text())
    directory = summary_path.parent
    row = summary['mutants'][0]
    if change == 'summary-source':
        row['source'] = 'baseline.c'
        row['source_sha256'] = summary['baseline']['source_sha256']
        row['sha256'] = row['source_sha256']
    elif change == 'duplicate-row':
        summary['mutants'].append(dict(row))
    elif change == 'duplicate-only':
        summary['mutants'][1] = dict(row)
    elif change == 'malformed-row':
        summary['mutants'].append('not-a-row')
    elif change == 'missing-row':
        summary['mutants'].pop()
    elif change == 'missing-report':
        (directory / row['report']).unlink()
    elif change == 'changed-report':
        (directory / row['report']).write_text('{"status": "pass"}')
    elif change == 'bad-report-json':
        (directory / row['report']).write_bytes(b'{')
        row['report_sha256'] = hashlib.sha256(b'{').hexdigest()
    elif change == 'changed-baseline':
        (directory / summary['baseline']['source']).write_bytes(b'changed baseline')
    summary_path.write_text(json.dumps(summary))
    f._accept()
    assert cmre.accepted == []
    assert not (root / 'recovery/c/fn_00401000.c').exists()
    detail = f.state['parked']['fn_00401000']['detail']
    assert detail.startswith('BLOCKER:')
    assert 'x87 operand' in detail


@pytest.mark.parametrize('change', ['survivor', 'no-inconclusive', 'missing-source-sha'])
def test_x87_acceptance_revalidates_retained_report_bytes_and_classification(root, change):
    f, cmre, summary_path = x87_pending_accept(root)
    summary = json.loads(summary_path.read_text())
    directory = summary_path.parent
    row = summary['mutants'][0]
    report_path = directory / row['report']
    source_path = directory / row['source']
    if change == 'survivor':
        report = x87_mutant_report('survivor')
    elif change == 'no-inconclusive':
        report = x87_mutant_report('no-inconclusive')
    else:
        report = json.loads(report_path.read_text())
    report = bind_x87_report(report, source_path, 'fn_00401000')
    if change == 'missing-source-sha':
        report.pop('source_sha256')
    report_path.write_text(json.dumps(report))
    row['report_sha256'] = hashlib.sha256(report_path.read_bytes()).hexdigest()
    row['outcome'] = 'killed'
    summary_path.write_text(json.dumps(summary))
    f._accept()
    assert cmre.accepted == []
    detail = f.state['parked']['fn_00401000']['detail']
    assert detail.startswith('BLOCKER:')
    assert 'x87 operand' in detail


def test_x87_operand_evidence_hashes_exact_crlf_source(root):
    raw = BRIDGED.replace('\n', '\r\n').encode()

    def write_crlf(job, repo):
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_bytes(raw)

    f, cmre, _ = make(root, [item('fn_00401000')], write_crlf, workers=1, ladder=['m/cheap'])
    x87_gate(root, cmre, 'killed')
    drain(f)
    assert cmre.accepted == ['fn_00401000']
    canonical = (root / 'recovery/c/fn_00401000.c').read_bytes()
    assert canonical == raw
    summary = json.loads(next((f.run_dir / 'x87-mutants').glob('*/summary.json')).read_text())
    assert summary['candidate_sha256'] == hashlib.sha256(raw).hexdigest()
    directory = next((f.run_dir / 'x87-mutants').glob('*/summary.json')).parent
    assert (directory / summary['baseline']['source']).read_bytes() == raw
    for row in summary['mutants']:
        mutant = (directory / row['source']).read_bytes()
        assert b'\r\n' in mutant and b'+ 1.0L' in mutant
        assert hashlib.sha256(mutant).hexdigest() == row['source_sha256']


def x87_journal_candidate(root, phase):
    f, cmre, _ = make(root, [], writes(BRIDGED), workers=0)
    fn = 'fn_00401000'
    canonical = root / f'recovery/c/{fn}.c'
    canonical.write_bytes(b'prior canonical')
    job = f._new_job('translate', fn, 'hand', bytes=200)
    source = root / 'candidate.c'
    source.write_bytes(BRIDGED.encode())
    job.update(source=str(source), _snap={'c': b'prior canonical', 'hint': None})
    f._begin_transaction(job)
    data = f._transaction(job, phase)
    canonical.write_bytes(BRIDGED.encode())
    f.save()
    return f, cmre, job, canonical, data


def test_restart_parks_installed_bridge_candidate_without_operand_proof(root):
    f, cmre, _, canonical, _ = x87_journal_candidate(root, 'installed')
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert 'fn_00401000' not in resumed.state['accepted']
    assert canonical.read_bytes() == b'prior canonical'
    assert 'fn_00401000' in resumed.state['parked']
    detail = resumed.state['parked']['fn_00401000']['detail']
    assert detail.startswith('BLOCKER:')
    assert 'x87 operand evidence' in detail


@pytest.mark.parametrize('phase', ['accepted', 'accepting'])
def test_restart_preserves_authoritative_bridge_acceptance_without_new_proof(root, phase):
    f, cmre, _, canonical, data = x87_journal_candidate(root, phase)
    if phase == 'accepting':
        cmre.acceptance_identity = lambda fn: data['identity']
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert 'fn_00401000' in resumed.state['accepted']
    assert canonical.read_bytes() == BRIDGED.encode()
    assert 'fn_00401000' not in resumed.state['parked']


def test_restart_routes_installed_bridge_candidate_with_intact_proof_to_acceptance(root):
    f, cmre, _ = x87_pending_accept(root)
    canonical = root / 'recovery/c/fn_00401000.c'
    installed = canonical.read_bytes()
    f.save()
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert canonical.read_bytes() == installed          # no unproven rollback
    assert 'fn_00401000' not in resumed.state['accepted']
    assert resumed.accept_queue
    resumed._accept()
    assert cmre.accepted == ['fn_00401000']
    assert 'fn_00401000' in resumed.state['accepted']


def test_restart_parks_accepting_bridge_candidate_after_nonacceptance_without_proof(root):
    f, cmre, _, canonical, _ = x87_journal_candidate(root, 'accepting')
    resumed = farm.Farm(f.run_dir, root, cmre, None, f.config)
    assert 'fn_00401000' not in resumed.state['accepted']
    assert canonical.read_bytes() == b'prior canonical'
    assert 'fn_00401000' in resumed.state['parked']
    assert resumed.state['parked']['fn_00401000']['detail'].startswith('BLOCKER:')


def test_x87_unbound_gate_reports_are_not_treated_as_operand_proof(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(BRIDGED), workers=1, ladder=['m/cheap'])
    original = cmre.test

    def gate(fn, source=None):
        path = Path(source) if source else root / 'recovery/c' / f'{fn}.c'
        text = path.read_text() if path.exists() else ''
        if '+ 1.0L' in text:
            return x87_mutant_report('killed')
        return original(fn, source)

    cmre.test = gate
    drain(f)
    assert cmre.accepted == []
    detail = f.state['parked']['fn_00401000']['detail']
    assert detail.startswith('BLOCKER:')
    assert 'not bound' in detail
