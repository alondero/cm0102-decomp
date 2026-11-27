"""Input lane: farm model workers author entry inputs for unlifted functions (ADR 0013).

Supply for the C farm is lifted functions. Most remaining bytes are unlifted
because the lifted gate's cases do not reach 60% of their instructions. Until
21 Nov only an expert coordinator wrote structured inputs for them, one at a
time, while the farm's translation workers sat idle. This lane does it with the
same workers: one function per job, the worker writes a compact input spec
(scripts/cmre_input_probe.py) and gates it; the lane re-gates the spec itself,
appends the reaching fixtures to the canonical hints under the writer lease and
runs `cmre accept`. A refused accept restores the previous hint bytes. The
gate, the comparison and the threshold are unchanged; inputs only add cases.

    py -3.13 scripts/cmre_input_lane.py run RUN [--workers 4] [--ladder m1,m2]
    py -3.13 scripts/cmre_input_lane.py status RUN
    py -3.13 scripts/cmre_input_lane.py plan RUN [--top 20]

The farm starts `run` itself (`--input-lane-workers`, FARM.md "Input lane") and
it stops with the farm (RUN/STOP). State is local evidence under
RUN/input-lane/; never publish it.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(ROOT / 'scripts'))

PY = 'py -3.13'
LANE_VERSION = 'input-lane/1'
MAX_ATTEMPTS = 3            # worker attempts per function and hint state (one per ladder rung)
ATTEMPT_MINUTES = 30
STALL_MINUTES = 15
INFRA_PAUSE_SECONDS = 3600   # auth/key failure: pause the provider, retry hourly in case it is renewed
CANDIDATE_REFRESH_SECONDS = 600
IDLE_REFRESH_SECONDS = 60     # an idle lane re-scans (about 7 s of CPU) at most this often
SCREEN_JOBS = 2
REGATE_JOBS = 2             # model-free re-gates of best specs in flight (each is one full lifted gate)
SKIP_STATUSES = ('lift-blocked', 'fail', 'error', 'not-unlifted')
WORKER_COMMANDS = ('brief', 'test', 'explain', 'show', 'disasm', 'family', 'work show')
# Narrower than the farm's list: `rg --pre` runs a program and `sed -n` accepts `w FILE`/`-i`.
# Workers have the read/grep/glob tools for searching.
READ_ONLY_SHELL = ('cat *', 'head *', 'tail *', 'wc *', 'Get-Content *')
# Written inside YAML double quotes: r'*\n*' is a YAML escape for a real newline.
SHELL_DENY = ('*>*', '*<*', '*|*', '*;*', '*&*', '*`*', '*$(*', r'*\n*', r'*\r*')
ABSENT = 'absent'         # hints_sha256 of a function with no canonical hint file
RESULT = re.compile(r'FARM-RESULT:\s*([a-z][a-z-]*)')


def _fn(va):
    return f'fn_{va:08x}'


def child_env():
    """Environment for gate and screen subprocesses. coordination.writer() puts the lease token
    in os.environ, and a gate started by a pool thread while apply holds the lease would
    otherwise join it (re-review 2026-11-21)."""
    return {k: v for k, v in os.environ.items() if not k.startswith('CMRE_WRITER_')}


def threshold(raw, default=0.6):
    """The lifted coverage threshold from canonical hint bytes, or None if they are unusable."""
    if raw is None:
        return default
    try:
        data = json.loads(raw)
    except ValueError:
        return None
    if not isinstance(data, dict):
        return None
    value = data.get('min_coverage', default)
    return float(value) if isinstance(value, (int, float)) and not isinstance(value, bool) else None


def hint_key(raw):
    """State key: a function is retried when its canonical hints change."""
    return hashlib.sha256(LANE_VERSION.encode() + b'\n' + (raw if raw is not None else b'absent')).hexdigest()[:16]


# --- state ----------------------------------------------------------------------

def _load(path):
    try:
        return json.loads(Path(path).read_text(encoding='utf-8'))
    except (OSError, ValueError):
        return {'functions': {}, 'limited': {}, 'counter': 0}


def _save(path, state):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix('.tmp')
    tmp.write_text(json.dumps(state, indent=1, sort_keys=True) + '\n', encoding='utf-8')
    os.replace(tmp, path)


def probe_tool():
    """The input probe's tool version: a best spec gated by an older probe is re-gated once."""
    import cmre_input_probe
    return cmre_input_probe.TOOL_VERSION


def current_attempts(row, harness):
    """Attempts made under this harness digest. Attempts with no digest predate the field."""
    if harness is None:
        return list(row.get('attempts', []))
    return [a for a in row.get('attempts', []) if a.get('harness') == harness]


def no_progress_run(row, harness):
    """Trailing consecutive coverage-bearing attempts that did not improve on the best
    coverage reached before them (the pre-attempt baseline first). Error attempts have
    no coverage: they neither count toward a plateau nor reset the run."""
    baseline = row.get('baseline', row.get('coverage'))
    best = float(baseline) if isinstance(baseline, (int, float)) and not isinstance(baseline, bool) else -1.0
    trailing = []
    for attempt in current_attempts(row, harness):
        coverage = attempt.get('coverage')
        if not isinstance(coverage, (int, float)) or isinstance(coverage, bool):
            continue
        if coverage > best:
            best = coverage
            trailing = []
        else:
            trailing.append(attempt)
    return trailing


def plan(candidates, rows, active=(), harness=None):
    """Functions to work, best first: near the coverage threshold, then larger.

    candidates: {fn: {'bytes', 'coverage' (None if never gated), 'key', 'status'}}.
    A function is skipped while active, after MAX_ATTEMPTS at its current hint key and
    harness digest, while its coverage plateau stands (two consecutive no-progress
    attempts; FARM.md "Input lane"), or when its lifted gate cannot be helped by inputs
    (blocked lift, mismatch, error). A terminal status set under an older harness reopens
    (the screen re-runs it once); rows with no harness digest are legacy and also reopen.
    `accepted` stays done. A same-key `pass-unaccepted` row is intentionally harness
    independent: its inputs already passed the gate and acceptance refused for another
    reason, so more input attempts cannot help; changed inputs reopen it. harness=None
    ignores digests."""
    todo = []
    for fn, info in candidates.items():
        row = rows.get(fn, {})
        if fn in active or info.get('status') in SKIP_STATUSES:
            continue
        if row.get('key') == info['key']:
            status = row.get('status')
            if status in ('accepted', 'pass-unaccepted'):
                continue
            if status in SKIP_STATUSES and (harness is None or row.get('harness') == harness):
                continue
            plateau = row.get('plateau')
            if plateau and (harness is None or plateau.get('harness') == harness):
                continue
            if len(current_attempts(row, harness)) >= MAX_ATTEMPTS:
                continue
        coverage = info.get('coverage')
        priority = coverage
        if row.get('key') == info['key']:
            best_coverage = (row.get('best') or {}).get('coverage')
            if (isinstance(best_coverage, (int, float)) and not isinstance(best_coverage, bool)
                    and (priority is None or best_coverage > priority)):
                priority = best_coverage   # ordering heuristic only; the prompt/baseline keep canonical coverage
        band = round(priority, 1) if priority is not None else -1.0
        todo.append((-band, -info['bytes'], fn))
    return [fn for _, _, fn in sorted(todo)]


# --- worker prompt and permissions -----------------------------------------------

def _rel(root, path):
    try:
        return Path(path).resolve().relative_to(Path(root).resolve()).as_posix()
    except ValueError:
        return Path(path).as_posix()


def probe_commands(root, job):
    spec = _rel(root, job['spec'])
    return (f"{PY} scripts/cmre_input_probe.py try {job['fn']} {spec}",
            f"{PY} scripts/cmre_input_probe.py gate {job['fn']} {spec}")


def agent_text(root, job):
    """OpenCode 2.x agent: read anything, edit only the job's spec, run the probe and read-only cmre."""
    rules = [('*', '*', 'deny'), ('read', '*', 'allow'), ('grep', '*', 'allow'), ('glob', '*', 'allow'),
             ('edit', _rel(root, job['spec']), 'allow')]
    shell = [f'{PY} -m cmre {command} *' for command in WORKER_COMMANDS]
    shell += [*probe_commands(root, job), *READ_ONLY_SHELL]
    rules += [('shell', command, 'allow') for command in shell]
    # Later rules win (the catch-all deny comes first). `cat x > y` matches `cat *`, so
    # redirection and chaining are denied after every allow.
    rules += [('shell', pattern, 'deny') for pattern in SHELL_DENY]
    lines = ['---', f"description: CM0102 input lane {job['fn']}", 'mode: primary', 'permissions:']
    lines += [f'  - {{ action: "{a}", resource: "{r}", effect: {e} }}' for a, r, e in rules]
    lines += ['---', 'You are a CM0102 input-lane worker. Follow the user message exactly.', '']
    return '\n'.join(lines)


def prompt(root, job):
    try_cmd, gate_cmd = probe_commands(root, job)
    fn = job['fn']
    coverage = job.get('coverage')
    prior = job.get('prior')
    lines = [
        f"Author ENTRY INPUTS for {fn} ({job['bytes']} original body bytes) so the unchanged lifted "
        f"differential gate reaches at least {job.get('threshold', 0.6):.0%} of its instructions. "
        f"Current coverage: {coverage if coverage is not None else 'unknown'}.",
        'You write test inputs only. You never write C, never change acceptance policy, and never edit any '
        f"file except your spec {_rel(root, job['spec'])} (it already exists; keep it valid JSON).",
        '',
        'Steps:',
        f'1. `{PY} -m cmre brief {fn}` (listing, callees, globals) and `{PY} -m cmre explain {fn}` '
        '(where the original stops on the current cases, and which globals it reads). '
        f'`{PY} -m cmre disasm 0xVA --count 40` reads more of the listing.',
        '2. Work out what the function reads at entry: ECX receiver (thiscall), stack arguments, records '
        'behind pointers, writable globals that point at tables. Model them as arena records in the spec '
        '(see the format in scripts/cmre_input_probe.py\'s docstring: `cat scripts/cmre_input_probe.py`).',
        '   Read faults ("read-unmapped") mean a pointer the function follows is not mapped: point it into the '
        'arena ("A+0x...") and fill the record it reads. Use `vary` over the selector, flag, count and '
        'switch values the listing compares (cmp/test/jcc operands, jump-table bounds).',
        f'3. `{try_cmd}` shows each fixture\'s outcome, stop detail and new instructions (seconds, original only).',
        f'4. `{gate_cmd}` adds the fixtures that reach new code to a private proposal and runs the full lifted '
        'gate (about a minute). Iterate on the spec until the gate status is `pass`, at most 8 gate runs.',
        '',
        'Rules:',
        '- Inputs model what the listing reads. Cite instruction VAs in the spec "notes" for each record shape.',
        '- A gate `fail` (mismatch) is a finding about the lift, not an input problem: stop and report it; '
        'never reshape inputs to avoid a mismatch.',
        '- Never invent behaviour. Import results stay seeded scaffolding (vary `import_seed` if a branch '
        'depends on one).',
        '',
        'End with exactly one final line:',
        'FARM-RESULT: pass | FARM-RESULT: low-coverage <coverage> | FARM-RESULT: mismatch <case> | '
        'FARM-RESULT: blocked <one-line reason>',
    ]
    if prior:
        seeded = ('its spec is already in your spec file. Continue from it.' if prior.get('seeded', True)
                  else 'your spec file starts empty.')
        lines[2:2] = ['', f"A previous attempt ({prior.get('model')}) reached coverage {prior.get('coverage')}; "
                          f"{seeded} Its note: {prior.get('detail', '')[:300]}"]
    return '\n'.join(lines)


EMPTY_SPEC = {'notes': '', 'base': {}, 'fixtures': []}


# --- the lane ----------------------------------------------------------------------

class _Done:
    """A finished future: the default gate executor runs gates inline (tests, `plan`)."""

    def __init__(self, fn, *args):
        try:
            self._result, self._error = fn(*args), None
        except Exception as exc:   # surfaced by result(), as a real future would
            self._result, self._error = None, exc

    def done(self):
        return True

    def result(self):
        if self._error is not None:
            raise self._error
        return self._result


class InlineGates:
    def submit(self, fn, *args):
        return _Done(fn, *args)


class Lane:
    def __init__(self, run_dir, ops, launcher, workers=4, ladder=(), clock=time.time,
                 attempt_minutes=ATTEMPT_MINUTES, stall_minutes=STALL_MINUTES, root=ROOT, only=None,
                 gates=None):
        self.run_dir, self.ops, self.launcher, self.clock = Path(run_dir), ops, launcher, clock
        self.dir = self.run_dir / 'input-lane'
        self.state_path = self.dir / 'state.json'
        self.state = _load(self.state_path)
        self.state.setdefault('functions', {})
        self.state.setdefault('limited', {})
        self.state.setdefault('counter', 0)
        self.workers, self.ladder = workers, list(ladder)
        self.attempt_seconds, self.stall_seconds = attempt_minutes * 60, stall_minutes * 60
        self.root = Path(root)
        self.only = set(only) if only else None   # restrict to these functions (smoke runs)
        # Gates (up to a few minutes each) run beside dispatch, never inside it.
        self.gates = gates or InlineGates()
        self.running = []          # (job, handle, started)
        self.gating = []           # (job, claimed, interrupted, future)
        self.candidates, self.refreshed = {}, None
        self.harness = None        # harness digest, read once per refresh
        self.wake = False          # work may have appeared (a gate result or accept): rescan an idle lane now
        self.queue = []

    # bookkeeping
    def event(self, name, **data):
        self.dir.mkdir(parents=True, exist_ok=True)
        with (self.dir / 'events.jsonl').open('a', encoding='utf-8') as out:
            out.write(json.dumps({'at': self.clock(), 'event': name, **data}) + '\n')

    def save(self):
        _save(self.state_path, self.state)

    def stopping(self):
        return (self.run_dir / 'STOP').exists() or (self.dir / 'STOP').exists()

    def _control(self):
        try:
            control = json.loads((self.run_dir / 'control.json').read_text(encoding='utf-8'))
        except (OSError, ValueError):
            return
        if isinstance(control.get('input_lane_workers'), int):
            self.workers = max(0, control['input_lane_workers'])
        if isinstance(control.get('ladder'), list) and control['ladder']:
            self.ladder = list(control['ladder'])

    def _available(self, model):
        """A provider limit pauses every tier; provider:paid only paid models; provider:free only free."""
        import cmre_farm
        provider = model.split('/', 1)[0]
        tier = 'free' if cmre_farm._is_free_model(model) else 'paid'
        limited = self.state['limited']
        return max(limited.get(provider, 0), limited.get(f'{provider}:{tier}', 0),
                   limited.get(model.split('#', 1)[0], 0)) <= self.clock()

    def _pause(self, model, message, seconds):
        import cmre_farm
        provider = model.split('/', 1)[0]
        if cmre_farm.NO_ROUTE.search(message or ''):
            key = model.split('#', 1)[0]   # the provider stopped routing this model only
        elif cmre_farm._is_free_model(model):
            key = f'{provider}:free'
        else:   # a limit that says to switch to a free model covers the paid tier only
            key = f'{provider}:paid' if cmre_farm.FREE_EXEMPT.search(message or '') else provider
        self.state['limited'][key] = self.clock() + seconds

    def _limit_keys(self, model):
        """Provider/tier/model pause keys that apply to this model, broadest first."""
        import cmre_farm
        bare = model.split('#', 1)[0]
        provider = bare.split('/', 1)[0]
        tier = 'free' if cmre_farm._is_free_model(model) else 'paid'
        return (provider, f'{provider}:{tier}', bare)

    def _expired_probe(self, model):
        """(key, expiry) of the broadest expired pause this model must probe, or None."""
        now = self.clock()
        for key in self._limit_keys(model):
            until = self.state['limited'].get(key, 0)
            if 0 < until <= now:
                return key, until
        return None

    def _probe_blocked(self, model):
        """Only a tagged post-expiry job blocks another probe of the same pause generation."""
        probe = self._expired_probe(model)
        return probe is not None and any((job.get('probe_key'), job.get('probe_expiry')) == probe
                                         for job, _, _ in self.running)

    def _clear_probe(self, job):
        """Clear only the exact pause generation this tagged post-expiry job probed."""
        key, expiry = job.get('probe_key'), job.get('probe_expiry')
        if key is not None and self.state['limited'].get(key) == expiry:
            self.state['limited'].pop(key, None)
            return True
        return False

    def _model(self, attempt):
        """The least busy available model of the first rung at or above the attempt that has one;
        a rung is a group `a|b`, a plain model a group of one."""
        for entry in self.ladder[min(attempt, len(self.ladder) - 1):] if self.ladder else []:
            usable = [m for m in (g.strip() for g in entry.split('|'))
                      if m and self._available(m) and not self._probe_blocked(m)]
            if usable:
                return min(usable, key=lambda m: sum(1 for job, _, _ in self.running if job['model'] == m))
        return None

    def _busy(self):
        return {job['fn'] for job, _, _ in self.running} | {job['fn'] for job, _, _, _ in self.gating}

    # candidates
    def refresh(self, force=False, idle=False):
        period = IDLE_REFRESH_SECONDS if idle else CANDIDATE_REFRESH_SECONDS
        if not force and self.refreshed is not None and self.clock() - self.refreshed < period:
            return
        self.refreshed, self.wake = self.clock(), False
        self.harness = self.ops.harness_digest()
        self.candidates = self.ops.candidates()
        if self.only is not None:
            self.candidates = {fn: info for fn, info in self.candidates.items() if fn in self.only}
        for fn, info in self.candidates.items():
            row = self.state['functions'].get(fn, {})
            if row.get('key') != info['key']:
                continue
            if row.get('status') in SKIP_STATUSES and row.get('harness') != self.harness:
                info['coverage'] = None   # settled under an older harness: the screen runs it again
            elif info.get('coverage') is None and row.get('coverage') is not None:
                info['coverage'] = row['coverage']   # this lane's own screen of the same hints
        self.queue = plan(self.candidates, self.state['functions'], self._busy(), self.harness)

    def regate(self):
        """Re-gate each function's best spec once per probe tool version, with no model.

        A probe release can turn a spec the lane already holds into a pass: on 27 Nov
        the 48-fixture cap kept 0.53-0.55 of specs whose fixtures reached 0.91-0.96.
        A re-gate is not a model attempt. It counts toward neither MAX_ATTEMPTS nor a
        plateau, and its result is applied exactly like a worker's (FARM.md "Input lane")."""
        if self.stopping():
            return
        tool = probe_tool()
        busy = self._busy()
        inflight = sum(1 for job, _, _, _ in self.gating if job.get('kind') == 'regate')
        todo = []
        for fn, info in self.candidates.items():
            row = self.state['functions'].get(fn)
            if (not row or fn in busy or row.get('key') != info['key'] or info.get('status') in SKIP_STATUSES
                    or row.get('status') in ('accepted', 'pass-unaccepted', *SKIP_STATUSES)
                    or row.get('pending_apply')):
                continue
            best = row.get('best') or {}
            if best.get('spec') and best.get('probe_tool') != tool:
                todo.append((-(best.get('coverage') or 0), -info['bytes'], fn))
        submitted = False
        for _, _, fn in sorted(todo):
            if inflight >= REGATE_JOBS:
                break
            info, best = self.candidates[fn], self.state['functions'][fn]['best']
            try:
                data = Path(best['spec']).read_bytes()
            except OSError:
                continue
            self.state['counter'] += 1
            n = self.state['counter']
            spec = self.dir / 'specs' / f'{fn}-{n}.regate.json'
            spec.parent.mkdir(parents=True, exist_ok=True)
            spec.write_bytes(data)
            job = {'fn': fn, 'n': n, 'kind': 'regate', 'model': best.get('model') or 'unknown',
                   'bytes': info['bytes'], 'coverage': info.get('coverage'),
                   'threshold': info.get('threshold', 0.6), 'spec': str(spec),
                   'hints_sha256': info['hints_sha256'], 'key': info['key'],
                   'seed_sha256': hashlib.sha256(data).hexdigest(), 'harness': self.harness,
                   'regate_of': best['spec'], 'tool': tool}
            self.event('regate', fn=fn, n=n, of=best['spec'], coverage=best.get('coverage'),
                       bytes=info['bytes'], tool=tool)
            self._submit_gate(job, '', None)
            inflight += 1
            submitted = True
        if submitted:
            self.save()

    def _finish_regate(self, job, row, result):
        fn, status, coverage = job['fn'], result.get('status'), result.get('coverage')
        row.setdefault('regates', []).append(
            {'n': job['n'], 'status': status, 'coverage': coverage, 'tool': job['tool'], 'of': job['regate_of'],
             'at': self.clock(), 'harness': job.get('harness', self.harness)})
        best = row.get('best') or {}
        if coverage is not None and status in ('low-coverage', 'pass') and coverage > (best.get('coverage') or 0):
            best = {**best, 'spec': job['spec'], 'coverage': coverage, 'model': job['model'],
                    'detail': f"regate {status}; first uncovered {result.get('first_uncovered', [])[:4]}"}
        if best:
            row['best'] = {**best, 'probe_tool': job['tool']}
        self.event('result', fn=fn, n=job['n'], model=job['model'], status=status, coverage=coverage,
                   kind='regate')
        if status == 'pass':
            self._apply(fn, row, {'proposal': result['proposal'], 'expected': job['hints_sha256'],
                                  'sha256': result.get('proposal_sha256'), 'bytes': job['bytes'],
                                  'model': job['model'], 'coverage': coverage})
        elif status == 'fail':
            row['status'] = 'fail'
            row['harness'] = job.get('harness', self.harness)
            row['mismatches'] = result.get('mismatches', [])[:3]
            self.event('lift-mismatch', fn=fn, mismatches=row['mismatches'])
        self.save()

    def _screen(self, fn):
        """A never-gated function is gated once on its canonical hints before any model time."""
        info = self.candidates[fn]
        report = self.ops.screen(fn)
        info.update(status=report.get('status'), coverage=report.get('coverage'))
        row = self.state['functions'].setdefault(fn, {})
        if row.get('key') != info['key']:
            row.clear()
        if row.get('status') in SKIP_STATUSES and row.get('harness') != self.harness:
            for stale in ('status', 'mismatches', 'screen_errors'):
                row.pop(stale, None)   # reopened by a harness release: judge it afresh
        row.update(key=info['key'], harness=self.harness, screened=report.get('status'),
                   coverage=report.get('coverage'))
        if report.get('status') == 'pass':
            outcome = self.ops.accept(fn)
            row['status'] = 'accepted' if outcome == 'accepted' else 'pass-unaccepted'
            row['accept'] = outcome
            self.wake = True
            self.event('screen-accept', fn=fn, outcome=outcome, bytes=info['bytes'])
            return False
        if report.get('status') != 'low-coverage':
            status = report.get('status') if report.get('status') in SKIP_STATUSES else 'error'
            if status == 'error' and row.get('screen_errors', 0) < 1:
                row['screen_errors'] = row.get('screen_errors', 0) + 1   # transient: rescreen next refresh
                self.event('screen-error', fn=fn, error=str(report.get('error', ''))[:200])
                return False
            row['status'] = status
            self.event('screen-skip', fn=fn, status=report.get('status'))
            return False
        return True

    # dispatch
    def dispatch(self):
        if self.stopping():
            return
        screens = 0
        deferred = []
        while len(self.running) < self.workers and self.queue:
            fn = self.queue[0]
            info = self.candidates.get(fn)
            if info is None or fn in self._busy():
                self.queue.pop(0)
                continue
            if info.get('coverage') is None:
                if screens >= SCREEN_JOBS:
                    break   # bounded per tick: screens must not starve result handling
                screens += 1
                self.queue.pop(0)
                if not self._screen(fn):
                    self.save()
                    continue
                self.queue.insert(0, fn)
            row = self.state['functions'].setdefault(fn, {})
            if row.get('key') != info['key']:
                row.clear()
                row['key'] = info['key']
            # The pre-attempt coverage is the plateau baseline: an attempt that does not
            # improve on it is a no-progress attempt. Comparability is harness-scoped
            # (2026-11-27 review): coverage reached under one harness is no evidence for
            # progress under another, so a harness release resets the baseline and the
            # first attempt under the new harness becomes the reference. Cleared with the
            # row on a hint change; historical attempts stay for diagnostics.
            if 'baseline_harness' not in row:
                if row.get('attempts'):
                    # Predates harness-scoped baselines: the seeding harness is unknown.
                    row.pop('baseline', None)
                else:
                    row['baseline'] = info.get('coverage')
                row['baseline_harness'] = self.harness
            elif row['baseline_harness'] != self.harness:
                row.pop('baseline', None)
                row['baseline_harness'] = self.harness
            row.setdefault('bytes', info['bytes'])
            attempts = row.setdefault('attempts', [])
            model = self._model(len(current_attempts(row, self.harness)))
            if model is None:
                deferred.append(self.queue.pop(0))   # limited for now: later candidates may be usable
                continue
            self.queue.pop(0)
            self.state['counter'] += 1
            n = self.state['counter']
            spec = self.dir / 'specs' / f'{fn}-{n}.json'
            spec.parent.mkdir(parents=True, exist_ok=True)
            best = row.get('best')
            seed = Path(best['spec']).read_bytes() if best and Path(best['spec']).exists() else None
            seed = seed or (json.dumps(EMPTY_SPEC, indent=1) + '\n').encode()
            spec.write_bytes(seed)
            probe = self._expired_probe(model)
            job = {'fn': fn, 'n': n, 'kind': 'inputs', 'model': model, 'bytes': info['bytes'],
                   'coverage': info.get('coverage'), 'threshold': info.get('threshold', 0.6),
                   'spec': str(spec), 'hints_sha256': info['hints_sha256'], 'key': info['key'],
                   'seed_sha256': hashlib.sha256(seed).hexdigest(), 'harness': self.harness}
            if probe is not None:
                job['probe_key'], job['probe_expiry'] = probe
            if best:
                job['prior'] = {'model': best.get('model'), 'coverage': best.get('coverage'),
                                'detail': best.get('detail', '')}
            elif attempts:
                last = attempts[-1]
                job['prior'] = {'model': last.get('model'), 'coverage': last.get('coverage'), 'seeded': False,
                                'detail': f"gate {last.get('status')}: its fixtures reached no instruction the "
                                          'current cases miss; try different record shapes or selectors'}
            handle = self.launcher.start(job, prompt(self.root, job), agent_text(self.root, job))
            self.running.append((job, handle, self.clock()))
            event = {'fn': fn, 'n': n, 'model': model, 'bytes': info['bytes'], 'coverage': info.get('coverage')}
            if probe is not None:
                event.update(probe_key=probe[0], probe_expiry=probe[1])
            self.event('dispatch', **event)
            self.save()
        self.queue = deferred + self.queue

    # results
    def collect(self):
        still = []
        for job, handle, started in self.running:
            code = handle.poll()
            if code is None:
                if self.stopping():
                    handle.kill()   # a stopping farm does not wait 30 minutes for inputs
                    self.event('interrupted', fn=job['fn'], n=job['n'], reason='stop')
                    continue
                terminal = self._snapshot_terminal_probe(job)
                if terminal is not None:
                    snapshot, status = terminal
                    handle.kill()
                    limit, infra = self._provider_errors(handle)
                    limited = self._record_limit(job, limit)
                    if infra:
                        self._record_infra(job, infra)
                    if not limited and not infra:
                        self._clear_probe(job)   # a completed tagged probe used the provider
                    self._submit_gate({**job, 'spec': str(snapshot)}, status, 'probe-complete')
                    continue
                if hasattr(handle, 'pending_limit') and handle.pending_limit():
                    handle.kill()   # waiting out a provider limit: pause the model now, no attempt
                    self._worker_done(job, handle, None)
                    continue
                activity = handle.last_activity() if hasattr(handle, 'last_activity') else None
                stalled = activity is not None and time.monotonic() - activity > self.stall_seconds
                if self.clock() - started > self.attempt_seconds or stalled:
                    handle.kill()
                    self._worker_done(job, handle, 'stalled' if stalled else 'timeout')
                else:
                    still.append((job, handle, started))
                continue
            self._worker_done(job, handle, None, code)
        self.running = still

    def _snapshot_terminal_probe(self, job):
        spec = Path(job['spec'])
        try:
            receipt = json.loads(spec.with_suffix('.probe-result.json').read_bytes())
            if (receipt['fn'] != job['fn'] or receipt['status'] not in ('pass', 'fail')
                    or (receipt['prior_sha256'] or ABSENT) != job['hints_sha256']):
                return None
            data = spec.read_bytes()
            if hashlib.sha256(data).hexdigest() != receipt['spec_sha256']:
                return None
            snapshot = spec.with_suffix('.lane-spec.json')
            with snapshot.open('xb') as out:
                out.write(data)
        except (OSError, ValueError, TypeError, KeyError):
            return None
        return snapshot, receipt['status']

    def _worker_line(self, handle):
        try:
            text = Path(handle.log).read_text(encoding='utf-8', errors='replace')
        except (OSError, AttributeError):
            return ''
        found = RESULT.findall(text.replace('\\n', '\n'))
        return found[-1] if found else ''

    def _provider_errors(self, handle):
        """(limit, infra); real handles classify overlapping rows, test doubles may not."""
        if hasattr(handle, 'provider_errors'):
            return handle.provider_errors()
        limit = handle.limit_error() if hasattr(handle, 'limit_error') else None
        infra = handle.infra_error() if hasattr(handle, 'infra_error') else None
        if limit and infra:
            # A method-per-detector double cannot say whether both hits were one row. Keep the
            # lane's limit-first overlap rule; independent rows need the row-aware provider_errors API.
            infra = None
        return limit, infra

    def _record_limit(self, job, limit):
        if not limit:
            return False
        message, seconds = limit
        self._pause(job['model'], message, seconds)
        self.event('model-limited', fn=job['fn'], n=job['n'], model=job['model'], seconds=seconds, message=message)
        self.save()
        return True

    def _spec_changed(self, job):
        try:
            return hashlib.sha256(Path(job['spec']).read_bytes()).hexdigest() != job.get('seed_sha256')
        except OSError:
            return False

    def _pause_provider(self, model, message, seconds):
        """A key/auth failure covers every model of the provider, free or paid."""
        self.state['limited'][model.split('/', 1)[0]] = self.clock() + seconds

    def _record_infra(self, job, infra):
        """Pause the provider for an auth/infrastructure error, whether or not work was written."""
        self._pause_provider(job['model'], infra, INFRA_PAUSE_SECONDS)
        self.event('model-unavailable', fn=job['fn'], n=job['n'], model=job['model'],
                   seconds=INFRA_PAUSE_SECONDS, error=str(infra)[:300])
        self.save()

    def _worker_done(self, job, handle, interrupted, code=None):
        self.wake = True   # a worker ended: its function may return to the queue on the next refresh
        limit, infra = self._provider_errors(handle)
        limited = self._record_limit(job, limit)
        changed = self._spec_changed(job)
        if infra:
            # A provider/auth failure is never proof of recovery, even after partial work.
            # Written bytes still go through the gate below.
            self._record_infra(job, infra)
        if (limited or infra) and not changed:
            return   # not an attempt: the function returns to the queue on the next refresh
        claimed = self._worker_line(handle)
        completed = interrupted is None and code == 0
        if not limited and not infra and completed and (changed or claimed):
            self._clear_probe(job)
        # A limit that ends a worker after it wrote a new spec still gates that work.
        self._submit_gate(job, claimed, interrupted)

    def _submit_gate(self, job, claimed, interrupted):
        future = self.gates.submit(self.ops.gate, job['fn'], Path(job['spec']), job['model'])
        self.gating.append((job, claimed, interrupted, future))

    def process_gates(self):
        still = []
        for job, claimed, interrupted, future in self.gating:
            if not future.done():
                still.append((job, claimed, interrupted, future))
                continue
            try:
                result = future.result()
            except Exception as exc:
                result = {'status': 'error', 'error': f'{type(exc).__name__}: {exc}'[:300]}
            self.finish(job, claimed, interrupted, result)
        self.gating = still

    def drain_gates(self):
        """Collect finished workers and wait for every gate (tests and shutdown)."""
        self.collect()
        while self.gating:
            self.process_gates()
            if self.gating:
                time.sleep(0.2)

    def finish(self, job, claimed, interrupted, result):
        fn = job['fn']
        row = self.state['functions'].setdefault(fn, {'key': job['key']})
        status, coverage = result.get('status'), result.get('coverage')
        if status in ('pass', 'fail') and (result.get('prior_sha256') or ABSENT) != job['hints_sha256']:
            row['accept'] = 'hints-changed'
            self.event('result', fn=fn, n=job['n'], model=job['model'], status='hints-changed',
                       gated_status=status, coverage=coverage, claimed=claimed, interrupted=interrupted)
            self.refreshed = None
            self.save()
            return
        self.wake = True
        if job.get('kind') == 'regate':
            return self._finish_regate(job, row, result)
        attempt = {'n': job['n'], 'model': job['model'], 'status': status, 'coverage': coverage,
                   'claimed': claimed, 'interrupted': interrupted, 'at': self.clock(),
                   'harness': job.get('harness', self.harness)}
        row.setdefault('attempts', []).append(attempt)
        best = row.get('best')
        if coverage is not None and status in ('low-coverage', 'pass') and (
                best is None or coverage > (best.get('coverage') or 0)):
            row['best'] = {'spec': job['spec'], 'coverage': coverage, 'model': job['model'],
                           'detail': f"gate {status}; first uncovered {result.get('first_uncovered', [])[:4]}",
                           'probe_tool': result.get('tool')}
        self.event('result', fn=fn, n=job['n'], model=job['model'], status=status, coverage=coverage,
                   claimed=claimed, interrupted=interrupted)
        harness = job.get('harness', self.harness)
        trailing = no_progress_run(row, harness)
        if len(trailing) >= 2 and row.get('plateau', {}).get('harness') != harness:
            # Two consecutive no-progress attempts: repeated same-coverage dispatches stop
            # here instead of spending the rest of the ladder (FARM.md "Input lane").
            obligation = ('import-transcript' if all(a.get('status') == 'no-new-reach' for a in trailing[-2:])
                          else 'input')
            row['plateau'] = {'at': self.clock(), 'coverage': trailing[-1].get('coverage'),
                              'harness': harness, 'obligation': obligation,
                              'first_uncovered': result.get('first_uncovered', [])[:4]}
            self.event('input-plateau', fn=fn, coverage=trailing[-1].get('coverage'),
                       obligation=obligation, first_uncovered=row['plateau']['first_uncovered'])
        if status == 'pass':
            pending = {'proposal': result['proposal'], 'expected': job['hints_sha256'],
                       'sha256': result.get('proposal_sha256'), 'bytes': job['bytes'], 'model': job['model'],
                       'coverage': coverage}
            self._apply(fn, row, pending)
        elif status == 'fail':
            row['status'] = 'fail'
            row['harness'] = attempt['harness']
            row['mismatches'] = result.get('mismatches', [])[:3]
            self.event('lift-mismatch', fn=fn, mismatches=row['mismatches'])
        self.save()

    def _apply(self, fn, row, pending, late=False):
        outcome = self.ops.apply(fn, pending['expected'], Path(pending['proposal']),
                                 proposal_sha256=pending.get('sha256'))
        row['accept'] = outcome
        self.wake = True
        if outcome == 'accepted':
            row['status'] = 'accepted'
            row.pop('pending_apply', None)
            self.event('accepted', fn=fn, bytes=pending.get('bytes'), model=pending.get('model'),
                       coverage=pending.get('coverage'), **({'late': True} if late else {}))
        elif outcome == 'busy':
            row['pending_apply'] = pending
        else:
            row.pop('pending_apply', None)
            self.event('apply-refused', fn=fn, reason=outcome[:200])

    def retry_applies(self):
        for fn, row in self.state['functions'].items():
            pending = row.get('pending_apply')
            if isinstance(pending, dict) and row.get('status') != 'accepted':
                self._apply(fn, row, pending, late=True)
            elif pending is not None and not isinstance(pending, dict):
                row.pop('pending_apply')   # pre-review format: no base identity, never applied

    def tick(self):
        self._control()
        self.collect()
        self.process_gates()
        self.retry_applies()
        idle = not self.queue and not self.running and not self.gating
        self.refresh(force=idle and self.wake, idle=idle)
        self.regate()
        self.process_gates()
        self.dispatch()
        self.save()

    def finished(self):
        return self.stopping() and not self.running and not self.gating

    def run(self, poll=10.0, sleep=time.sleep):
        self.event('lane-start', pid=os.getpid(), workers=self.workers, ladder=self.ladder)
        try:
            while not self.finished():
                self.tick()
                sleep(poll)
        finally:
            for job, handle, _ in self.running:
                handle.kill()   # a crashed lane must not leave model workers spending
                self.event('interrupted', fn=job['fn'], n=job['n'], reason='lane-exit')
            self.running = []
            self.save()
            self.event('lane-exit')

    def summary(self):
        counts, accepted_bytes = {}, 0
        for row in self.state['functions'].values():
            status = row.get('status') or ('attempted' if row.get('attempts') else 'screened')
            counts[status] = counts.get(status, 0) + 1
        for line in _events(self.dir / 'events.jsonl'):
            if line.get('event') in ('accepted', 'screen-accept') and line.get('outcome', 'accepted') == 'accepted':
                accepted_bytes += line.get('bytes') or 0
        return {'functions': counts, 'accepted_bytes': accepted_bytes, 'running': len(self.running),
                'limited': {k: v for k, v in self.state['limited'].items() if v > self.clock()}}


def _events(path):
    try:
        with Path(path).open(encoding='utf-8') as handle:
            for line in handle:
                try:
                    yield json.loads(line)
                except ValueError:
                    continue
    except OSError:
        return


# --- the real repository ---------------------------------------------------------

class CmreOps:
    def __init__(self, root=ROOT, run_dir=None):
        self.root = Path(root)
        self.run_dir = Path(run_dir) if run_dir else None

    def _lift_rows(self):
        from cmre import env
        try:
            data = json.loads((env.ANALYSIS / 'lift-lane' / 'state.json').read_text(encoding='utf-8'))
        except (OSError, ValueError, AttributeError):
            try:
                data = json.loads((self.root / 'CM0102/Analysis/cmre/lift-lane/state.json').read_text(encoding='utf-8'))
            except (OSError, ValueError):
                return {}
        return data.get('functions', {})

    def harness_digest(self):
        from cmre import recover
        return recover.harness_digest()

    def candidates(self):
        """Unlifted inventory functions with no hard work hold, and their last known coverage."""
        from cmre import difftest, env, inventory, ledger, queue, work
        known = {entry.va for entry in ledger.cmre_entries()} | {entry.va for entry in ledger.legacy_entries()}
        blocked = set(ledger.blocked())
        records = work.read()['functions']
        lift_rows = self._lift_rows()
        out = {}
        for va, function in inventory.load().functions.items():
            if va in known or va in blocked or queue.compiler_generated(va):
                continue
            hold = work.dispatch_hold(va, records)
            if hold and hold.get('reason') == 'work-state-hold':
                continue
            fn = _fn(va)
            path = env.HINTS_DIR / f'{fn}.json'
            raw = path.read_bytes() if path.exists() else None
            row = lift_rows.get(fn, {})
            coverage = row.get('coverage') if row.get('status') == 'low-coverage' else None
            status = row.get('status') if row.get('status') in SKIP_STATUSES else None
            # Scheduling needs only the threshold; the gate validates hints itself. Full
            # validation here allocated an arena per fixture for every candidate (MemoryError).
            minimum = threshold(raw, difftest.DEFAULTS['min_coverage'])
            if minimum is None:
                continue    # unusable canonical hints: not this lane's to repair
            out[fn] = {'bytes': function.body_bytes, 'coverage': coverage, 'status': status,
                       'key': hint_key(raw), 'threshold': minimum,
                       'hints_sha256': hashlib.sha256(raw).hexdigest() if raw is not None else ABSENT}
        return out

    def screen(self, fn):
        done = subprocess.run([sys.executable, '-m', 'cmre', 'test', fn, '--json'], cwd=self.root,
                              env=child_env(), capture_output=True, text=True, encoding='utf-8', errors='replace', timeout=1800)
        try:
            report = json.loads(done.stdout)
        except ValueError:
            return {'status': 'error', 'error': (done.stderr or done.stdout)[-300:]}
        diff = report.get('difftest', {})
        return {'status': report.get('status'), 'coverage': diff.get('coverage')}

    def gate(self, fn, spec, model):
        proposal = spec.with_suffix('.lane-proposal.json')
        command = [sys.executable, str(self.root / 'scripts/cmre_input_probe.py'), 'gate', fn, str(spec),
                   '--proposal', str(proposal), '--model', model]
        try:
            done = subprocess.run(command, cwd=self.root, env=child_env(), capture_output=True, text=True, encoding='utf-8',
                                  errors='replace', timeout=3600)
            return json.loads(done.stdout.strip().splitlines()[-1])
        except (subprocess.TimeoutExpired, ValueError, IndexError) as exc:
            return {'status': 'error', 'error': str(exc)[:300]}

    def accept(self, fn):
        done = subprocess.run([sys.executable, '-m', 'cmre', 'accept', fn], cwd=self.root, capture_output=True,
                              text=True, encoding='utf-8', errors='replace')
        last = (done.stdout or '').strip().splitlines()[-1:] or ['']
        try:
            status = json.loads(last[0]).get('status')
        except (ValueError, AttributeError):
            status = None
        if done.returncode == 0 and status == 'accepted':
            return 'accepted'
        text = done.stderr or ''
        if 'Writer active' in text or 'BusyError' in text:
            return 'busy'
        return 'refused: ' + ((text or done.stdout or '').strip().splitlines() or ['no output'])[-1][:200]

    def apply(self, fn, expected_sha256, proposal, proposal_sha256=None):
        """Write the gated proposal as canonical hints and accept, under one writer lease.

        expected_sha256 is the canonical hint identity the job was dispatched with (ABSENT
        when there was no file); it is always compared. proposal_sha256, when given, must
        match the bytes the lane's own gate wrote. Anything but an accept, including an
        exception, restores the previous hint bytes exactly."""
        from cmre import coordination, difftest, env
        import cmre_input_probe as probe
        if expected_sha256 is None:
            raise ValueError('apply needs the dispatch-time hint identity (sha256 or ABSENT)')
        path = env.HINTS_DIR / f'{fn}.json'
        try:
            data = proposal.read_bytes()
            if proposal_sha256 is not None and hashlib.sha256(data).hexdigest() != proposal_sha256:
                return 'refused: proposal bytes differ from the gated proposal'
            proposed = json.loads(data)
            difftest.validate_hints(proposed)
            with coordination.writer():
                prior = path.read_bytes() if path.exists() else None
                prior_sha = hashlib.sha256(prior).hexdigest() if prior is not None else ABSENT
                if prior_sha != expected_sha256:
                    return 'hints-changed'
                if not probe.append_only(json.loads(prior) if prior is not None else {}, proposed):
                    return 'refused: proposal is not append-only against the current hints'
                outcome = None
                try:
                    coordination.atomic_bytes(path, data)
                    outcome = self.accept(fn)
                finally:
                    if outcome != 'accepted':
                        if prior is None:
                            path.unlink(missing_ok=True)
                        else:
                            coordination.atomic_bytes(path, prior)
                return outcome
        except coordination.BusyError:
            return 'busy'
        except (OSError, ValueError) as exc:
            return f'refused: {exc}'[:200]


class Launcher:
    """opencode 2.x worker with a per-job agent file (the farm's launcher, input-lane agent)."""

    def __init__(self, root, run_dir):
        import cmre_farm
        self.farm = cmre_farm
        self.inner = cmre_farm.OpencodeV2Launcher(root, Path(run_dir) / 'input-lane')

    def start(self, job, prompt_text, agent):
        farm, inner = self.farm, self.inner
        logs = inner.run_dir / 'logs'
        logs.mkdir(parents=True, exist_ok=True)
        stem = f"{job['fn']}-inputs-{job['n']}"
        (logs / f'{stem}.prompt.txt').write_text(prompt_text, encoding='utf-8')
        name = re.sub(r'[^A-Za-z0-9-]', '-', f"lane-{inner.run_dir.parent.name}-{stem}")
        agent_path = inner.root / '.opencode/agents' / f'{name}.md'
        agent_path.parent.mkdir(parents=True, exist_ok=True)
        agent_path.write_text(agent, encoding='utf-8')
        inner._wait_for_agent(name)
        command = [inner.executable, 'run', '--agent', name, '-m', job['model'],
                   '--format', 'json', '--title', f"farm inputs {job['fn']}", prompt_text]
        flags = (subprocess.CREATE_NO_WINDOW | subprocess.CREATE_NEW_PROCESS_GROUP) if os.name == 'nt' else 0
        out = (logs / f'{stem}.jsonl').open('wb')
        process = subprocess.Popen(command, cwd=inner.root, env=dict(os.environ), stdout=out,
                                   stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, creationflags=flags)
        out.close()
        return farm.OpencodeV2Handle(process, logs / f'{stem}.jsonl', agent_path, inner.executable)


class LaneLock:
    """One lane process per run; process death releases the lock."""

    def __init__(self, directory):
        self.path = Path(directory) / 'lane.lock'
        self.handle = None

    def __enter__(self):
        self.path.parent.mkdir(parents=True, exist_ok=True)
        self.handle = self.path.open('a+b')
        try:
            if self.handle.seek(0, os.SEEK_END) == 0:
                self.handle.write(b'0')
                self.handle.flush()
            self.handle.seek(0)
            if os.name == 'nt':
                import msvcrt
                msvcrt.locking(self.handle.fileno(), msvcrt.LK_NBLCK, 1)
            else:
                import fcntl
                fcntl.flock(self.handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except OSError as exc:
            self.handle.close()
            raise RuntimeError('another input lane owns this run') from exc
        return self

    def __exit__(self, *args):
        if os.name == 'nt':
            import msvcrt
            self.handle.seek(0)
            msvcrt.locking(self.handle.fileno(), msvcrt.LK_UNLCK, 1)
        self.handle.close()


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest='action', required=True)
    r = sub.add_parser('run', help='author, gate and accept inputs until RUN/STOP')
    r.add_argument('run_dir', type=Path)
    r.add_argument('--workers', type=int, default=4)
    r.add_argument('--ladder', default='', help='comma-separated models, cheapest first')
    r.add_argument('--attempt-minutes', type=float, default=ATTEMPT_MINUTES)
    r.add_argument('--only', default='', help='comma-separated fn_X: work only these (smoke runs)')
    s = sub.add_parser('status', help='lane counts, accepted bytes and running jobs')
    s.add_argument('run_dir', type=Path)
    p = sub.add_parser('plan', help='the next functions the lane would work (read-only)')
    p.add_argument('run_dir', type=Path)
    p.add_argument('--top', type=int, default=20)
    a = parser.parse_args(argv)
    if a.action == 'status':
        lane = Lane(a.run_dir, None, None)
        print(json.dumps(lane.summary()))
        return 0
    ops = CmreOps(run_dir=a.run_dir)
    if a.action == 'plan':
        lane = Lane(a.run_dir, ops, None)
        lane.refresh(force=True)
        for fn in lane.queue[:a.top]:
            info = lane.candidates[fn]
            print(f"{fn} {info['bytes']:>7} B coverage {info.get('coverage')}")
        print(f'{len(lane.queue)} functions, {sum(lane.candidates[f]["bytes"] for f in lane.queue)} B')
        return 0
    ladder = [m for m in a.ladder.split(',') if m]
    if not ladder:
        import cmre_farm
        ladder = list(cmre_farm.DEFAULT_LADDER)
    lane_dir = a.run_dir / 'input-lane'
    from concurrent.futures import ThreadPoolExecutor
    with LaneLock(lane_dir), ThreadPoolExecutor(max_workers=max(2, a.workers)) as gates:
        lane = Lane(a.run_dir, ops, Launcher(ROOT, a.run_dir), workers=a.workers, ladder=ladder,
                    attempt_minutes=a.attempt_minutes, only=[f for f in a.only.split(',') if f] or None,
                    gates=gates)
        lane.run()
    return 0


if __name__ == '__main__':
    sys.exit(main())
