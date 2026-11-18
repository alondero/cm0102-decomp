"""C must distinguish retained inline data from reads of its own instructions."""
from types import SimpleNamespace
import pytest
from cmre import difftest,emu,env,recover

VA=0x00401000
REGION=(VA,VA+0x100)
TABLE=VA+0x80
RAW=VA+0xA0

@pytest.mark.parametrize('profile',['semantic','abi'])
@pytest.mark.parametrize('read_address,changed,expected,excluded',[
 (TABLE,True,'fail',0),
 (RAW,True,'fail',0),
 (TABLE,False,'pass',0),
 (TABLE-1,True,'pass',1),
 (RAW+4,True,'pass',1),
])
def test_c_forwards_known_data_spans_without_hiding_changed_return(tmp_path,monkeypatch,profile,read_address,changed,expected,excluded):
 source=tmp_path/'fn_00401000.c';source.write_text('unsigned int cm_fn_00401000(void) { return 41; }\n')
 monkeypatch.setattr(env,'RUNS',tmp_path/'runs')
 monkeypatch.setattr(recover,'execution_identity',lambda:{'synthetic':'unit contract, not game evidence'})
 monkeypatch.setattr(env,'toolchain_identity',lambda:{})
 monkeypatch.setattr(recover,'c_dependencies',lambda source:{'files':{},'preprocessed':{}})
 monkeypatch.setattr(recover,'_semantic_contract',lambda va,text:('int',[]))
 monkeypatch.setattr(recover,'jump_contract',lambda va:None)
 monkeypatch.setattr(recover,'x87_contract',lambda va:None)
 monkeypatch.setattr(recover,'_indirect_calls',lambda lifted:{})
 monkeypatch.setattr(recover,'c_inputs',lambda va:'v2')
 lifted=SimpleNamespace(va=VA,region_end=REGION[1],name='inline-data-test',insns={VA:None},data=[SimpleNamespace(va=RAW,raw=b'abcd')],tables={TABLE:SimpleNamespace(va=TABLE,targets=[VA,VA+1])})
 monkeypatch.setattr(recover.lift,'discover',lambda va:lifted)
 monkeypatch.setattr(recover.build,'build',lambda *args:SimpleNamespace(symbols={},image=SimpleNamespace(return_bindings=(),jump_bindings=(),x87_bindings=())))
 captured=[]
 def run(va,region,insns,built,call_returns,opts,profile,data_spans=(),**kwargs):
  captured.append(list(data_spans))
  a=emu.Result('return',(),(41,)+(0,)*8,[('r',read_address,1,7)],{},set())
  b=emu.Result('return',(),((42 if changed else 41),)+(0,)*8,[],{},set())
  na=difftest._normalize(a,None,profile)
  nb=difftest._normalize(b,None,profile)
  verdict,difference=difftest._classify(na,nb,a,None,region,data_spans)
  return {'mismatches':[difference] if verdict=='mismatch' else [],'code_as_data_cases':int(verdict=='code-as-data'),'coverage':1.0,'inconclusive':0}
 monkeypatch.setattr(difftest,'run',run)
 report=recover.test_c(VA,{'cases':100},fresh=True,source=source,profile=profile)
 assert report['status']==expected
 assert all(b['difftest']['code_as_data_cases']==excluded for b in report['builds'].values())
 assert captured==[[(RAW,RAW+4),(TABLE,TABLE+8)]]*2
