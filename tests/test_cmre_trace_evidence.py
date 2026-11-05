"""Final-byte diagnostics fail closed on capped, clipped and legacy traces."""
import hashlib
import json

import pytest

from cmre import cli, trace_evidence


def report(outcome='return'):
    return {'case': 17, 'difference': {'field': 'memory', 'pages': ['0x3000000']},
            'original': {'outcome': outcome}, 'recovered': {'outcome': outcome},
            'events_original_raw': ["('w', 50331648, 2, 1)"],
            'events_recovered_raw': ["('w', 50331648, 2, 2)"]}


@pytest.mark.parametrize('outcome', ['return', 'fault'])
def test_complete_memory_streams_admit_returns_and_faults(outcome):
    result = trace_evidence.describe(report(outcome), 10000)
    assert result['complete']
    assert result['sides']['recovered']['raw_start'] == 0


@pytest.mark.parametrize('sides', [('original',), ('recovered',), ('original', 'recovered')])
def test_cap_in_either_stream_prevents_final_byte_inference(sides):
    data = report()
    for side in sides:
        data[f'events_{side}_raw'] *= 6
    result = trace_evidence.describe(data, 6)
    assert not result['complete']
    assert all(not result['sides'][side]['complete'] for side in sides)


@pytest.mark.parametrize('change', ['context', 'index', 'nonmemory', 'budget',
                                    'missing', 'malformed', 'empty'])
def test_unproven_streams_stay_incomplete(change):
    data = report()
    if change == 'context':
        data['event_context'] = {'original_raw_index': 50, 'recovered_raw_index': 60}
    elif change == 'index':
        data['difference']['index'] = 1
    elif change == 'nonmemory':
        data['difference']['field'] = 'regs'
    elif change == 'budget':
        data['recovered']['outcome'] = 'budget'
    elif change == 'missing':
        del data['events_original_raw']
    elif change == 'malformed':
        data['events_original_raw'] = [23]
    else:
        data['events_original_raw'] = []
    assert not trace_evidence.describe(data, 10000)['complete']


@pytest.mark.parametrize('window', [None, True, 0, -1, 10001, '10000'])
def test_unknown_or_invalid_window_fails_closed(window):
    assert not trace_evidence.describe(report(), window)['complete']


@pytest.mark.parametrize('event', ['', 'not an event', "('w', 1)",
                                  "('x', 1, 2, 3)", "('w', 1, 0, 3)",
                                  "('r', True, 2, 3)", "('c', 'name', 2, (1,))",
                                  "('c', 'name', 1, (-1,))"])
def test_invalid_serialized_event_prevents_completeness(event):
    data = report()
    data['events_recovered_raw'] = [event]
    assert not trace_evidence.describe(data, 10000)['complete']


def test_import_event_shape_is_supported():
    data = report()
    data['events_original_raw'].append("('c', 'name', 2, (0, 4294967295))")
    assert trace_evidence.describe(data, 10000)['complete']


@pytest.mark.parametrize('encoding', ['utf-8', 'utf-8-sig', 'utf-16', 'utf-16-be'])
def test_offline_checker_reads_windows_and_utf8_reports(tmp_path, encoding):
    data = report()
    data['trace_evidence'] = trace_evidence.describe(data, 10000)
    raw = json.dumps(data).encode(encoding)
    if encoding == 'utf-16-be':
        raw = b'\xfe\xff' + raw
    path = tmp_path / 'case.json'
    path.write_bytes(raw)
    assert trace_evidence.check_file(path)['trace_evidence']['complete']


def test_offline_checker_recomputes_flags_and_rejects_legacy(tmp_path):
    data = report()
    path = tmp_path / 'case.json'
    path.write_text(json.dumps(data), encoding='utf-8')
    assert not trace_evidence.check_file(path)['trace_evidence']['complete']
    data['events_recovered_raw'] *= 6
    data['trace_evidence'] = {'schema': trace_evidence.SCHEMA, 'window': 6, 'complete': True}
    path.write_text(json.dumps(data), encoding='utf-8-sig')
    assert not trace_evidence.check_file(path)['trace_evidence']['complete']


def test_offline_command_is_compact_and_does_not_replay(monkeypatch, tmp_path, capsys):
    from cmre import recover

    def no_vm(*args, **kwargs):
        raise AssertionError('offline check must not replay code')

    monkeypatch.setattr(recover, 'explain_c', no_vm)
    monkeypatch.setattr(recover, 'explain', no_vm)
    data = report()
    data['trace_evidence'] = trace_evidence.describe(data, 10000)
    data['unrelated_large_payload'] = 'x' * 100000
    path = tmp_path / 'case.json'
    path.write_text(json.dumps(data), encoding='utf-8-sig')
    assert cli.main(['trace-check', str(path)]) == 0
    output = capsys.readouterr().out
    summary = json.loads(output)
    assert summary['sha256'] == hashlib.sha256(path.read_bytes()).hexdigest()
    assert len(output) < 2000
    assert 'events_original_raw' not in summary


def test_offline_command_rejects_capped_report(tmp_path, capsys):
    data = report()
    data['events_recovered_raw'] *= 6
    data['trace_evidence'] = trace_evidence.describe(data, 6)
    path = tmp_path / 'capped.json'
    path.write_text(json.dumps(data), encoding='utf-8')
    assert cli.main(['trace-check', str(path)]) == 1
    assert not json.loads(capsys.readouterr().out)['trace_evidence']['complete']


def test_explain_adds_actual_requested_window_without_changing_exit(monkeypatch, capsys):
    from cmre import recover

    data = report()
    data['events_recovered_raw'] *= 6
    seen = []

    def explain(*args, **kwargs):
        seen.append(kwargs['window'])
        return data

    monkeypatch.setattr(recover, 'explain_c', explain)
    assert cli.main(['explain', 'fn_00401000', '--c', '--case', '17', '--window', '6']) == 0
    output = json.loads(capsys.readouterr().out)
    assert seen == [6]
    assert output['trace_evidence']['window'] == 6
    assert not output['trace_evidence']['complete']
