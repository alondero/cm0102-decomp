"""Asset-free projections of provider evidence; no recovery/acceptance authority."""
from __future__ import annotations

import copy
import hashlib
import json
import ntpath
import os
from pathlib import Path

READ_LINE_LIMIT = 1800


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def text(path):
    try:
        value = Path(path).read_bytes().decode('utf-8-sig')
    except UnicodeError as error:
        raise ValueError(f'Required text is not UTF-8: {path}') from error
    if '\0' in value:
        raise ValueError(f'NUL in required text: {path}')
    return value


def load(path):
    return json.loads(text(path))


def strict_json(value):
    def unique_object(pairs):
        result = {}
        for key, item in pairs:
            if key in result:
                raise ValueError(f'Duplicate JSON key: {key}')
            result[key] = item
        return result

    def invalid_constant(item):
        raise ValueError(f'Non-JSON constant: {item}')

    return json.loads(value, object_pairs_hook=unique_object, parse_constant=invalid_constant)


def readable_lines(value, path, *, max_line_length=READ_LINE_LIMIT):
    """Reject text the pinned reader cannot prove before spending an inference."""
    lines = value.splitlines()
    if not lines:
        raise ValueError(f'Required text is empty: {path}')
    if lines[-1] == '':
        raise ValueError(f'Required text has terminal blank lines: {path}; normalize a packet copy')
    for number, line in enumerate(lines, 1):
        if len(line) > max_line_length:
            raise ValueError(f'Required text line {number} exceeds {max_line_length} characters: '
                             f'{path}; prepare bounded packet copies')
    return lines


def prepare_packet(sources, target, *, max_line_length=READ_LINE_LIMIT):
    """Copy bounded provider inputs, retaining raw and rendered byte identities."""
    target = Path(target)
    if target.exists():
        raise FileExistsError(target)
    if type(max_line_length) is not int or not 1 <= max_line_length <= READ_LINE_LIMIT:
        raise ValueError(f'Use an integer line bound from 1 through {READ_LINE_LIMIT}')
    if not isinstance(sources, list) or not sources:
        raise ValueError('Select at least one packet input')

    rendered, files, names = {}, {}, {'manifest.json'}
    for item in sources:
        name, kind = item['name'], item['kind']
        if (not isinstance(name, str) or not name or name in ('.', '..')
                or any(c in name for c in '/\\:') or ntpath.isreserved(name)
                or name.casefold() in names
                or name.rstrip(' .') != name):
            raise ValueError(f'Use distinct plain packet filenames: {name}')
        names.add(name.casefold())
        source = Path(item['source'])
        if source.is_symlink() or not source.is_file():
            raise ValueError(f'Packet input must be a regular file: {source}')
        raw = source.read_bytes()
        try:
            value = raw.decode('utf-8-sig')
        except UnicodeError as error:
            raise ValueError(f'Packet input is not UTF-8: {source}') from error
        if '\0' in value:
            raise ValueError(f'NUL in packet input: {source}')
        if kind == 'json':
            value = json.dumps(strict_json(value), indent=2,
                               ensure_ascii=True, allow_nan=False) + '\n'
        elif kind == 'text':
            # Retain raw bytes in provenance; display.splitlines cannot prove
            # a final empty numbered line. Instruction/content lines stay intact.
            value = value.rstrip('\r\n') + '\n'
        else:
            raise ValueError('Packet kind must be text or json')
        readable_lines(value, source, max_line_length=max_line_length)
        rendered[name] = value.encode('utf-8')
        files[name] = {'source': str(source.resolve()), 'kind': kind,
                       'source_sha256': hashlib.sha256(raw).hexdigest(),
                       'sha256': hashlib.sha256(rendered[name]).hexdigest()}
    target.mkdir(parents=True, exist_ok=False)
    for name, value in rendered.items():
        with (target / name).open('xb') as output:
            output.write(value)
    result = {'schema': 'cmre-provider-packet/1', 'max_line_length': max_line_length,
              'files': files}
    with (target / 'manifest.json').open('x', encoding='utf-8') as output:
        json.dump(result, output, indent=2)
        output.write('\n')
    return result


def relative_file(root, name):
    if not isinstance(name, str) or not name:
        raise ValueError(f'Invalid workspace-relative path: {name}')
    path = root / name
    if (Path(name).is_absolute()
            or '..' in Path(name).parts or path.is_symlink()
            or not path.resolve().is_relative_to(root)):
        raise ValueError(f'Invalid workspace-relative path: {name}')
    return path


#: Variables an enclosing OpenCode session exports for its own process tree.
#: A provider child that inherits them resolves the parent's session config
#: (``OPENCODE_CONFIG`` points at a full config file) instead of the scoped
#: packet permissions, and the client can refuse to start. They are dropped
#: before the spec environment is applied, so a profile may still set any of
#: them explicitly.
LAUNCHER_ENVIRONMENT = ('OPENCODE', 'OPENCODE_CONFIG', 'OPENCODE_CONFIG_CONTENT',
                        'OPENCODE_SESSION_ID', 'OPENCODE_TERMINAL')


def child_environment(spec, base=None):
    """Environment for one provider process.

    The readiness identity and the launched child must agree on the effective
    environment, so both call this function. Inherited launcher variables are
    removed first and the spec wins last.
    """
    environment = dict(os.environ if base is None else base)
    for name in LAUNCHER_ENVIRONMENT:
        environment.pop(name, None)
    environment.update(spec.get('environment', {}))
    return environment


def readiness_identity(spec, root):
    """Validate before dispatch; bind smoke to actual input and permission bytes."""
    if spec.get('phase') not in ('smoke', 'work'):
        if spec.get('command') is not None and 'phase' not in spec:
            return None  # legacy local fake-process supervision only
        raise ValueError('Real provider runs require phase smoke or work')
    if spec.get('command') is None:
        if not all(isinstance(spec.get(k), str) and spec[k].strip()
                   for k in ('model', 'client_version', 'executable', 'prompt_file')):
            raise ValueError('Pin model, client_version, executable and prompt_file')
    files = spec.get('required_text_files')
    permissions = spec.get('permission_files')
    if not isinstance(files, dict) or not files or not isinstance(permissions, list) or not permissions:
        raise ValueError('Declare required_text_files and permission_files for scoped smoke')
    hashes = {}
    for name, kind in files.items():
        path = relative_file(root, name)
        value = text(path)
        if kind == 'json':
            json.loads(value)
        elif kind != 'text':
            raise ValueError('Required text kind must be text or json')
        readable_lines(value, path)
        hashes[name] = digest(path)
    permission_hashes = {name: digest(relative_file(root, name)) for name in permissions}
    if set(permissions) & set(spec['allowed_files']):
        raise ValueError('Permission files must be read-only')
    if spec.get('prompt_file'):
        text(spec['prompt_file'])
    smoke = spec.get('smoke_file')
    relative_file(root, smoke)
    if smoke not in spec['allowed_files'] or smoke in files or smoke in permissions:
        raise ValueError('Use a separate allowed harmless smoke_file')
    executable = spec.get('executable')
    environment = child_environment(spec)
    return {'schema': 'cmre-provider-readiness/1', 'directory': str(root),
            'required_text_files': hashes, 'permission_files': permission_hashes,
            'allowed_files': sorted(spec['allowed_files']), 'smoke_file': smoke,
            'model': spec.get('model'), 'client_version': spec.get('client_version'),
            'executable': str(Path(executable).resolve()) if executable else None,
            'executable_sha256': digest(executable) if executable else None,
            'environment': spec.get('environment', {}),
            'effective_environment_sha256': hashlib.sha256(
                json.dumps(environment, sort_keys=True).encode('utf-8')).hexdigest(),
            'fake': spec.get('command') is not None}


def smoke_readiness(spec, root, records, changed):
    """Recognize OpenCode 1.18.3 read/write evidence, never assistant success text."""
    identity = readiness_identity(spec, root)
    covered = {name: set() for name in identity['required_text_files']}
    written = False
    failures = []
    for event in records:
        part = event.get('part', {})
        if event.get('type') != 'tool_use' or part.get('tool') not in ('read', 'write'):
            continue
        state = part.get('state', {})
        name = state.get('input', {}).get('filePath')
        if not isinstance(name, str):
            continue
        path = (root / name).resolve()
        if not path.is_relative_to(root):
            continue
        name = path.relative_to(root).as_posix()
        # Resolve declarations too, allowing native separators without aliasing paths.
        required = next((n for n in covered if relative_file(root, n).resolve() == path), None)
        metadata = state.get('metadata', {})
        if part['tool'] == 'read' and required is not None:
            display = metadata.get('display', {})
            lines = text(path).splitlines()
            start, end = display.get('lineStart'), display.get('lineEnd')
            valid = (state.get('status') == 'completed' and not state.get('error')
                     and display.get('type') == 'file'
                     and display.get('totalLines') == len(lines)
                     and type(start) is int and type(end) is int
                     and 1 <= start <= end <= len(lines)
                     and isinstance(display.get('text'), str)
                     and display['text'].splitlines() == lines[start - 1:end])
            if valid:
                covered[required].update(range(start, end + 1))
            else:
                failures.append(f'Unproven read: {required}')
        elif part['tool'] == 'write' and path == relative_file(root, spec['smoke_file']).resolve():
            content = state.get('input', {}).get('content')
            written = (state.get('status') == 'completed' and not state.get('error')
                       and isinstance(content, str) and path.is_file()
                       and text(path).splitlines() == content.splitlines())
    missing = [name for name, rows in covered.items()
               if rows != set(range(1, len(text(relative_file(root, name)).splitlines()) + 1)) or not rows]
    if missing or not written or changed != [spec['smoke_file']]:
        failures.append('Missing full reads, scoped harmless write, or unexpected changes')
    return {'identity': identity, 'passed': not failures, 'missing_reads': missing,
            'write_proven': written, 'errors': failures}


def assistant_messages(path, session):
    data = load(path)
    if not isinstance(data, dict) or data.get('info', {}).get('id') != session:
        raise ValueError('Export session identity mismatch')
    if not isinstance(data.get('messages'), list):
        raise ValueError('Export messages must be a list')
    result = []
    for message in data['messages']:
        info = message['info']
        if info.get('role') == 'assistant':
            if info.get('sessionID') != session:
                raise ValueError('Assistant session identity mismatch')
            result.append(info)
    if not result:
        raise ValueError('Export contains no assistant messages')
    return result


def exported_calls(messages):
    calls = {}
    for message in messages:
        accounted = bool(message.get('time', {}).get('completed')
                         and message.get('finish') in ('stop', 'length', 'tool-calls', 'content-filter')
                         and not message.get('error'))
        tokens = (message.get('tokens') or {}) if accounted else {}
        cache = tokens.get('cache') or {}
        call = {'call_id': message['id'], 'model': message['providerID'] + '/' + message['modelID'],
                'input_tokens': tokens.get('input'), 'output_tokens': tokens.get('output'),
                'reasoning_tokens': tokens.get('reasoning'), 'cache_read_tokens': cache.get('read'),
                'cache_write_tokens': cache.get('write'),
                'cost': message.get('cost') if accounted else None,
                'finish': message.get('finish'), 'usage_observed': accounted}
        for key, value in call.items():
            if key.endswith('_tokens') and value is not None and (type(value) is not int or value < 0):
                raise ValueError(f'Invalid token count: {key}')
        key = call['call_id']
        if not isinstance(key, str) or not key.strip() or not call['model'].strip('/'):
            raise ValueError('Missing call/model identity')
        if key in calls and calls[key] != call:
            raise ValueError(f'Conflicting duplicate call: {key}')
        calls[key] = call
    return list(calls.values())


def call_projection(rows):
    """Render observed physical call words exactly; never infer or narrow bits."""
    def word(value):
        if type(value) is not int or not 0 <= value <= 0xffffffff:
            raise ValueError('Physical call values must be unsigned32 integers')
        return f'0x{value:08x}'

    if not isinstance(rows, list):
        raise ValueError('Physical calls must be a list')
    result = []
    for row in rows:
        if (not isinstance(row, dict)
                or set(row) != {'function_va', 'ecx_argument', 'stack_args'}
                or not isinstance(row['stack_args'], list)):
            raise ValueError('Physical calls require function_va, ecx_argument and stack_args')
        result.append({'function_va': word(row['function_va']),
                       'ecx_argument': None if row['ecx_argument'] is None else word(row['ecx_argument']),
                       'stack_args': [word(value) for value in row['stack_args']]})
    return result


def control_projection(bundle):
    """The selected-control outcome and bundle acceptance readiness are distinct."""
    if bundle.get('schema') != 'cmre-control-bundle/2':
        raise ValueError('Unsupported control bundle schema')
    status = bundle.get('status')
    if status not in ('controls-complete', 'incomplete') or bundle.get('readiness') != 'proposal':
        raise ValueError('Unknown control status/readiness')
    return {'controls_complete': status == 'controls-complete', 'readiness': 'proposal',
            'acceptance_ready': False}


def mismatch_projection(rows):
    witnesses, omissions = [], []
    for row in rows:
        if not isinstance(row, dict) or type(row.get('case')) is not int or row['case'] < 0:
            raise ValueError('Mismatch case must be a nonnegative integer')
        if 'note' in row:
            if row['note'] != 'further mismatches omitted' or set(row) != {'case', 'note'}:
                raise ValueError('Unknown mismatch omission marker')
            omissions.append(row)
        else:
            witnesses.append(row)
    return {'witnesses': witnesses, 'omissions': omissions, 'truncated': bool(omissions)}


def apply_accounting(data, calls, adoptions, record_usage):
    """Prepare one atomic pilot update, including adoption corrections, before finish."""
    if data.get('ended') is not None:
        raise ValueError('Finished pilot is immutable')
    if not isinstance(adoptions, dict) or adoptions.get('schema') != 'cmre-provider-adoptions/1':
        raise ValueError('Use cmre-provider-adoptions/1')
    corrections = {}
    for row in adoptions['calls']:
        key, count = row['call_id'], row['review_corrections']
        if key in corrections or type(count) is not int or count < 0:
            raise ValueError('Use unique call IDs and nonnegative correction counts')
        corrections[key] = count
    by_id = {}
    for call in calls:
        key = call['call_id']
        if key in by_id and by_id[key] != call:
            raise ValueError(f'Conflicting duplicate call: {key}')
        by_id[key] = call
    if set(corrections) != set(by_id):
        raise ValueError('Adoption receipt must explicitly count corrections (including zero) for every call')
    updated = copy.deepcopy(data)
    for key, call in by_id.items():
        values = {k: call[k] for k in ('input_tokens', 'cache_read_tokens', 'cache_write_tokens', 'output_tokens')}
        desired = {'model': call['model'], **values, 'review_corrections': corrections[key],
                   'reasoning_tokens': call['reasoning_tokens']}
        if key in updated['calls']:
            if updated['calls'][key] != desired:
                raise ValueError(f'Already recorded call differs: {key}')
            continue
        record_usage(updated, key, call['model'], values, corrections[key])
        updated['calls'][key]['reasoning_tokens'] = call['reasoning_tokens']
    return updated
