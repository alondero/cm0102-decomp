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
    """PID reuse must never authorize collection as a terminal run."""
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
            return None
        try:
            code = wintypes.DWORD()
            times = [ctypes.c_uint64() for _ in range(4)]
            if not kernel.GetExitCodeProcess(handle, ctypes.byref(code)) or code.value != 259:
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


def collect_export(executable, directory, environment, session, run_dir):
    output = run_dir / 'session-export.json'
    with output.open('wb') as out, (run_dir / 'export.stderr.log').open('wb') as err:
        subprocess.run([executable, '--pure', 'export', session], cwd=directory, env=environment,
                       stdout=out, stderr=err, timeout=30, check=True,
                       creationflags=subprocess.CREATE_NO_WINDOW if os.name == 'nt' else 0)
    data = json.loads(output.read_text(encoding='utf-8-sig'))
    return [m['info'] for m in data['messages'] if m.get('info', {}).get('role') == 'assistant']


def exported_calls(messages):
    calls = []
    for message in messages:
        # OpenCode initializes in-flight usage to zero. An interrupted message's
        # placeholders are not observed provider usage, even after a clean export.
        accounted = bool(message.get('time', {}).get('completed') and message.get('finish')
                         and not message.get('error'))
        tokens = (message.get('tokens') or {}) if accounted else {}
        cache = tokens.get('cache') or {}
        calls.append({'call_id': message['id'],
                      'model': message['providerID'] + '/' + message['modelID'],
                      'input_tokens': tokens.get('input'), 'output_tokens': tokens.get('output'),
                      'cache_read_tokens': cache.get('read'), 'cache_write_tokens': cache.get('write'),
                      'cost': message.get('cost') if accounted else None})
    return calls


def run(spec_path, run_dir):
    spec = json.loads(Path(spec_path).read_text(encoding='utf-8-sig'))
    root = Path(spec['directory']).resolve(strict=True)
    allowed = spec['allowed_files']
    if len(allowed) != len(set(allowed)) or not allowed:
        raise ValueError('Use distinct exact allowed paths')
    for name in allowed:
        p = root / name
        if Path(name).is_absolute() or not p.resolve().is_relative_to(root) or p.is_symlink():
            raise ValueError(f'Invalid allowed path: {name}')
    run_dir = Path(run_dir).resolve()
    run_dir.mkdir(parents=True, exist_ok=False)  # never overwrite/relaunch an existing run
    write_json(run_dir / 'spec.json', spec)
    before = snapshot(root)
    write_json(run_dir / 'before.json', before)
    pinned = {str(Path(p).resolve()): digest(p) for p in spec.get('pinned_files', [])}
    write_json(run_dir / 'pins.json', pinned)
    environment = os.environ.copy()
    environment.update(spec.get('environment', {}))  # paths/config only, never credential values
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
                if (run_dir / 'cancel.request').exists():
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
    after = snapshot(root)
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
    record['state'] = reason or 'completed'
    record['elapsed_seconds'] = round(record['ended'] - record['started'], 3)
    write_json(run_dir / 'after.json', after)
    write_json(run_dir / 'receipt.json', record)
    return record


def inspect(run_dir, action):
    run_dir = Path(run_dir).resolve()
    record = json.loads((run_dir / 'receipt.json').read_text(encoding='utf-8'))
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
    return record


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest='action', required=True)
    p = sub.add_parser('run')
    p.add_argument('spec')
    p.add_argument('run_directory')
    for action in ('status', 'cancel', 'collect'):
        sub.add_parser(action).add_argument('run_directory')
    args = parser.parse_args()
    result = run(args.spec, args.run_directory) if args.action == 'run' else inspect(args.run_directory, args.action)
    print(json.dumps(result, indent=2))
    return 0 if args.action != 'run' or result['state'] == 'completed' else 1


if __name__ == '__main__':
    sys.exit(main())
