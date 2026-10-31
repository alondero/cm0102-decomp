"""Real Git worktree -> broker test -> review -> real C acceptance, all local."""
import json
import os
import shutil
import subprocess
import sys

from cmre import env, ledger, worktrees


def test_pinned_binary_worker_review_integrator_cycle(tmp_path, monkeypatch):
    original = env.ROOT
    root, worker = tmp_path / 'coordinator', tmp_path / 'worker'
    root.mkdir()
    shutil.copytree(original / 'cmre', root / 'cmre', ignore=shutil.ignore_patterns('__pycache__'))
    shutil.copytree(original / 'recovery/include', root / 'recovery/include')
    shutil.copy2(original / '.gitignore', root / '.gitignore')
    def git(*args):
        result = subprocess.run(['git', '-c', 'core.autocrlf=false', '-C', str(root), *args],
                                capture_output=True, text=True)
        assert result.returncode == 0, result.stderr
        return result.stdout
    git('init', '-q')
    git('add', '.gitignore', 'cmre', 'recovery/include')
    git('-c', 'user.name=Test', '-c', 'user.email=test@example.invalid', 'commit', '-qm', 'authored tooling')
    git('worktree', 'add', '--detach', str(worker))
    va = 0x004671b0
    record = json.loads(ledger.record_path(va).read_text(encoding='utf-8'))
    record.pop('c', None)
    record['status']['decompiled'] = False
    lift = (env.LIFTED_DIR / f'fn_{va:08x}.S').read_bytes()
    contract = (env.C_DIR / 'contracts' / f'fn_{va:08x}.json').read_bytes()
    pins = {root / path.relative_to(original): expected for path, expected in env.INVENTORY_SHA256.items()}
    # Only the isolated coordinator receives ignored local inputs. Workers have
    # no game, inventory, authoritative ledger, hints or asset junctions.
    for source in (env.TARGET, *env.INVENTORY_SHA256):
        destination = root / source.relative_to(original)
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, destination)
    for name, relative in (
        ('ROOT', '.'), ('ANALYSIS', 'CM0102/Analysis'), ('TARGET', 'CM0102/Game/cm0102.exe'),
        ('RECOVERY', 'recovery'), ('C_DIR', 'recovery/c'), ('LIFTED_DIR', 'recovery/lifted'),
        ('LEDGER_DIR', 'recovery/ledger'), ('BLOCKED_DIR', 'recovery/blocked'),
        ('HINTS_DIR', 'recovery/hints'), ('RUNS', 'CM0102/Analysis/cmre/runs'),
        ('REGISTRY', 'recovery/registry.json')):
        monkeypatch.setattr(env, name, root / relative)
    monkeypatch.setattr(env, 'INVENTORY_SHA256', pins)
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(env.ANALYSIS / 'cmre/command.lock'))
    monkeypatch.delenv('CMRE_WRITER_TOKEN', raising=False)
    for directory in (env.C_DIR / 'contracts', env.LIFTED_DIR, env.LEDGER_DIR, env.HINTS_DIR):
        directory.mkdir(parents=True, exist_ok=True)
    env.REGISTRY.write_text('{"current": []}', encoding='utf-8')
    ledger.record_path(va).write_text(json.dumps(record), encoding='utf-8')
    (env.LIFTED_DIR / f'fn_{va:08x}.S').write_bytes(lift)
    (env.C_DIR / 'contracts' / f'fn_{va:08x}.json').write_bytes(contract)
    hint = original / f'recovery/hints/fn_{va:08x}.json'
    if hint.exists():
        shutil.copy2(hint, env.HINTS_DIR / hint.name)
    worktrees.bootstrap(worker)
    worktrees.claim(worker, va, 'integration-translator')
    source = worker / f'recovery/c/fn_{va:08x}.c'
    source.parent.mkdir(parents=True)
    source.write_text('#include "cmre.h"\n'
                      'CM_THISCALL void *cm_fn_004671b0(void *self) { return (void *)((u32)self + 0x20u); }\n',
                      encoding='utf-8', newline='\n')
    child_env = dict(os.environ)
    for key in ('CMRE_WRITER_TOKEN', 'CMRE_WRITER_PATH', 'CMRE_COORDINATION_PATH', 'PYTHONPATH'):
        child_env.pop(key, None)
    tested = subprocess.run([sys.executable, '-m', 'cmre', 'worktree', 'test', f'fn_{va:08x}'],
                            cwd=worker, env=child_env, capture_output=True, text=True)
    assert tested.returncode == 0, tested.stderr + tested.stdout
    result = json.loads(tested.stdout)
    assert result['status'] == 'submitted'
    report = worker / result['report']
    evidence = json.loads(report.read_text(encoding='utf-8'))
    assert evidence['profile'] == 'semantic'
    assert set(evidence['builds']) == {'-O0', '-O2'}
    assert all(build['status'] == 'pass' for build in evidence['builds'].values())
    assert not (env.C_DIR / source.name).exists()
    worktrees.review(worker, va, reviewer='integration-reviewer',
                     reason='Checked original instructions and matching O0/O2 evidence',
                     source_sha256=env.digest(source), report_sha256=env.digest(report))
    assert worktrees.publish(worker, va)['status'] == 'published'
    accepted = json.loads(ledger.record_path(va).read_text(encoding='utf-8'))
    assert accepted['c']['source_sha256'] == env.digest(source)
    assert accepted['status']['decompiled'] is True
    assert accepted['status']['integrated'] is False
    assert not (worker / 'CM0102/Game').exists()
    assert not (worker / 'recovery/ledger').exists()
    assert not (worker / 'CM0102/Analysis').is_symlink()
    assert all(not path.startswith(('CM0102/', 'recovery/ledger/', 'recovery/lifted/'))
               for path in git('ls-files').splitlines())
    ignored = subprocess.run(['git', '-C', str(worker), 'check-ignore', str(report)],
                             capture_output=True, text=True)
    assert ignored.returncode == 0
