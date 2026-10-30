"""A mapping witness never overrides native faults or true relocated pointers."""
from copy import deepcopy
from dataclasses import replace
from types import SimpleNamespace

import pytest
import unicorn

from cmre import build, composition, difftest, emu, image


@pytest.fixture
def paired(tmp_path):
    va = 0x00768290
    lifted, _text, compiled = build.lift_and_build(va, tmp_path)
    assert compiled is not None, lifted.problems
    mapping = emu.AddressMap(compiled.symbols, {}, compiled.image.text.va, compiled.image.text.end)

    def run(original_pointer, recovered_pointer):
        def case(pointer):
            return emu.Case({'ecx': pointer, 'eflags': 0x202}, [0] * 10, bytes(emu.ARENA_SIZE))
        a = emu.Machine(image.primary()).run(va, case(original_pointer), 20)
        b = emu.Machine(image.primary(), compiled.image).run(compiled.entry, case(recovered_pointer), 20)
        return a, b

    a, b = run(compiled.entry, compiled.entry)
    assert a.detail[0] == (20, compiled.entry, 4)
    assert b.detail[0] == (22, compiled.entry, 4)
    assert b.fault_mapping is not None
    return va, compiled.entry, mapping, run, a, b


def normalized(a, b, mapping, profile='exact'):
    return difftest._normalize(a, None, profile), difftest._normalize(b, mapping, profile)


@pytest.mark.parametrize('profile', ['exact', 'abi', 'semantic'])
def test_same_raw_fault_address_needs_complete_mapping_witness(paired, profile):
    _va, _entry, mapping, _run, a, b = paired
    na, nb = normalized(a, b, mapping, profile)
    assert difftest._diff(na, nb, mapping)['field'] == 'detail'
    assert difftest._diff(na, nb, mapping, fault_mapping=b.fault_mapping) == {}
    assert a.detail[0][0] == 20 and b.detail[0][0] == 22


@pytest.mark.parametrize('profile', ['exact', 'abi', 'semantic'])
def test_true_relocated_code_pointer_keeps_native_protection_fault(paired, profile):
    va, entry, mapping, run, _a, _b = paired
    # Explicit seam setup models one known code pointer before/after relocation.
    a, b = run(va, entry)
    assert a.detail[0] == (22, va, 4)
    assert b.detail[0] == (22, entry, 4)
    assert b.fault_mapping is not None
    na, nb = normalized(a, b, mapping, profile)
    assert difftest._diff(na, nb, mapping, fault_mapping=b.fault_mapping) == {}
    assert not difftest._mapping_fault_pair(na[1], nb[1], b.fault_mapping)


@pytest.mark.parametrize('change', [
    'address', 'size', 'schema', 'raw_access', 'projected_access', 'applied', 'relation',
    'missing_page', 'wrong_page', 'original_page', 'common_page', 'initial_write',
    'current_write', 'unknown_permission', 'writable_section', 'wrong_section',
])
def test_missing_stale_or_corrupted_mapping_witness_cannot_allow_fault(paired, change):
    _va, _entry, mapping, _run, a, b = paired
    proof = deepcopy(b.fault_mapping)
    if change in ('address', 'size'):
        proof[change] += 1
    elif change == 'schema':
        proof['schema'] = 'untrusted'
    elif change == 'raw_access':
        proof['raw_access'] = 20
    elif change == 'projected_access':
        proof['projected_access'] = 22
    elif change == 'applied':
        proof['applied'] = True
    elif change == 'relation':
        proof['relation'] = 'general-protection-fault'
    elif change == 'missing_page':
        proof['pages'] = []
    elif change == 'wrong_page':
        proof['pages'][0]['page'] += emu.PAGE
    elif change in ('original_page', 'common_page'):
        proof['pages'][0]['original_mapped' if change == 'original_page' else 'common_mapped'] = True
    elif change in ('initial_write', 'current_write', 'unknown_permission'):
        key = 'initial_perms' if change == 'initial_write' else 'current_perms'
        proof['pages'][0][key] |= 8 if change == 'unknown_permission' else unicorn.UC_PROT_WRITE
    elif change == 'writable_section':
        proof['pages'][0]['recovered_sections'][0]['writable'] = True
    elif change == 'wrong_section':
        proof['pages'][0]['recovered_sections'][0]['va'] += 0x10000
    na, nb = normalized(a, b, mapping)
    assert difftest._diff(na, nb, mapping, fault_mapping=proof)['field'] == 'detail'


@pytest.mark.parametrize('change', ['address', 'width', 'eip', 'access', 'relocated_address'])
def test_mapping_exception_never_hides_a_different_fault(paired, change):
    va, _entry, mapping, _run, a, b = paired
    (access, address, width), eip = a.detail
    if change == 'address':
        address += 1
    elif change == 'width':
        width = 2
    elif change == 'eip':
        eip += 1
    elif change == 'access':
        access = 19
    elif change == 'relocated_address':
        address = va
    a = replace(a, detail=((access, address, width), eip))
    na, nb = normalized(a, b, mapping)
    assert difftest._diff(na, nb, mapping, fault_mapping=b.fault_mapping)['field'] == 'detail'


def test_experimental_report_retains_witness_and_still_requires_a_return(tmp_path, monkeypatch):
    va = 0x00768290
    lifted, _text, compiled = build.lift_and_build(va, tmp_path)
    assert compiled is not None, lifted.problems
    case = emu.Case({'ecx': compiled.entry, 'eflags': 0x202}, [0] * 10, bytes(emu.ARENA_SIZE))
    a = emu.Machine(image.primary()).run(va, case, 20)
    b = emu.Machine(image.primary(), compiled.image).run(compiled.entry, case, 20)
    # Actual native results above supply the witness. Only the report runner is replayed.
    hooks = SimpleNamespace(hook_add=lambda *args, **kwargs: None)
    machines = iter([SimpleNamespace(uc=hooks, run=lambda *args: a),
                     SimpleNamespace(uc=hooks, _escape=None, run=lambda *args: b)])
    monkeypatch.setattr(composition.emu, 'Machine', lambda *args, **kwargs: next(machines))
    monkeypatch.setattr(composition.difftest, 'make_case', lambda *args: None)
    options = difftest.Options(**{**difftest.DEFAULTS, 'cases': 1, 'discovery': 0}, seed=va, hints={})
    report = composition._run(va, {va: lifted}, compiled, {va: compiled.entry}, options)
    assert report['mismatches'] == []
    assert report['mapping_faults'] == [{'case': 0, 'proof': b.fault_mapping, 'relation_used': True}]
    assert report['outcomes'] == {'fault': 1}
    assert report['status'] == 'fail'  # A matching fault never supplies a successful return.
    assert b.detail[0][0] == 22

