"""CLI status and process exits give workers unambiguous outcomes."""
from types import SimpleNamespace

from cmre import cli, tasks


def test_failed_attempt_exits_nonzero_before_attempt_budget_exhausted(monkeypatch):
    result = {'status': 'claimed', 'attempts': [{'status': 'failed'}]}
    monkeypatch.setattr(tasks, 'submit', lambda *a, **k: result)
    monkeypatch.setattr(tasks, 'summary', lambda *a, **k: result)
    args = SimpleNamespace(action='submit', task_id='task', owner='worker', token='token',
                           metrics=None, outcome='candidate', summary='')
    assert cli.cmd_task(args) == 1


def test_successful_submission_exits_zero(monkeypatch):
    result = {'status': 'submitted', 'attempts': [{'status': 'pass'}]}
    monkeypatch.setattr(tasks, 'submit', lambda *a, **k: result)
    monkeypatch.setattr(tasks, 'summary', lambda *a, **k: result)
    args = SimpleNamespace(action='submit', task_id='task', owner='worker', token='token',
                           metrics=None, outcome='candidate', summary='')
    assert cli.cmd_task(args) == 0


def test_revise_routes_reason_and_does_not_disclose_a_lease(monkeypatch):
    seen = []
    result = {'status': 'prepared', 'lease': None}
    monkeypatch.setattr(tasks, 'revise', lambda task_id, reason: seen.append((task_id, reason)) or result, raising=False)
    monkeypatch.setattr(tasks, 'summary', lambda value, **kw: seen.append(kw) or value)
    args = SimpleNamespace(action='revise', task_id='task', reason='Composition declaration conflict')
    assert cli.cmd_task(args) == 0
    assert seen == [('task', args.reason), {'include_token': False}]
