"""Publication is bound to the exact reviewed task attempt and report."""
import json
from pathlib import Path

import pytest

from cmre.provider_runtime import Executor, _hash, _pin


class FakeTaskAPI:
    def __init__(self, status):
        self._status = status
        self.published = []

    def status(self, task_id):
        return self._status

    def publish(self, task_id):
        self.published.append(task_id)
        return {'status': 'published'}


def make_case(tmp_path, monkeypatch, *, attempts=None, current_status='submitted',
              current_fingerprint='fp', reviewed_attempt=1, reviewed_fingerprint='fp',
              reviewed_report=True, review_model='review-model',
              latest_number=1, latest_status='pass',
              latest_report_sha=None, latest_candidate_sha=None):
    campaign = tmp_path / 'campaign'
    root = campaign / 'jobs' / 'compute'
    root.mkdir(parents=True)
    source = root / 'fn.c'
    source.write_text('int f(void) { return 1; }\n', encoding='utf-8')
    source_sha = _hash(source)
    report_path = root / 'report.json'
    report_path.write_text(json.dumps({'status': 'pass'}), encoding='utf-8')
    report_pin = _pin(report_path)
    if attempts is None:
        attempts = [{
            'number': latest_number,
            'status': latest_status,
            'candidate_sha256': latest_candidate_sha or source_sha,
            'report_sha256': latest_report_sha or report_pin['sha256'],
        }]
    config = {'directory': str(root), 'function': '0x004d1600', 'mode': 'c',
              'reviewer': 'reviewer-role', 'candidate': str(source)}
    job = {'id': 'compute', 'config': config}
    executor = Executor(campaign)
    monkeypatch.setattr(executor, '_execution', lambda: {'target': 'pinned'})
    from cmre import provider_jobs
    state = {'jobs': {'compute': {'author_model': 'author-model'}},
             'manifest': {'roles': {'reviewer-role': {'model': 'review-model'}}}}
    monkeypatch.setattr(provider_jobs, 'load', lambda directory: state)
    task = {'task_id': 'task-1', 'attempt': reviewed_attempt,
            'input_fingerprint': reviewed_fingerprint,
            'source_sha256': source_sha}
    if reviewed_report:
        task['report'] = report_pin
    payload = {'required_checks': ['exact-bundle'], 'execution': {'target': 'pinned'},
               'artifacts': {}, 'source': {'pin': _pin(source),
                                           'text': source.read_text(encoding='utf-8')},
               'task_id': 'task-1', 'task': task}
    bundle_path = root / 'bundle.json'
    bundle_path.write_text(json.dumps(payload), encoding='utf-8')
    bundle_sha = _hash(bundle_path)
    bundle = {'pin': _pin(bundle_path), 'sha256': bundle_sha,
              'required_checks': ['exact-bundle']}
    review = {'verdict': 'pass', 'unknowns': [], 'bundle_sha256': bundle_sha,
              'checks': ['exact-bundle']}
    action = {'bundle': bundle, 'review': review, 'review_model': review_model}
    api = FakeTaskAPI({'status': current_status,
                       'input_fingerprint': current_fingerprint,
                       'attempts': attempts})
    executor.task_api = api
    output = root / 'finalize'
    return executor, job, action, output, api, source_sha, report_pin


def finalize(case):
    executor, job, action, output, api, _, _ = case
    root = Path(job['config']['directory'])
    return executor._finalize(job, job['config'], root, action, output)


def test_historical_review_cannot_authorize_a_later_attempt(tmp_path, monkeypatch):
    case = make_case(tmp_path, monkeypatch)
    _, _, _, _, api, source_sha, _ = case
    api._status['attempts'].append({
        'number': 2, 'status': 'pass', 'candidate_sha256': source_sha,
        'report_sha256': 'b' * 64})
    with pytest.raises(ValueError):
        finalize(case)
    assert api.published == []


def test_exact_current_attempt_is_published_once(tmp_path, monkeypatch):
    case = make_case(tmp_path, monkeypatch, latest_number=2, reviewed_attempt=2)
    result = finalize(case)
    assert result['state'] == 'accepted'
    assert result['task_id'] == 'task-1'
    assert case[4].published == ['task-1']


@pytest.mark.parametrize('changes', [
    {'current_status': 'published'},
    {'latest_report_sha': '0' * 64},
    {'current_fingerprint': 'different'},
    {'latest_candidate_sha': '0' * 64},
    {'review_model': 'intruder-model'},
    {'reviewed_report': False},
    {'reviewed_fingerprint': None},
])
def test_nonexact_review_never_reaches_publisher(tmp_path, monkeypatch, changes):
    case = make_case(tmp_path, monkeypatch, **changes)
    with pytest.raises(ValueError):
        finalize(case)
    assert case[4].published == []
