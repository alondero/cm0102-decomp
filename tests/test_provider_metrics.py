import json
from types import SimpleNamespace

import pytest

from cmre import inventory, provider_campaign, provider_jobs, provider_metrics


def store(tmp_path):
    directory = tmp_path / 'run'
    state = provider_jobs.create(directory, {'duration_seconds': 900, 'jobs': [
        {'id': 'alpha', 'config': {'function': '0x00401000', 'mode': 'c'}},
        {'id': 'beta', 'config': {'function': '0x00401010', 'mode': 'input'}},
    ]}, {})
    provider_campaign._ensure_run(directory, state, lambda: 100)
    return directory


def receipt(directory, job='alpha', phase='work', **values):
    path = directory / 'jobs' / job / 'rounds/1/a0001-owner/pair' / phase / 'receipt.json'
    path.parent.mkdir(parents=True, exist_ok=True)
    value = {'schema': 'cmre-provider-worker/1', 'state': 'completed',
             'model_requested': 'model-a', 'elapsed_seconds': 30, 'calls': []}
    value.update(values)
    path.write_text(json.dumps(value))
    return path


def test_report_retains_unknown_usage_and_running_time(tmp_path):
    directory = store(tmp_path)
    receipt(directory, phase='smoke', state='running', started=150, calls=None)
    result = provider_metrics.report(directory, clock=lambda: 200)
    assert result['elapsed_seconds'] == 100
    assert result['workers'][0]['worker_seconds'] == 50
    assert result['workers'][0]['running'] == 1
    assert result['workers'][0]['tokens']['output_tokens'] is None
    assert result['accepted_c_body_bytes'] == 0
    assert result['accepted_c_bytes_per_wall_hour'] == 0


def test_report_deduplicates_calls_and_keeps_usage_classes_separate(tmp_path):
    directory = store(tmp_path)
    call = {'call_id': 'call-1', 'model': 'model-a', 'usage_observed': True,
            'input_tokens': 10, 'cache_read_tokens': 20, 'cache_write_tokens': 0,
            'output_tokens': 30, 'reasoning_tokens': 5}
    receipt(directory, calls=[call, call])
    result = provider_metrics.report(directory, clock=lambda: 200)
    group = result['workers'][0]
    assert group['calls'] == 1
    assert group['tokens'] == {field: call[field] for field in provider_metrics.TOKEN_FIELDS}
    assert 'cost' not in group


def test_empty_calls_after_export_failure_stay_unknown(tmp_path):
    directory = store(tmp_path)
    receipt(directory, state='export_failed', calls=[])
    result = provider_metrics.report(directory, clock=lambda: 200)
    assert result['workers'][0]['tokens'] == dict.fromkeys(provider_metrics.TOKEN_FIELDS)


def test_conflicting_duplicate_usage_stays_unknown(tmp_path):
    directory = store(tmp_path)
    call = {'call_id': 'call-1', 'model': 'model-a', 'usage_observed': True,
            'input_tokens': 10, 'cache_read_tokens': 20, 'cache_write_tokens': 0,
            'output_tokens': 30, 'reasoning_tokens': 5}
    receipt(directory, calls=[call, dict(call, output_tokens=999)])
    result = provider_metrics.report(directory, clock=lambda: 200)
    assert result['workers'][0]['tokens']['output_tokens'] is None
    assert result['diagnostics']


def test_accepted_body_bytes_use_inventory_not_packet_limit(tmp_path, monkeypatch):
    directory = store(tmp_path)
    provider_jobs.update(directory, 'alpha', {'state': 'accepted'}, expected_revision=0)
    monkeypatch.setattr(inventory, 'load', lambda: SimpleNamespace(functions={
        0x00401000: SimpleNamespace(body_bytes=99)}))
    result = provider_metrics.report(directory, clock=lambda: 3700)
    assert result['accepted_c_functions'] == 1
    assert result['accepted_c_body_bytes'] == 99
    assert result['accepted_c_bytes_per_wall_hour'] == 99
    assert result['modes'] == {'c': {'accepted': 1}, 'input': {'prepared': 1}}


def test_unavailable_inventory_does_not_invent_accepted_bytes(tmp_path, monkeypatch):
    directory = store(tmp_path)
    provider_jobs.update(directory, 'alpha', {'state': 'accepted'}, expected_revision=0)
    monkeypatch.setattr(inventory, 'load', lambda: SimpleNamespace(functions={}))
    result = provider_metrics.report(directory, clock=lambda: 200)
    assert result['accepted_c_functions'] == 1
    assert result['accepted_c_body_bytes'] is None
    assert result['accepted_c_bytes_per_wall_hour'] is None
    assert result['diagnostics']


def test_incomplete_receipt_is_diagnostic_and_output_is_bounded(tmp_path):
    directory = store(tmp_path)
    receipt(directory).write_text('{')
    provider_jobs.update(directory, 'alpha', {'state': 'held', 'hold_reason': 'x' * 10000}, expected_revision=0)
    result = provider_metrics.report(directory, clock=lambda: 200)
    assert result['diagnostics']
    assert len(result['holds'][0]['reason']) == 300
    assert 'xxxxxxxxxx' not in json.dumps(result['workers'])


def test_report_rejects_tampered_store(tmp_path):
    directory = store(tmp_path)
    path = directory / 'state.json'
    state = json.loads(path.read_text())
    state['jobs']['alpha']['state'] = 'accepted'
    path.write_text(json.dumps(state))
    with pytest.raises(provider_jobs.CorruptStore):
        provider_metrics.report(directory)


def test_report_rejects_edited_deadline(tmp_path):
    directory = store(tmp_path)
    path = directory / 'run.json'
    timing = json.loads(path.read_text())
    timing['deadline'] += 100
    path.write_text(json.dumps(timing))
    with pytest.raises(provider_campaign.CampaignError):
        provider_metrics.report(directory)


def test_cli_report_dispatch(tmp_path, capsys):
    from cmre import cli
    directory = store(tmp_path)
    assert cli.main(['pipeline', 'report', str(directory)]) == 0
    result = json.loads(capsys.readouterr().out)
    assert result['schema'] == 'cmre-provider-metrics/1'


def test_holds_show_recent_revisions_and_bounded_categories(tmp_path):
    directory = store(tmp_path)
    provider_jobs.update(directory, 'beta', {'state': 'held', 'hold_reason': 'actor error: bad action'},
                         expected_revision=0)
    provider_jobs.update(directory, 'alpha', {'state': 'held', 'hold_reason': 'Evidence unknown'},
                         expected_revision=0)
    result = provider_metrics.report(directory, clock=lambda: 200)
    assert [row['job'] for row in result['holds']] == ['alpha', 'beta']
    assert result['hold_categories'] == {'other': 1, 'actor': 1}
