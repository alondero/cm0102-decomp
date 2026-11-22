import json
import os
import sys
import time
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import cmre_defect_lane as lane
import cmre_farm as farm


def text_log(path, text):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps({'type': 'step_start'}) + '\n'
                    + json.dumps({'type': 'text', 'part': {'type': 'text', 'text': text}}) + '\n',
                    encoding='utf-8')


class Handle:
    def __init__(self, log, code=0, limit=None, infra=None):
        self.log, self.code, self.limit, self.infra, self.killed = Path(log), code, limit, infra, False

    def poll(self):
        return self.code

    def kill(self):
        self.killed = True

    def limit_error(self):
        return self.limit

    def infra_error(self):
        return self.infra


class Launcher:
    """Runs `behaviour(job) -> result text` and writes it as the worker's final text part."""

    def __init__(self, tmp, behaviour, **handle):
        self.tmp, self.behaviour, self.handle, self.started = Path(tmp), behaviour, handle, []

    def start(self, job, prompt, agent):
        self.started.append((job, prompt, agent))
        log = self.tmp / 'logs' / f"{job['fn']}-{job['n']}.jsonl"
        text_log(log, self.behaviour(job))
        return Handle(log, **self.handle)


class Ops:
    def __init__(self, candidates, code=0):
        self._candidates, self.code, self.submitted = candidates, code, []
        self._outcomes = {}

    def outcomes(self):
        return json.loads(json.dumps(self._outcomes))

    def candidates(self):
        return json.loads(json.dumps(self._candidates))

    def command(self, action, job, args):
        return [action, job['fn'], json.dumps(args, sort_keys=True)]

    def submit(self, command):
        self.submitted.append(command)
        return self.code, 'queued'


def cand(bytes_=100, key='k1'):
    return {'bytes': bytes_, 'key': key, 'findings': 1}


def adopt_line(job, note='fixed the reload; gate passes'):
    Path(job['hand']).parent.mkdir(parents=True, exist_ok=True)
    Path(job['hand']).write_text('/* fixed */\n')
    return f"Checked both findings.\nADOPT {job['hand']} {job['receipt']} {note}"


def make(tmp, candidates, behaviour, submit=True, code=0, **handle):
    ops, launcher = Ops(candidates, code), Launcher(tmp, behaviour, **handle)
    run = tmp / 'run'
    run.mkdir(exist_ok=True)
    return lane.Lane(run, ops, launcher, workers=2, model='openai/m', root=tmp, submit=submit), ops, launcher


def drain(l, ticks=4):
    for _ in range(ticks):
        l.tick()


# --- planning and parsing ----------------------------------------------------------

def test_plan_takes_the_largest_defect_first_and_honours_attempts_and_pending_submissions():
    now = 10_000.0
    candidates = {'fn_a': cand(10), 'fn_b': cand(500), 'fn_c': cand(300), 'fn_d': cand(200), 'fn_e': cand(50)}
    rows = {'fn_c': {'key': 'k1', 'attempts': [{}, {}]},                       # attempts used up
            'fn_d': {'key': 'k1', 'status': 'submitted', 'submitted_at': now - 60},
            'fn_e': {'key': 'old', 'attempts': [{}, {}]}}                       # findings changed
    assert lane.plan(candidates, rows, active={'fn_a'}, now=now) == ['fn_b', 'fn_e']
    rows['fn_d']['submitted_at'] = now - lane.SUBMIT_GRACE_SECONDS - 1
    assert lane.plan(candidates, rows, now=now) == ['fn_b', 'fn_d', 'fn_e', 'fn_a']


def test_the_last_result_line_decides():
    text = ('I will finish with ADOPT <source> <review_receipt> <note>.\n'
            'UNRESOLVED needs an ABI entry decision\n`DISMISS r.json both findings disproved`\n')
    assert lane.parse_result(text) == ('DISMISS', 'r.json both findings disproved')
    assert lane.parse_result('no verdict here') == (None, '')


def test_a_result_line_must_name_the_jobs_own_claim_and_hand_file(tmp_path):
    job = {'receipt': str(tmp_path / 'reviews/c/receipt.json'), 'hand': str(tmp_path / 'hand/fn_x-c.c')}
    other = str(tmp_path / 'reviews/someone/receipt.json')
    assert lane.decide(job, 'ADOPT', f"{job['hand']} {job['receipt']} note")[0] == 'none'   # no file yet
    Path(job['hand']).parent.mkdir(parents=True)
    Path(job['hand']).write_text('x')
    assert lane.decide(job, 'ADOPT', f"{job['hand']} {job['receipt']} fixed it") == (
        'adopt', {'source': str(Path(job['hand']).resolve()), 'receipt': job['receipt'], 'note': 'fixed it'})
    assert lane.decide(job, 'ADOPT', f"{job['hand']} {other} fixed it")[0] == 'none'
    assert lane.decide(job, 'ADOPT', f"{job['hand']} {job['receipt']}")[0] == 'none'          # no note
    assert lane.decide(job, 'DISMISS', f"{job['receipt']} VA 0x401000 disproves it") == (
        'dismiss', {'receipt': job['receipt'], 'reason': 'VA 0x401000 disproves it'})
    assert lane.decide(job, 'DISMISS', f'{other} reason')[0] == 'none'
    assert lane.decide(job, 'UNRESOLVED', 'abi')[1] == 'unresolved: abi'


# --- the lane ------------------------------------------------------------------------

def test_an_adopt_line_becomes_one_farm_request(tmp_path):
    l, ops, launcher = make(tmp_path, {'fn_00401000': cand()}, adopt_line)
    drain(l)
    assert len(launcher.started) == 1
    job, prompt, agent = launcher.started[0]
    assert job['claim'].startswith(lane.CLAIM_PREFIX) and f"--claim {job['claim']}" in prompt
    assert ops.submitted == [['adopt', 'fn_00401000', json.dumps(
        {'note': 'fixed the reload; gate passes', 'receipt': job['receipt'],
         'source': str(Path(job['hand']).resolve())}, sort_keys=True)]]
    row = l.state['functions']['fn_00401000']
    assert row['status'] == 'submitted' and len(row['attempts']) == 1


def test_no_submit_records_the_command_without_queuing_it(tmp_path):
    l, ops, _ = make(tmp_path, {'fn_00401000': cand()}, adopt_line, submit=False)
    drain(l)
    assert ops.submitted == []
    row = l.state['functions']['fn_00401000']
    assert row['status'] == 'recorded' and row['attempts'][0]['command'][0] == 'adopt'
    assert lane.lane_summary(l.run_dir) is None   # the live lane's counts are untouched


def test_unresolved_gets_one_more_look_with_the_prior_result_then_stops(tmp_path):
    l, ops, launcher = make(tmp_path, {'fn_00401000': cand()}, lambda job: 'UNRESOLVED entry-stack alias')
    drain(l, ticks=6)
    assert len(launcher.started) == lane.MAX_ATTEMPTS == 2
    assert 'UNRESOLVED entry-stack alias' in launcher.started[1][1]
    assert ops.submitted == []
    # New findings or inputs are a new key: the lane looks again.
    ops._candidates['fn_00401000']['key'] = 'k2'
    l.refreshed = None
    drain(l, ticks=1)
    assert len(launcher.started) == 3 and launcher.started[2][0]['key'] == 'k2'


def test_a_provider_limit_is_not_an_attempt_and_pauses_the_model(tmp_path):
    l, ops, launcher = make(tmp_path, {'fn_00401000': cand()}, lambda job: 'working...',
                            limit=('usage limit reached', 3600))
    drain(l, ticks=3)
    assert len(launcher.started) == 1
    assert l.state['functions']['fn_00401000'].get('attempts') == []
    assert l.state['limited']['openai'] > time.time()


def test_a_worker_that_finishes_despite_a_limit_message_still_counts(tmp_path):
    l, ops, _ = make(tmp_path, {'fn_00401000': cand()}, adopt_line, limit=('rate limit', 60))
    drain(l)
    assert len(ops.submitted) == 1


def test_a_refused_submission_is_recorded_and_counts_as_an_attempt(tmp_path):
    l, ops, _ = make(tmp_path, {'fn_00401000': cand()}, adopt_line, code=2)
    drain(l)
    row = l.state['functions']['fn_00401000']
    assert row['status'] == 'submit-refused' and row['attempts'][0]['submit']['code'] == 2


def test_stop_ends_dispatch(tmp_path):
    l, ops, launcher = make(tmp_path, {'fn_00401000': cand()}, adopt_line)
    (l.run_dir / 'STOP').write_text('x')
    drain(l)
    assert launcher.started == [] and l.finished()


def test_status_summary_counts_results(tmp_path):
    l, _, _ = make(tmp_path, {'fn_00401000': cand(), 'fn_00402000': cand()},
                   lambda job: adopt_line(job) if job['fn'] == 'fn_00401000' else 'UNRESOLVED x')
    drain(l, ticks=2)
    # The UNRESOLVED defect is already back for its second look.
    assert lane.lane_summary(l.run_dir) == {'running': 1, 'adopt': 1, 'dismiss': 0, 'unresolved': 1,
                                            'applied': 0, 'refused': 0}


def test_agent_edits_only_its_hand_file_and_denies_shell_chaining(tmp_path):
    job = {'fn': 'fn_00401000', 'hand': str(tmp_path / 'run/hand/fn_00401000-c.c')}
    text = lane.agent_text(tmp_path, job)
    rules = [line for line in text.splitlines() if line.strip().startswith('- {')]
    edits = [r for r in rules if 'action: "edit"' in r or 'action: "write"' in r]
    assert edits and all('run/hand/fn_00401000-c.c' in r for r in edits)
    assert rules[-1].endswith('effect: deny }')            # OpenCode applies the last matching rule
    assert any('"*>*"' in r for r in rules) and 'mode: primary' in text
    assert farm.DEFECT_AGENT_BODY.strip().splitlines()[0] in text


# --- candidates from a farm run -------------------------------------------------------

def test_candidates_skip_quarantined_requested_and_recently_claimed_by_others(tmp_path):
    run = tmp_path / 'run'
    (tmp_path / 'recovery/c').mkdir(parents=True)
    fns = ['fn_00401000', 'fn_00402000', 'fn_00403000', 'fn_00404000', 'fn_00405000']
    for fn in fns:
        (tmp_path / f'recovery/c/{fn}.c').write_text(f'/* {fn} */')
    (tmp_path / 'run').mkdir()
    (tmp_path / 'run/events.jsonl').write_text(''.join(
        json.dumps({'event': 'open-defect', 'fn': fn, 'at': 1.0}) + '\n' for fn in fns))
    state = {'defects': {fn: [{'line': 1, 'detail': 'd'}] for fn in fns},
             'accepted': {fn: {'bytes': 100 + i} for i, fn in enumerate(fns)},
             'quarantined': {'fn_00402000': {}}}
    (run / 'state.json').write_text(json.dumps(state))
    (run / 'requests').mkdir()
    (run / 'requests/1-fn_00403000-adopt.json').write_text(json.dumps({'fn': 'fn_00403000'}))
    for name, fn in (('terra-x', 'fn_00404000'), (lane.CLAIM_PREFIX + '7-405000', 'fn_00405000')):
        (run / 'reviews' / name).mkdir(parents=True)
        (run / 'reviews' / name / 'receipt.json').write_text(json.dumps({'fn': fn}))
    out = lane.FarmOps(run, root=tmp_path).candidates()
    assert sorted(out) == ['fn_00401000', 'fn_00405000']
    old = time.time() - lane.MANUAL_CLAIM_SECONDS - 10
    os.utime(run / 'reviews/terra-x/receipt.json', (old, old))
    assert 'fn_00404000' in lane.FarmOps(run, root=tmp_path).candidates()
    # The key follows the accepted source.
    key = out['fn_00401000']['key']
    (tmp_path / 'recovery/c/fn_00401000.c').write_text('/* changed */')
    assert lane.FarmOps(run, root=tmp_path).candidates()['fn_00401000']['key'] != key


def test_farm_ops_commands_use_the_farm_request_protocol(tmp_path):
    ops = lane.FarmOps(tmp_path / 'run', root=tmp_path)
    job = {'fn': 'fn_00401000', 'claim': 'defect-lane-3-401000', 'model': 'openai/m'}
    adopt = ops.command('adopt', job, {'source': 's.c', 'receipt': 'r.json', 'note': 'n'})
    assert adopt[2:5] == ['adopt', str(tmp_path / 'run'), 'fn_00401000']
    assert adopt[5:9] == ['--source', 's.c', '--review-receipt', 'r.json'] and 'defect-lane-3' in adopt[-1]
    dismiss = ops.command('dismiss', job, {'receipt': 'r.json', 'reason': 'disproved'})
    assert dismiss[5:7] == ['--review-receipt', 'r.json'] and dismiss[-2] == '--reason'


# --- review round 1 (22 Nov) --------------------------------------------------------

def tool_then_text_log(path, text, trailing_tool=False):
    path.parent.mkdir(parents=True, exist_ok=True)
    rows = [{'type': 'text', 'part': {'type': 'text', 'text': text}}]
    if trailing_tool:
        rows.append({'type': 'tool_use', 'part': {'type': 'tool', 'tool': 'bash'}})
    path.write_text(''.join(json.dumps(r) + '\n' for r in rows), encoding='utf-8')


def test_a_result_written_before_more_tool_calls_is_not_a_result(tmp_path):
    log = tmp_path / 'w.jsonl'
    tool_then_text_log(log, 'DISMISS r.json planned', trailing_tool=True)
    assert lane.final_text(log) == ''
    tool_then_text_log(log, 'DISMISS r.json done')
    assert lane.final_text(log) == 'DISMISS r.json done'


def test_a_killed_worker_submits_nothing(tmp_path):
    l, ops, launcher = make(tmp_path, {'fn_00401000': cand()}, adopt_line)
    launcher.handle = {'code': None}   # still running when its time runs out
    l.attempt_seconds = -1
    drain(l, ticks=1)
    drain(l, ticks=1)
    assert ops.submitted == []
    row = l.state['functions']['fn_00401000']
    assert row['attempts'][0]['detail'].startswith('timeout') and row['attempts'][0]['verb'] is None


def test_the_prompt_uses_the_farm_test_command_as_it_parses(tmp_path):
    job = {'fn': 'fn_00401000', 'run_dir': str(tmp_path / 'run'), 'claim': 'c',
           'receipt': str(tmp_path / 'run/reviews/c/receipt.json'), 'hand': str(tmp_path / 'run/hand/fn_00401000-c.c')}
    text = lane.prompt(tmp_path, job)
    command = next(line for line in text.splitlines() if 'cmre_farm.py test' in line)
    args = command.split('`')[1].split()[3:]
    assert args[:4] == ['test', 'run', 'fn_00401000', 'run/hand/fn_00401000-c.c'] and '--source' not in args


def test_root_relative_result_paths_match_from_any_working_directory(tmp_path, monkeypatch):
    job = {'receipt': str(tmp_path / 'run/reviews/c/receipt.json'), 'hand': str(tmp_path / 'run/hand/fn_x-c.c')}
    Path(job['hand']).parent.mkdir(parents=True)
    Path(job['hand']).write_text('x')
    monkeypatch.chdir(tmp_path.parent)
    action, _ = lane.decide(job, 'ADOPT', 'run/hand/fn_x-c.c run/reviews/c/receipt.json fixed', root=tmp_path)
    assert action == 'adopt'


def test_a_smoke_run_keeps_its_own_state_and_stops_after_one_recorded_result(tmp_path):
    l, ops, launcher = make(tmp_path, {'fn_00401000': cand()}, adopt_line, submit=False)
    drain(l, ticks=6)
    assert len(launcher.started) == 1 and ops.submitted == []
    assert l.dir == l.run_dir / 'defect-lane' / 'smoke'
    assert launcher.started[0][0]['claim'].startswith(lane.SMOKE_PREFIX)
    live = lane.Lane(l.run_dir, ops, launcher, workers=1, model='openai/m', root=tmp_path)
    assert live.state['functions'] == {}


def fix_run(tmp_path, events):
    run = tmp_path / 'run'
    (tmp_path / 'recovery/c').mkdir(parents=True, exist_ok=True)
    (tmp_path / 'recovery/c/fn_00401000.c').write_text('x')
    run.mkdir(exist_ok=True)
    (run / 'state.json').write_text(json.dumps({'defects': {'fn_00401000': [{'d': 1}]},
                                                 'accepted': {'fn_00401000': {'bytes': 9}}}))
    (run / 'events.jsonl').write_text(''.join(json.dumps(e) + '\n' for e in events))
    return lane.FarmOps(run, root=tmp_path, clock=lambda: 100_000.0)


def test_candidates_wait_for_the_farm_to_report_the_defect_open(tmp_path):
    fn = 'fn_00401000'
    # _defect records the defect and queues its fix in one step: no event yet means not ours.
    assert fix_run(tmp_path, []).candidates() == {}
    assert fix_run(tmp_path, [{'event': 'open-defect', 'fn': fn, 'at': 1.0},
                              {'event': 'dispatch', 'kind': 'fix', 'fn': fn, 'at': 99_000.0}]).candidates() == {}
    assert list(fix_run(tmp_path, [{'event': 'dispatch', 'kind': 'fix', 'fn': fn, 'at': 99_000.0},
                                   {'event': 'open-defect', 'fn': fn, 'at': 99_500.0}]).candidates()) == [fn]
    # A farm that died during its fix never reports it open: an old fix dispatch frees it.
    old = 100_000.0 - lane.STALE_FIX_SECONDS - 1
    assert list(fix_run(tmp_path, [{'event': 'dispatch', 'kind': 'fix', 'fn': fn, 'at': old}]).candidates()) == [fn]


def test_a_relative_result_line_is_submitted(tmp_path):
    def relative(job):
        adopt_line(job)
        rel = lambda path: Path(path).relative_to(tmp_path).as_posix()
        return f"ADOPT {rel(job['hand'])} {rel(job['receipt'])} fixed"
    l, ops, _ = make(tmp_path, {'fn_00401000': cand()}, relative)
    drain(l)
    assert len(ops.submitted) == 1


def test_a_submit_timeout_is_recorded_not_fatal(tmp_path, monkeypatch):
    import subprocess

    def slow(*args, **kwargs):
        raise subprocess.TimeoutExpired('cmd', 600)
    monkeypatch.setattr(lane.subprocess, 'run', slow)
    code, output = lane.FarmOps(tmp_path, root=tmp_path).submit(['x'])
    assert code is None and 'timed out' in output


def test_a_launch_failure_pauses_the_model_instead_of_crashing(tmp_path):
    l, ops, launcher = make(tmp_path, {'fn_00401000': cand()}, adopt_line)

    def broken(*args):
        raise OSError('opencode missing')
    launcher.start = broken
    drain(l, ticks=2)
    assert l.state['limited']['openai/m'] > time.time() and not l.running


# --- farm verdicts on submitted requests (22 Nov: fn_006ec010 refused twice) -------

X87 = ('BLOCKER: x87 operand safeguard: unsupported: cm_x87_CIpow inside a preprocessor directive '
       'cannot be mutated per call')


def submitted_once(tmp_path, behaviour=adopt_line):
    l, ops, launcher = make(tmp_path, {'fn_00401000': cand()}, behaviour)
    l.workers = 1
    drain(l, ticks=2)
    assert l.state['functions']['fn_00401000']['status'] == 'submitted' and len(launcher.started) == 1
    return l, ops, launcher


def test_a_farm_refusal_frees_the_defect_at_once_and_the_next_worker_is_told_why(tmp_path):
    l, ops, launcher = submitted_once(tmp_path)
    ops._outcomes = {'fn_00401000': [{'at': time.time() + 1, 'event': 'request-failed', 'kind': 'adopt',
                                      'detail': X87}]}
    l.refreshed = None
    drain(l, ticks=1)
    row = l.state['functions']['fn_00401000']
    assert row['attempts'][0]['farm'] == {'outcome': 'refused', 'detail': X87}
    assert len(launcher.started) == 2            # not held for SUBMIT_GRACE_SECONDS
    second = launcher.started[1][1]
    assert 'The farm refused' in second and 'preprocessor directive' in second
    assert any(e['event'] == 'farm-refused' for e in lane._events(l.dir / 'events.jsonl'))


def test_an_applied_request_is_final_for_its_key(tmp_path):
    l, ops, launcher = submitted_once(tmp_path)
    ops._outcomes = {'fn_00401000': [{'at': time.time() + 1, 'event': 'accepted', 'kind': 'adopt'}]}
    l.refreshed = None
    drain(l, ticks=3)
    assert l.state['functions']['fn_00401000']['status'] == 'applied' and len(launcher.started) == 1


def test_older_or_other_verb_outcomes_do_not_settle_a_submission(tmp_path):
    l, ops, launcher = submitted_once(tmp_path)
    ops._outcomes = {'fn_00401000': [
        {'at': time.time() - 3600, 'event': 'request-failed', 'kind': 'adopt', 'detail': 'old'},
        {'at': time.time() + 1, 'event': 'request-failed', 'kind': 'dismiss', 'detail': 'someone else'},
        {'at': time.time() + 1, 'event': 'dismissed'}]}
    l.refreshed = None
    drain(l, ticks=1)
    assert l.state['functions']['fn_00401000']['status'] == 'submitted' and len(launcher.started) == 1


def test_a_dismiss_is_settled_by_the_farm_dismissed_event(tmp_path):
    def dismiss(job):
        return f"DISMISS {job['receipt']} VA 0x401010 disproves it"
    l, ops, _ = submitted_once(tmp_path, dismiss)
    ops._outcomes = {'fn_00401000': [{'at': time.time() + 1, 'event': 'dismissed'}]}
    l.refreshed = None
    drain(l, ticks=1)
    assert l.state['functions']['fn_00401000']['status'] == 'applied'


def test_status_counts_what_the_farm_applied_and_refused(tmp_path):
    l, ops, _ = submitted_once(tmp_path)
    ops._outcomes = {'fn_00401000': [{'at': time.time() + 1, 'event': 'request-failed', 'kind': 'adopt',
                                      'detail': X87}]}
    l.refreshed = None
    drain(l, ticks=1)
    summary = lane.lane_summary(l.run_dir)
    assert summary['refused'] == 1 and summary['applied'] == 0


def test_farm_ops_reads_request_outcomes_from_the_run_events(tmp_path):
    run = tmp_path / 'run'
    run.mkdir()
    rows = [{'at': 1, 'event': 'request-failed', 'fn': 'fn_a', 'kind': 'adopt', 'detail': X87},
            {'at': 2, 'event': 'accepted', 'fn': 'fn_a', 'kind': 'adopt'},
            {'at': 3, 'event': 'accepted', 'fn': 'fn_b', 'kind': 'translate'},
            {'at': 4, 'event': 'dismissed', 'fn': 'fn_c', 'reason': 'r'}]
    (run / 'events.jsonl').write_text(''.join(json.dumps(r) + '\n' for r in rows), encoding='utf-8')
    out = lane.FarmOps(run, root=tmp_path).outcomes()
    assert [o['event'] for o in out['fn_a']] == ['request-failed', 'accepted'] and out['fn_a'][0]['detail'] == X87
    assert 'fn_b' not in out and out['fn_c'][0]['event'] == 'dismissed'


def test_defect_workers_are_told_to_keep_x87_calls_out_of_macros(tmp_path):
    job = {'fn': 'fn_00401000', 'hand': str(tmp_path / 'run/hand/fn_00401000-c.c')}
    text = lane.agent_text(tmp_path, job)
    assert 'cm_x87_' in text and '#define' in text and 'call sites' in text
