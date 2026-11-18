"""Synthetic end-to-end acceptance checks with an isolated ledger."""
import json
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


def request_for(canonical, gate, *, kind='translate', reviews=(), prior_hint=None):
    directory = env.ANALYSIS / 'farm' / 'acceptance'
    directory.mkdir(parents=True)
    report = directory / 'gate.json'
    report.write_text(json.dumps(gate(VA)), encoding='utf-8')
    request = directory / 'request.json'
    hint = acceptance_request.optional_bytes(env.HINTS_DIR / f'fn_{VA:08x}.json')
    request.write_text(json.dumps({
        'schema': acceptance_request.SCHEMA, 'fn': f'fn_{VA:08x}', 'job': 1, 'kind': kind,
        'source_sha256': env.digest(canonical), 'hint_sha256': acceptance_request.optional_digest(hint),
        'prior_hint': prior_hint.hex() if prior_hint is not None else None,
        'canonical_gate': {'path': str(report), 'sha256': env.digest(report)},
        'operand_proof': None, 'reviews': list(reviews),
    }), encoding='utf-8')
    return request


@pytest.mark.parametrize('drift', [None, 'source', 'hints', 'execution', 'options', 'review', 'missing-proof'])
def test_farm_acceptance_rechecks_typed_request_under_the_lease(evidence, monkeypatch, drift):
    canonical, _, gate = evidence
    canonical.write_text('int f(void) { return 1; }', encoding='utf-8')
    if drift == 'missing-proof':
        canonical.write_text('long double f(void) { return cm_x87_CIpow(2.0L, 3.0L); }', encoding='utf-8')
    before = ledger.record_path(VA).read_bytes()
    if drift == 'review':
        origin = env.ANALYSIS / 'farm' / 'review.json'
        origin.parent.mkdir()
        origin.write_text('{}', encoding='utf-8')
        request = request_for(canonical, gate, kind='fix',
                              reviews=[{'path': str(origin), 'sha256': env.digest(origin)}])
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
