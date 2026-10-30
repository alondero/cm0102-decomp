"""OS-owned command leases and atomic local publication, independent of agent client.

An inherited random lease lets a supervised command's own pool workers publish.
Unrelated CLI/API writers must acquire the same OS lock. This coordinates trusted
local processes; it is not a sandbox against arbitrary filesystem writes.
"""
from __future__ import annotations

from contextlib import contextmanager, ExitStack
from functools import wraps
import json
import os
from pathlib import Path
import secrets
import tempfile
import time

from . import env


class BusyError(RuntimeError):
    pass


def _atomic_bytes(path: Path, data: bytes) -> None:
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    fd, temporary = tempfile.mkstemp(prefix=f'.{path.name}.', suffix='.tmp', dir=path.parent)
    try:
        with os.fdopen(fd, 'wb') as stream:
            stream.write(data)
            stream.flush()
            os.fsync(stream.fileno())
        os.replace(temporary, path)
    finally:
        Path(temporary).unlink(missing_ok=True)


def atomic_bytes(path: Path, data: bytes) -> None:
    token = os.environ.get('CMRE_WRITER_TOKEN')
    lease_path = os.environ.get('CMRE_WRITER_PATH')
    if token and lease_path and Path(lease_path).resolve() == _path().resolve():
        # Serialize the ownership check + replace with coordinator acquisition.
        # An in-flight orphan may finish computing, but cannot publish after its
        # parent releases ownership or a new command replaces the lease.
        with _gate(_path()):
            if not _live(_path(), token):
                raise BusyError('Inherited command lease expired before publication')
            _atomic_bytes(path, data)
    else:
        _atomic_bytes(path, data)


def atomic_json(path: Path, value) -> None:
    atomic_bytes(path, (json.dumps(value, indent=2, ensure_ascii=False) + '\n').encode('utf-8'))


@contextmanager
def lock(path: Path):
    """Nonblocking exclusive lock, automatically released on process termination."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open('a+b') as stream:
        if path.stat().st_size == 0:
            stream.write(b' ')
            stream.flush()
        stream.seek(0)
        try:
            if os.name == 'nt':
                import msvcrt
                msvcrt.locking(stream.fileno(), msvcrt.LK_NBLCK, 1)
            else:
                import fcntl
                fcntl.flock(stream, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except OSError as exc:
            raise BusyError(f'Writer active: {path}; inspect its receipt before retrying') from exc
        try:
            yield
        finally:
            stream.seek(0)
            if os.name == 'nt':
                msvcrt.locking(stream.fileno(), msvcrt.LK_UNLCK, 1)
            else:
                fcntl.flock(stream, fcntl.LOCK_UN)


def _path() -> Path:
    return Path(os.environ.get('CMRE_COORDINATION_PATH',
                               str(env.ROOT / 'CM0102/Analysis/cmre/command.lock')))


@contextmanager
def _gate(path: Path):
    # Contention is a short metadata/replace operation, not a whole machine job.
    deadline = time.monotonic() + 30
    while True:
        context = lock(path.with_suffix('.publication.lock'))
        try:
            context.__enter__()
            break
        except BusyError:
            if time.monotonic() >= deadline:
                raise
            time.sleep(0.01)
    try:
        yield
    finally:
        context.__exit__(None, None, None)


def _live(path: Path, token: str) -> bool:
    try:
        owner = json.loads(path.with_suffix('.json').read_text(encoding='utf-8'))
    except (OSError, ValueError):
        return False
    if not secrets.compare_digest(token, str(owner.get('token', ''))):
        return False
    try:
        with lock(path):
            pass
    except BusyError:
        return True
    return False


@contextmanager
def writer():
    """Join a live inherited command lease, or acquire a new exclusive lease."""
    path = _path()
    metadata = path.with_suffix('.json')
    inherited = os.environ.get('CMRE_WRITER_TOKEN')
    inherited_path = os.environ.get('CMRE_WRITER_PATH')
    if inherited and (inherited_path is None or Path(inherited_path).resolve() == path.resolve()):
        try:
            owner = json.loads(metadata.read_text(encoding='utf-8'))
        except (OSError, ValueError):
            owner = {}
        if secrets.compare_digest(inherited, str(owner.get('token', ''))):
            try:
                with lock(path):
                    pass  # An unlocked file describes an expired lease.
            except BusyError:
                yield
                return
        raise BusyError('Inherited command lease expired; parent must restart the supervised job')
    with ExitStack() as ownership:
        with _gate(path):
            ownership.enter_context(lock(path))
            token = secrets.token_hex(32)
            _atomic_bytes(metadata, (json.dumps({'pid': os.getpid(), 'token': token}) + '\n').encode())
        os.environ['CMRE_WRITER_TOKEN'] = token
        os.environ['CMRE_WRITER_PATH'] = str(path.resolve())
        try:
            yield
        finally:
            if inherited is None:
                os.environ.pop('CMRE_WRITER_TOKEN', None)
            else:
                os.environ['CMRE_WRITER_TOKEN'] = inherited
            if inherited_path is None:
                os.environ.pop('CMRE_WRITER_PATH', None)
            else:
                os.environ['CMRE_WRITER_PATH'] = inherited_path


def coordinated(function):
    @wraps(function)
    def call(*args, **kwargs):
        with writer():
            return function(*args, **kwargs)
    return call
