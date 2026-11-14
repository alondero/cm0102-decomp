"""Withdrawal retains exact source/lift/provenance and fails closed before publication."""
import copy
import hashlib
import json
from pathlib import Path
import shutil
from types import SimpleNamespace

import pytest

from cmre import cli, coordination, env, inventory, ledger, queue, recover, retract, work

VA = 0x00401000


def write(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2) + '\n')


@pytest.fixture
def evidence(tmp_path, monkeypatch):
    paths = {'ROOT': tmp_path, 'RECOVERY': tmp_path / 'recovery',
             'ANALYSIS': tmp_path / 'CM0102/Analysis',
             'LEDGER_DIR': tmp_path / 'recovery/ledger',
             'C_DIR': tmp_path / 'recovery/c', 'LIFTED_DIR': tmp_path / 'recovery/lifted',
             'HINTS_DIR': tmp_path / 'recovery/hints', 'BLOCKED_DIR': tmp_path / 'recovery/blocked',
             'REGISTRY': tmp_path / 'recovery/registry.json',
             'RUNS': tmp_path / 'CM0102/Analysis/cmre/runs'}
    for name, path in paths.items():
        monkeypatch.setattr(env, name, path)
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(tmp_path / 'command.lock'))
    monkeypatch.delenv('CMRE_WRITER_TOKEN', raising=False)
    monkeypatch.delenv('CMRE_WRITER_PATH', raising=False)
    native = b'12345678'
    monkeypatch.setattr(ledger.image, 'primary', lambda: SimpleNamespace(read=lambda va, size: native[:size]))
    function = SimpleNamespace(body_bytes=5)
    monkeypatch.setattr(inventory, 'load', lambda: SimpleNamespace(functions={VA: function}, modules={}))
    monkeypatch.setattr(queue, 'compiler_generated', lambda va: False)
    write(env.REGISTRY, {'current': []})
    source = env.C_DIR / f'fn_{VA:08x}.c'
    source.parent.mkdir(parents=True)
    source.write_bytes(b'faithful source\r\n')
    lifted = env.LIFTED_DIR / f'fn_{VA:08x}.S'
    lifted.parent.mkdir(parents=True)
    lifted.write_bytes(b'validated original lift\n')
    header = env.RECOVERY / 'include/cmre.h'
    header.parent.mkdir(parents=True)
    header.write_bytes(b'header\n')
    hints = env.HINTS_DIR / f'fn_{VA:08x}.json'
    write(hints, {'args': ['ptr']})
    record = {'schema': ledger.SCHEMA, 'va': f'0x{VA:08x}', 'input_sha256': env.TARGET_SHA256,
              'extent': {'bytes': 8, 'sha256': hashlib.sha256(native).hexdigest()},
              'status': {'identified': True, 'decompiled': True, 'compiled': True,
                         'validated': True, 'integrated': False},
              'hints': env.rel(hints), 'lifted': {'source': env.rel(lifted), 'source_sha256': env.digest(lifted)},
              'c': {'source': env.rel(source), 'source_sha256': env.digest(source),
                    'harness_sha256': 'old-harness', 'run_key': '0123456789abcdef',
                    'source_dependencies': {env.rel(header): env.digest(header)}}}
    write(ledger.record_path(VA), record)
    report = env.RUNS / f'fn_{VA:08x}' / 'c-0123456789abcdef/report.json'
    write(report, {'key': record['c']['run_key'], 'status': 'pass',
                   'source_sha256': record['c']['source_sha256'], 'harness_sha256': 'old-harness'})
    witness = tmp_path / 'witness.json'
    write(witness, {'function': record['va'], 'accepted_source_sha256': env.digest(source),
                    'accepted_ledger_sha256': env.digest(ledger.record_path(VA)), 'case': 97,
                    'opt': '-O0', 'after_classifier': ['mismatch', {'field': 'memory'}]})
    return SimpleNamespace(record=record, source=source, lifted=lifted, header=header, hints=hints,
                           report=report, witness=witness, reason='Named mismatch invalidates C evidence')


@pytest.mark.parametrize('profile,source,bad_lines', [
    ('semantic', 'int f(void) { __asm__("nop"); return 1; }', False),
    ('abi', 'int f(void) { __asm__("nop"); return 1; }', False),
    ('semantic', 'int f(void) { /* __asm__("nop"); */ return 1; }', False),
    ('semantic', 'int f(void) { __asm__("nop"); return 1; }', True),
])
def test_semantic_assembly_withdrawal_requires_mechanical_proof(evidence, profile, source, bad_lines):
    from cmre.source_policy import inline_assembly_lines
    evidence.source.write_text(source)
    record = copy.deepcopy(evidence.record)
    record['c']['source_sha256'] = env.digest(evidence.source)
    record['c']['validation'] = {opt: {'profile': profile} for opt in ('-O0', '-O2')}
    write(ledger.record_path(VA), record)
    report = json.loads(evidence.report.read_text())
    report.update(source_sha256=env.digest(evidence.source), profile=profile)
    write(evidence.report, report)
    write(evidence.witness, {'kind': 'semantic-inline-assembly', 'function': record['va'],
        'accepted_source_sha256': env.digest(evidence.source),
        'accepted_ledger_sha256': env.digest(ledger.record_path(VA)),
        'lines': [99] if bad_lines else inline_assembly_lines(source)})
    if profile != 'semantic' or bad_lines or not inline_assembly_lines(source):
        with pytest.raises(ValueError, match='Witness must pin'):
            retract.preview(VA, evidence.reason, evidence.witness)
        return
    before = evidence.source.read_bytes()
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    result = retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    assert result['state'] == 'applied'
    assert evidence.source.read_bytes() == before
    assert 'c' not in json.loads(ledger.record_path(VA).read_text())


def test_preview_has_no_mutation_and_uses_inventory_body_bytes(evidence):
    before = ledger.record_path(VA).read_bytes()
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    assert ledger.record_path(VA).read_bytes() == before
    assert not Path(plan['archive']).exists() and not work.path().exists()
    assert plan['count_delta']['source_body_bytes'] == -5  # Extent is8; inventory body is5.
    assert plan['after_ledger']['lifted'] == evidence.record['lifted']
    assert plan['after_ledger']['status'] == {**evidence.record['status'], 'decompiled': False}
    assert 'c' not in plan['after_ledger']


def test_apply_preserves_bytes_lift_provenance_and_exact_verify_counts(evidence):
    keep = {p: p.read_bytes() for p in [evidence.source, evidence.lifted, evidence.header, evidence.hints, evidence.report, evidence.witness]}
    ledger_before = ledger.record_path(VA).read_bytes()
    counts_before = queue.coverage_summary()
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    result = retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    assert result['state'] == 'applied'
    assert all(p.read_bytes() == blob for p, blob in keep.items())
    record = json.loads(ledger.record_path(VA).read_text())
    assert 'c' not in record and record['lifted'] == evidence.record['lifted']
    assert record['status']['validated'] and record['status']['compiled']
    archive = Path(plan['archive'])
    assert (archive / 'ledger-before.json').read_bytes() == ledger_before
    assert json.loads((archive / 'accepted-c-metadata.json').read_text()) == evidence.record['c']
    assert (archive / 'accepted-source.c').read_bytes() == keep[evidence.source]
    assert record['c_retractions'][0]['archive_manifest_sha256'] == env.digest(archive / 'manifest.json')
    assert work.read()['functions'][f'fn_{VA:08x}']['state'] == 'evidence-needed'
    assert ledger.verify() == []
    counts_after = queue.coverage_summary()
    for name, delta in plan['count_delta'].items():
        assert counts_after[name] - counts_before[name] == delta
    assert counts_after['source_functions'] == 0 and counts_after['recovered_functions'] == 1
    # Original ledger can be recovered byte-for-byte; no automatic restoration or reacceptance.
    assert hashlib.sha256(ledger_before).hexdigest() == plan['before_ledger_sha256']


@pytest.mark.parametrize('change', ['ledger', 'source', 'witness', 'report', 'header', 'hints', 'lift'])
def test_stale_preview_refused(evidence, change):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    target = {'ledger': ledger.record_path(VA), 'source': evidence.source,
              'witness': evidence.witness, 'report': evidence.report, 'header': evidence.header,
              'hints': evidence.hints, 'lift': evidence.lifted}[change]
    target.write_bytes(target.read_bytes() + b' ')
    before = ledger.record_path(VA).read_bytes()
    with pytest.raises(ValueError):
        retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    assert ledger.record_path(VA).read_bytes() == before and not work.path().exists()


@pytest.mark.parametrize('kind', ['absent', 'not-json', 'other-function', 'no-mismatch', 'wrong-source'])
def test_bad_witness_refused(evidence, kind):
    if kind == 'absent':
        evidence.witness.unlink()
    elif kind == 'not-json':
        evidence.witness.write_text('A witness assertion without captured evidence')
    else:
        proof = json.loads(evidence.witness.read_text())
        proof[{'other-function': 'function', 'no-mismatch': 'after_classifier',
               'wrong-source': 'accepted_source_sha256'}[kind]] = {
                   'other-function': '0x00402000', 'no-mismatch': ['equal', {}],
                   'wrong-source': '0' * 64}[kind]
        write(evidence.witness, proof)
    before = ledger.record_path(VA).read_bytes()
    with pytest.raises(ValueError):
        retract.preview(VA, evidence.reason, evidence.witness)
    assert ledger.record_path(VA).read_bytes() == before and not work.path().exists()


def test_legacy_refused(evidence, monkeypatch):
    monkeypatch.setattr(ledger, 'legacy_entries', lambda: [SimpleNamespace(va=VA)])
    with pytest.raises(ValueError, match='Legacy'):
        retract.preview(VA, evidence.reason, evidence.witness)


def test_no_c_refused(evidence):
    record = copy.deepcopy(evidence.record)
    record.pop('c')
    write(ledger.record_path(VA), record)
    with pytest.raises(ValueError, match='no active accepted C'):
        retract.preview(VA, evidence.reason, evidence.witness)


def test_apply_requires_reviewed_hash(evidence):
    with pytest.raises(ValueError, match='expect-plan'):
        retract.apply(VA, evidence.reason, evidence.witness, None)


def test_archival_failure_leaves_ledger_and_work_unchanged(evidence, monkeypatch):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    before = ledger.record_path(VA).read_bytes()
    def failed_archive(*args):
        raise OSError('Disk or archival failure')
    monkeypatch.setattr(retract, '_archive', failed_archive)
    with pytest.raises(OSError, match='archival failure'):
        retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    assert ledger.record_path(VA).read_bytes() == before and not work.path().exists()


def test_publication_failure_keeps_old_c_and_conservative_hold(evidence, monkeypatch):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    before = ledger.record_path(VA).read_bytes()
    original = coordination.atomic_bytes
    def fail_ledger(path, blob):
        if Path(path) == ledger.record_path(VA):
            raise OSError('Ledger publication failed')
        original(path, blob)
    monkeypatch.setattr(coordination, 'atomic_bytes', fail_ledger)
    with pytest.raises(OSError, match='publication failed'):
        retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    assert ledger.record_path(VA).read_bytes() == before
    assert work.read()['functions'][f'fn_{VA:08x}']['state'] == 'evidence-needed'
    assert (Path(plan['archive']) / 'ledger-before.json').read_bytes() == before


def test_archive_tampering_is_detected_by_verify(evidence):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    (Path(plan['archive']) / 'accepted-source.c').write_bytes(b'changed')
    assert any('retraction archive' in p for p in ledger.verify())


def test_cli_defaults_to_preview_and_apply_needs_explicit_hash(evidence, capsys):
    args = ['retract-c', f'fn_{VA:08x}', '--reason', evidence.reason, '--witness', str(evidence.witness)]
    assert cli.main(args) == 0
    plan = json.loads(capsys.readouterr().out)
    assert plan['state'] == 'preview' and not Path(plan['archive']).exists()
    assert cli.main(args + ['--apply']) == 2
    assert 'expect-plan' in capsys.readouterr().err


def test_live_claim_refused(evidence, monkeypatch):
    monkeypatch.setattr(work, 'claims', lambda: {f'fn_{VA:08x}': {'owner': 'another-session'}})
    with pytest.raises(ValueError, match='live claim'):
        retract.preview(VA, evidence.reason, evidence.witness)


def test_apply_respects_existing_writer_lease(evidence):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    before = ledger.record_path(VA).read_bytes()
    with coordination.lock(coordination._path()):
        with pytest.raises(coordination.BusyError):
            retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    assert ledger.record_path(VA).read_bytes() == before and not Path(plan['archive']).exists()


def test_archive_directory_never_overwritten(evidence):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    folder = Path(plan['archive'])
    folder.mkdir(parents=True)
    marker = folder / 'retained-first-attempt'
    marker.write_bytes(b'keep')
    before = ledger.record_path(VA).read_bytes()
    with pytest.raises(FileExistsError):
        retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    assert marker.read_bytes() == b'keep' and ledger.record_path(VA).read_bytes() == before


def test_existing_hard_hold_is_preserved_and_withdrawal_hold_added(evidence):
    work.update(VA, 'harness-blocker', 'Earlier boundary', 'Resolve earlier boundary', 'earlier')
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    hold = work.read()['functions'][f'fn_{VA:08x}']
    assert hold['blocker'] == 'earlier'
    assert hold['additional_holds'][0]['blocker'] == 'withdrawn-c-evidence'


def test_final_receipt_failure_reports_actual_withdrawal_without_hiding_it(evidence, monkeypatch):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    original_open = Path.open
    def failed_receipt(path, *args, **kwargs):
        if path.name == 'applied.json':
            raise OSError('Final receipt write unavailable')
        return original_open(path, *args, **kwargs)
    monkeypatch.setattr(Path, 'open', failed_receipt)
    result = retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    assert result['state'] == 'applied-receipt-incomplete'
    assert 'c' not in json.loads(ledger.record_path(VA).read_text())
    assert ledger.verify() == []


def test_lift_refresh_retains_withdrawal_history(evidence, monkeypatch):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    previous = json.loads(ledger.record_path(VA).read_text())
    key = 'a' * 16
    run_source = env.RUNS / f'fn_{VA:08x}' / key / evidence.lifted.name
    run_source.parent.mkdir(parents=True)
    run_source.write_bytes(evidence.lifted.read_bytes())
    identity = {'fixture': 'static lift refresh; no candidate execution'}
    report = {'status': 'pass', 'tier': 'lifted', 'key': key,
              'source_sha256': env.digest(evidence.lifted), 'execution_identity': identity,
              'difftest': {'result_digest': 'same'}, 'lift': {'callees': [], 'imports': []}}
    monkeypatch.setattr(recover, 'test_lifted', lambda *args, **kwargs: report)
    monkeypatch.setattr(recover, 'execution_identity', lambda: identity)
    monkeypatch.setattr(recover, '_tier_record', lambda report, source: previous['lifted'])
    body = SimpleNamespace(name='retained', va=VA, tables={},
                           insns={VA: SimpleNamespace(va=VA, size=8)})
    monkeypatch.setattr(recover.lift, 'discover', lambda va: body)
    ok, result = recover.accept(VA, 'lifted', refresh=True)
    assert ok and result['status'] == 'accepted'
    after = json.loads(ledger.record_path(VA).read_text())
    assert after.get('c_retractions') == previous['c_retractions']
    assert 'c' not in after and ledger.verify() == []


def test_partial_archival_failure_does_not_publish_withdrawal(evidence, monkeypatch):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    original = retract._exclusive_bytes
    writes = []
    def partial(path, blob):
        writes.append(path)
        if len(writes) == 3:
            raise OSError('Archive stopped after two retained files')
        original(path, blob)
    monkeypatch.setattr(retract, '_exclusive_bytes', partial)
    before = ledger.record_path(VA).read_bytes()
    with pytest.raises(OSError, match='two retained files'):
        retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    assert ledger.record_path(VA).read_bytes() == before and not work.path().exists()
    assert all(path.is_file() for path in writes[:2])
    assert not (Path(plan['archive']) / 'applied.json').exists()


def isolate_implementation(tmp_path, monkeypatch):
    source = Path(retract.__file__).parent
    target = tmp_path / 'reviewed-implementation'
    for module in source.rglob('*.py'):
        destination = target / module.relative_to(source)
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(module, destination)
    monkeypatch.setattr(retract, '__file__', str(target / 'retract.py'))
    return target


@pytest.mark.parametrize('module', ['difftest.py', 'emu.py', 'build.py', 'env.py',
                                    'work.py', 'coordination.py', 'cli.py', 'queue.py'])
def test_reviewed_plan_pins_fresh_harness_and_policy_bytes(evidence, tmp_path, monkeypatch, module):
    implementation = isolate_implementation(tmp_path, monkeypatch)
    recover.harness_digest()  # A cached harness digest must not conceal the change.
    first = retract.preview(VA, evidence.reason, evidence.witness)
    path = implementation / module
    path.write_bytes(path.read_bytes() + b'\n# changed after reviewed preview\n')
    second = retract.preview(VA, evidence.reason, evidence.witness)
    assert first['plan_sha256'] != second['plan_sha256']
    assert first['archive_manifest']['implementation_sha256'][module] != second['archive_manifest']['implementation_sha256'][module]
    if module in recover.HARNESS_MODULES:
        assert first['archive_manifest']['harness_sha256'] != second['archive_manifest']['harness_sha256']
    before = ledger.record_path(VA).read_bytes()
    with pytest.raises(ValueError, match='stale'):
        retract.apply(VA, evidence.reason, evidence.witness, first['plan_sha256'])
    assert ledger.record_path(VA).read_bytes() == before and not work.path().exists()


def test_implementation_drift_during_archive_cannot_publish(evidence, tmp_path, monkeypatch):
    implementation = isolate_implementation(tmp_path, monkeypatch)
    first = retract.preview(VA, evidence.reason, evidence.witness)
    original_archive = retract._archive
    def archive_then_drift(*args):
        folder = original_archive(*args)
        path = implementation / 'work.py'
        path.write_bytes(path.read_bytes() + b'\n# policy drift during archival\n')
        return folder
    monkeypatch.setattr(retract, '_archive', archive_then_drift)
    before = ledger.record_path(VA).read_bytes()
    with pytest.raises(ValueError, match='changed during archival'):
        retract.apply(VA, evidence.reason, evidence.witness, first['plan_sha256'])
    assert ledger.record_path(VA).read_bytes() == before and not work.path().exists()
    assert (Path(first['archive']) / 'manifest.json').exists()


@pytest.mark.parametrize('initially_present', [False, True])
def test_canonical_hints_are_pinned_when_ledger_hints_is_null(evidence, initially_present):
    record = json.loads(ledger.record_path(VA).read_text())
    record['hints'] = None
    write(ledger.record_path(VA), record)
    proof = json.loads(evidence.witness.read_text())
    proof['accepted_ledger_sha256'] = env.digest(ledger.record_path(VA))
    write(evidence.witness, proof)
    if not initially_present:
        evidence.hints.unlink()
    first = retract.preview(VA, evidence.reason, evidence.witness)
    assert (first['archive_manifest']['canonical_hints']['sha256'] is not None) == initially_present
    write(evidence.hints, {'args': ['small']})
    second = retract.preview(VA, evidence.reason, evidence.witness)
    assert first['plan_sha256'] != second['plan_sha256']
    with pytest.raises(ValueError, match='stale'):
        retract.apply(VA, evidence.reason, evidence.witness, first['plan_sha256'])
    assert 'c' in json.loads(ledger.record_path(VA).read_text()) and not work.path().exists()


def test_post_commit_ledger_digest_read_is_not_required(evidence, monkeypatch):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    original_digest = env.digest
    def unavailable_after_commit(path):
        if Path(path) == ledger.record_path(VA) and 'c' not in json.loads(Path(path).read_text()):
            raise OSError('Post-commit ledger digest read unavailable')
        return original_digest(path)
    monkeypatch.setattr(env, 'digest', unavailable_after_commit)
    result = retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    assert result['state'] == 'applied'
    assert result['after_ledger_sha256'] == hashlib.sha256(ledger.record_path(VA).read_bytes()).hexdigest()


def test_intact_local_archive_relocates_without_rewriting_history(evidence, tmp_path, monkeypatch):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    record = json.loads(ledger.record_path(VA).read_text())
    reference = record['c_retractions'][0]['archive_manifest']
    assert not Path(reference).is_absolute() and '..' not in Path(reference).parts
    archive = Path(plan['archive'])
    restored = tmp_path / 'restored-workspace'
    shutil.copytree(env.RECOVERY, restored / 'recovery')
    restored_archive = restored / archive.relative_to(tmp_path)
    shutil.copytree(archive, restored_archive)
    moved_archive = archive.with_name(archive.name + '-retained-original')
    assert archive.resolve().is_relative_to(tmp_path.resolve()) and moved_archive.resolve().is_relative_to(tmp_path.resolve())
    archive.rename(moved_archive)
    monkeypatch.setattr(env, 'ROOT', restored)
    monkeypatch.setattr(env, 'RECOVERY', restored / 'recovery')
    monkeypatch.setattr(env, 'ANALYSIS', restored / 'CM0102/Analysis')
    monkeypatch.setattr(env, 'LEDGER_DIR', restored / 'recovery/ledger')
    monkeypatch.setattr(env, 'REGISTRY', restored / 'recovery/registry.json')
    assert retract.verify_history(record) == [] and ledger.verify() == []
    (restored_archive / 'accepted-source.c').write_bytes(b'tampered restored archive')
    assert any('retraction archive' in p for p in ledger.verify())


@pytest.mark.parametrize('reference', ['/absolute/manifest.json', '../manifest.json',
                                      'CM0102/Analysis/cmre/c-retractions/fn_00402000/0000000000000000/manifest.json',
                                      'CM0102\\Analysis\\manifest.json'])
def test_history_rejects_paths_outside_its_validated_relative_archive(evidence, reference):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    record = json.loads(ledger.record_path(VA).read_text())
    record['c_retractions'][0]['archive_manifest'] = reference
    assert retract.verify_history(record)


@pytest.mark.parametrize('profile', ['abi', 'semantic'])
@pytest.mark.parametrize('inputs', [None, 'v1', 'v2'])
def test_withdrawal_preserves_original_profile_inputs_and_effective_options(evidence, profile, inputs):
    record = json.loads(ledger.record_path(VA).read_text())
    settings = {'profile': profile}
    if inputs is not None:
        settings['inputs'] = inputs
    record['c']['validation'] = {'-O0': dict(settings), '-O2': dict(settings)}
    write(ledger.record_path(VA), record)
    proof = json.loads(evidence.witness.read_text())
    proof['accepted_ledger_sha256'] = env.digest(ledger.record_path(VA))
    write(evidence.witness, proof)
    from cmre import difftest
    before_options = difftest.Options.for_function(VA, tier='c')
    before_options.inputs = recover.c_inputs(VA)
    assert recover.c_profile(VA) == profile and before_options.inputs == (inputs or 'v1')
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    after_options = difftest.Options.for_function(VA, tier='c')
    after_options.inputs = recover.c_inputs(VA)
    assert recover.c_profile(VA) == profile and after_options == before_options
    assert 'c' not in json.loads(ledger.record_path(VA).read_text())
    archive = Path(plan['archive'])
    retained = json.loads((archive / 'validation-options-before.json').read_text())
    assert retained == json.loads(json.dumps(before_options.__dict__))
    assert ledger.verify() == []


@pytest.mark.parametrize('malformation', ['missing-contract', 'bad-profile', 'bad-inputs', 'bad-options-hash'])
def test_withdrawn_policy_fails_closed_when_retained_contract_is_invalid(evidence, malformation):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    record = json.loads(ledger.record_path(VA).read_text())
    entry = record['c_retractions'][-1]
    if malformation == 'missing-contract':
        entry.pop('validation_contract')
    else:
        key = {'bad-profile': 'profile', 'bad-inputs': 'inputs',
               'bad-options-hash': 'effective_options_sha256'}[malformation]
        entry['validation_contract'][key] = 'invalid'
    write(ledger.record_path(VA), record)
    with pytest.raises(ValueError, match='missing or malformed'):
        recover.c_inputs(VA)
    with pytest.raises(ValueError, match='missing or malformed'):
        recover.c_profile(VA)
    assert retract.verify_history(record)


@pytest.mark.parametrize('field,value', [('profile', 'abi'), ('inputs', 'v2')])
def test_type_valid_policy_edit_cannot_override_its_hash_bound_archive(evidence, field, value):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    record = json.loads(ledger.record_path(VA).read_text())
    assert record['c_retractions'][-1]['validation_contract'][field] != value
    record['c_retractions'][-1]['validation_contract'][field] = value
    write(ledger.record_path(VA), record)
    with pytest.raises(ValueError, match='changed'):
        recover.c_inputs(VA)
    with pytest.raises(ValueError, match='changed'):
        recover.c_profile(VA)
    assert retract.verify_history(record)


@pytest.mark.parametrize('history', [None, []])
def test_present_but_invalid_history_cannot_select_new_c_defaults(evidence, history):
    plan = retract.preview(VA, evidence.reason, evidence.witness)
    retract.apply(VA, evidence.reason, evidence.witness, plan['plan_sha256'])
    record = json.loads(ledger.record_path(VA).read_text())
    record['c_retractions'] = history
    write(ledger.record_path(VA), record)
    with pytest.raises(ValueError, match='missing or malformed'):
        recover.c_inputs(VA)
    assert ledger.verify()
