"""Supplemental, source-specific checks for the two retained stack adapters.

This is not an acceptance gate. In particular, original callees, import stubs,
and VM return transport remain boundaries. Fail closed on unknown instructions.
"""
from __future__ import annotations

import os
from pathlib import Path

import capstone
import unicorn

from cmre import emu, env, image, ledger, tasks

# The only alternative encodings seen in these two pinned straight-line bodies.
# Each is the same register-to-register operation with the opposite opcode form.
EQUIVALENT_ENCODINGS = {
    0x0040a7eb: ('8bce', '89f1'),  # mov ecx, esi
    0x0040a7fa: ('8bc6', '89f0'),  # mov eax, esi
    0x00521ea3: ('2acb', '28d9'),  # sub cl, bl
}


def admit_output(path, diagnostics_root, *, protected_roots=None):
    """Admit only a new, non-redirected child of disposable diagnostics."""
    requested = Path(os.path.abspath(path))
    root_requested = Path(os.path.abspath(diagnostics_root))
    root = root_requested.resolve()
    resolved = requested.resolve()
    if protected_roots is None:
        protected_roots = [env.ROOT / 'CM0102' / name for name in
                           ('Game', 'Original-3.9.60', 'InstallMedia', 'Inspection')]
    protected = [Path(path).resolve() for path in protected_roots]
    if (root_requested != root or requested != resolved or resolved == root
            or not resolved.is_relative_to(root) or requested.exists()
            or any(resolved == item or resolved.is_relative_to(item) for item in protected)):
        raise ValueError('output must be a new non-redirected diagnostics child')
    return resolved


def validation_identity():
    """Pin modules used by task validation but excluded from harness identity."""
    return {'cmre/tasks.py': env.digest(Path(tasks.__file__)),
            'cmre/ledger.py': env.digest(Path(ledger.__file__))}


def snapshot_inputs(output, workspace, pins):
    """Copy exact pinned workspace inputs into a new evidence snapshot."""
    workspace, output = Path(workspace).resolve(), Path(output)
    for name, expected in sorted(pins.items()):
        relative = Path(name)
        if relative.is_absolute() or '..' in relative.parts:
            raise ValueError('snapshot input escapes workspace')
        source = workspace / relative
        if source.resolve() != source or not source.is_file():
            raise ValueError('snapshot input missing or redirected')
        data = source.read_bytes()
        if env.digest_bytes(data) != expected or env.digest(source) != expected:
            raise ValueError('snapshot input changed')
        destination = output / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        with destination.open('xb') as handle:
            handle.write(data)
        if env.digest(destination) != expected:
            raise ValueError('snapshot copy differs')
    return dict(pins)


def entry_context(case):
    return {'case_esp': case.regs.get('esp', 0) & 0xffffffff,
            'entry_esp': emu.STACK_TOP, 'esp_overridden': True, 'fpcw': case.fpcw}


def check_alias_reads(events, source):
    """The adapter's first three source-byte reads must occur in order."""
    if len(events) < 9 or any(tuple(events[index][:3]) != ('r', (source + offset) & 0xffffffff, 1)
                              for index, offset in ((1, 2), (6, 4), (8, 3))):
        raise ValueError('named source aliases were not read in order')


def check_output_stores(events, destination):
    """The last two writes must be the named word and byte stores."""
    writes = [tuple(event[:3]) for event in events if event[0] == 'w']
    if len(writes) < 2 or writes[-2:] != [('w', destination & 0xffffffff, 2),
                                          ('w', (destination + 2) & 0xffffffff, 1)]:
        raise ValueError('named output stores were not observed in order')


def require_returning_seed(result, va, index):
    """Fail closed even when Python assertions are disabled."""
    if result.outcome != 'return':
        raise ValueError(f'corpus seed {va:#x} case {index} did not return: {result.outcome} {result.detail}')


def check_submission(task, record, report, digest, report_digest, identity):
    """Bind a witness run to the still-submitted, passing task and source."""
    if (record['task_id'] != task or record['status'] != 'submitted'
            or len(record['attempts']) != 1 or record['attempts'][0]['status'] != 'pass'
            or record['attempts'][0]['candidate_sha256'] != digest
            or record['attempts'][0]['report_sha256'] != report_digest
            or report['status'] != 'pass' or report['source_sha256'] != digest
            or report['execution_identity'] != identity):
        raise ValueError(f'submitted task identity mismatch: {task}')


def linked_instruction_map(va, end, call, built):
    """Compare every reachable instruction of a straight-line adapter through RET.

    Only the explicitly bound original CALL may have a different instruction
    sequence. Other instructions must decode to the same mnemonic and operands.
    Return an exact native-PC to original-PC map, never interpolated labels.
    """
    original = image.primary()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    bindings = [b for b in built.image.return_bindings if b.owner == va]
    if len(bindings) != 1 or bindings[0].original_call != call:
        raise ValueError('expected exactly one owned original CALL binding')
    binding = bindings[0]
    op, cp = va, built.entry
    mapping, rows = {}, []
    while op < end:
        oi = next(md.disasm(original.read_upto(op, 16), op, count=1), None)
        ci = next(md.disasm(built.image.read_upto(cp, 16), cp, count=1), None)
        if oi is None or ci is None:
            raise ValueError('undecodable instruction')
        mapping[cp] = op
        if op == call:
            if (oi.mnemonic != 'call' or cp != binding.compiled_push
                    or binding.original_return != op + oi.size
                    or binding.compiled_continuation != cp + 10):
                raise ValueError('CALL binding mismatch')
            rows.append((op, cp, 'owned-call'))
            op += oi.size
            cp = binding.compiled_continuation
            continue
        if (oi.mnemonic, oi.op_str) != (ci.mnemonic, ci.op_str):
            raise ValueError(f'instruction mismatch at {op:#x}: {oi.mnemonic} {oi.op_str} / {ci.mnemonic} {ci.op_str}')
        raw = (bytes(oi.bytes).hex(), bytes(ci.bytes).hex())
        if raw[0] != raw[1] and EQUIVALENT_ENCODINGS.get(op) != raw:
            raise ValueError(f'unreviewed instruction encoding at {op:#x}')
        rows.append((op, cp, *raw))
        op += oi.size
        cp += ci.size
        if oi.mnemonic == 'ret':
            if op != end:
                raise ValueError('unexpected RET extent')
            break
        if oi.group(capstone.CS_GRP_JUMP) or oi.mnemonic.startswith('call'):
            raise ValueError('unsupported control flow')
    else:
        raise ValueError('missing RET')
    for name, address in built.symbols.items():
        if name.startswith('v_') and address in mapping:
            if name != f'v_{mapping[address]:08x}':
                raise ValueError(f'inaccurate source label {name}')
    return mapping, rows


def normalize_fault(detail, mapping, text_lo, text_hi):
    """Translate only a verified instruction start in the linked text section."""
    detail = list(detail)
    pc = detail[-1]
    if text_lo <= pc < text_hi:
        if pc not in mapping:
            raise ValueError(f'unverified compiled fault PC {pc:#x}')
        detail[-1] = mapping[pc]
    return detail


def check_protected_fault(observation, site):
    """Require a protected four-byte read at the current stack pointer."""
    detail = observation['detail']
    if (observation['outcome'] != 'fault' or len(detail) != 2 or detail[-1] != site
            or tuple(detail[0]) != (unicorn.UC_MEM_READ_PROT, observation['regs'][4], 4)):
        raise ValueError(f'expected protected stack read at {site:#x}')


def comparable_dirty_pages(machine, result):
    """Retain changed original and shared pages, excluding the compiled PE."""
    pages = machine._common_pages | machine._original_pages
    return {str(page): data.hex() for page, data in result.memory.items() if page in pages}
