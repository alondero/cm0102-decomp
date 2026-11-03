"""Reproduce the two submitted adapters' supplemental raw VM witnesses.

Use --output with a new directory. This is not an acceptance/review receipt.
"""
import argparse
import copy
import json
import random
from pathlib import Path
import hashlib
import sys
import unicorn

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from cmre import build, difftest, emu, env, image, lift, recover, tasks
from scripts import cmre_adapter_witness_checks as adapter_witness

TASKS = {
    0x0040a7d0: ('c-0040a7d0-1cbd4513f10c385d', 0x0040a801, 0x0040a7f5),
    0x00521e60: ('c-00521e60-117b723a1e163f6d', 0x00521eb0, 0x00521e83),
}
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--output', type=Path, required=True)
args = parser.parse_args()
OUT = adapter_witness.admit_output(args.output, env.ANALYSIS / 'cmre/diagnostics')
OUT.mkdir(parents=True, exist_ok=False)
identity = recover.execution_identity()
driver_hash = env.digest(Path(__file__))
checker_hash = env.digest(Path(adapter_witness.__file__))
validation_code = adapter_witness.validation_identity()
inputs = {}
SOURCES = {}
for va, (task, _, _) in TASKS.items():
    taskdir = env.ROOT / 'CM0102/Analysis/cmre/tasks' / task
    taskpath, reportpath = taskdir / 'task.json', taskdir / 'report-1.json'
    record = json.loads(taskpath.read_text(encoding='utf-8'))
    report = json.loads(reportpath.read_text(encoding='utf-8'))
    tasks.validate_current(record)
    source = env.ROOT / 'recovery/c/tasks' / task / f'fn_{va:08x}.c'
    digest = env.digest(source)
    adapter_witness.check_submission(task, record, report, digest, env.digest(reportpath), identity)
    SOURCES[va] = source
    inputs[task] = {'task': env.digest(taskpath), 'report': env.digest(reportpath), 'source': digest}

snapshot_paths = [Path(__file__), Path(adapter_witness.__file__),
                  env.ROOT / 'tests/test_cmre_adapter_witness.py',
                  env.ROOT / 'cmre/tasks.py', env.ROOT / 'cmre/ledger.py']
for va, (task, _, _) in TASKS.items():
    directory = env.ROOT / 'CM0102/Analysis/cmre/tasks' / task
    snapshot_paths.extend([directory / 'task.json', directory / 'report-1.json', SOURCES[va]])
snapshot_pins = {env.rel(path): env.digest(path) for path in snapshot_paths}
if (snapshot_pins['scripts/cmre_adapter_witness.py'] != driver_hash
        or snapshot_pins['scripts/cmre_adapter_witness_checks.py'] != checker_hash
        or any(snapshot_pins[name] != digest for name, digest in validation_code.items())):
    raise ValueError('witness code changed before snapshot')
for va, (task, _, _) in TASKS.items():
    expected = inputs[task]
    for kind, path in [('task', env.ROOT / 'CM0102/Analysis/cmre/tasks' / task / 'task.json'),
                       ('report', env.ROOT / 'CM0102/Analysis/cmre/tasks' / task / 'report-1.json'),
                       ('source', SOURCES[va])]:
        if snapshot_pins[env.rel(path)] != expected[kind]:
            raise ValueError('task inputs changed before snapshot')
adapter_witness.snapshot_inputs(OUT / 'input-snapshot', env.ROOT, snapshot_pins)


def corpus_case(va, target, *, expected_base='return'):
    opts = difftest.Options.for_function(va, tier='c')
    opts.inputs = recover.c_inputs(va)
    lifted = lift.discover(va)
    region = (va, lifted.region_end)
    vm = emu.Machine(image.primary(), watch=region)
    difftest._stop_indirect(vm, None, region, None, recover._indirect_calls(lifted))
    rng = random.Random(opts.seed)
    templates = difftest.arena_templates()
    inputs = set()
    for index in range(target + 1):
        case = difftest.make_case(rng, opts, sorted(inputs), templates)
        own = difftest._discovery(vm, opts, index, region)
        result = vm.run(va, case, opts.budget)
        if index < opts.discovery:
            inputs |= difftest._global_reads(result, image.primary(), own)
    if expected_base == 'return':
        adapter_witness.require_returning_seed(result, va, target)
    elif result.outcome != expected_base:
        raise ValueError(f'corpus seed {va:#x} case {target} changed outcome: {result.outcome}')
    return case, opts, lifted


def observe(vm, entry, case, budget, mapping=None, protect_at=None):
    if protect_at is not None:
        protected = []
        def protect(uc, address, size, user):
            page = uc.reg_read(emu.R.UC_X86_REG_ESP) & ~(emu.PAGE - 1)
            protected.append(page)
            uc.mem_protect(page, emu.PAGE, unicorn.UC_PROT_NONE)
        def restore(uc, access, address, size, value, user):
            for page in protected:
                uc.mem_protect(page, emu.PAGE, unicorn.UC_PROT_READ | unicorn.UC_PROT_WRITE)
            return False
        vm.uc.hook_add(unicorn.UC_HOOK_CODE, protect, begin=protect_at, end=protect_at)
        vm.uc.hook_add(unicorn.UC_HOOK_MEM_INVALID, restore)
    result = vm.run(entry, case, budget)
    detail = list(result.detail)
    if mapping is not None and result.outcome in ('fault', 'budget', 'error'):
        detail = adapter_witness.normalize_fault(detail, mapping, vm.recovered.text.va, vm.recovered.text.end)
    if protect_at is not None and len(protected) != 1:
        raise ValueError('protection hook did not run exactly once')
    return {'outcome': result.outcome, 'detail': detail, 'regs': result.regs,
            'events': result.events,
            'stack': bytes(vm.uc.mem_read(emu.STACK, emu.STACK_SIZE)).hex(),
            'arena': bytes(vm.uc.mem_read(emu.ARENA, emu.ARENA_SIZE)).hex(),
            'comparable_dirty_pages': adapter_witness.comparable_dirty_pages(vm, result)}


rows = []
for va, index in [(0x0040a7d0, 50), (0x00521e60, 3)]:
    base, opts, lifted = corpus_case(va, index)
    probes = []
    if va == 0x0040a7d0:
        for offset in (-16, -8, -4, 0, 4, 8):
            case = copy.deepcopy(base)
            case.regs['ecx'] = emu.STACK_TOP + offset
            probes.append((f'ecx-stack-{offset:+d}', case, None))
        other, _, _ = corpus_case(va, 35, expected_base='fault')
        other.regs['ecx'] = emu.STACK_TOP - 4
        probes.append(('ecx-stack--4-returning-seed-35', other, None))
        for site in (0x0040a7fc, 0x0040a7fd, 0x0040a7fe):
            probes.append((f'protected-stack-at-{site:08x}', copy.deepcopy(base), site))
    else:
        for offset in (-12, -8, -4, 0, 4, 8, 12):
            case = copy.deepcopy(base)
            signed = (case.stack[1] & 0xffff)
            signed = signed - 0x10000 if signed & 0x8000 else signed
            case.stack[0] = (emu.STACK_TOP + offset - signed * 104) & 0xffffffff
            probes.append((f'output-stack-{offset:+d}', case, None))
        case = copy.deepcopy(base)
        signed = case.stack[1] & 0xffff
        signed = signed - 0x10000 if signed & 0x8000 else signed
        case.stack[0] = (emu.ARENA + emu.ARENA_SIZE - 2 - signed * 104) & 0xffffffff
        probes.append(('output-arena-end-minus-2', case, None))
        output_alias = next(case for name, case, _ in probes if name == 'output-stack--12')
        for site in (0x00521ea5, 0x00521eaf):
            probes.append((f'protected-stack-at-{site:08x}', copy.deepcopy(output_alias), site))
    for opt in ('-O0', '-O2'):
        work = OUT / f'{va:08x}' / opt[1:]
        work.mkdir(parents=True, exist_ok=True)
        compiled = build.build(SOURCES[va], f'cm_fn_{va:08x}', work, opt)
        mapping, instructions = adapter_witness.linked_instruction_map(va, TASKS[va][1], TASKS[va][2], compiled)
        (work / 'instructions.json').write_text(json.dumps(instructions, indent=2), encoding='utf-8')
        for name, case, protect_at in probes:
            original = emu.Machine(image.primary())
            candidate = emu.Machine(image.primary(), compiled.image, watch=(va, lifted.region_end), escape=True)
            a = observe(original, va, case, opts.budget, protect_at=protect_at)
            matching_sites = [native for native, original in mapping.items() if original == protect_at]
            if protect_at is not None and len(matching_sites) != 1:
                raise ValueError('protected site is not one verified linked instruction')
            compiled_site = matching_sites[0] if protect_at is not None else None
            b = observe(candidate, compiled.entry, case, opts.budget, mapping, compiled_site)
            row = {'va': hex(va), 'opt': opt, 'name': name,
                   'source_sha256': env.digest(SOURCES[va]),
                   'case': {'regs': case.regs, 'entry_context': adapter_witness.entry_context(case),
                            'stack': case.stack, 'globals': case.globals,
                            'import_seed': case.import_seed, 'arena_sha256': hashlib.sha256(case.arena).hexdigest(),
                            'arena': case.arena.hex(), 'protect_at': protect_at},
                   'original': a, 'candidate': b, 'equal': a == b}
            path = work / (name + '.json')
            path.write_text(json.dumps(row), encoding='utf-8')
            if a != b:
                raise ValueError(f'raw witness mismatch: {va:#x} {opt} {name}')
            if protect_at is not None:
                adapter_witness.check_protected_fault(a, protect_at)
            if name.startswith('ecx-stack-'):
                adapter_witness.check_alias_reads(a['events'], case.regs['ecx'])
                if name == 'ecx-stack--4-returning-seed-35' and a['outcome'] != 'return':
                    raise ValueError('returning saved-slot alias did not return')
            if name.startswith('output-stack-'):
                offset = int(name.removeprefix('output-stack-'))
                adapter_witness.check_output_stores(a['events'], emu.STACK_TOP + offset)
            if name == 'output-arena-end-minus-2':
                if (a['outcome'] != 'fault' or a['detail'][-1] != 0x00521ea9
                        or a['events'][-2][:3] != ('w', emu.ARENA + emu.ARENA_SIZE - 2, 2)
                        or a['events'][-1][:3] != ('w', emu.ARENA + emu.ARENA_SIZE, 1)
                        or a['arena'][-4:] != '0100' or case.arena[-2:].hex() != '0003'):
                    raise ValueError('late word-store persistence not observed')
            rows.append({'va': hex(va), 'opt': opt, 'name': name, 'equal': a == b,
                         'outcome': a['outcome'], 'detail': a['detail'], 'path': str(path)})
            print(json.dumps(rows[-1]), flush=True)
if (len(rows) != 40 or identity != recover.execution_identity()
        or driver_hash != env.digest(Path(__file__))
        or checker_hash != env.digest(Path(adapter_witness.__file__))
        or validation_code != adapter_witness.validation_identity()
        or any(env.digest(env.ROOT / name) != digest for name, digest in snapshot_pins.items())
        or any(env.digest(OUT / 'input-snapshot' / name) != digest for name, digest in snapshot_pins.items())):
    raise ValueError('witness count or execution identity changed during run')
for va, (task, _, _) in TASKS.items():
    taskdir = env.ROOT / 'CM0102/Analysis/cmre/tasks' / task
    current = {'task': env.digest(taskdir / 'task.json'), 'report': env.digest(taskdir / 'report-1.json'),
               'source': env.digest(SOURCES[va])}
    if current != inputs[task]:
        raise ValueError('task inputs changed during run')
    tasks.validate_current(json.loads((taskdir / 'task.json').read_text(encoding='utf-8')))
receipt = {'status': 'pass', 'execution_identity': identity, 'inputs': inputs,
           'driver_sha256': driver_hash, 'checker_sha256': checker_hash,
           'validation_code': validation_code, 'snapshot_inputs': snapshot_pins, 'rows': rows,
           'artifacts': {str(path.relative_to(OUT)): env.digest(path)
                         for path in sorted(OUT.rglob('*')) if path.is_file()},
           'limitations': ['Supplemental VM witnesses, not acceptance or independent review.',
                           'Compared eight general registers and EFLAGS, not x87, SIMD or segment state.',
                           'Original callees, import stubs and VM continuation remain boundaries.',
                           'Guest VM faults do not establish native Windows exception semantics.']}
(OUT / 'summary.json').write_text(json.dumps(receipt, indent=2), encoding='utf-8')
