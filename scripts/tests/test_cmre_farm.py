"""Farm scheduling, verification and accounting with fake cmre and fake workers.

No test starts a paid model or runs the real gate.
"""
import importlib.util
import hashlib
import json
import os
import re
from pathlib import Path
import sys
import time

import pytest

MODULE = Path(__file__).resolve().parents[1] / 'cmre_farm.py'
spec = importlib.util.spec_from_file_location('cmre_farm', MODULE)
farm = importlib.util.module_from_spec(spec)
sys.modules['cmre_farm'] = farm
spec.loader.exec_module(farm)

GOOD = '#include "cmre.h"\nu32 cm_fn_00401000(u32 a) { return a + 1; }\n'


@pytest.mark.parametrize("include_held", [False, True])
def test_real_adapter_forwards_explicit_held_claim_option(monkeypatch, tmp_path, include_held):
    from cmre import work
    calls = []
    monkeypatch.setattr(work, "claim", lambda *args, **kwargs: calls.append((args, kwargs)))
    farm.CmreAdapter(tmp_path, include_held=include_held).claim("fn_00401000", "farm-t", 2)
    assert calls == [(([0x401000], "farm-t", 2), {"include_held": include_held})]


class FakeCmre:
    """Gate stand-in: a source passes when it contains PASS or is GOOD."""

    def __init__(self, items, holds=None, accept_codes=None):
        self.items = list(items)
        self.holds = dict(holds or {})
        self.accept_codes = list(accept_codes or [])
        self.claimed, self.accepted, self.retried, self.tests = [], [], [], []
        self.root = None

    def frontier(self, limit, min_bytes, max_bytes, include_held, owner):
        out = [i for i in self.items if i.bytes >= min_bytes and (max_bytes is None or i.bytes <= max_bytes)]
        if not include_held:
            out = [i for i in out if i.fn not in self.holds]
        return out[:limit]

    def claim(self, fn, owner, hours):
        if fn == 'fn_badbad00':
            raise ValueError('exhausted its five-attempt budget')
        self.claimed.append(fn)

    def release(self, owner, fns):
        pass

    def test(self, fn, source=None):
        path = Path(source) if source else self.root / 'recovery/c' / f'{fn}.c'
        self.tests.append((fn, source))
        text = path.read_text() if path.exists() else ''
        ok = text == GOOD or 'PASS' in text
        return {'status': 'pass' if ok else 'fail', 'problems': [] if ok else ['mismatch'],
                'builds': {'-O0': {'status': 'pass' if ok else 'mismatch', 'detail': 'case 7 eax'}}}

    def accept(self, fn):
        code = self.accept_codes.pop(0) if self.accept_codes else 0
        if code == 0:
            self.accepted.append(fn)
        return code, 'ok' if code == 0 else 'busy'

    def hold(self, fn):
        return self.holds.get(fn)

    def held(self, fns):
        return {fn for fn in fns if fn in self.holds}

    def retry(self, fn, reason):
        self.retried.append(fn)
        self.holds.pop(fn, None)


class FakeHandle:
    def __init__(self, job, behaviour, root):
        self.job, self.behaviour, self.root, self.done, self.killed = job, behaviour, root, False, False

    def poll(self):
        if not self.done:
            self.done = True
            self.behaviour(self.job, self.root)
        return 0

    def kill(self):
        self.killed = True


class FakeLauncher:
    def __init__(self, root, behaviour):
        self.root, self.behaviour, self.started = root, behaviour, []

    def start(self, job, prompt, config):
        self.started.append((job, prompt, config))
        return FakeHandle(job, self.behaviour, self.root)


def writes(text):
    def behaviour(job, root):
        if job['kind'] == 'audit':
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).write_text(json.dumps({'verdict': 'ok', 'findings': []}))
            return
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(text(job) if callable(text) else text)
    return behaviour


@pytest.fixture(autouse=True)
def _restore_claim_owner(monkeypatch):
    # Farm() exports CMRE_CLAIM_OWNER process-wide (right for the real farm process);
    # restore it so it never leaks into the rest of the suite.
    monkeypatch.setenv('CMRE_CLAIM_OWNER', 'pytest-placeholder')


@pytest.fixture
def root(tmp_path):
    r = tmp_path / 'repo'
    (r / 'recovery/c').mkdir(parents=True)
    (r / 'recovery/hints').mkdir(parents=True)
    return r


def make(root, items, behaviour, holds=None, accept_codes=None, **cfg):
    cmre = FakeCmre(items, holds, accept_codes)
    cmre.root = root
    launcher = FakeLauncher(root, behaviour)
    options = dict(workers=2, ladder=['m/cheap', 'm/strong'], audit_workers=0, audit_model='m/strong',
                   attempt_minutes=30, hours=1, min_bytes=0, max_bytes=None, include_held=False, owner='farm-t')
    options.update(cfg)
    run = root / 'run'
    f = farm.Farm(run, root, cmre, launcher, farm.FarmConfig(**options), clock=lambda: 0.0)
    return f, cmre, launcher


def item(fn, size=200):
    return farm.Item(fn, size, True, 0, 'mod.cpp')


@pytest.mark.parametrize("valid", [False, True])
def test_hash_bound_interrupted_candidate_seeds_private_worker(root, valid):
    f, _, _ = make(root, [], writes(GOOD))
    saved = f.run_dir / "retained.c"
    saved.write_bytes(GOOD.encode())
    (f.run_dir / "resume-candidates.json").write_text(json.dumps({
        "schema": "cmre-farm-resume-candidates/1",
        "sources": {"fn_00401000": {
            "path": saved.name,
            "sha256": hashlib.sha256(saved.read_bytes()).hexdigest() if valid else "0" * 64,
        }},
    }))
    job = f._new_job("translate", "fn_00401000", "m/cheap", bytes=200)
    f._launch(job)
    assert Path(job["source"]).exists() is valid
    if valid:
        assert Path(job["source"]).read_bytes() == saved.read_bytes()
        assert job["seeded"] is True
    assert not (root / "recovery/c/fn_00401000.c").exists()
    assert f.state_path.exists()  # dispatch heartbeat precedes the next tick


def test_interrupted_seed_cannot_escape_run_directory(root):
    f, _, _ = make(root, [], writes(GOOD))
    outside = root / "outside.c"
    outside.write_bytes(GOOD.encode())
    (f.run_dir / "resume-candidates.json").write_text(json.dumps({
        "schema": "cmre-farm-resume-candidates/1",
        "sources": {"fn_00401000": {
            "path": "../outside.c", "sha256": hashlib.sha256(outside.read_bytes()).hexdigest(),
        }},
    }))
    job = f._new_job("translate", "fn_00401000", "m/cheap", bytes=200)
    f._launch(job)
    assert not Path(job["source"]).exists()


def test_run_saves_heartbeat_before_first_tick(root, monkeypatch):
    f, _, _ = make(root, [], writes(GOOD))
    def first_tick():
        assert f.state_path.exists()
        f.frontier = []
    monkeypatch.setattr(f, "tick", first_tick)
    f.run(poll=0)


def drain(f, ticks=20):
    for _ in range(ticks):
        f.tick()


def test_queue_parsing_reads_cmre_next_lines():
    text = 'fn_0055f620    394B waits:5  display.cpp\nfn_0094bc6e     13B ready    \nnoise\n'
    got = farm.parse_queue(text)
    assert [(i.fn, i.bytes, i.ready, i.waits) for i in got] == [('fn_0055f620', 394, False, 5), ('fn_0094bc6e', 13, True, 0)]


def test_passing_worker_source_is_copied_tested_and_accepted(root):
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD))
    drain(f)
    assert cmre.accepted == ['fn_00401000']
    assert (root / 'recovery/c/fn_00401000.c').read_text() == GOOD
    assert f.state['accepted']['fn_00401000']['bytes'] == 200
    assert f.summary()['accepted_bytes'] == 200


def test_failed_attempt_restores_previous_source_and_escalates_model(root):
    (root / 'recovery/c/fn_00401000.c').write_text('/* WIP old */')
    attempts = []

    def behaviour(job, r):
        attempts.append(job['model'])
        text = 'PASS' if job['model'] == 'm/strong' else 'bad'
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD.replace('a + 1', 'a + 1 /*PASS*/') if text == 'PASS' else text)

    f, cmre, launcher = make(root, [item('fn_00401000')], behaviour, workers=1)
    drain(f)
    assert attempts == ['m/cheap', 'm/strong']
    assert cmre.accepted == ['fn_00401000']
    # The second prompt carries the first attempt's failure and its source path.
    second_prompt = launcher.started[1][1]
    assert 'Previous attempt' in second_prompt and 'mismatch' in second_prompt


def test_failure_restores_exact_prior_bytes(root):
    (root / 'recovery/c/fn_00401000.c').write_text('/* WIP old */')
    f, cmre, _ = make(root, [item('fn_00401000')], writes('bad'), ladder=['m/cheap'])
    drain(f)
    assert cmre.accepted == []
    assert (root / 'recovery/c/fn_00401000.c').read_text() == '/* WIP old */'
    assert 'fn_00401000' in f.state['parked']


def test_failure_without_prior_source_removes_candidate(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes('bad'), ladder=['m/cheap'])
    drain(f)
    assert not (root / 'recovery/c/fn_00401000.c').exists()


def hint_fixture(value):
    return {'regs': {name: 0 for name in ('eax', 'ecx', 'edx', 'ebx', 'esp', 'ebp', 'esi', 'edi', 'eflags')},
            'stack': [value, 0], 'arena_template': 0, 'arena_writes': {}, 'globals': {}, 'import_seed': 1}


@pytest.mark.parametrize('field,value', [
    ('cases', 1),
    ('min_coverage', 0.000001),
    ('discovery', 0),
    ('original_x87', {}),
    ('lift_original_calls', {}),
    ('fixtures', [hint_fixture(0)]),
])
def test_worker_shared_hint_change_is_rejected_before_gate_and_restored(root, field, value):
    hint = root / 'recovery/hints/fn_00401000.json'
    shared = {'cases': 800, 'min_coverage': 0.7, 'discovery': 32, 'arg_count': 2,
              'fixtures': [hint_fixture(7)]}
    before = (json.dumps(shared, indent=2).replace('\n', '\r\n') + '\r\n').encode()
    hint.write_bytes(before)

    def behaviour(job, r):
        changed = json.loads(before)
        changed[field] = value
        changed['c_hints'] = {'fixtures': [hint_fixture(9)]}
        hint.write_text(json.dumps(changed))
        writes(GOOD)(job, r)

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, ladder=['m/cheap'])
    drain(f)
    assert cmre.tests == []
    assert cmre.accepted == []
    assert hint.read_bytes() == before
    assert not (root / 'recovery/c/fn_00401000.c').exists()
    assert 'hint' in f.state['parked']['fn_00401000']['detail']


@pytest.mark.parametrize('proposal', [
    {'cases': 1},
    {'min_coverage': 0.000001},
    {'discovery': 0},
    {'original_x87': {}},
    {'lift_original_calls': {}},
    {'fixtures': [{'stack': [9]}]},
])
@pytest.mark.parametrize('existing', [False, True])
def test_worker_invalid_c_hints_are_rejected_before_gate_and_restore_snapshot(root, proposal, existing):
    hint = root / 'recovery/hints/fn_00401000.json'
    before = b'{"cases": 800, "arg_count": 2}\r\n' if existing else None
    if existing:
        hint.write_bytes(before)

    def behaviour(job, r):
        changed = json.loads(before) if existing else {}
        changed['c_hints'] = proposal
        hint.write_text(json.dumps(changed))
        writes(GOOD)(job, r)

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, ladder=['m/cheap'])
    drain(f)
    assert cmre.tests == []
    assert cmre.accepted == []
    assert (hint.read_bytes() if hint.exists() else None) == before
    assert 'hint' in f.state['parked']['fn_00401000']['detail']


@pytest.mark.parametrize('replacement', [None, b'{', b'[]', b'{}'])
def test_worker_cannot_delete_corrupt_or_remove_shared_hints(root, replacement):
    hint = root / 'recovery/hints/fn_00401000.json'
    before = b'{"cases": 800, "fixtures": []}\r\n'
    hint.write_bytes(before)

    def behaviour(job, r):
        if replacement is None:
            hint.unlink()
        else:
            hint.write_bytes(replacement)
        writes(GOOD)(job, r)

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, ladder=['m/cheap'])
    drain(f)
    assert cmre.tests == []
    assert cmre.accepted == []
    assert hint.read_bytes() == before


def test_worker_hint_edit_is_reverted_on_failure_and_kept_on_success(root):
    hint = root / 'recovery/hints/fn_00401000.json'
    shared = {'cases': 800, 'min_coverage': 0.7, 'discovery': 32, 'arg_count': 2,
              'original_x87': {}, 'lift_original_calls': {}, 'fixtures': [hint_fixture(7)]}
    hint.write_text(json.dumps(shared))
    edited = {**shared, 'c_hints': {'fixtures': [hint_fixture(9)]}}
    starts = []

    def behaviour(job, r):
        starts.append(json.loads(hint.read_text()))
        hint.write_text(json.dumps(edited))
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text('bad' if job['model'] == 'm/cheap' else GOOD)

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, workers=1)
    f.tick()  # dispatch cheap
    f.tick()  # reap failure, restore, dispatch strong
    assert json.loads(hint.read_text()) in (shared, edited)
    drain(f)
    assert cmre.accepted == ['fn_00401000']
    assert starts == [shared, shared]  # failure restored the original before escalation dispatch
    assert json.loads(hint.read_text()) == edited


def test_mechanical_rejections_never_reach_the_gate(root):
    asm = GOOD.replace('return a + 1;', '__asm__("nop"); return a + 1;')
    f, cmre, _ = make(root, [item('fn_00401000')], writes(asm), ladder=['m/cheap'])
    drain(f)
    assert cmre.accepted == []
    assert all(source is not None for _, source in cmre.tests) or cmre.tests == []
    assert 'inline assembly' in json.dumps(f.state['parked'])


@pytest.mark.parametrize('text,needle', [
    ('__attribute__((optimize("O2"))) u32 cm_fn_00401000(void){return 0;}', 'optimize attribute'),
    ('u32 cm_fn_00401000(void){ register u32 x = 0; return x; }', 'register'),
    ('u32 cm_fn_00401000(void){ return CM_ORIGINAL_CALL(0); }', 'CM_ORIGINAL_CALL'),
    ('/* WIP: case 3 */ u32 cm_fn_00401000(void){return 0;}', 'WIP'),
    ('u32 cm_fn_00409999(void){return 0;}', 'entry'),
])
def test_mechanical_problems(text, needle):
    problems = farm.mechanical_problems('fn_00401000', text)
    assert any(needle in p for p in problems), problems


def test_comment_mentions_do_not_trip_checks():
    text = '/* no asm here; register pressure note */\nu32 cm_fn_00401000(void){return 0;}\n'
    assert farm.mechanical_problems('fn_00401000', text) == []


@pytest.mark.parametrize('directive', [
    '#pragma GCC diagnostic ignored "-Wreturn-type"',
    '# pragma GCC diagnostic warning "-Wreturn-type"',
    '#pragma clang diagnostic ignored "-Wreturn-type"',
    '#pragma GCC diagnostic /* reason */ ignored "-Wreturn-type"',
    '#pragma GCC diagnostic ignored \\\n "-Wreturn-type"',
    '_Pragma("GCC diagnostic ignored \\"-Wreturn-type\\"")',
    '_Pragma ( /* reason */ "GCC diagnostic warning \\"-Wreturn-type\\"" )',
    '_Pragma(L"clang diagnostic ignored \\"-Wreturn-type\\"")',
])
def test_return_type_diagnostic_weakening_rejected(directive):
    source = (directive + '\n'
              'static u32 __attribute__((noinline)) capture_eax(void) {}\n'
              'u32 cm_fn_00401000(void) { return capture_eax(); }\n')
    assert farm.mechanical_problems('fn_00401000', source) == [
        'return-type diagnostic weakening permits undefined register capture']


def test_return_type_diagnostic_rejection_never_runs_gate(root):
    source = ('#pragma GCC diagnostic ignored "-Wreturn-type"\n'
              'static u32 __attribute__((noinline)) capture_eax(void) {}\n'
              'u32 cm_fn_00401000(void) { return capture_eax(); }\n')
    f, cmre, _ = make(root, [item('fn_00401000')], writes(source), ladder=['m/cheap'])
    drain(f)
    assert cmre.tests == []
    assert cmre.accepted == []
    assert 'return-type diagnostic weakening' in json.dumps(f.state['parked'])


@pytest.mark.parametrize('decoy', [
    '/*\n#pragma GCC diagnostic ignored "-Wreturn-type"\n*/',
    '// _Pragma("GCC diagnostic ignored \\"-Wreturn-type\\"")',
    r'const char *s = "#pragma GCC diagnostic ignored \"-Wreturn-type\"";',
    r'const char *s = "_Pragma(\"GCC diagnostic ignored \\\"-Wreturn-type\\\"\")";',
    '#pragma GCC diagnostic error "-Wreturn-type"',
    '#pragma GCC diagnostic ignored "-Wunused-parameter"',
    '#pragma GCC diagnostic ignored "-Wreturn-type-extra"',
    '_Pragma("GCC diagnostic error \\"-Wreturn-type\\"")',
])
def test_return_type_checker_ignores_decoys_and_nonweakening_pragmas(decoy):
    assert farm.mechanical_problems('fn_00401000', decoy + '\n' + GOOD) == []


@pytest.mark.parametrize('abi', ['regparm(3), stdcall', 'thiscall', 'fastcall'])
def test_explicit_entry_abi_is_not_undefined_register_capture(abi):
    source = (f'u32 __attribute__(({abi})) cm_fn_00401000(u32 eax, u32 edx, u32 ecx, u32 stack) '
              '{ return eax + edx + ecx + stack; }\n')
    assert farm.mechanical_problems('fn_00401000', source) == []


def test_busy_accept_is_retried_not_failed(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD), accept_codes=[2, 0])
    drain(f)
    assert cmre.accepted == ['fn_00401000']


def test_held_items_are_retried_before_accept_and_flagged_for_audit(root):
    holds = {'fn_00401000': {'state': 'evidence-needed', 'reason': 'return width unknown'}}
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), holds=holds,
                             include_held=True, audit_workers=1)
    drain(f)
    assert 'return width unknown' in launcher.started[0][1]
    assert cmre.retried == ['fn_00401000']
    assert cmre.accepted == ['fn_00401000']
    assert f.state['accepted']['fn_00401000']['was_held'] is True
    assert f.state['audits']['fn_00401000']['verdict'] == 'ok'


def test_claim_refusal_skips_item(root):
    f, cmre, launcher = make(root, [item('fn_badbad00'), item('fn_00401000')], writes(GOOD))
    drain(f)
    assert cmre.accepted == ['fn_00401000']
    assert 'fn_badbad00' in f.state['skipped']


def test_largest_ready_first_ordering(root):
    items = [farm.Item('fn_00000001', 100, True, 0, ''), farm.Item('fn_00000002', 900, False, 2, ''),
             farm.Item('fn_00000003', 500, True, 0, '')]
    f, cmre, launcher = make(root, items, writes(GOOD), workers=1)
    f.tick()
    assert launcher.started[0][0]['fn'] == 'fn_00000003'


def test_audit_defect_dispatches_fix_with_finding(root):
    calls = []

    def behaviour(job, r):
        calls.append(job['kind'])
        if job['kind'] == 'audit':
            verdict = 'defect' if calls.count('audit') == 1 else 'ok'
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).write_text(json.dumps(
                {'verdict': verdict, 'findings': [{'line': 3, 'detail': 'byte counter widened', 'evidence': '0x401010'}]}))
            return
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD if job['kind'] == 'translate' else GOOD.replace('a + 1', 'a + 1 /*PASS fixed*/'))

    f, cmre, launcher = make(root, [item('fn_00401000')], behaviour, audit_workers=1)
    drain(f, 40)
    fix_prompts = [p for job, p, _ in launcher.started if job['kind'] == 'fix']
    assert fix_prompts and 'byte counter widened' in fix_prompts[0]
    assert cmre.accepted == ['fn_00401000', 'fn_00401000']
    assert 'PASS fixed' in (root / 'recovery/c/fn_00401000.c').read_text()
    assert f.state['audits']['fn_00401000']['verdict'] == 'ok'


def test_unfixed_defect_restores_accepted_source_and_is_reported(root):
    def behaviour(job, r):
        if job['kind'] == 'audit':
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': [{'detail': 'x'}]}))
            return
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD if job['kind'] == 'translate' else 'broken')

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, audit_workers=1)
    drain(f, 40)
    assert (root / 'recovery/c/fn_00401000.c').read_text() == GOOD
    assert 'fn_00401000' in f.summary()['open_defects']


def test_timeout_kills_worker_and_counts_failure(root):
    now = [0.0]

    class Slow(FakeHandle):
        def poll(self):
            return None

    class SlowLauncher(FakeLauncher):
        def start(self, job, prompt, config):
            h = Slow(job, None, self.root)
            self.started.append((job, prompt, config))
            self.handles.append(h)
            return h

    cmre = FakeCmre([item('fn_00401000')])
    cmre.root = root
    launcher = SlowLauncher(root, None)
    launcher.handles = []
    cfg = farm.FarmConfig(workers=1, ladder=['m/cheap'], audit_workers=0, audit_model=None, attempt_minutes=1,
                          hours=1, min_bytes=0, max_bytes=None, include_held=False, owner='farm-t')
    f = farm.Farm(root / 'run', root, cmre, launcher, cfg, clock=lambda: now[0])
    f.tick()
    now[0] = 120.0
    f.tick()
    assert launcher.handles[0].killed
    assert 'timeout' in json.dumps(f.state['parked'])


def test_deadline_stops_dispatch_and_stop_file_drains(root):
    f, cmre, launcher = make(root, [item('fn_00401000'), item('fn_00401100')], writes(GOOD), workers=1)
    (f.run_dir / 'STOP').write_text('')
    drain(f)
    assert launcher.started == []
    assert f.finished()


def test_worker_config_limits_edits_and_commands(root):
    job = {'fn': 'fn_00401000', 'kind': 'translate', 'source': str(root / 'run/src/fn_00401000.c')}
    cfg = farm.worker_config(root, job)
    perm = cfg['permission']
    assert perm['edit']['*'] == 'deny'
    assert any(k.endswith('run/src/fn_00401000.c') and v == 'allow' for k, v in perm['edit'].items())
    assert perm['edit']['recovery/hints/fn_00401000.json'] == 'allow'
    assert perm['bash']['*'] == 'deny'
    assert perm['bash']['py -3.13 -m cmre test *'] == 'allow'
    assert 'py -3.13 -m cmre accept *' not in perm['bash']
    assert cfg['tools']['task'] is False


def test_audit_config_cannot_edit_sources(root):
    job = {'fn': 'fn_00401000', 'kind': 'audit', 'audit_path': str(root / 'run/audit/fn_00401000.json')}
    perm = farm.worker_config(root, job)['permission']
    assert 'recovery/hints/fn_00401000.json' not in perm['edit']
    assert any(k.endswith('run/audit/fn_00401000.json') for k in perm['edit'])


def test_state_survives_restart(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD))
    drain(f)
    again, _, _ = make(root, [], writes(GOOD))
    assert again.summary()['accepted_bytes'] == 200


def test_real_adapter_maps_cmre_queue_items(monkeypatch):
    import types
    from cmre import queue
    real = queue.Item(0x401000, 'FUN_00401000', 321, False, 2, 'mod.cpp')
    monkeypatch.setattr(queue, 'c_frontier', lambda *a, **k: [real])
    got = farm.CmreAdapter().frontier(5, 0, None, False, 'farm-t')
    assert got == [farm.Item('fn_00401000', 321, False, 2, 'mod.cpp')]


def test_status_command_reads_state_without_inventing_a_deadline(root, capsys):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD))
    drain(f)
    assert farm.main(['status', str(f.run_dir)]) == 0
    out = capsys.readouterr().out
    assert 'accepted 1 fn' in out and 'STOPPING' not in out
    (f.run_dir / 'STOP').write_text('')
    farm.main(['status', str(f.run_dir)])
    assert 'STOPPING' in capsys.readouterr().out


def test_status_line_reports_rate_and_models(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD))
    drain(f)
    line = farm.status_line(f.summary())
    assert 'accepted 1 fn' in line and '200 B' in line and 'm/cheap' in line


def test_translate_prompt_bounds_exploration(root):
    job = {'fn': 'fn_00401000', 'kind': 'translate', 'bytes': 300, 'source': str(root / 'run/src/fn_00401000.c')}
    prompt = farm.translate_prompt(root, job)
    assert 'recovery/include/cmre.h' in prompt
    assert 'Do not search the repository' in prompt
    assert 'first complete version' in prompt


def test_worker_shell_allowlist_excludes_unbounded_recursive_search(root):
    job = {'fn': 'fn_00401000', 'kind': 'translate', 'source': str(root / 'run/src/fn_00401000.c')}
    bash = farm.worker_config(root, job)['permission']['bash']
    assert bash.get('rg *') == 'allow'
    assert 'grep *' not in bash and 'ls *' not in bash


class ScriptedHandle:
    def __init__(self, code=None, infra=None, activity=0.0):
        self.code, self.infra, self.activity, self.killed = code, infra, activity, False

    def poll(self):
        return self.code

    def kill(self):
        self.killed = True

    def infra_error(self):
        return self.infra

    def last_activity(self):
        return self.activity


class ScriptedLauncher:
    def __init__(self, plan):
        self.plan, self.started, self.handles = plan, [], []

    def start(self, job, prompt, config):
        self.started.append((job, prompt, config))
        h = self.plan(job)
        self.handles.append(h)
        return h


def scripted_farm(root, plan, clock, **cfg):
    cmre = FakeCmre([item('fn_00401000')])
    cmre.root = root
    launcher = ScriptedLauncher(plan)
    options = dict(workers=1, ladder=['m/cheap', 'm/strong'], audit_workers=0, audit_model=None,
                   attempt_minutes=30, hours=1, min_bytes=0, max_bytes=None, include_held=False,
                   owner='farm-t', stall_minutes=5)
    options.update(cfg)
    return farm.Farm(root / 'run', root, cmre, launcher, farm.FarmConfig(**options), clock=clock), cmre, launcher


def test_auth_error_marks_model_unavailable_without_spending_an_attempt(root):
    def plan(job):
        if job['model'] == 'm/cheap':
            return ScriptedHandle(code=1, infra='401 This API key has expired')
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD)
        return ScriptedHandle(code=0)

    f, cmre, launcher = scripted_farm(root, plan, lambda: 0.0)
    drain(f)
    assert [j['model'] for j, _, _ in launcher.started] == ['m/cheap', 'm/strong']
    assert cmre.accepted == ['fn_00401000']
    assert 'm/cheap' in f.summary()['unavailable_models']
    assert all(r['model'] != 'm/cheap' for r in f.state['attempts']['fn_00401000'])
    # A later item never goes to the unavailable model.
    assert farm.status_line(f.summary()).count('UNAVAILABLE') == 1


def test_stalled_worker_is_killed_before_the_attempt_timeout(root):
    now = [0.0]
    f, cmre, launcher = scripted_farm(root, lambda job: ScriptedHandle(code=None, activity=0.0),
                                      lambda: now[0], ladder=['m/cheap'])
    f.tick()
    now[0] = 4 * 60
    f.tick()
    assert not launcher.handles[0].killed
    now[0] = 6 * 60
    f.tick()
    assert launcher.handles[0].killed
    assert 'stalled' in json.dumps(f.state['parked'])


def test_probe_reports_each_model(monkeypatch, capsys):
    import subprocess as sp

    def fake_run(command, **kwargs):
        flag = '-m' if '-m' in command else '--model'
        model = command[command.index(flag) + 1]
        out = ('{"type":"text","part":{"text":"OK"}}' if model == 'm/ok' else
               '{"type":"error","error":{"name":"APIError","data":{"message":"This API key has expired"}}}')
        return sp.CompletedProcess(command, 0, stdout=out, stderr='')

    monkeypatch.setattr(farm.subprocess, 'run', fake_run)
    assert farm.main(['probe', '--ladder', 'm/ok,m/dead']) == 1
    out = capsys.readouterr().out
    assert 'm/ok: OK' in out and 'm/dead: UNAVAILABLE This API key has expired' in out


def test_status_counts_running_workers_from_events(root, capsys):
    f, cmre, launcher = make(root, [item('fn_00401000'), item('fn_00401100')], writes(GOOD), workers=2)
    f._dispatch()
    f.save()
    farm.main(['status', str(f.run_dir)])
    assert 'running 2' in capsys.readouterr().out


def test_worker_config_sets_explicit_reasoning_effort_for_the_job_model(root):
    job = {'fn': 'fn_00401000', 'kind': 'translate', 'model': 'prov/model-x',
           'source': str(root / 'run/src/fn_00401000.c')}
    cfg = farm.worker_config(root, job, effort='low')
    assert cfg['provider'] == {'prov': {'models': {'model-x': {'options': {'effort': 'low', 'reasoningEffort': 'low'}}}}}
    assert 'provider' not in farm.worker_config(root, job, effort=None)


def test_output_cap_is_reported_distinctly(root):
    def plan(job):
        h = ScriptedHandle(code=0)
        h.finish_reason = lambda: 'length'
        return h

    f, cmre, launcher = scripted_farm(root, plan, lambda: 0.0, ladder=['m/cheap'])
    drain(f)
    assert 'output cap' in json.dumps(f.state['parked'])


def test_farm_exports_its_claim_owner_for_workers_and_retests(root, monkeypatch):
    # cmre fences --source diagnostics on a claimed function to the claim owner only.
    monkeypatch.delenv('CMRE_CLAIM_OWNER', raising=False)
    f, cmre, launcher = scripted_farm(root, lambda job: ScriptedHandle(code=0), lambda: 0.0)
    assert os.environ['CMRE_CLAIM_OWNER'] == 'farm-t'


def test_audit_is_bounded_to_brief_source_and_checklist(root):
    # 2026-11-13 smoke: mutate outlived opencode's 120 s shell timeout and the auditor
    # then wandered through run directories, the ledger and its own transcript.
    job = {'fn': 'fn_00401000', 'kind': 'audit', 'model': 'm/x',
           'audit_path': str(root / 'run/audit/fn_00401000.json')}
    prompt = farm.audit_prompt(root, job)
    assert 'mutate' not in prompt
    assert 'Do not read farm run files' in prompt
    assert 'at most 20 tool calls' in prompt  # an unbounded audit wrote no verdict in 19 min
    bash = farm.worker_config(root, job)['permission']['bash']
    assert not any('mutate' in k for k in bash)


def test_errored_audit_is_retried_once_and_reported(root):
    # 2026-11-13 smoke: an auditor wrote no verdict; the error vanished from status.
    audits = []

    def behaviour(job, r):
        if job['kind'] == 'audit':
            audits.append(job['fn'])
            return  # never writes the verdict file
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD)

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, audit_workers=1)
    drain(f, 40)
    assert audits == ['fn_00401000', 'fn_00401000']
    assert f.summary()['audit_errors'] == 1
    assert 'error 1' in farm.status_line(f.summary())


def test_unaudited_accepts_are_requeued_after_restart(root):
    f, cmre, _ = make(root, [item('fn_00401000')], writes(GOOD))  # audit_workers=0: accepted, never audited
    drain(f)
    audits = []

    def behaviour(job, r):
        audits.append(job['fn'])
        Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['audit_path']).write_text(json.dumps({'verdict': 'ok', 'findings': []}))

    again, _, _ = make(root, [], behaviour, audit_workers=1)
    drain(again)
    assert audits == ['fn_00401000']


def test_include_held_dispatches_unheld_items_first(root):
    # An unattended run drains the ready queue before spending attempts on held functions.
    items = [farm.Item('fn_00000001', 900, True, 0, ''), farm.Item('fn_00000002', 100, True, 0, '')]
    holds = {'fn_00000001': {'state': 'harness-blocker', 'reason': 'x87'}}
    f, cmre, launcher = make(root, items, writes(GOOD), holds=holds, include_held=True, workers=1)
    f.tick()
    assert launcher.started[0][0]['fn'] == 'fn_00000002'


def test_frontier_request_covers_the_whole_queue(root):
    # cmre orders its frontier smallest-first; a 500 limit hid most held bytes (691 items on 2026-11-13).
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD))
    limits = []
    real = cmre.frontier
    cmre.frontier = lambda limit, *a: limits.append(limit) or real(limit, *a)
    f.tick()
    assert limits and limits[0] >= 5000


def _job(root, kind='translate'):
    job = {'fn': 'fn_0000beef', 'kind': kind, 'model': 'opencode-go/deepseek-v4.1-flash', 'n': 7}
    if kind == 'audit':
        job['audit_path'] = str(root / 'CM0102/Analysis/cmre/farm/r/audit/fn_0000beef.json')
    else:
        job['source'] = str(root / 'CM0102/Analysis/cmre/farm/r/src/fn_0000beef.c')
    return job


def test_v2_agent_scopes_edit_to_own_files_and_shell_to_the_allowlist(root):
    # OpenCode 2.x takes permissions from an agent definition (verified 2026-11-13:
    # exact relative edit paths and shell globs are enforced; the rest is denied).
    text = farm.v2_agent(root, _job(root))
    assert text.startswith('---\n') and 'mode: primary' in text
    assert '{ action: "*", resource: "*", effect: deny }' in text
    assert '{ action: "edit", resource: "CM0102/Analysis/cmre/farm/r/src/fn_0000beef.c", effect: allow }' in text
    assert '{ action: "edit", resource: "recovery/hints/fn_0000beef.json", effect: allow }' in text
    assert '{ action: "shell", resource: "py -3.13 -m cmre test *", effect: allow }' in text
    assert '{ action: "shell", resource: "rg *", effect: allow }' in text
    assert 'grep *' not in text and 'ls *' not in text and 'mutate' not in text
    audit = farm.v2_agent(root, _job(root, 'audit'))
    assert 'recovery/hints' not in audit and 'audit/fn_0000beef.json' in audit


def test_v2_launcher_runs_the_job_agent_and_removes_it_when_done(root, monkeypatch):
    started = {}

    class Proc:
        pid, code = 1, None

        def poll(self):
            return self.code

    proc = Proc()

    def popen(command, **kw):
        started.update(command=command, cwd=kw['cwd'], env=kw['env'])
        return proc

    monkeypatch.setattr(farm.subprocess, 'Popen', popen)
    monkeypatch.setattr(farm.subprocess, 'run',
                        lambda command, **kw: farm.subprocess.CompletedProcess(command, 0))  # agent visible
    launcher = farm.OpencodeV2Launcher(root, root / 'run', executable='oc2')
    handle = launcher.start(_job(root), 'PROMPT', {})
    cmd = started['command']
    name = cmd[cmd.index('--agent') + 1]
    agent = root / '.opencode/agents' / f'{name}.md'
    # Shared background service: concurrent --standalone servers hung at startup and mid-step
    # (2026-11-13: 8-minute stalls on both models; the same probe took 5 s on the service).
    assert cmd[:2] == ['oc2', 'run'] and '--standalone' not in cmd and '--pure' not in cmd
    assert cmd[cmd.index('-m') + 1] == 'opencode-go/deepseek-v4.1-flash' and cmd[-1] == 'PROMPT'
    assert started['cwd'] == root and agent.exists()
    assert handle.poll() is None and agent.exists()
    proc.code = 0
    assert handle.poll() == 0 and not agent.exists()


def test_v1_launcher_raises_the_output_cap(root, monkeypatch):
    # opencode 1.18 caps output at 32k unless OPENCODE_EXPERIMENTAL_OUTPUT_TOKEN_MAX is set;
    # 5 of 8 parked production jobs hit that cap.
    seen = {}
    monkeypatch.setattr(farm.subprocess, 'Popen', lambda command, **kw: seen.update(kw) or object())
    farm.OpencodeLauncher(root, root / 'run').start(_job(root), 'P', {})
    assert int(seen['env']['OPENCODE_EXPERIMENTAL_OUTPUT_TOKEN_MAX']) >= 131072


def test_cli_selects_the_opencode_build(root):
    assert isinstance(farm.make_launcher('v2', root, root / 'run'), farm.OpencodeV2Launcher)
    assert isinstance(farm.make_launcher('v1', root, root / 'run'), farm.OpencodeLauncher)
    v2 = farm.probe_command('v2', 'm/x')
    assert v2[1:3] == ['run', '-m'] and '--standalone' not in v2 and v2[-1] == 'Reply with exactly: OK'
    assert farm.probe_command('v1', 'm/x')[:3] == ['opencode', '--pure', 'run']


def test_claim_race_with_another_farm_is_a_refusal_not_a_crash(monkeypatch):
    # 2026-11-13: two farms claimed one function at once; cmre's attempt fence raised
    # BusyError, which killed the second farm before its first dispatch.
    from cmre import coordination, work

    def busy(*a, **k):
        raise coordination.BusyError('Writer active: attempt-fences/fn_0056c570.lock')

    monkeypatch.setattr(work, 'claim', busy)
    with pytest.raises(ValueError, match='Writer active'):
        farm.CmreAdapter().claim('fn_0056c570', 'farm-t', 1)


def test_release_survives_a_busy_work_lock(monkeypatch):
    # Claims expire on their own; a busy queue lock must not crash the farm.
    from cmre import coordination, work
    calls = []

    def busy(owner, vas=None):
        calls.append(owner)
        raise coordination.BusyError('Writer active: work.lock')

    monkeypatch.setattr(work, 'release', busy)
    monkeypatch.setattr(farm.time, 'sleep', lambda s: None)
    farm.CmreAdapter().release('farm-t', ['fn_00401000'])
    assert len(calls) == 3


def test_v2_kill_interrupts_the_session_in_the_shared_service(root, monkeypatch):
    # Killing only the run client leaves the session running in the service.
    log = root / 'w.jsonl'
    log.write_text('{"type":"step_start","sessionID":"ses_abc123","part":{}}\n', encoding='utf-8')
    agent = root / 'agent.md'
    agent.write_text('x')
    calls = []
    monkeypatch.setattr(farm.subprocess, 'run', lambda command, **kw: calls.append(command))

    class Proc:
        pid = 42

        def poll(self):
            return None

    handle = farm.OpencodeV2Handle(Proc(), log, agent, executable='oc2')
    handle.kill()
    assert calls[0] == ['oc2', 'api', 'session.interrupt', '--param', 'sessionID=ses_abc123']
    assert not agent.exists()


def test_v2_launcher_waits_until_the_service_sees_the_new_agent(root, monkeypatch):
    # 2026-11-13: a run launched right after its agent file was written failed with
    # 'Agent not found'; the shared service picks new agent files up after a short delay.
    checks, order = [], []

    def run(command, **kw):
        checks.append(command)
        order.append('check')
        return farm.subprocess.CompletedProcess(command, 0 if len(checks) >= 3 else 1, stdout='', stderr='')

    class Proc:
        pid = 1

        def poll(self):
            return None

    monkeypatch.setattr(farm.subprocess, 'run', run)
    monkeypatch.setattr(farm.subprocess, 'Popen', lambda command, **kw: order.append('launch') or Proc())
    monkeypatch.setattr(farm.time, 'sleep', lambda s: None)
    farm.OpencodeV2Launcher(root, root / 'run', executable='oc2').start(_job(root), 'P', {})
    assert order == ['check', 'check', 'check', 'launch']
    assert checks[0][:3] == ['oc2', 'api', 'agent.get']
    assert f'location[directory]={root}' in checks[0]


def test_default_stall_limit_allows_a_long_silent_thinking_step():
    # opencode 2.x emits nothing while a step thinks; DeepSeek spent 178 s and 20k reasoning
    # tokens in one step, and 8 minutes killed legitimate steps (2026-11-13).
    assert farm.DEFAULT_STALL_MINUTES >= 15


# --- 2026-11-13 evening: throughput and the OpenCode coordinator -------------------

def audit_writer(verdicts):
    """Audit/confirm behaviour from a list of (verdict, findings), consumed in order; sources pass."""
    seen = []

    def behaviour(job, r):
        if job['kind'] == 'audit':
            seen.append(job)
            verdict, findings = verdicts.pop(0) if verdicts else ('ok', [])
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).write_text(json.dumps({'verdict': verdict, 'findings': findings}))
            return
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD if job['kind'] == 'translate' else GOOD.replace('a + 1', 'a + 1 /*PASS*/'))
    behaviour.seen = seen
    return behaviour


A = {'line': 2, 'detail': 'cmp si, cx treated as 32-bit', 'evidence': '0x401004'}
B = {'line': 3, 'detail': 'EAX return dropped', 'evidence': '0x401010 mov eax, ecx'}


def test_x87_runtime_helpers_are_skipped_by_default(root):
    # 7 of 10 production audit defects came from 0x0094xxxx-0x00952xxx runtime helpers whose
    # x87 control-word and caller-stack effects the gate cannot see (2026-11-13).
    assert any(lo <= 0x0094bc15 < hi and lo <= 0x00952f05 < hi for lo, hi in farm.DEFAULT_SKIP_RANGES)
    items = [item('fn_0094bc15', 900), item('fn_00401000', 100)]
    f, cmre, launcher = make(root, items, writes(GOOD), workers=2, skip_ranges=farm.DEFAULT_SKIP_RANGES)
    f.tick()
    assert [j['fn'] for j, _, _ in launcher.started] == ['fn_00401000']
    assert farm.parse_ranges('0x00940000-0x00953000') == [(0x00940000, 0x00953000)]
    assert farm.parse_ranges('') == []


def _docs(root):
    (root / 'docs/agent').mkdir(parents=True, exist_ok=True)
    (root / 'docs/agent/C.md').write_text(
        '# C\n## Loop\n1. brief\n2. Write `recovery/c/fn_X.c`. Rules:\n   - RULE-CMD\n3. `py -3.13 -m cmre test`\n')
    (root / 'docs/agent/FARM.md').write_text(
        '# Farm\n## Worker rules\n- WORKER-RULE\n## Audit checklist\n1. CHECK-ITEM\n## Supply lane\nSUPPLY\n')
    (root / 'recovery/include').mkdir(parents=True, exist_ok=True)
    (root / 'recovery/include/cmre.h').write_text('#define HEADER-TEXT\n')
    (root / 'drafts').mkdir(exist_ok=True)
    (root / 'drafts/fn_00401000.c').write_text('DRAFT-TEXT\n')


def test_translate_job_gets_one_bundle_with_brief_draft_header_and_rules(root):
    # Workers spent 5-8 of ~20 steps re-reading the brief, draft, header and rules (2026-11-13).
    _docs(root)
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), workers=1)
    cmre.brief = lambda fn: f'BRIEF {fn}\ndraft: drafts/{fn}.c  NEEDS-REVIEW\nlisting (VA, instruction):\n'
    f._dispatch()
    job, prompt, _ = launcher.started[0]
    text = (root / 'run/bundle/fn_00401000.md').read_text()
    for needle in ('BRIEF fn_00401000', 'DRAFT-TEXT', 'HEADER-TEXT', 'RULE-CMD', 'WORKER-RULE'):
        assert needle in text
    assert 'SUPPLY' not in text and 'CHECK-ITEM' not in text
    assert 'run/bundle/fn_00401000.md' in prompt
    assert f'cmre brief fn_00401000' not in prompt


def test_audit_bundle_carries_the_checklist_and_accepted_source(root):
    _docs(root)
    (root / 'recovery/c/fn_00401000.c').write_text(GOOD)
    job = {'fn': 'fn_00401000', 'kind': 'audit', 'model': 'm/x', 'n': 1,
           'audit_path': str(root / 'run/audit/fn_00401000.json')}
    text = farm.bundle_text(root, job, 'BRIEF fn_00401000\n')
    assert 'CHECK-ITEM' in text and 'cm_fn_00401000' in text and 'RULE-CMD' not in text


def test_bundle_survives_a_failed_brief(root):
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), workers=1)

    def broken(fn):
        raise RuntimeError('brief crashed')
    cmre.brief = broken
    f._dispatch()
    assert 'cmre brief fn_00401000' in launcher.started[0][1]


def test_a_defect_needs_a_second_auditor_before_any_fix(root):
    # MiniMax M3 flagged 8 of 8 functions including false positives (xaudit-mm-on-ds, 2026-11-13).
    behaviour = audit_writer([('defect', [A]), ('ok', [])])
    f, cmre, launcher = make(root, [item('fn_00401000')], behaviour, audit_workers=1, confirm_model='m/judge')
    drain(f, 40)
    assert 'fix' not in [j['kind'] for j, _, _ in launcher.started]
    confirm = behaviour.seen[1]
    assert confirm['model'] == 'm/judge' and confirm.get('confirm')
    confirm_prompt = [p for j, p, _ in launcher.started if j.get('confirm')][0]
    assert 'cmp si, cx treated as 32-bit' in confirm_prompt and 'false positive' in confirm_prompt
    audit = f.state['audits']['fn_00401000']
    assert audit['verdict'] == 'ok' and audit['disputed'] == [A]
    assert f.summary()['audits_disputed'] == 1 and not f.summary()['open_defects']


def test_a_confirmed_defect_fixes_only_the_confirmed_findings(root):
    behaviour = audit_writer([('defect', [A, B]), ('defect', [B]), ('ok', [])])
    f, cmre, launcher = make(root, [item('fn_00401000')], behaviour, audit_workers=1, confirm_model='m/judge')
    drain(f, 60)
    fix_prompt = [p for j, p, _ in launcher.started if j['kind'] == 'fix'][0]
    assert 'EAX return dropped' in fix_prompt and 'cmp si, cx' not in fix_prompt
    assert cmre.accepted == ['fn_00401000', 'fn_00401000']


def test_unconfirmed_defects_from_earlier_runs_are_confirmed_on_restart(root):
    run = root / 'run'
    run.mkdir()
    (run / 'state.json').write_text(json.dumps({
        'schema': 'cmre-farm/1', 'started_at': 0, 'owner': 'farm-t', 'attempts': {},
        'accepted': {'fn_00401000': {'bytes': 200, 'model': 'm/x', 'kind': 'translate', 'at': 0}},
        'parked': {}, 'skipped': {}, 'defects': {}, 'fix_rounds': {}, 'unavailable_models': {},
        'audits': {'fn_00401000': {'rounds': 1, 'verdict': 'unconfirmed', 'findings': [A], 'model': 'm/x'}}}))
    behaviour = audit_writer([('ok', [])])
    f, cmre, launcher = make(root, [], behaviour, audit_workers=1, confirm_model='m/judge')
    drain(f)
    assert [j.get('confirm') for j in behaviour.seen] == [True]


class ManualExecutor:
    """Runs submitted verification only when told to."""

    def __init__(self):
        self.tasks = []

    def submit(self, fn, *args):
        from concurrent.futures import Future
        future = Future()
        self.tasks.append((future, fn, args))
        return future

    def run_all(self):
        for future, fn, args in self.tasks:
            future.set_result(fn(*args))
        self.tasks = []


def test_verification_runs_off_the_main_loop(root):
    # Gate re-tests took a median 48 s per job (smoke, 2026-11-13); run inline they block every
    # dispatch, which caps the farm near 20 workers.
    cmre = FakeCmre([item('fn_00401000'), item('fn_00401100')])
    cmre.root = root
    launcher = FakeLauncher(root, writes(GOOD))
    executor = ManualExecutor()
    cfg = farm.FarmConfig(workers=1, ladder=['m/cheap'], audit_workers=0, audit_model=None, attempt_minutes=30,
                          hours=1, min_bytes=0, max_bytes=None, include_held=False, owner='farm-t')
    f = farm.Farm(root / 'run', root, cmre, launcher, cfg, clock=lambda: 0.0, verifier=executor)
    f.tick()            # dispatch fn_00401000
    f.tick()            # it exits: verification submitted, slot freed, next item dispatched
    assert executor.tasks and cmre.accepted == []
    assert [j['fn'] for j, _, _ in launcher.started] == ['fn_00401000', 'fn_00401100']
    assert not f.finished()
    executor.run_all()
    f.tick()
    assert cmre.accepted == ['fn_00401000']


def events(run):
    path = Path(run) / 'events.jsonl'
    return [json.loads(line) for line in path.read_text().splitlines()] if path.exists() else []


def test_run_records_start_and_exit_and_supply_exhaustion(root, monkeypatch):
    monkeypatch.setattr(farm.time, 'sleep', lambda s: None)
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), workers=1)
    f.run(poll=0)
    names = [e['event'] for e in events(f.run_dir)]
    assert names[0] == 'farm-start' and names[-1] == 'farm-exit'
    assert names.count('supply-empty') == 1


def test_an_unfixable_defect_raises_an_open_defect_event(root):
    def behaviour(job, r):
        if job['kind'] == 'audit':
            Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
            Path(job['audit_path']).write_text(json.dumps({'verdict': 'defect', 'findings': [B]}))
            return
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD if job['kind'] == 'translate' else 'broken')

    f, cmre, _ = make(root, [item('fn_00401000')], behaviour, audit_workers=1)
    drain(f, 40)
    assert [e['fn'] for e in events(f.run_dir) if e['event'] == 'open-defect'] == ['fn_00401000']


def test_wait_returns_on_a_notable_event_and_ignores_older_ones(root):
    f, cmre, _ = make(root, [], writes(GOOD))
    f._event('open-defect', fn='fn_00000001')
    f.save()
    now = [0.0]

    def sleep(seconds):
        now[0] += seconds
        if now[0] == 30:
            f._event('accepted', fn='fn_00000002', bytes=1, model='m', kind='translate')
            f._event('open-defect', fn='fn_00000003')
        (f.run_dir / 'state.json').touch()

    reason, seen = farm.wait_for(f.run_dir, minutes=10, poll=15, clock=lambda: now[0], sleep=sleep)
    assert reason == 'event' and [e['fn'] for e in seen] == ['fn_00000003']


def test_wait_times_out_and_detects_a_dead_farm(root):
    f, cmre, _ = make(root, [], writes(GOOD))
    f.save()
    now = [0.0]

    def sleep(seconds):
        now[0] += seconds
        (f.run_dir / 'state.json').touch()

    assert farm.wait_for(f.run_dir, minutes=1, poll=15, clock=lambda: now[0], sleep=sleep)[0] == 'timeout'
    old = time.time() - 3600
    os.utime(f.run_dir / 'state.json', (old, old))
    reason, _ = farm.wait_for(f.run_dir, minutes=1, poll=15, clock=lambda: 0.0, sleep=lambda s: None)
    assert reason == 'farm not running'


def test_start_detaches_the_farm_and_records_its_pid(root, monkeypatch):
    seen = {}

    class Proc:
        pid = 4242

    def popen(command, **kw):
        seen['command'], seen['kw'] = command, kw
        return Proc()

    monkeypatch.setattr(farm.subprocess, 'Popen', popen)
    run = root / 'run'
    assert farm.main(['start', str(run), '--workers', '3', '--include-held']) == 0
    command = seen['command']
    assert command[command.index('run') + 1:] == [str(run), '--workers', '3', '--include-held']
    assert (run / 'farm.pid').read_text().strip() == '4242'
    assert seen['kw']['stdin'] is farm.subprocess.DEVNULL
    if os.name == 'nt':
        assert seen["kw"]["creationflags"] & farm.subprocess.CREATE_NO_WINDOW  # a detached farm pops a console per child


def test_adopt_request_accepts_a_hand_fix_and_reaudits_it(root):
    f, cmre, launcher = make(root, [], audit_writer([('ok', [])]), audit_workers=1, workers=0)
    f.state['accepted']['fn_00401000'] = {'bytes': 200, 'model': 'm/x', 'kind': 'translate', 'at': 0}
    f.state['audits']['fn_00401000'] = {'rounds': 1, 'verdict': 'defect', 'findings': [B]}
    f.state['defects']['fn_00401000'] = [B]
    (root / 'recovery/c/fn_00401000.c').write_text(GOOD)
    f.save()
    hand = root / 'hand.c'
    hand.write_text(GOOD.replace('a + 1', 'a + 1 /*PASS by hand*/'))
    assert farm.main(['adopt', str(f.run_dir), 'fn_00401000', '--source', str(hand), '--note', 'restored EAX']) == 0
    drain(f, 10)
    assert cmre.accepted == ['fn_00401000']
    assert 'PASS by hand' in (root / 'recovery/c/fn_00401000.c').read_text()
    assert 'fn_00401000' not in f.state['defects']
    assert f.state['audits']['fn_00401000']['verdict'] == 'ok'   # re-audited after the adopt
    assert not list((f.run_dir / 'requests').glob('*.json'))


def test_failed_adopt_keeps_the_defect_and_the_accepted_source(root):
    f, cmre, launcher = make(root, [], writes(GOOD), workers=0)
    f.state['accepted']['fn_00401000'] = {'bytes': 200, 'model': 'm/x', 'kind': 'translate', 'at': 0}
    f.state['defects']['fn_00401000'] = [B]
    (root / 'recovery/c/fn_00401000.c').write_text(GOOD)
    (root / 'bad.c').write_text('#include "cmre.h"\nu32 cm_fn_00401000(u32 a) { return a; }\n')
    f.save()   # a live farm: fresh state, no exit event
    farm.main(['adopt', str(f.run_dir), 'fn_00401000', '--source', str(root / 'bad.c')])
    drain(f, 5)
    assert (root / 'recovery/c/fn_00401000.c').read_text() == GOOD
    assert 'fn_00401000' in f.state['defects'] and cmre.accepted == []
    assert any(e['event'] == 'request-failed' for e in events(f.run_dir))


def test_dismiss_request_closes_a_false_positive_with_its_reason(root):
    f, cmre, launcher = make(root, [], writes(GOOD), workers=0)
    f.state['accepted']['fn_00401000'] = {'bytes': 200, 'model': 'm/x', 'kind': 'translate', 'at': 0}
    f.state['audits']['fn_00401000'] = {'rounds': 1, 'verdict': 'defect', 'findings': [A]}
    f.state['defects']['fn_00401000'] = [A]
    f.save()
    farm.main(['dismiss', str(f.run_dir), 'fn_00401000', '--reason', 'cmp si, cx is a 16-bit compare'])
    f.tick()
    audit = f.state['audits']['fn_00401000']
    assert audit['verdict'] == 'ok' and audit['dismissed'] == 'cmp si, cx is a 16-bit compare'
    assert 'fn_00401000' not in f.state['defects']


def test_hand_test_runs_as_the_farm_claim_owner(root, monkeypatch):
    # The defect subagent runs in the shared OpenCode service, which does not inherit the
    # coordinator shell's CMRE_CLAIM_OWNER; the farm still holds the claim.
    f, cmre, _ = make(root, [], writes(GOOD))
    f.save()
    seen = {}
    monkeypatch.setattr(farm.subprocess, 'call',
                        lambda command, **kw: seen.update(command=command, env=kw['env']) or 0)
    assert farm.main(['test', str(f.run_dir), 'fn_00401000', 'some/path.c']) == 0
    assert seen['env']['CMRE_CLAIM_OWNER'] == 'farm-t'
    assert seen['command'][-5:] == ['test', 'fn_00401000', '--c', '--source', 'some/path.c']


def test_workers_zero_run_ends_when_nothing_is_left(root, monkeypatch):
    monkeypatch.setattr(farm.time, 'sleep', lambda s: None)
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(GOOD), workers=0)
    f.run(poll=0)
    assert launcher.started == []


def test_install_agents_writes_the_coordinator_and_defect_agents(root):
    paths = farm.install_agents(root)
    coordinator = (root / '.opencode/agents/cm-coordinator.md').read_text()
    defect = (root / '.opencode/agents/cm-defect.md').read_text()
    assert sorted(p.name for p in paths) == ['cm-coordinator.md', 'cm-defect.md']
    assert 'mode: primary' in coordinator and 'docs/agent/FARM.md' in coordinator
    assert '{ action: "subagent", resource: "cm-defect", effect: allow }' in coordinator
    assert '{ action: "shell", resource: "py -3.13 scripts/cmre_farm.py *", effect: allow }' in coordinator
    assert '{ action: "*", resource: "*", effect: ask }' in coordinator
    assert 'mode: subagent' in defect
    assert '{ action: "*", resource: "*", effect: deny }' in defect
    assert '{ action: "edit", resource: "CM0102/Analysis/cmre/farm/*/hand/*.c", effect: allow }' in defect
    assert '{ action: "shell", resource: "py -3.13 scripts/cmre_farm.py test *", effect: allow }' in defect
    assert not any('accept' in line for line in defect.splitlines() if 'shell' in line)


def test_summary_counts_audit_records_that_have_no_verdict_yet(root):
    # An adopted hand fix resets its audit to {'rounds': 0} until the re-audit; status crashed on it.
    f, cmre, _ = make(root, [], writes(GOOD), workers=0)
    f.state['accepted']['fn_00401000'] = {'bytes': 200, 'model': 'hand', 'kind': 'adopt', 'at': 0}
    f.state['audits']['fn_00401000'] = {'rounds': 0, 'adopted': 'restored EAX'}
    assert f.summary()['audits_ok'] == 0
    assert 'audits ok 0' in farm.status_line(f.summary())


def test_model_pass_rate_counts_translation_attempts_not_audits(root):
    # A model that is both an auditor and a ladder rung looked like 0/5 from its audit verdicts,
    # and the "drop under 20%" rule would have removed it (2026-11-13 backlog run).
    behaviour = audit_writer([('defect', [A]), ('ok', [])])
    f, cmre, launcher = make(root, [item('fn_00401000')], behaviour, audit_workers=1, confirm_model='m/strong')
    drain(f, 40)
    assert f.summary()['by_model'] == {'m/cheap': {'attempts': 1, 'passes': 1}}


# --- provider usage limits (2026-11-13: OpenCode Go 5-hour limit, HTTP 429) ---------------

GO_LIMIT_LINE = json.dumps({'type': 'error', 'sessionID': 'ses_x', 'error': {
    'type': 'provider.go-limit', 'status': 429, 'retryAfterMs': 4693000,
    'message': 'OpenCode Go 5 hour usage limit reached. Switch to a free model, or turn on Extra Usage.'}})


def limited_handle(seconds=3600):
    h = ScriptedHandle(code=1)
    h.limit_error = lambda: ('OpenCode Go 5 hour usage limit reached', seconds)
    return h


def test_handle_reads_a_usage_limit_and_its_reset_time(tmp_path):
    log = tmp_path / 'w.jsonl'
    log.write_text(GO_LIMIT_LINE + '\n')
    message, seconds = farm.OpencodeHandle(None, log).limit_error()
    assert 'usage limit' in message and seconds == 4693
    log.write_text(json.dumps({'type': 'error', 'error': {
        'type': 'provider.go-limit', 'status': 429,
        'message': 'OpenCode Go usage limit reached (5-hour, resets in 1h 19m).'}}) + '\n')
    assert farm.OpencodeHandle(None, log).limit_error()[1] == 79 * 60
    log.write_text('{"type":"step_finish","part":{"reason":"stop"}}\n')
    assert farm.OpencodeHandle(None, log).limit_error() is None


def test_usage_limit_pauses_the_provider_and_requeues_without_an_attempt(root, monkeypatch):
    # Four backlog fix attempts were spent on the limit and reported as unchanged sources.
    now = [1000.0]
    monkeypatch.setattr(farm.time, 'time', lambda: now[0])

    def plan(job):
        if job['model'].startswith('go/') and now[0] < 4600:
            return limited_handle()
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD)
        return ScriptedHandle(code=0)

    f, cmre, launcher = scripted_farm(root, plan, lambda: 0.0, ladder=['go/a', 'go/b'])
    drain(f, 5)
    assert [j['model'] for j, _, _ in launcher.started] == ['go/a']   # go/b shares the account limit
    assert 'fn_00401000' not in f.state['attempts'] and not f.state['parked']
    assert 'LIMITED go' in farm.status_line(f.summary())
    assert any(e['event'] == 'model-limited' for e in events(f.run_dir))
    now[0] = 5000.0
    drain(f, 5)
    assert cmre.accepted == ['fn_00401000']
    assert [j['model'] for j, _, _ in launcher.started] == ['go/a', 'go/a']


def test_a_limited_provider_falls_through_to_another_providers_rung(root, monkeypatch):
    monkeypatch.setattr(farm.time, 'time', lambda: 1000.0)

    def plan(job):
        if job['model'].startswith('go/'):
            return limited_handle()
        Path(job['source']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['source']).write_text(GOOD)
        return ScriptedHandle(code=0)

    f, cmre, launcher = scripted_farm(root, plan, lambda: 0.0, ladder=['go/a', 'mm/b'])
    drain(f, 5)
    assert [j['model'] for j, _, _ in launcher.started] == ['go/a', 'mm/b']
    assert cmre.accepted == ['fn_00401000']
    assert [r['model'] for r in f.state['attempts']['fn_00401000']] == ['mm/b']


@pytest.mark.parametrize('source, accepted', [(GOOD, True), (GOOD.replace('a + 1', 'a + 2'), False)])
def test_default_terra_lane_works_when_other_providers_are_limited(root, monkeypatch, source, accepted):
    monkeypatch.setattr(farm.time, 'time', lambda: 1000.0)
    f, cmre, launcher = make(root, [item('fn_00401000')], writes(source), workers=1,
                             ladder=list(farm.DEFAULT_LADDER), audit_workers=1,
                             audit_model=farm.DEFAULT_AUDIT_MODEL,
                             confirm_model=farm.DEFAULT_CONFIRM_MODEL)
    f.state['limited_providers'] = {
        provider: {'until': 4600, 'message': 'quota exhausted'}
        for provider in ('opencode-go', 'minimax-coding-plan')}
    drain(f)
    assert launcher.started and all(j['model'] == 'openai/gpt-6.1-terra'
                                    for j, _, _ in launcher.started)
    assert cmre.accepted == (['fn_00401000'] if accepted else [])
    assert len(cmre.tests) == (2 if accepted else 1)
    if accepted:
        assert f.state['audits']['fn_00401000']['verdict'] == 'ok'
    else:
        assert 'fn_00401000' in f.state['parked']


@pytest.mark.parametrize('confirm', [False, True])
@pytest.mark.parametrize('failure', ['limited', 'auth'])
def test_audit_and_confirmation_fall_back_to_terra(root, monkeypatch, confirm, failure):
    monkeypatch.setattr(farm.time, 'time', lambda: 1000.0)
    preferred = 'opencode-go/deepseek-v4.1-flash'
    terra = 'openai/gpt-6.1-terra'

    def plan(job):
        if job['model'] == preferred:
            return ScriptedHandle(code=1, infra='401 API key expired')
        Path(job['audit_path']).parent.mkdir(parents=True, exist_ok=True)
        Path(job['audit_path']).write_text(json.dumps({'verdict': 'ok', 'findings': []}))
        return ScriptedHandle(code=0)

    f, cmre, launcher = scripted_farm(root, plan, lambda: 0.0, workers=0,
                                      ladder=[preferred, terra], audit_workers=1,
                                      audit_model=preferred, confirm_model=preferred)
    fn = 'fn_00401000'
    (root / 'recovery/c' / f'{fn}.c').write_text(GOOD)
    f.state['accepted'][fn] = {'bytes': 200, 'model': preferred, 'kind': 'translate', 'at': 0}
    if confirm:
        f.state['audits'][fn] = {'rounds': 1, 'verdict': 'unconfirmed',
                                 'findings': [B], 'model': preferred}
    f.audit_queue.append(fn)
    if failure == 'limited':
        f.state['limited_providers']['opencode-go'] = {'until': 4600, 'message': 'usage limit'}
    drain(f)
    models = [j['model'] for j, _, _ in launcher.started]
    assert models == ([terra] if failure == 'limited' else [preferred, terra])
    assert bool(launcher.started[-1][0].get('confirm')) == confirm
    assert f.state['audits'][fn]['verdict'] == 'ok'
    assert f.config.audit_model == preferred and f.config.confirm_model == preferred
    assert cmre.accepted == [] and cmre.tests == []
    assert all(row['model'] == terra for row in f.state['attempts'][fn])


def test_a_limited_audit_waits_for_the_reset_instead_of_erroring(root, monkeypatch):
    now = [1000.0]
    monkeypatch.setattr(farm.time, 'time', lambda: now[0])
    calls = []

    class Launcher(FakeLauncher):
        def start(self, job, prompt, config):
            self.started.append((job, prompt, config))
            if job['kind'] == 'audit':
                calls.append(now[0])
                if now[0] < 4600:
                    return limited_handle()
            return FakeHandle(job, writes(GOOD), self.root)

    cmre = FakeCmre([item('fn_00401000')])
    cmre.root = root
    cfg = farm.FarmConfig(workers=1, ladder=['go/cheap'], audit_workers=1, audit_model='go/judge',
                          attempt_minutes=30, hours=1, min_bytes=0, max_bytes=None, include_held=False,
                          owner='farm-t')
    f = farm.Farm(root / 'run', root, cmre, Launcher(root, None), cfg, clock=lambda: 0.0)
    drain(f, 5)
    assert calls == [1000.0] and f.state['audits'].get('fn_00401000', {}).get('verdict') is None
    now[0] = 5000.0
    drain(f, 5)
    assert f.state['audits']['fn_00401000']['verdict'] == 'ok'


def test_adopt_and_dismiss_apply_directly_when_the_run_has_no_live_farm(root, monkeypatch):
    # The backlog run's open defects outlive its farm; the coordinator resolves them later.
    f, cmre, launcher = make(root, [], writes(GOOD), workers=0)
    for fn in ('fn_00401000', 'fn_00401100'):
        f.state['accepted'][fn] = {'bytes': 200, 'model': 'm/x', 'kind': 'translate', 'at': 0}
        f.state['audits'][fn] = {'rounds': 1, 'verdict': 'defect', 'findings': [B]}
        f.state['defects'][fn] = [B]
    f._event('farm-start', pid=1)
    f._event('farm-exit', status='done')
    f.save()
    (root / 'recovery/c/fn_00401000.c').write_text(GOOD)
    hand = root / 'hand.c'
    hand.write_text(GOOD.replace('a + 1', 'a + 1 /*PASS by hand*/'))
    monkeypatch.setattr(farm, 'ROOT', root)
    monkeypatch.setattr(farm, 'CmreAdapter', lambda *a, **k: cmre)
    assert farm.main(['adopt', str(f.run_dir), 'fn_00401000', '--source', str(hand)]) == 0
    assert farm.main(['dismiss', str(f.run_dir), 'fn_00401100', '--reason', 'false positive']) == 0
    state = json.loads((f.run_dir / 'state.json').read_text())
    assert cmre.accepted == ['fn_00401000']
    assert state['defects'] == {} and state['audits']['fn_00401100']['dismissed'] == 'false positive'
    assert not list((f.run_dir / 'requests').glob('*.json'))


def test_coordinator_runs_on_a_different_provider_than_the_go_limited_lanes():
    # 2026-11-13: the OpenCode Go 5-hour limit stopped every DeepSeek model at once.
    coordinator = farm.coordinator_agent()
    assert f'model: {farm.COORDINATOR_MODEL}' in coordinator
    assert farm.COORDINATOR_MODEL.split('/')[0] != farm.DEFAULT_CONFIRM_MODEL.split('/')[0]


def test_defects_command_lists_each_open_defect_on_one_line(root, capsys):
    # A MiniMax coordinator read state.json and the farm source to learn this (2026-11-13 probe).
    f, cmre, _ = make(root, [], writes(GOOD), workers=0)
    f.state['accepted']['fn_00401000'] = {'bytes': 321, 'model': 'm/x', 'kind': 'translate', 'at': 0}
    f.state['audits']['fn_00401000'] = {'rounds': 1, 'verdict': 'defect', 'findings': [B],
                                        'confirmed_by': 'go/judge'}
    f.state['defects']['fn_00401000'] = [B]
    f.state['attempts']['fn_00401000'] = [{'model': 'go/judge', 'kind': 'fix', 'outcome': 'fail',
                                           'detail': 'compile failed', 'at': 0}]
    f.save()
    assert farm.main(['defects', str(f.run_dir)]) == 0
    out = capsys.readouterr().out.strip().splitlines()
    assert len(out) == 1
    assert out[0].startswith('fn_00401000 321B') and 'EAX return dropped' in out[0] and 'compile failed' in out[0]


def test_coordinator_agent_names_its_information_sources():
    assert 'status, wait and defects' in farm.coordinator_agent()


def test_every_default_deepseek_model_is_v4_1_flash():
    # Adam's choice (2026-11-13): DeepSeek runs use v4.1 Flash.
    defaults = [*farm.DEFAULT_LADDER, farm.DEFAULT_AUDIT_MODEL, farm.DEFAULT_CONFIRM_MODEL, farm.COORDINATOR_MODEL]
    agents = farm.coordinator_agent() + farm.defect_agent()
    models = defaults + re.findall(r'^model: (\S+)', agents, re.M)
    deepseek = [m for m in models if 'deepseek' in m]
    assert deepseek and all(m.split('#')[0] == 'opencode-go/deepseek-v4.1-flash' for m in deepseek), deepseek
