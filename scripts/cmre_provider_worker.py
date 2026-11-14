"""Bound one external coding process; cmre still owns tasks, tests and acceptance.

Run a JSON spec with ``run SPEC RUN_DIRECTORY``. Separate invocations can inspect
``status RUN_DIRECTORY``, request ``cancel RUN_DIRECTORY``, or ``collect`` a
terminal receipt. Logs and receipts belong in ignored local analysis storage.
The scope audit is a cooperative control, not a filesystem sandbox.
"""
from __future__ import annotations

import argparse
import ctypes
import hashlib
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import time

# Direct script execution puts scripts/, rather than the repository, on sys.path.
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from scripts import cmre_provider_artifacts as artifacts


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def write_json(path, value):
    path = Path(path)
    temporary = path.with_suffix('.tmp')
    temporary.write_text(json.dumps(value, indent=2), encoding='utf-8')
    deadline = time.monotonic() + 2
    while True:
        try:
            temporary.replace(path)
            return
        except PermissionError as error:
            # Windows readers can deny delete-sharing while inspect() briefly
            # has the old receipt open. Keep the complete old/new files intact.
            if (os.name != 'nt' or getattr(error, 'winerror', None) not in (5, 32, 33)
                    or time.monotonic() >= deadline):
                raise
            time.sleep(.01)


def read_json(path):
    """Read a file write_json() may be replacing: Windows denies the open while the old
    receipt is delete-pending, so retry those sharing errors for up to 2 s."""
    path = Path(path)
    deadline = time.monotonic() + 2
    while True:
        try:
            return json.loads(path.read_text(encoding='utf-8'))
        except PermissionError as error:
            if (os.name != 'nt' or getattr(error, 'winerror', None) not in (5, 32, 33)
                    or time.monotonic() >= deadline):
                raise
            time.sleep(.01)


def snapshot(root):
    names = subprocess.check_output(
        ['git', '-C', str(root), 'ls-files', '-z', '--cached', '--others', '--exclude-standard'])
    result = {}
    for name in set(names.decode('utf-8').split('\0')) - {''}:
        path = root / name
        if path.is_symlink() or (path.exists() and not path.resolve().is_relative_to(root)):
            raise ValueError(f'Scope contains redirected path: {name}')
        result[name] = digest(path) if path.is_file() else None
    return result


def events(path):
    records, errors = [], []
    for number, line in enumerate(Path(path).read_text(encoding='utf-8', errors='replace').splitlines(), 1):
        try:
            value = json.loads(line)
            if not isinstance(value, dict) or not isinstance(value.get('type'), str):
                raise ValueError('missing event type')
            part = value.get('part', {})
            if not isinstance(part, dict):
                raise ValueError('event part must be an object')
            tokens = part.get('tokens', {})
            if not isinstance(tokens, dict) or not isinstance(tokens.get('cache', {}), dict):
                raise ValueError('token usage must be an object')
            usage = [tokens.get(k) for k in ('input', 'output')]
            usage += [tokens.get('cache', {}).get(k) for k in ('read', 'write')]
            if any(n is not None and (type(n) is not int or n < 0) for n in usage):
                raise ValueError('token counts must be nonnegative integers or absent')
            records.append(value)
        except (ValueError, TypeError) as error:
            errors.append(f'line {number}: {error}')
    return records, errors


def summarize(records):
    sessions = sorted({e['sessionID'] for e in records if isinstance(e.get('sessionID'), str)})
    finishes = [e for e in records if e['type'] == 'step_finish']
    calls, seen = [], set()
    for event in finishes:
        part = event.get('part', {})
        key = part.get('messageID')
        if not key or key in seen:
            continue
        seen.add(key)
        tokens = part.get('tokens') or {}
        cache = tokens.get('cache') or {}
        calls.append({'call_id': key, 'input_tokens': tokens.get('input'),
                      'output_tokens': tokens.get('output'), 'cache_read_tokens': cache.get('read'),
                      'cache_write_tokens': cache.get('write'), 'cost': part.get('cost')})
    return {'session_ids': sessions, 'calls': calls,
            'steps': sum(e['type'] == 'step_start' for e in records),
            'finished': bool(finishes and finishes[-1].get('part', {}).get('reason') == 'stop'),
            'provider_errors': [e for e in records if e['type'] == 'error']}


class ProcessTree:
    """Windows job closes descendants even if the controller itself is killed."""

    def __init__(self, process):
        self.process, self.job = process, None
        if os.name != 'nt':
            return
        from ctypes import wintypes

        class Basic(ctypes.Structure):
            _fields_ = [('process_time', ctypes.c_int64), ('job_time', ctypes.c_int64),
                        ('flags', wintypes.DWORD), ('minimum', ctypes.c_size_t),
                        ('maximum', ctypes.c_size_t), ('active', wintypes.DWORD),
                        ('affinity', ctypes.c_size_t), ('priority', wintypes.DWORD),
                        ('scheduling', wintypes.DWORD)]

        class IO(ctypes.Structure):
            _fields_ = [(name, ctypes.c_uint64) for name in
                        ('read_ops', 'write_ops', 'other_ops', 'read_bytes', 'write_bytes', 'other_bytes')]

        class Limits(ctypes.Structure):
            _fields_ = [('basic', Basic), ('io', IO), ('process_memory', ctypes.c_size_t),
                        ('job_memory', ctypes.c_size_t), ('peak_process', ctypes.c_size_t),
                        ('peak_job', ctypes.c_size_t)]

        kernel = ctypes.WinDLL('kernel32', use_last_error=True)
        kernel.CreateJobObjectW.argtypes = [ctypes.c_void_p, wintypes.LPCWSTR]
        kernel.CreateJobObjectW.restype = wintypes.HANDLE
        kernel.SetInformationJobObject.argtypes = [wintypes.HANDLE, ctypes.c_int, ctypes.c_void_p, wintypes.DWORD]
        kernel.AssignProcessToJobObject.argtypes = [wintypes.HANDLE, wintypes.HANDLE]
        kernel.TerminateJobObject.argtypes = [wintypes.HANDLE, wintypes.UINT]
        kernel.CloseHandle.argtypes = [wintypes.HANDLE]
        self.kernel = kernel
        job = kernel.CreateJobObjectW(None, None)
        limits = Limits()
        limits.basic.flags = 0x2000  # JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
        if not job:
            raise ctypes.WinError(ctypes.get_last_error())
        if not kernel.SetInformationJobObject(job, 9, ctypes.byref(limits), ctypes.sizeof(limits)):
            kernel.CloseHandle(job)
            raise ctypes.WinError(ctypes.get_last_error())
        if not kernel.AssignProcessToJobObject(job, int(process._handle)):
            kernel.CloseHandle(job)
            raise ctypes.WinError(ctypes.get_last_error())
        self.job = job

    def resume(self):
        if not self.job:
            return
        # The process was created suspended: no worker instruction or descendant
        # can run until job assignment has succeeded. Find its primary thread.
        from ctypes import wintypes

        class ThreadEntry(ctypes.Structure):
            _fields_ = [('size', wintypes.DWORD), ('usage', wintypes.DWORD),
                        ('tid', wintypes.DWORD), ('pid', wintypes.DWORD),
                        ('base_priority', wintypes.LONG), ('delta_priority', wintypes.LONG),
                        ('flags', wintypes.DWORD)]

        kernel = self.kernel
        kernel.CreateToolhelp32Snapshot.argtypes = [wintypes.DWORD, wintypes.DWORD]
        kernel.CreateToolhelp32Snapshot.restype = wintypes.HANDLE
        kernel.Thread32First.argtypes = [wintypes.HANDLE, ctypes.POINTER(ThreadEntry)]
        kernel.Thread32Next.argtypes = [wintypes.HANDLE, ctypes.POINTER(ThreadEntry)]
        kernel.OpenThread.argtypes = [wintypes.DWORD, wintypes.BOOL, wintypes.DWORD]
        kernel.OpenThread.restype = wintypes.HANDLE
        kernel.ResumeThread.argtypes = [wintypes.HANDLE]
        kernel.ResumeThread.restype = wintypes.DWORD
        handle = kernel.CreateToolhelp32Snapshot(4, 0)  # TH32CS_SNAPTHREAD
        if handle == ctypes.c_void_p(-1).value:
            raise ctypes.WinError(ctypes.get_last_error())
        try:
            entry = ThreadEntry()
            entry.size = ctypes.sizeof(entry)
            found = kernel.Thread32First(handle, ctypes.byref(entry))
            while found:
                if entry.pid == self.process.pid:
                    thread = kernel.OpenThread(2, False, entry.tid)  # THREAD_SUSPEND_RESUME
                    if not thread:
                        raise ctypes.WinError(ctypes.get_last_error())
                    try:
                        if kernel.ResumeThread(thread) == 0xffffffff:
                            raise ctypes.WinError(ctypes.get_last_error())
                        return
                    finally:
                        kernel.CloseHandle(thread)
                found = kernel.Thread32Next(handle, ctypes.byref(entry))
            raise RuntimeError('Suspended worker primary thread is missing')
        finally:
            kernel.CloseHandle(handle)

    def stop(self):
        if self.job:
            self.kernel.TerminateJobObject(self.job, 1)
        elif os.name != 'nt':
            try:
                os.killpg(self.process.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
        elif self.process.poll() is None:
            self.process.kill()
        self.process.wait(timeout=15)

    def close(self):
        if self.job:
            self.kernel.CloseHandle(self.job)
            self.job = None
        elif os.name != 'nt':
            self.stop()


def identity(pid):
    """Return live creation identity or confirmed exit; unavailable state raises."""
    if type(pid) is not int or pid <= 0:
        raise ValueError('Process identity requires a positive integer PID')
    if os.name == 'nt':
        from ctypes import wintypes
        kernel = ctypes.WinDLL('kernel32', use_last_error=True)
        kernel.OpenProcess.argtypes = [wintypes.DWORD, wintypes.BOOL, wintypes.DWORD]
        kernel.OpenProcess.restype = wintypes.HANDLE
        kernel.GetProcessTimes.argtypes = [wintypes.HANDLE] + [ctypes.c_void_p] * 4
        kernel.GetExitCodeProcess.argtypes = [wintypes.HANDLE, ctypes.c_void_p]
        kernel.CloseHandle.argtypes = [wintypes.HANDLE]
        handle = kernel.OpenProcess(0x1000, False, pid)
        if not handle:
            error = ctypes.get_last_error()
            if error == 87:  # ERROR_INVALID_PARAMETER: positive PID no longer exists
                return None
            raise ctypes.WinError(error)
        try:
            code = wintypes.DWORD()
            times = [ctypes.c_uint64() for _ in range(4)]
            if not kernel.GetExitCodeProcess(handle, ctypes.byref(code)):
                raise ctypes.WinError(ctypes.get_last_error())
            if code.value != 259:
                return None
            if not kernel.GetProcessTimes(handle, *(ctypes.byref(t) for t in times)):
                raise ctypes.WinError(ctypes.get_last_error())
            return str(times[0].value)
        finally:
            kernel.CloseHandle(handle)
    path = Path(f'/proc/{pid}/stat')
    try:
        fields = path.read_text().rsplit(')', 1)[1].split()
        return None if fields[0] == 'Z' else fields[19]
    except FileNotFoundError:
        return None


def collect_export(executable, directory, environment, session, run_dir, timeout=30):
    output = run_dir / 'session-export.json'
    with output.open('wb') as out, (run_dir / 'export.stderr.log').open('wb') as err:
        process = subprocess.Popen([executable, '--pure', 'export', session], cwd=directory, env=environment,
                                   stdout=out, stderr=err, stdin=subprocess.DEVNULL,
                                   start_new_session=os.name != 'nt',
                                   creationflags=(subprocess.CREATE_NO_WINDOW | 4) if os.name == 'nt' else 0)
        tree = None
        try:
            tree = ProcessTree(process)
            tree.resume()
            result = process.wait(timeout=timeout)
            if result:
                raise subprocess.CalledProcessError(result, process.args)
        finally:
            if tree:
                tree.stop()
                tree.close()
            elif process.poll() is None:
                process.kill()
                process.wait()
    return artifacts.assistant_messages(output, session)


def exported_calls(messages):
    return artifacts.exported_calls(messages)


def scoped_snapshot(root, allowed):
    result = snapshot(root)
    # Explicit scope includes ignored smoke/proposal paths too.
    for name in allowed:
        path = artifacts.relative_file(root, name)
        result[name] = digest(path) if path.is_file() else None
    return result


def validate_smoke(spec, root, readiness):
    if not spec.get('smoke_run'):
        raise ValueError('Work requires a successful scoped smoke_run')
    directory = Path(spec['smoke_run']).resolve(strict=True)
    receipt = inspect(directory, 'collect')
    old_spec = artifacts.load(directory / 'spec.json')
    if (receipt['state'] != 'completed' or old_spec.get('phase') != 'smoke'
            or receipt.get('readiness', {}).get('identity') != readiness):
        raise ValueError('Successful smoke identity does not match work inputs/permissions')
    records, errors = events(directory / 'stdout.jsonl')
    proof = artifacts.smoke_readiness(old_spec, root, records, receipt['changed_files'])
    if errors or not proof['passed'] or proof != receipt['readiness']:
        raise ValueError('Retained smoke read/write evidence is incomplete or changed')
    return {'receipt': str(directory / 'receipt.json'), 'sha256': digest(directory / 'receipt.json')}


def run(spec_path, run_dir, *, cancel_request=None):
    spec = artifacts.load(spec_path)
    root = Path(spec['directory']).resolve(strict=True)
    allowed = spec['allowed_files']
    if len(allowed) != len(set(allowed)) or not allowed:
        raise ValueError('Use distinct exact allowed paths')
    for name in allowed:
        p = root / name
        if Path(name).is_absolute() or not p.resolve().is_relative_to(root) or p.is_symlink():
            raise ValueError(f'Invalid allowed path: {name}')
    readiness = artifacts.readiness_identity(spec, root)
    smoke = validate_smoke(spec, root, readiness) if spec.get('phase') == 'work' else None
    run_dir = Path(run_dir).resolve()
    run_dir.mkdir(parents=True, exist_ok=False)  # never overwrite/relaunch an existing run
    write_json(run_dir / 'spec.json', spec)
    before = scoped_snapshot(root, allowed)
    write_json(run_dir / 'before.json', before)
    pinned = {str(Path(p).resolve()): digest(p) for p in spec.get('pinned_files', [])}
    if spec.get('prompt_file'):
        pinned[str(Path(spec['prompt_file']).resolve())] = digest(spec['prompt_file'])
    if readiness:
        for name in (*readiness['required_text_files'], *readiness['permission_files']):
            if name not in allowed:
                pinned[str((root / name).resolve())] = digest(root / name)
    write_json(run_dir / 'pins.json', pinned)
    environment = artifacts.child_environment(spec)  # paths/config only, never credential values
    command = spec.get('command')
    if command is None:
        command = [spec['executable'], '--pure', 'run', '--dir', str(root),
                   '--model', spec['model'], '--format', 'json', '--title', spec['owner'],
                   Path(spec['prompt_file']).read_text(encoding='utf-8-sig')]
    record = {'schema': 'cmre-provider-worker/1', 'owner': spec['owner'],
              'functions': spec.get('functions', []), 'model_requested': spec.get('model'),
              'parent_receipt': spec.get('parent_receipt'), 'client_version': spec.get('client_version'),
              'started': time.time(), 'state': 'starting', 'controller_pid': os.getpid(),
              'controller_identity': identity(os.getpid()), 'source_before': {p: before.get(p) for p in allowed}}
    if readiness:
        record['readiness_identity'] = readiness
        record['smoke_receipt'] = smoke
    receipt = run_dir / 'receipt.json'
    write_json(receipt, record)
    process, tree, reason = None, None, None
    try:
        with (run_dir / 'stdout.jsonl').open('wb') as out, (run_dir / 'stderr.log').open('wb') as err:
            process = subprocess.Popen(command, cwd=root, env=environment, stdout=out, stderr=err,
                                       stdin=subprocess.DEVNULL, start_new_session=os.name != 'nt',
                                       creationflags=(subprocess.CREATE_NO_WINDOW | 4) if os.name == 'nt' else 0)
            tree = ProcessTree(process)
            record.update(state='running', pid=process.pid, process_identity=identity(process.pid))
            write_json(receipt, record)
            tree.resume()
            while process.poll() is None:
                if ((run_dir / 'cancel.request').exists()
                        or (cancel_request is not None and Path(cancel_request).exists())):
                    reason = 'cancelled'
                elif time.time() - record['started'] >= spec.get('timeout_seconds', 900):
                    reason = 'timeout'
                else:
                    current, _ = events(run_dir / 'stdout.jsonl')
                    if summarize(current)['steps'] > spec.get('max_steps', 30):
                        reason = 'step_limit'
                if reason:
                    tree.stop()
                    break
                time.sleep(0.2)
            record['exit_code'] = process.wait()
    except BaseException as error:
        reason = 'controller_error'
        record['error'] = f'{type(error).__name__}: {error}'
        if tree:
            tree.stop()
        elif process and process.poll() is None:
            process.kill()
            process.wait()
    finally:
        if tree:
            tree.close()
    try:
        return finalize(spec, root, run_dir, environment, before, pinned, record, reason)
    except Exception as error:
        record.update(state='finalization_error', ended=time.time(),
                      error=f'{type(error).__name__}: {error}')
        record['elapsed_seconds'] = round(record['ended'] - record['started'], 3)
        write_json(receipt, record)
        return record


def finalize(spec, root, run_dir, environment, before, pinned, record, reason):
    records, errors = events(run_dir / 'stdout.jsonl') if (run_dir / 'stdout.jsonl').exists() else ([], ['no log'])
    summary = summarize(records)
    after = scoped_snapshot(root, spec['allowed_files'])
    changed = sorted(p for p in set(before) | set(after) if before.get(p) != after.get(p))
    drift = [p for p, value in pinned.items() if not Path(p).exists() or digest(p) != value]
    record.update(summary, ended=time.time(), changed_files=changed,
                  out_of_scope=[p for p in changed if p not in spec['allowed_files']], pin_drift=drift,
                  source_after={p: after.get(p) for p in spec['allowed_files']}, parse_errors=errors)
    if spec.get('command') is None and len(summary['session_ids']) != 1:
        reason = reason or 'session_identity_error'
    elif spec.get('command') is None:
        try:
            messages = collect_export(spec['executable'], root, environment, summary['session_ids'][0], run_dir)
            record['models_observed'] = sorted({m['providerID'] + '/' + m['modelID'] for m in messages})
            record['calls'] = exported_calls(messages)
            if record['models_observed'] != [spec['model']]:
                reason = reason or 'model_mismatch'
        except Exception as error:
            record['export_error'] = str(error)
            reason = reason or 'export_failed'
    if record['out_of_scope'] or drift:
        reason = 'scope_violation' if record['out_of_scope'] else 'input_drift'
    if errors or summary['provider_errors'] or not summary['finished'] or record.get('exit_code') != 0:
        reason = reason or 'incomplete'
    if summary['steps'] > spec.get('max_steps', 30):
        reason = reason or 'step_limit'
    if spec.get('phase') == 'smoke':
        proof = artifacts.smoke_readiness(spec, root, records, changed)
        record['readiness'] = proof
        if not proof['passed']:
            reason = reason or 'readiness_failed'
    record['state'] = reason or 'completed'
    record['elapsed_seconds'] = round(record['ended'] - record['started'], 3)
    write_json(run_dir / 'after.json', after)
    write_json(run_dir / 'receipt.json', record)
    return record


def pair(smoke_spec_path, work_spec_path, run_dir):
    """Run exactly two scoped stages in one environment; never retry or accept."""
    smoke_spec, work_spec = artifacts.load(smoke_spec_path), artifacts.load(work_spec_path)
    smoke_root = Path(smoke_spec['directory']).resolve(strict=True)
    work_root = Path(work_spec['directory']).resolve(strict=True)
    if (smoke_spec.get('phase') != 'smoke' or work_spec.get('phase') != 'work'
            or smoke_root != work_root
            or artifacts.readiness_identity(smoke_spec, smoke_root)
            != artifacts.readiness_identity(work_spec, work_root)):
        raise ValueError('Paired smoke/work identity must match before dispatch')
    pins = {str(Path(path).resolve()): digest(path)
            for path in (smoke_spec_path, work_spec_path,
                         *smoke_spec.get('pinned_files', []), *work_spec.get('pinned_files', []))}
    for spec in (smoke_spec, work_spec):
        if spec.get('prompt_file'):
            pins[str(Path(spec['prompt_file']).resolve())] = digest(spec['prompt_file'])
    run_dir = Path(run_dir).resolve()
    run_dir.mkdir(parents=True, exist_ok=False)
    record = {'schema': 'cmre-provider-pair/1', 'state': 'starting', 'started': time.time(),
              'controller_pid': os.getpid(), 'controller_identity': identity(os.getpid()),
              'pins': pins, 'stages': []}
    receipt = run_dir / 'receipt.json'
    write_json(receipt, record)
    try:
        for stage, spec_path in (('smoke', smoke_spec_path), ('work', work_spec_path)):
            if (run_dir / 'cancel.request').exists():
                record['state'] = 'cancelled'
                break
            if any(not Path(path).is_file() or digest(path) != sha for path, sha in pins.items()):
                record['state'] = 'input_drift'
                break
            if stage == 'work':
                effective = dict(work_spec, smoke_run=str(run_dir / 'smoke'))
                spec_path = run_dir / 'work-spec.json'
                write_json(spec_path, effective)
            record.update(state='running', current_stage=stage,
                          current_run_directory=str(run_dir / stage))
            write_json(receipt, record)
            result = run(spec_path, run_dir / stage, cancel_request=run_dir / 'cancel.request')
            record['stages'].append({'stage': stage, 'state': result['state'],
                                     'receipt': str(run_dir / stage / 'receipt.json')})
            record['state'] = result['state']
            if result['state'] != 'completed':
                break
    except BaseException as error:
        record.update(state='controller_error', error=f'{type(error).__name__}: {error}')
        raise
    finally:
        record.update(ended=time.time())
        record['elapsed_seconds'] = round(record['ended'] - record['started'], 3)
        write_json(receipt, record)
    return record


def inspect(run_dir, action):
    run_dir = Path(run_dir).resolve()
    record = read_json(run_dir / 'receipt.json')
    active = record['state'] in ('starting', 'running')
    if active:
        current = identity(record['controller_pid'])
        if not current or current != record['controller_identity']:
            record['state'] = 'orphaned-needs-reconciliation'
        elif action == 'cancel':
            (run_dir / 'cancel.request').write_text('cancel', encoding='utf-8')
            record['cancel_requested'] = True
    if action == 'collect' and (active or record['state'].startswith('orphaned')):
        raise ValueError('Run is not reconciled and terminal; never relaunch or reassign blindly')
    if action == 'collect' and record.get('pid'):
        current = identity(record['pid'])
        if current and (record.get('process_identity') is None or current == record['process_identity']):
            raise ValueError('Terminal receipt still has a live worker; reconcile before collection')
    return record


def collect(run_dir, recover_export=False, timeout=120):
    """Append a collection version; only export can run, never inference or acceptance."""
    from cmre import coordination
    if type(timeout) is not int or not 1 <= timeout <= 120:
        raise ValueError('Export recovery timeout must be 1..120 seconds')
    run_dir = Path(run_dir).resolve(strict=True)
    if artifacts.load(run_dir / 'receipt.json').get('schema') == 'cmre-provider-pair/1':
        raise ValueError('Collect each retained smoke/work stage separately')
    with coordination.lock(run_dir / 'collect.lock'):
        record = inspect(run_dir, 'collect')
        spec = artifacts.load(run_dir / 'spec.json')
        versions = run_dir / 'collections'
        versions.mkdir(exist_ok=True)
        number = max((int(p.name) for p in versions.iterdir() if p.is_dir() and p.name.isdigit()), default=0) + 1
        output = versions / f'{number:04d}'
        output.mkdir()
        result = {'schema': 'cmre-provider-collection/1', 'run_state': record['state'],
                  'run_directory': str(run_dir), 'started': time.time(), 'usage_status': 'pending',
                  'calls': None, 'recovered_export': False,
                  'artifacts': {str(run_dir / name): digest(run_dir / name)
                                for name in ('receipt.json', 'spec.json')}}
        write_json(output / 'receipt.json', result)
        try:
            sessions = record.get('session_ids', [])
            if len(sessions) != 1 or spec.get('command') is not None:
                raise ValueError('Collection needs one real provider session')
            session = sessions[0]
            source = run_dir / 'session-export.json'
            try:
                messages = artifacts.assistant_messages(source, session)
            except (OSError, ValueError, KeyError, TypeError, AttributeError):
                if not recover_export:
                    raise
                pid = record.get('controller_pid')
                current = identity(pid) if pid else None
                if current and (record.get('controller_identity') is None or current == record['controller_identity']):
                    raise ValueError('Controller is still live; reconcile before recovery')
                pinned_executable = record.get('readiness_identity', {}).get('executable_sha256')
                if not pinned_executable or digest(spec['executable']) != pinned_executable:
                    raise ValueError('Export executable identity is missing or changed')
                environment = artifacts.child_environment(spec)
                messages = collect_export(spec['executable'], Path(spec['directory']), environment,
                                          session, output, timeout=timeout)
                source = output / 'session-export.json'
                result['recovered_export'] = True
            result['models_observed'] = sorted({m['providerID'] + '/' + m['modelID'] for m in messages})
            if result['models_observed'] != [spec['model']] or spec['model'] != record.get('model_requested'):
                raise ValueError('Export requested/observed model identity mismatch')
            result['calls'] = exported_calls(messages)
            result['artifacts'][str(source)] = digest(source)
            result['export_path'] = str(source)
            result['usage_status'] = 'collected'
        except Exception as error:
            result.update(usage_status='unknown', calls=None, error=f'{type(error).__name__}: {error}')
        result['ended'] = time.time()
        if (run_dir / 'session-export.json').exists():
            result['artifacts'][str(run_dir / 'session-export.json')] = digest(run_dir / 'session-export.json')
        # Preserve even failed, partial recovery artifacts in this immutable version.
        for name in ('session-export.json', 'export.stderr.log'):
            if (output / name).exists():
                result['artifacts'][str(output / name)] = digest(output / name)
        write_json(output / 'receipt.json', result)
        return result


def account(manifest_path):
    """Import checked collection projections and explicit adoption counts once."""
    from cmre import coordination, env, pilot
    manifest = artifacts.load(manifest_path)
    if manifest.get('schema') != 'cmre-provider-accounting/1':
        raise ValueError('Use cmre-provider-accounting/1')
    calls, pins = [], {str(Path(manifest_path).resolve()): digest(manifest_path)}
    for name in manifest['collections']:
        collection = artifacts.load(name)
        if collection.get('schema') != 'cmre-provider-collection/1' or collection.get('usage_status') != 'collected':
            raise ValueError('Account only collected usage; retain unknown runs separately')
        for path, sha in collection['artifacts'].items():
            if digest(path) != sha:
                raise ValueError(f'Collection artifact changed: {path}')
        # Reproject from the pinned raw export, rather than trusting edited totals.
        run_dir = Path(collection['run_directory'])
        record = artifacts.load(run_dir / 'receipt.json')
        export = collection['export_path']
        if export not in collection['artifacts'] or Path(export).name != 'session-export.json':
            raise ValueError('Collection must identify its pinned raw export')
        projected = exported_calls(artifacts.assistant_messages(export, record['session_ids'][0]))
        if projected != collection['calls'] or {c['model'] for c in projected} != {record['model_requested']}:
            raise ValueError('Collection projection/model identity changed')
        calls.extend(projected)
        pins[str(Path(name).resolve())] = digest(name)
    if not calls:
        raise ValueError('Select at least one observed provider call')
    adoption_path = manifest['adoptions']
    adoptions = artifacts.load(adoption_path)
    pins[str(Path(adoption_path).resolve())] = digest(adoption_path)
    with coordination.lock(env.ANALYSIS / 'cmre/pilots.lock'):
        path = pilot.path(manifest['pilot'])
        updated = artifacts.apply_accounting(artifacts.load(path), calls, adoptions, pilot.record_usage)
        updated.setdefault('provider_imports', {}).update(pins)
        coordination.atomic_json(path, updated)
    return {'pilot': manifest['pilot'], 'imported_call_ids': sorted({c['call_id'] for c in calls}),
            'review_corrections': sum(c['review_corrections'] for c in adoptions['calls']),
            'usage_complete': False, 'artifacts': pins}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest='action', required=True)
    p = sub.add_parser('run')
    p.add_argument('spec')
    p.add_argument('run_directory')
    p = sub.add_parser('pair')
    p.add_argument('smoke_spec')
    p.add_argument('work_spec')
    p.add_argument('run_directory')
    p = sub.add_parser('packet')
    p.add_argument('sources_manifest')
    p.add_argument('target_directory')
    p.add_argument('--max-line-length', type=int, default=artifacts.READ_LINE_LIMIT)
    for action in ('status', 'cancel'):
        sub.add_parser(action).add_argument('run_directory')
    p = sub.add_parser('collect')
    p.add_argument('run_directory')
    p.add_argument('--recover-export', action='store_true')
    p.add_argument('--timeout', type=int, default=120)
    sub.add_parser('account').add_argument('manifest')
    p = sub.add_parser('project')
    p.add_argument('kind', choices=('controls', 'mismatches', 'calls'))
    p.add_argument('artifact')
    args = parser.parse_args()
    if args.action == 'run':
        result = run(args.spec, args.run_directory)
    elif args.action == 'pair':
        result = pair(args.smoke_spec, args.work_spec, args.run_directory)
    elif args.action == 'packet':
        result = artifacts.prepare_packet(artifacts.load(args.sources_manifest),
                                          args.target_directory, max_line_length=args.max_line_length)
    elif args.action == 'collect':
        result = collect(args.run_directory, args.recover_export, args.timeout)
    elif args.action == 'account':
        result = account(args.manifest)
    elif args.action == 'project':
        value = (artifacts.strict_json(artifacts.text(args.artifact)) if args.kind == 'calls'
                 else artifacts.load(args.artifact))
        if args.kind == 'controls':
            result = artifacts.control_projection(value)
        elif args.kind == 'calls':
            result = artifacts.call_projection(value)
        else:
            result = artifacts.mismatch_projection(value['mismatches'])
    else:
        result = inspect(args.run_directory, args.action)
    print(json.dumps(result, indent=2))
    if args.action == 'collect':
        return 0 if result['usage_status'] == 'collected' else 1
    return 0 if args.action not in ('run', 'pair') or result['state'] == 'completed' else 1


if __name__ == '__main__':
    sys.exit(main())
