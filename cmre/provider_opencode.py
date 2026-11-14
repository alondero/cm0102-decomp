"""OpenCode actor adapter for the cmre durable controller.

This module binds a durable controller to a real OpenCode client through the
existing ``scripts.cmre_provider_worker.pair`` runner.  It prepares a private
Git workspace, writes two worker specs (smoke then work), dispatches them
through the pair runner and validates the receipts, the observed model and the
single JSON action before anything is returned.  No subprocess lifecycle, no
broker action and no canonical write is implemented here.

Actual provider job/config shape (a mapping, optionally wrapped in ``config``)::

    {
      'goal': str,
      'function': '0x...',
      'allowed_functions': ['0x...'],
      'candidate': '/absolute/private/jobroot/path.c',   # optional
      'directory': '/absolute/campaign/jobs/ID',
      'mode': 'input' | 'c',
      'owner': 'deepseek' | 'minimax',
      'reviewer': 'deepseek' | 'minimax',
      'provider_timeout_seconds': int,                    # optional
      'context_files': [{'path': abs, 'sha256': hex}],
      'seed_fixture': {'path': abs, 'sha256': hex},
      'existing_context': {...}
    }

The actor ``role`` is the provider role (``deepseek`` or ``minimax``); the
operational phase comes from ``context.phase`` (``owner`` or ``review``).
"""
from __future__ import annotations

import hashlib
import json
import math
import os
import re
import shutil
import subprocess
import time
from pathlib import Path

MAX_LINE = 1800
PART_LINES = 100
CHUNK_BUDGET = 1500
MAX_CONTEXT_PARTS = 64
MAX_CONTEXT_BYTES = 256 * 1024
SMOKE_MAX_SECONDS = 60
CLEANUP_RESERVE_SECONDS = 30
MINIMUM_REMAINING_SECONDS = 10
DEFAULT_PROVIDER_TIMEOUT = 180
KNOWN_ROLES = ('deepseek', 'minimax')
KNOWN_PHASES = ('owner', 'review')
KNOWN_MODES = ('input', 'c')
CONTEXT_DIR = 'context'
PROTOCOL_DIR = 'protocol'
PACKET_DIR = 'packet'
PERMISSIONS_FILE = 'packet/permissions.json'
SMOKE_FILE = 'packet/smoke.txt'
ACTION_FILE = 'packet/action.json'
SMOKE_TEXT = 'READY-SCOPE'
RESERVED_ROOTS = (CONTEXT_DIR, PROTOCOL_DIR, PACKET_DIR)
SOURCE_ROOT = 'recovery/c'
U32_MAX = 0xFFFFFFFF

ACTION_KEYS = {
    'evidence': ((), ('function', 'start', 'limit')),
    'memory': (('address', 'size'), ()),
    'probe': (('request',), ('base',)),
    'inspect': (('artifact', 'section'), ('start', 'limit')),
    'build': ((), ()),
    'screen': ((), ()),
    'test': ((), ()),
    'controls': ((), ()),
    'propose': (('artifacts', 'summary'), ('resolution',)),
    'control-plan': (('controls',), ()),
    'review': (('verdict', 'bundle_sha256', 'checks', 'unknowns'), ('reason',)),
    'hold': (('reason',), ()),
}

INPUT_ACTIONS = frozenset(
    {'evidence', 'memory', 'probe', 'inspect', 'screen', 'propose', 'hold'})
C_ACTIONS = frozenset(
    {'evidence', 'memory', 'probe', 'inspect', 'screen', 'build', 'test',
     'controls', 'control-plan', 'propose', 'hold'})
REVIEW_ACTIONS = frozenset(
    {'evidence', 'memory', 'probe', 'inspect', 'screen', 'review', 'hold'})


def _permitted_actions(phase, mode):
    return REVIEW_ACTIONS if phase == 'review' else INPUT_ACTIONS if mode == 'input' else C_ACTIONS

ACTION_ID_RE = re.compile(r'^a\d{4}$')
FILE_ID_RE = re.compile(r'^a\d{4}/artifacts/[A-Za-z0-9._-]+(?:/[A-Za-z0-9._-]+)*$')

# Only these receipt states and call finish labels may be named in a failure
# diagnostic.  Everything else stays unknown rather than being echoed.
SAFE_STATE_LABELS = ('timeout', 'step_limit', 'incomplete', 'deadline')
SAFE_FINISH_LABELS = ('stop', 'length', 'tool-calls')
MAX_DIAGNOSTIC = 160
MAX_DIAGNOSTIC_DIGITS = 15

DENIED_CAPABILITIES = ('network', 'shell', 'delegation', 'skill', 'webfetch', 'question',
                       'todowrite', 'glob', 'grep', 'canonical-writes',
                       'read-outside-enumerated-files', 'write-outside-enumerated-files')

GITIGNORE = '*.tmp\n*.bak\n__pycache__/\n.cache/\n.opencode/\n'


class OpenCodeError(ValueError):
    """A rejected provider request; the durable core owns retry and hold.

    ``retryable`` is true only for a fully terminal, clean-scope stage that can
    be retried with a fresh durable action stage.  ``retained`` carries any
    stage and collection references that were already retained on disk.
    """

    def __init__(self, message, *, retryable=False, retained=None):
        super().__init__(message)
        self.retryable = bool(retryable)
        self.retained = retained if retained is not None else {}


def _digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def _read_text(path):
    try:
        value = Path(path).read_bytes().decode('utf-8-sig')
    except UnicodeError as error:
        raise OpenCodeError(f'Required text is not UTF-8: {path}') from error
    if '\0' in value:
        raise OpenCodeError(f'NUL in required text: {path}')
    return value


def _validate_source_bytes(data, label):
    try:
        text = data.decode('utf-8')
    except UnicodeError as error:
        raise OpenCodeError(f'{label} is not UTF-8') from error
    if '\0' in text:
        raise OpenCodeError(f'{label} contains a NUL byte')
    for number, line in enumerate(text.splitlines(), 1):
        if len(line) > MAX_LINE:
            raise OpenCodeError(f'{label} line {number} exceeds {MAX_LINE} characters')
    return text


def _strict_loads(text):
    def unique_object(pairs):
        result = {}
        for key, item in pairs:
            if key in result:
                raise ValueError(f'Duplicate JSON key: {key}')
            result[key] = item
        return result

    def invalid_constant(item):
        raise ValueError(f'Non-JSON constant: {item}')

    try:
        return json.loads(text, object_pairs_hook=unique_object, parse_constant=invalid_constant)
    except ValueError as error:
        raise OpenCodeError(f'Invalid JSON: {error}') from error


def _dumps(value):
    try:
        return json.dumps(value, indent=2, ensure_ascii=True, allow_nan=False) + '\n'
    except ValueError as error:
        raise OpenCodeError(f'Value is not serializable JSON: {error}') from error


def _finite(value):
    if isinstance(value, float):
        if value != value or value in (float('inf'), float('-inf')):
            raise OpenCodeError('Nonfinite number')
    elif isinstance(value, dict):
        for item in value.values():
            _finite(item)
    elif isinstance(value, list):
        for item in value:
            _finite(item)


def _escaped_length(text):
    return len(json.dumps(text, ensure_ascii=True))


def _chunk_text(text):
    items, current = [], ''
    for character in text:
        candidate = current + character
        if current and _escaped_length(candidate) > CHUNK_BUDGET:
            items.append(current)
            current = character
        else:
            current = candidate
    items.append(current)
    return items


def _chunk_value(text):
    items = _chunk_text(text)
    return {'__cmre_chunks__': {'count': len(items),
                                'items': [{'n': number, 'text': part}
                                          for number, part in enumerate(items, 1)]}}


def encode_context(value):
    """Make every string reversible inside a bounded, readable JSON document."""
    if isinstance(value, dict):
        encoded = {}
        for key, item in value.items():
            if not isinstance(key, str):
                raise OpenCodeError('Context keys must be strings')
            encoded[key] = encode_context(item)
        return encoded
    if isinstance(value, (list, tuple)):
        return [encode_context(item) for item in value]
    if isinstance(value, str):
        if '\n' in value:
            lines = value.split('\n')
            if all(_escaped_length(line) <= CHUNK_BUDGET for line in lines):
                return {'__cmre_multiline__': lines}
            return _chunk_value(value)
        if _escaped_length(value) > CHUNK_BUDGET:
            return _chunk_value(value)
        return value
    if isinstance(value, bool) or value is None or isinstance(value, int):
        return value
    if isinstance(value, float):
        if value != value or value in (float('inf'), float('-inf')):
            raise OpenCodeError('Nonfinite number in context')
        return value
    raise OpenCodeError('Unsupported context value type')


def decode_context(value):
    """Reverse :func:`encode_context` exactly."""
    if isinstance(value, dict):
        if set(value) == {'__cmre_multiline__'}:
            lines = value['__cmre_multiline__']
            if not isinstance(lines, list) or not all(isinstance(line, str) for line in lines):
                raise OpenCodeError('Malformed multiline encoding')
            return '\n'.join(lines)
        if set(value) == {'__cmre_chunks__'}:
            body = value['__cmre_chunks__']
            if not isinstance(body, dict) or not isinstance(body.get('items'), list):
                raise OpenCodeError('Malformed chunk encoding')
            items = body['items']
            if body.get('count') != len(items):
                raise OpenCodeError('Chunk count does not match the chunk list')
            text = []
            for number, item in enumerate(items, 1):
                if not isinstance(item, dict) or item.get('n') != number:
                    raise OpenCodeError('Chunk numbering is not contiguous')
                if not isinstance(item.get('text'), str):
                    raise OpenCodeError('Chunk text must be a string')
                text.append(item['text'])
            return ''.join(text)
        return {key: decode_context(item) for key, item in value.items()}
    if isinstance(value, list):
        return [decode_context(item) for item in value]
    return value


def _normalize_config(job):
    if not isinstance(job, dict):
        raise OpenCodeError('job must be a mapping')
    config = job.get('config', job)
    if not isinstance(config, dict):
        raise OpenCodeError('config must be a mapping')
    return config


def _remaining_seconds(context):
    if not isinstance(context, dict):
        raise OpenCodeError('context must be a mapping')
    value = context.get('remaining_seconds')
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise OpenCodeError('context.remaining_seconds is required')
    remaining = float(value)
    if not remaining > 0:
        raise OpenCodeError('context.remaining_seconds must be positive')
    if remaining <= MINIMUM_REMAINING_SECONDS:
        raise OpenCodeError('Insufficient remaining time for a provider stage')
    return remaining


def _budget(config, remaining):
    provider = config.get('provider_timeout_seconds', DEFAULT_PROVIDER_TIMEOUT)
    if (isinstance(provider, bool) or not isinstance(provider, (int, float))
            or not math.isfinite(provider) or provider < 1):
        raise OpenCodeError('provider_timeout_seconds must be a finite number of at least one second')
    provider = int(provider)
    smoke_limit = config.get('smoke_timeout_seconds', SMOKE_MAX_SECONDS)
    if type(smoke_limit) is not int or not 1 <= smoke_limit <= 180:
        raise OpenCodeError('smoke_timeout_seconds must be 1..180')
    smoke = min(provider, smoke_limit)
    work = int(min(provider, remaining - smoke - CLEANUP_RESERVE_SECONDS))
    if work <= 0:
        raise OpenCodeError('Insufficient remaining time for the work stage')
    return {'provider': provider, 'smoke': int(smoke), 'work': int(work)}


def _required_text(config, key):
    value = config.get(key)
    if not isinstance(value, str) or not value.strip():
        raise OpenCodeError(f'config.{key} is required')
    return value


def _int_in(action, key, low, high):
    if key not in action:
        return
    value = action[key]
    if isinstance(value, bool) or not isinstance(value, int):
        raise OpenCodeError(f'{key} must be an integer')
    if value < low or (high is not None and value > high):
        raise OpenCodeError(f'{key} is out of range')


def _address(value, label):
    if isinstance(value, bool):
        raise OpenCodeError(f'{label} must be an address')
    if isinstance(value, int):
        address = value
    elif isinstance(value, str):
        text = value.strip()
        if not text:
            raise OpenCodeError(f'{label} must be an address')
        try:
            address = int(text, 0)
        except ValueError as error:
            raise OpenCodeError(f'{label} {value!r} is not an address') from error
    else:
        raise OpenCodeError(f'{label} must be an address')
    if address < 0 or address > U32_MAX:
        raise OpenCodeError(f'{label} must be within 0..0x{U32_MAX:X}')
    return address


def _address_in(action, key, *, required=False):
    if key not in action:
        if required:
            raise OpenCodeError(f'Action misses {key}')
        return
    _address(action[key], key)


def _file_id(value, label):
    if not isinstance(value, str) or not FILE_ID_RE.fullmatch(value):
        raise OpenCodeError(
            f'{label} must be an exact aNNNN/artifacts/... file ID, not a path')
    parts = value.split('/')
    if any(part in ('', '.', '..') for part in parts):
        raise OpenCodeError(f'{label} must be an exact retained file ID')
    return value


def _default_runner():
    from scripts import cmre_provider_worker as worker
    return worker.pair


def _default_collector():
    from scripts import cmre_provider_worker as worker
    return worker.collect


def _latest_collection_receipt(stage_dir):
    versions = Path(stage_dir) / 'collections'
    if not versions.is_dir():
        return None
    numbered = [item for item in versions.iterdir() if item.is_dir() and item.name.isdigit()]
    if not numbered:
        return None
    receipt = max(numbered, key=lambda item: item.name) / 'receipt.json'
    return receipt if receipt.is_file() else None


def _provider_validator():
    try:
        from . import provider_probe
    except ImportError:  # pragma: no cover - direct module import
        try:
            import provider_probe  # type: ignore
        except ImportError:
            return None
    validate = getattr(provider_probe, 'validate_request', None)
    return validate if callable(validate) else None


def _diagnostic_count(value):
    """Render one usage count, or ``unknown`` when it is not a safe count."""
    if isinstance(value, bool) or not isinstance(value, int) or value < 0:
        return 'unknown'
    if len(str(value)) > MAX_DIAGNOSTIC_DIGITS:
        return 'unknown'
    return str(value)


def _last_relevant_call(receipt):
    """The last structured call entry of a receipt, or ``None``."""
    calls = receipt.get('calls') if isinstance(receipt, dict) else None
    if not isinstance(calls, list):
        return None
    for entry in reversed(calls):
        if isinstance(entry, dict):
            return entry
    return None


def _failure_diagnostic(receipt):
    """Bounded, safe diagnostic for a stage that did not complete.

    Only the structured receipt state and call accounting are read: the last
    relevant call finish label, its output and reasoning token counts and
    whether usage was observed.  Absent, unobserved, malformed or unrecognized
    values are reported as ``unknown``; no count is invented and no raw text,
    provider error, environment or secret is serialized.
    """
    state = receipt.get('state') if isinstance(receipt, dict) else None
    label = state if state in SAFE_STATE_LABELS else 'unknown'
    finish, output, reasoning, usage = 'unknown', 'unknown', 'unknown', 'unknown'
    call = _last_relevant_call(receipt)
    if call is not None:
        observed_finish = call.get('finish')
        if isinstance(observed_finish, str) and observed_finish in SAFE_FINISH_LABELS:
            finish = observed_finish
        if call.get('usage_observed') is True:
            usage = 'observed'
            output = _diagnostic_count(call.get('output_tokens'))
            reasoning = _diagnostic_count(call.get('reasoning_tokens'))
    suffix = (f'; diagnostics: state={label} finish={finish} output={output} '
              f'reasoning={reasoning} usage={usage}')
    return suffix[:MAX_DIAGNOSTIC]


class OpenCodeActor:
    """Prepare and validate one OpenCode stage for a durable controller."""

    def __init__(self, profiles, parent_receipt, *, runner=None, collector=None):
        if not isinstance(profiles, dict):
            raise OpenCodeError('profiles must be a mapping of role to worker profile')
        self.profiles = dict(profiles)
        self.parent_receipt = Path(parent_receipt)
        self.runner = runner if runner is not None else _default_runner()
        self.collector = collector if collector is not None else _default_collector()

    def __call__(self, job, role, context, stage_path):
        started = time.monotonic()
        if role not in KNOWN_ROLES:
            raise OpenCodeError(f'Unknown role: {role}')
        config = _normalize_config(job)
        mode = config.get('mode', 'c')
        if mode not in KNOWN_MODES:
            raise OpenCodeError(f'Unknown mode: {mode}')
        phase = context.get('phase') if isinstance(context, dict) else None
        if phase not in KNOWN_PHASES:
            raise OpenCodeError('context.phase must be owner or review')
        remaining = _remaining_seconds(context)
        budget = _budget(config, remaining)
        self._configured_roles(config)
        expected_role = config.get('owner' if phase == 'owner' else 'reviewer')
        if expected_role is not None and role != expected_role:
            raise OpenCodeError('The dispatched role differs from the configured phase role')
        profile = self._profile(role)
        self._independent_model(role, profile)
        candidate = self._candidate(job, config, phase)
        if not self.parent_receipt.is_file():
            raise OpenCodeError('parent receipt is missing')
        function = _required_text(config, 'function')
        allowed_functions = config.get('allowed_functions', [])
        if not isinstance(allowed_functions, list) or not all(
                isinstance(item, str) for item in allowed_functions):
            raise OpenCodeError('config.allowed_functions must be a list of strings')
        goal = config.get('goal', '')
        if not isinstance(goal, str):
            raise OpenCodeError('config.goal must be a string')

        stage = Path(stage_path)
        workspace = stage / 'workspace'
        if workspace.exists():
            raise OpenCodeError('Stage workspace already exists; use a fresh stage path')
        stage.mkdir(parents=True, exist_ok=True)
        workspace.mkdir(parents=True)
        (workspace / '.gitignore').write_text(GITIGNORE, encoding='utf-8')

        document = self._context_document(config, context, role, phase, mode)
        context_text = _dumps(encode_context(document))
        _require_bounded(context_text, 'context')
        context_parts = self._write_parts(workspace, CONTEXT_DIR, context_text, '.json')
        context_preflight = stage / 'context-preflight.json'
        exceeded = (["parts"] if len(context_parts) > MAX_CONTEXT_PARTS else [])
        if len(context_text.encode('utf-8')) > MAX_CONTEXT_BYTES:
            exceeded.append('bytes')
        context_preflight.write_text(_dumps({
            'schema': 'cmre-provider-context-preflight/1',
            'state': 'held' if exceeded else 'ready',
            'parts': len(context_parts), 'bytes': len(context_text.encode('utf-8')),
            'limits': {'parts': MAX_CONTEXT_PARTS, 'bytes': MAX_CONTEXT_BYTES},
            'exceeded': exceeded, **({'provider_dispatched': False} if exceeded else {}),
        }), encoding='utf-8')
        if exceeded:
            raise OpenCodeError(
                f'Provider context budget exceeded: {len(context_parts)} parts, '
                f'{len(context_text.encode("utf-8"))} bytes; '
                'compact redundant evidence before a fresh stage; no provider dispatched',
                retained={'context_preflight': str(context_preflight)})
        protocol_text = self._protocol_text(role, phase, mode)
        protocol_parts = self._write_parts(workspace, PROTOCOL_DIR, protocol_text, '.txt')
        part_names = [name for name, _ in context_parts] + [name for name, _ in protocol_parts]
        part_lines = context_parts + protocol_parts

        source_relative = self._prepare_source(workspace, candidate, phase, mode)

        permissions = self._permissions(role, phase, mode, part_names, source_relative)
        (workspace / PERMISSIONS_FILE).parent.mkdir(parents=True, exist_ok=True)
        (workspace / PERMISSIONS_FILE).write_text(_dumps(permissions), encoding='utf-8')

        environment = dict(profile['environment'])
        effective = self._opencode_config(permissions['readable'], permissions['writable'])
        effort = profile.get('effort')
        if effort is not None:
            if effort not in ('low', 'medium', 'high', 'xhigh', 'max'):
                raise OpenCodeError('Profile effort must be low/medium/high/xhigh/max')
            # OpenCode 1.18.3 adapters use Anthropic effort and OpenAI-compatible
            # reasoningEffort respectively. Keep these explicit; replacing the
            # scope config must not silently revert to provider defaults.
            provider_id, model_id = profile['model'].split('/', 1)
            effective['provider'] = {provider_id: {'models': {model_id: {
                'options': {'effort': effort, 'reasoningEffort': effort}}}}}
        environment['OPENCODE_CONFIG_CONTENT'] = _dumps(effective).strip()

        self._init_git(workspace)
        prompts = stage / 'prompts'
        prompts.mkdir(parents=True, exist_ok=True)
        smoke_prompt = prompts / 'smoke.txt'
        work_prompt = prompts / 'work.txt'
        smoke_prompt.write_text(self._prompt('smoke', role, phase, mode, part_lines, source_relative),
                                encoding='utf-8')
        work_prompt.write_text(self._prompt('work', role, phase, mode, part_lines, source_relative),
                               encoding='utf-8')

        allowed = [SMOKE_FILE, ACTION_FILE]
        if source_relative and role == config.get('owner') and mode == 'c' and phase == 'owner':
            allowed.append(source_relative)
        pinned = [str(self.parent_receipt.resolve()), str(smoke_prompt.resolve()),
                  str(work_prompt.resolve()), str(context_preflight.resolve())]
        pinned += [str((workspace / name).resolve()) for name in part_names]
        pinned.append(str((workspace / PERMISSIONS_FILE).resolve()))

        common = {'directory': str(workspace), 'owner': role, 'model': profile['model'],
                  'client_version': profile['client_version'], 'executable': profile['executable'],
                  'environment': environment, 'functions': [function],
                  'parent_receipt': str(self.parent_receipt),
                  'required_text_files': {name: 'text' for name in part_names},
                  'permission_files': [PERMISSIONS_FILE], 'allowed_files': allowed,
                  'smoke_file': SMOKE_FILE, 'pinned_files': pinned, 'max_steps': 30}
        smoke_spec = dict(common, phase='smoke', prompt_file=str(smoke_prompt.resolve()),
                          timeout_seconds=budget['smoke'])
        work_spec = dict(common, phase='work', prompt_file=str(work_prompt.resolve()),
                         timeout_seconds=budget['work'])
        smoke_path = stage / 'smoke-spec.json'
        work_path = stage / 'work-spec.json'
        smoke_path.write_text(_dumps(smoke_spec), encoding='utf-8')
        work_path.write_text(_dumps(work_spec), encoding='utf-8')

        pair_dir = stage / 'pair'
        try:
            record = self.runner(str(smoke_path), str(work_path), str(pair_dir))
        except Exception as error:
            smoke_receipt = self._stage_receipt(pair_dir, 'smoke', {})
            work_receipt = self._stage_receipt(pair_dir, 'work', {})
            collections = self._collect(pair_dir, smoke_receipt, work_receipt)
            retained = {'pair': str(pair_dir / 'receipt.json'),
                        'collections': collections}
            raise OpenCodeError(
                f'pair runner failed: {type(error).__name__}: {error}',
                retained=retained) from error
        pair = record if isinstance(record, dict) else {}
        smoke_receipt = self._stage_receipt(pair_dir, 'smoke', pair)
        work_receipt = self._stage_receipt(pair_dir, 'work', pair)
        collections = self._collect(pair_dir, smoke_receipt, work_receipt)
        receipts = {
            'schema': 'cmre-provider-opencode-receipt/1',
            'phase': phase,
            'role': role,
            'mode': mode,
            'model': profile['model'],
            'observed_models': self._observed(smoke_receipt, work_receipt),
            'stages': {
                'smoke': self._summary('smoke', smoke_receipt,
                                       pair_dir / 'smoke' / 'receipt.json'),
                'work': self._summary('work', work_receipt,
                                      pair_dir / 'work' / 'receipt.json'),
            },
            'collections': collections,
        }
        retained = {'pair': str(pair_dir / 'receipt.json'), 'stages': receipts['stages'],
                    'collections': collections}

        def fail(message, *, retryable=False):
            error = OpenCodeError(message, retryable=retryable, retained=retained)
            raise error

        try:
            self._require_completed('smoke', smoke_receipt, profile['model'], fail)
            self._require_completed('work', work_receipt, profile['model'], fail)
            readiness = smoke_receipt.get('readiness')
            if not isinstance(readiness, dict) or readiness.get('passed') is not True:
                fail('Smoke readiness did not pass')
            smoke_file = workspace / SMOKE_FILE
            if not smoke_file.is_file():
                fail('Smoke file is missing')
            captured = (smoke_receipt.get('source_after') or {}).get(SMOKE_FILE)
            known_markers = {hashlib.sha256((SMOKE_TEXT + ending).encode()).hexdigest()
                             for ending in ('', '\n', '\r\n')}
            # Work has the same audited file scope and can update the harmless
            # marker. Validate the captured smoke artifact before that update.
            marker_proven = captured in known_markers
            if not marker_proven and (SMOKE_TEXT != _read_text(smoke_file).strip()
                    or (captured is not None and captured != _digest(smoke_file))):
                fail('Smoke file does not contain READY-SCOPE')
        except OpenCodeError:
            raise

        action_path = workspace / ACTION_FILE
        if not action_path.is_file():
            fail('The stage wrote no action', retryable=True)
        action = self._action(workspace)
        try:
            self._validate_action(action, role=role, phase=phase, mode=mode,
                                  context=context, config=config)
        except OpenCodeError as error:
            error.retryable = True
            error.retained = retained
            raise

        copied = self._copy_back(workspace, candidate, role, phase, mode, fail)

        notes = ['The pair runner may overrun the reserved export/cleanup budget; elapsed seconds are actual.']
        return {
            'schema': 'cmre-provider-opencode/1',
            'role': role,
            'phase': phase,
            'mode': mode,
            'action': action,
            'model': profile['model'],
            'observed_models': receipts['observed_models'],
            'calls': work_receipt.get('calls'),
            'receipt': receipts,
            'receipts': {
                'pair': self._summary('pair', pair, pair_dir / 'receipt.json'),
                'smoke': receipts['stages']['smoke'],
                'work': receipts['stages']['work'],
            },
            'collections': collections,
            'source': copied,
            'parts': {'context': [name for name, _ in context_parts],
                      'protocol': [name for name, _ in protocol_parts]},
            'workspace': str(workspace),
            'elapsed_seconds': round(time.monotonic() - started, 3),
            'notes': notes,
        }

    def _configured_roles(self, config):
        owner = config.get('owner')
        reviewer = config.get('reviewer')
        if owner is not None and owner not in KNOWN_ROLES:
            raise OpenCodeError('config.owner must be a provider role')
        if reviewer is not None and reviewer not in KNOWN_ROLES:
            raise OpenCodeError('config.reviewer must be a provider role')
        if owner is not None and reviewer is not None and owner == reviewer:
            raise OpenCodeError('config.owner and config.reviewer must differ')

    def _profile(self, role):
        if role not in self.profiles:
            raise OpenCodeError(f'No worker profile for role: {role}')
        raw = self.profiles[role]
        if isinstance(raw, (str, Path)):
            path = Path(raw)
            if not path.is_file():
                raise OpenCodeError(f'Worker profile is missing: {path}')
            data = _strict_loads(_read_text(path))
        elif isinstance(raw, dict):
            data = dict(raw)
        else:
            raise OpenCodeError('Worker profile must be a path or a mapping')
        if not isinstance(data, dict):
            raise OpenCodeError('Worker profile must be a mapping')
        executable = data.get('executable')
        model = data.get('model')
        client_version = data.get('client_version')
        environment = data.get('environment', {})
        if not isinstance(executable, str) or not executable.strip():
            raise OpenCodeError('Worker profile needs an executable')
        if not Path(executable).is_file():
            raise OpenCodeError(f'Worker profile executable is missing: {executable}')
        if not isinstance(model, str) or not model.strip():
            raise OpenCodeError('Worker profile needs a model')
        if not isinstance(client_version, str) or not client_version.strip():
            raise OpenCodeError('Worker profile needs a client_version')
        if not isinstance(environment, dict):
            raise OpenCodeError('Worker profile environment must be a mapping')
        result = {'executable': str(Path(executable).resolve()), 'model': model,
                  'client_version': client_version, 'environment': dict(environment)}
        if 'effort' in data:
            result['effort'] = data['effort']
        return result

    def _independent_model(self, role, profile):
        if 'deepseek' not in self.profiles or 'minimax' not in self.profiles:
            return
        other = 'minimax' if role == 'deepseek' else 'deepseek'
        if self._profile(other)['model'] == profile['model']:
            raise OpenCodeError('The provider role models must differ')

    def _candidate(self, job, config, phase):
        mode = config.get('mode', 'c')
        raw = config.get('candidate', job.get('candidate'))
        if mode == 'input':
            return None
        if raw is None:
            return None
        if not isinstance(raw, str) or not raw.strip():
            raise OpenCodeError('candidate must be a non-empty absolute path')
        directory = config.get('directory', job.get('directory'))
        if not isinstance(directory, str) or not directory.strip():
            raise OpenCodeError('job.directory is required for a source candidate')
        root = Path(directory).resolve(strict=True)
        target = Path(raw)
        if not target.is_absolute():
            raise OpenCodeError('candidate must be an absolute path under job.directory')
        if target.is_symlink():
            raise OpenCodeError('candidate must not be a symbolic link')
        resolved = target.resolve(strict=False)
        if not resolved.is_relative_to(root):
            raise OpenCodeError('candidate escapes job.directory')
        if resolved.is_symlink():
            raise OpenCodeError('candidate must not be a symbolic link')
        exists = resolved.exists()
        if exists:
            if not resolved.is_file():
                raise OpenCodeError('candidate must be a regular file')
            sha256 = _digest(resolved)
        else:
            if phase != 'owner':
                raise OpenCodeError('The reviewer requires an existing candidate')
            sha256 = None
        name = resolved.name
        if not name or name in ('.', '..'):
            raise OpenCodeError('candidate must have a file name')
        relative = f'{SOURCE_ROOT}/{name}'
        if relative.split('/')[0] in RESERVED_ROOTS:  # defensive
            raise OpenCodeError('candidate collides with the packet directories')
        return {'root': root, 'path': resolved, 'relative': relative,
                'sha256': sha256, 'exists': exists}

    def _prepare_source(self, workspace, candidate, phase, mode):
        if not candidate or mode != 'c':
            return None
        target = workspace / candidate['relative']
        target.parent.mkdir(parents=True, exist_ok=True)
        if candidate['exists']:
            data = candidate['path'].read_bytes()
            _validate_source_bytes(data, 'candidate source')
            target.write_bytes(data)
            if _digest(target) != candidate['sha256']:
                raise OpenCodeError('Source copy does not match the original bytes')
        elif phase == 'owner' and not target.exists():
            target.write_text('', encoding='utf-8')
        else:
            raise OpenCodeError('The reviewer requires an existing candidate')
        return candidate['relative']

    def _context_document(self, config, context, role, phase, mode):
        results = context.get('results') or []
        if not isinstance(results, list) or any(not isinstance(result, dict) for result in results):
            raise OpenCodeError('context.results must be a list of objects')
        displayed = {key: value for key, value in context.items() if key not in ('bundle', 'existing')}
        displayed['results'] = [
            {key: value for key, value in result.items() if key != 'payload'}
            for result in results]
        document = {
            'schema': 'cmre-provider-context/1',
            'role': role,
            'phase': phase,
            'mode': mode,
            'goal': config.get('goal', ''),
            'function': _required_text(config, 'function'),
            'allowed_functions': list(config.get('allowed_functions') or []),
            'encoding': {
                'multiline': '{"__cmre_multiline__": [lines]} joins the lines with one newline character',
                'chunks': '{"__cmre_chunks__": {"count": n, "items": [{"n": index, "text": part}]}} '
                          'concatenates the item texts in ascending n order',
                'note': 'Read every context part and every protocol part in full before acting.',
            },
            'context': displayed,
            'existing_context': context.get('existing', config.get('existing_context', {})),
            'artifact_ids': self._artifact_ids(config, context),
        }
        if config.get('adoption_guard') is not None:
            from .pipeline_adoption import obligations
            document['promotion'] = {
                'obligations': obligations(config['adoption_guard'].get('work')),
                'retained_source_requires_reconciliation': bool(config['adoption_guard'].get('source')),
                'required_resolution_fields': ['reason', 'obligations', 'evidence', 'unknowns'],
                'review_note': 'Claims need original-byte evidence and no unresolved obligations.'}
        if 'bundle' in context:
            if not isinstance(context['bundle'], dict):
                raise OpenCodeError('context.bundle must be an object')
            bundle = dict(context['bundle'])
            payload = dict(bundle.get('payload') or {})
            # Native evidence, source and complete emitted code stay intact.
            # Billing and duplicated responses remain in the pinned full bundle.
            payload['records'] = [{key: value for key, value in record.items()
                                   if key in ('id', 'kind', 'action', 'role', 'model',
                                              'hash', 'output_path')}
                                  for record in payload.get('records', [])]
            bundle['payload'] = payload
            bundle['display_note'] = 'Accounting is omitted from this review projection; full records remain pinned.'
            document['bundle'] = bundle
        if 'required_checks' in context:
            document['required_checks'] = context['required_checks']
        seed = config.get('seed')
        if seed is not None:
            document['seed'] = seed
        fixture = self._verified(config.get('seed_fixture'), 'seed_fixture')
        if fixture and phase != 'review':
            document['seed_fixture'] = fixture
        evidence = self._verified(config.get('context_files'), 'context_files')
        if evidence:
            document['context_files'] = evidence
        return document

    def _artifact_ids(self, config, context):
        ids = []
        for ref in context.get('results') or []:
            if not isinstance(ref, dict):
                continue
            for file_id in ref.get('artifacts') or []:
                if isinstance(file_id, str) and FILE_ID_RE.fullmatch(file_id) and file_id not in ids:
                    ids.append(file_id)
        return ids

    def _verified(self, value, label):
        if value is None:
            return []
        entries = [value] if isinstance(value, dict) else value
        if not isinstance(entries, list):
            raise OpenCodeError(f'config.{label} must be a list of mappings')
        result = []
        for entry in entries:
            if not isinstance(entry, dict):
                raise OpenCodeError(f'config.{label} entries must be mappings')
            path, expected = entry.get('path'), entry.get('sha256')
            if not isinstance(path, str) or not path.strip():
                raise OpenCodeError(f'config.{label} entries need an absolute path')
            if not isinstance(expected, str) or len(expected) != 64:
                raise OpenCodeError(f'config.{label} entries need a sha256')
            target = Path(path)
            if not target.is_absolute():
                raise OpenCodeError(f'config.{label} paths must be absolute')
            if not target.is_file():
                raise OpenCodeError(f'config.{label} path is missing: {target.name}')
            actual = _digest(target)
            if actual != expected.lower():
                raise OpenCodeError(f'Stale {label} hash: {target.name}')
            result.append({'name': target.name, 'sha256': actual, 'text': _read_text(target)})
        return result

    def _protocol_text(self, role, phase, mode):
        if phase == 'review':
            source_line = 'You must not edit any source file.'
        elif mode == 'c':
            source_line = 'You may additionally edit the assigned source file; edit nothing else.'
        else:
            source_line = 'You must not edit any source file.'
        mode_line = ('This is C source mode: the answer may change the assigned source file.'
                     if mode == 'c'
                     else 'This is input mode: propose data only and never source.')
        return '\n'.join([
            f'CMRE provider protocol (role={role}, phase={phase}, mode={mode})',
            '',
            'Scope',
            '- The working directory is a private workspace for this stage only.',
            '- Only the files listed in packet/permissions.json are readable and writable.',
            '- Canonical CMRE data, ledger files and hints are never readable or writable.',
            '- No network, shell, delegation, glob or grep capability is available.',
            '- Unknown engine logic stays unknown: report it as unknown instead of guessing.',
            '- Preserve original EAX when the entry return is not proven dead. No indexed direct',
            '  callers does not prove void; indirect use remains unknown. Destructor/free-shaped',
            '  code and a void differential gate do not establish return provenance.',
            '',
            'Reading',
            '- Read every context part and every protocol part completely, with a full read.',
            '- Do not use an offset or a limit when reading a part.',
            '- Context parts are JSON fragments: join them in part order before reading.',
            '- A string may be {"__cmre_multiline__": [lines]}: join the lines with one newline.',
            '- A long string may be {"__cmre_chunks__": {"count": n, "items": [...]}}:',
            '  concatenate the item texts in ascending n order.',
            '',
            'Acting',
            '- Allowed action kinds for this stage: ' + ', '.join(sorted(_permitted_actions(phase, mode))) + '.',
            '- Exactly one action per stage.',
            '- Write packet/action.json as a single JSON object; write it early and concretely.',
            '- A plan, a prose reply or an empty file is not an accepted answer.',
            '- Address fields accept an exact integer or a 0xHEX string.',
            '- Probe shapes must include the root function itself, not only its callees.',
            '- Patch register, stack, global and arena_writes VALUES are JSON integers in',
            '  0..4294967295; hexadecimal strings are address KEYS, never values or byte blobs.',
            '- arena_writes keys are byte offsets from arena base 0x03000000; values are DWORDs.',
            '- Probe request.name is a lowercase slug matching [a-z0-9][a-z0-9_-]{0,63},',
            '  for example "returning_case". Spaces are invalid.',
            '- For a retained C implementation, request build and test before speculative rewriting.',
            '  Repair only the first evidenced discrepancy; avoid whole-packet restatement.',
            '- C controls and control-plan require a passing full test for the current source.',
            '  A passing build alone does not satisfy that prerequisite.',
            '',
            'Actions',
            '- {"kind": "evidence", "function": int|"0x..", "start": int|"0x..", "limit": int <= 120}',
            '- {"kind": "memory", "address": int|"0x..", "size": int <= 128}',
            '- {"kind": "probe", "request": {"function": int|"0x..", "name": str,',
            '  "patch": {"regs": {...}, "stack": [int], "arena_writes": {...}, "globals": {...}},',
            '  "budget": int <= 200000, "shapes": {"0x...": {"ecx": bool, "stack_words": int}}},',
            '  "base": "aNNNN/artifacts/fixture.json"}',
            '- {"kind": "inspect", "artifact": "aNNNN/artifacts/name", "section": str,',
            '  "start": int, "limit": int <= 120}',
            '- {"kind": "screen"}',
            *(['- {"kind": "build"} {"kind": "test"} {"kind": "controls"}']
              if phase == 'owner' and mode == 'c' else []),
            *(['- {"kind": "propose", "artifacts": ["aNNNN"], "summary": str}',
               '  Input promotion may add resolution: {reason: str, obligations: [str],',
               '  evidence: [{va: "0xXXXXXXXX", bytes: "hex", supports: str}], unknowns: []}.']
              if phase == 'owner' else []),
            *(['- C only: {"kind": "control-plan", "controls": [{"site": "0xXXXXXXXX",',
            '  "name": str, "transform": {"old": str, "new": str, "count": int},',
            '  "expect": {"kind": "mismatch", "field": "regs"}}]}.',
            '  Use exact source substitutions at original computation sites; every',
            '  required helper site needs a control. The broker pins/runs the normal gate.']
              if phase == 'owner' and mode == 'c' else []),
            *(['- {"kind": "review", "verdict": "pass" | "revise" | "hold", "bundle_sha256": str,',
               '  "checks": [str], "unknowns": [str], "reason": str}'] if phase == 'review' else []),
            '- {"kind": "hold", "reason": str}',
            '- Every action has exactly the listed keys; any other key is rejected.',
              '- Optional: evidence.function/start/limit, probe.base, inspect.start/limit, review.reason.',
            '',
            'Roles',
            f'- {source_line}',
            f'- {mode_line}',
            '- The owner proposes only retained action IDs (aNNNN).',
            '- Broker base/inspect fields name exact aNNNN/artifacts/... file IDs, never paths.',
            '- The reviewer returns the exact frozen bundle hash.',
            '- A pass review must carry every required check and no unknowns.',
            '- A revise or hold review may carry a subset of checks and nonempty unknowns.',
            '- The reviewer model is different from the owner model.',
            '',
        ])

    def _permissions(self, role, phase, mode, part_names, source_relative):
        readable = list(part_names) + [PERMISSIONS_FILE]
        writable = [SMOKE_FILE, ACTION_FILE]
        if source_relative and mode == 'c':
            readable.append(source_relative)
            if phase == 'owner':
                writable.append(source_relative)
        return {
            'schema': 'cmre-provider-permissions/1',
            'role': role,
            'phase': phase,
            'mode': mode,
            'readable': readable,
            'writable': writable,
            'denied': list(DENIED_CAPABILITIES),
            'canonical_writes': 'denied',
            'note': 'Read and write only the listed files; a missing capability is denied by default.',
        }

    def _opencode_config(self, readable, writable):
        edit = {'*': 'deny'}
        for name in writable:
            edit[name] = 'allow'
        read = {'*': 'deny'}
        for name in readable:
            read[name] = 'allow'
        return {
            'permission': {'*': 'deny', 'bash': 'deny', 'edit': edit,
                           'read': read, 'webfetch': 'deny'},
            'plugin': [],
            'snapshot': False,
            'autoupdate': False,
            'share': 'disabled',
            'tools': {'task': False, 'skill': False, 'webfetch': False,
                      'question': False, 'todowrite': False},
        }

    def _prompt(self, stage, role, phase, mode, part_lines, source_relative):
        listing = '\n'.join(f'- {name} ({count} lines)' for name, count in part_lines)
        if stage == 'smoke':
            body = [
                f'CMRE OpenCode smoke stage (role={role}, phase={phase}, mode={mode}).',
                '',
                'Do the work now; do not answer with a plan.',
                '',
                '1. Read every listed file completely, with a full read and no offset or limit:',
                listing,
                '',
                f'2. Then create {SMOKE_FILE} whose only content is the single line {SMOKE_TEXT}.',
                '3. Write nothing else, anywhere.',
                f'4. {PERMISSIONS_FILE} lists the readable files, the writable files and the denied capabilities.',
            ]
        else:
            body = [
                f'CMRE OpenCode work stage (role={role}, phase={phase}, mode={mode}).',
                '',
                'Do the work now; do not answer with a plan.',
                '',
                '1. Read every listed file completely, with a full read and no offset or limit:',
                listing,
                '',
                f'2. Write {ACTION_FILE} early and concretely: exactly one JSON object holding the',
                '   single action for this stage, following the protocol parts.',
                '   An empty file, a plan or a prose reply is not accepted.',
            ]
            if source_relative and phase == 'owner' and mode == 'c':
                body.append(f'3. You may edit {source_relative}; edit nothing else.')
            else:
                body.append('3. You must not edit any source file.')
            body.append(f'4. {PERMISSIONS_FILE} lists the readable files, the writable files and the denied')
            body.append('   capabilities. Canonical CMRE data is never readable or writable.')
        return '\n'.join(body) + '\n'

    def _write_parts(self, workspace, prefix, text, suffix):
        directory = workspace / prefix
        directory.mkdir(parents=True, exist_ok=True)
        lines = text.splitlines()
        if not lines:
            lines = [' ']
        parts = []
        for number in range(0, len(lines), PART_LINES):
            chunk = list(lines[number:number + PART_LINES])
            while chunk and chunk[-1].strip() == '':
                chunk.pop()
            if not chunk:
                chunk = [' ']
            for line in chunk:
                if len(line) > MAX_LINE:
                    raise OpenCodeError(
                        f'{prefix} part line exceeds {MAX_LINE} characters')
            name = f'{prefix}/part-{number // PART_LINES + 1:04d}{suffix}'
            (workspace / name).write_text('\n'.join(chunk) + '\n', encoding='utf-8')
            parts.append((name, len(chunk)))
        return parts

    def _init_git(self, workspace):
        try:
            subprocess.run(['git', 'init', '-q', str(workspace)], check=True,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=120)
        except (OSError, subprocess.SubprocessError) as error:
            raise OpenCodeError(
                'git init failed before inference; refusing to run the provider') from error

    def _stage_receipt(self, pair_dir, name, pair):
        path = Path(pair_dir) / name / 'receipt.json'
        if path.is_file():
            receipt = _strict_loads(_read_text(path))
            if not isinstance(receipt, dict):
                raise OpenCodeError(f'{name} receipt is not a mapping')
            return receipt
        stages = pair.get('stages')
        if isinstance(stages, list):
            for entry in stages:
                if isinstance(entry, dict) and entry.get('stage') == name:
                    return dict(entry)
        return {}

    def _require_completed(self, name, receipt, model, fail):
        state = receipt.get('state')
        clean = not receipt.get('out_of_scope') and not receipt.get('pin_drift')
        call = _last_relevant_call(receipt)
        length_limited = call is not None and call.get('finish') == 'length'
        retryable = (clean and not length_limited
                     and state in ('timeout', 'step_limit', 'incomplete', 'deadline'))
        if state != 'completed':
            fail(f'{name} stage did not complete: {state}{_failure_diagnostic(receipt)}',
                 retryable=retryable)
        if receipt.get('out_of_scope'):
            fail(f'{name} stage changed files outside the scope')
        if receipt.get('pin_drift'):
            fail(f'{name} stage drifted from its pinned inputs')
        exit_code = receipt.get('exit_code')
        if exit_code is not None and exit_code != 0:
            fail(f'{name} stage exited with {exit_code}', retryable=retryable)
        if receipt.get('finished') is False:
            fail(f'{name} stage did not finish its session')
        requested = receipt.get('model_requested')
        if requested is not None and requested != model:
            fail(f'{name} stage requested another model')
        observed = receipt.get('models_observed')
        if observed != [model]:
            fail(f'{name} stage observed another model: {observed}')

    def _action(self, workspace):
        path = workspace / ACTION_FILE
        action = _strict_loads(_read_text(path))
        if not isinstance(action, dict):
            raise OpenCodeError('The action must be one JSON object')
        _finite(action)
        return action

    def _validate_action(self, action, *, role, phase, mode, context, config):
        kind = action.get('kind')
        if not isinstance(kind, str) or kind not in ACTION_KEYS:
            raise OpenCodeError('Unknown action kind')
        required, optional = ACTION_KEYS[kind]
        allowed = set(required) | set(optional) | {'kind'}
        extra = sorted(set(action) - allowed)
        if extra:
            raise OpenCodeError(f'Unknown action keys: {extra}')
        for key in required:
            if key not in action:
                raise OpenCodeError(f'Action {kind} misses {key}')

        permitted = _permitted_actions(phase, mode)
        if kind not in permitted:
            raise OpenCodeError(f'Action {kind} is not permitted in phase {phase} mode {mode}')

        if kind == 'evidence':
            _address_in(action, 'function')
            _address_in(action, 'start')
            _int_in(action, 'limit', 1, 120)
        elif kind == 'memory':
            _address_in(action, 'address', required=True)
            _int_in(action, 'size', 1, 128)
        elif kind == 'probe':
            self._validate_probe(action, config)
        elif kind == 'inspect':
            _file_id(action['artifact'], 'inspect artifact')
            if not isinstance(action['section'], str) or not action['section'].strip():
                raise OpenCodeError('inspect section must be a string')
            _int_in(action, 'start', 0, None)
            _int_in(action, 'limit', 1, 120)
        elif kind == 'propose':
            artifacts = action['artifacts']
            if not isinstance(artifacts, list) or not artifacts:
                raise OpenCodeError('propose artifacts must be a non-empty list')
            if not all(isinstance(item, str) and ACTION_ID_RE.fullmatch(item)
                       for item in artifacts):
                raise OpenCodeError('propose artifacts must be retained action IDs (aNNNN)')
            if not isinstance(action['summary'], str) or not action['summary'].strip():
                raise OpenCodeError('propose summary must be a string')
            if 'resolution' in action and not isinstance(action['resolution'], dict):
                raise OpenCodeError('propose resolution must be an object')
            retained = set()
            for ref in context.get('results') or []:
                if isinstance(ref, dict) and isinstance(ref.get('id'), str):
                    retained.add(ref['id'])
            unknown = [item for item in artifacts if item not in retained]
            if unknown:
                raise OpenCodeError(f'Proposal names evidence that was not retained: {unknown}')
        elif kind == 'review':
            self._validate_review(action, context)
        elif kind == 'hold':
            if not isinstance(action['reason'], str) or not action['reason'].strip():
                raise OpenCodeError('hold reason must be a string')

    def _validate_review(self, action, context):
        verdict = action['verdict']
        if verdict not in ('pass', 'revise', 'hold'):
            raise OpenCodeError('review verdict must be pass, revise or hold')
        bundle_sha = action['bundle_sha256']
        if not isinstance(bundle_sha, str) or len(bundle_sha) != 64:
            raise OpenCodeError('review bundle_sha256 must be a sha256')
        checks = action['checks']
        if not isinstance(checks, list) or not all(isinstance(item, str) for item in checks):
            raise OpenCodeError('review checks must be a list of names')
        unknowns = action['unknowns']
        if not isinstance(unknowns, list) or not all(isinstance(item, str) for item in unknowns):
            raise OpenCodeError('review unknowns must be a list of strings')
        if 'reason' in action and not isinstance(action['reason'], str):
            raise OpenCodeError('review reason must be a string')

        bundle = context.get('bundle')
        expected_bundle = bundle.get('sha256') if isinstance(bundle, dict) else None
        expected_checks = context.get('required_checks')
        if expected_bundle is None:
            expected_bundle = context.get('bundle_sha256')
        if expected_checks is None:
            expected_checks = context.get('checks')
        if not isinstance(expected_bundle, str) or not isinstance(expected_checks, list):
            raise OpenCodeError('The reviewer context lacks the frozen bundle and checks')
        if bundle_sha != expected_bundle:
            raise OpenCodeError('The review must carry the exact frozen bundle hash')
        required = [item for item in expected_checks if isinstance(item, str)]
        if verdict == 'pass':
            missing = [item for item in required if item not in checks]
            if missing:
                raise OpenCodeError('The pass review misses required checks: ' + ', '.join(missing))
            if unknowns:
                raise OpenCodeError('A pass review cannot retain unresolved unknowns')
        else:
            extra = [item for item in checks if item not in required]
            if extra:
                raise OpenCodeError('The review names unknown checks: ' + ', '.join(extra))

    def _validate_probe(self, action, config):
        request = action['request']
        if not isinstance(request, dict):
            raise OpenCodeError('probe request must be an object')
        if 'base' in action:
            _file_id(action['base'], 'probe base')
        validator = _provider_validator()
        allowed = config.get('allowed_functions') or [config.get('function')]
        if validator is not None:
            try:
                validator(request, allowed_functions=allowed)
            except ValueError as error:
                raise OpenCodeError(str(error)) from error
            return
        allowed_keys = {'function', 'name', 'patch', 'budget', 'shapes'}
        extra = sorted(set(request) - allowed_keys)
        if extra:
            raise OpenCodeError(f'Unknown probe request keys: {extra}')
        for key in ('function', 'name'):
            if key not in request:
                raise OpenCodeError(f'probe {key} is required')
        _address(request['function'], 'probe function')
        if not isinstance(request['name'], str) or not request['name'].strip():
            raise OpenCodeError('probe name must be a string')
        patch = request.get('patch', {})
        if not isinstance(patch, dict):
            raise OpenCodeError('probe patch must be an object')
        budget = request.get('budget')
        if budget is not None and (isinstance(budget, bool) or not isinstance(budget, int)
                                   or not 1 <= budget <= 200000):
            raise OpenCodeError('probe budget must be 1..200000')
        shapes = request.get('shapes')
        if not isinstance(shapes, dict):
            raise OpenCodeError('probe shapes must be an object')
        root = str(_address(request['function'], 'probe function'))
        if root not in shapes:
            raise OpenCodeError('probe shapes must supply the explicit root ABI')
        for key, shape in shapes.items():
            _address(key, 'probe shape key')
            if not isinstance(shape, dict) or set(shape) != {'ecx', 'stack_words'}:
                raise OpenCodeError('probe shapes need ecx and stack_words')
            if not isinstance(shape['ecx'], bool):
                raise OpenCodeError('probe shape ecx must be a boolean')
            if isinstance(shape['stack_words'], bool) or not isinstance(shape['stack_words'], int) \
                    or shape['stack_words'] < 0:
                raise OpenCodeError('probe shape stack_words must be a nonnegative integer')

    def _copy_back(self, workspace, candidate, role, phase, mode, fail):
        if not candidate:
            return {'copied_back': False, 'relative': None, 'sha256': None}
        if phase != 'owner' or mode != 'c':
            return {'copied_back': False, 'relative': candidate['relative'], 'sha256': None}
        source = workspace / candidate['relative']
        if not source.is_file():
            fail('The owner did not create the assigned source file')
        data = source.read_bytes()
        _validate_source_bytes(data, 'workspace source')
        target = candidate['path']
        if target.is_symlink():
            fail('candidate must not be a symbolic link')
        changed = not target.is_file() or data != target.read_bytes()
        if changed:
            target.parent.mkdir(parents=True, exist_ok=True)
            temporary = target.with_name(target.name + '.cmre-tmp')
            temporary.write_bytes(data)
            os.replace(temporary, target)
        return {'copied_back': True, 'relative': candidate['relative'],
                'sha256': hashlib.sha256(data).hexdigest(), 'changed': changed}

    def _collect(self, pair_dir, smoke_receipt, work_receipt):
        collections = []
        for name, receipt in (('smoke', smoke_receipt or {}), ('work', work_receipt or {})):
            stage_dir = Path(pair_dir) / name
            if not stage_dir.is_dir():
                collections.append({'stage': name, 'usage_status': 'missing',
                                    'calls': None, 'models_observed': None,
                                    'export_path': None, 'recovered_export': False,
                                    'receipt_path': None, 'receipt_sha256': None,
                                    'state': receipt.get('state'),
                                    'error': 'stage directory is missing'})
                continue
            try:
                result = self.collector(str(stage_dir))
            except Exception as error:
                result = {'usage_status': 'unknown', 'calls': None,
                          'error': f'{type(error).__name__}: {error}'}
            if not isinstance(result, dict):
                result = {'usage_status': 'unknown', 'calls': None,
                          'error': 'collector returned no receipt'}
            collected = result.get('usage_status') == 'collected'
            collection_receipt = _latest_collection_receipt(stage_dir)
            collections.append({
                'stage': name,
                'state': receipt.get('state'),
                'usage_status': result.get('usage_status'),
                'calls': result.get('calls') if collected else None,
                'models_observed': result.get('models_observed'),
                'export_path': result.get('export_path'),
                'recovered_export': result.get('recovered_export'),
                'receipt_path': str(collection_receipt) if collection_receipt else None,
                'receipt_sha256': _digest(collection_receipt) if collection_receipt else None,
                'error': result.get('error'),
            })
        return collections

    def _observed(self, *receipts):
        observed = []
        for receipt in receipts:
            value = (receipt or {}).get('models_observed')
            if isinstance(value, list):
                for item in value:
                    if isinstance(item, str) and item not in observed:
                        observed.append(item)
        return observed

    def _summary(self, name, receipt, path):
        receipt = receipt or {}
        summary = {
            'stage': name,
            'state': receipt.get('state'),
            'model_requested': receipt.get('model_requested'),
            'models_observed': receipt.get('models_observed'),
            'exit_code': receipt.get('exit_code'),
            'changed_files': receipt.get('changed_files'),
            'out_of_scope': receipt.get('out_of_scope'),
            'pin_drift': receipt.get('pin_drift'),
            'calls': receipt.get('calls'),
            'elapsed_seconds': receipt.get('elapsed_seconds'),
            'receipt_path': str(path) if Path(path).is_file() else None,
            'receipt_sha256': _digest(path) if Path(path).is_file() else None,
        }
        readiness = receipt.get('readiness')
        if isinstance(readiness, dict):
            summary['readiness'] = {
                'passed': readiness.get('passed'),
                'missing_reads': readiness.get('missing_reads'),
                'write_proven': readiness.get('write_proven'),
                'errors': readiness.get('errors'),
            }
        return summary


def _require_bounded(text, label):
    for number, line in enumerate(text.splitlines(), 1):
        if len(line) > MAX_LINE:
            raise OpenCodeError(f'{label} line {number} exceeds {MAX_LINE} characters')
