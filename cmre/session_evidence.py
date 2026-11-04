"""Exact-input reuse of successful full pytest steps; never substitutes for verify."""
from __future__ import annotations

import importlib.metadata
import json
import os
from pathlib import Path
import subprocess
import sys

from . import env, validation


def identity(root: Path, *, scope: str = 'full') -> dict:
    if scope not in ('full', 'tooling', 'workspace'):
        raise ValueError('Unknown pytest identity scope')
    # Include ignored recovery inputs and local fixtures: tests use them too.
    # Git supplies all authored files, including untracked additions/deletions.
    listing = subprocess.run(['git', 'ls-files', '-co', '--exclude-standard', '-z'], cwd=root,
                             capture_output=True, check=True).stdout.decode('utf-8')
    paths = {root / name for name in listing.split('\0') if name}
    # This checkout excludes unknown files locally for publication safety. Scan
    # authored code/test roots explicitly so a new ignored module/test is still
    # a declared dependency before it has been staged.
    for directory in ('cmre', 'scripts', 'tests', 'recovery', 'local/fixtures', '.agents'):
        paths.update(p for p in (root / directory).rglob('*') if p.is_file())
    paths.update([root / 'CM0102/Game/cm0102.exe',
                  *(root / env.rel(p) for p in env.INVENTORY_SHA256)])
    if scope == 'tooling':
        paths = {p for p in paths if not p.is_relative_to(root / 'recovery')}
    files = {str(p.relative_to(root)).replace('\\', '/'): env.digest(p) if p.is_file() else None
             for p in sorted(paths) if '__pycache__' not in p.parts and p.suffix != '.pyc'}
    packages = sorted([d.metadata['Name'], d.version] for d in importlib.metadata.distributions())
    # Compiler discovery is environment-dependent; subprocesses and tests can
    # also read PYTHON*/CMRE* and pytest's own options.
    variables = {k: v for k, v in os.environ.items()
                 if k.startswith(('PYTHON', 'PYTEST', 'CMRE'))
                 or k in validation.BUILD_ENVIRONMENT or k == 'PATH'}
    variables = {k: v for k, v in variables.items()
                 if k not in {'PYTEST_CURRENT_TEST', 'CMRE_WRITER_TOKEN', 'CMRE_WRITER_PATH'}}
    result = {'schema': 'cmre-pytest-inputs/1', 'files': files, 'python': sys.version,
            'executable': sys.executable, 'packages': packages, 'environment': variables,
            'toolchain': {k: env.digest(Path(env.tool(k))) for k in ('gcc', 'as', 'ld', 'nm')}}
    if scope != 'full':
        result['scope'] = scope
    return result


def successful_step(path: Path, expected: dict) -> dict | None:
    """Only an actual completed full-suite step can seed reuse (no chains)."""
    try:
        data = json.loads(path.read_text(encoding='utf-8'))
        for index, step in enumerate(data.get('steps', [])):
            if (step.get('command') == ['py', '-3.13', '-m', 'pytest']
                    and step.get('state') == 'finished' and step.get('exit_code') == 0
                    and step.get('inputs_before') == expected == step.get('inputs_after')
                    and step.get('evidence') == 'executed'):
                log = path.parent / step['log']
                if log.is_file() and env.digest(log) == step.get('log_sha256'):
                    return {'receipt': str(path.resolve()), 'sha256': env.digest(path),
                            'step': index, 'log_sha256': step['log_sha256']}
    except (OSError, ValueError, KeyError, TypeError):
        return None
    return None


def find(sessions: Path, expected: dict, exclude: Path) -> dict | None:
    for path in sorted(sessions.glob('*/receipt.json'), reverse=True):
        if path != exclude and (found := successful_step(path, expected)):
            return found
    return None
