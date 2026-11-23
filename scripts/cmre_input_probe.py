"""Authored entry inputs for unlifted functions: expand a compact spec, probe it, gate it.

Tooling, not harness (ADR 0013). A spec describes structured entry inputs (arena
records, stack arguments, writable globals) in a compact form. This tool expands
them into ordinary hint `fixtures` (ADR 0008 schema), runs them on the ORIGINAL
side only to report reach, and `gate` appends the fixtures that reach new
instructions to the function's current hints in a private proposal file, then
runs the unchanged lifted gate on that proposal. Nothing here writes canonical
hints, the ledger or the C directory; the input lane applies passing proposals.

    py -3.13 scripts/cmre_input_probe.py try  fn_X SPEC.json    # original-only reach per fixture
    py -3.13 scripts/cmre_input_probe.py gate fn_X SPEC.json    # proposal + full lifted gate

Spec (JSON object; unknown keys are refused):

    {"notes": "why these inputs, citing listing VAs",
     "base": {<fixture fields shared by every fixture>},
     "fixtures": [{"name": "...", "regs": {"ecx": "A+0x1000"}, "stack": ["A+0x1000", 1],
                   "template": 0, "zero": [["0x1000", "0x200"]], "dwords": {"0x1004": 5},
                   "bytes": {"0x1010": "01 02 ff"}, "ascii": {"0x1020": "Row"},
                   "globals": {"0x00ae23b4": "A+0x4000"}, "import_seed": 0,
                   "vary": [{"field": "stack.1", "values": [0, 1, 2]},
                            {"field": "dwords.0x1004", "range": [0, 8]}]}]}

Values are integers or strings: "0x10", "16", "A" / "A+0x1000" (arena address).
Arena offsets are relative to the arena base. Unset registers are 0, EFLAGS 0x202,
the stack is padded with zeros to the function's arg_count, and `vary` expands the
cartesian product of its fields (fields: regs.NAME, stack.N, dwords.OFF,
globals.VA, import_seed).
"""
from __future__ import annotations

import argparse
import copy
import hashlib
import itertools
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

TOOL_VERSION = 'input-probe/1'
PROVENANCE_KIND = 'authored-structured-entry-inputs'
MAX_EXPANDED = 600      # fixtures one spec may expand to: the gate runs 600 cases (difftest.DEFAULTS['cases'])
                        # and a fixture is only drawn among them, so more expansions can never run
MAX_SELECTED = 48       # fixtures one gate adds: each must still be drawn among ~300 fixture cases
SPEC_KEYS = {'notes', 'base', 'fixtures'}
FIXTURE_KEYS = {'name', 'regs', 'stack', 'template', 'zero', 'dwords', 'bytes', 'ascii',
                'globals', 'import_seed', 'fpcw', 'vary'}
DEFAULT_EFLAGS = 0x202


def _arena_base():
    from cmre import emu
    return emu.ARENA


def value(raw, arena=None):
    """An unsigned dword from an int or a "0x..", decimal or "A+off" string."""
    arena = _arena_base() if arena is None else arena
    if type(raw) is int:
        number = raw
    elif isinstance(raw, str):
        text = raw.replace(' ', '')
        if text.upper() == 'A':
            number = arena
        elif text[:2].upper() == 'A+':
            number = arena + int(text[2:], 0)
        else:
            number = int(text, 0)
    else:
        raise ValueError(f'value must be an integer or string, not {raw!r}')
    if not 0 <= number <= 0xFFFFFFFF:
        raise ValueError(f'value out of dword range: {raw!r}')
    return number


def _offset(raw, size):
    offset = int(raw, 0) if isinstance(raw, str) else raw
    if type(offset) is not int or not 0 <= offset < size:
        raise ValueError(f'arena offset out of range: {raw!r}')
    return offset


def _set_path(fixture, field, raw):
    head, _, tail = field.partition('.')
    if field == 'import_seed':
        fixture['import_seed'] = raw
    elif head == 'regs' and tail:
        fixture.setdefault('regs', {})[tail] = raw
    elif head == 'stack' and tail.isdigit():
        stack = fixture.setdefault('stack', [])
        index = int(tail)
        stack.extend([0] * (index + 1 - len(stack)))
        stack[index] = raw
    elif head in ('dwords', 'globals') and tail:
        fixture.setdefault(head, {})[tail] = raw
    else:
        raise ValueError(f'unsupported vary field {field!r}')


def _too_many(what):
    return (f'{what} expands to more than {MAX_EXPANDED} fixtures; the gate runs only '
            f'{MAX_EXPANDED} cases, so narrow the vary ranges (use fewer, more varied fixtures)')


def _variants(compact):
    vary = compact.get('vary', [])
    if not isinstance(vary, list):
        raise ValueError('vary must be a list')
    axes = []
    for axis in vary:
        if not isinstance(axis, dict) or set(axis) - {'field', 'values', 'range'} or 'field' not in axis:
            raise ValueError('vary entries are {"field", "values"} or {"field", "range": [lo, hi(, step)]}')
        if 'range' in axis:
            span = range(*[value(v) for v in axis['range']])
            if len(span) > MAX_EXPANDED:   # len() of a range is O(1): refuse before building it
                raise ValueError(_too_many('vary range'))
            values = list(span)
        else:
            values = list(axis.get('values') or [])
        if not values:
            raise ValueError(f'vary field {axis["field"]!r} has no values')
        axes.append((axis['field'], values))
    total = 1
    for _, values in axes:
        total *= len(values)
    if total > MAX_EXPANDED:
        raise ValueError(_too_many('vary axes'))
    base = {k: v for k, v in compact.items() if k != 'vary'}
    for combo in itertools.product(*[values for _, values in axes]):
        fixture = copy.deepcopy(base)
        for (field, _), raw in zip(axes, combo):
            _set_path(fixture, field, raw)
        yield fixture


def _merge(base, fixture):
    merged = copy.deepcopy(base)
    for key, item in fixture.items():
        if isinstance(item, dict) and isinstance(merged.get(key), dict):
            merged[key] = {**merged[key], **item}
        else:
            merged[key] = copy.deepcopy(item)
    return merged


def _encode(compact, arg_count, templates, regs_names, arena):
    """One compact fixture -> the canonical hint fixture schema (difftest._fixture_case)."""
    if set(compact) - FIXTURE_KEYS:
        raise ValueError(f'unknown fixture fields: {sorted(set(compact) - FIXTURE_KEYS)}')
    template = compact.get('template', 0)
    if type(template) is not int or not 0 <= template < len(templates):
        raise ValueError('template is out of range')
    base = templates[template]
    size = len(base)
    blob = bytearray(base)
    for region in compact.get('zero', []):
        start, length = _offset(region[0], size), value(region[1])
        if start + length > size:
            raise ValueError('zero region exceeds the arena')
        blob[start:start + length] = bytes(length)
    for key, raw in compact.get('dwords', {}).items():
        at = _offset(key, size - 3)
        struct.pack_into('<I', blob, at, value(raw, arena))
    for key, text in compact.get('bytes', {}).items():
        data = bytes.fromhex(text)
        at = _offset(key, size)
        if at + len(data) > size:
            raise ValueError('bytes exceed the arena')
        blob[at:at + len(data)] = data
    for key, text in compact.get('ascii', {}).items():
        data = text.encode('latin-1') + b'\x00'
        at = _offset(key, size)
        if at + len(data) > size:
            raise ValueError('ascii exceeds the arena')
        blob[at:at + len(data)] = data
    writes = {}
    for at in range(0, size, 4):
        if blob[at:at + 4] != base[at:at + 4]:
            writes[f'{at:x}'] = struct.unpack_from('<I', blob, at)[0]
    regs = dict.fromkeys(regs_names, 0)
    regs['eflags'] = DEFAULT_EFLAGS
    for name, raw in compact.get('regs', {}).items():
        if name not in regs:
            raise ValueError(f'unknown register {name!r}')
        regs[name] = value(raw, arena)
    stack = [value(raw, arena) for raw in compact.get('stack', [])]
    if len(stack) > arg_count:
        raise ValueError(f'stack has {len(stack)} words; this function takes arg_count {arg_count}')
    stack += [0] * (arg_count - len(stack))
    fixture = {'regs': regs, 'stack': stack, 'arena_template': template, 'arena_writes': writes,
               'globals': {f'{value(k):x}': value(v, arena) for k, v in compact.get('globals', {}).items()},
               'import_seed': value(compact.get('import_seed', 0))}
    if 'fpcw' in compact:
        fixture['fpcw'] = value(compact['fpcw'])
    return fixture


def iter_expand(spec, arg_count, templates, regs_names=None, arena=None):
    """Yield (name, canonical fixture) for every fixture and `vary` combination of a spec,
    one at a time: a fixture can carry tens of thousands of arena writes, so callers keep
    only what they need."""
    from cmre import emu
    regs_names = regs_names or emu.REG_NAMES
    arena = _arena_base() if arena is None else arena
    if not isinstance(spec, dict) or set(spec) - SPEC_KEYS or not isinstance(spec.get('fixtures'), list):
        raise ValueError(f'spec must be an object with "fixtures" and only {sorted(SPEC_KEYS)}')
    base = spec.get('base', {})
    if not isinstance(base, dict):
        raise ValueError('base must be an object')
    count = 0
    for index, compact in enumerate(spec['fixtures']):
        if not isinstance(compact, dict):
            raise ValueError('each fixture must be an object')
        merged = _merge(base, compact)
        name = str(merged.pop('name', f'f{index}'))
        for n, variant in enumerate(_variants(merged)):
            count += 1
            if count > MAX_EXPANDED:
                raise ValueError(_too_many('spec'))
            yield (name if 'vary' not in merged else f'{name}#{n}',
                   _encode(variant, arg_count, templates, regs_names, arena))
    if not count:
        raise ValueError('spec has no fixtures')


def expand(spec, arg_count, templates, regs_names=None, arena=None):
    """[(name, canonical fixture)] for a whole spec (small specs and tests)."""
    return list(iter_expand(spec, arg_count, templates, regs_names, arena))


def picked_fixtures(spec, indices, arg_count, templates, regs_names=None, arena=None):
    """The canonical fixtures at the given expansion indices, regenerated one at a time."""
    wanted = set(indices)
    found = {i: fixture for i, (_, fixture) in enumerate(iter_expand(spec, arg_count, templates, regs_names, arena))
             if i in wanted}
    return [found[i] for i in indices]


def select(rows, baseline, limit=MAX_SELECTED):
    """Greedy: fixtures that reach instructions neither the baseline nor earlier picks reach,
    plus one returning fixture when none was picked. rows: [(name, fixture, covered, outcome)]."""
    covered, picked = set(baseline), []
    order = sorted(range(len(rows)), key=lambda i: -len(rows[i][2] - covered))
    while order and len(picked) < limit:
        best = max(order, key=lambda i: (len(rows[i][2] - covered), -i))
        if not rows[best][2] - covered:
            break
        picked.append(best)
        covered |= rows[best][2]
        order.remove(best)
    if picked and not any(rows[i][3] == 'return' for i in picked):
        returning = next((i for i in range(len(rows)) if rows[i][3] == 'return' and i not in picked), None)
        if returning is not None and len(picked) < limit:
            picked.append(returning)
    return sorted(picked)


def merge_hints(current, fixtures, provenance):
    """Current hints plus appended fixtures; every existing field and fixture is kept."""
    proposal = copy.deepcopy(current)
    proposal.pop('auto', None)  # a claimed hint: automatic inference no longer rewrites it
    existing = proposal.setdefault('fixtures', [])
    for fixture in fixtures:
        if fixture not in existing:
            existing.append(fixture)
    proposal.setdefault('fixture_provenance', {})[provenance['id']] = {
        k: v for k, v in provenance.items() if k != 'id'}
    return proposal


def append_only(current, proposal):
    """True when the proposal keeps every current field, fixture and capture unchanged."""
    current = {k: v for k, v in current.items() if k != 'auto'}
    for key, item in current.items():
        if key == 'fixtures':
            if proposal.get('fixtures', [])[:len(item)] != item:
                return False
        elif key == 'fixture_provenance':
            if any(proposal.get(key, {}).get(k) != v for k, v in item.items()):
                return False
        elif proposal.get(key) != item:
            return False
    extra = set(proposal) - set(current) - {'fixtures', 'fixture_provenance'}
    return not extra


# --- execution (original side only, then the unchanged lifted gate) -----------------

def _templates(size):
    from cmre import difftest
    return [t + bytes(size - len(t)) for t in difftest.arena_templates()]


def _context(va):
    from cmre import capture_expand, difftest
    starts, region = capture_expand._function(va)
    opts, machine = capture_expand._machine(va, region)
    templates = _templates(machine.arena_size)
    return starts, opts, machine, templates


def _run_rows(va, spec, context=None):
    from cmre import difftest
    starts, opts, machine, templates = context or _context(va)
    rows = []   # (name, None, covered, outcome, detail): fixtures are regenerated for the picks only
    for name, fixture in iter_expand(spec, opts.arg_count, templates):
        case = difftest._fixture_case(fixture, templates, opts.arg_count)
        result = machine.run(va, case, opts.budget)
        rows.append((name, None, result.covered & starts, result.outcome, result.detail))
        del fixture, case, result
    return starts, opts, machine, templates, rows


def baseline_coverage(va, opts, machine, starts):
    """Instructions the current gate's own cases reach on the original (same case derivation)."""
    import random
    from cmre import difftest, image
    rng, covered, globals_in = random.Random(opts.seed), set(), set()
    templates = difftest.arena_templates()
    for index in range(opts.cases):
        case = difftest.make_case(rng, opts, sorted(globals_in), templates)
        result = machine.run(va, case, opts.budget)
        if index < opts.discovery:
            globals_in.update(difftest._global_reads(result, image.primary()))
        covered |= result.covered & starts
    return covered


def try_spec(va, spec):
    starts, opts, machine, templates, rows = _run_rows(va, spec)
    union, report = set(), []
    for name, _, covered, outcome, detail in rows:
        fresh = covered - union
        union |= covered
        report.append({'name': name, 'outcome': outcome, 'detail': str(detail)[:160],
                       'own_instructions': len(covered), 'fresh': len(fresh)})
    uncovered = sorted(starts - union)
    return {'fn': f'fn_{va:08x}', 'fixtures': len(rows), 'instructions': len(starts),
            'fixture_only_coverage': round(len(union) / len(starts), 4) if starts else 0.0,
            'threshold': opts.min_coverage, 'rows': report[:200],
            'first_uncovered': [f'{x:#010x}' for x in uncovered[:20]]}


def current_hints(va):
    from cmre import env
    path = env.HINTS_DIR / f'fn_{va:08x}.json'
    raw = path.read_bytes() if path.exists() else None
    return (json.loads(raw) if raw is not None else {}), raw


def gate_spec(va, spec, spec_bytes, proposal_path, model=None):
    """Write the appended proposal next to the spec and run the unchanged lifted gate on it."""
    from cmre import difftest, env, recover
    current, raw = current_hints(va)
    context = _context(va)
    starts, opts, machine, templates, rows = _run_rows(va, spec, context)
    baseline = baseline_coverage(va, opts, machine, starts)
    picked = select([(n, f, c, o) for n, f, c, o, _ in rows], baseline)
    provenance = {'id': f'input-lane-{hashlib.sha256(spec_bytes).hexdigest()[:12]}',
                  'kind': PROVENANCE_KIND, 'tool': TOOL_VERSION, 'target_sha256': env.TARGET_SHA256,
                  'spec_sha256': hashlib.sha256(spec_bytes).hexdigest(),
                  **({'model': model} if model else {})}
    proposal = merge_hints(current, picked_fixtures(spec, picked, opts.arg_count, templates), provenance)
    difftest.validate_hints(proposal)
    if not append_only(current, proposal):
        raise ValueError('proposal would change existing hints; only appended fixtures are allowed')
    encoded = (json.dumps(proposal, indent=1) + '\n').encode('utf-8')
    Path(proposal_path).write_bytes(encoded)
    result = {'fn': f'fn_{va:08x}', 'expanded': len(rows), 'selected': len(picked),
              'baseline_coverage': round(len(baseline) / len(starts), 4) if starts else 0.0,
              'proposal': str(proposal_path), 'proposal_sha256': hashlib.sha256(encoded).hexdigest(),
              'prior_sha256': hashlib.sha256(raw).hexdigest() if raw is not None else None}
    if not picked:
        return {**result, 'status': 'no-new-reach', 'coverage': result['baseline_coverage']}
    load_hints = difftest.load_hints
    difftest.load_hints = lambda address: (json.loads(encoded) if address == va else load_hints(address))
    try:
        report = recover.test_lifted(va, fresh=True)
    finally:
        difftest.load_hints = load_hints
    diff = report.get('difftest', {})
    return {**result, 'status': report['status'], 'coverage': diff.get('coverage'),
            'threshold': opts.min_coverage, 'mismatches': diff.get('mismatches', [])[:3],
            'outcomes': diff.get('outcomes', {}), 'first_uncovered': diff.get('uncovered', [])[:12],
            'key': report.get('key')}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest='action', required=True)
    for name, text in (('try', 'original-only reach of every fixture in a spec'),
                       ('gate', 'append the reaching fixtures to a private proposal and run the lifted gate')):
        p = sub.add_parser(name, help=text)
        p.add_argument('fn')
        p.add_argument('spec', type=Path)
        if name == 'gate':
            p.add_argument('--proposal', type=Path, help='default: SPEC with .proposal.json')
            p.add_argument('--model', help='recorded in the fixture provenance')
    a = parser.parse_args(argv)
    try:
        va = int(a.fn[3:], 16) if a.fn.startswith('fn_') else int(a.fn, 16)
        spec_bytes = a.spec.read_bytes()
        spec = json.loads(spec_bytes)
        if a.action == 'try':
            out = try_spec(va, spec)
        else:
            proposal = a.proposal or a.spec.with_suffix('.proposal.json')
            out = gate_spec(va, spec, spec_bytes, proposal, a.model)
            if a.proposal is None and out['status'] in ('pass', 'fail'):
                from cmre.coordination import atomic_bytes
                receipt = {key: out[key] for key in ('fn', 'status', 'prior_sha256')}
                receipt['spec_sha256'] = hashlib.sha256(spec_bytes).hexdigest()
                try:
                    atomic_bytes(a.spec.with_suffix('.probe-result.json'), json.dumps(receipt).encode('utf-8'))
                except OSError as exc:
                    print(f'Could not write optional probe result: {exc}', file=sys.stderr)
    except (OSError, ValueError, KeyError, TypeError, MemoryError) as exc:
        print(json.dumps({'status': 'error', 'error': f'{type(exc).__name__}: {exc}'[:600]}))
        return 2
    print(json.dumps(out))
    return 0


if __name__ == '__main__':
    sys.exit(main())
