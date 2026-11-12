"""Durable decisions/events only. The native parent, never Python, launches workers.

Queue lock is short-lived and distinct from the canonical writer/test lease.
Trusted steward validates filesystem pins; this is not a permission sandbox.
"""
from __future__ import annotations

from collections import Counter
import hashlib
import json
import math
from pathlib import Path
import time

if __package__ in (None, ''):
    import sys
    _repo = next(p for p in Path(__file__).resolve().parents
                 if (p / 'cmre/coordination.py').is_file())
    sys.path.insert(0, str(_repo))

from cmre.coordination import atomic_json, lock

SOL = 'openai/gpt-6.1-sol'
MODELS = {SOL, 'opencode-go/deepseek-v4.1-flash',
          'minimax-coding-plan/MiniMax-M3.1-Flash'}
LIVE = {'intent', 'running', 'uncertain'}


def digest(value):
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(',', ':'),
                                    allow_nan=False).encode()).hexdigest()


def check_pin(pin):
    if set(pin) != {'path', 'sha256'}:
        raise ValueError('Expected exact path/sha256 pin')
    data = Path(pin['path']).read_bytes()
    if hashlib.sha256(data).hexdigest() != pin['sha256']:
        raise ValueError('Pin mismatch: ' + pin['path'])
    return data


def positive(value):
    return type(value) is int and value > 0


class Queue:
    def __init__(self, root, *, clock=time.time):
        self.root, self.clock = Path(root), clock
        self.path = self.root / 'queue.json'

    def read(self):
        return json.loads(self.path.read_text(encoding='utf-8'))

    def _commit(self, state, kind, /, **fields):
        # Positional-only arguments preserve event metadata named "state".
        state['events'].append({'seq': len(state['events']) + 1,
                                'at': self.clock(), 'kind': kind, **fields})
        atomic_json(self.path, state)

    def initialize(self, config):
        with lock(self.root / 'queue.lock'):
            if self.path.exists():
                raise ValueError('Existing finite queue cannot be reset')
            for name in ('parent_receipt', 'validated_plan'):
                check_pin(config[name])
            if not config['parent_owner'] or not config['steward']:
                raise ValueError('Named parent and steward required')
            if config['steward_model'] != SOL:
                raise ValueError('Single Sol steward required')
            for name in ('dispatch_budget', 'supply_limit', 'buffer_limit',
                         'ready_limit', 'author_cap', 'reviewer_cap'):
                if not positive(config[name]):
                    raise ValueError('Positive finite integer bounds required')
            for name in ('deadline', 'reserve_gib', 'worker_gib'):
                if type(config[name]) not in (int, float) or not math.isfinite(config[name]):
                    raise ValueError('Finite resource/deadline bounds required')
            if (config['deadline'] <= self.clock() or config['reserve_gib'] < 0
                    or config['worker_gib'] <= 0):
                raise ValueError('Invalid resource/deadline bounds')
            state = {'schema': 'cmre-native-queue/1', 'config': config,
                     'started_at': self.clock(), 'stopped': False, 'tasks': {},
                     'requests': {}, 'seen': {}, 'events': [], 'attempts': 0,
                     'steward_slot': None, 'steward_receipts': 0}
            self._commit(state, 'initialized')

    def event(self, event):
        """Exactly-once event IDs; conflicting replay fails rather than rewrites."""
        with lock(self.root / 'queue.lock'):
            state = self.read()
            identity = event['event_id']
            if not isinstance(identity, str) or not identity:
                raise ValueError('Event ID required')
            fingerprint = digest(event)
            if identity in state['seen']:
                if state['seen'][identity] != fingerprint:
                    raise ValueError('Conflicting duplicate event')
                return state
            self._apply(state, event)
            state['seen'][identity] = fingerprint
            self._commit(state, event['kind'], event=event)
            return state

    def _apply(self, state, event):
        kind, config = event['kind'], state['config']
        now = self.clock()
        if kind == 'prepare':
            task = event['task']
            key = task['id']
            if key in state['tasks']:
                raise ValueError('Frozen task already exists; use a fresh task ID')
            if len(state['tasks']) >= config['supply_limit']:
                raise ValueError('Finite supply budget exhausted')
            if (len({task['author'], task['reviewer'], config['steward']}) != 3
                    or task['author_model'] not in MODELS
                    or task['reviewer_model'] not in MODELS):
                raise ValueError('Distinct workers and authorized exact models required')
            if (not positive(task['max_messages']) or task['max_messages'] > 30
                    or not positive(task['max_seconds']) or task['max_seconds'] > 1200
                    or type(task['remaining_failures']) is not int
                    or not 0 <= task['remaining_failures'] <= 5):
                raise ValueError('Fresh finite worker bounds required')
            if not task['required_checks']:
                raise ValueError('Required independent checks missing')
            occupied = {str(Path(p).resolve()).casefold()
                        for t in state['tasks'].values()
                        for paths in t['definition']['outputs'].values() for p in paths}
            private = Path(config['private_root']).resolve()
            paths = []
            for phase in ('author', 'reviewer'):
                check_pin(task['packets'][phase])
                if not task['outputs'][phase]:
                    raise ValueError('Exact private output paths required')
                for name in task['outputs'][phase]:
                    path = Path(name).resolve()
                    if path == private or not path.is_relative_to(private):
                        raise ValueError('Output outside private root')
                    paths.append(str(path).casefold())
            all_paths = [Path(p) for p in paths]
            if (len(set(paths)) != len(paths) or occupied.intersection(paths)
                    or any(a != b and (a.is_relative_to(b) or b.is_relative_to(a))
                           for a in all_paths for b in all_paths)
                    or any(Path(a).is_relative_to(Path(b)) or Path(b).is_relative_to(Path(a))
                           for a in paths for b in occupied)):
                raise ValueError('Overlapping private outputs')
            state['tasks'][key] = {'definition': task, 'state': 'prepared',
                                   'queued_at': now, 'requests': []}
            return
        if kind == 'stop':
            state['stopped'] = True
            return
        if kind == 'hold':
            task = state['tasks'][event['task']]
            if any(state['requests'][r]['state'] in LIVE for r in task['requests']):
                raise ValueError('Live ownership requires terminal reconciliation')
            if task['state'] in ('steward-running', 'completed'):
                raise ValueError('Use steward receipt for steward-owned task')
            if not event['reason'] or not event['retry_when']:
                raise ValueError('Named local hold/retry condition required')
            task.update(state='held', reason=event['reason'], retry_when=event['retry_when'])
            return
        if kind == 'steward-receipt':
            key = event['task']
            task = state['tasks'][key]
            if state['steward_slot'] != key or event['steward'] != config['steward']:
                raise ValueError('Single canonical steward slot binding mismatch')
            if event['outcome'] not in ('completed', 'held'):
                raise ValueError('Explicit steward completed/held outcome required')
            check_pin(event['receipt'])
            if event['bundle_sha256'] != task['bundle_sha256']:
                raise ValueError('Steward receipt bundle mismatch')
            task.update(state='completed' if event['outcome'] == 'completed' else 'held',
                        steward_receipt=event)
            state['steward_slot'] = None
            state['steward_receipts'] += 1
            return
        request = state['requests'][event['request']]
        task = state['tasks'][request['task']]
        if kind == 'admit':
            if request.get('subagent_id') == event['subagent_id']:
                return
            if request['state'] not in LIVE or not event['subagent_id']:
                raise ValueError('Admission must bind unresolved dispatch intent')
            sid = event['subagent_id']
            if request.get('subagent_id', sid) != sid or any(
                    r.get('subagent_id') == sid and r['id'] != request['id']
                    for r in state['requests'].values()):
                raise ValueError('Subagent identity collision')
            request.update(state='running', subagent_id=sid, admitted_at=now)
            return
        if kind == 'launch-uncertain':
            if request['state'] not in LIVE:
                raise ValueError('Dispatch already terminal')
            request.update(state='uncertain', recovery_reason=event['reason'])
            return
        if kind == 'launch-not-created':
            if request['state'] not in ('intent', 'uncertain') or request.get('subagent_id'):
                raise ValueError('Cannot discard known live ownership')
            if event['steward'] != config['steward']:
                raise ValueError('Steward reconciliation required')
            check_pin(event['evidence'])
            request.update(state='terminal', terminal_at=now, terminal=event)
            task.update(state='held', reason='confirmed-no-native-worker-created')
            return
        if kind == 'terminal' and request['state'] == 'terminal':
            previous = {k: v for k, v in request['terminal'].items() if k != 'event_id'}
            incoming = {k: v for k, v in event.items() if k != 'event_id'}
            if previous == incoming:
                return
        if kind != 'terminal' or request['state'] not in LIVE:
            raise ValueError('Unknown event or terminal already recorded')
        if not request.get('subagent_id') or event['subagent_id'] != request['subagent_id']:
            raise ValueError('Admit/reconcile actual subagent ID before completion')
        observed = event.get('observed_model')
        request.update(state='terminal', terminal_at=now, terminal=event,
                       observed_model=observed, usage=event.get('usage'))
        if (event['outcome'] != 'succeeded' or now >= request['deadline']
                or observed not in (None, request['model'])):
            task.update(state='held', reason='worker-failed-overdue-or-model-mismatch')
            return
        if request['phase'] == 'author':
            bundle = event['bundle']
            if set(bundle) != {'source', 'hints', 'report'}:
                raise ValueError('Exact source/hints/report pins required')
            allowed = task['definition']['outputs']['author']
            for pin in bundle.values():
                if pin['path'] not in allowed:
                    raise ValueError('Artifact not in declared private outputs')
                check_pin(pin)
            task.update(state='needs-review', bundle=bundle,
                        bundle_sha256=digest(bundle), queued_at=now)
        else:
            pin = event['verdict']
            if pin['path'] not in task['definition']['outputs']['reviewer']:
                raise ValueError('Verdict not in declared private outputs')
            verdict = json.loads(check_pin(pin))
            definition = task['definition']
            approved = (verdict.get('schema') == 'cmre-native-queue-review/1'
                        and verdict.get('task') == request['task']
                        and verdict.get('author') == definition['author']
                        and verdict.get('reviewer') == definition['reviewer']
                        and verdict.get('bundle_sha256') == task['bundle_sha256']
                        and verdict.get('pins') == task['bundle']
                        and verdict.get('verdict') == 'approve'
                        and verdict.get('unknowns') == []
                        and set(verdict.get('checks', [])) == set(definition['required_checks']))
            for artifact in task['bundle'].values():
                check_pin(artifact)
            task.update(state='ready' if approved else 'held', review=pin, queued_at=now)
            if not approved:
                task['reason'] = 'independent-review-not-approved'

    def reserve(self, *, owner, available_gib, sampled_at):
        """Persist new intents then release lock BEFORE returning native readyactions.

        Never re-emit pending intents: a lost response requires reconciliation,
        not automatic retry. All pending/live requests continue to consume capacity.
        A measurement funds at most one nonempty native batch, across all phases.
        Fresh measurements already include running workers: do not debit them twice.
        Admit/reconcile the previous batch before measuring again (external protocol).
        """
        with lock(self.root / 'queue.lock'):
            state = self.read()
            config, now = state['config'], self.clock()
            if owner != config['parent_owner']:
                raise ValueError('Parent dispatch owner mismatch')
            if (type(available_gib) not in (int, float) or not math.isfinite(available_gib)
                    or available_gib < 0 or not math.isfinite(sampled_at)):
                raise ValueError('Measured finite headroom required')
            # A missing watermark is a legacy queue, not renewed headroom. Its
            # latest intent time conservatively bounds every old consumed sample.
            consumed = state.get('headroom_consumed_at', max(
                (r['intent_at'] for r in state['requests'].values()), default=None))
            slots = (max(0, int((available_gib - config['reserve_gib']) / config['worker_gib']))
                     if (0 <= now - sampled_at <= 5
                         and (consumed is None or sampled_at > consumed)) else 0)
            actions = []
            live = Counter(r['phase'] for r in state['requests'].values() if r['state'] in LIVE)
            tasks = state['tasks']
            counts = Counter(t['state'] for t in tasks.values())
            debt = sum(counts[s] for s in ('author-pending', 'needs-review',
                                         'reviewer-pending', 'ready', 'steward-running'))
            ready_debt = sum(counts[s] for s in ('reviewer-pending', 'ready', 'steward-running'))
            # Oldest review debt first; author and reviewer caps scale independently.
            for phase, queued in (('reviewer', 'needs-review'), ('author', 'prepared')):
                for key, task in sorted(tasks.items(), key=lambda item: item[1]['queued_at']):
                    if task['state'] != queued:
                        continue
                    if (state['stopped'] or now >= config['deadline'] or not slots
                            or live[phase] >= config[phase + '_cap']
                            or state['attempts'] >= config['dispatch_budget']):
                        break
                    if ((phase == 'author' and debt >= config['buffer_limit'])
                            or (phase == 'reviewer' and ready_debt >= config['ready_limit'])):
                        break
                    definition = task['definition']
                    for pin in (config['parent_receipt'], config['validated_plan'],
                                definition['packets'][phase]):
                        check_pin(pin)
                    if phase == 'reviewer':
                        for pin in task['bundle'].values():
                            check_pin(pin)
                    state['attempts'] += 1
                    rid = f"{key}:{phase}:{state['attempts']}"
                    action = {'id': rid, 'task': key, 'phase': phase, 'owner': owner,
                              'worker': definition[phase], 'model': definition[phase + '_model'],
                              'packet': definition['packets'][phase],
                              'private_outputs': definition['outputs'][phase],
                              'parent_receipt': config['parent_receipt'],
                              'validated_plan': config['validated_plan'],
                              'max_messages': definition['max_messages'],
                              'max_seconds': definition['max_seconds'],
                              'remaining_failures': definition['remaining_failures'],
                              'role': 'WORKER', 'preflight': 'inherit',
                              'deadline': min(config['deadline'], now + definition['max_seconds']),
                              'intent_at': now, 'state': 'intent', 'observed_model': None,
                              'usage': None}
                    if phase == 'reviewer':
                        action.update(bundle=task['bundle'], bundle_sha256=task['bundle_sha256'])
                    state['requests'][rid] = action
                    task['requests'].append(rid)
                    task['state'] = phase + '-pending'
                    live[phase] += 1
                    slots -= 1
                    if phase == 'author':
                        debt += 1
                    else:
                        ready_debt += 1
                    actions.append(dict(action))
            # No native action (including cap denial) leaves a sample unspent.
            # Commit the watermark and intents together; errors commit neither.
            if actions:
                state['headroom_consumed_at'] = sampled_at
            # Metadata slot only; NEVER acquires or substitutes for canonical lease.
            steward_action = None
            if state['steward_slot'] is None:
                for key, task in sorted(tasks.items(), key=lambda item: item[1]['queued_at']):
                    if task['state'] == 'ready':
                        for pin in (*task['bundle'].values(), task['review']):
                            check_pin(pin)
                        task['state'] = 'steward-running'
                        state['steward_slot'] = key
                        steward_action = {'task': key, 'steward': config['steward'],
                                          'bundle': task['bundle'], 'review': task['review'],
                                          'bundle_sha256': task['bundle_sha256']}
                        break
            metrics = self._metrics(state)
            self._commit(state, 'reserved', actions=[a['id'] for a in actions], metrics=metrics)
            return {'native_actions': actions, 'steward_action': steward_action, 'metrics': metrics}

    def _metrics(self, state):
        now = self.clock()
        counts = Counter(t['state'] for t in state['tasks'].values())
        age = lambda name: max((max(0, now - t['queued_at']) for t in state['tasks'].values()
                                if t['state'] == name), default=0)
        live = {key: r for key, r in state['requests'].items() if r['state'] in LIVE}
        debt = sum(counts[s] for s in ('author-pending', 'needs-review',
                                     'reviewer-pending', 'ready', 'steward-running'))
        ready_debt = sum(counts[s] for s in ('reviewer-pending', 'ready', 'steward-running'))
        return {'queues': dict(counts), 'author_queue_age_seconds': age('prepared'),
                'review_queue_age_seconds': age('needs-review'),
                'ready_queue_age_seconds': age('ready'),
                'live_obligations': list(live),
                'overdue_obligations': [key for key, r in live.items() if now >= r['deadline']],
                'drained': not live and state['steward_slot'] is None,
                'stopped': state['stopped'],
                'author_backpressure': debt >= state['config']['buffer_limit'],
                'review_backpressure': ready_debt >= state['config']['ready_limit'],
                'remaining_dispatches': state['config']['dispatch_budget'] - state['attempts'],
                'steward_receipts': state['steward_receipts'],
                'steward_receipts_per_second': state['steward_receipts'] / max(1, now - state['started_at']),
                'acceptance_count': None}


def main(argv=None):
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('root', type=Path)
    parser.add_argument('action', choices=('init', 'event', 'reserve', 'status'))
    parser.add_argument('--input', type=Path)
    args = parser.parse_args(argv)
    queue = Queue(args.root)
    payload = json.loads(args.input.read_text(encoding='utf-8')) if args.input else None
    if args.action == 'init':
        queue.initialize(payload)
        result = queue.read()
    elif args.action == 'event':
        result = queue.event(payload)
    elif args.action == 'reserve':
        result = queue.reserve(**payload)
    else:
        result = queue.read()
    print(json.dumps(result, allow_nan=False))


if __name__ == '__main__':
    main()
