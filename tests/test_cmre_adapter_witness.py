"""Rejection controls for supplemental adapter witness admission."""
from types import SimpleNamespace
import copy

import pytest
import unicorn

from scripts import cmre_adapter_witness_checks as witness


class Code:
    def __init__(self, base, data):
        self.base, self.data = base, data
        self.text = SimpleNamespace(va=base, end=base + len(data))
        self.return_bindings = ()

    def read_upto(self, address, size):
        offset = address - self.base
        return self.data[offset:offset + size]


def synthetic(monkeypatch):
    original = Code(0x1000, bytes.fromhex('51 e800000000 59 c3'))
    native = Code(0x2000, bytes.fromhex('51 6806100000 e900000000 59 c3'))
    native.return_bindings = (SimpleNamespace(owner=0x1000, original_call=0x1001,
                                               original_return=0x1006, compiled_push=0x2001,
                                               compiled_continuation=0x200b),)
    built = SimpleNamespace(image=native, entry=0x2000, symbols={'v_00001006': 0x200b})
    monkeypatch.setattr(witness.image, 'primary', lambda: original)
    return built


def test_linked_stream_rejects_changed_instruction_and_wrong_label(monkeypatch):
    built = synthetic(monkeypatch)
    mapping, rows = witness.linked_instruction_map(0x1000, 0x1008, 0x1001, built)
    assert mapping[0x200b] == 0x1006 and len(rows) == 4
    built.image.data = bytes.fromhex('52 6806100000 e900000000 59 c3')
    with pytest.raises(ValueError, match='instruction mismatch'):
        witness.linked_instruction_map(0x1000, 0x1008, 0x1001, built)
    built = synthetic(monkeypatch)
    built.symbols = {'v_00001007': 0x200b}
    with pytest.raises(ValueError, match='inaccurate source label'):
        witness.linked_instruction_map(0x1000, 0x1008, 0x1001, built)


def test_fault_mapping_rejects_unverified_instruction_instead_of_interpolating():
    assert witness.normalize_fault([('fault',), 0x200b], {0x200b: 0x1006}, 0x2000, 0x2100)[-1] == 0x1006
    with pytest.raises(ValueError, match='unverified compiled fault PC'):
        witness.normalize_fault([('fault',), 0x200c], {0x200b: 0x1006}, 0x2000, 0x2100)


def test_protected_stack_control_rejects_wrong_page_and_wrong_site():
    obs = {'outcome': 'fault', 'detail': [(unicorn.UC_MEM_READ_PROT, 0x2000, 4), 0x1000],
           'regs': (0, 0, 0, 0, 0x2000)}
    witness.check_protected_fault(obs, 0x1000)
    for changed in ({**obs, 'detail': [(unicorn.UC_MEM_READ_PROT, 0x3000, 4), 0x1000]},
                    {**obs, 'detail': [(unicorn.UC_MEM_READ_PROT, 0x2000, 4), 0x1001]}):
        with pytest.raises(ValueError, match='expected protected stack read'):
            witness.check_protected_fault(changed, 0x1000)


def test_dirty_pages_include_original_and_shared_but_not_compiled_image():
    machine = SimpleNamespace(_original_pages={0x1000}, _common_pages={0x2000})
    result = SimpleNamespace(memory={0x1000: b'a', 0x2000: b'b', 0x3000: b'c'})
    assert witness.comparable_dirty_pages(machine, result) == {'4096': '61', '8192': '62'}


def test_nonreturning_corpus_seed_is_rejected():
    witness.require_returning_seed(SimpleNamespace(outcome='return', detail=()), 0x1000, 4)
    with pytest.raises(ValueError, match='case 4 did not return'):
        witness.require_returning_seed(SimpleNamespace(outcome='fault', detail=('fault',)), 0x1000, 4)


def test_output_must_be_new_and_inside_diagnostics(tmp_path):
    root = tmp_path / 'diagnostics'
    root.mkdir()
    assert witness.admit_output(root / 'run', root) == (root / 'run').resolve()
    existing = root / 'existing'
    existing.mkdir()
    for path in (tmp_path / 'Game' / 'run', root, existing):
        with pytest.raises(ValueError):
            witness.admit_output(path, root)


def test_output_rejects_symlink_redirection(tmp_path):
    root = tmp_path / 'diagnostics'
    root.mkdir()
    elsewhere = tmp_path / 'elsewhere'
    elsewhere.mkdir()
    link = root / 'redirect'
    try:
        link.symlink_to(elsewhere, target_is_directory=True)
    except OSError:
        pytest.skip('symlinks unavailable')
    with pytest.raises(ValueError):
        witness.admit_output(link / 'run', root)


def test_output_rejects_redirected_diagnostics_root(tmp_path):
    protected = tmp_path / 'Game'
    protected.mkdir()
    root = tmp_path / 'diagnostics'
    try:
        root.symlink_to(protected, target_is_directory=True)
    except OSError:
        pytest.skip('symlinks unavailable')
    with pytest.raises(ValueError):
        witness.admit_output(protected / 'run', root)


def test_output_rejects_explicit_protected_root(tmp_path):
    protected = tmp_path / 'Game'
    protected.mkdir()
    with pytest.raises(ValueError):
        witness.admit_output(protected / 'run', protected, protected_roots=[protected])


def test_task_validation_code_identity_detects_drift(tmp_path, monkeypatch):
    task = tmp_path / 'tasks.py'
    ledger = tmp_path / 'ledger.py'
    task.write_text('task-before', encoding='utf-8')
    ledger.write_text('ledger-before', encoding='utf-8')
    monkeypatch.setattr(witness.tasks, '__file__', str(task))
    monkeypatch.setattr(witness.ledger, '__file__', str(ledger))
    before = witness.validation_identity()
    task.write_text('task-after', encoding='utf-8')
    assert witness.validation_identity() != before
    task.write_text('task-before', encoding='utf-8')
    ledger.write_text('ledger-after', encoding='utf-8')
    assert witness.validation_identity() != before


def test_effective_entry_stack_pointer_is_explicit():
    case = SimpleNamespace(regs={'esp': 0x12345678}, fpcw=None)
    recorded = witness.entry_context(case)
    assert recorded['case_esp'] == 0x12345678
    assert recorded['entry_esp'] == witness.emu.STACK_TOP
    assert recorded['esp_overridden'] is True


def test_alias_controls_reject_wrong_order_or_addresses():
    source = 0x1000
    events = [('w', 0, 4, 0), ('r', source + 2, 1, 0), ('r', 0, 4, 0),
              ('w', 0, 4, 0), ('r', 0, 4, 0), ('w', 0, 4, 0),
              ('r', source + 4, 1, 0), ('w', 0, 4, 0), ('r', source + 3, 1, 0)]
    witness.check_alias_reads(events, source)
    wrong = list(events)
    wrong[6], wrong[8] = wrong[8], wrong[6]
    with pytest.raises(ValueError):
        witness.check_alias_reads(wrong, source)
    writes = [('w', 0x2000, 2, 1), ('r', 5, 1, 0), ('w', 0x2002, 1, 2)]
    witness.check_output_stores(writes, 0x2000)
    with pytest.raises(ValueError):
        witness.check_output_stores(writes, 0x2001)


def test_input_snapshot_rejects_drift_and_path_escape(tmp_path):
    source = tmp_path / 'source.c'
    source.write_bytes(b'original')
    digest = witness.env.digest(source)
    out = tmp_path / 'snapshot'
    witness.snapshot_inputs(out, tmp_path, {'source.c': digest})
    assert (out / 'source.c').read_bytes() == b'original'
    source.write_bytes(b'changed')
    with pytest.raises(ValueError):
        witness.snapshot_inputs(tmp_path / 'stale', tmp_path, {'source.c': digest})
    with pytest.raises(ValueError):
        witness.snapshot_inputs(tmp_path / 'escape', tmp_path, {'../outside': digest})


def test_submission_rejects_stale_source_report_and_identity():
    task = {'task_id': 't', 'status': 'submitted', 'attempts': [{'status': 'pass', 'candidate_sha256': 'a',
                                                                  'report_sha256': 'r'}]}
    report = {'status': 'pass', 'source_sha256': 'a', 'execution_identity': {'target': 'x'}}
    witness.check_submission('t', task, report, 'a', 'r', {'target': 'x'})
    for changed_task, changed_report, digest, report_digest, identity in [
        (task, report, 'b', 'r', {'target': 'x'}),
        (task, report, 'a', 'replaced', {'target': 'x'}),
        (task, {**report, 'status': 'fail'}, 'a', 'r', {'target': 'x'}),
        (task, report, 'a', 'r', {'target': 'y'}),
        ({**task, 'status': 'published'}, report, 'a', 'r', {'target': 'x'}),
        ({**task, 'attempts': copy.deepcopy(task['attempts']) * 2}, report, 'a', 'r', {'target': 'x'}),
    ]:
        with pytest.raises(ValueError, match='identity mismatch'):
            witness.check_submission('t', changed_task, changed_report, digest, report_digest, identity)
