"""Conservative diagnostic slice checks; not a differential acceptance gate.

Memory-first explain reports slice from raw event zero. A slice shorter than the
recorded CLI window therefore contains the complete event stream. Equality with
the window is ambiguous and stays incomplete. Other report shapes stay unknown.
"""
from __future__ import annotations

import ast
import hashlib
import json
from pathlib import Path


SCHEMA = 'cmre-trace-evidence/1'


def _valid_event(raw: object) -> bool:
    if not isinstance(raw, str):
        return False
    try:
        event = ast.literal_eval(raw)
    except (ValueError, SyntaxError, TypeError, RecursionError):
        return False
    if not isinstance(event, tuple) or len(event) != 4:
        return False
    tag, first, second, third = event
    if tag in ('r', 'w'):
        return (type(first) is int and first >= 0 and type(second) is int
                and second > 0 and type(third) is int and third >= 0)
    if tag == 'c':
        return (isinstance(first, str) and type(second) is int and second >= 0
                and isinstance(third, tuple) and len(third) == second
                and all(type(arg) is int and 0 <= arg <= 0xffffffff for arg in third))
    return False


def describe(report: object, window: object) -> dict:
    """Describe completeness without replaying code or trusting stored flags."""
    result = {'schema': SCHEMA, 'window': window, 'complete': False,
              'scope': 'memory-first raw event streams only', 'sides': {}}
    if type(window) is not int or not 1 <= window <= 10000:
        result['reason'] = 'unknown or invalid diagnostic window'
        return result
    if not isinstance(report, dict):
        result['reason'] = 'report must be a JSON object'
        return result
    difference = report.get('difference')
    if (not isinstance(difference, dict) or difference.get('field') != 'memory'
            or type(difference.get('index', 0)) is not int
            or difference.get('index', 0) != 0 or 'event_context' in report):
        result['reason'] = 'memory-first report with unclipped raw context required'
        return result
    for side in ('original', 'recovered'):
        terminal = report.get(side)
        events = report.get(f'events_{side}_raw')
        outcome = terminal.get('outcome') if isinstance(terminal, dict) else None
        valid_events = (isinstance(events, list) and bool(events)
                        and all(_valid_event(event) for event in events))
        count = len(events) if isinstance(events, list) else None
        if not valid_events:
            reason = 'missing or malformed raw events'
        elif outcome not in ('return', 'fault'):
            reason = 'return or fault outcome required'
        elif count >= window:
            reason = 'slice reaches or exceeds diagnostic window'
        else:
            reason = None
        result['sides'][side] = {'events': count, 'raw_start': 0,
                                 'outcome': outcome, 'complete': reason is None,
                                 'reason': reason}
    result['complete'] = all(side['complete'] for side in result['sides'].values())
    result['reason'] = None if result['complete'] else 'both raw streams must be complete'
    return result


def check_file(path: Path) -> dict:
    """Return a compact retained-file check; legacy window provenance is unknown."""
    path = Path(path)
    try:
        raw = path.read_bytes()
        report = json.loads(raw)
    except (OSError, UnicodeError, ValueError) as exc:
        raise ValueError(f'cannot read diagnostic JSON: {exc}') from exc
    metadata = report.get('trace_evidence') if isinstance(report, dict) else None
    window = (metadata.get('window') if isinstance(metadata, dict)
              and metadata.get('schema') == SCHEMA else None)
    evidence = describe(report, window)
    return {'schema': 'cmre-trace-check/1', 'path': str(path),
            'sha256': hashlib.sha256(raw).hexdigest(),
            'case': report.get('case') if isinstance(report, dict) else None,
            'trace_evidence': evidence,
            'limitation': 'Completeness only; output attribution, native fidelity and acceptance require review.'}
