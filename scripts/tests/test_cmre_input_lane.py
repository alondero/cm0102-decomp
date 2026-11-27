import hashlib
import json
import sys
import time
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import cmre_input_lane as lane
import cmre_input_probe as probe


# --- spec expansion (cmre_input_probe) -------------------------------------------

ARENA = 0x03000000
REGS = ('eax', 'ecx', 'edx', 'ebx', 'esp', 'ebp', 'esi', 'edi', 'eflags')


def templates(size=0x100):
    return [bytes(range(256)) * (size // 256), bytes([0xAA]) * size]


def expand(spec, arg_count=4):
    return probe.expand(spec, arg_count, templates(), REGS, ARENA)


def test_a_compact_fixture_becomes_the_canonical_fixture_schema():
    [(name, fixture)] = expand({'fixtures': [{
        'name': 'recv', 'regs': {'ecx': 'A+0x10'}, 'stack': ['A+0x20', '0x5'], 'template': 1,
        'zero': [['0x40', '0x8']], 'dwords': {'0x10': 7}, 'ascii': {'0x22': 'Ro'},
        'globals': {'0x00ae23b4': 'A+0x80'}, 'import_seed': 3}]})
    assert name == 'recv'
    assert fixture['regs'] == {**dict.fromkeys(REGS, 0), 'ecx': ARENA + 0x10, 'eflags': 0x202}
    assert fixture['stack'] == [ARENA + 0x20, 5, 0, 0]
    assert fixture['arena_template'] == 1 and fixture['import_seed'] == 3
    assert fixture['globals'] == {'ae23b4': ARENA + 0x80}
    # Only dwords that differ from the chosen template are written; byte writes keep the
    # template's neighbouring bytes.
    assert fixture['arena_writes'] == {'10': 7, '20': 0x6F52AAAA, '24': 0xAAAAAA00, '40': 0, '44': 0}


def test_vary_expands_the_cartesian_product_and_names_each_variant():
    out = expand({'base': {'stack': [1]}, 'fixtures': [{'name': 'sel', 'vary': [
        {'field': 'stack.1', 'values': [0, 1]}, {'field': 'dwords.0x4', 'range': [5, 8]}]}]})
    assert [name for name, _ in out] == [f'sel#{n}' for n in range(6)]
    assert {(f['stack'][0], f['stack'][1], f['arena_writes']['4']) for _, f in out} == {
        (1, s, v) for s in (0, 1) for v in (5, 6, 7)}


@pytest.mark.parametrize('spec, message', [
    ({'fixtures': [{'stack': [0] * 5}]}, 'arg_count'),
    ({'fixtures': [{'policy': 1}]}, 'unknown fixture fields'),
    ({'fixtures': [], 'min_coverage': 0.1}, 'spec must be an object'),
    ({'fixtures': [{'vary': [{'field': 'stack.0', 'range': [0, 3000]}]}]}, 'more than'),
    ({'fixtures': [{'dwords': {'0x200': 1}}]}, 'out of range'),
])
def test_specs_cannot_change_policy_or_escape_their_limits(spec, message):
    with pytest.raises(ValueError, match=message):
        expand(spec)


def test_selection_keeps_fixtures_that_reach_new_code_plus_one_return():
    rows = [('a', {'a': 1}, {1, 2}, 'fault'), ('b', {'b': 1}, {2}, 'return'),
            ('c', {'c': 1}, {3, 4, 5}, 'fault'), ('d', {'d': 1}, {9}, 'fault')]
    assert probe.select(rows, baseline={9}) == [0, 1, 2]
    assert probe.select(rows, baseline={1, 2, 3, 4, 5, 9}) == []


def test_proposals_only_append_fixtures_to_the_current_hints():
    current = {'auto': True, 'arg_count': 4, 'fixtures': [{'old': 1}], 'captures': ['ab' * 32]}
    proposal = probe.merge_hints(current, [{'old': 1}, {'new': 1}], {'id': 'input-lane-x', 'kind': 'k'})
    assert proposal['fixtures'] == [{'old': 1}, {'new': 1}] and 'auto' not in proposal
    assert proposal['fixture_provenance'] == {'input-lane-x': {'kind': 'k'}}
    assert probe.append_only(current, proposal)
    assert not probe.append_only(current, {**proposal, 'fixtures': [{'new': 1}]})
    assert not probe.append_only(current, {**proposal, 'min_coverage': 0.1})
    assert not probe.append_only(current, {**proposal, 'captures': []})


# --- the lane --------------------------------------------------------------------

class Ops:
    def __init__(self, candidates, gate=None, screen=None, apply='accepted'):
        self._candidates = candidates
        self.gate_result = gate or {}
        self.screen_result = screen or {}
        self.apply_result = apply
        self.gates, self.applies, self.accepts, self.screens = [], [], [], []
        self.harness, self.candidate_calls = 'H1', 0

    def harness_digest(self):
        return self.harness

    def candidates(self):
        self.candidate_calls += 1
        return json.loads(json.dumps(self._candidates))

    def screen(self, fn):
        self.screens.append(fn)
        return self.screen_result.get(fn, {'status': 'low-coverage', 'coverage': 0.3})

    def accept(self, fn):
        self.accepts.append(fn)
        return 'accepted'

    def gate(self, fn, spec, model):
        self.gates.append((fn, model))
        return {'proposal': str(spec.with_suffix('.p.json')), 'prior_sha256': 'h' * 64, 'tool': probe.TOOL_VERSION,
                **self.gate_result.get(fn, {'status': 'low-coverage', 'coverage': 0.4})}

    def apply(self, fn, expected, proposal, proposal_sha256=None):
        self.applies.append((fn, expected))
        return self.apply_result


class Handle:
    def __init__(self, log, code=0, limit=None):
        self.log, self.code, self.limit, self.killed = log, code, limit, False

    def poll(self):
        return self.code

    def kill(self):
        self.killed = True

    def limit_error(self):
        return self.limit


class Launcher:
    def __init__(self, tmp, text='FARM-RESULT: pass', limit=None):
        self.tmp, self.text, self.limit, self.started = Path(tmp), text, limit, []

    def start(self, job, prompt, agent):
        self.started.append((job, prompt, agent))
        log = self.tmp / f"{job['fn']}-{job['n']}.jsonl"
        log.write_text(self.text)
        return Handle(log, limit=self.limit)


def candidate(size, coverage=0.5, key='k1', status=None):
    return {'bytes': size, 'coverage': coverage, 'key': key, 'status': status, 'threshold': 0.6,
            'hints_sha256': 'h' * 64}


def make_lane(tmp_path, candidates, **kw):
    ops = Ops(candidates, **{k: v for k, v in kw.items() if k in ('gate', 'screen', 'apply')})
    launcher = Launcher(tmp_path, **{k: v for k, v in kw.items() if k in ('text', 'limit')})
    clock = {'now': 1000.0}
    run = tmp_path / 'run'
    run.mkdir()
    lane_ = lane.Lane(run, ops, launcher, workers=kw.get('workers', 2), ladder=['p/cheap', 'q/strong'],
                      clock=lambda: clock['now'], root=tmp_path)
    return lane_, ops, launcher, clock


def test_plan_orders_near_threshold_then_larger_and_skips_unhelpable_functions():
    candidates = {'fn_a': candidate(100, 0.55), 'fn_b': candidate(5000, 0.21), 'fn_c': candidate(900, 0.58),
                  'fn_d': candidate(9000, None), 'fn_e': candidate(800, 0.5, status='lift-blocked')}
    rows = {'fn_c': {'key': 'k1', 'attempts': [{}, {}, {}]}}
    assert lane.plan(candidates, rows) == ['fn_a', 'fn_b', 'fn_d']
    rows['fn_c']['key'] = 'old'                       # hints changed: the function reopens
    assert lane.plan(candidates, rows)[0] == 'fn_c'


def test_a_worker_pass_is_regated_by_the_lane_and_applied_under_the_lease(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)},
                                        gate={'fn_00401000': {'status': 'pass', 'coverage': 0.71}})
    l.tick()
    assert len(launcher.started) == 1
    job, prompt, agent = launcher.started[0]
    assert job['model'] == 'p/cheap' and Path(job['spec']).exists()
    assert 'cmre_input_probe.py gate fn_00401000' in prompt
    assert 'edit", resource: "run/input-lane/specs/fn_00401000-1.json", effect: allow' in agent
    assert 'shell", resource: "py -3.13 -m cmre accept' not in agent
    l.tick()
    assert ops.gates == [('fn_00401000', 'p/cheap')]
    assert ops.applies == [('fn_00401000', 'h' * 64)]
    row = l.state['functions']['fn_00401000']
    assert row['status'] == 'accepted' and row['attempts'][0]['claimed'] == 'pass'
    assert l.summary()['accepted_bytes'] == 700


def test_a_worker_claim_is_not_evidence(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, text='FARM-RESULT: pass',
                                        gate={'fn_00401000': {'status': 'low-coverage', 'coverage': 0.52}})
    l.tick()
    l.tick()
    assert ops.applies == []
    row = l.state['functions']['fn_00401000']
    assert row['best']['coverage'] == 0.52 and 'status' not in row


@pytest.mark.parametrize(('reported', 'verified'), [('pass', 'pass'), ('pass', 'low-coverage'), ('fail', 'fail')])
def test_a_terminal_probe_result_gates_the_exact_spec_without_waiting_for_worker_exit(tmp_path, reported, verified):
    fn = 'fn_00401000'
    l, ops, _, _ = make_lane(tmp_path, {fn: candidate(700)}, workers=1,
                              gate={fn: {'status': verified, 'coverage': 0.71}})
    l.tick()
    job, handle, _ = l.running[0]
    handle.code = None
    spec = Path(job['spec'])
    data = b'{"fixtures":[{"name":"checked"}]}'
    spec.write_bytes(data)
    spec.with_suffix('.probe-result.json').write_text(json.dumps({
        'fn': fn, 'status': reported, 'spec_sha256': hashlib.sha256(data).hexdigest(),
        'prior_sha256': 'h' * 64,
    }))
    gate = ops.gate

    def check_snapshot(fn, path, model):
        spec.write_bytes(b'{"fixtures":[{"name":"different"}]}')
        assert path != spec and path.read_bytes() == data
        return gate(fn, path, model)

    ops.gate = check_snapshot
    l.tick()
    assert handle.killed and ops.gates == [(fn, 'p/cheap')]
    assert bool(ops.applies) == (verified == 'pass')
    assert l.state['functions'][fn]['attempts'][0]['status'] == verified


@pytest.mark.parametrize('stale', ['spec_sha256', 'prior_sha256'])
def test_a_terminal_probe_from_different_inputs_does_not_interrupt_the_worker(tmp_path, stale):
    fn = 'fn_00401000'
    l, ops, _, _ = make_lane(tmp_path, {fn: candidate(700)}, workers=1)
    l.tick()
    job, handle, _ = l.running[0]
    handle.code = None
    spec = Path(job['spec'])
    result = {'fn': fn, 'status': 'pass', 'spec_sha256': hashlib.sha256(spec.read_bytes()).hexdigest(),
              'prior_sha256': 'h' * 64}
    result[stale] = '0' * 64
    spec.with_suffix('.probe-result.json').write_text(json.dumps(result))
    l.tick()
    assert not handle.killed and not ops.gates


def test_early_probe_keeps_the_provider_cooldown_and_does_not_park_mismatch_on_drifted_hints(tmp_path):
    fn = 'fn_00401000'
    l, ops, _, _ = make_lane(tmp_path, {fn: candidate(700)}, workers=1, limit=('quota', 600),
                              gate={fn: {'status': 'fail', 'prior_sha256': 'x' * 64,
                                         'mismatches': [{'case': 3}]}})
    l.tick()
    job, handle, _ = l.running[0]
    handle.code = None
    spec = Path(job['spec'])
    spec.with_suffix('.probe-result.json').write_text(json.dumps({
        'fn': fn, 'status': 'fail', 'spec_sha256': hashlib.sha256(spec.read_bytes()).hexdigest(),
        'prior_sha256': 'h' * 64,
    }))
    l.tick()
    assert l.state['limited'].get('p') == pytest.approx(1600.0)
    row = l.state['functions'][fn]
    assert row['accept'] == 'hints-changed' and 'mismatches' not in row and row.get('status') != 'fail'
    assert not ops.applies


def test_worker_probe_publishes_terminal_result_bound_to_the_bytes_it_read(tmp_path, monkeypatch):
    fn = 'fn_00401000'
    spec = tmp_path / 'spec.json'
    data = b'{"fixtures":[]}'
    spec.write_bytes(data)
    monkeypatch.setattr(probe, 'gate_spec', lambda *args: {'fn': fn, 'status': 'pass', 'prior_sha256': None})
    assert probe.main(['gate', fn, str(spec)]) == 0
    assert json.loads(spec.with_suffix('.probe-result.json').read_text()) == {
        'fn': fn, 'status': 'pass', 'prior_sha256': None, 'spec_sha256': hashlib.sha256(data).hexdigest(),
    }


def test_worker_probe_still_prints_its_result_if_the_optional_receipt_cannot_be_written(tmp_path, monkeypatch, capsys):
    from cmre import coordination

    def refuse(*args):
        raise PermissionError('receipt not writable')

    fn = 'fn_00401000'
    spec = tmp_path / 'spec.json'
    spec.write_bytes(b'{"fixtures":[]}')
    result = {'fn': fn, 'status': 'fail', 'prior_sha256': None, 'mismatches': [{'case': 3}]}
    monkeypatch.setattr(probe, 'gate_spec', lambda *args: result)
    monkeypatch.setattr(coordination, 'atomic_bytes', refuse)
    assert probe.main(['gate', fn, str(spec)]) == 0
    assert json.loads(capsys.readouterr().out) == result


def test_failed_attempts_escalate_and_continue_from_the_best_spec(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1)
    l.tick()
    Path(launcher.started[0][0]['spec']).write_text('{"fixtures": [{"name": "kept"}]}')
    l.tick()            # collects attempt 1 (low coverage), dispatches attempt 2 on the next rung
    l.tick()
    models = [job['model'] for job, _, _ in launcher.started]
    assert models[:2] == ['p/cheap', 'q/strong']
    second = launcher.started[1]
    assert json.loads(Path(second[0]['spec']).read_text()) == {'fixtures': [{'name': 'kept'}]}
    assert 'previous attempt' in second[1]


def test_attempts_stop_at_the_cap_until_the_hints_change(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1)
    # The cap, not the plateau: each attempt improves on the best coverage so far.
    gate_sequence(ops, 'fn_00401000', [0.55, 0.56, 0.57, 0.58])
    for _ in range(10):
        l.tick()
        clock['now'] += 1
    assert len(launcher.started) == lane.MAX_ATTEMPTS
    ops._candidates['fn_00401000']['key'] = 'k2'
    clock['now'] += lane.CANDIDATE_REFRESH_SECONDS + 1
    l.tick()
    assert len(launcher.started) == lane.MAX_ATTEMPTS + 1


def test_a_never_gated_function_is_screened_and_a_free_pass_is_accepted(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(300, None),
                                                    'fn_00402000': candidate(200, None)},
                                        screen={'fn_00401000': {'status': 'pass', 'coverage': 0.9},
                                                'fn_00402000': {'status': 'lift-blocked'}})
    l.tick()
    assert ops.accepts == ['fn_00401000'] and launcher.started == []
    assert l.state['functions']['fn_00402000']['status'] == 'lift-blocked'


def test_a_lift_mismatch_is_recorded_not_retried(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1,
                                        gate={'fn_00401000': {'status': 'fail', 'coverage': 0.7,
                                                              'mismatches': [{'case': 3}]}})
    for _ in range(4):
        l.tick()
        clock['now'] += lane.CANDIDATE_REFRESH_SECONDS + 1
    assert len(launcher.started) == 1 and ops.applies == []
    assert l.state['functions']['fn_00401000']['mismatches'] == [{'case': 3}]


def test_a_usage_limit_pauses_the_provider_without_spending_an_attempt(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1,
                                        limit=('quota', 600))
    l.tick()
    l.tick()
    assert ops.gates == [] and l.state['functions']['fn_00401000'].get('attempts', []) == []
    assert l.state['limited']['p'] == pytest.approx(1600.0)
    assert [job['model'] for job, _, _ in launcher.started][-1] == 'q/strong'


GO_MONTHLY = ('OpenCode Go monthly usage limit reached. Switch to a free model, or turn on Extra Usage '
              'on the Go page in OpenCode Console to keep going.')


def test_a_paid_usage_limit_leaves_free_models_usable(tmp_path):
    l, _, _, _ = make_lane(tmp_path, {})
    l._pause('p/paid', GO_MONTHLY, 600)
    assert not l._available('p/paid')
    assert l._available('p/hy4-flash-free')
    l._pause('p/hy4-flash-free', 'Too Many Requests', 60)
    assert not l._available('p/hy4-flash-free')
    assert not l._available('p/paid')
    l._pause('q/paid', 'quota', 600)
    assert not l._available('q/other-free')


class WorkingLauncher(Launcher):
    """The worker writes a new spec, then its stream ends with a limit (job 31, 2026-11-21:
    5.5 minutes of work, a passing spec, then provider.no-route)."""

    def start(self, job, prompt, agent):
        handle = super().start(job, prompt, agent)
        Path(job['spec']).write_text('{"fixtures": [{"new": 1}]}\n')
        return handle


def test_a_limit_after_real_work_still_gates_the_written_spec(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1,
                                        gate={'fn_00401000': {'status': 'pass', 'coverage': 0.7}})
    l.launcher = WorkingLauncher(tmp_path, limit=('Model unavailable: p/cheap', 3600))
    l.tick()
    l.drain_gates()
    assert ops.gates == [('fn_00401000', 'p/cheap')]
    assert ops.applies and l.state['functions']['fn_00401000']['status'] == 'accepted'
    assert not l._available('p/cheap')


class AuthLauncher(Launcher):
    def start(self, job, prompt, agent):
        handle = super().start(job, prompt, agent)
        handle.infra_error = lambda: 'This API key has expired. Create a new API key in OpenCode Console'
        return handle


def test_an_expired_key_pauses_the_provider_and_spends_no_attempt(tmp_path):
    # 2026-11-21 23:04: jobs 39-50 died on "401 api_key_expired"; each spent an attempt.
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1)
    l.launcher = AuthLauncher(tmp_path)
    l.tick()
    l.tick()
    assert ops.gates == [] and l.state['functions']['fn_00401000'].get('attempts', []) == []
    assert not l._available('p/cheap') and not l._available('p/other')
    assert any(e['event'] == 'model-unavailable' for e in
               map(json.loads, (l.dir / 'events.jsonl').read_text().splitlines()))


def test_a_quota_limit_that_also_looks_like_infra_stays_a_timed_limit(tmp_path):
    # MiniMax "Token Plan usage limit" is type provider.quota: it matches both detectors.
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1)
    l.launcher = AuthLauncher(tmp_path, limit=('The Token Plan usage limit has been reached.', 900))
    l.tick()
    l.tick()
    assert l.state['limited'] == {'p': pytest.approx(1900.0)}
    names = [json.loads(x)['event'] for x in (l.dir / 'events.jsonl').read_text().splitlines()]
    assert 'model-limited' in names and 'model-unavailable' not in names


def test_an_expired_key_on_a_free_model_pauses_the_whole_provider(tmp_path):
    l, _, _, _ = make_lane(tmp_path, {})
    l.ladder = ['p/x-free']
    l._pause_provider('p/x-free', 'This API key has expired', 3600)
    assert not l._available('p/x-free') and not l._available('p/paid')


def test_the_farm_lane_summary_ends_a_job_on_model_unavailable(tmp_path):
    import cmre_farm
    l, _, _, _ = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1)
    l.launcher = AuthLauncher(tmp_path)
    l.tick()
    l.tick()
    # the key failure ended job 1; job 2 (the next rung) is the only one still running
    assert [job['model'] for job, _, _ in l.running] == ['q/strong']
    assert cmre_farm.input_lane_summary(l.run_dir)['running'] == 1


def test_handle_infra_error_reports_the_error_message(tmp_path):
    import cmre_farm
    log = tmp_path / 'w.jsonl'
    log.write_text(json.dumps({'type': 'error', 'error': {
        'type': 'provider.auth', 'status': 401,
        'message': 'This API key has expired. Create a new API key in OpenCode Console.'}}) + '\n')
    assert cmre_farm.OpencodeHandle(None, log).infra_error().startswith('This API key has expired')


def test_a_no_route_pauses_only_that_model_and_spends_no_attempt(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1,
                                        limit=('Model unavailable: p/cheap', 3600))
    l.tick()
    l.tick()
    assert ops.gates == [] and l.state['functions']['fn_00401000'].get('attempts', []) == []
    assert not l._available('p/cheap') and l._available('p/other')
    assert [job['model'] for job, _, _ in launcher.started][-1] == 'q/strong'


def test_a_busy_writer_leaves_the_pass_for_a_later_apply(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, apply='busy',
                                        gate={'fn_00401000': {'status': 'pass', 'coverage': 0.7}})
    l.tick()
    l.tick()
    assert l.state['functions']['fn_00401000']['pending_apply']
    ops.apply_result = 'accepted'
    l.tick()
    assert l.state['functions']['fn_00401000']['status'] == 'accepted'


def test_the_lane_stops_dispatching_on_the_farm_stop_file(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)})
    (l.run_dir / 'STOP').write_text('stop')
    l.tick()
    assert launcher.started == [] and l.finished()


def test_workers_follow_the_farm_control_file(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {f'fn_0040{i}000': candidate(700) for i in range(1, 6)})
    (l.run_dir / 'control.json').write_text(json.dumps({'input_lane_workers': 0}))
    l.tick()
    assert launcher.started == []
    (l.run_dir / 'control.json').write_text(json.dumps({'input_lane_workers': 3, 'ladder': ['r/x']}))
    l.tick()
    assert [job['model'] for job, _, _ in launcher.started] == ['r/x'] * 3


# --- canonical apply (real files, fake accept) -------------------------------------

def test_apply_restores_the_previous_hints_when_accept_is_refused(tmp_path, monkeypatch):
    from cmre import coordination, env
    hints = tmp_path / 'hints'
    hints.mkdir()
    monkeypatch.setattr(env, 'HINTS_DIR', hints)
    monkeypatch.setattr(coordination, '_path', lambda: tmp_path / 'command.lock')
    prior = json.dumps({'arg_count': 4}).encode()
    (hints / 'fn_00401000.json').write_bytes(prior)
    proposal = tmp_path / 'p.json'
    proposal.write_text(json.dumps({'arg_count': 4, 'fixtures': []}))
    import hashlib
    ops = lane.CmreOps(root=tmp_path)
    seen = []
    monkeypatch.setattr(ops, 'accept', lambda fn: seen.append((hints / f'{fn}.json').read_bytes()) or 'refused: x')
    assert ops.apply('fn_00401000', hashlib.sha256(prior).hexdigest(), proposal) == 'refused: x'
    assert seen == [proposal.read_bytes()]                   # accept saw the proposal as canonical
    assert (hints / 'fn_00401000.json').read_bytes() == prior   # and the refusal restored it
    assert ops.apply('fn_00401000', 'stale', proposal) == 'hints-changed'
    monkeypatch.setattr(ops, 'accept', lambda fn: 'accepted')
    proposal.write_text(json.dumps({'fixtures': []}))          # drops arg_count: not append-only
    assert ops.apply('fn_00401000', hashlib.sha256(prior).hexdigest(), proposal).startswith('refused')


def test_the_worker_claim_is_read_from_a_json_event_stream(tmp_path):
    stream = '{"type":"text","part":{"text":"done\nFARM-RESULT: pass"},"time":{"start":1}}\n'
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, text=stream)
    l.tick()
    l.tick()
    assert l.state['functions']['fn_00401000']['attempts'][0]['claimed'] == 'pass'


def test_a_retry_after_no_new_reach_is_told_what_the_last_attempt_found(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1,
                                        gate={'fn_00401000': {'status': 'no-new-reach', 'coverage': 0.5}})
    l.tick()
    l.tick()
    l.tick()
    second = launcher.started[1]
    assert 'no-new-reach' in second[1]


# --- review findings (2026-11-21) ---------------------------------------------------

def test_a_huge_vary_range_is_refused_before_it_is_built():
    with pytest.raises(ValueError, match='more than'):
        expand({'fixtures': [{'vary': [{'field': 'stack.0', 'range': [0, '0xffffffff']}]}]})


def test_an_absent_hint_file_is_part_of_the_stale_check(tmp_path, monkeypatch):
    """A worker that plants recovery/hints/fn_X.json (e.g. a lowered min_coverage) must not be applied."""
    from cmre import coordination, env
    hints = tmp_path / 'hints'
    hints.mkdir()
    monkeypatch.setattr(env, 'HINTS_DIR', hints)
    monkeypatch.setattr(coordination, '_path', lambda: tmp_path / 'command.lock')
    (hints / 'fn_00401000.json').write_text(json.dumps({'min_coverage': 0.05}))
    proposal = tmp_path / 'p.json'
    proposal.write_text(json.dumps({'min_coverage': 0.05, 'fixtures': []}))
    ops = lane.CmreOps(root=tmp_path)
    monkeypatch.setattr(ops, 'accept', lambda fn: 'accepted')
    assert ops.apply('fn_00401000', lane.ABSENT, proposal) == 'hints-changed'
    with pytest.raises(ValueError):
        ops.apply('fn_00401000', None, proposal)


def test_apply_restores_the_hints_when_accept_raises(tmp_path, monkeypatch):
    from cmre import coordination, env
    hints = tmp_path / 'hints'
    hints.mkdir()
    monkeypatch.setattr(env, 'HINTS_DIR', hints)
    monkeypatch.setattr(coordination, '_path', lambda: tmp_path / 'command.lock')
    proposal = tmp_path / 'p.json'
    proposal.write_text(json.dumps({'fixtures': []}))
    ops = lane.CmreOps(root=tmp_path)

    def boom(fn):
        raise KeyboardInterrupt
    monkeypatch.setattr(ops, 'accept', boom)
    with pytest.raises(KeyboardInterrupt):
        ops.apply('fn_00401000', lane.ABSENT, proposal)
    assert not (hints / 'fn_00401000.json').exists()


def test_apply_refuses_a_proposal_other_than_the_one_the_lane_gated(tmp_path, monkeypatch):
    from cmre import coordination, env
    hints = tmp_path / 'hints'
    hints.mkdir()
    monkeypatch.setattr(env, 'HINTS_DIR', hints)
    monkeypatch.setattr(coordination, '_path', lambda: tmp_path / 'command.lock')
    proposal = tmp_path / 'p.json'
    proposal.write_text(json.dumps({'fixtures': []}))
    ops = lane.CmreOps(root=tmp_path)
    monkeypatch.setattr(ops, 'accept', lambda fn: 'accepted')
    assert ops.apply('fn_00401000', lane.ABSENT, proposal, proposal_sha256='0' * 64).startswith('refused')


def test_a_screen_pass_whose_accept_is_refused_is_not_retried_every_refresh(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(300, None)},
                                        screen={'fn_00401000': {'status': 'pass', 'coverage': 0.9}})
    ops.accept = lambda fn: ops.accepts.append(fn) or 'refused: x'
    for _ in range(3):
        l.tick()
        clock['now'] += lane.CANDIDATE_REFRESH_SECONDS + 1
    assert ops.accepts == ['fn_00401000'] and ops.screens == ['fn_00401000']


def test_a_screened_coverage_is_kept_across_refreshes(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(300, None)}, workers=1)
    for _ in range(4):
        l.tick()
        clock['now'] += lane.CANDIDATE_REFRESH_SECONDS + 1
    assert ops.screens == ['fn_00401000']
    assert launcher.started[-1][0]['coverage'] == 0.3


def test_gates_run_beside_dispatch_not_inside_it(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700), 'fn_00402000': candidate(600)},
                                        workers=2)
    l.tick()
    assert len(launcher.started) == 2
    l.drain_gates()
    assert sorted(fn for fn, _ in ops.gates) == ['fn_00401000', 'fn_00402000']


def test_stop_interrupts_running_workers(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)})
    l.tick()
    handle = l.running[0][1]
    handle.code = None
    (l.run_dir / 'STOP').write_text('stop')
    l.tick()
    assert handle.killed and l.finished()


def test_a_pass_gated_on_hints_other_than_the_dispatched_ones_is_not_applied(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)},
                                        gate={'fn_00401000': {'status': 'pass', 'coverage': 0.8,
                                                              'prior_sha256': 'x' * 64}})
    l.tick()
    l.tick()
    assert ops.applies == [] and l.state['functions']['fn_00401000']['accept'] == 'hints-changed'


# --- re-review findings (2026-11-21) -----------------------------------------------

def test_child_processes_do_not_inherit_the_lanes_writer_lease(monkeypatch):
    monkeypatch.setenv('CMRE_WRITER_TOKEN', 't')
    monkeypatch.setenv('CMRE_WRITER_PATH', 'p')
    env = lane.child_env()
    assert 'CMRE_WRITER_TOKEN' not in env and 'CMRE_WRITER_PATH' not in env and 'PATH' in env


def test_one_screen_error_is_retried_but_a_repeated_one_is_skipped(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(300, None)}, workers=1,
                                        screen={'fn_00401000': {'status': 'error'}})
    l.tick()
    assert l.state['functions']['fn_00401000'].get('status') != 'error'
    clock['now'] += lane.CANDIDATE_REFRESH_SECONDS + 1
    l.tick()
    assert ops.screens == ['fn_00401000'] * 2 and l.state['functions']['fn_00401000']['status'] == 'error'


def test_a_crashing_tick_kills_its_workers(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)})
    l.tick()
    handle = l.running[0][1]
    handle.code = None
    l.tick = lambda: (_ for _ in ()).throw(RuntimeError('boom'))
    with pytest.raises(RuntimeError):
        l.run(poll=0, sleep=lambda s: None)
    assert handle.killed


@pytest.mark.parametrize('raw, expected', [(None, 0.6), (b'{}', 0.6), (b'{"min_coverage": 0.45}', 0.45),
                                           (b'{"min_coverage": "x"}', None), (b'[1]', None), (b'{', None)])
def test_the_scheduling_threshold_is_read_strictly(raw, expected):
    assert lane.threshold(raw) == expected


def test_worker_shell_cannot_redirect_or_chain_commands(tmp_path):
    job = {'fn': 'fn_00401000', 'spec': str(tmp_path / 'spec.json')}
    lines = lane.agent_text(tmp_path, job).splitlines()
    rules = [line for line in lines if 'action: "shell"' in line]
    last_allow = max(i for i, line in enumerate(rules) if 'effect: allow' in line)
    # A two-line command is two commands; the YAML double-quoted escape is a real newline.
    for pattern in ('*>*', '*<*', '*|*', '*;*', '*&*', '*`*', '*$(*', r'*\n*', r'*\r*'):
        denies = [i for i, line in enumerate(rules) if f'resource: "{pattern}"' in line and 'effect: deny' in line]
        assert denies and min(denies) > last_allow, pattern   # later rules win: denies come last


class WaitingHandle(Handle):
    """22 Nov: input-lane workers sat out a ~9 h free-tier 429 until their time limit."""

    def __init__(self, log):
        super().__init__(log, code=None, limit=('Rate limit exceeded', 32932))

    def pending_limit(self):
        return self.limit


def test_a_worker_waiting_out_a_limit_is_stopped_without_an_attempt(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1)
    handles = []

    def start(job, prompt, agent):
        launcher.started.append((job, prompt, agent))
        log = tmp_path / f"{job['fn']}-{job['n']}.jsonl"
        log.write_text('')
        handles.append(WaitingHandle(log))
        return handles[-1]
    launcher.start = start
    l.tick()
    l.tick()
    assert handles[0].killed and ops.gates == []
    assert l.state['functions']['fn_00401000'].get('attempts', []) == []
    assert l.state['limited']['p'] == pytest.approx(1000.0 + 32932)


# --- harness release re-opens exhausted functions ---------------------------------

def attempts_under(digest, count, status='no-new-reach'):
    return [{'n': i, 'model': 'p/cheap', 'status': status, 'coverage': 0.5, 'harness': digest} for i in range(count)]


def test_attempts_from_an_older_harness_do_not_count_toward_the_cap():
    candidates = {'fn_a': candidate(100, 0.55)}
    rows = {'fn_a': {'key': 'k1', 'attempts': attempts_under('old', 3)}}
    assert lane.plan(candidates, rows, harness='new') == ['fn_a']
    assert lane.plan(candidates, rows, harness='old') == []


def test_attempts_without_a_digest_are_an_older_harness():
    rows = {'fn_a': {'key': 'k1', 'attempts': [{}, {}, {}]}}
    assert lane.plan({'fn_a': candidate(100, 0.55)}, rows, harness='new') == ['fn_a']


def test_current_harness_attempts_still_count_and_mixed_attempts_count_only_current():
    candidates = {'fn_a': candidate(100, 0.55)}
    rows = {'fn_a': {'key': 'k1', 'attempts': attempts_under('new', 3)}}
    assert lane.plan(candidates, rows, harness='new') == []
    rows['fn_a']['attempts'] = attempts_under('old', 5) + attempts_under('new', 2)
    assert lane.plan(candidates, rows, harness='new') == ['fn_a']
    rows['fn_a']['attempts'] = attempts_under('old', 5) + attempts_under('new', 3)
    assert lane.plan(candidates, rows, harness='new') == []


def test_terminal_rows_from_an_older_harness_reopen_but_accepted_rows_stay_done():
    candidates = {name: candidate(100, 0.55) for name in ('fn_f', 'fn_e', 'fn_acc', 'fn_cur')}
    rows = {'fn_f': {'key': 'k1', 'status': 'fail'},
            'fn_e': {'key': 'k1', 'status': 'error', 'harness': 'old'},
            'fn_acc': {'key': 'k1', 'status': 'accepted', 'harness': 'old'},
            'fn_cur': {'key': 'k1', 'status': 'fail', 'harness': 'new'}}
    assert sorted(lane.plan(candidates, rows, harness='new')) == ['fn_e', 'fn_f']


def test_pass_unaccepted_is_key_aware_and_same_key_rows_are_harness_independent():
    candidates = {'fn_same': candidate(100, 0.55), 'fn_changed': candidate(100, 0.55, key='new')}
    rows = {'fn_same': {'key': 'k1', 'status': 'pass-unaccepted'},
            'fn_changed': {'key': 'old', 'status': 'pass-unaccepted'}}
    assert lane.plan(candidates, rows, harness='H1') == ['fn_changed']
    assert lane.plan(candidates, rows, harness='H2') == ['fn_changed']


def test_a_legacy_plateau_without_a_harness_digest_does_not_suppress_forever():
    candidates = {'fn_a': candidate(100, 0.5)}
    rows = {'fn_a': {'key': 'k1', 'attempts': attempts_under('H1', 2),
                     'plateau': {'coverage': 0.5, 'obligation': 'input', 'at': 1}}}
    assert lane.plan(candidates, rows, harness='H1') == ['fn_a']


def test_best_spec_coverage_sets_the_priority_band_but_not_the_baseline():
    candidates = {'fn_big': candidate(22766, 0.1021, key='k1'),
                  'fn_small': candidate(271, 0.3977, key='k1')}
    rows = {'fn_big': {'key': 'k1', 'best': {'coverage': 0.5923}}}
    assert lane.plan(candidates, rows, harness='H') == ['fn_big', 'fn_small']


def test_best_spec_coverage_from_other_hint_bytes_does_not_set_priority():
    candidates = {'fn_big': candidate(22766, 0.1021, key='new'),
                  'fn_small': candidate(271, 0.3977, key='new')}
    rows = {'fn_big': {'key': 'old', 'best': {'coverage': 0.5923}}}
    assert lane.plan(candidates, rows, harness='H') == ['fn_small', 'fn_big']


def test_best_spec_priority_does_not_change_dispatch_baseline_or_prompt_coverage(tmp_path):
    big, small = 'fn_00401000', 'fn_00402000'
    candidates = {big: candidate(22766, 0.1021), small: candidate(271, 0.3977)}
    l, _, launcher, _ = make_lane(tmp_path, candidates, workers=1)
    best = tmp_path / 'best.json'
    best.write_text('{"fixtures": [{"name": "kept"}]}')
    l.state['functions'][big] = {'key': 'k1', 'best': {'spec': str(best), 'coverage': 0.5923,
                                                       'model': 'p/cheap', 'detail': 'gate low-coverage'}}
    l.tick()
    assert launcher.started[0][0]['fn'] == big   # the ordering heuristic did fire
    job, prompt, _ = launcher.started[0]
    assert job['coverage'] == 0.1021
    assert l.state['functions'][big]['baseline'] == 0.1021
    assert 'Current coverage: 0.1021.' in prompt
    assert 'Current coverage: 0.5923' not in prompt
    assert 'reached coverage 0.5923' in prompt   # the prior-attempt note may still mention it
    assert json.loads(Path(job['spec']).read_text()) == {'fixtures': [{'name': 'kept'}]}


def test_ladder_position_comes_from_current_harness_attempts_only(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1)
    l.state['functions']['fn_00401000'] = {'key': 'k1', 'attempts': attempts_under('old', 3)}
    l.tick()
    assert launcher.started[0][0]['model'] == 'p/cheap'
    l.tick()
    l.tick()
    assert [job['model'] for job, _, _ in launcher.started][:2] == ['p/cheap', 'q/strong']
    row = l.state['functions']['fn_00401000']
    assert [a.get('harness') for a in row['attempts']][3:] == ['H1', 'H1']


def test_new_attempts_still_start_from_the_best_spec_after_a_harness_change(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1)
    best = tmp_path / 'best.json'
    best.write_text('{"fixtures": [{"name": "kept"}]}')
    l.state['functions']['fn_00401000'] = {'key': 'k1', 'attempts': attempts_under('old', 3),
                                           'best': {'spec': str(best), 'coverage': 0.55, 'model': 'p/cheap'}}
    l.tick()
    assert json.loads(Path(launcher.started[0][0]['spec']).read_text()) == {'fixtures': [{'name': 'kept'}]}


def test_a_lane_stops_at_the_cap_per_harness_and_reopens_when_the_harness_changes(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1)
    # The cap, not the plateau: coverage keeps improving across all six attempts.
    gate_sequence(ops, 'fn_00401000', [0.51, 0.52, 0.53, 0.54, 0.55, 0.56])
    for _ in range(10):
        l.tick()
        clock['now'] += 1
    assert len(launcher.started) == lane.MAX_ATTEMPTS
    ops.harness = 'H2'
    clock['now'] += lane.CANDIDATE_REFRESH_SECONDS + 1
    for _ in range(10):
        l.tick()
        clock['now'] += 1
    assert len(launcher.started) == 2 * lane.MAX_ATTEMPTS


def test_an_old_harness_fail_is_rescreened_once_then_stays_done(tmp_path):
    l, ops, launcher, clock = make_lane(
        tmp_path, {'fn_00401000': candidate(700, 0.5)}, workers=1,
        screen={'fn_00401000': {'status': 'fail'}})
    l.state['functions']['fn_00401000'] = {'key': 'k1', 'status': 'fail', 'coverage': 0.5,
                                           'mismatches': [{'case': 1}]}
    for _ in range(4):
        l.tick()
        clock['now'] += lane.CANDIDATE_REFRESH_SECONDS + 1
    row = l.state['functions']['fn_00401000']
    assert ops.screens == ['fn_00401000'] and launcher.started == []
    assert row['status'] == 'fail' and row['harness'] == 'H1' and 'mismatches' not in row


def test_an_old_harness_fail_that_now_reaches_low_coverage_gets_attempts(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700, 0.5)}, workers=1)
    l.state['functions']['fn_00401000'] = {'key': 'k1', 'status': 'fail', 'coverage': 0.5}
    l.tick()
    assert ops.screens == ['fn_00401000'] and len(launcher.started) == 1
    assert 'status' not in l.state['functions']['fn_00401000']


def test_an_accepted_row_is_not_rescreened_after_a_harness_change(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700, 0.5)}, workers=1)
    l.state['functions']['fn_00401000'] = {'key': 'k1', 'status': 'accepted', 'harness': 'old'}
    l.tick()
    assert ops.screens == [] and launcher.started == []


def test_harness_digest_is_read_once_per_refresh_not_per_function(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {f'fn_0040{n}000': candidate(100) for n in range(1, 6)})
    calls = []
    ops.harness_digest = lambda: calls.append(1) or 'H1'
    l.refresh(force=True)
    assert len(calls) == 1


# --- idle refresh rate --------------------------------------------------------------

def test_an_idle_lane_rescans_at_most_once_per_idle_period(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {}, workers=1)
    for _ in range(30):                   # 30 polls, 10 s apart
        l.tick()
        clock['now'] += 10
    assert ops.candidate_calls == 5       # t=0,60,120,180,240


def test_an_idle_lane_rescans_immediately_after_a_gate_result(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1)
    for _ in range(10):
        l.tick()
        clock['now'] += 1
    before = ops.candidate_calls
    l.finish({'fn': 'fn_00401000', 'n': 99, 'model': 'p/cheap', 'key': 'k1', 'hints_sha256': 'h' * 64,
              'harness': 'H1'}, 'x', False, {'status': 'low-coverage', 'coverage': 0.4, 'prior_sha256': 'h' * 64})
    l.tick()
    assert ops.candidate_calls == before + 1


def test_a_busy_lane_still_refreshes_on_the_long_period(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {'fn_00401000': candidate(700)}, workers=1)
    launcher.start = lambda job, prompt, agent: Handle(tmp_path / 'log.jsonl', code=None)
    l.tick()
    assert l.running and ops.candidate_calls == 1
    for _ in range(5):
        clock['now'] += 100
        l.tick()
        assert l.running
    assert ops.candidate_calls == 1       # 500 s: busy lane waits for the 600 s period
    clock['now'] += 101
    l.tick()
    assert ops.candidate_calls == 2


# -- mixed model rungs

def test_a_mixed_rung_spreads_jobs_by_running_count_and_skips_limited_members(tmp_path):
    l, _, _, clock = make_lane(tmp_path, {})
    l.ladder = ['p/a|q/b', 'r/strong']
    assert l._model(0) == 'p/a'
    l.running.append(({'model': 'p/a'}, None, 0))
    assert l._model(0) == 'q/b'
    l.running.append(({'model': 'q/b'}, None, 0))
    l.running.append(({'model': 'q/b'}, None, 0))
    assert l._model(0) == 'p/a'
    l.state['limited']['p'] = clock['now'] + 600
    assert l._model(0) == 'q/b'
    l.state['limited']['q'] = clock['now'] + 600
    assert l._model(0) == 'r/strong'      # the whole rung is down: the next rung
    assert l._model(1) == 'r/strong'
    l.state['limited']['r'] = clock['now'] + 600
    assert l._model(0) is None


def test_the_lane_reads_grouped_ladders_from_control_and_dispatches_concrete_models(tmp_path):
    l, _, launcher, _ = make_lane(tmp_path, {'fn_00401000': candidate(700), 'fn_00401100': candidate(600)})
    (l.run_dir / 'control.json').write_text(json.dumps({'ladder': ['p/a|q/b', 'r/strong']}))
    l._control()
    assert l.ladder == ['p/a|q/b', 'r/strong']
    l.tick()
    assert sorted(job['model'] for job, _, _ in launcher.started) == ['p/a', 'q/b']


# --- provider-limited head candidates -------------------------------------------------

def test_a_limited_high_rung_head_does_not_block_a_later_dispatchable_candidate(tmp_path):
    head, later = 'fn_00401000', 'fn_00402000'
    l, _, launcher, _ = make_lane(tmp_path, {head: candidate(1000, 0.58), later: candidate(900, 0.57)},
                                  workers=1)
    l.state['functions'][head] = {'key': 'k1', 'attempts': attempts_under('H1', 2)}
    l._pause('q/strong', 'quota', 600)
    l.tick()
    assert [(job['fn'], job['model']) for job, _, _ in launcher.started] == [(later, 'p/cheap')]
    assert len(l.state['functions'][head]['attempts']) == 2


def test_a_deferred_head_keeps_priority_when_its_provider_cools_down(tmp_path):
    head, later, queued = 'fn_00401000', 'fn_00402000', 'fn_00403000'
    l, _, launcher, clock = make_lane(tmp_path, {head: candidate(1000, 0.58), later: candidate(900, 0.57),
                                                 queued: candidate(800, 0.56)}, workers=1)
    l.state['functions'][head] = {'key': 'k1', 'attempts': attempts_under('H1', 2)}
    l._pause('q/strong', 'quota', 60)
    l.tick()
    clock['now'] += 61
    l.tick()
    assert [(job['fn'], job['model']) for job, _, _ in launcher.started] == [
        (later, 'p/cheap'), (head, 'q/strong')]


# --- input plateaus (2026-11-27) -----------------------------------------------------
# Run 20261115T1250Z spent repeated model attempts on functions whose coverage never
# moved (200 rows with no-new-reach attempts). Two consecutive no-progress attempts at
# the same hint key and harness plateau the function: dispatch stops until the inputs
# or the harness change, and the row records the precise obligation.

def gate_sequence(ops, fn, coverages, status='low-coverage'):
    results = iter(coverages)

    def gate(fn_, spec, model):
        coverage = next(results, coverages[-1])
        ops.gates.append((fn_, model))
        return {'status': status, 'coverage': coverage, 'prior_sha256': 'h' * 64,
                'first_uncovered': ['0x00401aa0'], 'tool': probe.TOOL_VERSION}
    ops.gate = gate


def test_two_attempts_at_the_starting_coverage_plateau_before_the_attempt_cap(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, clock = make_lane(tmp_path, {fn: candidate(700, 0.5)}, workers=1)
    gate_sequence(ops, fn, [0.5, 0.5, 0.5])
    for _ in range(8):
        l.tick()
        clock['now'] += 1
    assert len(launcher.started) == 2   # not MAX_ATTEMPTS (3): the plateau stops the third
    row = l.state['functions'][fn]
    assert row['plateau']['coverage'] == 0.5 and row['plateau']['harness'] == 'H1'
    assert row['plateau']['obligation'] == 'input'
    assert row['plateau']['first_uncovered'] == ['0x00401aa0']
    names = [json.loads(x)['event'] for x in (l.dir / 'events.jsonl').read_text().splitlines()]
    assert names.count('input-plateau') == 1


def test_a_progress_attempt_resets_the_no_progress_count(tmp_path, monkeypatch):
    monkeypatch.setattr(lane, 'MAX_ATTEMPTS', 6)
    fn = 'fn_00401000'
    l, ops, launcher, clock = make_lane(tmp_path, {fn: candidate(700, 0.5)}, workers=1)
    gate_sequence(ops, fn, [0.55, 0.55, 0.6, 0.6, 0.6])
    for _ in range(10):
        l.tick()
        clock['now'] += 1
    # 0.55 over the 0.5 baseline and 0.6 over 0.55 are progress: the flat attempts never
    # reach two in a row before the last one, so the plateau lands only after the fifth.
    assert len(launcher.started) == 5
    assert l.state['functions'][fn]['plateau']['coverage'] == 0.6


def test_a_plateaued_function_is_skipped_by_the_plan_until_inputs_or_harness_change():
    candidates = {'fn_a': candidate(100, 0.5)}
    plateau = {'harness': 'H1', 'coverage': 0.5, 'obligation': 'input', 'at': 1}
    rows = {'fn_a': {'key': 'k1', 'attempts': attempts_under('H1', 2), 'plateau': plateau}}
    assert lane.plan(candidates, rows, harness='H1') == []
    assert lane.plan(candidates, rows, harness='H2') == ['fn_a']          # harness release reopens
    rows['fn_a']['key'] = 'k0'                                            # hint key moved on: reopens
    assert lane.plan(candidates, rows, harness='H1') == ['fn_a']


def test_a_plateaued_function_reopens_when_its_hints_change(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, clock = make_lane(tmp_path, {fn: candidate(700, 0.5)}, workers=1)
    gate_sequence(ops, fn, [0.5, 0.5, 0.5])
    for _ in range(6):
        l.tick()
        clock['now'] += 1
    assert len(launcher.started) == 2
    ops._candidates[fn]['key'] = 'k2'
    clock['now'] += lane.CANDIDATE_REFRESH_SECONDS + 1
    l.tick()
    assert len(launcher.started) == 3


def test_a_plateaued_function_reopens_on_a_harness_release(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, clock = make_lane(tmp_path, {fn: candidate(700, 0.5)}, workers=1)
    gate_sequence(ops, fn, [0.5, 0.5, 0.5])
    for _ in range(6):
        l.tick()
        clock['now'] += 1
    assert len(launcher.started) == 2
    ops.harness = 'H2'
    clock['now'] += lane.CANDIDATE_REFRESH_SECONDS + 1
    for _ in range(8):
        l.tick()
        clock['now'] += 1
    # The harness release reopens the function. Comparability resets with the harness
    # (2026-11-27 review): the first H2 attempt is the reference, so two further flat
    # attempts — three H2 dispatches in all — re-plateau it under H2.
    assert len(launcher.started) == 5
    assert l.state['functions'][fn]['plateau']['harness'] == 'H2'


def test_a_harness_release_resets_baseline_comparability(tmp_path):
    """Review 2026-11-27: the baseline seeded under H1 must not judge H2 attempts.

    The H1 baseline is higher (0.6) than everything H2 reaches. The first two H2
    coverages are flat relative to each other (0.4, 0.4) but below it. Cross-harness
    comparison would call both no-progress and plateau after two; harness-scoped
    comparability treats the first H2 attempt as the reference, so the plateau
    comes exactly one flat attempt later."""
    fn = 'fn_00401000'
    l, ops, launcher, clock = make_lane(tmp_path, {fn: candidate(700, 0.3)}, workers=1)
    l.state['functions'][fn] = {
        'key': 'k1', 'baseline': 0.6, 'baseline_harness': 'H1',
        'attempts': [{'n': 0, 'model': 'q/strong', 'status': 'low-coverage', 'coverage': 0.6, 'harness': 'H1'}]}
    ops.harness = 'H2'
    gate_sequence(ops, fn, [0.4] * 6)

    def h2_attempts():
        return [a for a in l.state['functions'][fn]['attempts'] if a.get('harness') == 'H2']

    def tick_until(done):
        for _ in range(20):
            if done():
                return
            l.tick()
            clock['now'] += 1
        raise AssertionError('timed out waiting for the lane')

    tick_until(lambda: len(h2_attempts()) == 2)
    # The H1 baseline must not plateau them: cross-harness comparison would call both
    # flat attempts no-progress and stop here.
    assert 'plateau' not in l.state['functions'][fn]
    tick_until(lambda: len(h2_attempts()) == 3)
    for _ in range(3):
        l.tick()
        clock['now'] += 1
    # Reference attempt, then two flat no-progress attempts: exactly three H2 dispatches.
    assert len(launcher.started) == 3
    row = l.state['functions'][fn]
    assert row['plateau']['harness'] == 'H2' and row['plateau']['coverage'] == 0.4
    assert row['attempts'][0]['harness'] == 'H1'   # historical attempts stay for diagnostics


def test_a_fresh_row_still_uses_its_candidate_coverage_baseline(tmp_path):
    """The harness scoping must not defer first-lifecycle plateaus: the candidate scan
    coverage is current-harness evidence, so two flat attempts plateau as before."""
    fn = 'fn_00401000'
    l, ops, launcher, clock = make_lane(tmp_path, {fn: candidate(700, 0.5)}, workers=1)
    gate_sequence(ops, fn, [0.5, 0.5, 0.5])
    for _ in range(6):
        l.tick()
        clock['now'] += 1
    assert len(launcher.started) == 2
    assert l.state['functions'][fn]['plateau']['harness'] == 'H1'


def test_two_no_new_reach_attempts_record_an_import_transcript_obligation(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, clock = make_lane(tmp_path, {fn: candidate(700, 0.5)}, workers=1)
    gate_sequence(ops, fn, [0.5, 0.5], status='no-new-reach')
    for _ in range(6):
        l.tick()
        clock['now'] += 1
    assert len(launcher.started) == 2
    assert l.state['functions'][fn]['plateau']['obligation'] == 'import-transcript'


def test_error_attempts_do_not_count_toward_the_plateau(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, clock = make_lane(tmp_path, {fn: candidate(700, 0.5)}, workers=1)
    coverages = iter([0.5, None, 0.5])

    def gate(fn_, spec, model):
        coverage = next(coverages, 0.5)
        ops.gates.append((fn_, model))
        status = 'error' if coverage is None else 'low-coverage'
        return {'status': status, 'coverage': coverage, 'prior_sha256': 'h' * 64, 'tool': probe.TOOL_VERSION}
    ops.gate = gate
    for _ in range(8):
        l.tick()
        clock['now'] += 1
    # flat, error, flat: only one consecutive coverage-bearing no-progress pair is broken
    # by the error, so no plateau after two attempts; the third attempt still dispatches.
    assert len(launcher.started) == 3


def test_a_plateau_is_recorded_once_not_on_every_tick(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, clock = make_lane(tmp_path, {fn: candidate(700, 0.5)}, workers=1)
    gate_sequence(ops, fn, [0.5, 0.5, 0.5])
    for _ in range(10):
        l.tick()
        clock['now'] += lane.CANDIDATE_REFRESH_SECONDS + 1
    names = [json.loads(x)['event'] for x in (l.dir / 'events.jsonl').read_text().splitlines()]
    assert names.count('input-plateau') == 1
def test_only_one_worker_probes_a_provider_after_its_cooldown(tmp_path):
    fns = ['fn_00401000', 'fn_00402000', 'fn_00403000']
    l, _, launcher, clock = make_lane(
        tmp_path, {fn: candidate(1000 - i, 0.58) for i, fn in enumerate(fns)}, workers=3)
    for fn in fns:
        l.state['functions'][fn] = {'key': 'k1', 'attempts': attempts_under('H1', 2)}
    l.state['limited']['q'] = clock['now'] - 1
    l.tick()
    assert [(job['fn'], job['model']) for job, _, _ in launcher.started] == [(fns[0], 'q/strong')]


def test_a_finished_probe_reopens_the_provider_to_normal_concurrency(tmp_path):
    fns = ['fn_00401000', 'fn_00402000', 'fn_00403000']
    l, _, launcher, clock = make_lane(
        tmp_path, {fn: candidate(1000 - i, 0.58) for i, fn in enumerate(fns)}, workers=3)
    for fn in fns:
        l.state['functions'][fn] = {'key': 'k1', 'attempts': attempts_under('H1', 2)}
    l.state['limited']['q'] = clock['now'] - 1
    l.tick()
    l.tick()
    assert [(job['fn'], job['model']) for job, _, _ in launcher.started] == [
        (fns[0], 'q/strong'), (fns[1], 'q/strong'), (fns[2], 'q/strong')]


# --- explicit post-expiry probe identity ----------------------------------------------

class ControlledLauncher(Launcher):
    """Workers stay running until the test completes their handles."""

    def __init__(self, tmp, **kw):
        super().__init__(tmp, **kw)
        self.handles = []

    def start(self, job, prompt, agent):
        handle = super().start(job, prompt, agent)
        handle.code = None
        self.handles.append(handle)
        return handle


class PartialAuthLauncher(WorkingLauncher):
    """The worker writes real spec bytes, then the provider reports an auth failure."""

    def start(self, job, prompt, agent):
        handle = super().start(job, prompt, agent)
        handle.infra_error = lambda: 'This API key has expired. Create a new API key in OpenCode Console'
        return handle


def high_rung_rows(l, fns):
    for fn in fns:
        l.state['functions'][fn] = {'key': 'k1', 'attempts': attempts_under('H1', 2)}


def write_terminal_probe(job, fn, data=b'{"fixtures":[{"name":"checked"}]}'):
    spec = Path(job['spec'])
    spec.write_bytes(data)
    spec.with_suffix('.probe-result.json').write_text(json.dumps({
        'fn': fn, 'status': 'pass', 'spec_sha256': hashlib.sha256(data).hexdigest(),
        'prior_sha256': 'h' * 64,
    }))


def test_a_stale_worker_neither_blocks_nor_clears_a_post_expiry_probe(tmp_path):
    fns = ['fn_00401000', 'fn_00402000']
    l, _, _, clock = make_lane(
        tmp_path, {fn: candidate(1000 - i, 0.58) for i, fn in enumerate(fns)}, workers=1)
    l.launcher = ControlledLauncher(tmp_path)
    high_rung_rows(l, fns)
    l.tick()
    assert len(l.launcher.started) == 1
    old_job = l.launcher.started[0][0]
    assert 'probe_key' not in old_job

    expiry = clock['now'] - 1
    l.state['limited']['q'] = expiry
    l.workers = 2
    l.tick()
    assert len(l.launcher.started) == 2
    probe_job = l.launcher.started[1][0]
    assert (probe_job['probe_key'], probe_job['probe_expiry']) == ('q', expiry)

    l.launcher.handles[0].code = 0       # the older worker finishes first
    l.tick()
    assert l.state['limited']['q'] == expiry
    l.launcher.handles[1].code = 0       # the tagged post-expiry probe validates it
    l.tick()
    assert 'q' not in l.state['limited']


def test_a_terminal_probe_auth_error_repauses_but_still_gates_the_snapshot(tmp_path):
    fn = 'fn_00401000'
    l, ops, _, clock = make_lane(tmp_path, {fn: candidate(1000, 0.58)}, workers=1)
    l.launcher = ControlledLauncher(tmp_path)
    high_rung_rows(l, [fn])
    l.state['limited']['q'] = clock['now'] - 1
    l.tick()
    job, handle, _ = l.running[0]
    write_terminal_probe(job, fn)
    handle.infra_error = lambda: 'This API key has expired. Create a new API key in OpenCode Console'
    l.tick()
    assert l.state['limited']['q'] == pytest.approx(clock['now'] + lane.INFRA_PAUSE_SECONDS)
    assert ops.gates == [(fn, 'q/strong')]


def test_a_terminal_probe_does_not_clear_a_fresh_limit_generation(tmp_path):
    fn = 'fn_00401000'
    l, ops, _, clock = make_lane(tmp_path, {fn: candidate(1000, 0.58)}, workers=1)
    l.launcher = ControlledLauncher(tmp_path)
    high_rung_rows(l, [fn])
    expiry = clock['now'] - 1
    l.state['limited']['q'] = expiry
    l.tick()
    job, handle, _ = l.running[0]
    write_terminal_probe(job, fn)
    l._pause('q/strong', 'quota', 1000)
    clock['now'] += 1001
    l.tick()
    assert l.state['limited']['q'] == pytest.approx(expiry + 1001)
    assert ops.gates == [(fn, 'q/strong')]


def test_an_auth_error_after_partial_work_repauses_and_still_gates_the_spec(tmp_path):
    fn = 'fn_00401000'
    l, ops, _, clock = make_lane(tmp_path, {fn: candidate(700)}, workers=1)
    l.launcher = PartialAuthLauncher(tmp_path)
    l.tick()
    l.tick()
    assert ops.gates == [(fn, 'p/cheap')]
    assert l.state['limited']['p'] == pytest.approx(clock['now'] + lane.INFRA_PAUSE_SECONDS)


def test_a_probe_completion_does_not_clear_a_fresh_limit_generation(tmp_path):
    fn = 'fn_00401000'
    l, _, _, clock = make_lane(tmp_path, {fn: candidate(1000, 0.58)}, workers=1)
    l.launcher = ControlledLauncher(tmp_path)
    high_rung_rows(l, [fn])
    expiry = clock['now'] - 1
    l.state['limited']['q'] = expiry
    l.tick()
    job = l.launcher.started[0][0]
    assert (job['probe_key'], job['probe_expiry']) == ('q', expiry)
    events = [json.loads(x) for x in (l.dir / 'events.jsonl').read_text().splitlines()]
    dispatch = [event for event in events if event['event'] == 'dispatch'][-1]
    assert (dispatch['probe_key'], dispatch['probe_expiry']) == ('q', expiry)

    l._pause('q/strong', 'quota', 1000)   # a fresh limit replaces the probed generation
    clock['now'] += 1001                  # even its expiry is now in the past
    l.launcher.handles[0].code = 0
    l.tick()
    assert l.state['limited']['q'] == pytest.approx(expiry + 1001)


def test_overlapping_probe_scopes_do_not_clear_each_other(tmp_path):
    fns = ['fn_00401000', 'fn_00402000']
    l, _, _, clock = make_lane(
        tmp_path, {fn: candidate(1000 - i, 0.58) for i, fn in enumerate(fns)}, workers=1)
    l.launcher = ControlledLauncher(tmp_path)
    high_rung_rows(l, fns)
    expiry = clock['now'] - 1
    l.state['limited']['q'] = expiry
    l.state['limited']['q/strong'] = expiry
    l.tick()
    first = l.launcher.started[0][0]
    assert (first['probe_key'], first['probe_expiry']) == ('q', expiry)

    l.launcher.handles[0].code = 0
    l.tick()
    assert 'q' not in l.state['limited']
    assert l.state['limited']['q/strong'] == expiry
    assert len(l.launcher.started) == 2
    second = l.launcher.started[1][0]
    assert (second['probe_key'], second['probe_expiry']) == ('q/strong', expiry)


def start_tagged_probe(tmp_path):
    fn = 'fn_00401000'
    l, _, _, clock = make_lane(tmp_path, {fn: candidate(1000, 0.58)}, workers=1)
    l.launcher = ControlledLauncher(tmp_path)
    high_rung_rows(l, [fn])
    expiry = clock['now'] - 1
    l.state['limited']['q'] = expiry
    l.tick()
    job, handle, _ = l.running[0]
    assert (job['probe_key'], job['probe_expiry']) == ('q', expiry)
    return l, clock, job, handle, expiry


@pytest.mark.parametrize('mode', ['timeout', 'stall', 'nonzero', 'stop'])
def test_an_interrupted_probe_never_clears_the_pause(tmp_path, mode):
    l, clock, _, handle, expiry = start_tagged_probe(tmp_path)
    if mode == 'timeout':
        clock['now'] += l.attempt_seconds + 1
    elif mode == 'stall':
        handle.last_activity = lambda: time.monotonic() - l.stall_seconds - 1
    elif mode == 'nonzero':
        handle.code = 1
    else:
        (l.run_dir / 'STOP').write_text('stop')
    l.tick()
    assert l.state['limited']['q'] == expiry


def test_a_zero_exit_without_work_or_a_claim_does_not_validate_a_probe(tmp_path):
    l, _, _, handle, expiry = start_tagged_probe(tmp_path)
    Path(handle.log).write_text('')
    handle.code = 0
    l.tick()
    assert l.state['limited']['q'] == expiry


def paid_tier_probe_lane(tmp_path, first, second, key):
    fns = ['fn_00401000', 'fn_00402000', 'fn_00403000']
    l, _, _, clock = make_lane(
        tmp_path, {fn: candidate(1000 - i, 0.58) for i, fn in enumerate(fns)}, workers=3)
    l.launcher = ControlledLauncher(tmp_path)
    l.ladder = [first, second]
    expiry = clock['now'] - 1
    l.state['limited'][key] = expiry
    l.tick()
    return l, [(job['model'], job.get('probe_key')) for job, _, _ in l.launcher.started]


def test_a_paid_tier_probe_is_serialized_without_blocking_free_models(tmp_path):
    _, started = paid_tier_probe_lane(tmp_path, 'p/paid', 'p/hy4-flash-free', 'p:paid')
    assert started == [('p/paid', 'p:paid'), ('p/hy4-flash-free', None), ('p/hy4-flash-free', None)]


def test_a_free_tier_probe_is_serialized_without_blocking_paid_models(tmp_path):
    _, started = paid_tier_probe_lane(tmp_path, 'p/hy4-flash-free', 'p/paid', 'p:free')
    assert started == [('p/hy4-flash-free', 'p:free'), ('p/paid', None), ('p/paid', None)]


# --- simultaneous provider error rows -------------------------------------------------

def error_row(error):
    return {'type': 'error', 'error': error}


GO_LIMIT_ROW = error_row({
    'type': 'provider.go-limit', 'status': 429, 'retryAfterMs': 600000, 'message': GO_MONTHLY})
AUTH_ROW = error_row({
    'type': 'provider.auth', 'status': 403,
    'message': 'You have run out of credits or need a Grok subscription.'})
QUOTA_ROW = error_row({
    'type': 'provider.quota', 'status': 429, 'retryAfterMs': 900000,
    'message': 'The Token Plan usage limit has been reached: quota exhausted.'})


class JsonLogLauncher(Launcher):
    """A completed worker whose JSON event log is parsed by the real Opencode handle."""

    def __init__(self, tmp, rows, **kw):
        super().__init__(tmp, **kw)
        self.rows = rows

    def start(self, job, prompt, agent):
        fake = super().start(job, prompt, agent)
        with Path(fake.log).open('a', encoding='utf-8') as out:
            out.write('\n')
            for row in self.rows:
                out.write(json.dumps(row) + '\n')
        import cmre_farm
        handle = cmre_farm.OpencodeHandle(None, fake.log)
        handle.code = 0
        handle.poll = lambda: handle.code
        handle.kill = lambda: setattr(handle, 'killed', True)
        return handle


def test_independent_limit_and_auth_rows_are_classified_separately(tmp_path):
    import cmre_farm
    log = tmp_path / 'w.jsonl'
    log.write_text(json.dumps(GO_LIMIT_ROW) + '\n' + json.dumps(AUTH_ROW) + '\n')
    limit, infra = cmre_farm.OpencodeHandle(None, log).provider_errors()
    assert 'usage limit' in limit[0] and limit[1] == 600
    assert 'run out of credits' in infra


def test_one_usage_limit_row_is_not_an_independent_infrastructure_error(tmp_path):
    import cmre_farm
    log = tmp_path / 'w.jsonl'
    log.write_text(json.dumps(QUOTA_ROW) + '\n')
    limit, infra = cmre_farm.OpencodeHandle(None, log).provider_errors()
    assert 'Token Plan' in limit[0] and infra is None


def test_an_independent_auth_error_pauses_the_provider_beside_a_tier_limit(tmp_path):
    fn = 'fn_00401000'
    l, ops, _, clock = make_lane(tmp_path, {fn: candidate(700)}, workers=1)
    l.launcher = JsonLogLauncher(tmp_path, [GO_LIMIT_ROW, AUTH_ROW])
    l.tick()
    l.tick()
    assert l.state['limited']['p:paid'] == pytest.approx(clock['now'] + 600)
    assert l.state['limited']['p'] == pytest.approx(clock['now'] + lane.INFRA_PAUSE_SECONDS)
    assert ops.gates == [] and l.state['functions'][fn].get('attempts', []) == []
    events = [json.loads(x)['event'] for x in (l.dir / 'events.jsonl').read_text().splitlines()]
    assert 'model-limited' in events and 'model-unavailable' in events


# --- memory bound of a varied spec -------------------------------------------------

class _FakeMachine:
    def run(self, va, case, budget):
        import types
        return types.SimpleNamespace(covered={1, 2}, outcome='return', detail='')


def _fake_context(size=0x10000):
    import types
    opts = types.SimpleNamespace(arg_count=0, budget=1)
    return {1, 2, 3}, opts, _FakeMachine(), [bytes([0xAA]) * size, bytes(size)]


def _wide_spec(count):
    # Zeroing the arena makes every variant carry ~16K dword writes, like the farm's real specs.
    return {'fixtures': [{'name': 's', 'zero': [['0x0', '0x10000']],
                          'vary': [{'field': 'import_seed', 'range': [0, count]}]}]}


def _peak(count):
    import tracemalloc
    tracemalloc.start()
    try:
        probe._run_rows(0x401000, _wide_spec(count), _fake_context())
        return tracemalloc.get_traced_memory()[1]
    finally:
        tracemalloc.stop()


def test_running_a_varied_spec_does_not_retain_every_expanded_fixture():
    small, large = _peak(10), _peak(80)
    assert large < small * 3, (small, large)


def test_rows_keep_names_and_reach_for_every_expansion():
    *_, rows = probe._run_rows(0x401000, _wide_spec(5), _fake_context())
    assert [(r[0], r[2], r[3]) for r in rows] == [(f's#{n}', {1, 2}, 'return') for n in range(5)]


def test_picked_fixtures_are_regenerated_identically_to_expand():
    spec = _wide_spec(6)
    full = probe.expand(spec, 0, _fake_context()[3])
    assert probe.picked_fixtures(spec, [1, 4], 0, _fake_context()[3]) == [full[1][1], full[4][1]]


def test_expansion_limit_is_the_gate_case_budget():
    from cmre import difftest
    assert probe.MAX_EXPANDED == difftest.DEFAULTS['cases']


def test_a_spec_at_the_expansion_limit_runs_and_one_over_is_refused_with_advice():
    at = probe.MAX_EXPANDED
    spec = lambda n: {'fixtures': [{'zero': [], 'vary': [{'field': 'import_seed', 'range': [0, n]}]}]}
    out = probe.expand(spec(at), 0, _fake_context(0x100)[3])
    assert len(out) == at
    with pytest.raises(ValueError, match=f'more than {at} fixtures.*gate'):
        probe.expand(spec(at + 1), 0, _fake_context(0x100)[3])
    with pytest.raises(ValueError, match=f'more than {at} fixtures'):
        probe.expand({'fixtures': [{'vary': [{'field': 'import_seed', 'range': [0, 25]},
                                             {'field': 'stack.0', 'range': [0, 25]}]}]}, 1,
                     _fake_context(0x100)[3])


# --- probe selection cap and model-free re-gates (2026-11-27) ----------------------

def test_a_gate_can_keep_well_over_48_fixtures_that_each_reach_new_code():
    # 27 Nov: the lane's own best specs reached 0.91-0.96 original-only coverage on
    # fn_005576e0/fn_00487f60, but 48 greedy picks kept only 0.55/0.53, exactly the
    # coverage the gate recorded. The cap, not the authoring, was binding.
    rows = [(f'f{i}', {'i': i}, {i}, 'return') for i in range(150)]
    assert len(probe.select(rows, baseline=set())) == 150
    assert probe.MAX_SELECTED >= 150
    assert probe.TOOL_VERSION != 'input-probe/1'


def _regate_lane(tmp_path, fn, gate, attempts=3, plateau=True, **row):
    l, ops, launcher, clock = make_lane(tmp_path, {fn: candidate(9000, 0.55)}, gate={fn: gate})
    spec = l.dir / 'specs' / f'{fn}-7.json'
    spec.parent.mkdir(parents=True, exist_ok=True)
    spec.write_text('{"fixtures": [{"name": "a"}]}\n')
    state = {'key': 'k1', 'bytes': 9000,
             'attempts': [{'n': i, 'model': 'p/cheap', 'status': 'low-coverage', 'coverage': 0.55,
                           'harness': 'H1'} for i in range(attempts)],
             'best': {'spec': str(spec), 'coverage': 0.55, 'model': 'q/strong', 'detail': 'gate low-coverage'}}
    if plateau:
        state['plateau'] = {'at': 1, 'coverage': 0.55, 'harness': 'H1', 'obligation': 'input'}
    state.update(row)
    l.state['functions'][fn] = state
    return l, ops, launcher, spec


def test_a_best_spec_gated_by_an_older_probe_is_regated_without_a_model(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, spec = _regate_lane(tmp_path, fn, {'status': 'pass', 'coverage': 0.83})
    l.tick()
    assert launcher.started == []                         # no model time
    assert ops.gates == [(fn, 'q/strong')]                # provenance keeps the authoring model
    assert ops.applies == [(fn, 'h' * 64)]
    row = l.state['functions'][fn]
    assert row['status'] == 'accepted'
    assert len(row['attempts']) == 3                      # a re-gate is not a model attempt
    assert row['regates'][0]['status'] == 'pass' and row['regates'][0]['tool'] == probe.TOOL_VERSION
    regated = [e for e in lane._events(l.dir / 'events.jsonl') if e['event'] == 'regate']
    assert regated and regated[0]['fn'] == fn and regated[0]['of'] == str(spec)


def test_a_regate_runs_once_per_probe_version_and_is_not_a_model_attempt(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, _ = _regate_lane(tmp_path, fn, {'status': 'low-coverage', 'coverage': 0.58})
    l.tick()
    l.tick()
    l.refresh(force=True)
    l.tick()
    assert ops.gates == [(fn, 'q/strong')]
    row = l.state['functions'][fn]
    assert row['best']['coverage'] == 0.58 and row['best']['probe_tool'] == probe.TOOL_VERSION
    assert len(row['attempts']) == 3
    assert launcher.started == []                         # attempts exhausted: no model either


def test_a_regate_that_does_not_beat_the_plateau_keeps_it(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, spec = _regate_lane(tmp_path, fn, {'status': 'no-new-reach', 'coverage': 0.55}, attempts=2)
    l.tick()
    l.refresh(force=True)
    l.tick()
    row = l.state['functions'][fn]
    assert row['plateau']['harness'] == 'H1' and launcher.started == []
    assert row['best']['spec'] == str(spec) and row['best']['probe_tool'] == probe.TOOL_VERSION
    assert len(ops.gates) == 1


@pytest.mark.parametrize('row', [{'status': 'accepted'}, {'status': 'fail', 'harness': 'H1'},
                                 {'key': 'old'}, {'status': 'pass-unaccepted'}])
def test_a_regate_skips_settled_or_stale_rows(tmp_path, row):
    fn = 'fn_00401000'
    l, ops, launcher, _ = _regate_lane(tmp_path, fn, {'status': 'pass', 'coverage': 0.9}, **row)
    l.tick()
    assert all(model != 'q/strong' for _, model in ops.gates)


def test_a_regate_needs_the_spec_file(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, spec = _regate_lane(tmp_path, fn, {'status': 'pass', 'coverage': 0.9})
    spec.unlink()
    l.tick()
    assert ops.gates == []


def test_a_regate_that_errors_is_retried_a_bounded_number_of_times(tmp_path):
    # Review of 9d4ca8a: a timeout or crash marked the best spec as gated by the new probe,
    # so one transient error lost the re-gate of fn_005576e0/fn_00487f60 for good.
    fn = 'fn_00401000'
    l, ops, launcher, _ = _regate_lane(tmp_path, fn, {'status': 'error', 'error': 'TimeoutExpired'})
    for _ in range(lane.REGATE_ERRORS + 2):
        l.tick()
    assert len(ops.gates) == lane.REGATE_ERRORS
    row = l.state['functions'][fn]
    assert [r['status'] for r in row['regates']] == ['error'] * lane.REGATE_ERRORS
    assert 'status' not in row and len(row['attempts']) == 3


def test_a_regate_after_an_error_can_still_pass(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, _ = _regate_lane(tmp_path, fn, {'status': 'error', 'error': 'MemoryError'})
    l.tick()
    ops.gate_result[fn] = {'status': 'pass', 'coverage': 0.83}
    l.tick()
    assert l.state['functions'][fn]['status'] == 'accepted' and len(ops.gates) == 2


def test_a_function_a_regate_just_accepted_gets_no_model_worker(tmp_path):
    # Review of 9d4ca8a: refresh queued the row, the re-gate accepted it, and dispatch then
    # started a model job on an accepted function in the same tick.
    fn = 'fn_00401000'
    l, ops, launcher, _ = _regate_lane(tmp_path, fn, {'status': 'pass', 'coverage': 0.83},
                                       attempts=1, plateau=False)
    l.tick()
    assert l.state['functions'][fn]['status'] == 'accepted'
    assert launcher.started == []


def test_no_regate_runs_while_the_lane_has_no_workers(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, _ = _regate_lane(tmp_path, fn, {'status': 'pass', 'coverage': 0.83})
    l.workers = 0
    l.tick()
    assert ops.gates == []


def test_one_regate_at_a_time_leaves_gate_threads_for_workers(tmp_path):
    l, ops, launcher, clock = make_lane(tmp_path, {f'fn_0040{i}000': candidate(9000, 0.55) for i in range(1, 4)})
    for i in range(1, 4):
        fn = f'fn_0040{i}000'
        spec = l.dir / 'specs' / f'{fn}-7.json'
        spec.parent.mkdir(parents=True, exist_ok=True)
        spec.write_text('{"fixtures": []}\n')
        l.state['functions'][fn] = {
            'key': 'k1', 'attempts': [{'n': j, 'status': 'low-coverage', 'coverage': 0.55, 'harness': 'H1'}
                                      for j in range(3)],
            'best': {'spec': str(spec), 'coverage': 0.5 + i / 100, 'model': 'q/strong'}}
    l.refresh(force=True)
    l.gates = DeferredGates()
    l.regate()
    l.regate()
    assert [job['fn'] for job, *_ in l.gating] == ['fn_00403000']   # best coverage first
    assert lane.REGATE_JOBS == 1


def test_a_regate_that_beats_the_plateau_reopens_model_work_from_the_better_spec(tmp_path):
    fn = 'fn_00401000'
    l, ops, launcher, _ = _regate_lane(tmp_path, fn, {'status': 'low-coverage', 'coverage': 0.58}, attempts=2)
    l.tick()
    row = l.state['functions'][fn]
    assert 'plateau' not in row and row['best']['coverage'] == 0.58
    l.refresh(force=True)
    l.tick()
    assert len(launcher.started) == 1
    job = launcher.started[0][0]
    assert Path(job['spec']).read_bytes() == Path(row['best']['spec']).read_bytes()


def test_a_regate_without_a_known_author_records_no_model(tmp_path, monkeypatch):
    seen = []

    class Done:
        stdout = '{"status": "low-coverage", "coverage": 0.5}\n'

    monkeypatch.setattr(lane.subprocess, 'run', lambda command, **kw: seen.append(command) or Done())
    lane.CmreOps(tmp_path).gate('fn_00401000', tmp_path / 's.json', None)
    assert '--model' not in seen[0]
    lane.CmreOps(tmp_path).gate('fn_00401000', tmp_path / 's.json', 'p/cheap')
    assert seen[1][-2:] == ['--model', 'p/cheap']


class DeferredGates:
    """Gates that never finish, to observe how many the lane keeps in flight."""

    def submit(self, fn, *args):
        class Pending:
            def done(self):
                return False
        return Pending()
