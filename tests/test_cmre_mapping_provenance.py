"""Added PE page provenance never changes native fault execution or detail."""
from pathlib import Path
from types import SimpleNamespace

import pytest
import unicorn

from cmre import emu, env, image

EXTRA = 0x6E5D2000
STORE = 0x00412EF0  # pinned mov dword ptr [ecx], immediate; ret
LOAD = 0x00450A70   # pinned mov ax, word ptr [ecx]; ret
READONLY = 0x40000000
READWRITE = 0xC0000000


def recovered(*sections):
    return SimpleNamespace(sections=list(sections), return_bindings=(),
                           jump_bindings=(), x87_bindings=())


def section(start=EXTRA, size=0x200, flags=READONLY):
    return image.Section('.rdata', start, size, b'\xa5' * size, flags)


def case(pointer):
    return emu.Case({'ecx': pointer, 'eax': 0x12345678, 'eflags': 0x202},
                    [0] * 4, bytes(emu.ARENA_SIZE))


def test_imports_use_the_current_workspace():
    assert env.ROOT == Path(__file__).resolve().parents[1]


@pytest.mark.parametrize('offset', [0x10, 0xDFC])
def test_extra_readonly_pe_page_records_candidate_without_changing_native_state(offset):
    pointer = EXTRA + offset
    original = emu.Machine(image.primary())
    compiled = emu.Machine(image.primary(), recovered(section()))
    expected = original.run(STORE, case(pointer), 10)
    before = bytes(compiled.uc.mem_read(EXTRA, emu.PAGE))
    actual = compiled.run(STORE, case(pointer), 10)
    assert expected.detail == ((unicorn.UC_MEM_WRITE_UNMAPPED, pointer, 4), STORE)
    assert actual.detail == ((unicorn.UC_MEM_WRITE_PROT, pointer, 4), STORE)
    assert compiled.fault == (unicorn.UC_MEM_WRITE_PROT, pointer, 4)
    assert actual.regs == expected.regs
    assert actual.events == expected.events
    assert actual.memory == expected.memory == {}
    assert bytes(compiled.uc.mem_read(EXTRA, emu.PAGE)) == before
    assert actual.fault_mapping['raw_access'] == unicorn.UC_MEM_WRITE_PROT
    assert actual.fault_mapping['projected_access'] == unicorn.UC_MEM_WRITE_UNMAPPED
    assert actual.fault_mapping['address'] == pointer
    assert actual.fault_mapping['size'] == 4
    assert actual.fault_mapping['applied'] is False
    assert [page['page'] for page in actual.fault_mapping['pages']] == [EXTRA]
    assert actual.fault_mapping['pages'][0]['mapped_end'] == EXTRA + emu.PAGE
    assert actual.fault_mapping['pages'][0]['section_end'] == EXTRA + 0x200


def test_cross_page_candidate_requires_every_page_to_be_added_and_readonly():
    compiled = emu.Machine(image.primary(), recovered(section(size=2 * emu.PAGE)))
    pointer = EXTRA + emu.PAGE - 2
    expected = emu.Machine(image.primary()).run(STORE, case(pointer), 10)
    before = bytes(compiled.uc.mem_read(EXTRA, 2 * emu.PAGE))
    actual = compiled.run(STORE, case(pointer), 10)
    assert actual.detail == ((unicorn.UC_MEM_WRITE_PROT, pointer, 4), STORE)
    assert [page['page'] for page in actual.fault_mapping['pages']] == [EXTRA, EXTRA + emu.PAGE]
    assert actual.regs == expected.regs
    assert actual.events == expected.events
    assert actual.memory == expected.memory == {}
    assert bytes(compiled.uc.mem_read(EXTRA, 2 * emu.PAGE)) == before


@pytest.mark.parametrize('second', ['writable', 'unmapped', 'common'])
def test_mixed_cross_page_span_keeps_native_protection_fault(second):
    sections = [section(size=emu.PAGE)]
    if second == 'writable':
        sections.append(section(EXTRA + emu.PAGE, emu.PAGE, READWRITE))
    compiled = emu.Machine(image.primary(), recovered(*sections))
    if second == 'common':
        compiled._map(EXTRA + emu.PAGE, bytes(emu.PAGE), perms=unicorn.UC_PROT_READ)
    actual = compiled.run(STORE, case(EXTRA + emu.PAGE - 2), 10)
    assert actual.detail[0][0] == unicorn.UC_MEM_WRITE_PROT
    assert actual.fault_mapping is None


@pytest.mark.parametrize('pointer', [0x00400000, 0x00401000, emu.STACK + 0x100,
                                    emu.TEB, emu.GDT, emu.TRAP, emu.STOP])
def test_original_and_common_pages_never_project_a_true_protection_fault(pointer):
    compiled = emu.Machine(image.primary(), recovered(section()))
    if pointer >= emu.STACK:
        compiled.uc.mem_protect(pointer & ~(emu.PAGE - 1), emu.PAGE, unicorn.UC_PROT_READ)
    actual = compiled.run(STORE, case(pointer), 10)
    assert actual.detail == ((unicorn.UC_MEM_WRITE_PROT, pointer, 4), STORE)
    assert actual.fault_mapping is None


def test_initially_writable_added_page_is_not_an_environment_projection():
    compiled = emu.Machine(image.primary(), recovered(section(flags=READWRITE)))
    compiled.uc.mem_protect(EXTRA, emu.PAGE, unicorn.UC_PROT_READ)
    actual = compiled.run(STORE, case(EXTRA + 16), 10)
    assert actual.detail == ((unicorn.UC_MEM_WRITE_PROT, EXTRA + 16, 4), STORE)
    assert actual.fault_mapping is None


@pytest.mark.parametrize('entry,permissions,access', [
    (LOAD, unicorn.UC_PROT_NONE, unicorn.UC_MEM_READ_PROT),
    (EXTRA, unicorn.UC_PROT_READ, unicorn.UC_MEM_FETCH_PROT),
])
def test_read_and_execute_permission_faults_keep_native_class(entry, permissions, access):
    compiled = emu.Machine(image.primary(), recovered(section()))
    compiled.uc.mem_protect(EXTRA, emu.PAGE, permissions)
    actual = compiled.run(entry, case(EXTRA), 10)
    assert actual.detail[0][0] == access
    assert actual.fault_mapping is None


def test_readable_extra_page_collision_remains_explicitly_unsolved():
    original = emu.Machine(image.primary())
    compiled = emu.Machine(image.primary(), recovered(section()))
    expected = original.run(LOAD, case(EXTRA), 10)
    actual = compiled.run(LOAD, case(EXTRA), 10)
    assert expected.outcome == 'fault'
    assert actual.outcome == 'return'
    assert actual.fault_mapping is None


def test_fault_proof_resets_between_cases():
    compiled = emu.Machine(image.primary(), recovered(section()))
    first = compiled.run(STORE, case(EXTRA + 16), 10)
    assert first.fault_mapping is not None
    second = compiled.run(0x004671B0, case(1), 10)
    assert second.outcome == 'return'
    assert second.fault_mapping is None


def test_previous_result_proof_cannot_mutate_the_next_case_mapping_provenance():
    compiled = emu.Machine(image.primary(), recovered(section()))
    first = compiled.run(STORE, case(EXTRA + 16), 10)
    first.fault_mapping['pages'][0]['recovered_sections'][0]['writable'] = True
    second = compiled.run(STORE, case(EXTRA + 16), 10)
    assert second.fault_mapping['pages'][0]['recovered_sections'][0]['writable'] is False


@pytest.mark.parametrize('start', [0x00401000, emu.STACK])
def test_added_pe_overlap_with_original_or_common_map_is_rejected(start):
    with pytest.raises(unicorn.UcError):
        emu.Machine(image.primary(), recovered(section(start=start)))


@pytest.mark.parametrize('address,size', [(EXTRA, 0), (EXTRA, -1),
                                         (0xFFFFFFFE, 4), (-1, 4)])
def test_invalid_or_wrapping_span_has_no_projection_proof(address, size):
    compiled = emu.Machine(image.primary(), recovered(section()))
    assert compiled._fault_mapping_proof(unicorn.UC_MEM_WRITE_PROT, address, size) is None
