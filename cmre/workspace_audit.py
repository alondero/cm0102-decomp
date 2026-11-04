"""Opt-in Python-process audit for accesses to the real recovery tree.

The audit does not instrument child processes or native syscalls. It is an
experimental proof aid, not authority to reuse split results in session gates.
"""
import os
import sys
import threading

import pytest

# Derived from all events in the retained 2026-11-03 readset, including stat.
WORKSPACE_FILES = frozenset('''test_cmre_acceptance_policy.py test_cmre_boundaries.py test_cmre_compiler_generated.py test_cmre_composition.py test_cmre_composition_fault_owner.py test_cmre_composition_legacy_lift.py test_cmre_composition_separate.py test_cmre_drafts.py test_cmre_extended_indirect_transport.py test_cmre_fixtures.py test_cmre_jobs.py test_cmre_jump_transport.py test_cmre_lifted_budget.py test_cmre_owned_return_stop.py test_cmre_pipeline.py test_cmre_review_regressions.py test_cmre_screen.py test_cmre_selection.py test_cmre_semantic.py test_cmre_tasks_integration.py test_cmre_worktrees.py test_cmre_worktrees_integration.py test_cmre_x87_transport.py'''.split())

_active = None
_root = None
_violations = []
_original_stat = os.stat
_checking = threading.local()


def pytest_addoption(parser):
    parser.addoption('--cmre-workspace-audit', action='store_true', help='audit recovery reads in the pytest process')
    parser.addoption('--cmre-recovery-root', default=None, help='override recovery root for audit fixtures')


def _access(event, args):
    if getattr(_checking, 'active', False) or _active is None or _active or event not in {'open', 'os.listdir', 'os.scandir', 'glob.glob', 'os.stat'} or not args:
        return
    _checking.active = True
    try:
        target = args[0]
        if target is None or isinstance(target, int):
            return
        path = os.path.normcase(os.path.abspath(os.fsdecode(target)))
        if os.path.commonpath([path, _root]) == _root:
            _violations.append((event, path))
    except (TypeError, ValueError, OSError):
        return
    finally:
        _checking.active = False


def _stat(path, *args, **kwargs):
    _access('os.stat', (path,))
    return _original_stat(path, *args, **kwargs)


def pytest_configure(config):
    global _root
    config.addinivalue_line('markers', 'workspace: reads the real recovery tree')
    if config.getoption('--cmre-workspace-audit'):
        from . import env
        _root = os.path.normcase(os.path.abspath(config.getoption('--cmre-recovery-root') or env.RECOVERY))
        sys.addaudithook(_access)
        os.stat = _stat


def pytest_unconfigure(config):
    global _active, _root
    _active = None
    _root = None
    if os.stat is _stat:
        os.stat = _original_stat


def pytest_collection_modifyitems(items):
    for item in items:
        if item.path.name in WORKSPACE_FILES and item.path.parent.name == 'tests':
            item.add_marker(pytest.mark.workspace)


@pytest.hookimpl(hookwrapper=True)
def pytest_make_collect_report(collector):
    global _active
    if _root is None or not isinstance(collector, pytest.Module):
        yield
        return
    previous = _active
    _active = False
    _violations.clear()
    try:
        outcome = yield
        report = outcome.get_result()
        marked_file = collector.path.name in WORKSPACE_FILES and collector.path.parent.name == 'tests'
        items = report.result or []
        marked_items = bool(items) and all(item.get_closest_marker('workspace') for item in items)
        if _violations and not (marked_file or marked_items):
            event, path = _violations[0]
            report.outcome = 'failed'
            report.longrepr = f'unmarked recovery access during collection: {event} {path}'
        _violations.clear()
    finally:
        _active = previous


@pytest.hookimpl(hookwrapper=True)
def pytest_runtest_protocol(item, nextitem):
    global _active
    _violations.clear()
    _active = bool(item.get_closest_marker('workspace'))
    try:
        yield
    finally:
        _active = None


@pytest.hookimpl(hookwrapper=True)
def pytest_runtest_makereport(item, call):
    outcome = yield
    report = outcome.get_result()
    if _violations:
        event, path = _violations[0]
        report.outcome = 'failed'
        report.longrepr = f'unmarked recovery access: {event} {path}'
        _violations.clear()
