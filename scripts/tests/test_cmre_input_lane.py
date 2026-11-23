import hashlib
import json
import sys
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
        return {'proposal': str(spec.with_suffix('.p.json')), 'prior_sha256': 'h' * 64,
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
