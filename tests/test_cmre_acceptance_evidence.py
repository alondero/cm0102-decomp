"""Synthetic end-to-end acceptance checks with an isolated ledger."""
import json
import os
from pathlib import Path

import pytest

from cmre import acceptance_request, cli, env, ledger, recover
from test_cmre_tasks import VA, workspace


@pytest.fixture
def evidence(workspace, tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'BLOCKED_DIR', tmp_path / 'blocked')
    monkeypatch.setattr(env, 'RUNS', tmp_path / 'runs')
    monkeypatch.setattr(ledger, 'legacy_entries', lambda: [])
    digest = '1' * 64
    identity = {
        'target': digest, 'harness': digest, 'python': 'test-python',
        'inventory': {'functions': digest}, 'headers': {'cmre.h': digest},
        'dependencies': dict.fromkeys(('capstone', 'unicorn', 'pefile'), 'test-version'),
        'toolchain': {name: {'version': 'test-version', 'sha256': digest}
                      for name in ('gcc', 'as', 'ld', 'nm')},
    }
    monkeypatch.setattr(recover, 'execution_identity', lambda: identity)
    ledger.record_path(VA).write_text('{"status": {}, "lifted": {}}', encoding='utf-8')
    canonical = env.C_DIR / f'fn_{VA:08x}.c'
    calls = []

    def gate(va, fresh=False, *, source=None, profile=None, **kwargs):
        path = Path(source) if source else canonical
        calls.append((path, fresh))
        raw = path.read_bytes()
        sha = env.digest_bytes(raw)
        bridge = b'cm_x87_CIpow' in raw
        killed = b'+ 1.0L' in raw and b'OBSERVED' in raw
        status = 'fail' if killed else 'pass'
        return {
            'harness_version': 1, 'harness_sha256': identity['harness'],
            'toolchain': identity['toolchain'], 'va': f'0x{va:08x}', 'tier': 'c',
            'profile': profile or 'semantic', 'status': status, 'source': env.rel(path),
            'source_sha256': sha, 'key': sha[:16], 'execution_identity': identity,
            'effective_options_sha256': digest, 'preprocessed_sha256': {'-O0': sha, '-O2': sha},
            'source_dependencies': {env.rel(path): sha, 'recovery/include/cmre.h': digest},
            'builds': {opt: {'status': status, 'cipow_calls': int(bridge),
                             'difftest': {'coverage': 1, 'inconclusive': 0, 'result_digest': sha,
                                          'mismatches': [{'case': 0, 'field': 'st0'}] if killed else []}}
                       for opt in recover.C_OPT_LEVELS},
        }

    monkeypatch.setattr(recover, 'test_c', gate)
    return canonical, calls, gate


@pytest.mark.parametrize('killed', [False, True])
def test_direct_c_acceptance_requires_both_operand_controls(evidence, killed):
    canonical, calls, _ = evidence
    canonical.write_text('long double f(void) { return cm_x87_CIpow(2.0L, 3.0L); }'
                         + (' // OBSERVED' if killed else ''), encoding='utf-8')
    before = ledger.record_path(VA).read_bytes()

    ok, result = recover.accept(VA, 'c')

    assert ok == killed
    assert {p.name for p, _ in calls if p != canonical} == {'call-000-base.c', 'call-000-exp.c'}
    if killed:
        proof = json.loads(ledger.record_path(VA).read_bytes())['c']['operand_evidence']
        assert proof['sha256'] == env.digest(env.ROOT / proof['path'])
        assert calls[-1] == (canonical, True)
    else:
        assert result['status'] == 'operand-evidence-needed'
        assert 'survivor' in result['detail']
        assert ledger.record_path(VA).read_bytes() == before


def audit_origin(run, fn, snapshot):
    directory = run / 'audit/snapshots/17'
    directory.mkdir(parents=True)
    for key, name in (('c', 'source.c'), ('hint', 'hints.json')):
        if snapshot[key] is not None:
            (directory / name).write_bytes(snapshot[key])
    metadata = directory / 'identity.json'
    metadata.write_text(json.dumps({'fn': fn, 'kind': 'audit', 'n': 17,
                                   'identity': acceptance_request._identity(snapshot),
                                   'bundle': None, 'bundle_sha256': None, 'confirm': False}))
    verdict = run / 'audit/verdict.json'
    findings = [{'detail': 'original branch is missing', 'evidence': 'synthetic instruction witness'}]
    verdict.write_text(json.dumps({'verdict': 'defect', 'findings': findings}))
    return {'review': {'snapshot': str(directory), 'snapshot_metadata_sha256': env.digest(metadata),
                       'audit_path': str(verdict), 'verdict_sha256': env.digest(verdict),
                       'bundle': None, 'bundle_sha256': None}, 'findings': findings}


def request_for(canonical, gate, *, kind='translate', reviews=(), prior_hint=None, legacy=False):
    directory = env.ANALYSIS / 'farm' / 'acceptance'
    directory.mkdir(parents=True)
    report = directory / 'gate.json'
    report.write_text(json.dumps(gate(VA)), encoding='utf-8')
    request = directory / 'request.json'
    hint = acceptance_request.optional_bytes(env.HINTS_DIR / f'fn_{VA:08x}.json')
    data = {
        'schema': 'cmre-farm-acceptance/1', 'fn': f'fn_{VA:08x}', 'job': 1, 'kind': kind,
        'source_sha256': env.digest(canonical), 'hint_sha256': acceptance_request.optional_digest(hint),
        'prior_hint': prior_hint.hex() if prior_hint is not None else None,
        'canonical_gate': {'path': str(report), 'sha256': env.digest(report)},
        'operand_proof': None, 'reviews': list(reviews),
    }
    if not legacy:
        snapshot = {'c': canonical.read_bytes(), 'hint': prior_hint}
        origin = audit_origin(directory.parent, data['fn'], snapshot) if kind == 'fix' else None
        dispatch = acceptance_request.register_dispatch(
            directory.parent, env.ROOT, data['fn'], 1, kind, snapshot, origin=origin)
        data.update(schema=acceptance_request.SCHEMA, dispatch=dispatch)
        del data['prior_hint'], data['reviews']
    request.write_text(json.dumps(data), encoding='utf-8')
    return request


@pytest.mark.parametrize('drift', [None, 'source', 'hints', 'execution', 'options', 'review', 'missing-proof'])
def test_farm_acceptance_rechecks_typed_request_under_the_lease(evidence, monkeypatch, drift):
    canonical, _, gate = evidence
    canonical.write_text('int f(void) { return 1; }', encoding='utf-8')
    if drift == 'missing-proof':
        canonical.write_text('long double f(void) { return cm_x87_CIpow(2.0L, 3.0L); }', encoding='utf-8')
    before = ledger.record_path(VA).read_bytes()
    if drift == 'review':
        request = request_for(canonical, gate, kind='fix')
        origin = env.ANALYSIS / 'farm/audit/verdict.json'
        origin.write_text('{"changed":true}', encoding='utf-8')
    else:
        request = request_for(canonical, gate)
    if drift == 'source':
        canonical.write_text('int f(void) { return 2; }', encoding='utf-8')
    elif drift == 'hints':
        (env.HINTS_DIR / f'fn_{VA:08x}.json').write_text('{"cases":2}', encoding='utf-8')
    elif drift in ('execution', 'options'):
        def drifted_gate(*args, **kwargs):
            report = gate(*args, **kwargs)
            if drift == 'execution':
                report['execution_identity'] = {**report['execution_identity'], 'harness': '2' * 64}
            else:
                report['effective_options_sha256'] = '2' * 64
            return report

        monkeypatch.setattr(recover, 'test_c', drifted_gate)
    code = cli.main(['farm-accept', f'fn_{VA:08x}', '--request', str(request)])
    if drift is None:
        assert code == 0
        assert json.loads(ledger.record_path(VA).read_bytes())['c']['source_sha256'] == env.digest(canonical)
    else:
        assert code == 3
        assert ledger.record_path(VA).read_bytes() == before


def test_farm_acceptance_rejects_weakened_shared_hints(evidence):
    canonical, _, gate = evidence
    canonical.write_text('int f(void) { return 1; }', encoding='utf-8')
    (env.HINTS_DIR / f'fn_{VA:08x}.json').write_text('{"cases":2}', encoding='utf-8')
    request = request_for(canonical, gate, prior_hint=b'{"cases":3}')
    before = ledger.record_path(VA).read_bytes()
    ok, result = recover.accept_farm(VA, request)
    assert not ok and 'shared hints' in result['detail']
    assert ledger.record_path(VA).read_bytes() == before


@pytest.mark.parametrize('attack', ['forged-prior', 'empty-review'])
def test_sender_claims_do_not_authorize_farm_acceptance(evidence, attack):
    """Exercise the authoritative CLI, not just a schema helper."""
    canonical, _, gate = evidence
    canonical.write_text('int f(void) { return 1; }', encoding='utf-8')
    if attack == 'forged-prior':
        hint = env.HINTS_DIR / f'fn_{VA:08x}.json'
        hint.write_bytes(b'{"cases":1}')
        request = request_for(canonical, gate, prior_hint=hint.read_bytes(), legacy=True)
    else:
        review = env.ANALYSIS / 'farm' / 'unrelated.json'
        review.parent.mkdir(parents=True)
        review.write_bytes(b'{}')
        request = request_for(canonical, gate, kind='fix',
                              reviews=[{'path': str(review), 'sha256': env.digest(review)}], legacy=True)
    before = ledger.record_path(VA).read_bytes()
    assert cli.main(['farm-accept', f'fn_{VA:08x}', '--request', str(request)]) == 3
    assert ledger.record_path(VA).read_bytes() == before


@pytest.mark.parametrize('attack', [
    None, 'forged-prior', 'empty-review', 'other-va', 'origin-identity',
    'nondefect-verdict', 'changed-verdict', 'changed-dispatch', 'changed-snapshot',
])
def test_registered_dispatch_and_origin_are_verified_by_cli(evidence, attack):
    canonical, _, gate = evidence
    canonical.write_text('int f(void) { return 1; }', encoding='utf-8')
    hint = env.HINTS_DIR / f'fn_{VA:08x}.json'
    hint.write_bytes(b'{"cases":3}')
    request = request_for(canonical, gate, kind='fix', prior_hint=hint.read_bytes())
    data = json.loads(request.read_bytes())
    authority_path = Path(data['dispatch']['path'])
    authority = json.loads(authority_path.read_bytes())
    origin = authority['origin']['review']
    verdict = Path(origin['audit_path'])
    metadata_path = Path(origin['snapshot']) / 'identity.json'
    if attack == 'forged-prior':
        # Keep the actual immutable dispatched hints at cases=3. Rebind every
        # sender-controlled current field to cases=1: authority must still win.
        hint.write_bytes(b'{"cases":1}')
        data['hint_sha256'] = env.digest(hint)
    elif attack in ('empty-review', 'nondefect-verdict'):
        verdict.write_text('{}' if attack == 'empty-review' else '{"verdict":"ok","findings":[]}')
        origin['verdict_sha256'] = env.digest(verdict)  # correctly hashed attack
    elif attack in ('other-va', 'origin-identity'):
        metadata = json.loads(metadata_path.read_bytes())
        if attack == 'other-va':
            metadata['fn'] = 'fn_00402000'
        else:
            metadata['identity']['c'] = 'f' * 64
        metadata_path.write_text(json.dumps(metadata))
        origin['snapshot_metadata_sha256'] = env.digest(metadata_path)
    elif attack == 'changed-verdict':
        verdict.write_text('{"verdict":"defect","findings":["changed"]}')
    elif attack == 'changed-dispatch':
        (authority_path.parent / 'receipt.json').write_bytes(b'{}')
    elif attack == 'changed-snapshot':
        (authority_path.parent / 'hints.json').write_bytes(b'{"cases":1}')
    if attack in ('empty-review', 'nondefect-verdict', 'other-va', 'origin-identity'):
        # Even internally consistent hashes cannot substitute for semantics.
        authority_path.write_text(json.dumps(authority))
        data['dispatch']['sha256'] = env.digest(authority_path)
    report_path = Path(data['canonical_gate']['path'])
    report_path.write_text(json.dumps(gate(VA)))
    data['canonical_gate']['sha256'] = env.digest(report_path)
    request.write_text(json.dumps(data))
    before = ledger.record_path(VA).read_bytes()
    code = cli.main(['farm-accept', f'fn_{VA:08x}', '--request', str(request)])
    assert code == (0 if attack is None else 3)
    if attack:
        assert ledger.record_path(VA).read_bytes() == before


@pytest.mark.parametrize('kind', ['translate', 'fix', 'adopt'])
@pytest.mark.parametrize('drift', [None, 'dispatch', 'origin'])
def test_real_farm_request_reaches_leased_acceptance_and_rechecks_after_fresh_gate(
        evidence, monkeypatch, kind, drift):
    """Real producer/CLI/acceptance; only model work and differential execution are fake."""
    from scripts.tests.test_cmre_farm import (
        bound_automatic_fix, make, writes, item, GOOD, reviewed_request)

    canonical, _, gate = evidence
    fn = f'fn_{VA:08x}'
    if kind == 'fix':
        f, adapter, _, _ = bound_automatic_fix(env.ROOT)
    else:
        canonical.write_text(GOOD, encoding='utf-8')
        f, adapter, _ = make(env.ROOT, [item(fn)] if kind == 'translate' else [],
                             writes(GOOD + '// PASS candidate\n'), workers=1)
    if kind == 'adopt':
        candidate = env.ROOT / 'hand.c'
        candidate.write_text(GOOD + '// PASS reviewed repair\n', encoding='utf-8')
        reviewed_request(f, fn, 'adopt', source=str(candidate))
    adapter.test = lambda function, source=None: gate(VA, source=source)
    results = []

    def accept(function, *, request):
        results.append(cli.main(['farm-accept', function, '--request', str(request)]))
        return results[-1], 'leased acceptance'

    adapter.accept = accept
    before = ledger.record_path(VA).read_bytes()
    monkeypatch.setenv('CMRE_CLAIM_OWNER', f.config.owner)
    if kind == 'adopt':
        f._requests()
    else:
        f._dispatch()
        f._reap()
    f._collect()
    assert len(f.accept_queue) == 1
    job = f.accept_queue[0]
    dispatch = Path(job['_dispatch_authority']['path'])
    authority = json.loads(dispatch.read_bytes())

    def final_gate(*args, fresh=False, **kwargs):
        result = gate(*args, fresh=fresh, **kwargs)
        if fresh and drift is not None:
            assert os.environ.get('CMRE_WRITER_TOKEN'), 'must mutate inside the actual command lease'
            if drift == 'origin' and kind == 'fix':
                Path(authority['origin']['review']['audit_path']).write_bytes(b'{}')
            elif drift == 'origin' and kind == 'adopt':
                Path(authority['origin']['receipt']['path']).write_bytes(b'{}')
            else:
                (dispatch.parent / 'source.c').write_bytes(b'changed dispatched source')
        return result

    monkeypatch.setattr(recover, 'test_c', final_gate)
    f._accept()
    assert results == [0 if drift is None else 3]
    if drift is not None:
        assert ledger.record_path(VA).read_bytes() == before
    else:
        assert json.loads(ledger.record_path(VA).read_bytes())['c']['source_sha256'] == env.digest(canonical)


def test_legacy_pending_transaction_is_not_rebound_to_current_inputs(evidence):
    from scripts.tests.test_cmre_farm import make, writes, GOOD

    canonical, _, _ = evidence
    canonical.write_text(GOOD, encoding='utf-8')
    f, _, _ = make(env.ROOT, [], writes(GOOD), workers=0)
    job = f._new_job('translate', f'fn_{VA:08x}', 'historical', bytes=200)
    with pytest.raises(ValueError, match='explicit redispatch'):
        f._write_acceptance_request(job, {'identity': {'c': env.digest(canonical)}})
    assert not (f.run_dir / 'dispatch').exists()


@pytest.mark.parametrize('changed', [False, True])
def test_byte_identical_maintenance_preserves_historical_operand_contract(evidence, changed):
    canonical, calls, _ = evidence
    raw = b'long double f(void) { return cm_x87_CIpow(2.0L, 3.0L); }'
    canonical.write_bytes(raw)
    path = ledger.record_path(VA)
    record = json.loads(path.read_bytes())
    retained = {'path': 'retained/summary.json', 'sha256': '3' * 64, 'status': 'current'}
    record['c'] = {'source_sha256': env.digest(canonical), 'operand_evidence': retained}
    path.write_text(json.dumps(record))
    before = path.read_bytes()
    if changed:
        canonical.write_bytes(raw + b'\n// amended')
    ok, report = recover.accept(VA, 'c', refresh=True)
    assert ok is not changed
    assert all(source == canonical for source, _ in calls), 'maintenance must not generate fresh operand proof'
    if changed:
        assert report['status'] == 'source-drift'
        assert path.read_bytes() == before
    else:
        assert json.loads(path.read_bytes())['c']['operand_evidence'] == {**retained, 'status': 'historical'}
