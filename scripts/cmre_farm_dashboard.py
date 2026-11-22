"""Serve a local view of recovery farm activity and reported supply work."""
from __future__ import annotations

import argparse
from collections import deque
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
import os
from pathlib import Path
import re
import sys
from threading import Lock
import time
from urllib.parse import parse_qs, urlsplit

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

from scripts import cmre_farm, cmre_provider_worker

ASSETS = Path(__file__).with_name('farm_dashboard')
LOCAL = ROOT / 'CM0102/Analysis/cmre'
ASSET_ROUTES = {'/': ('index.html', 'text/html'), '/dashboard.css': ('style.css', 'text/css'),
                '/dashboard.js': ('app.js', 'text/javascript')}
ACTIVITY_FIELDS = ('at', 'event', 'fn', 'n', 'kind', 'confirm', 'model', 'provider', 'outcome', 'bytes',
                   'until', 'idle', 'coverage', 'dry_minutes')
EVENT_GROUPS = {
    'supply': {'supply-empty', 'supply-resumed', 'input-needed', 'input-arrived', 'hint-evidence-needed',
               'fix-evidence-needed', 'recheck', 'retry'},
    'workers': {'dispatch', 'result', 'interrupted', 'model-limited', 'model-unavailable'},
    'recovery': {'accepted', 'open-defect', 'dismissed', 'acceptance-blocked', 'acceptance-quarantined',
                 'request-failed'},
    'farm': {'farm-start', 'farm-exit', 'farm-restart', 'farm-crash-loop', 'config-changed'},
}
EVENT_CATEGORY = {name: group for group, names in EVENT_GROUPS.items() for name in names}
ID_PATTERN = re.compile(r'[a-zA-Z0-9_-]{1,80}')


def read_json(path, *, share_delete=False):
    try:
        if os.name == 'nt' and share_delete:
            # The farm and the set command replace these files while the dashboard reads.
            import _winapi
            import msvcrt
            share_read_write_delete = 0x07
            handle = _winapi.CreateFile(str(path), _winapi.GENERIC_READ, share_read_write_delete,
                                        0, _winapi.OPEN_EXISTING, 0, 0)
            try:
                fd = msvcrt.open_osfhandle(handle, os.O_RDONLY | os.O_BINARY)
            except OSError:
                _winapi.CloseHandle(handle)
                raise
            with os.fdopen(fd, 'rb') as source:
                raw = source.read()
        else:
            raw = path.read_bytes()
        data = json.loads(raw)
        return data if isinstance(data, dict) else {}
    except (OSError, ValueError):
        return {}


def dispatch_row(event):
    return {k: event[k] for k in ('fn', 'model', 'kind', 'n', 'bytes', 'module', 'confirm') if k in event} | {
        'stage': 'model', 'since': event['at']}


class RunEvents:
    """Read each complete journal line once and retain bounded activity for the UI."""

    def __init__(self):
        self.offset = 0
        self.lifecycle = {}
        self.start = {}
        self.settings = {}
        self.dispatches = {}
        self.accepted = {}
        self.hourly = {}
        self.recent = {group: deque(maxlen=35) for group in ('all', *EVENT_GROUPS)}
        self.last_at = None
        self.last_settings = None

    def update(self, path):
        try:
            if path.stat().st_size < self.offset:
                self.__init__()
        except OSError:
            return
        events, self.offset = cmre_farm._read_events(path, self.offset)
        for event in events:
            name, at = event.get('event'), event.get('at')
            if not isinstance(at, (int, float)):
                continue
            self.last_at = at
            if name in EVENT_GROUPS['farm'] - {'config-changed'}:
                self.lifecycle = event
                self.dispatches.clear()
                if name == 'farm-start':
                    self.start = event
                    self.settings = {key: event[key] for key in cmre_farm.CONTROL_KEYS if key in event}
                    self.last_settings = None
            if name == 'config-changed':
                self.settings.update({key: pair[1] for key, pair in event['changes'].items()})
                self.last_settings = at
            if name == 'dispatch':
                self.dispatches[event['n']] = dispatch_row(event)
            if name in ('result', 'model-unavailable', 'model-limited', 'interrupted'):
                self.dispatches.pop(event.get('n'), None)
            if name == 'accepted':
                self.accepted[event['fn']] = event['bytes']
                # Fixes and adoptions replace source. Count translation output here.
                if event.get('kind') == 'translate':
                    bucket = self.hourly.setdefault(int(at // 3600), {'bytes': 0, 'functions': 0})
                    bucket['bytes'] += event['bytes']
                    bucket['functions'] += 1
            group = EVENT_CATEGORY.get(name)
            if group:
                row = {key: event[key] for key in ACTIVITY_FIELDS if key in event}
                if name == 'config-changed':
                    row['changes'] = {key: value for key, value in event['changes'].items()
                                      if key in cmre_farm.CONTROL_KEYS or key == 'deadline_hours'}
                self.recent['all'].appendleft(row)
                self.recent[group].appendleft(row)

    def throughput(self, end):
        hour = int(end // 3600)
        return [{'at': h * 3600, **self.hourly.get(h, {'bytes': 0, 'functions': 0})}
                for h in range(hour - 5, hour + 1)]


def process_alive(pid):
    try:
        return cmre_provider_worker.identity(pid) is not None
    except (OSError, ValueError):
        return None


def run_status(directory, events, snapshot, now):
    last = events.lifecycle
    event = last.get('event')
    if event == 'farm-crash-loop':
        return 'crash loop'
    if event == 'farm-restart':
        return 'restarting'
    if event != 'farm-start':
        return 'stopped' if event == 'farm-exit' else 'unknown'
    age_limit = max(60, 3 * snapshot.get('poll_seconds', 10))
    alive = process_alive(last.get('pid'))
    if alive is False:
        return 'interrupted'
    if snapshot and now - snapshot['at'] > age_limit:
        return 'stale'
    if alive is None:
        return 'unverified'
    if not snapshot:
        try:
            if now - (directory / 'state.json').stat().st_mtime > cmre_farm.STALE_SECONDS:
                return 'stale'
        except OSError:
            return 'unverified'
        return 'legacy live'
    if snapshot['stopping'] or (directory / 'STOP').exists():
        return 'draining'
    return 'live'


class Dashboard:
    def __init__(self, farm_root=LOCAL / 'farm', reports=LOCAL / 'farm-activity', extra_runs=()):
        self.farm_root = farm_root.resolve()
        self.reports = reports
        self.extra_runs = tuple(path.resolve() for path in extra_runs)
        self.events = {}
        self.run_lock = Lock()
        self.inventory_lock = Lock()
        self.inventory_cache = {}

    def overview(self, requested=None):
        with self.run_lock:
            return self._overview(requested)

    def _overview(self, requested):
        paths = self.extra_runs + tuple(p for p in self.farm_root.glob('*') if p.is_dir())
        runs = []
        now = time.time()
        for directory in dict.fromkeys(paths):
            if not (directory / 'events.jsonl').is_file():
                continue
            events = self.events.setdefault(directory, RunEvents())
            events.update(directory / 'events.jsonl')
            snapshot = read_json(directory / 'live.json')
            # The current run must have emitted this snapshot after its own start.
            if (snapshot.get('schema') != 'cmre-farm-live/1'
                    or snapshot.get('pid') != events.start.get('pid')
                    or snapshot.get('at', 0) < events.start.get('at', 0)):
                snapshot = {}
            status = run_status(directory, events, snapshot, now)
            runs.append((directory, events, snapshot, status))

        runs.sort(key=lambda row: (row[3] in ('live', 'draining', 'legacy live', 'stale', 'restarting', 'unverified'),
                                   row[1].last_at or 0), reverse=True)
        selected = next((row for row in runs if row[0].as_posix() == requested), None) if requested else None
        selected = selected or next(iter(runs), None)
        supply = [row for path in self.reports.glob('*.json') if (row := read_json(path)).get('expires_at', 0) > now]
        return {'at': now, 'runs': [{'id': path.as_posix(), 'name': path.name, 'status': status}
                                    for path, _, _, status in runs],
                'run': self.describe(*selected, now) if selected else None,
                'supply': sorted(supply, key=lambda row: row['id'])}

    @staticmethod
    def describe(directory, events, snapshot, status, now):
        running = status in ('live', 'draining', 'legacy live')
        stopped = status in ('stopped', 'interrupted', 'crash loop')
        state = read_json(directory / 'state.json', share_delete=True) if status == 'stopped' and not snapshot else {}
        accepted = {fn: row['bytes'] for fn, row in state['accepted'].items()} if 'accepted' in state else events.accepted
        counts = {
            'accepted_functions': len(accepted), 'accepted_bytes': sum(accepted.values()),
            'parked': len(state['parked']) if 'parked' in state else None,
            'input_needed': len(state.get('input_needed', {})) if state else None,
            'input_bytes': sum(row.get('bytes', 0) for row in state.get('input_needed', {}).values()) if state else None,
            'defects': len(state['defects']) if 'defects' in state else None,
        }
        if snapshot:
            counts.update(snapshot['counts'])
        settings = snapshot.get('settings', events.settings)
        control = read_json(directory / 'control.json', share_delete=True) if snapshot and not stopped else {}
        pending = {key: control[key] for key in cmre_farm.CONTROL_KEYS
                   if key in control and control[key] != settings.get(key)}
        return {
            'id': directory.as_posix(), 'name': directory.name, 'status': status,
            'pid': events.start.get('pid'), 'last_at': snapshot.get('at', events.last_at) if not stopped else events.last_at,
            'settings': settings, 'settings_changed_at': events.last_settings, 'requested_settings': pending,
            'counts': counts, 'scheduler': snapshot.get('scheduler'), 'verifier_slots': snapshot.get('verifier_slots'),
            'telemetry': bool(snapshot),
            'jobs': snapshot.get('jobs', list(events.dispatches.values())) if not stopped else [],
            'activity': {name: list(rows) for name, rows in events.recent.items()},
            'throughput': events.throughput(now if running else events.last_at or now),
        }

    def inventory(self):
        # Inventory is independent of the fast farm poll and cached across tabs.
        with self.inventory_lock:
            now = time.time()
            if now - self.inventory_cache.get('at', 0) >= 60:
                try:
                    self.inventory_cache = {'at': now, **inventory_summary()}
                except (OSError, ValueError, KeyError, RuntimeError):
                    self.inventory_cache = {'at': now, 'error': 'Local inventory unavailable. Check cmre doctor.'}
            return self.inventory_cache


def inventory_summary():
    from cmre import env, inventory, pipeline, queue, work

    # Missing local inputs are different from an empty pipeline.
    if not env.REGISTRY.is_file():
        raise FileNotFoundError(env.REGISTRY)
    sizes = {f'fn_{va:08x}': f.body_bytes for va, f in inventory.load().functions.items()}
    bands = pipeline.survey().bands
    records = work.read()['functions']
    held = {fn: row for fn, row in records.items() if row.get('state') in ('evidence-needed', 'harness-blocker')}
    families = []
    for name, row in work.blocker_families(held).items():
        ids = row['functions']
        families.append({'name': name, 'functions': len(ids), 'bytes': sum(sizes.get(fn, 0) for fn in ids)})
    coverage = queue.coverage_summary()
    return {'coverage': {key: coverage[key] for key in (
        'inventory_body_bytes', 'source_percent', 'source_body_bytes', 'source_functions',
        'recovered_percent', 'recovered_body_bytes', 'recovered_functions', 'integrated_functions')},
            'bands': {name: {'functions': row.functions, 'bytes': row.body_bytes} for name, row in bands.items()},
            'families': sorted(families, key=lambda row: (-row['bytes'], row['name']))[:10]}


def handler_for(dashboard):
    class Handler(BaseHTTPRequestHandler):
        def reply(self, status, body, content_type):
            self.send_response(status)
            self.send_header('Content-Type', content_type + '; charset=utf-8')
            self.send_header('Cache-Control', 'no-store')
            self.send_header('Content-Security-Policy', "default-src 'self'; object-src 'none'; frame-ancestors 'none'; style-src 'self' 'unsafe-inline'")
            self.send_header('X-Content-Type-Options', 'nosniff')
            self.send_header('Content-Length', str(len(body)))
            self.end_headers()
            self.wfile.write(body)

        def do_GET(self):
            # Accept loopback hosts only, including when somebody aliases a DNS name to loopback.
            if self.headers.get('Host') not in (f'127.0.0.1:{self.server.server_port}', f'localhost:{self.server.server_port}'):
                self.reply(403, b'Open this dashboard on loopback.', 'text/plain')
                return
            url = urlsplit(self.path)
            if url.path in ASSET_ROUTES:
                file_name, content_type = ASSET_ROUTES[url.path]
                self.reply(200, (ASSETS / file_name).read_bytes(), content_type)
                return
            if url.path == '/favicon.ico':
                self.reply(204, b'', 'image/x-icon')
                return
            if url.path == '/api/overview':
                data = dashboard.overview(parse_qs(url.query).get('run', [None])[0])
            elif url.path == '/api/inventory':
                data = dashboard.inventory()
            else:
                self.reply(404, b'Not found', 'text/plain')
                return
            self.reply(200, json.dumps(data).encode(), 'application/json')

        def log_message(self, format, *args):
            pass  # Avoid one access log line per browser poll.

    return Handler


class ExclusiveServer(ThreadingHTTPServer):
    """One dashboard per port. ThreadingHTTPServer sets SO_REUSEADDR, which on Windows lets a
    second server bind a port that is already listening (review 2026-11-22)."""
    allow_reuse_address = False

    def server_bind(self):
        import socket
        if hasattr(socket, 'SO_EXCLUSIVEADDRUSE'):
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_EXCLUSIVEADDRUSE, 1)
        super().server_bind()


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest='action', required=True)
    serve = commands.add_parser('serve', help='serve the dashboard on loopback')
    serve.add_argument('--port', type=int, default=8042)
    serve.add_argument('--run', type=Path, action='append', default=[], help='include an additional farm run directory')
    report = commands.add_parser('report', help='report an expert session assignment with an expiry')
    report.add_argument('id')
    report.add_argument('--model', required=True)
    report.add_argument('--role', choices=('supply', 'coordinator', 'defect review'), default='supply')
    report.add_argument('--work', required=True, help='short assignment label, such as fixture family and function IDs')
    report.add_argument('--minutes', type=int, default=30)
    finish = commands.add_parser('finish', help='clear an expert session assignment')
    finish.add_argument('id')
    args = parser.parse_args(argv)
    reports = LOCAL / 'farm-activity'
    if args.action == 'serve':
        with ExclusiveServer(('127.0.0.1', args.port), handler_for(Dashboard(extra_runs=args.run))) as server:
            print(f'CM 01/02 pipeline dashboard: http://127.0.0.1:{server.server_port}/', flush=True)
            try:
                server.serve_forever()
            except KeyboardInterrupt:
                pass
        return 0
    if not ID_PATTERN.fullmatch(args.id):
        parser.error('id must use 1-80 letters, digits, underscores or hyphens')
    path = reports / f'{args.id.lower()}.json'
    if args.action == 'finish':
        path.unlink(missing_ok=True)
        return 0
    if not (0 < len(args.work) <= 160 and 0 < len(args.model) <= 80 and 1 <= args.minutes <= 1440):
        parser.error('provide a short work label and model and an expiry of 1-1440 minutes')
    from cmre.coordination import atomic_json

    reports.mkdir(parents=True, exist_ok=True)
    now = time.time()
    atomic_json(path, {'id': args.id.lower(), 'role': args.role, 'model': args.model, 'work': args.work,
                       'reported_at': now, 'expires_at': now + 60 * args.minutes})
    print(f'{args.id} reported for {args.minutes} minutes')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
