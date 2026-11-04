import random
from types import SimpleNamespace
import pytest
from cmre import difftest, emu, image, imports


def test_capacity_is_opt_in_and_default_stream_unchanged():
    opts = difftest.Options(2, 100, 0, 2, .6, 1, {})
    old = difftest.make_case(random.Random(3), opts, [], difftest.arena_templates())
    opts.hints = {'arena_size': 0x260000}
    large = difftest.make_case(random.Random(3), opts, [], difftest.arena_templates())
    assert large.arena[:emu.ARENA_SIZE] == old.arena
    assert large.regs == old.regs and large.stack == old.stack
    assert len(large.arena) == 0x260000
    assert large.arena[emu.ARENA_SIZE:] == bytes(0x220000)


@pytest.mark.parametrize('size', [True, 0, 0x40001, 0x261000])
def test_capacity_rejects_invalid_sizes(size):
    with pytest.raises(ValueError):
        difftest.validate_hints({'arena_size': size})


def test_transcript_heap_and_outputs_are_explicit_and_checked():
    p = emu.ARENA + 0x100
    steps = [
        {'name': 'KERNEL32.dll!HeapAlloc', 'args': [7, 0, 32], 'result': p, 'heap': 'alloc'},
        {'name': 'KERNEL32.dll!HeapSize', 'args': [7, 0, p], 'result': 32, 'heap': 'size'},
        {'name': 'KERNEL32.dll!ReadFile', 'args': [4, p, 3, p+8, 0], 'result': 1,
         'writes': [{'arg': 1, 'data': '616263'}, {'arg': 3, 'data': '03000000'}]},
        {'name': 'KERNEL32.dll!HeapFree', 'args': [7, 0, p], 'result': 1, 'heap': 'free'},
    ]
    runtime = imports.Transcript(steps, emu.ARENA, emu.ARENA_SIZE)
    writes = []
    for step in steps:
        assert runtime.call(step['name'], tuple(step['args']), lambda a,b: writes.append((a,b))) == step['result']
    assert writes == [(p,b'abc'), (p+8,b'\x03\0\0\0')]
    with pytest.raises(ValueError, match='exhausted'):
        runtime.call(steps[0]['name'], tuple(steps[0]['args']), lambda a,b: None)


@pytest.mark.parametrize('change', ['size', 'args', 'bounds', 'overlap'])
def test_transcript_near_misses_fail(change):
    p = emu.ARENA+16
    alloc = {'name':'KERNEL32.dll!HeapAlloc','args':[7,0,32],'result':p,'heap':'alloc'}
    runtime = imports.Transcript([alloc, dict(alloc)], emu.ARENA, emu.ARENA_SIZE)
    runtime.call(alloc['name'], tuple(alloc['args']), lambda a,b:None)
    if change == 'overlap':
        step = alloc
    elif change == 'size':
        step = {'name':'KERNEL32.dll!HeapSize','args':[7,0,p],'result':31,'heap':'size'}
    elif change == 'args':
        with pytest.raises(ValueError, match='arguments'):
            runtime.call(alloc['name'], (8,0,32), lambda a,b:None)
        return
    else:
        step = {'name':'KERNEL32.dll!ReadFile','args':[1,emu.ARENA+emu.ARENA_SIZE-1,2,0,0],
                'result':1,'writes':[{'arg':1,'data':'0102'}]}
    runtime.steps = [step]
    runtime.index = 0
    with pytest.raises(ValueError):
        runtime.call(step['name'], tuple(step['args']), lambda a,b:None)


def test_machine_large_arena_reset_and_guest_observation():
    # Authored instructions, no supplied executable required.
    code = bytes.fromhex('8b01c3')
    img = SimpleNamespace(base=0x400000, headers=bytes(0x1000), imports={},
        sections=[image.Section('.text',0x401000,0x1000,code,0x60000000)])
    machine = emu.Machine(img, arena_size=0x260000)
    arena = bytearray(0x260000)
    arena[-4:] = b'abcd'
    case = emu.Case({'ecx':emu.ARENA+len(arena)-4,'eflags':0x202}, [], bytes(arena))
    result = machine.run(0x401000,case,20)
    assert result.outcome == 'return' and result.regs[0] == 0x64636261
    with pytest.raises(ValueError, match='arena'):
        machine.run(0x401000,emu.Case({},[],bytes(emu.ARENA_SIZE)),20)


def test_referenced_callback_relative_bytes_fail_closed():
    from cmre import lift
    rec = lift.Insn(0x401000, b'', 'push', '0x401020', substitutions={0x401020:'v_00401020'})
    problems = []
    lift._check_referenced_relative_data(lift._cs(), {rec.va:rec},
        [lift.DataRun(0x401020, bytes.fromhex('e94bfd0800'))], problems)
    assert len(problems) == 1 and 'PC-relative' in problems[0]
    problems = []
    lift._check_referenced_relative_data(lift._cs(), {rec.va:rec},
        [lift.DataRun(0x401020, b'hello\0'), lift.DataRun(0x401030, bytes.fromhex('e94bfd0800'))], problems)
    assert problems == []


def test_equal_transcript_rejections_cannot_pass_comparison():
    result = emu.Result('error', ('import transcript', 'arguments differ', 0x401000), (0,)*9, [], {}, set())
    normalized = difftest._normalize(result,None,'exact')
    verdict, difference = difftest._classify(normalized, normalized, result,None,(0x401000,0x401010),())
    assert verdict == 'mismatch' and difference['field'] == 'import-transcript'


def test_fixture_extension_writes_and_script_are_case_local():
    fixture = {'regs':{n:0 for n in emu.REG_NAMES}, 'stack':[], 'globals':{},
        'arena_template':0,'arena_writes':{'0x250000':0x12345678},'import_seed':0,
        'import_transcript':[]}
    difftest.validate_hints({'arg_count':0,'arena_size':0x260000,'fixtures':[fixture]})
    with pytest.raises(ValueError, match='in-range'):
        difftest.validate_hints({'arg_count':0,'fixtures':[fixture]})


def test_guest_import_outputs_reset_and_rejections_remain_failures():
    import struct
    p = emu.ARENA + 0x250000
    name = 'KERNEL32.dll!GetLogicalDriveStringsA'
    # push buffer; push count; call [IAT]; mov eax,[buffer]; ret
    code = b'\x68'+struct.pack('<I',p)+b'\x6a\x04\xff\x15\x00\x20\x40\x00\xa1'+struct.pack('<I',p)+b'\xc3'
    img = SimpleNamespace(base=0x400000, headers=bytes(0x1000), imports={0x402000:name},
        sections=[image.Section('.text',0x401000,0x1000,code,0x60000000),
                  image.Section('.data',0x402000,0x1000,bytes(0x1000),0xc0000000)])
    machine = emu.Machine(img, arena_size=0x260000)
    script = [{'name':name,'args':[4,p],'result':3,'writes':[{'arg':1,'data':'433a0000'}]}]
    case = emu.Case({'eflags':0x202}, [], bytes(0x260000), import_transcript=script)
    first = machine.run(0x401000,case,50)
    assert first.outcome == 'return' and first.regs[0] == 0x3a43
    assert first.memory[p & ~0xfff][:4] == b'C:\0\0'
    assert ('w',p,1,ord('C')) in first.events
    case.import_transcript = []
    rejected = machine.run(0x401000,case,50)
    assert rejected.outcome == 'error' and rejected.detail[0] == 'import transcript'
    case.import_transcript = script
    assert machine.run(0x401000,case,50).memory == first.memory


def test_large_struct_pointer_reserves_full_object():
    hints = {'arena_size':0x260000,'struct_size':0x250000,'ecx':'struct'}
    difftest.validate_hints(hints)
    opts = difftest.Options(2,100,0,0,.6,1,hints)
    for seed in range(10):
        case = difftest.make_case(random.Random(seed),opts,[],difftest.arena_templates())
        assert emu.ARENA <= case.regs['ecx'] <= emu.ARENA+0x10000


@pytest.mark.parametrize('policy,expected', [
    ('v1','8368513a64672e5c1f3691f8a341579ebe1e5f5ef88f25fe61c2cbf3473401f6'),
    ('v2','5fdd74fb7406bea631bf2753d0747bbb7dcb6253c1bf5291500365f4f183647b')])
def test_default_corpus_matches_origin_main_golden(policy, expected):
    # Generated from origin/main 445032b before this release, 20 seeds each.
    import hashlib
    hashes = []
    opts = difftest.Options(2,100,0,10,.6,1,{},inputs=policy)
    for seed in range(20):
        case = difftest.make_case(random.Random(seed),opts,[],difftest.arena_templates())
        view = (case.regs,case.stack,case.arena,case.globals,case.import_seed,case.fpcw)
        hashes.append(hashlib.sha256(repr(view).encode()).hexdigest())
    assert hashlib.sha256(''.join(hashes).encode()).hexdigest() == expected


def test_recovered_transcript_rejection_normalizes_with_real_address_map():
    amap = emu.AddressMap({'v_00401000':0x6e5d1000},{},0x6e5d1000,0x6e5d2000)
    rejected = emu.Result('error',('import transcript','arguments differ',0x6e5d1000),
                          (0,)*9,[],{},set())
    original = emu.Result('return',(),(0,)*9,[],{},set())
    for profile in ('exact','semantic','abi'):
        na = difftest._normalize(original,None,profile)
        nb = difftest._normalize(rejected,amap,profile)
        verdict,_ = difftest._classify(na,nb,original,amap,(0x401000,0x401010),())
        assert verdict == 'mismatch'


def test_heap_script_cannot_skip_coherence_annotation():
    with pytest.raises(ValueError, match='coherence'):
        imports.Transcript([{'name':'KERNEL32.dll!HeapSize','args':[1,0,emu.ARENA],
                             'result':1}],emu.ARENA,emu.ARENA_SIZE)


@pytest.mark.parametrize('name', ['KERNEL32.dll!ExitProcess', 'KERNEL32.dll!RaiseException', None])
@pytest.mark.parametrize('steps', [[], [{'name':'KERNEL32.dll!GetDriveTypeA','args':[emu.ARENA],'result':3}]])
def test_terminal_or_unknown_import_retains_baseline_but_rejects_active_transcript(name, steps):
    import struct
    # Authored jump directly into a known or unknown trap, without a CRT model.
    target = emu.TRAP if name else emu.TRAP + 4
    code = b'\xe9' + struct.pack('<I',(target - 0x401005) & 0xffffffff)
    slots = {0x402000:name} if name else {}
    img = SimpleNamespace(base=0x400000,headers=bytes(0x1000),imports=slots,
        sections=[image.Section('.text',0x401000,0x1000,code,0x60000000),
                  image.Section('.data',0x402000,0x1000,bytes(0x1000),0xc0000000)])
    machine = emu.Machine(img)
    case = emu.Case({'eflags':0x202},[0]*4,bytes(emu.ARENA_SIZE))
    baseline = machine.run(0x401000,case,20)
    assert baseline.outcome == 'import'
    assert baseline.detail == (name or hex(target),)
    # Even an already-consumed/empty transcript must not allow a terminal escape.
    case.import_transcript = steps
    first = machine.run(0x401000,case,20)
    second = machine.run(0x401000,case,20)
    assert first.outcome == second.outcome == 'error'
    assert first.detail[0] == 'import transcript'
    for profile in ('exact','abi','semantic'):
        na = difftest._normalize(first,None,profile)
        nb = difftest._normalize(second,None,profile)
        verdict,difference = difftest._classify(na,nb,first,None,(0x401000,0x401005),())
        assert verdict == 'mismatch' and difference['field'] == 'import-transcript'
        returned = emu.Result('return',(),first.regs,[],{},set())
        good = difftest._normalize(returned,None,profile)
        verdict,difference = difftest._classify(good,nb,returned,None,(0x401000,0x401005),())
        assert verdict == 'mismatch' and difference['field'] == 'import-transcript'


@pytest.mark.parametrize('name', [
    'KERNEL32.dll!HeapReAlloc', 'KERNEL32.dll!HeapDestroy',
    'USER32.dll!CreateWindowExA', 'USER32.dll!DispatchMessageA',
    'WINSPOOL.DRV!EnumPrintersA', 'KERNEL32.dll!ExitProcess'])
def test_transcript_rejects_imports_without_explicit_supported_contract(name):
    with pytest.raises(ValueError,match='unsupported transcript import'):
        imports.Transcript([{'name':name,'args':[0]*imports.STDCALL_ARGS[name],
                             'result':0}],emu.ARENA,emu.ARENA_SIZE)
