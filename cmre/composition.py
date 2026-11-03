"""Experimental composed-C differential reports, separate from ledger acceptance.

Selected C definitions compile separately and link into one image. Outside
functions/data and import scaffolding remain boundaries.
"""
from __future__ import annotations

import hashlib
import json
import random
import re
import struct
import tempfile
from bisect import bisect_left
from itertools import accumulate
from pathlib import Path

import unicorn

from . import build, campaign, coordination, difftest, emu, env, image, jump_transport, ledger, lift, recover, return_transport, x87_transport


_SOURCE = re.compile(r'fn_([0-9a-f]{8})\.(?:c|S)\Z')
_LOCATION = re.compile(r'v_[0-9a-f]{8}\Z')
_TRANSPORT_PREFIXES = ('cmre_call_', 'cmre_return_', 'cmre_indirect_', 'cmre_jump_', 'cmre_x87_')
_DATE_OWNER_SOURCE = '0635d55a18cbf1478fab44c23c7f379f08bfca4a9bbf81b96a7a6a875343ce58'
_DATE_OWNER_HEADER = 'a12695b8bbf986f11b80f0be03c1cd72b5cf91dba8e44a85b02c7d21a9c08cec'
_DATE_OWNER_GCC = '690d171db384af0f69002374a7fc4755128ee82c688d22818bd981f869a65890'
_DATE_OWNER_AS = '5e1a093518848dd42c445f8d985d904c4723e6bf0a2c52d543fb853357ef0bc3'
_DATE_OWNER_SYMBOLS = '42fdb0559befff44e75b46a55f1f8bbae1911e0927d441aba25cafb68fe961a5'
_DATE_OWNER_TEXT = {'-O0': '0ad78046eef3dde61d1ae7fbe02145acf8f4ea97493088338fcaa65f1c410fea',
                    '-O2': '7908790707cdbda988f08ba1f4cb612f6a477174431963511f44583caadffba6'}
_LEGACY_LIFT = 0x00417c10
_LEGACY_EXTENT = '95f85417c6b96a43ef68136ef4f656434e23a60a51771a31a52f8aeff01dc289'
_LEGACY_LIFT_SOURCE = '2fbd49caa65f184ebb3c3f19a17fee8da71777e89618ea9a804edc1064968fe2'
_LEGACY_FILES = {
    'recovery/date/ledger.json': '2b836c288ab34102217087cee8f084d0f8c642f925ff77436bc3832b04c2c179',
    'recovery/date/cm_date.c': '518e6024ab6a4f8512b73708ddf689d7f79d6bb92f79c86bc2cc4e85f5ebcb94',
    'recovery/date/cm_date.h': '6f1df7e3420f1429fd17f0ec7604ee6500892881959b0bac1cc0c773e4a597cd',
}


def composition_c_options(va, overrides=None):
    """Use identical per-entry C policy snapshot at start and drift check."""
    opts = difftest.Options.for_function(va, overrides, tier='c')
    opts.inputs = recover.c_inputs(va)
    return opts


def _checked_nm(path):
    command = [env.tool('nm'), str(path)]
    result = env.run_tool(command)
    if result.returncode:
        raise build.BuildError('nm failed', result.stderr)
    rows = []
    for line in result.stdout.splitlines():
        fields = line.split()
        if len(fields) < 2:
            continue
        kind, name = fields[-2:]
        if len(kind) != 1:
            raise build.BuildError('cannot parse nm symbol type')
        address = None
        if kind != 'U':
            if len(fields) != 3:
                raise build.BuildError('cannot parse defined nm symbol')
            try:
                address = int(fields[0], 16)
            except ValueError as exc:
                raise build.BuildError('cannot parse nm symbol address') from exc
        rows.append((kind, name, address))
    return rows


def _object_sections(path):
    """Read COFF symbol sections and function boundaries; fail closed on missing metadata."""
    result = env.run_tool([env.tool('objdump'), '-t', str(path)])
    if result.returncode:
        raise build.BuildError('objdump symbol table failed', result.stderr)
    pattern = re.compile(r'^\[\s*\d+\]\(sec\s+(-?\d+)\)\(fl\s+0x[0-9a-fA-F]+\)'
                         r'\(ty\s+([0-9a-fA-F]+)\)\(scl\s+(\d+)\)\s+\(nx\s+\d+\)\s+'
                         r'0x([0-9a-fA-F]+)\s+(\S+)$')
    rows = []
    for line in result.stdout.splitlines():
        match = pattern.match(line.strip())
        if match:
            section, kind, storage, offset, name = match.groups()
            rows.append((int(section), int(kind, 16), int(offset, 16), name, int(storage)))
    if not rows:
        raise build.BuildError('unreadable object symbol table')
    return rows


def _coff_text(path):
    """Extract a single raw .text section from a 32-bit COFF object."""
    data = path.read_bytes()
    if len(data) < 20:
        raise build.BuildError('truncated COFF object')
    machine, count, _, _, _, optional, _ = struct.unpack_from('<HHIIIHH', data)
    start = 20 + optional
    if machine != 0x14c or start + count * 40 > len(data):
        raise build.BuildError('invalid 32-bit COFF sections')
    found = []
    for index in range(count):
        at = start + index * 40
        name = data[at:at + 8].rstrip(b'\0')
        size, offset = struct.unpack_from('<II', data, at + 16)
        if name == b'.text':
            relocations = struct.unpack_from('<H', data, at + 32)[0]
            if relocations:
                raise build.BuildError('reviewed COFF text must not contain relocations')
            if offset + size > len(data):
                raise build.BuildError('truncated COFF text')
            found.append(data[offset:offset + size])
    if len(found) != 1:
        raise build.BuildError('expected one COFF .text section')
    return found[0]


def _reviewed_date_finish(va, source_hash, obj, section_rows, opt):
    """Admit only the independently reviewed date helper's exact object layout."""
    if va != 0x00549ef0 or source_hash != _DATE_OWNER_SOURCE:
        return False
    symbols = env.digest_bytes(json.dumps(section_rows, sort_keys=True, separators=(',', ':')).encode())
    return (env.digest(env.ROOT / 'recovery/include/cmre.h') == _DATE_OWNER_HEADER
            and env.toolchain_identity()['gcc']['sha256'] == _DATE_OWNER_GCC
            and env.toolchain_identity()['as']['sha256'] == _DATE_OWNER_AS
            and symbols == _DATE_OWNER_SYMBOLS
            and env.digest_bytes(_coff_text(obj)) == _DATE_OWNER_TEXT[opt])


def _build_separate(sources, entry_symbol, workdir, opt='-O0', *, lift_companions=None):
    """Compile exact source TUs, link selected definitions and bind VM transports.

    The set of selected functions is the set of fn_<va>.c filenames. A selected
    undefined symbol must have an exactly matching decorated selected definition;
    only undefined nonselected names may bind to original image addresses.
    """
    sources = [Path(p) for p in sources]
    lift_companions = lift_companions or {}
    workdir = Path(workdir)
    if opt not in ('-O0', '-O2') or len(sources) < 2:
        raise build.BuildError('separate composition requires at least two sources and O0/O2')
    if not workdir.is_dir():
        raise build.BuildError('work directory must already exist')
    selected = {}
    for source in sources:
        match = _SOURCE.fullmatch(source.name)
        if not match or not source.is_file():
            raise build.BuildError('invalid selected source filename')
        va = int(match.group(1), 16)
        if (source.suffix == '.S') != (va in lift_companions):
            raise build.BuildError('assembly sources require an explicit lift companion')
        if source.suffix == '.S':
            body = lift_companions[va]
            if va != _LEGACY_LIFT or body.va != va or source.read_text(encoding='utf-8') != lift.render(body):
                raise build.BuildError('unrecognized or changed lift companion')
            if env.digest(source) != _LEGACY_LIFT_SOURCE:
                raise build.BuildError('lift companion source differs from reviewed instructions')
            if any(ins.kind in ('call', 'indirect_call', 'import_call', 'table_jmp') for ins in body.insns.values()):
                raise build.BuildError('legacy companion has unsupported call or table transport')
        if va in selected:
            raise build.BuildError('duplicate selected source')
        selected[va] = source
    entry_match = build.DECORATED.fullmatch(entry_symbol)
    if not entry_match or int(entry_match.group(1), 16) not in selected:
        raise build.BuildError('entry must be one selected function')
    entry_va = int(entry_match.group(1), 16)
    source_hashes = {source.name: env.digest(source) for source in sources}
    objects, object_hashes, commands = [], {}, []
    for source in sources:
        obj = workdir / (source.stem + '.o')
        cc = [env.tool('gcc'), '-m32', '-c', str(source), '-o', str(obj), '-Wall', '-Wextra', '-Werror']
        if source.suffix == '.c':
            cc += [opt, '-mno-stack-arg-probe', '-fno-asynchronous-unwind-tables', '-ffreestanding',
                   '-fno-builtin', f'-I{env.ROOT / "recovery/include"}']
        commands.append(cc)
        result = env.run_tool(cc)
        if result.returncode:
            raise build.BuildError(f'compile failed: {source.name}', result.stderr)
        if env.digest(source) != source_hashes[source.name]:
            raise build.BuildError('source changed during compilation')
        objects.append(obj)
        object_hashes[source.name] = env.digest(obj)

    # Preserve object-local labels for address mapping and transport binding.
    # A duplicate v_<VA> or transport marker at distinct object offsets is
    # inherently ambiguous even if `nm` on the final PE would hide one in dict.
    defs_by_va, undefined, address_labels, marker_sources = {}, set(), set(), {}
    object_rows = {}
    for source, obj in zip(sources, objects):
        rows = _checked_nm(obj)
        object_rows[int(_SOURCE.fullmatch(source.name).group(1), 16)] = rows
        for kind, name, _ in rows:
            if kind != 'U' and (_LOCATION.fullmatch(name) or name.startswith(_TRANSPORT_PREFIXES)):
                if name in address_labels:
                    raise build.BuildError(f'duplicate location/transport symbol: {name}')
                address_labels.add(name)
                marker_sources[name] = int(_SOURCE.fullmatch(source.name).group(1), 16)
            match = build.DECORATED.fullmatch(name)
            if not match:
                continue
            va = int(match.group(1), 16)
            if kind == 'U':
                undefined.add(name)
            elif va in selected:
                source_va = int(_SOURCE.fullmatch(source.name).group(1), 16)
                if kind != 'T' or va in defs_by_va or va != source_va:
                    raise build.BuildError(f'ambiguous selected definition: {name}')
                defs_by_va[va] = name
            else:
                # An unselected compiled function would silently change a
                # declared outside-original boundary, so reject it.
                raise build.BuildError(f'unselected compiled definition: {name}')
    if set(defs_by_va) != set(selected):
        raise build.BuildError('each selected function needs one global text definition')
    for va, rows in object_rows.items():
        original_body = lift.discover(va)
        if original_body.problems:
            raise build.BuildError(f'unresolved original owner fn_{va:08x}')
        section_rows = _object_sections(workdir / (selected[va].stem + '.o'))
        reviewed_date_finish = _reviewed_date_finish(
            va, source_hashes[selected[va].name], workdir / (selected[va].stem + '.o'), section_rows, opt)
        text_rows = [(name, address) for kind, name, address in rows if kind in ('t', 'T')]
        for name, address in text_rows:
            if sum(section > 0 and offset == address and symbol == name
                   for section, _, offset, symbol, _ in section_rows) != 1:
                raise build.BuildError(f'object text symbol section metadata is incomplete or ambiguous: {name}')
        entries_in_object = [(section, offset) for section, kind, offset, name, storage in section_rows
                             if name == defs_by_va[va] and section > 0
                             and (kind == 0x20 or (va in lift_companions and kind == 0 and storage == 2))]
        if len(entries_in_object) != 1:
            raise build.BuildError('ambiguous object entry')
        entry_section, entry_offset = entries_in_object[0]
        other_functions = [offset for section, kind, offset, name, storage in section_rows
                           if section == entry_section and name != defs_by_va[va]
                           and not (reviewed_date_finish and name == '_date_finish' and storage == 6)
                           and not _LOCATION.fullmatch(name) and not name.startswith(_TRANSPORT_PREFIXES)
                           and not (kind == 0 and offset == 0 and name.startswith('.text'))]
        if entry_offset in other_functions:
            raise build.BuildError('ambiguous compiled function entry')
        next_function = min((offset for offset in other_functions if offset > entry_offset), default=None)
        marker_rows = [(section, offset, name) for section, _, offset, name, _ in section_rows
                       if _LOCATION.fullmatch(name) or name.startswith(_TRANSPORT_PREFIXES)]
        expected_markers = {name for kind, name, _ in rows if kind != 'U'
                            and (_LOCATION.fullmatch(name) or name.startswith(_TRANSPORT_PREFIXES))}
        if len(marker_rows) != len(expected_markers) or {name for _, _, name in marker_rows} != expected_markers:
            raise build.BuildError('object marker section metadata is incomplete or ambiguous')
        for section, address, name in marker_rows:
            if _LOCATION.fullmatch(name) and int(name[2:], 16) not in original_body.insns:
                raise build.BuildError(f'location marker belongs to a different original owner: {name}')
            if section != entry_section or address < entry_offset or (next_function is not None and address >= next_function):
                raise build.BuildError(f'location/transport marker outside compiled owner: {name}')
    externs = {}
    for name in undefined:
        match = build.DECORATED.fullmatch(name)
        va = int(match.group(1), 16)
        if va in selected:
            if name != defs_by_va[va]:
                raise build.BuildError(f'selected symbol decoration mismatch: {name} vs {defs_by_va[va]}')
        else:
            externs[name] = va
    script = workdir / 'composed.externs.ld'
    script.write_text(''.join(f'"{name}" = {va:#x};\n' for name, va in sorted(externs.items())),
                      encoding='utf-8', newline='\n')
    exe = workdir / 'composed.exe'
    ld = [env.tool('ld'), '-mi386pe', '--image-base', hex(env.LINK_BASE), '--entry', defs_by_va[entry_va],
          '--disable-reloc-section', '-o', str(exe), *(str(obj) for obj in objects), str(script)]
    commands.append(ld)
    result = env.run_tool(ld)
    if result.returncode:
        raise build.BuildError('composition link failed', result.stderr)
    if any(env.digest(obj) != object_hashes[source.name] for source, obj in zip(sources, objects)):
        raise build.BuildError('object changed during linking')
    if any(env.digest(source) != source_hashes[source.name] for source in sources):
        raise build.BuildError('source changed during linking')
    symbols = {}
    for kind, name, address in _checked_nm(exe):
        if kind == 'U':
            raise build.BuildError(f'unresolved linked symbol: {name}')
        if name in symbols and symbols[name] != address and (_LOCATION.fullmatch(name)
                or name.startswith(_TRANSPORT_PREFIXES) or build.DECORATED.fullmatch(name)):
            raise build.BuildError(f'ambiguous linked address symbol: {name}')
        symbols[name] = address
    img = image.Image(exe.read_bytes(), env.LINK_BASE)
    entries = {}
    for va, decorated in defs_by_va.items():
        address = symbols.get(decorated)
        if address is None or not img.text.va <= address < img.text.end:
            raise build.BuildError(f'selected symbol fn_{va:08x} not in compiled text')
        entries[va] = address
    for name, va in externs.items():
        if symbols.get(name) != va:
            raise build.BuildError(f'outside symbol binding changed: {name}')
    try:
        img.return_bindings = return_transport.bind(img, symbols, entries)
        img.jump_bindings = jump_transport.bind(img, symbols, entries)
        img.x87_bindings = x87_transport.bind(img, symbols, entries)
        for binding in img.return_bindings:
            starts = [name for name in marker_sources if name.startswith(('cmre_call_', 'cmre_indirect_'))
                      and symbols.get(name) == binding.compiled_push]
            ends = [name for name in marker_sources if name.startswith('cmre_return_')
                    and symbols.get(name) == binding.compiled_continuation]
            if (len(starts) != 1 or len(ends) != 1
                    or marker_sources[starts[0]] != binding.owner or marker_sources[ends[0]] != binding.owner):
                raise ValueError('CALL transport marker belongs to a different source owner')
        for binding in img.jump_bindings:
            name = f'cmre_jump_{binding.original_jump:08x}'
            if marker_sources.get(name) != binding.owner:
                raise ValueError('indexed JMP marker belongs to a different source owner')
        for binding in img.x87_bindings:
            site = f'cmre_x87_{binding.original_instruction:08x}'
            end = f'cmre_x87_return_{binding.original_instruction:08x}_{binding.original_successor:08x}'
            if marker_sources.get(site) != binding.owner or marker_sources.get(end) != binding.owner:
                raise ValueError('x87 transport marker belongs to a different source owner')
    except ValueError as exc:
        raise build.BuildError(f'composition transport: {exc}') from exc
    compiled = build.Built(img, symbols, entries[entry_va], commands)
    for va, body in lift_companions.items():
        if symbols.get(lift.label(va)) != entries[va] or build.round_trip(body, compiled):
            raise build.BuildError(f'lift companion fn_{va:08x} failed entry or instruction round-trip')
    compiled.composition_artifacts = {
        'sources': source_hashes,
        'objects': object_hashes,
        'linked_sha256': env.digest(exe),
        'linker_script_sha256': env.digest(script),
        'entry': hex(entries[entry_va]),
        'compiled_entries': {f'fn_{va:08x}': hex(address) for va, address in entries.items()},
        'outside_original_bindings': {name: hex(va) for name, va in sorted(externs.items())},
        'commands': commands,
    }
    return compiled


def _symbols(built, selected: list[int]) -> dict[int, int]:
    symbols = {}
    for name, address in built.symbols.items():
        match = build.DECORATED.match(name)
        if match and int(match.group(1), 16) in selected:
            va = int(match.group(1), 16)
            if not built.image.text.va <= address < built.image.text.end:
                raise ValueError(f"selected function fn_{va:08x} still binds outside recovered code")
            if va in symbols and symbols[va] != address:
                raise ValueError(f"ambiguous selected function fn_{va:08x}")
            symbols[va] = address
    if set(symbols) != set(selected):
        raise ValueError("every selected function needs a compiled definition")
    return symbols


def _instruction_access_index(instructions):
    """Index strict interval overlap, preserving gaps and nested instructions."""
    spans = sorted((ins.va, ins.va + ins.size) for ins in instructions)
    starts = tuple(start for start, _ in spans)
    furthest_ends = tuple(accumulate((end for _, end in spans), max))

    def overlaps(address, size):
        # A matching span starts strictly before the access end. Among every
        # eligible span, one ends after the access start iff this prefix's
        # maximum end does. Prefix maxima retain nested/overlapping spans.
        candidate = bisect_left(starts, address + size) - 1
        return candidate >= 0 and furthest_ends[candidate] > address

    return overlaps


def _verified_fault_locations(lifted, built, entries):
    """Map exact labels only when original and compiled ownership agree."""
    ordered = sorted((addr, va) for va, addr in entries.items())
    if len({address for address, _ in ordered}) != len(ordered):
        raise ValueError('ambiguous compiled owner entry')
    intervals = {va: (address, ordered[i + 1][0] if i + 1 < len(ordered) else built.image.text.end)
                 for i, (address, va) in enumerate(ordered)}
    locations = {}
    seen_addresses = set()
    for name, address in built.symbols.items():
        if not _LOCATION.fullmatch(name):
            continue
        original = int(name[2:], 16)
        owners = [va for va, body in lifted.items() if original in body.insns]
        if len(owners) != 1 or owners[0] not in intervals:
            raise ValueError('ambiguous original fault label owner')
        lo, hi = intervals[owners[0]]
        if not lo <= address < hi or address in seen_addresses:
            raise ValueError('fault label outside or ambiguous compiled owner')
        seen_addresses.add(address)
        instruction = lifted[owners[0]].insns[original]
        size = instruction.size
        if (type(size) is not int or size <= 0 or len(instruction.raw) != size
                or not built.image.text.va <= address < built.image.text.end
                or address + size > hi or address + size > built.image.text.end):
            raise ValueError('fault instruction span crosses owner or executable section')
        # A label is only an address claim. Directly comparable ordinary
        # instruction bytes are required before mapping an exact fault PC.
        # CALL/branch/RET adapters remain unproven raw locations here.
        if instruction.kind != 'plain' or built.image.read(address, instruction.size) != instruction.raw:
            continue
        locations[address] = original
    return locations


def _run(va, lifted, built, symbols, opts):
    original = emu.Machine(image.primary(), watch=(va, lifted[va].region_end))
    recovered = emu.Machine(image.primary(), built.image)
    # A hard-coded original call/callback to ANY selected member is a failure.
    for item in lifted.values():
        recovered.uc.hook_add(unicorn.UC_HOOK_CODE, recovered._escape,
                              begin=item.va, end=item.region_end - 1)
    visits = {}
    reverse = {address: entry for entry, address in symbols.items()}

    def visit(uc, address, size, data):
        if address in reverse:
            entry = reverse[address]
            visits[entry] = visits.get(entry, 0) + 1

    recovered.uc.hook_add(unicorn.UC_HOOK_CODE, visit,
                          begin=built.image.text.va, end=built.image.text.end - 1)
    fault_locations = _verified_fault_locations(lifted, built, symbols)
    amap = emu.AddressMap(built.symbols, {}, built.image.text.va, built.image.text.end,
                          verified_fault_locations=fault_locations)
    rng, templates = random.Random(opts.seed), difftest.arena_templates()
    inputs, covered, outcomes = set(), set(), {}
    instruction_access = _instruction_access_index(lifted[va].insns.values())
    mismatches, inconclusive, code_as_data = [], 0, 0
    mapping_faults = []
    digest = hashlib.sha256()
    compare_fp = bool(getattr(built.image, 'x87_bindings', ()))
    for index in range(opts.cases):
        case = difftest.make_case(rng, opts, sorted(inputs), templates)
        own = (difftest._discovery(original, opts, index, (va, lifted[va].region_end))
               if opts.discovery else None)
        a = original.run(va, case, opts.budget)
        fp_a = x87_transport.snapshot(original.uc) if compare_fp else None
        b = recovered.run(symbols[va], case, opts.budget)
        fp_b = x87_transport.snapshot(recovered.uc) if compare_fp else None
        if index < opts.discovery:
            inputs |= difftest._global_reads(a, image.primary(), own)
        covered |= a.covered
        outcomes[a.outcome] = outcomes.get(a.outcome, 0) + 1
        na, nb = difftest._normalize(a, None, "abi"), difftest._normalize(b, amap, "abi")
        fault_mapping = getattr(b, 'fault_mapping', None)
        if fault_mapping is not None:
            mapping_faults.append({'case': index, 'proof': fault_mapping,
                                   'relation_used': a.outcome == b.outcome == 'fault'
                                   and difftest._mapping_fault_pair(na[1], nb[1], fault_mapping)})
        digest.update(difftest._digest_view(na).encode())
        if compare_fp:
            digest.update(repr(fp_a).encode())
        if "budget" in (a.outcome, b.outcome):
            inconclusive += 1
            continue
        difference = difftest._diff(na, nb, amap, fault_mapping=fault_mapping, strict_data_fault=True)
        if compare_fp:
            difference = x87_transport.difference(fp_a, fp_b) or difference
        if difference and len(mismatches) < 5:
            mismatches.append({"case": index, **difference})
        # Embedded tables/alignment are data. Check the whole access so reads
        # starting in data or outside the extent cannot hide instruction bytes.
        if any(kind in ("r", "w") and instruction_access(address, size)
               for kind, address, size, _ in a.events):
            code_as_data += 1
    coverage = len(covered & set(lifted[va].insns)) / len(lifted[va].insns)
    good = (not mismatches and not inconclusive and not code_as_data
            and coverage >= opts.min_coverage and outcomes.get("return", 0) > 0)
    return {"status": "pass" if good else "fail", "cases": opts.cases, "seed": opts.seed,
            "budget": opts.budget, "min_coverage": opts.min_coverage,
            "coverage": coverage, "outcomes": outcomes, "mismatches": mismatches,
            "inconclusive": inconclusive, "code_as_data_cases": code_as_data,
            "result_digest": digest.hexdigest(),
            **({'mapping_faults': mapping_faults} if mapping_faults else {}),
            "fp_registers_compared": list(x87_transport.FP_REGISTERS) if compare_fp else [],
            "selected_entry_visits": {campaign._id(v): n for v, n in sorted(visits.items())},
            "observed_other_selected_entries": [campaign._id(v) for v in sorted(visits) if v != va]}


def test(manifest: dict, overrides: dict | None = None, *, lift_companions=()) -> dict:
    """Build and compare every selected C entry twice at O0/O2, without acceptance."""
    manifest = campaign._validate(manifest)
    selected = [campaign._va(v) for v in manifest["functions"]]
    if len(selected) < 2:
        raise ValueError("composition requires at least two selected functions")
    companions = tuple(campaign._va(value) for value in lift_companions)
    if len(companions) != len(set(companions)) or any(va not in selected or va != _LEGACY_LIFT for va in companions):
        raise ValueError('lift companions must be distinct selected supported legacy functions')
    legacy_snapshot = {}
    if companions:
        entries = [entry for entry in ledger.legacy_entries() if entry.va == _LEGACY_LIFT]
        if len(entries) != 1 or entries[0].source != 'recovery/date/ledger.json' or entries[0].sha256 != _LEGACY_EXTENT:
            raise ValueError('legacy companion ledger identity differs')
        if env.digest_bytes(image.primary().read(_LEGACY_LIFT, entries[0].size)) != _LEGACY_EXTENT:
            raise ValueError('legacy companion original extent differs')
        legacy_snapshot = {name: env.digest(env.ROOT / name) for name in _LEGACY_FILES}
        if legacy_snapshot != _LEGACY_FILES:
            raise ValueError('legacy companion dependencies differ from reviewed evidence')
    sources_by_va = {va: env.C_DIR / f"fn_{va:08x}.c" for va in selected if va not in companions}
    sources = list(sources_by_va.values())
    missing = [str(p) for p in sources if not p.is_file()]
    if missing:
        raise ValueError(f"selected C sources missing: {missing}")
    identities = {p.name: env.digest(p) for p in sources}
    jump_contracts = {campaign._id(va): contract for va in selected
                      if (contract := recover.jump_contract(va)) is not None}
    x87_contracts = {campaign._id(va): contract for va in selected
                     if (contract := recover.x87_contract(va)) is not None}
    execution_identity = recover.execution_identity()
    composition_identity = env.digest(Path(__file__))
    objdump_path = Path(env.tool('objdump'))
    objdump_sha256 = env.digest(objdump_path)
    objdump_version = env.run_tool([str(objdump_path), '--version'])
    if objdump_version.returncode or not objdump_version.stdout.splitlines():
        raise build.BuildError('objdump version unavailable', objdump_version.stderr)
    objdump_version = objdump_version.stdout.splitlines()[0]
    dependencies = {p.name: recover.c_dependencies(p) for p in sources}
    header = env.ROOT / "recovery/include/cmre.h"
    header_identity = env.digest(header)
    lifted = {va: lift.discover(va) for va in selected}
    if any(item.problems for item in lifted.values()):
        raise ValueError("selected function discovery has unresolved problems")
    if any(any(ins.mnemonic.startswith('f') for ins in item.insns.values())
           and campaign._id(va) not in x87_contracts for va, item in lifted.items()):
        raise ValueError('experimental composition excludes x87 instructions without a dedicated parent contract')
    options = {va: composition_c_options(va, overrides) for va in selected}
    if any(o.cases <= 0 or o.budget <= 0 or not 0 < o.min_coverage <= 1 for o in options.values()):
        raise ValueError("positive cases/budget and coverage in (0, 1] are required")
    builds = {}
    with tempfile.TemporaryDirectory(prefix="cmre-composition-") as temporary:
        root = Path(temporary)
        for va in companions:
            path = root / f'fn_{va:08x}.S'
            path.write_text(lift.render(lifted[va]), encoding='utf-8', newline='\n')
            sources_by_va[va] = path
            identities[path.name] = env.digest(path)
        sources = [sources_by_va[va] for va in selected]
        for opt in ("-O0", "-O2"):
            work = root / opt[1:]
            work.mkdir()
            try:
                built = _build_separate(sources, lift.symbol(selected[0]), work, opt,
                                        lift_companions={va: lifted[va] for va in companions})
                if any(binding.owner not in selected for binding in built.image.jump_bindings):
                    raise build.BuildError('composition excludes unselected indexed-JMP owners')
                x87_transport.require_complete(built.image.x87_bindings, lifted)
                symbols = _symbols(built, selected)
            except (build.BuildError, ValueError) as exc:
                builds[opt] = {"status": "build-failed", "detail": str(exc),
                               "stderr": getattr(exc, "stderr", "")[-2000:]}
                continue
            entries = {}
            for va in selected:
                first = _run(va, lifted, built, symbols, options[va])
                second = _run(va, lifted, built, symbols, options[va])
                if first != second:
                    first = {**first, "status": "nondeterministic"}
                entries[campaign._id(va)] = first
            builds[opt] = {**built.composition_artifacts,
                           "object_sha256": built.composition_artifacts['objects'],
                           "status": "pass" if all(r["status"] == "pass" for r in entries.values()) else "fail",
                           "return_transport": return_transport.report(built.image.return_bindings),
                           "jump_transport": jump_transport.report(built.image.jump_bindings),
                           "x87_transport": x87_transport.report(built.image.x87_bindings),
                           "observed_composed_calls": any(r["observed_other_selected_entries"] for r in entries.values()),
                           "compiled_symbols": {campaign._id(v): hex(a) for v, a in sorted(symbols.items())},
                           "entries": entries}
        changed = [p.name for p in sources if env.digest(p) != identities[p.name]]
    for name, digest in legacy_snapshot.items():
        if env.digest(env.ROOT / name) != digest:
            changed.append(name + ':legacy')
    if env.digest(header) != header_identity:
        changed.append("recovery/include/cmre.h")
    if recover.execution_identity() != execution_identity:
        changed.append('execution-identity')
    if env.digest(Path(__file__)) != composition_identity:
        changed.append('composition')
    current_objdump_version = env.run_tool([str(objdump_path), '--version'])
    if (Path(env.tool('objdump')) != objdump_path or env.digest(objdump_path) != objdump_sha256
            or current_objdump_version.returncode or not current_objdump_version.stdout.splitlines()
            or current_objdump_version.stdout.splitlines()[0] != objdump_version):
        changed.append('objdump')
    for va, source in sources_by_va.items():
        contract = jump_contracts.get(campaign._id(va))
        if contract and (not (env.ROOT / contract['path']).exists()
                         or env.digest(env.ROOT / contract['path']) != contract['sha256']):
            changed.append(source.name + ':jump-contract')
        if recover.x87_contract(va) != x87_contracts.get(campaign._id(va)):
            changed.append(source.name + ':x87-contract')
        if va not in companions and recover.c_dependencies(source) != dependencies[source.name]:
            changed.append(source.name + ':dependencies')
        if composition_c_options(va, overrides).__dict__ != options[va].__dict__:
            changed.append(source.name + ':test-inputs')
    return {"schema": "cmre-composition-report/1", "input_sha256": env.TARGET_SHA256,
            "campaign": manifest, "source_sha256": identities,
            "execution_tiers": {campaign._id(va): ({'execution_tier': 'lift-companion',
                'provenance': 'fresh-original-derived-legacy', 'ledger': 'recovery/date/ledger.json',
                'source_sha256': identities[f'fn_{va:08x}.S'], 'legacy_dependencies': legacy_snapshot}
                if va in companions else {'execution_tier': 'c'}) for va in selected},
            "header_sha256": header_identity,
            "source_dependencies": dependencies, "execution_identity": execution_identity,
            "jump_contracts": jump_contracts,
            "x87_contracts": x87_contracts,
            "options": {campaign._id(v): o.__dict__ for v, o in options.items()},
            "harness_sha256": recover.harness_digest(),
            "objdump": {"path": str(objdump_path), "version": objdump_version, "sha256": objdump_sha256},
            "composition_sha256": composition_identity, "toolchain": env.toolchain_identity(),
            "status": "pass" if not changed and all(b["status"] == "pass" for b in builds.values()) else "fail",
            "observed_composed_calls_at_both_optimizations": all(b.get("observed_composed_calls", False) for b in builds.values()),
            "changed_inputs": changed, "builds": builds, "milestone_accepted": False,
            "limitations": ["Experimental composed C corpus; no ledger acceptance or live integration.",
                            "Outside callees and original image data remain dependencies; imports are scaffolding.",
                            "Sources compile separately; outside callees remain original boundaries.",
                            "A pass requires a normal return but may contain matching faults; inspect outcomes and selected-entry visits.",
                            "Compiler-selected includes are bounded to each source and recovery/include."]}


def _command(args) -> int:
    try:
        overrides = {"cases": args.cases} if args.cases is not None else None
        result = test(campaign.load(args.manifest), overrides,
                      lift_companions=getattr(args, 'lift_companion', ()))
        identity = env.digest_bytes(json.dumps(result, sort_keys=True).encode())
        path = env.ANALYSIS / 'cmre/compositions' / (identity + '.json')
        coordination.atomic_json(path, result)
        response = result if args.json else {'status': result['status'], 'report_path': env.rel(path),
                  'observed_composed_calls_at_both_optimizations': result['observed_composed_calls_at_both_optimizations'],
                  'milestone_accepted': False, 'limitations': result['limitations']}
        print(json.dumps(response, indent=2))
        return 0 if result["status"] == "pass" else 1
    except (ValueError, OSError, build.BuildError) as exc:
        print(json.dumps({"status": "error", "detail": str(exc)}))
        return 1


def add_commands(sub) -> None:
    parser = sub.add_parser("compose-test", help="experimental selected-C composition differential report")
    parser.add_argument("manifest")
    parser.add_argument("--cases", type=int)
    parser.add_argument('--lift-companion', action='append', default=[],
                        help='explicitly use a supported original-derived legacy lift companion')
    parser.add_argument('--json', action='store_true', help='print full report as well as saving it')
    parser.set_defaults(func=_command)
