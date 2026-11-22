"""Defect lane: farm model workers resolve open audit defects (FARM.md "Defect subagent").

An accepted function whose audit defect survived the farm's one automatic fix stays
open until someone checks each finding against the listing and either fixes it
(`adopt`, which the farm re-gates) or shows it is a false positive (`dismiss`).
Until 22 Nov only the coordinator did this, by sending cm-defect subagents one at a
time: 11 reviews in 6 hours against 126 open defects. This lane runs the same
cm-defect procedure headless, one function per job, and turns each worker's final
line into the farm's checked `adopt`/`dismiss` request.

    py -3.13 scripts/cmre_defect_lane.py run RUN [--workers 2] [--model M] [--no-submit] [--only fn,..]
    py -3.13 scripts/cmre_defect_lane.py status RUN
    py -3.13 scripts/cmre_defect_lane.py plan RUN [--top 20]

Each job's claim name, receipt and hand-source path are fixed before dispatch, so a
result line naming any other path is refused. `--no-submit` records the adopt/dismiss
command without queuing it (smoke runs against a live farm). The farm starts `run`
itself when `--defect-lane-workers` is above 0. State is local evidence under
RUN/defect-lane/; never publish it.
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
LANE_VERSION = 'defect-lane/1'
DEFAULT_MODEL = 'openai/gpt-6.2-sol'   # Adam, 2026-11-22
MAX_ATTEMPTS = 2            # per open-defect identity: a failed adopt gets one more look, as in FARM.md
ATTEMPT_MINUTES = 40
STALL_MINUTES = 15
SUBMIT_GRACE_SECONDS = 2 * 3600   # a submitted request the farm has not applied yet
MANUAL_CLAIM_SECONDS = 6 * 3600   # someone else's recent review claim: leave that defect to them
INFRA_PAUSE_SECONDS = 3600
CANDIDATE_REFRESH_SECONDS = 300
CLAIM_PREFIX = 'defect-lane-'
SMOKE_PREFIX = 'defect-smoke-'   # --no-submit runs: own state under defect-lane/smoke/, own claims
LAUNCH_PAUSE_SECONDS = 300
STALE_FIX_SECONDS = 2 * 3600   # a fix dispatch this old with no open-defect after it: the farm died mid-fix
# A surviving operand mutant means the gate inputs never expose that x87 operand: no source
# rewrite fixes it, so the defect waits for inputs instead of spending its last attempt.
NEEDS_INPUTS = re.compile(r'\bsurvivor\b', re.I)
WAITING = ('submitted', 'submit-unknown')   # a request that may still be waiting for the farm
RESULT = re.compile(r'^\s*`*(ADOPT|DISMISS|UNRESOLVED)\b[ \t]*(.*?)`*\s*$')


def defect_key(identity, findings):
    """Attempts are counted per (accepted inputs, open findings); either changing starts afresh."""
    data = json.dumps({'identity': identity, 'findings': findings}, sort_keys=True).encode()
    return hashlib.sha256(LANE_VERSION.encode() + b'\n' + data).hexdigest()[:16]


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


def plan(candidates, rows, active=(), now=None):
    """Open defects to work, largest accepted function first.

    A defect is skipped while active, after MAX_ATTEMPTS at its current key, and while a
    request this lane submitted for that key may still be waiting for the farm."""
    now = time.time() if now is None else now
    todo = []
    for fn, info in candidates.items():
        row = rows.get(fn, {})
        if fn in active:
            continue
        if row.get('key') == info['key']:
            if len(row.get('attempts', [])) >= MAX_ATTEMPTS:
                continue
            if row.get('status') in WAITING and now - row.get('submitted_at', 0) < SUBMIT_GRACE_SECONDS:
                continue
            if row.get('status') in ('recorded', 'applied', 'needs-inputs'):
                continue   # smoke: one recorded result; applied: the farm took it; needs-inputs: no fix helps
        todo.append((-info['bytes'], fn))
    return [fn for _, fn in sorted(todo)]


def parse_result(text):
    """(verb, rest) from the worker's last ADOPT/DISMISS/UNRESOLVED line, or (None, '')."""
    found = None
    for line in text.splitlines():
        match = RESULT.match(line)
        if match:
            found = (match.group(1), match.group(2).strip())
    return found or (None, '')


def final_text(log):
    """The worker's closing text from an `opencode run --format json` stream, or ''.

    Only text that no tool call follows is a result: a worker that writes its planned
    verdict and keeps working has not finished (review 2026-11-22)."""
    last = ''
    try:
        lines = Path(log).read_text(encoding='utf-8', errors='replace').splitlines()
    except OSError:
        return ''
    for line in lines:
        try:
            row = json.loads(line)
        except ValueError:
            continue
        if not isinstance(row, dict):
            continue
        part = row.get('part') or {}
        if row.get('type') == 'tool_use' or part.get('type') == 'tool':
            last = ''
        elif row.get('type') == 'text' and part.get('type') == 'text' and part.get('text'):
            last = part['text']
    return last


def decide(job, verb, rest, root=None):
    """Check a result line against the job's fixed paths: ('adopt'|'dismiss', args) or ('none', why).

    Relative paths are read against the repository root, as the prompt writes them."""
    receipt, hand = job['receipt'], job['hand']

    def same(given, expected):
        path = Path(given)
        if not path.is_absolute() and root is not None:
            path = Path(root) / path
        try:
            return path.resolve() == Path(expected).resolve()
        except (OSError, ValueError):
            return False

    words = rest.split(None, 2)
    if verb == 'ADOPT':
        if len(words) < 3 or not words[2].strip():
            return 'none', 'ADOPT needs <source> <review_receipt> <note>'
        if not same(words[0], hand) or not same(words[1], receipt):
            return 'none', f'ADOPT names paths other than this job\'s claim ({hand}, {receipt})'
        if not Path(hand).is_file():
            return 'none', 'ADOPT source does not exist'
        return 'adopt', {'source': str(Path(hand).resolve()), 'receipt': receipt, 'note': words[2].strip()[:500]}
    if verb == 'DISMISS':
        words = rest.split(None, 1)
        if len(words) < 2 or not words[1].strip():
            return 'none', 'DISMISS needs <review_receipt> <reason>'
        if not same(words[0], receipt):
            return 'none', f'DISMISS names a receipt other than this job\'s claim ({receipt})'
        return 'dismiss', {'receipt': receipt, 'reason': words[1].strip()[:500]}
    if verb == 'UNRESOLVED':
        return 'none', 'unresolved: ' + rest[:300]
    return 'none', 'no result line'


# --- worker prompt and permissions -----------------------------------------------

def _rel(root, path):
    try:
        return Path(path).resolve().relative_to(Path(root).resolve()).as_posix()
    except ValueError:
        return Path(path).as_posix()


def prompt(root, job):
    run = _rel(root, job['run_dir'])
    receipt, hand = _rel(root, job['receipt']), _rel(root, job['hand'])
    lines = [
        f"RUN={run} {job['fn']}",
        '',
        'You are the farm\'s defect-lane worker. Read docs/agent/FARM.md sections "Defect subagent" and',
        '"Audit checklist", then resolve this one open audit defect exactly as they say.',
        f"1. Claim with exactly this name: `{PY} scripts/cmre_farm.py review {run} {job['fn']} --claim {job['claim']}`.",
        f'   Its review_receipt is {receipt}. Never create another claim.',
        f"2. For a confirmed defect: `{PY} scripts/cmre_farm.py prepare-hand {run} {job['fn']} "
        f'--review-receipt {receipt} --claim-scoped`, which creates {hand}. Edit only that file and gate it with',
        f"   `{PY} scripts/cmre_farm.py test {run} {job['fn']} {hand}` "
        f"(add `--case N` to explain one case).",
        '3. Finish with exactly one of these as your last line:',
        f'   ADOPT {hand} {receipt} <one-line note: what you fixed and the gate result>',
        f'   DISMISS {receipt} <one-line reason citing the listing VAs that disprove each finding>',
        '   UNRESOLVED <why>',
        'ADOPT only a source whose gate passes. DISMISS only when every open finding is disproved from',
        'the listing. Anything you cannot settle from the listing and the gate is UNRESOLVED.',
    ]
    prior = job.get('prior')
    if prior:
        lines += ['', f"An earlier attempt on these same findings ended: {prior['detail'][:600]}"]
        if prior.get('refused'):
            lines += [f"The farm refused that result when it applied it: {prior['refused'][:600]}",
                      'A source the gate passes can still fail the farm\'s own checks: fix what the refusal names.']
        lines += ['Start again from the claimed inputs; do not reuse its claim or hand file.']
    return '\n'.join(lines) + '\n'


def agent_text(root, job):
    """OpenCode 2.x agent: cm-defect's procedure, with edits limited to this job's hand file."""
    import cmre_farm as farm
    import cmre_input_lane as inputs
    rules = [('*', '*', 'deny'), ('read', '*', 'allow'), ('grep', '*', 'allow'), ('glob', '*', 'allow'),
             ('edit', _rel(root, job['hand']), 'allow'), ('write', _rel(root, job['hand']), 'allow')]
    shell = [f'{PY} scripts/cmre_farm.py test *', f'{PY} scripts/cmre_farm.py review *',
             f'{PY} scripts/cmre_farm.py prepare-hand *']
    shell += [f'{PY} -m cmre {command} *' for command in farm.WORKER_COMMANDS if command not in ('test', 'explain')]
    shell += list(inputs.READ_ONLY_SHELL)
    rules += [('shell', command, 'allow') for command in shell]
    # OpenCode applies the last matching rule: these keep redirection and chaining out of the allows.
    rules += [('shell', pattern, 'deny') for pattern in inputs.SHELL_DENY]
    lines = ['---', f"description: CM0102 defect lane {job['fn']}", 'mode: primary', 'steps: 40',
             'permissions:']
    lines += [f'  - {{ action: "{a}", resource: "{r}", effect: {e} }}' for a, r, e in rules]
    lines += ['---', farm.DEFECT_AGENT_BODY.strip(), *farm.AUDIT_AUTHORITY, '']
    return '\n'.join(lines)


# --- the lane --------------------------------------------------------------------

class Lane:
    def __init__(self, run_dir, ops, launcher, workers=2, model=DEFAULT_MODEL, clock=time.time,
                 attempt_minutes=ATTEMPT_MINUTES, stall_minutes=STALL_MINUTES, root=ROOT, only=None,
                 submit=True):
        self.run_dir, self.ops, self.launcher, self.clock = Path(run_dir), ops, launcher, clock
        # A --no-submit smoke run keeps its own state, events and claims beside the live lane's.
        self.dir = self.run_dir / 'defect-lane' if submit else self.run_dir / 'defect-lane' / 'smoke'
        self.prefix = CLAIM_PREFIX if submit else SMOKE_PREFIX
        self.state_path = self.dir / 'state.json'
        self.state = _load(self.state_path)
        for key, default in (('functions', {}), ('limited', {}), ('counter', 0)):
            self.state.setdefault(key, default)
        self.workers, self.model, self.submit = workers, model, submit
        self.attempt_seconds, self.stall_seconds = attempt_minutes * 60, stall_minutes * 60
        self.root = Path(root)
        self.only = set(only) if only else None
        self.running = []          # (job, handle, started)
        self.candidates, self.refreshed, self.queue = {}, None, []

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
        if isinstance(control.get('defect_lane_workers'), int):
            self.workers = max(0, control['defect_lane_workers'])
        if isinstance(control.get('defect_lane_model'), str) and control['defect_lane_model']:
            self.model = control['defect_lane_model']

    def _available(self, model):
        import cmre_farm
        provider = model.split('/', 1)[0]
        tier = 'free' if cmre_farm._is_free_model(model) else 'paid'
        limited = self.state['limited']
        return max(limited.get(provider, 0), limited.get(f'{provider}:{tier}', 0),
                   limited.get(model.split('#', 1)[0], 0)) <= self.clock()

    def _busy(self):
        return {job['fn'] for job, _, _ in self.running}

    def refresh(self, force=False):
        if not force and self.refreshed is not None and self.clock() - self.refreshed < CANDIDATE_REFRESH_SECONDS:
            return
        self.refreshed = self.clock()
        self.reconcile()
        self.candidates = self.ops.candidates()
        if self.only is not None:
            self.candidates = {fn: info for fn, info in self.candidates.items() if fn in self.only}
        self.queue = plan(self.candidates, self.state['functions'], self._busy(), now=self.clock())

    def reconcile(self):
        """Settle submitted requests from the farm's own verdict on them.

        The farm applies a request on a later tick and records `accepted`/`dismissed` or
        `request-failed`. On 22 Nov it refused two gate-passing fn_006ec010 adopts (a cm_x87_*
        call inside a #define) while the lane still counted them as submitted; a refusal now
        frees the defect at once and its reason reaches the next worker."""
        outcomes = self.ops.outcomes() if hasattr(self.ops, 'outcomes') else {}
        for fn, row in self.state['functions'].items():
            if row.get('status') not in WAITING or not row.get('attempts'):
                continue
            attempt = row['attempts'][-1]
            kind = (attempt.get('verb') or '').lower()
            for outcome in outcomes.get(fn, []):
                if outcome.get('at', 0) < row.get('submitted_at', 0):
                    continue
                name = outcome.get('event')
                if name == 'request-failed' and outcome.get('kind') == kind:
                    detail = str(outcome.get('detail', ''))[:600]
                    attempt['farm'] = {'outcome': 'refused', 'detail': detail}
                    row['status'] = 'needs-inputs' if NEEDS_INPUTS.search(detail) else 'farm-refused'
                    self.event('farm-refused', fn=fn, n=attempt.get('n'), verb=attempt.get('verb'),
                               route=row['status'], detail=detail[:300])
                    break
                if (kind == 'adopt' and name == 'accepted' and outcome.get('kind') == 'adopt') or \
                        (kind == 'dismiss' and name == 'dismissed'):
                    attempt['farm'] = {'outcome': 'applied'}
                    row['status'] = 'applied'
                    self.event('farm-applied', fn=fn, n=attempt.get('n'), verb=attempt.get('verb'))
                    break

    def dispatch(self):
        if self.stopping():
            return
        while len(self.running) < self.workers and self.queue:
            fn = self.queue.pop(0)
            info = self.candidates.get(fn)
            if info is None or fn in self._busy():
                continue
            if not self._available(self.model):
                self.queue.insert(0, fn)
                return
            row = self.state['functions'].setdefault(fn, {})
            if row.get('key') != info['key']:
                row.clear()
                row['key'] = info['key']
            attempts = row.setdefault('attempts', [])
            self.state['counter'] += 1
            n = self.state['counter']
            claim = f'{self.prefix}{n}-{fn[3:]}'
            job = {'fn': fn, 'n': n, 'kind': 'defect', 'model': self.model, 'bytes': info['bytes'],
                   'key': info['key'], 'claim': claim, 'run_dir': str(self.run_dir),
                   'receipt': str(self.run_dir / 'reviews' / claim / 'receipt.json'),
                   'hand': str(self.run_dir / 'hand' / f'{fn}-{claim}.c')}
            if attempts:
                job['prior'] = {'detail': attempts[-1].get('detail', '')}
                if attempts[-1].get('farm', {}).get('outcome') == 'refused':
                    job['prior']['refused'] = attempts[-1]['farm'].get('detail', '')
            try:
                handle = self.launcher.start(job, prompt(self.root, job), agent_text(self.root, job))
            except Exception as exc:   # a broken launcher must not crash-loop the lane every 5 minutes
                self.state['limited'][self.model.split('#', 1)[0]] = self.clock() + LAUNCH_PAUSE_SECONDS
                self.event('launch-failed', fn=fn, n=n, model=self.model, error=f'{type(exc).__name__}: {exc}'[:300])
                self.queue.insert(0, fn)
                self.save()
                return
            self.running.append((job, handle, self.clock()))
            row['status'] = 'running'
            self.event('dispatch', fn=fn, n=n, model=self.model, bytes=info['bytes'], claim=claim)
            self.save()

    def collect(self):
        still = []
        for job, handle, started in self.running:
            code = handle.poll()
            if code is None:
                if self.stopping():
                    handle.kill()
                    self._release(job, 'interrupted')
                    self.event('interrupted', fn=job['fn'], n=job['n'], reason='stop')
                    continue
                activity = handle.last_activity() if hasattr(handle, 'last_activity') else None
                stalled = activity is not None and time.monotonic() - activity > self.stall_seconds
                if self.clock() - started > self.attempt_seconds or stalled:
                    handle.kill()
                    self._finish(job, handle, 'stalled' if stalled else 'timeout')
                else:
                    still.append((job, handle, started))
                continue
            self._finish(job, handle, None)
        self.running = still

    def _release(self, job, status):
        row = self.state['functions'].get(job['fn'], {})
        if row.get('status') == 'running':
            row['status'] = status

    def _pause(self, job, key, seconds, event, message):
        self.state['limited'][key] = self.clock() + seconds
        self._release(job, 'limited')
        self.event(event, fn=job['fn'], n=job['n'], model=job['model'], seconds=seconds, message=str(message)[:300])
        self.save()

    def _finish(self, job, handle, interrupted):
        import cmre_farm
        verb, rest = parse_result(final_text(getattr(handle, 'log', '')))
        if verb is None:
            # Limits and provider failures are not attempts unless the worker finished anyway.
            limit = handle.limit_error() if hasattr(handle, 'limit_error') else None
            if limit:
                message, seconds = limit
                provider = job['model'].split('/', 1)[0]
                key = (job['model'].split('#', 1)[0] if cmre_farm.NO_ROUTE.search(message or '')
                       else f'{provider}:free' if cmre_farm._is_free_model(job['model']) else provider)
                return self._pause(job, key, seconds, 'model-limited', message)
            infra = handle.infra_error() if hasattr(handle, 'infra_error') else None
            if infra:
                return self._pause(job, job['model'].split('/', 1)[0], INFRA_PAUSE_SECONDS,
                                   'model-unavailable', infra)
        if interrupted:
            # A killed worker had not finished: whatever it wrote so far is not its result.
            action, detail = 'none', f'{interrupted}: worker killed before finishing'
            verb, rest = None, ''
        else:
            action, detail = decide(job, verb, rest, root=self.root)
        row = self.state['functions'].setdefault(job['fn'], {'key': job['key']})
        attempt = {'n': job['n'], 'model': job['model'], 'verb': verb, 'at': self.clock(),
                   'claim': job['claim'], 'detail': (verb + ' ' + rest if verb else detail)[:600]}
        row.setdefault('attempts', []).append(attempt)
        if action == 'none':
            row['status'] = 'unresolved' if verb == 'UNRESOLVED' else 'no-result'
            self.event('result', fn=job['fn'], n=job['n'], model=job['model'], verb=verb, outcome=row['status'],
                       detail=detail[:300])
            self.save()
            return
        command = self.ops.command(action, job, detail)
        attempt['command'] = command
        if not self.submit:
            row['status'] = 'recorded'
            self.event('result', fn=job['fn'], n=job['n'], model=job['model'], verb=verb, outcome='recorded',
                       command=command)
            self.save()
            return
        submitted_at = self.clock()   # before submit: the farm may apply it before submit returns
        code, output = self.ops.submit(command)
        attempt['submit'] = {'code': code, 'output': output[-400:]}
        row['status'] = 'submitted' if code == 0 else 'submit-unknown' if code is None else 'submit-refused'
        row['submitted_at'] = submitted_at
        self.event('result', fn=job['fn'], n=job['n'], model=job['model'], verb=verb, outcome=row['status'],
                   code=code, output=output[-300:])
        self.save()

    def tick(self):
        self._control()
        self.collect()
        self.refresh(force=not self.queue and not self.running)
        self.dispatch()
        self.save()

    def finished(self):
        return self.stopping() and not self.running

    def run(self, poll=10.0, sleep=time.sleep):
        self.event('lane-start', pid=os.getpid(), workers=self.workers, model=self.model, submit=self.submit)
        try:
            while not self.finished():
                self.tick()
                sleep(poll)
        finally:
            for job, handle, _ in self.running:
                handle.kill()   # a crashed lane must not leave model workers spending
                self._release(job, 'interrupted')
                self.event('interrupted', fn=job['fn'], n=job['n'], reason='lane-exit')
            self.running = []
            self.save()
            self.event('lane-exit')

    def summary(self):
        counts = {}
        for row in self.state['functions'].values():
            counts[row.get('status', 'new')] = counts.get(row.get('status', 'new'), 0) + 1
        verbs = {}
        for line in _events(self.dir / 'events.jsonl'):
            if line.get('event') == 'result':
                verbs[line.get('verb') or 'none'] = verbs.get(line.get('verb') or 'none', 0) + 1
        return {'functions': counts, 'results': verbs, 'running': len(self.running),
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


def lane_summary(run_dir):
    """Defect-lane counts for the farm status line, or None when the lane never ran."""
    path = Path(run_dir) / 'defect-lane' / 'events.jsonl'
    if not path.exists():
        return None
    running, verbs, farm = set(), {}, {'farm-applied': 0, 'farm-refused': 0}
    for event in _events(path):
        name, key = event.get('event'), (event.get('fn'), event.get('n'))
        if name in farm:
            farm[name] += 1
        elif name == 'dispatch':
            running.add(key)
        elif name in ('result', 'model-limited', 'model-unavailable', 'interrupted'):
            running.discard(key)
            if name == 'result':
                verb = event.get('verb') if event.get('outcome') == 'submitted' else None
                verbs[verb or 'none'] = verbs.get(verb or 'none', 0) + 1
        elif name in ('lane-start', 'lane-exit'):
            running.clear()
    return {'running': len(running), 'adopt': verbs.get('ADOPT', 0), 'dismiss': verbs.get('DISMISS', 0),
            'unresolved': verbs.get('UNRESOLVED', 0) + verbs.get('none', 0),
            'applied': farm['farm-applied'], 'refused': farm['farm-refused']}


# --- the real repository ---------------------------------------------------------

class FarmOps:
    def __init__(self, run_dir, root=ROOT, clock=time.time):
        self.run_dir, self.root, self.clock = Path(run_dir), Path(root), clock

    def _read(self, path):
        try:
            return Path(path).read_bytes()
        except OSError:
            return None

    def _reported_open(self):
        """Functions whose open defect the farm has handed over.

        _defect records the defect and queues its one automatic fix in the same step, so a
        defect belongs to the lane only once its last event is `open-defect`. A fix dispatch
        older than STALE_FIX_SECONDS with nothing after it means the farm died during that fix,
        which it never requeues (review 2026-11-22). A defect with neither event stays by hand."""
        import cmre_farm as farm
        last = {}
        events, _ = farm._read_events(self.run_dir / 'events.jsonl', 0)
        for event in events:
            if event.get('event') == 'open-defect' or (event.get('event') == 'dispatch'
                                                       and event.get('kind') == 'fix'):
                last[event.get('fn')] = (event.get('event'), event.get('at') or 0)
        now = self.clock()
        return {fn for fn, (name, at) in last.items()
                if name == 'open-defect' or now - at > STALE_FIX_SECONDS}

    def outcomes(self):
        """The farm's verdicts on adopt/dismiss requests, per function, in event order."""
        import cmre_farm as farm
        out = {}
        events, _ = farm._read_events(self.run_dir / 'events.jsonl', 0)
        for event in events:
            name = event.get('event')
            if (name == 'request-failed' and event.get('kind') in ('adopt', 'dismiss')) or name == 'dismissed' \
                    or (name == 'accepted' and event.get('kind') == 'adopt'):
                out.setdefault(event.get('fn'), []).append(
                    {'at': event.get('at') or 0, 'event': name, 'kind': event.get('kind'),
                     'detail': str(event.get('detail', ''))[:600]})
        return out

    def _others_busy(self):
        """Functions with a queued request or someone else's recent review claim."""
        busy = set()
        for path in (self.run_dir / 'requests').glob('*.json'):
            try:
                busy.add(json.loads(path.read_bytes())['fn'])
            except (OSError, ValueError, KeyError, TypeError):
                continue
        reviews = self.run_dir / 'reviews'
        for receipt in reviews.glob('*/receipt.json'):
            if receipt.parent.name.startswith((CLAIM_PREFIX, SMOKE_PREFIX)):
                continue
            try:
                if self.clock() - receipt.stat().st_mtime < MANUAL_CLAIM_SECONDS:
                    busy.add(json.loads(receipt.read_bytes())['fn'])
            except (OSError, ValueError, KeyError, TypeError):
                continue
        return busy

    def candidates(self):
        import cmre_farm as farm
        try:
            state = json.loads((self.run_dir / 'state.json').read_text(encoding='utf-8'))
        except (OSError, ValueError):
            return {}
        busy = self._others_busy()
        handed_over = self._reported_open()
        quarantined = state.get('quarantined', {})
        out = {}
        for fn, findings in state.get('defects', {}).items():
            if fn in quarantined or fn in busy or fn not in handed_over or not findings:
                continue
            c = self._read(self.root / 'recovery/c' / f'{fn}.c')
            if c is None:
                continue   # claim requires actual source
            identity = farm._identity({'c': c, 'hint': self._read(self.root / 'recovery/hints' / f'{fn}.json')})
            out[fn] = {'bytes': state.get('accepted', {}).get(fn, {}).get('bytes', 0),
                       'key': defect_key(identity, findings), 'findings': len(findings)}
        return out

    def command(self, action, job, args):
        base = [sys.executable, str(self.root / 'scripts/cmre_farm.py'), action, str(self.run_dir), job['fn']]
        if action == 'adopt':
            return base + ['--source', args['source'], '--review-receipt', args['receipt'],
                           '--note', f"defect lane {job['claim']} ({job['model']}): {args['note']}"]
        return base + ['--review-receipt', args['receipt'],
                       '--reason', f"defect lane {job['claim']} ({job['model']}): {args['reason']}"]

    def submit(self, command):
        try:
            done = subprocess.run(command, cwd=self.root, capture_output=True, text=True, encoding='utf-8',
                                  errors='replace', timeout=600)
        except subprocess.TimeoutExpired:
            return None, 'submit timed out after 600 s; the request state is unknown'
        return done.returncode, (done.stdout or '') + (done.stderr or '')


class Launcher:
    """opencode 2.x worker with a per-job agent file (the farm's launcher, defect-lane agent)."""

    def __init__(self, root, run_dir):
        import cmre_farm
        self.farm = cmre_farm
        self.inner = cmre_farm.OpencodeV2Launcher(root, Path(run_dir) / 'defect-lane')

    def start(self, job, prompt_text, agent):
        farm, inner = self.farm, self.inner
        logs = inner.run_dir / 'logs'
        logs.mkdir(parents=True, exist_ok=True)
        stem = f"{job['fn']}-defect-{job['n']}"
        (logs / f'{stem}.prompt.txt').write_text(prompt_text, encoding='utf-8')
        name = re.sub(r'[^A-Za-z0-9-]', '-', f"defect-{inner.run_dir.parent.name}-{stem}")
        agent_path = inner.root / '.opencode/agents' / f'{name}.md'
        agent_path.parent.mkdir(parents=True, exist_ok=True)
        agent_path.write_text(agent, encoding='utf-8')
        inner._wait_for_agent(name)
        command = [inner.executable, 'run', '--agent', name, '-m', job['model'],
                   '--format', 'json', '--title', f"farm defect {job['fn']}", prompt_text]
        flags = (subprocess.CREATE_NO_WINDOW | subprocess.CREATE_NEW_PROCESS_GROUP) if os.name == 'nt' else 0
        out = (logs / f'{stem}.jsonl').open('wb')
        process = subprocess.Popen(command, cwd=inner.root, env=dict(os.environ), stdout=out,
                                   stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, creationflags=flags)
        out.close()
        return farm.OpencodeV2Handle(process, logs / f'{stem}.jsonl', agent_path, inner.executable)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest='action', required=True)
    r = sub.add_parser('run', help='resolve open defects until RUN/STOP')
    r.add_argument('run_dir', type=Path)
    r.add_argument('--workers', type=int, default=2)
    r.add_argument('--model', default=DEFAULT_MODEL)
    r.add_argument('--attempt-minutes', type=float, default=ATTEMPT_MINUTES)
    r.add_argument('--only', default='', help='comma-separated fn_X: work only these (smoke runs)')
    r.add_argument('--no-submit', action='store_true',
                   help='record each adopt/dismiss command instead of queuing it (smoke runs on a live farm)')
    s = sub.add_parser('status', help='lane counts and results')
    s.add_argument('run_dir', type=Path)
    p = sub.add_parser('plan', help='the next open defects the lane would work (read-only)')
    p.add_argument('run_dir', type=Path)
    p.add_argument('--top', type=int, default=20)
    a = parser.parse_args(argv)
    if a.action == 'status':
        print(json.dumps(Lane(a.run_dir, None, None).summary()))
        return 0
    ops = FarmOps(a.run_dir)
    if a.action == 'plan':
        lane = Lane(a.run_dir, ops, None)
        lane.refresh(force=True)
        for fn in lane.queue[:a.top]:
            info = lane.candidates[fn]
            print(f"{fn} {info['bytes']:>7} B {info['findings']} finding(s)")
        print(f'{len(lane.queue)} defects, {sum(lane.candidates[f]["bytes"] for f in lane.queue)} B')
        return 0
    import cmre_input_lane
    lane_dir = a.run_dir / 'defect-lane' / ('smoke' if a.no_submit else '')
    with cmre_input_lane.LaneLock(lane_dir):
        lane = Lane(a.run_dir, ops, Launcher(ROOT, a.run_dir), workers=a.workers, model=a.model,
                    attempt_minutes=a.attempt_minutes, only=[f for f in a.only.split(',') if f] or None,
                    submit=not a.no_submit)
        lane.run()
    return 0


if __name__ == '__main__':
    sys.exit(main())
