"""Retain bounded paired composition witnesses without changing any gate.

Run with an existing composition report, entry VA and inclusive corpus end.
Every generated case and its RNG/discovery prefix is retained. Reports expose
each comparison component even when the normal gate stops at an earlier one.
This is diagnostic evidence, never an acceptance or composition receipt.
"""
from __future__ import annotations

import argparse
import dataclasses
from datetime import datetime, timezone
import gzip
import json
from pathlib import Path
import random
import shutil
import struct
import sys
import time

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))


def encode(value):
    if dataclasses.is_dataclass(value):
        return encode(dataclasses.asdict(value))
    if isinstance(value, bytes):
        import hashlib
        return {'hex': value.hex(), 'sha256': hashlib.sha256(value).hexdigest()}
    if isinstance(value, dict):
        return {str(k): encode(v) for k, v in value.items()}
    if isinstance(value, (list, tuple)):
        return [encode(v) for v in value]
    if isinstance(value, set):
        return sorted(value)
    return value


def save(path, value):
    from cmre import env
    path = Path(path)
    payload = (json.dumps(encode(value), separators=(',', ':'))+'\n').encode('utf-8')
    if path.suffix == '.gz':
        payload = gzip.compress(payload, mtime=0)
    with path.open('xb') as stream:
        stream.write(payload)
    return {'path': str(path.resolve()), 'sha256': env.digest(path)}


def components(a, b, amap=None, *, fault_mapping=None):
    """Isolate fields using the existing comparator, retaining recovered events.

    For each field, align every other component to the recovered view. This
    retains its return-byte relation for memory comparison. If outcomes differ,
    the separate detail probe uses the recovered outcome; both raw outcomes
    and the unmodified first difference remain in the caller's report.
    """
    from cmre import difftest
    result = {}
    for index, name in enumerate(('outcome', 'detail', 'regs', 'events', 'memory')):
        left, right = list(b), list(b)
        left[index] = a[index]
        result[name] = difftest._diff(tuple(left), tuple(right), amap,
                                    fault_mapping=fault_mapping, strict_data_fault=True)
    return result


def raw_result(result, machine, entry_stack):
    from cmre import emu
    row = {f.name: getattr(result, f.name) for f in dataclasses.fields(result)
           if f.name != 'pristine'}
    row['machine_pristine_changed_pages'] = {p: result.pristine[p] for p in result.memory}
    # The VM's initial-page helper excludes ordinary argument-frame writes.
    # Capture stack bytes before the first instruction instead of labeling
    # machine-pristine stack bytes as the actual caller frame.
    row['case_initial_changed_pages'] = {
        p: (entry_stack[p-emu.STACK:p-emu.STACK+emu.PAGE]
            if emu.STACK <= p < emu.STACK+emu.STACK_SIZE else machine._initial_page(p))
        for p in result.memory}
    return row


class Observer:
    def __init__(self, machine, entries, shapes, return_kind='int'):
        import capstone
        import unicorn
        self.machine, self.entries, self.shapes = machine, entries, shapes
        self.return_kind = return_kind
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        self.enabled = False
        self.entry_stack = None
        self.instructions, self.arrivals, self.returns, self.frames = [], [], [], []
        machine.uc.hook_add(unicorn.UC_HOOK_CODE, self.code)

    def code(self, uc, address, size, _):
        from cmre import emu
        if not self.enabled:
            return
        if self.entry_stack is None:
            self.entry_stack = bytes(uc.mem_read(emu.STACK, emu.STACK_SIZE))
        regs = {n: uc.reg_read(r) for n, r in zip(emu.REG_NAMES, emu.REGS)}
        ordinal = len(self.instructions)
        # Recursive calls can share a continuation PC. Only frames whose
        # physical return word has actually been popped have completed.
        while (self.frames and self.frames[-1]['return_pc'] == address
               and regs['esp'] > self.frames[-1]['entry_esp']):
            frame = self.frames.pop()
            self.returns.append({'arrival': frame['arrival'], 'instruction_index': ordinal,
                                 'event_index': len(self.machine.events), 'pc': address, 'regs': regs})
        if address in self.entries:
            va = self.entries[address]
            shape = self.shapes[str(va)]
            esp = regs['esp']
            words = shape['stack_words']
            ret = struct.unpack('<I', bytes(uc.mem_read(esp, 4)))[0]
            args = list(struct.unpack('<'+'I'*words, bytes(uc.mem_read(esp+4, words*4))))
            self.arrivals.append({'function_va': va, 'physical_entry': address,
                                  'instruction_index': ordinal, 'event_index': len(self.machine.events),
                                  'regs': regs, 'ecx_argument': regs['ecx'] if shape['ecx'] else None,
                                  'stack_args': args, 'return_pc': ret})
            self.frames.append({'arrival': len(self.arrivals)-1, 'return_pc': ret, 'entry_esp':esp})
        raw = bytes(uc.mem_read(address, size))
        ins = next(self.md.disasm(raw, address, count=1), None)
        self.instructions.append({'pc': address, 'raw': raw.hex(),
                                  'instruction': ins.mnemonic+' '+ins.op_str if ins else None,
                                  'regs': regs, 'event_index': len(self.machine.events)})

    def run(self, entry, case, budget):
        from cmre import difftest, emu
        self.instructions, self.arrivals, self.returns, self.frames = [], [], [], []
        self.entry_stack = None
        self.enabled = True
        try:
            result = self.machine.run(entry, case, budget)
            fp = difftest.semantic_fp(self.machine.uc, self.return_kind)
        finally:
            self.enabled = False
        if result.outcome == 'return' and self.frames:
            frame = self.frames[-1]
            if frame['return_pc'] == emu.STOP and len(self.frames) == 1:
                self.frames.pop()
                self.returns.append({'arrival': frame['arrival'], 'instruction_index': len(self.instructions),
                                     'event_index': len(result.events), 'pc': emu.STOP,
                                     'regs': dict(zip(emu.REG_NAMES, result.regs))})
        if self.entry_stack is None:
            raise ValueError('No first-instruction initial stack observation')
        return result, fp, {'observer': 'Read-only UC_HOOK_CODE; no guest writes.',
                            'trace_complete': True, 'result': raw_result(result, self.machine, self.entry_stack),
                            'instructions': self.instructions, 'arrivals': self.arrivals,
                            'returns': self.returns, 'unreturned_frames': self.frames, 'return_fp': fp}


def call_arguments(data):
    return [{k: row[k] for k in ('function_va', 'ecx_argument', 'stack_args')}
            for row in data['arrivals']]


def same_observed_result(a, b):
    return all(getattr(a, f.name) == getattr(b, f.name)
               for f in dataclasses.fields(a) if f.name not in ('pristine', 'read_sites'))


def collect(report_path, entry, through, shape_path, output):
    import unicorn
    from cmre import composition, difftest, emu, env, image, lift, recover
    started, monotonic = datetime.now(timezone.utc).isoformat(), time.monotonic()
    report = json.loads(Path(report_path).read_text(encoding='utf-8-sig'))
    shapes = json.loads(Path(shape_path).read_text(encoding='utf-8-sig'))
    selected = [int(v[3:], 16) for v in report['campaign']['functions']]
    if entry not in selected:
        raise ValueError('Entry must belong to the selected composition')
    for name, shape in shapes.items():
        if str(int(name)) != name or set(shape) != {'ecx', 'stack_words', 'evidence'}:
            raise ValueError('Shapes require decimal VA keys and exact ecx/stack_words/evidence fields')
        if type(shape['ecx']) is not bool or type(shape['stack_words']) is not int or not 0 <= shape['stack_words'] <= 64 or not shape['evidence']:
            raise ValueError('Invalid shape or missing original-instruction evidence')
    if any(str(va) not in shapes for va in selected):
        raise ValueError('Every selected entry needs a reviewed observation shape')
    sources = [env.C_DIR/f'fn_{va:08x}.c' for va in selected]
    execution = recover.execution_identity()
    if execution != report['execution_identity']:
        raise ValueError('Execution identity differs from retained report')
    if env.digest(env.ROOT/'cmre/composition.py') != report['composition_sha256']:
        raise ValueError('Composition implementation differs')
    if {p.name: env.digest(p) for p in sources} != report['source_sha256']:
        raise ValueError('Selected source bytes differ')
    if report.get('jump_contracts') or report.get('x87_contracts'):
        raise ValueError('Transport-bearing compositions require their own observer; unsupported here')
    returns, problems = recover._semantic_contract(entry, (env.C_DIR/f'fn_{entry:08x}.c').read_text(encoding='utf-8'))
    if problems:
        raise ValueError(f'Unresolved entry return contract: {problems}')
    opts = composition.composition_c_options(entry)
    if opts.__dict__ != report['options'][f'fn_{entry:08x}']:
        raise ValueError('Effective options differ')
    if opts.hints.get('arena_size',emu.ARENA_SIZE) != emu.ARENA_SIZE:
        raise ValueError('Expanded-arena composition observer is not established by this collector')
    if not 0 <= through < opts.cases:
        raise ValueError('Corpus end must be inside the original complete corpus')
    output = Path(output)
    output.mkdir(parents=True, exist_ok=False)
    pinned = sources+[env.HINTS_DIR/f'fn_{va:08x}.json' for va in selected]
    pinned += [env.ROOT/'recovery/include/cmre.h',env.ROOT/'cmre/composition.py',Path(shape_path),Path(__file__)]
    pinned += [env.ROOT/'cmre'/name for name in recover.HARNESS_MODULES]
    pins = {str(p.resolve()): env.digest(p) if p.is_file() else None for p in pinned}
    snapshots = output/'executed-inputs'; snapshots.mkdir()
    retained = {}
    for index,p in enumerate(pinned+[Path(report_path)]):
        if p.is_file():
            target = snapshots/f'{index:03d}-{p.name}'
            shutil.copyfile(p,target)
            retained[str(p.resolve())] = {'path':str(target.resolve()),'sha256':env.digest(target)}
    save(output/'SNAPSHOTS.json',retained)
    save(output/'IDENTITY.json', {'stage':'diagnostic-only', 'started':started, 'execution':execution,
         'report':str(Path(report_path).resolve()), 'report_sha256':env.digest(report_path),
         'pins':pins, 'options':opts.__dict__, 'shapes':shapes,'semantic_return_kind':returns,
         'address_convention':'preferred-image VA, image base 0x00400000',
         'input_sha256':report['input_sha256'] if 'input_sha256' in report else image.primary().sha256})
    lifted = {va: lift.discover(va) for va in selected}
    if any(b.problems for b in lifted.values()):
        raise ValueError('Original discovery has unresolved problems')
    save(output/'ORIGINAL-INSTRUCTIONS.json.gz',lifted)
    prefix, cases = [], []
    rng, templates, inputs = random.Random(opts.seed), difftest.arena_templates(), set()
    plain = emu.Machine(image.primary(),watch=(entry,lifted[entry].region_end),arena_size=opts.hints.get('arena_size',emu.ARENA_SIZE))
    for index in range(through+1):
        rng_before, inputs_before = rng.getstate(), sorted(inputs)
        case = difftest.make_case(rng,opts,inputs_before,templates)
        own = difftest._discovery(plain,opts,index,(entry,lifted[entry].region_end)) if opts.discovery else None
        original = plain.run(entry,case,opts.budget)
        if index < opts.discovery:
            inputs |= difftest._global_reads(original,image.primary(),own)
        case_pin = save(output/f'case-{index}-input.json.gz',case)
        prefix.append({'corpus_index':index,'case':case_pin,'rng_before':rng_before,
                       'rng_after':rng.getstate(),'global_inputs_before':inputs_before,
                       'global_inputs_after':sorted(inputs),'discovery_region':own,
                       'outcome':original.outcome,'detail':original.detail})
        cases.append((case,original))
    save(output/'GENERATOR-PREFIX.json.gz',prefix)
    rows, builds = [], {}
    for opt in ('-O0','-O2'):
        work = output/opt[1:]; work.mkdir()
        built = composition._build_separate(sources,lift.symbol(selected[0]),work,opt)
        symbols = composition._symbols(built,selected)
        artifacts = built.composition_artifacts
        artifacts['retained_files'] = {str(p.resolve()):env.digest(p) for p in work.iterdir() if p.is_file()}
        artifacts['symbols'] = built.symbols
        builds[opt] = save(work/'BUILD.json',artifacts)
        original_machine = emu.Machine(image.primary(),watch=(entry,lifted[entry].region_end),arena_size=opts.hints.get('arena_size',emu.ARENA_SIZE))
        recovered_machine = emu.Machine(image.primary(),built.image,arena_size=opts.hints.get('arena_size',emu.ARENA_SIZE))
        for body in lifted.values():
            recovered_machine.uc.hook_add(unicorn.UC_HOOK_CODE,recovered_machine._escape,begin=body.va,end=body.region_end-1)
        va_shapes = {int(v):int(v) for v in shapes}
        a_observer = Observer(original_machine,va_shapes,shapes,returns)
        b_observer = Observer(recovered_machine,{**{v:v for v in va_shapes if v not in selected},**{address:va for va,address in symbols.items()}},shapes,returns)
        amap = emu.AddressMap(built.symbols,{},built.image.text.va,built.image.text.end,
                             verified_fault_locations=composition._verified_fault_locations(lifted,built,symbols))
        for index,(case,prefix_result) in enumerate(cases):
            # Run discovery's observer setup at the same prefix position before
            # enabling instruction attribution; this does not alter case bytes.
            if opts.discovery:
                difftest._discovery(original_machine,opts,index,(entry,lifted[entry].region_end))
            a,fp_a,da = a_observer.run(entry,case,opts.budget)
            b,fp_b,db = b_observer.run(symbols[entry],case,opts.budget)
            if not same_observed_result(a,prefix_result):
                raise ValueError(f'Observer changed original case {index}')
            pair = [save(work/f'case-{index}-{side}.json.gz',data)
                    for side,data in [('original',da),('recovered',db)]]
            aa,bb = difftest._normalize(a,None,'abi'),difftest._normalize(b,amap,'abi')
            sa = difftest._normalize(a,None,'semantic',returns,fp_a)
            sb = difftest._normalize(b,amap,'semantic',returns,fp_b)
            row = {'opt':opt,'corpus_index':index,'witnesses':pair,
                   'outcomes':[a.outcome,b.outcome],
                   'first_abi_difference':difftest._diff(aa,bb,amap,fault_mapping=b.fault_mapping,strict_data_fault=True),
                   'abi_components':components(aa,bb,amap,fault_mapping=b.fault_mapping),
                   'semantic_components':components(sa,sb,amap,fault_mapping=b.fault_mapping),
                   'callee_arguments_equal':call_arguments(da)==call_arguments(db),
                   'calls_original':call_arguments(da),'calls_recovered':call_arguments(db),
                   'instruction_counts':[len(da['instructions']),len(db['instructions'])],
                   'event_counts':[len(a.events),len(b.events)],
                   'original_observer_matches_unobserved_prefix':True}
            rows.append(row)
            save(work/f'case-{index}-comparison.json',row)
    if recover.execution_identity() != execution or any((env.digest(Path(p)) if Path(p).is_file() else None)!=h for p,h in pins.items()):
        raise ValueError('Declared diagnostic inputs changed')
    summary = {'schema':'cmre-paired-diagnostics/1','stage':'diagnostic-only','started':started,
               'elapsed_seconds':time.monotonic()-monotonic,'corpus_indices':list(range(through+1)),
               'entry':f'fn_{entry:08x}','builds':builds,'rows':rows,'canonical_pins_unchanged':True,
               'gate_status':'unchanged; no acceptance or composition gate executed',
               'limitations':(['Bounded prefix only; remaining cases are unclassified.'] if through+1<opts.cases else ['All entry corpus cases observed; other selected entry corpora are outside this run.'])+[
                              'Retained generated PEs are diagnostic rebuilds, not historical gate PEs.',
                              'Physical arguments are limited to explicitly evidenced shapes.',
                              'Semantic comparisons are supplemental; ABI differences remain obligations.']}
    save(output/'SUMMARY.json',summary)
    print(json.dumps({'summary':str(output/'SUMMARY.json'),'rows':len(rows),
                      'elapsed_seconds':summary['elapsed_seconds']}))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report',required=True,type=Path)
    parser.add_argument('--entry',required=True,type=lambda v:int(v.removeprefix('fn_'),16))
    parser.add_argument('--through',required=True,type=int)
    parser.add_argument('--shapes',required=True,type=Path)
    parser.add_argument('--output',required=True,type=Path)
    args = parser.parse_args()
    collect(args.report,args.entry,args.through,args.shapes,args.output)


if __name__ == '__main__':
    main()
