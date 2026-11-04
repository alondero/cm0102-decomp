"""Run main-checkout pytest using only the candidate workspace audit plugin.

Usage: py -3.13 <candidate>/scripts/cmre_candidate_audit.py <main-root>
The main checkout's cmre package, env, tests, and child processes remain in use.
This is an experimental audit, not a session gate or proof of subprocess reads.
"""
import importlib.util
import os
from pathlib import Path
import sys


def main():
    if len(sys.argv) != 2:
        raise SystemExit('Pass the main checkout root as the sole argument')
    main_root = Path(sys.argv[1]).resolve(strict=True)
    candidate = Path(__file__).resolve().parents[1] / 'cmre/workspace_audit.py'
    os.chdir(main_root)
    sys.path.insert(0, str(main_root))
    from cmre import env
    if env.ROOT.resolve() != main_root:
        raise SystemExit('Imported cmre package does not belong to the requested main root')
    name = 'cmre.workspace_audit'
    spec = importlib.util.spec_from_file_location(name, candidate)
    plugin = importlib.util.module_from_spec(spec)
    sys.modules[name] = plugin
    spec.loader.exec_module(plugin)
    import pytest
    return pytest.main(['tests', 'scripts/tests', '--cmre-workspace-audit', '-p', name])


if __name__ == '__main__':
    raise SystemExit(main())
