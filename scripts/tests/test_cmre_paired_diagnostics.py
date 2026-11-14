"""Diagnostic reporting must expose later differences without changing gates."""
import gzip
import json
from types import SimpleNamespace

import pytest

from scripts import cmre_paired_diagnostics as paired


def test_all_components_survive_an_earlier_outcome_difference():
    a = ('return', (), (1,), [('c', 'A', 0, ())], {0x1000:b'abc'})
    b = ('import', ('B',), (2,), [('c', 'B', 0, ())], {0x1000:b'abd'})
    before = repr((a,b))
    result = paired.components(a,b)
    assert {k:v['field'] for k,v in result.items()} == {k:k for k in ('outcome','detail','regs','events','memory')}
    assert repr((a,b)) == before


def test_equal_components_and_a_memory_near_miss():
    a = ('return', (), (1,), [], {0x1000:b'abc'})
    assert all(not v for v in paired.components(a,a).values())
    b = (*a[:4], {0x1000:b'abd'})
    assert {k for k,v in paired.components(a,b).items() if v} == {'memory'}


def test_fault_detail_probe_retains_strict_fault_policy():
    a = ('fault', ((19,0x1000,4),0x400000), (), [], {})
    b = ('fault', ((19,0x1004,4),0x400000), (), [], {})
    assert paired.components(a,b)['detail']['field'] == 'detail'


def test_retains_case_initial_bytes_separately_from_machine_pristine():
    from cmre import emu
    r = emu.Result('return',(),(),[],{4096:b'after'},set(),{4096:b'pristine'})
    machine = SimpleNamespace(_initial_page=lambda p:b'case seeded')
    row = paired.raw_result(r,machine,b'')
    assert row['machine_pristine_changed_pages'] == {4096:b'pristine'}
    assert row['case_initial_changed_pages'] == {4096:b'case seeded'}


def test_stack_initial_bytes_come_from_entry_observation():
    from cmre import emu
    r = emu.Result('return',(),(),[],{emu.STACK_TOP:b'after'},set(),{emu.STACK_TOP:b'pristine'})
    machine = SimpleNamespace(_initial_page=lambda p:pytest.fail('Stack initial helper omits argument writes'))
    stack = bytearray(emu.STACK_SIZE)
    offset = emu.STACK_TOP-emu.STACK
    stack[offset:offset+8] = b'ARGUMENT'
    row = paired.raw_result(r,machine,bytes(stack))
    assert row['case_initial_changed_pages'][emu.STACK_TOP][:8] == b'ARGUMENT'


def test_recursive_frames_sharing_a_return_pc_unwind_by_physical_stack():
    from cmre import emu
    observer = object.__new__(paired.Observer)
    observer.enabled = True
    observer.entry_stack = b'already captured'
    observer.machine = SimpleNamespace(events=[])
    observer.instructions,observer.arrivals,observer.returns = [],[],[]
    observer.frames = [
        {'arrival':0,'return_pc':0x401000,'entry_esp':0x2000},
        {'arrival':1,'return_pc':0x401000,'entry_esp':0x1ff0},
    ]
    observer.entries = {}
    observer.md = SimpleNamespace(disasm=lambda *a,**k:iter([SimpleNamespace(mnemonic='nop',op_str='')]))
    current_esp = [0x2000]
    uc = SimpleNamespace(reg_read=lambda r:current_esp[0] if r==emu.REGS[4] else 0,
                         mem_read=lambda a,n:b'\x90'*n)
    observer.code(uc,0x401000,1,None)
    assert [r['arrival'] for r in observer.returns] == [1]
    assert [f['arrival'] for f in observer.frames] == [0]
    current_esp[0] = 0x2004
    observer.code(uc,0x401000,1,None)
    assert [r['arrival'] for r in observer.returns] == [1,0]
    assert observer.frames == []


def test_compressed_artifacts_roundtrip_and_refuse_overwrite(tmp_path):
    path = tmp_path/'input.json.gz'
    pin = paired.save(path, {'bytes':b'\0\xff'})
    assert json.loads(gzip.decompress(path.read_bytes()))['bytes']['hex'] == '00ff'
    assert len(pin['sha256']) == 64
    with pytest.raises(FileExistsError):
        paired.save(path, {})
