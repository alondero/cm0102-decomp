import json
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import cmre_lift_lane as lane


class Ops:
    """The capture store and cmre commands the lane drives, in memory."""

    def __init__(self, captures, recovered=(), apply_status=None, accept='accepted'):
        self.captures = {va: list(names) for va, names in captures.items()}
        self.recovered_set = set(recovered)
        self.apply_status = apply_status or {}
        self.accept_result = accept
        self.expanded, self.applied, self.accepts = [], [], []

    def stored(self):
        return sorted(self.captures)

    def recovered(self):
        return set(self.recovered_set)

    def names_for(self, va):
        return list(self.captures[va])

    def body_bytes(self, va):
        return 100

    def expand(self, vas, jobs):
        self.expanded.append(list(vas))
        for va in vas:
            self.captures[va].append(f'mut-{len(self.captures[va])}')
        return [{'va': f'{va:#010x}', 'added': 1} for va in vas]

    def apply(self, vas, jobs):
        self.applied.append(list(vas))
        return [{'va': f'{va:#010x}', **self.apply_status.get(va, {'status': 'low-coverage', 'coverage': 0.4})}
                for va in vas]

    def accept(self, fn):
        self.accepts.append(fn)
        result = self.accept_result(fn) if callable(self.accept_result) else self.accept_result
        if result == 'accepted':
            self.recovered_set.add(int(fn[3:], 16))
        return result


A, B, C = 0x00401000, 0x00402000, 0x00403000


def test_a_round_expands_applies_and_accepts_unrecovered_captured_functions(tmp_path):
    ops = Ops({A: ['c1'], B: ['c1'], C: ['c1']}, recovered={C},
              apply_status={A: {'status': 'pass', 'coverage': 0.9}})
    summary = lane.run_once(ops, tmp_path / 'state.json', jobs=2)
    assert ops.expanded == [[A, B]] and ops.applied == [[A, B]]
    assert ops.accepts == ['fn_00401000']
    assert summary['accepted'] == ['fn_00401000'] and summary['accepted_bytes'] == 100
    assert summary['statuses'] == {'pass': 1, 'low-coverage': 1}


def test_unchanged_captures_are_not_reprocessed(tmp_path):
    ops = Ops({A: ['c1']})
    lane.run_once(ops, tmp_path / 'state.json', jobs=1)
    summary = lane.run_once(ops, tmp_path / 'state.json', jobs=1)
    assert ops.expanded == [[A]] and summary['processed'] == 0


def test_new_captures_reopen_a_function(tmp_path):
    ops = Ops({A: ['c1']})
    lane.run_once(ops, tmp_path / 'state.json', jobs=1)
    ops.captures[A].append('recorded-overnight')
    lane.run_once(ops, tmp_path / 'state.json', jobs=1)
    assert ops.expanded == [[A], [A]]


def test_a_busy_writer_leaves_the_accept_for_the_next_round(tmp_path):
    answers = iter(['busy', 'accepted'])
    ops = Ops({A: ['c1']}, apply_status={A: {'status': 'pass', 'coverage': 0.9}}, accept=lambda fn: next(answers))
    first = lane.run_once(ops, tmp_path / 'state.json', jobs=1)
    assert first['accepted'] == [] and first['accept_waiting'] == ['fn_00401000']
    second = lane.run_once(ops, tmp_path / 'state.json', jobs=1)
    assert second['accepted'] == ['fn_00401000'] and ops.accepts == ['fn_00401000', 'fn_00401000']
    assert ops.expanded == [[A]]                    # the retry needs no new expansion


def test_a_refused_accept_is_recorded_and_not_retried_until_captures_change(tmp_path):
    ops = Ops({A: ['c1']}, apply_status={A: {'status': 'pass', 'coverage': 0.9}}, accept='refused: lift-blocked')
    lane.run_once(ops, tmp_path / 'state.json', jobs=1)
    lane.run_once(ops, tmp_path / 'state.json', jobs=1)
    assert ops.accepts == ['fn_00401000']
    row = json.loads((tmp_path / 'state.json').read_text())['functions']['fn_00401000']
    assert row['accept'] == 'refused: lift-blocked'


def test_errors_are_retried_a_bounded_number_of_times(tmp_path):
    ops = Ops({A: ['c1']}, apply_status={A: {'status': 'error', 'error': 'OSError'}})
    for _ in range(lane.MAX_ERRORS + 2):
        lane.run_once(ops, tmp_path / 'state.json', jobs=1)
    assert len(ops.applied) == lane.MAX_ERRORS


def test_functions_recovered_elsewhere_drop_out(tmp_path):
    ops = Ops({A: ['c1']}, apply_status={A: {'status': 'pass', 'coverage': 0.9}}, accept='busy')
    lane.run_once(ops, tmp_path / 'state.json', jobs=1)
    ops.recovered_set.add(A)                        # a manual session accepted it meanwhile
    summary = lane.run_once(ops, tmp_path / 'state.json', jobs=1)
    assert ops.accepts == ['fn_00401000'] and summary['accept_waiting'] == []


def test_limit_takes_the_largest_functions_first(tmp_path):
    ops = Ops({A: ['c1'], B: ['c1'], C: ['c1']})
    ops.body_bytes = {A: 10, B: 300, C: 20}.get
    lane.run_once(ops, tmp_path / 'state.json', jobs=1, limit=2)
    assert ops.expanded == [[B, C]]


def test_accept_command_classifies_cmre_output(monkeypatch):
    class Done:
        def __init__(self, code, out, err=''):
            self.returncode, self.stdout, self.stderr = code, out, err
    answers = {'fn_00401000': Done(0, '{"status": "accepted", "record": "x"}\n'),
               'fn_00402000': Done(1, '', 'cmre.coordination.BusyError: Writer active: lock'),
               'fn_00403000': Done(1, '{"status": "fail"}\n', 'lifted gate failed')}
    monkeypatch.setattr(lane.subprocess, 'run', lambda cmd, **kw: answers[cmd[-1]])
    ops = lane.CmreOps(Path('.'))
    assert ops.accept('fn_00401000') == 'accepted'
    assert ops.accept('fn_00402000') == 'busy'
    assert ops.accept('fn_00403000').startswith('refused')
