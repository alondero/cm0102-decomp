"""Scoped static guard: use the real production predicate and physical-step type."""
import subprocess
import sys
import pytest


def test_thread_ownership_rejects_bare_tids_at_typecheck_time():
    pytest.importorskip('mypy')  # CI installs the pinned optional checker explicitly.
    code = '''
from cmre.debug_observe import _owns_step, _PhysicalStep
from cmre.import_observe import ThreadInstance
owner = ThreadInstance(41, 2)
_owns_step(owner, owner)
_owns_step(owner, None)
_owns_step(41, owner)
_owns_step(owner, 41)
step: _PhysicalStep = {"owner": 41, "address": 1, "original_flags": 0, "executed": False}
'''
    result = subprocess.run(
        [sys.executable, '-m', 'mypy', '--config-file=', '--follow-imports=silent',
         '--ignore-missing-imports',
         '--no-pretty', '--no-color-output', '-c', code],
        capture_output=True, text=True)
    assert result.returncode == 1, result.stdout + result.stderr
    assert result.stdout.count('[arg-type]') == 2, result.stdout
    assert result.stdout.count('[typeddict-item]') == 1, result.stdout
