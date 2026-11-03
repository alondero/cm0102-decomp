"""Regression proposal for composition v1/v2 original-read discovery."""
from types import SimpleNamespace
from cmre import composition, difftest, emu, image


def test_composition_discovery_respects_policy(monkeypatch):
    va=0x005cc7d0
    owner_addr=0x00ae2a38
    outside_addr=0x00b68b00
    assert image.primary().section_at(owner_addr).writable
    assert image.primary().section_at(outside_addr).writable
    region=(va,va+0x57)
    inputs_seen=[]
    class FakeUC:
        def hook_add(self,*args,**kwargs): pass
    class FakeMachine:
        def __init__(self,*args,**kwargs):
            self.uc=FakeUC(); self.attribute_reads=False; self._escape=lambda *args: None
        def run(self,entry,case,budget):
            return SimpleNamespace(outcome='return',detail=(),regs=(0,)*9,
                events=[('r',owner_addr,4,0),('r',outside_addr,4,0)],
                read_sites={0:va,1:0x0082ff40} if self.attribute_reads else {},
                covered={va},memory={},pristine={},fault_mapping=None)
    def make_case(rng,opts,inputs,templates):
        inputs_seen.append(tuple(inputs)); return None
    monkeypatch.setattr(emu,'Machine',FakeMachine)
    monkeypatch.setattr(difftest,'make_case',make_case)
    monkeypatch.setattr(difftest,'arena_templates',lambda: [])
    monkeypatch.setattr(difftest,'_normalize',lambda *args,**kwargs: ('return',(),(),[],{}))
    monkeypatch.setattr(difftest,'_digest_view',lambda x: '')
    monkeypatch.setattr(difftest,'_diff',lambda *args,**kwargs: {})
    monkeypatch.setattr(composition,'_instruction_access_index',lambda insns: lambda a,s: False)
    rec=SimpleNamespace(va=va,region_end=region[1],insns={va:object()})
    compiled=SimpleNamespace(image=SimpleNamespace(text=SimpleNamespace(va=0x6e5d1000,end=0x6e5d2000)),symbols={})
    for policy,expected in [('v1',(owner_addr,outside_addr)),('v2',(owner_addr,))]:
        inputs_seen.clear()
        opts=SimpleNamespace(seed=1,cases=2,budget=100,discovery=1,min_coverage=0.6,inputs=policy)
        composition._run(va,{va:rec},compiled,{va:0x6e5d1000},opts)
        assert inputs_seen==[(),expected]

