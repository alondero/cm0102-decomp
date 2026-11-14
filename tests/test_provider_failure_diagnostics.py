"""Bounded failure-reason diagnostics for the OpenCode actor adapter.

A stage that did not complete now carries a short, safe suffix derived only
from the structured receipt state and call accounting: the last relevant call
finish label, its output and reasoning token counts and whether usage was
observed.  Absent, unobserved or malformed usage stays ``unknown``; no count
is invented and no raw text, provider error, environment or secret is named.
"""
from __future__ import annotations

import importlib.util
import json
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from cmre import provider_campaign as pc  # noqa: E402
from cmre.provider_opencode import OpenCodeActor, OpenCodeError  # noqa: E402

SECRET = 'sk-secret-value-42'
MODEL = 'deepseek/deepseek-chat'

LENGTH_CALL = {
    'call_id': 'msg_1c33c892d0011ZiFPeHv07Go7u',
    'model': 'minimax-coding-plan/MiniMax-M3.1-Flash',
    'input_tokens': 1059,
    'output_tokens': 16384,
    'reasoning_tokens': 0,
    'cache_read_tokens': 20481,
    'cache_write_tokens': 0,
    'cost': 0,
    'finish': 'length',
    'usage_observed': True,
}

TOOL_CALL = {
    'call_id': 'msg_1c365ae90001JM1HX2RZysxt2V',
    'model': 'opencode-go/deepseek-v4.1-flash',
    'input_tokens': 2021,
    'output_tokens': 1811,
    'reasoning_tokens': 22,
    'cache_read_tokens': 3840,
    'cache_write_tokens': 0,
    'cost': 0.00141447,
    'finish': 'tool-calls',
    'usage_observed': True,
}

UNOBSERVED_CALL = {
    'call_id': 'msg_1c365d69d0010t5ket6eRigorx',
    'model': 'opencode-go/deepseek-v4.1-flash',
    'input_tokens': None,
    'output_tokens': None,
    'reasoning_tokens': None,
    'cache_read_tokens': None,
    'cache_write_tokens': None,
    'cost': None,
    'finish': None,
    'usage_observed': False,
}


def _load_module(name):
    """Load one existing test module for its fake actors and helpers."""
    path = ROOT / 'tests' / f'{name}.py'
    spec = importlib.util.spec_from_file_location(f'_cmre_{name}', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def _require(name, receipt, model=MODEL):
    """Run the stage completion policy and capture its message and policy."""
    actor = OpenCodeActor({}, Path('unused'), runner=lambda *args: None,
                          collector=lambda *args: None)
    captured = {}

    def fail(message, *, retryable=False):
        captured['message'] = message
        captured['retryable'] = retryable
        raise OpenCodeError(message, retryable=retryable)

    try:
        actor._require_completed(name, receipt, model, fail)
    except OpenCodeError:
        pass
    return captured


def _call_runner(base, calls, **kwargs):
    """A fake runner whose work stage records the supplied call accounting."""
    class _Runner(base):
        def _stage(self, spec, directory, phase):
            receipt = super()._stage(spec, directory, phase)
            if phase == 'work':
                receipt['calls'] = calls
                (directory / 'receipt.json').write_text(
                    json.dumps(receipt, indent=2), encoding='utf-8')
            return receipt

    return _Runner(**kwargs)


def test_length_finish_reports_the_observed_zero_reasoning_count():
    result = _require('work', {'state': 'incomplete', 'calls': [LENGTH_CALL]})
    message = result['message']
    assert message.startswith('work stage did not complete: incomplete')
    assert 'state=incomplete' in message
    assert 'finish=length' in message
    assert 'output=16384' in message
    assert 'reasoning=0' in message
    assert 'usage=observed' in message
    assert 'unknown' not in message


def test_unobserved_usage_stays_unknown_and_never_invents_zero():
    call = dict(LENGTH_CALL, output_tokens=None, reasoning_tokens=None,
                usage_observed=False)
    result = _require('work', {'state': 'incomplete', 'calls': [call]})
    message = result['message']
    assert message.startswith('work stage did not complete: incomplete')
    assert 'usage=unknown' in message
    assert 'output=unknown' in message
    assert 'reasoning=unknown' in message
    assert 'output=0' not in message
    assert 'reasoning=0' not in message


def test_timeout_state_stays_timeout_when_the_last_call_has_no_usage():
    result = _require('work', {'state': 'timeout',
                               'calls': [TOOL_CALL, UNOBSERVED_CALL]})
    message = result['message']
    assert message.startswith('work stage did not complete: timeout')
    assert 'state=timeout' in message
    assert 'finish=unknown' in message
    assert 'output=unknown' in message
    assert 'reasoning=unknown' in message
    assert 'usage=unknown' in message
    assert 'length' not in message
    assert '1811' not in message


def test_the_last_relevant_call_supplies_the_diagnostic():
    result = _require('work', {'state': 'incomplete',
                               'calls': [LENGTH_CALL, TOOL_CALL]})
    message = result['message']
    assert 'finish=tool-calls' in message
    assert 'output=1811' in message
    assert 'reasoning=22' in message
    assert 'usage=observed' in message
    assert 'length' not in message


@pytest.mark.parametrize('calls', [
    'not-a-list',
    {},
    None,
    ['text', 3],
    [{'finish': 42, 'output_tokens': '12', 'reasoning_tokens': 1.5,
      'usage_observed': 'yes'}],
])
def test_malformed_call_containers_and_fields_report_unknown(calls):
    result = _require('work', {'state': 'incomplete', 'calls': calls})
    message = result['message']
    assert message.startswith('work stage did not complete: incomplete')
    assert 'finish=unknown' in message
    assert 'output=unknown' in message
    assert 'reasoning=unknown' in message
    assert 'usage=unknown' in message


def test_malformed_counts_are_unknown_but_a_known_finish_is_still_named():
    result = _require('work', {'state': 'incomplete', 'calls': [
        {'finish': 'length', 'output_tokens': True, 'reasoning_tokens': -1,
         'usage_observed': True}]})
    message = result['message']
    assert 'finish=length' in message
    assert 'output=unknown' in message
    assert 'reasoning=unknown' in message
    assert 'usage=observed' in message
    assert 'output=1' not in message
    assert 'reasoning=-1' not in message


def test_secret_bearing_call_fields_never_leak_into_the_reason():
    call = dict(LENGTH_CALL)
    call.update({'api_key': SECRET, 'authorization': f'Bearer {SECRET}',
                 'provider_errors': [f'boom {SECRET}'],
                 'environment': {'OPENCODE_API_KEY': SECRET},
                 'finish': SECRET})
    result = _require('work', {'state': 'incomplete', 'calls': [call]})
    message = result['message']
    assert SECRET not in message
    assert 'finish=unknown' in message
    assert 'api_key' not in message
    assert 'provider_errors' not in message
    assert 'environment' not in message


def test_completed_stage_keeps_its_message_and_policy():
    result = _require('work', {'state': 'completed', 'finished': True,
                               'exit_code': 0, 'calls': [LENGTH_CALL],
                               'models_observed': [MODEL]})
    assert result == {}


def test_scope_model_and_exit_checks_keep_their_own_messages():
    scope = _require('work', {'state': 'completed', 'out_of_scope': ['secret.c'],
                              'models_observed': [MODEL]})
    assert scope['message'] == 'work stage changed files outside the scope'
    assert 'diagnostics' not in scope['message']
    assert scope['retryable'] is False

    drift = _require('work', {'state': 'completed', 'pin_drift': ['a.txt'],
                              'models_observed': [MODEL]})
    assert drift['message'] == 'work stage drifted from its pinned inputs'
    assert 'diagnostics' not in drift['message']

    model = _require('work', {'state': 'completed', 'models_observed': ['other/model']})
    assert model['message'] == 'work stage observed another model: [\'other/model\']'
    assert 'diagnostics' not in model['message']


def test_the_state_check_keeps_its_order_and_the_retryable_policy():
    incomplete = _require('work', {'state': 'incomplete', 'calls': [LENGTH_CALL]})
    assert incomplete['retryable'] is False
    timeout = _require('work', {'state': 'timeout', 'calls': [UNOBSERVED_CALL]})
    assert timeout['retryable'] is True
    unknown_state = _require('work', {'state': 'cancelled'})
    assert unknown_state['retryable'] is False
    dirty = _require('work', {'state': 'incomplete', 'out_of_scope': ['a.c'],
                              'calls': [LENGTH_CALL]})
    assert dirty['message'].startswith('work stage did not complete: incomplete')
    assert dirty['retryable'] is False


def test_the_actor_failure_message_carries_the_diagnostic(tmp_path):
    adapter_tests = _load_module('test_provider_opencode')
    runner = _call_runner(adapter_tests.FakeRunner, [LENGTH_CALL],
                          work_state='incomplete')
    actor = adapter_tests._actor(tmp_path, runner=runner,
                                 collector=adapter_tests.FakeCollector())
    with pytest.raises(OpenCodeError) as caught:
        adapter_tests._call(actor, adapter_tests._job(tmp_path))
    message = str(caught.value)
    assert message.startswith('work stage did not complete: incomplete')
    assert 'finish=length' in message
    assert 'output=16384' in message
    assert 'reasoning=0' in message
    assert caught.value.retryable is False


def test_the_failure_reason_reaches_the_core_retry_context(tmp_path):
    adapter_tests = _load_module('test_provider_opencode')
    campaign_tests = _load_module('test_provider_campaign')
    runner = _call_runner(adapter_tests.FakeRunner, [TOOL_CALL],
                          work_state='incomplete')
    actor = adapter_tests._actor(tmp_path, runner=runner,
                                 collector=adapter_tests.FakeCollector())
    job = adapter_tests._job(tmp_path, mode='input')
    seen = []

    def campaign_actor(job_view, role, context, stage_path):
        seen.append(context.get('retry'))
        if len(seen) == 1:
            actor(job_view, role, context, stage_path)
        return {'action': {'kind': 'hold', 'reason': 'diagnostic observed'},
                'model': campaign_tests.MODELS[role]}

    backend = campaign_tests.Backend([], [])
    directory = campaign_tests.init_campaign(
        tmp_path, campaign_tests.make_manifest(duration=600))
    pc.run(directory, actor=campaign_actor, execute=backend.execute)

    job_state = campaign_tests.job_state(directory)
    assert job_state['provider_retries'] == 1
    assert job_state['state'] == 'held'
    assert seen[0] is None
    retry = seen[1]
    assert retry is not None
    assert retry['action'] == 'a0001'
    assert 'work stage did not complete: incomplete' in retry['reason']
    assert 'finish=tool-calls' in retry['reason']
    assert 'output=1811' in retry['reason']
    assert 'reasoning=22' in retry['reason']
    assert len(retry['reason']) <= min(pc.MAX_ERROR, 600)


def test_length_failure_holds_without_dispatching_an_identical_retry(tmp_path):
    adapter_tests = _load_module('test_provider_opencode')
    campaign_tests = _load_module('test_provider_campaign')
    runner = _call_runner(adapter_tests.FakeRunner, [LENGTH_CALL], work_state='incomplete')
    actor = adapter_tests._actor(tmp_path, runner=runner, collector=adapter_tests.FakeCollector())
    calls = []
    def campaign_actor(job, role, context, stage):
        calls.append(context.get('retry'))
        return actor(job, role, context, stage)
    backend = campaign_tests.Backend([], [])
    directory = campaign_tests.init_campaign(tmp_path, campaign_tests.make_manifest(duration=600))
    pc.run(directory, actor=campaign_actor, execute=backend.execute)
    state = campaign_tests.job_state(directory)
    assert calls == [None] and state.get('provider_retries', 0) == 0
    assert state['state'] == 'held' and len(state['results']) == 1
    assert 'finish=length' in state['last_failure']['error']
