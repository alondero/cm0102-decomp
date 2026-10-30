"""Candidate CLI and publication regressions; all writes use private recovery paths."""
import copy
import importlib.util
import json
import sys
from pathlib import Path

import pytest

import cmre
from cmre import coordination, env, ledger

HERE = Path(__file__).resolve().parent
VA = 0x00545A60
OLD = 'b22135d4b51681234ad554dea1554e82f3959367b4843714ba0a486025193a33'
NEW = 'b34dfe56807411bcf287db5efe52dfcaa85f0fbdbecd0a5ae9dda85fc087d79d'
FIXTURES = HERE / 'fixtures/lift-migration'
PROPOSAL = FIXTURES / 'proposed-hints.json'


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


@pytest.fixture
def private(tmp_path, monkeypatch):
    source = env.local_fixture(OLD)  # the accepted lift's original instructions stay local
    record = json.loads((FIXTURES / 'accepted-ledger.json').read_text(encoding='utf-8'))
    assert env.digest_bytes(source) == OLD and not record.get('c')
    if (HERE / 'lift_migration.py').is_file():
        # Isolated admission candidate. Installed tests import the production API.
        module = load('cmre.lift_migration', HERE / 'lift_migration.py')
        monkeypatch.setitem(sys.modules, 'cmre.lift_migration', module)
        monkeypatch.setattr(cmre, 'lift_migration', module, raising=False)
        candidate_cli = load('cmre._migration_cli_candidate', HERE / 'cli.py')
    else:
        from cmre import cli as candidate_cli, lift_migration as module
    for name in ('LEDGER_DIR', 'LIFTED_DIR', 'HINTS_DIR', 'C_DIR', 'RUNS'):
        directory = tmp_path / name.lower()
        directory.mkdir()
        monkeypatch.setattr(env, name, directory)
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(tmp_path / 'command.lock'))
    monkeypatch.delenv('CMRE_WRITER_TOKEN', raising=False)
    monkeypatch.delenv('CMRE_WRITER_PATH', raising=False)
    paths = module.paths_for(VA)
    record['lifted']['source'] = env.rel(paths['source'])
    record['hints'] = None
    paths['source'].write_bytes(source)
    coordination.atomic_json(paths['ledger'], record)
    return module, candidate_cli, paths, copy.deepcopy(record)


def snapshot(paths):
    return {key: path.read_bytes() if path.exists() else None for key, path in paths.items()}


def apply_args(old=OLD, new=NEW, proposal=PROPOSAL):
    return ['lift-migration', 'apply', 'fn_00545a60', '--hints', str(proposal),
            '--expected-old-sha256', old, '--expected-new-sha256', new]


def test_cli_requires_both_source_pins_before_execution(private, capsys):
    _, cli, paths, _ = private
    before = snapshot(paths)
    with pytest.raises(SystemExit) as error:
        cli.main(['lift-migration', 'apply', 'fn_00545a60', '--hints', str(PROPOSAL)])
    assert error.value.code == 2
    assert '--expected-old-sha256' in capsys.readouterr().err
    assert snapshot(paths) == before


def test_actual_helper_cli_migration_retains_provenance_and_no_source_credit(private, capsys):
    _, cli, paths, old_record = private
    assert cli.main(apply_args()) == 0
    result = json.loads(capsys.readouterr().out)
    directory = env.ROOT / result['journal']
    assert result['status'] == 'committed' and result['accepted_source_bytes_added'] == 0
    state = json.loads((directory / 'journal.json').read_text(encoding='utf-8'))
    assert state['schema'] == 'cmre-lift-migration/1'
    baseline = json.loads((directory / 'baseline-report.json').read_text(encoding='utf-8'))
    assert baseline['status'] == 'pass' and baseline['source_sha256'] == OLD
    assert baseline['key'] == state['baseline_key']
    assert baseline['difftest']['result_digest'] == state['baseline_digest']
    for phase in ('before', 'after'):
        for key, digest in state[phase].items():
            if digest is not None:
                assert env.digest(directory / f'{phase}-{key}') == digest
    assert env.digest(paths['source']) == NEW
    record = json.loads(paths['ledger'].read_text(encoding='utf-8'))
    assert record['status'] == old_record['status'] and not record.get('c')
    assert record['extent'] == old_record['extent']
    from cmre import cli as actual_cli
    assert actual_cli._revalidate_one(VA, refresh=False) == (VA, 'ok')
    assert ledger.verify() == []
    before = snapshot(paths)
    assert cli.main(['lift-migration', 'reconcile', str(directory)]) == 0
    assert json.loads(capsys.readouterr().out)['status'] == 'committed'
    assert snapshot(paths) == before


@pytest.mark.parametrize('case', ['wrong-old', 'C-source', 'weakened-policy', 'removed-fixture'])
def test_cli_rejects_unowned_or_weakened_proposals_without_publication(private, capsys, case):
    _, cli, paths, _ = private
    proposal = json.loads(PROPOSAL.read_text(encoding='utf-8'))
    old = OLD
    if case == 'wrong-old':
        old = '0' * 64
    elif case == 'C-source':
        (env.C_DIR / 'fn_00545a60.c').write_text('/* unrelated source */', encoding='utf-8')
    elif case == 'weakened-policy':
        proposal['cases'] = 1
    else:
        coordination.atomic_json(paths['hints'], {'fixtures': proposal['fixtures'][:1]})
        proposal['fixtures'] = []
    proposed = env.ANALYSIS / 'proposal.json'
    coordination.atomic_json(proposed, proposal)
    before = snapshot(paths)
    assert cli.main(apply_args(old=old, proposal=proposed)) == 2
    assert json.loads(capsys.readouterr().err)['status'] == 'error'
    assert snapshot(paths) == before


def prepared_journal(module, paths):
    """Synthetic publication states test recovery, never binary-fidelity evidence."""
    directory = env.ANALYSIS / 'synthetic-journal'
    directory.mkdir()
    before = snapshot(paths)
    after_record = json.loads(before['ledger'])
    after_record['lifted']['source_sha256'] = env.digest_bytes(b'new-source')
    after = {'source': b'new-source', 'hints': b'{"notes":"new hint"}\n',
             'ledger': module.encoded(after_record)}
    state = dict(schema='cmre-lift-migration/1', state='prepared', va=VA,
                 target_sha256=env.TARGET_SHA256,
                 paths={key: env.rel(path) for key, path in paths.items()},
                 before={key: env.digest_bytes(value) if value is not None else None
                         for key, value in before.items()},
                 after={key: env.digest_bytes(value) for key, value in after.items()})
    for phase, values in (('before', before), ('after', after)):
        for key, value in values.items():
            if value is not None:
                (directory / f'{phase}-{key}').write_bytes(value)
    coordination.atomic_json(directory / 'journal.json', state)
    return directory, before, after


@pytest.mark.parametrize('last_write', ['source', 'hints', 'ledger'])
def test_reconcile_recovers_whole_state_by_ledger_commit_point(private, capsys, last_write):
    module, cli, paths, _ = private
    directory, before, after = prepared_journal(module, paths)
    for key in ('source', 'hints', 'ledger'):
        paths[key].write_bytes(after[key])
        if key == last_write:
            break
    expected = 'committed' if last_write == 'ledger' else 'rolled-back'
    assert cli.main(['lift-migration', 'reconcile', str(directory)]) == 0
    assert json.loads(capsys.readouterr().out)['status'] == expected
    assert snapshot(paths) == (after if expected == 'committed' else before)
    assert cli.main(['lift-migration', 'reconcile', str(directory)]) == 0
    assert json.loads(capsys.readouterr().out)['status'] == expected


@pytest.mark.parametrize('key', ['source', 'hints', 'ledger'])
def test_reconcile_preserves_every_target_when_any_target_has_unrelated_edits(private, capsys, key):
    module, cli, paths, _ = private
    directory, _, after = prepared_journal(module, paths)
    paths['source'].write_bytes(after['source'])
    paths[key].write_bytes(b'unrelated edit')
    before = snapshot(paths)
    assert cli.main(['lift-migration', 'reconcile', str(directory)]) == 2
    assert 'outside this migration' in json.loads(capsys.readouterr().err)['detail']
    assert snapshot(paths) == before


def test_reconcile_rejects_corrupt_artifact_before_any_target_write(private, capsys):
    module, cli, paths, _ = private
    directory, _, after = prepared_journal(module, paths)
    paths['source'].write_bytes(after['source'])
    (directory / 'before-ledger').write_bytes(b'corrupt backup')
    before = snapshot(paths)
    assert cli.main(['lift-migration', 'reconcile', str(directory)]) == 2
    assert 'Backup/artifact changed' in json.loads(capsys.readouterr().err)['detail']
    assert snapshot(paths) == before


def test_revoked_inherited_owner_cannot_remove_replacement_hint(private, monkeypatch):
    module, _, paths, _ = private
    paths['hints'].write_bytes(b'replacement owner hint')
    with coordination.writer():
        monkeypatch.setattr(coordination, '_live', lambda *args: False)
        with pytest.raises(coordination.BusyError, match='expired before deletion'):
            module.remove_fenced(paths['hints'])
    assert paths['hints'].read_bytes() == b'replacement owner hint'


def test_command_refuses_active_other_writer_before_publication(private, capsys):
    _, cli, paths, _ = private
    before = snapshot(paths)
    with coordination.lock(coordination._path()):
        assert cli.main(apply_args()) == 2
    assert 'Writer active' in json.loads(capsys.readouterr().err)['detail']
    assert snapshot(paths) == before
