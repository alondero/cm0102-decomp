"""Machine restart resumes the frozen selection and rejects changed evidence."""
import pytest

from cmre import env, jobs, recover


@pytest.fixture
def isolated(tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    monkeypatch.setattr(recover, 'execution_identity', lambda: {'compiler': 'one'})
    monkeypatch.setattr(recover, 'harness_digest', lambda: 'one')


def test_resume_preserves_selection_and_results(isolated):
    first = jobs.Job('batch', [1, 2, 3], {'n': 3})
    first.record(1, True, 'accepted')
    resumed = jobs.Job('batch', [4, 5, 6], {'n': 3}, str(first.path))
    assert resumed.pending == [2, 3]
    resumed.record(2, False, 'low coverage')
    resumed.record(3, True, 'accepted')
    resumed.finish()
    assert resumed.data['state'] == 'finished'
    assert resumed.failures == 1


def _revalidate_selection(monkeypatch, tmp_path, vas):
    from types import SimpleNamespace
    from cmre import cli, ledger
    # These tests exercise the retained schema-2 worker/resume contract.
    monkeypatch.setattr(cli, 'cmd_revalidate', cli._legacy_revalidate)
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(tmp_path / 'command.lock'))
    monkeypatch.setattr(ledger, 'cmre_entries', lambda: [SimpleNamespace(va=va) for va in vas])


def test_an_item_exception_is_recorded_without_aborting_revalidation(isolated, tmp_path, monkeypatch, capsys):
    # 2026-10-21: one KeyError from a worker build ended a 7,929-item job at item 4,089.
    from cmre import cli
    _revalidate_selection(monkeypatch, tmp_path, [1, 2, 3])
    def one(va, refresh=False):
        if va == 2:
            raise KeyError('_cm_fn_00953950')
        return va, 'ok'
    monkeypatch.setattr(cli, '_revalidate_one', one)
    assert cli.main(['revalidate', '--jobs', '1']) == 1
    receipt = jobs.Job('revalidate', [1, 2, 3], {'n': 10**9, 'refresh': False},
                       str(next((tmp_path / 'cmre/jobs').glob('*.json'))))
    assert receipt.pending == [] and receipt.failures == 1
    assert "fn_00000002 ERROR KeyError: '_cm_fn_00953950'" in capsys.readouterr().out


def test_a_transient_item_exception_is_retried_once(isolated, tmp_path, monkeypatch, capsys):
    from cmre import cli
    _revalidate_selection(monkeypatch, tmp_path, [1])
    calls = []
    def flaky(va, refresh=False):
        calls.append(va)
        if len(calls) == 1:
            raise OSError('nm: cannot open file')
        return va, 'ok'
    monkeypatch.setattr(cli, '_revalidate_one', flaky)
    assert cli.main(['revalidate', '--jobs', '1']) == 0
    assert calls == [1, 1]
    assert 'retried after OSError' in capsys.readouterr().out


def test_changed_environment_cannot_reuse_completion(isolated, monkeypatch):
    first = jobs.Job('revalidate', [1], {'refresh': True})
    first.record(1, True, 'ok')
    monkeypatch.setattr(recover, 'execution_identity', lambda: {'compiler': 'two'})
    with pytest.raises(ValueError, match='inputs changed'):
        jobs.Job('revalidate', [1], {'refresh': True}, str(first.path))


def test_wrong_job_kind_cannot_resume(isolated):
    first = jobs.Job('batch', [1], {})
    with pytest.raises(ValueError, match='kind/schema'):
        jobs.Job('revalidate', [1], {}, str(first.path))


def test_changed_completed_hint_rejects_stale_success(isolated, tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path / 'hints')
    env.HINTS_DIR.mkdir()
    hint = env.HINTS_DIR / 'fn_00000001.json'
    hint.write_text('{}')
    first = jobs.Job('revalidate', [1, 2], {})
    first.record(1, True, 'ok')
    hint.write_text('{"seed": 123}')
    with pytest.raises(ValueError, match='fn_00000001'):
        jobs.Job('revalidate', [1, 2], {}, str(first.path))


def test_intentional_refresh_uses_postvalidation_ledger_identity(isolated, tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'LEDGER_DIR', tmp_path / 'ledger')
    env.LEDGER_DIR.mkdir()
    record = env.LEDGER_DIR / 'fn_00000001.json'
    record.write_text('{"harness": "old"}')
    first = jobs.Job('revalidate', [1], {})
    record.write_text('{"harness": "new"}')
    first.record(1, True, 'ok')
    assert jobs.Job('revalidate', [1], {}, str(first.path)).pending == []


def test_unrecorded_refresh_of_an_interrupted_item_is_rerun_on_resume(isolated, tmp_path, monkeypatch):
    # 2026-10-22: a crashed job's in-flight pool workers published 33 refreshed ledgers
    # without receipts; resume refused although those items had to rerun anyway.
    for name in ('LEDGER_DIR', 'HINTS_DIR'):
        monkeypatch.setattr(env, name, tmp_path / name)
        (tmp_path / name).mkdir()
    for va in (1, 2):
        (env.LEDGER_DIR / f'fn_{va:08x}.json').write_text('{"harness": "old"}')
    first = jobs.Job('revalidate', [1, 2, 3], {'refresh': True})
    first.record(1, True, 'ok')
    (env.LEDGER_DIR / 'fn_00000002.json').write_text('{"harness": "new"}')
    assert jobs.Job('revalidate', [1, 2, 3], {'refresh': True}, str(first.path)).pending == [2, 3]
    (env.HINTS_DIR / 'fn_00000003.json').write_text('{"seed": 1}')  # any other input still refuses
    with pytest.raises(ValueError, match='fn_00000003'):
        jobs.Job('revalidate', [1, 2, 3], {'refresh': True}, str(first.path))


def test_pending_ledger_change_without_refresh_still_refuses(isolated, tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'LEDGER_DIR', tmp_path / 'ledger')
    env.LEDGER_DIR.mkdir()
    record = env.LEDGER_DIR / 'fn_00000002.json'
    record.write_text('{"harness": "old"}')
    first = jobs.Job('revalidate', [1, 2], {})
    record.write_text('{"harness": "new"}')
    with pytest.raises(ValueError, match='fn_00000002'):
        jobs.Job('revalidate', [1, 2], {}, str(first.path))


def test_status_reads_item_completions_before_final_manifest(isolated):
    job = jobs.Job('revalidate', [1, 2, 3], {})
    job.record(1, True, 'ok')
    job.record(2, False, 'mismatch')
    observed = jobs.describe()
    assert observed['recorded_state'] == 'running'
    assert (observed['completed'], observed['passed'], observed['failed'], observed['remaining']) == (2, 1, 1, 1)


def test_retry_failed_preserves_original_receipt_and_reuses_only_successes(isolated, tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path / 'hints')
    env.HINTS_DIR.mkdir()
    first = jobs.Job('revalidate', [1, 2], {'refresh': True})
    first.record(1, True, 'ok')
    first.record(2, False, 'budget asymmetry')
    first.finish()
    original = first.path.read_bytes()
    (env.HINTS_DIR / 'fn_00000002.json').write_text('{"budget": 800000}')
    retry = jobs.Job('revalidate', [1, 2], {'refresh': True}, str(first.path), retry_failed=True)
    assert retry.path != first.path and first.path.read_bytes() == original
    assert retry.pending == [2] and retry.data['previous_failures']['00000002']['detail'] == 'budget asymmetry'
    retry.record(2, True, 'ok')
    retry.finish()
    assert retry.failures == 0 and len(retry.data['results']) == 2


def test_retry_failed_reruns_changed_success_and_rejects_incomplete_selection(isolated, tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path / 'hints')
    env.HINTS_DIR.mkdir()
    first = jobs.Job('revalidate', [1, 2], {})
    first.record(1, True, 'ok')
    first.record(2, False, 'fail')
    first.finish()
    with pytest.raises(ValueError, match='selection'):
        jobs.Job('revalidate', [1], {}, str(first.path), retry_failed=True)
    (env.HINTS_DIR / 'fn_00000001.json').write_text('{"seed": 123}')
    original = first.path.read_bytes()
    retry = jobs.Job('revalidate', [1, 2], {}, str(first.path), retry_failed=True)
    assert retry.pending == [1, 2]
    assert retry.data['previous_changed_successes']['00000001']['ok']
    assert first.path.read_bytes() == original


def test_retry_never_reuses_success_under_changed_harness(isolated, monkeypatch):
    first = jobs.Job('revalidate', [1, 2], {})
    first.record(1, True, 'ok')
    first.record(2, False, 'fail')
    first.finish()
    monkeypatch.setattr(recover, 'harness_digest', lambda: 'changed')
    with pytest.raises(ValueError, match='inputs changed'):
        jobs.Job('revalidate', [1, 2], {}, str(first.path), retry_failed=True)
