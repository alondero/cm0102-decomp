"""Retained ADR 0010 operand evidence shared by farm and direct C acceptance."""
from __future__ import annotations

import hashlib
import json
import re

from . import coordination

_X87_BRIDGE = 'cm_x87_CIpow'
_X87_BUILD_LEVELS = ('-O0', '-O2')
# /2 (24 Nov): rows for ADR 0006 condition 4 __ftol sites beside the ADR 0010 CIpow operands.
OPERAND_SCHEMA = 'cmre-farm-x87-operands/2'

_NONCODE = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*', re.S)

def _mask_noncode(match):
    return re.sub(r'[^\r\n]', ' ', match.group())


def operand_proof_needed(raw: bytes, gate: dict) -> bool:
    """Include compiler-expanded calls even when source spelling hid the bridge, and every
    routine whose original calls __ftol (ADR 0006 condition 4)."""
    return (_X87_BRIDGE.encode() in raw
            or any(level.get('cipow_calls') for level in gate.get('builds', {}).values())
            or bool(ftol_site_count(gate)))


def ftol_site_count(gate) -> int:
    """Original __ftol call sites the gate report recorded from the lifted routine."""
    sites = gate.get('ftol_sites') if isinstance(gate, dict) else None
    return len(sites) if isinstance(sites, list) else 0


def _compiled_sites(gate: dict) -> int:
    builds = gate.get('builds', {})
    ordinary = builds.get('-O0', {}).get('cipow_calls', 0)
    # O2 may clone an expression, so count its presence if O0 removed it.
    return ordinary or int(bool(builds.get('-O2', {}).get('cipow_calls')))


def dependency_hashes(report: dict) -> dict:
    """Compiler-selected dependencies shared by the scratch and canonical units."""
    name = report.get('source')
    return {path: value for path, value in report.get('source_dependencies', {}).items() if path != name}


def _options_compatible(report: dict, baseline: dict) -> bool:
    return (report.get('effective_options_sha256') == baseline.get('effective_options_sha256')
            and dependency_hashes(report) == dependency_hashes(baseline))


def _bridge_directive_spans(text):
    """Character spans of preprocessor directives, including backslash continuations."""
    spans, start, offset = [], None, 0
    for line in text.splitlines(keepends=True):
        stripped = line.lstrip(' \t')
        if start is not None or stripped.startswith('#'):
            if start is None:
                start = offset
            if not line.rstrip('\r\n').endswith('\\'):
                spans.append((start, offset + len(line)))
                start = None
        offset += len(line)
    if start is not None:
        spans.append((start, len(text)))
    return spans


def _bridge_matching_paren(masked, opening):
    depth = 0
    for index in range(opening, len(masked)):
        char = masked[index]
        if char == '(':
            depth += 1
        elif char == ')':
            depth -= 1
            if depth == 0:
                return index
    raise ValueError('unsupported: unterminated cm_x87_CIpow call')


def _bridge_operand_spans(masked, start, end):
    """Split a call's top-level comma separators, preserving exact source offsets."""
    spans, depth, part = [], 0, start
    for index in range(start, end):
        char = masked[index]
        if char in '([{':
            depth += 1
        elif char in ')]}':
            depth -= 1
            if depth < 0:
                raise ValueError('unsupported: unbalanced brackets in cm_x87_CIpow operands')
        elif char == ',' and depth == 0:
            spans.append((part, index))
            part = index + 1
    spans.append((part, end))
    return spans


def bridge_call_sites(text):
    """Exact per-call operand spans, or ValueError when parsing is not provable.

    This is deliberately narrower than a C parser: only a direct call with two
    observable operands is inspectable. Macro aliases, directive occurrences,
    indirect references and irregular argument lists fail closed.
    """
    masked = _NONCODE.sub(_mask_noncode, text)
    directives = _bridge_directive_spans(text)
    sites = []
    for match in re.finditer(r'\b%s\b' % re.escape(_X87_BRIDGE), masked):
        if any(lo <= match.start() < hi for lo, hi in directives):
            raise ValueError('unsupported: cm_x87_CIpow inside a preprocessor directive '
                             'cannot be mutated per call')
        opening = match.end()
        while opening < len(masked) and masked[opening].isspace():
            opening += 1
        if opening >= len(masked) or masked[opening] != '(':
            raise ValueError('unsupported: cm_x87_CIpow occurrence is not a direct function call')
        closing = _bridge_matching_paren(masked, opening)
        spans = _bridge_operand_spans(masked, opening + 1, closing)
        if len(spans) != 2:
            raise ValueError(f'unsupported: cm_x87_CIpow call has {len(spans)} operand(s), expected 2')
        operands = []
        for start, end in spans:
            while start < end and masked[start].isspace():
                start += 1
            while end > start and masked[end - 1].isspace():
                end -= 1
            if start == end:
                raise ValueError('unsupported: empty cm_x87_CIpow operand')
            if '#' in masked[start:end]:
                raise ValueError('unsupported: preprocessor token inside a cm_x87_CIpow operand')
            operands.append((start, end))
        sites.append({'index': len(sites), 'line': text.count('\n', 0, match.start()) + 1,
                      'operands': operands})
    return sites


def bridge_mutant_sources(text):
    """One mutant per call operand: base + 1.0L, then exponent + 1.0L."""
    rows = []
    for site in bridge_call_sites(text):
        for operand, (start, end) in zip(('base', 'exp'), site['operands']):
            rows.append({'index': site['index'], 'line': site['line'], 'operand': operand,
                         'source': text[:start] + f'(({text[start:end]}) + 1.0L)' + text[end:]})
    return rows


# --- ADR 0006 condition 4: __ftol site observation -------------------------

FTOL_HELPER = 0x00945640
FTOL_PROBE_TAG = 'cmre-ftol-float-'
FTOL_CONST_TAG = 'cmre_ftol_const_'
FTOL_MARKER = 'cmre_ftol_mark'
_FTOL_CAST = re.compile(r'\(\s*(?:(?:signed\s+)?long\s+long(?:\s+int)?|s64|i64|int64_t|__int64)\s*\)')
# A source-local alias of the construct (`typedef long long ll;`) spells the same cast.
_LONG_LONG_TYPEDEF = re.compile(r'\btypedef\s+(?:signed\s+)?long\s+long(?:\s+int)?\s+([A-Za-z_]\w*)\s*;')
_IDENT = re.compile(r'[A-Za-z_]\w*')
_NUMBER = re.compile(r'(?:0[xX][0-9a-fA-F]+|\d+\.?\d*(?:[eE][+-]?\d+)?|\.\d+(?:[eE][+-]?\d+)?)[uUlLfF]*')
# A parenthesis after one of these keywords can still open a cast; after any other identifier it
# opens a call, a prototype or sizeof/typeof, which is not a value conversion.
_CAST_AFTER_KEYWORD = {'return', 'case', 'else', 'do'}
_TYPE_WORDS = {'void', 'char', 'short', 'int', 'long', 'float', 'double', 'signed', 'unsigned',
               'const', 'volatile', '_Bool', 'struct', 'union', 'enum'}
_TYPEDEF = re.compile(r'\btypedef\b[^;{}]*?(?:\(\s*\*\s*([A-Za-z_]\w*)\s*\)\s*\([^;]*\)|'
                      r'\b([A-Za-z_]\w*)\s*(?:\[[^\];]*\]\s*)*);', re.S)


def type_names(*texts):
    """Typedef names declared in the given C texts (the source and cmre.h)."""
    names = set()
    for text in texts:
        masked = _NONCODE.sub(_mask_noncode, text)
        # `typedef s16 alias_s16 __attribute__((may_alias));` names alias_s16
        masked = re.sub(r'__attribute__\s*\(\((?:[^()]|\([^()]*\))*\)\)', ' ', masked)
        for match in _TYPEDEF.finditer(masked):
            names.add(match.group(1) or match.group(2))
    names.discard(None)
    return names


def _is_type_name(inner, names):
    tokens = re.findall(r'[A-Za-z_]\w*|\*|\S', inner)
    if not tokens:
        return False
    tagged = False
    for token in tokens:
        if token == '*' or token in _TYPE_WORDS or token in names:
            tagged = token in ('struct', 'union', 'enum')
            continue
        if tagged and _IDENT.fullmatch(token):
            tagged = False
            continue
        return False
    return True


def _skip_space(masked, text, index):
    start = index
    while index < len(masked) and masked[index].isspace():
        index += 1
    if any(char in text[start:index] for char in '\'"'):
        raise ValueError('unsupported: a literal operand of a 64-bit cast')
    return index


def _matching(masked, opening, pair):
    depth = 0
    for index in range(opening, len(masked)):
        if masked[index] == pair[0]:
            depth += 1
        elif masked[index] == pair[1]:
            depth -= 1
            if depth == 0:
                return index
    raise ValueError(f'unsupported: unterminated {pair[0]} in a 64-bit cast operand')


def _cast_operand_span(masked, text, index, names):
    """[start, end) of the cast-expression a cast at `index` applies to."""
    index = start = _skip_space(masked, text, index)
    while index < len(masked):
        if masked.startswith(('++', '--'), index):
            index = _skip_space(masked, text, index + 2)
        elif masked[index] in '-+!~*&':
            index = _skip_space(masked, text, index + 1)
        elif masked[index] == '(':
            closing = _matching(masked, index, '()')
            if not _is_type_name(masked[index + 1:closing], names):
                break
            index = _skip_space(masked, text, closing + 1)
        else:
            break
    if index >= len(masked):
        raise ValueError('unsupported: 64-bit cast without an operand')
    word = _IDENT.match(masked, index)
    if masked[index] == '(':
        index = _matching(masked, index, '()') + 1
    elif word and word.group() in ('sizeof', '_Alignof', '__alignof__', '_Generic'):
        raise ValueError(f'unsupported: {word.group()} operand of a 64-bit cast')
    elif word:
        index = word.end()
    elif number := _NUMBER.match(masked, index):
        index = number.end()
    else:
        raise ValueError(f'unsupported: 64-bit cast operand starts with {masked[index]!r}')
    while True:
        after = _skip_space(masked, text, index)
        if after < len(masked) and masked[after] == '[':
            index = _matching(masked, after, '[]') + 1
        elif after < len(masked) and masked[after] == '(':
            index = _matching(masked, after, '()') + 1
        elif masked.startswith('->', after) or (after < len(masked) and masked[after] == '.'
                                                 and not masked[after + 1:after + 2].isdigit()):
            member = _IDENT.match(masked, _skip_space(masked, text, after + (2 if masked[after] == '-' else 1)))
            if not member:
                raise ValueError('unsupported: member access without a member name')
            index = member.end()
        elif masked.startswith(('++', '--'), after):
            index = after + 2
        else:
            return start, index


def _unbracketed_macros(masked, directives):
    """Names the source #defines whose body is not one token or one fully bracketed group.

    Such a body can rebind a cast written around its use (`#define SUM v + w` makes
    `(long long)SUM` mean `(long long)v + w`), so the source span is not the expansion's.
    A name is unsafe if any of its definitions is, so #undef and redefinition cannot launder it."""
    definitions = _header_macro_bodies() + _macro_bodies(masked, directives)
    unsafe, callers = set(), []
    for name, params, body in definitions:
        call = re.match(r'([A-Za-z_]\w*)\s*\(', body)
        if not body or re.fullmatch(r'[0-9][\w.]*', body):
            continue
        if re.fullmatch(r'[A-Za-z_]\w*', body):
            if body in params:
                unsafe.add(name)   # `#define ID(x) x` passes its argument's text through
            else:
                callers.append((name, body))   # an alias is as bracketed as what it names
            continue
        if body.startswith('(') and _closes_at(body, 0) == len(body) - 1:
            continue
        if call and _closes_at(body, call.end() - 1) == len(body) - 1:
            if call.group(1) not in params:   # a parameter callee is one call whatever it names
                callers.append((name, call.group(1)))   # `NAME(...)`: one postfix expression if NAME is
            continue
        unsafe.add(name)
    changed = True
    while changed:   # a call of an unbracketed macro is itself unbracketed
        changed = False
        for name, callee in callers:
            if callee in unsafe and name not in unsafe:
                unsafe.add(name)
                changed = True
    return unsafe


def _macro_bodies(masked, directives):
    """Every #define as (name, parameter names, body), in source order."""
    definitions = []
    for start, end in directives:
        directive = re.sub(r'\\r?\n', ' ', masked[start:end])
        match = re.match(r'\s*#\s*define\s+([A-Za-z_]\w*)(\([^)]*\))?(.*)', directive, re.S)
        if match:
            params = frozenset(_IDENT.findall(match.group(2) or ''))
            definitions.append((match.group(1), params, match.group(3).strip()))
    return definitions


_HEADER_MACROS = None


def _header_macro_bodies():
    # Cached per process: a header change alters the execution identity and needs a restart.
    global _HEADER_MACROS
    if _HEADER_MACROS is None:
        from . import env
        path = env.ROOT / 'recovery/include/cmre.h'
        text = path.read_text(encoding='utf-8') if path.exists() else ''
        masked = _NONCODE.sub(_mask_noncode, text)
        _HEADER_MACROS = _macro_bodies(masked, _bridge_directive_spans(text))
    return _HEADER_MACROS


def _closes_at(body, index):
    depth = 0
    for position in range(index, len(body)):
        depth += {'(': 1, ')': -1}.get(body[position], 0)
        if not depth:
            return position
    return -1


def ftol_cast_sites(text, names=()):
    """Every explicit signed 64-bit cast in code, with the exact span of its operand.

    The ADR 0006 construct for an __ftol site is `(long long)x`. A cast inside a preprocessor
    directive cannot be mutated per use and fails closed, as do operands this narrow parser
    cannot delimit; `ftol_float_casts` then keeps only the casts of floating operands."""
    masked = _NONCODE.sub(_mask_noncode, text)
    directives = _bridge_directive_spans(text)
    unbracketed = _unbracketed_macros(masked, directives)
    names = set(names) | type_names(text)
    aliases = sorted(set(_LONG_LONG_TYPEDEF.findall(masked)))
    pattern = _FTOL_CAST if not aliases else re.compile(
        _FTOL_CAST.pattern + '|' + r'\(\s*(?:%s)\s*\)' % '|'.join(map(re.escape, aliases)))
    sites = []
    for match in pattern.finditer(masked):
        before = masked[:match.start()].rstrip()
        word = re.search(r'[A-Za-z_]\w*$', before)
        if word and word.group() not in _CAST_AFTER_KEYWORD:
            continue
        if any(lo <= match.start() < hi for lo, hi in directives):
            raise ValueError('unsupported: a 64-bit cast inside a preprocessor directive cannot be '
                             'mutated per __ftol site')
        start, end = _cast_operand_span(masked, text, match.end(), names)
        rebinding = sorted(set(_IDENT.findall(masked[start:end])) & unbracketed)
        if rebinding:
            raise ValueError(f'unsupported: 64-bit cast operand uses macro {rebinding[0]}, whose body is not '
                             'one bracketed expression; bracket the macro body or expand it')
        sites.append({'index': len(sites), 'line': text.count('\n', 0, match.start()) + 1,
                      'cast': (match.start(), match.end()), 'operand': (start, end)})
    return sites


def ftol_probe_source(text, sites):
    """The source with each cast operand wrapped in a compile-time floating-type assertion.

    GCC reports `static assertion failed: "cmre-ftol-float-N"` for every cast whose operand
    is floating, and -Wfloat-conversion reports implicit float-to-integer conversions."""
    inserts = []
    for site in sites:
        start, end = site['operand']
        inserts += [(start, 0, f"CMRE_FTOL_PROBE({site['index']}, ("), (end, 1, '))')]
    out, last = [], 0
    for position, _, piece in sorted(inserts, key=lambda row: (row[0], -row[1])):
        out += [text[last:position], piece]
        last = position
    out.append(text[last:])
    floating = '_Generic((x), float: 1, double: 1, long double: 1, default: 0)'
    # GCC leaves __builtin_constant_p of a local to the optimizer, so the constant check is a
    # block-scope array type: unresolved it is a VLA, a floating constant makes its size negative.
    header = (f'#define CMRE_FTOL_PROBE(k, x) ({{ _Static_assert(!{floating}, "{FTOL_PROBE_TAG}" #k); '
              f'typedef char {FTOL_CONST_TAG}##k[1 - 2 * ({floating} && __builtin_constant_p(x))]; (x); }})\n')
    return header + ''.join(out)


def ftol_marker_source(text, floating):
    """The source with each floating (long long) cast replaced by an opaque marker call.

    Its -O0 build must contain no x87 integer store: any that remains is a float-to-integer
    conversion this proof does not mutate, however it is spelled."""
    edits = []
    for site in floating:
        cast_start, cast_end = site['cast']
        edits += [(cast_start, cast_end, f'{FTOL_MARKER}('), (site['operand'][1], site['operand'][1], ')')]
    out, last = [], 0
    for start, end, piece in sorted(edits, key=lambda row: (row[0], row[1] - row[0])):
        out += [text[last:start], piece]
        last = end
    out.append(text[last:])
    return f'extern long long {FTOL_MARKER}(long double);\n' + ''.join(out)


_CONVERSION = re.compile(r"conversion from '[^']+'(?: \{aka '[^']+'\})? to '([^']+)'(?: \{aka '([^']+)'\})?")


def ftol_probe_result(returncode, stderr, sites):
    """Indexes of floating-operand casts from probe diagnostics; ValueError on anything else.

    The assertions are the only expected errors, so the compile fails exactly when some cast is
    floating. Any other outcome (a compiler that did not start, an unparsed error) fails closed."""
    floating, problems = set(), []
    for line in stderr.splitlines():
        if 'Wfloat-conversion' in line:
            conversion = _CONVERSION.search(line)
            target = (conversion.group(2) or conversion.group(1)) if conversion else ''
            if conversion and re.search(r'\b(?:float|double)\b', target):
                continue   # floating narrowing (an fstp store), not an __ftol site
            problems.append('implicit float-to-integer conversion; write each __ftol site as an '
                            f'explicit (long long) cast: {line.strip()[:200]}')
        elif ' error: ' in f' {line} ' or line.startswith('error:') or ': error:' in line:
            tag = re.search(re.escape(FTOL_PROBE_TAG) + r'"?\s*"?(\d+)', line)
            constant = re.search(r"size of array '%s(\d+)' is negative" % re.escape(FTOL_CONST_TAG), line)
            if constant:
                index = int(constant.group(1))
                where = next((f" at line {site['line']}" for site in sites if site['index'] == index), '')
                problems.append(f'(long long) cast of a floating constant{where} is folded at compile '
                                'time, not an __ftol site')
            elif tag and 'static assertion failed' in line:
                floating.add(int(tag.group(1)))
            else:
                problems.append(f'probe compile error: {line.strip()[:200]}')
    if problems:
        raise ValueError('unsupported: ' + problems[0])
    if not floating <= {site['index'] for site in sites}:
        raise ValueError('unsupported: probe reported an unknown cast index')
    if bool(returncode) != bool(floating):
        raise ValueError(f'unsupported: probe compile exited {returncode} with '
                         f'{len(floating)} floating-cast diagnostic(s): {stderr.strip()[:200]!r}')
    return floating


def ftol_mutant_sources(text, probe, required, names=()):
    """One mutant per floating-operand 64-bit cast (operand + 1.0L), or ValueError.

    `probe(source_text, kind)` is `_default_probe` in production. With every floating cast
    replaced by an opaque marker call, the -O0 build must hold no x87 integer store, so every
    conversion in the source is a mutated cast; a cast of a floating constant fails closed.
    `required` is the number of __ftol call sites in the original routine: fewer floating
    casts means some site is not individually written (for example a shared helper) and
    cannot be shown observed per site. `names` adds typedef names from included headers."""
    required = ftol_sites_in_source(text, required)
    if not required:
        return []
    sites = ftol_cast_sites(text, names)
    floating = ftol_float_casts(text, sites, probe)
    hidden = ftol_emitted_conversions(ftol_marker_source(text, floating), probe)
    if hidden:
        raise ValueError(f'{hidden} float-to-integer conversion(s) in the -O0 build are not floating '
                         '(long long) casts; write every float-to-integer conversion as its own '
                         '(long long) cast')
    if len(floating) < required:
        raise ValueError(f'{len(floating)} floating 64-bit cast(s) for {required} original __ftol '
                         'call site(s); write each site as its own (long long) cast')
    rows = []
    for ordinal, site in enumerate(floating):
        start, end = site['operand']
        rows.append({'index': ordinal, 'line': site['line'], 'operand': 'ftol',
                     'source': text[:start] + f'(({text[start:end]}) + 1.0L)' + text[end:]})
    return rows


def ftol_sites_in_source(text, original_sites):
    """Original __ftol sites the source must write as casts.

    A site that still calls the original helper as an explicit boundary (`cm_fn_00945640()`,
    ADR 0010 x87 transport) converts in original code: nothing in the source to observe."""
    masked = _NONCODE.sub(_mask_noncode, text)
    boundary = 0
    for match in re.finditer(r'\bcm_fn_%08x\s*\(\s*\)' % FTOL_HELPER, masked):
        word = re.search(r'[A-Za-z_]\w*$', masked[:match.start()].rstrip())
        if word and word.group() not in _CAST_AFTER_KEYWORD:
            continue   # `long long cm_fn_00945640();` declares; it converts nothing
        boundary += 1
    return max(0, original_sites - boundary)


def ftol_mutants_required(text, gate):
    """True when acceptance needs __ftol site mutants for this source."""
    return bool(ftol_sites_in_source(text, ftol_site_count(gate)))


def ftol_float_casts(text, sites, probe):
    if not sites:
        return []
    floating = ftol_probe_result(*probe(ftol_probe_source(text, sites), 'types'), sites)
    return [site for site in sites if site['index'] in floating]


def _default_probe(text, kind='types'):
    """`types`: (exit code, diagnostics) of the type probe. `fist`: (exit code, -O0 assembly, diagnostics)."""
    import tempfile
    from pathlib import Path
    from . import build
    with tempfile.TemporaryDirectory(prefix='cmre-ftol-') as tmp:
        if kind == 'fist':
            return build.assembly_probe(text, Path(tmp))
        return build.syntax_probe(text, Path(tmp))


# Under -m32 -mno-sse3 GCC lowers every float-to-integer conversion to an x87 integer store,
# whatever the source spelling (macro, typedef, enum, typeof, compound literal, inline asm,
# which may put it anywhere on a line).
_FIST = re.compile(r'\bfistt?p?(?:s|l|ll|q)?\b')


def ftol_emitted_conversions(text, probe):
    """x87 integer stores (fist/fistp/fisttp) in the source's -O0 build: its float-to-integer conversions."""
    returncode, assembly, stderr = probe(text, 'fist')
    if returncode:
        raise ValueError(f'unsupported: -O0 conversion census did not compile: {stderr.strip()[:200]!r}')
    return len(_FIST.findall(assembly))


def _header_type_names():
    from . import env
    return type_names((env.ROOT / 'recovery/include/cmre.h').read_text(encoding='utf-8'))


def operand_mutant_sources(text, gate, probe=None, names=None):
    """(CIpow rows, __ftol rows) for this source against the gate's original site census."""
    bridge = bridge_mutant_sources(text)
    required = ftol_site_count(gate)
    if not required:
        return bridge, []
    ftol = ftol_mutant_sources(text, probe or _default_probe, required,
                               _header_type_names() if names is None else names)
    return bridge, ftol


def describe_mutant(row):
    if row['operand'] == 'ftol':
        return f"__ftol site mutant {row['index'] + 1} (line {row['line']})"
    return f"x87 operand mutant {row['operand']} for call {row['index'] + 1} (line {row['line']})"


def mutant_stem(row):
    if row['operand'] == 'ftol':
        return f"ftol-{row['index']:03d}"
    return f"call-{row['index']:03d}-{row['operand']}"


def _baseline_inconclusive(baseline, opt):
    """The passing baseline's own inconclusive count at opt, or 0 when it grants no allowance."""
    if not isinstance(baseline, dict) or baseline.get('status') != 'pass':
        return 0
    build = (baseline.get('builds') or {}).get(opt)
    diff = build.get('difftest') if isinstance(build, dict) else None
    count = diff.get('inconclusive') if isinstance(diff, dict) else None
    return count if type(count) is int and count > 0 else 0


def bridge_mutant_outcome(report, baseline=None):
    """'killed' only for a differential witness in both ordinary builds.

    Cases the passing baseline already leaves inconclusive (execution budget on
    both sides) are not new uncertainty: a mutant may carry up to that many at
    each build, but still needs a concrete non-budget witness at both."""
    if not isinstance(report, dict):
        return 'unresolved', 'gate-report-not-an-object'
    if report.get('problems'):
        return 'unresolved', 'gate-problems'
    builds = report.get('builds')
    if not isinstance(builds, dict) or set(builds) != set(_X87_BUILD_LEVELS):
        return 'unresolved', 'optimization-builds-missing'
    if report.get('status') == 'pass':
        return 'survivor', 'differential-mismatch-not-observed'
    if report.get('status') != 'fail':
        return 'unresolved', f"gate-status-{report.get('status')}"
    for opt in _X87_BUILD_LEVELS:
        build = builds[opt]
        if not isinstance(build, dict):
            return 'unresolved', f'{opt}-build-record-missing'
        if build.get('status') == 'build-failed':
            return 'unresolved', f'{opt}-build-failed'
        if build.get('status') != 'fail':
            return 'unresolved', f"{opt}-gate-status-{build.get('status')}"
        diff = build.get('difftest')
        if not isinstance(diff, dict):
            return 'unresolved', f'{opt}-difftest-missing'
        inconclusive = diff.get('inconclusive')
        if type(inconclusive) is not int or inconclusive < 0:
            return 'unresolved', f'{opt}-inconclusive-counter-missing'
        allowed = _baseline_inconclusive(baseline, opt)
        if inconclusive and not allowed:
            return 'unresolved', f'{opt}-inconclusive-{inconclusive}'
        if inconclusive > allowed:
            return 'unresolved', f'{opt}-inconclusive-{inconclusive}-exceeds-baseline-{allowed}'
        mismatches = diff.get('mismatches')
        if not isinstance(mismatches, list) or not mismatches:
            return 'unresolved', f'{opt}-mismatch-missing'
        if not any(isinstance(m, dict) and type(m.get('case')) is int
                   and m.get('kind') != 'budget-asymmetry' for m in mismatches):
            return 'unresolved', f'{opt}-witness-missing'
    return 'killed', 'differential-mismatch-both-optimizations'


def bridge_report_identity_problems(report, source_sha256, expected_va, *,
                                    profile=None, execution_identity=None, baseline=None):
    """Require the real gate report's own identity fields, not summary claims."""
    if not isinstance(report, dict):
        return ['report is not an object']
    problems = []
    if report.get('va') != expected_va:
        problems.append(f"va={report.get('va')!r}")
    if report.get('tier') != 'c':
        problems.append(f"tier={report.get('tier')!r}")
    report_profile = report.get('profile')
    if not isinstance(report_profile, str) or not report_profile:
        problems.append('profile missing')
    elif profile is not None and report_profile != profile:
        problems.append(f'profile={report_profile!r} differs from baseline')
    if report.get('source_sha256') != source_sha256:
        problems.append('source_sha256 differs from the retained source')
    identity = report.get('execution_identity')
    if not isinstance(identity, dict) or not identity:
        problems.append('execution_identity missing')
    elif execution_identity is not None and identity != execution_identity:
        problems.append('execution_identity differs from baseline')
    if baseline is not None and not _options_compatible(report, baseline):
        problems.append('effective C options or compiler dependencies differ from baseline')
    key = report.get('key')
    if not isinstance(key, str) or not re.fullmatch(r'[0-9a-f]{16}', key):
        problems.append('gate key missing')
    return problems


def valid_execution_identity(identity):
    """Recognize the live recover.execution_identity contract, not a receipt claim."""
    def text(value):
        return isinstance(value, str) and bool(value)

    def sha(value):
        return isinstance(value, str) and re.fullmatch(r'[0-9a-f]{64}', value) is not None

    if not isinstance(identity, dict):
        return False
    if not all(sha(identity.get(key)) for key in ('target', 'harness')):
        return False
    if not text(identity.get('python')):
        return False
    for key in ('inventory', 'headers'):
        values = identity.get(key)
        if (not isinstance(values, dict) or not values
                or not all(text(name) and sha(value) for name, value in values.items())):
            return False
    dependencies = identity.get('dependencies')
    if (not isinstance(dependencies, dict)
            or not all(text(dependencies.get(name)) for name in ('capstone', 'unicorn', 'pefile'))):
        return False
    toolchain = identity.get('toolchain')
    if not isinstance(toolchain, dict):
        return False
    for name in ('gcc', 'as', 'ld', 'nm'):
        tool = toolchain.get(name)
        if (not isinstance(tool, dict) or not text(tool.get('version'))
                or not sha(tool.get('sha256'))):
            return False
    return True


def generate(summary_path, fn, job_number, text_bytes, baseline, hints_sha, *, gate, check_inputs, report_event,
             probe=None):
    """ADR 0010 condition 3 before any canonical install or acceptance.

    Each cm_x87_CIpow call is copied twice, once with +1.0L on the base and
    once on the exponent. Every mutant must show a differential mismatch at
    both ordinary builds. Reports and sources are bound by SHA-256; exact
    decoded raw bytes drive generation so CRLF is preserved. Survivors,
    build failures, inconclusive runs, unbound reports and source/hint drift
    are BLOCKERs, and all received evidence stays under RUN/x87-mutants/.

    ADR 0006 condition 4 adds one mutant per floating (long long) cast when the original
    calls __ftol (operand + 1.0L), under the same kill rule."""
    if not operand_proof_needed(text_bytes, baseline):
        return None
    encoded = text_bytes
    digest = hashlib.sha256(encoded).hexdigest()
    directory = summary_path.parent
    directory.mkdir(parents=True, exist_ok=True)
    summary = {'schema': OPERAND_SCHEMA, 'fn': fn, 'job': job_number,
               'candidate_sha256': digest, 'hints_sha256': hints_sha,
               'baseline': None, 'mutants': [], 'status': 'running'}

    def retained(detail):
        summary.update(status='blocked', detail=detail)
        coordination.atomic_json(summary_path, summary)
        report_event(summary['status'])
        return detail

    try:
        text = encoded.decode('utf-8')
    except UnicodeDecodeError as exc:
        return retained(f'BLOCKER: x87 operand safeguard: source is not UTF-8: {exc}; '
                        f'evidence {summary_path}')
    try:
        bridge, ftol = operand_mutant_sources(text, baseline, probe)
    except ValueError as exc:
        return retained(f'BLOCKER: x87 operand safeguard: {exc}; evidence {summary_path}')
    mutants = bridge + ftol
    if len(bridge) // 2 < _compiled_sites(baseline):
        return retained(f'BLOCKER: x87 compiled bridge calls lack individually parsed source calls; evidence {summary_path}')
    if not mutants:
        return None
    expected_va = '0x' + fn[3:]
    baseline_problems = bridge_report_identity_problems(baseline, digest, expected_va)
    if not isinstance(baseline, dict):
        baseline_problems.append('baseline report is not an object')
    else:
        if baseline.get('status') != 'pass':
            baseline_problems.append(f"baseline status={baseline.get('status')!r}")
        if baseline.get('problems'):
            baseline_problems.append('baseline gate problems')
    if baseline_problems:
        return retained('BLOCKER: x87 operand safeguard: baseline report is not bound: '
                        + '; '.join(baseline_problems) + f'; evidence {summary_path}')
    profile, execution = baseline.get('profile'), baseline.get('execution_identity')
    baseline_source = directory / 'baseline.c'
    baseline_report = directory / 'baseline.json'
    coordination.atomic_bytes(baseline_source, encoded)
    coordination.atomic_json(baseline_report, baseline)
    summary['baseline'] = {
        'source': baseline_source.name, 'source_sha256': digest,
        'report': baseline_report.name,
        'report_sha256': hashlib.sha256(baseline_report.read_bytes()).hexdigest(),
        'gate_key': baseline.get('key')}
    for row in mutants:
        stem = mutant_stem(row)
        mutant_path = directory / f'{stem}.c'
        report_path = directory / f'{stem}.json'
        coordination.atomic_bytes(mutant_path, row['source'].encode('utf-8'))
        gate_report = {'status': 'error', 'problems': ['gate did not return a report']}
        try:
            gate_report = gate(mutant_path)
        except (Exception, SystemExit) as exc:
            gate_report = {'status': 'error', 'problems': [f'{type(exc).__name__}: {exc}']}
        outcome, reason = bridge_mutant_outcome(gate_report, baseline)
        source_sha = hashlib.sha256(mutant_path.read_bytes()).hexdigest()
        identity_problems = bridge_report_identity_problems(
            gate_report, source_sha, expected_va, profile=profile, execution_identity=execution, baseline=baseline)
        if identity_problems:
            outcome, reason = 'unresolved', 'report-identity: ' + '; '.join(identity_problems)
        coordination.atomic_json(report_path, gate_report)
        row.update(source=mutant_path.name, source_sha256=source_sha,
                   report=report_path.name,
                   report_sha256=hashlib.sha256(report_path.read_bytes()).hexdigest(),
                   gate_key=gate_report.get('key') if isinstance(gate_report, dict) else None,
                   outcome=outcome, reason=reason)
        summary['mutants'].append(row)
        coordination.atomic_json(summary_path, summary)
        if detail := check_inputs():
            return retained(f'BLOCKER: x87 operand mutant evidence invalid: {detail}; evidence {summary_path}')
    blocked = [row for row in summary['mutants'] if row['outcome'] != 'killed']
    if blocked:
        row = blocked[0]
        return retained(
            f"BLOCKER: {describe_mutant(row)} {row['outcome']}: {row['reason']}; evidence {summary_path}")
    summary['status'] = 'killed'
    coordination.atomic_json(summary_path, summary)
    report_event('killed')
    return None

def verify_receipt(path, raw, *, fn, job_number, hints_sha, current_execution, active_gate=None, probe=None):
    text = raw.decode('utf-8')
    digest = hashlib.sha256(raw).hexdigest()
    expected_va = '0x' + fn[3:]
    try:
        summary = json.loads(path.read_bytes())
    except (OSError, ValueError) as exc:
        return f'BLOCKER: x87 operand evidence missing or unreadable for acceptance: {path}: {exc}'
    if (not isinstance(summary, dict)
            or summary.get('schema') != OPERAND_SCHEMA
            or summary.get('fn') != fn or summary.get('job') != job_number
            or summary.get('candidate_sha256') != digest
            or summary.get('hints_sha256') != hints_sha
            or summary.get('status') != 'killed'):
        return f'BLOCKER: x87 operand evidence is not a killed intact receipt: {path}'
    baseline = summary.get('baseline')
    if (not isinstance(baseline, dict)
            or baseline.get('source') != 'baseline.c'
            or baseline.get('report') != 'baseline.json'):
        return f'BLOCKER: x87 operand evidence has a malformed baseline record: {path}'
    baseline_source, baseline_report = path.parent / 'baseline.c', path.parent / 'baseline.json'
    if not baseline_source.is_file() or not baseline_report.is_file():
        return f'BLOCKER: x87 operand baseline evidence is missing: {path}'
    baseline_bytes = baseline_source.read_bytes()
    baseline_sha = hashlib.sha256(baseline_bytes).hexdigest()
    if (baseline_bytes != raw or baseline_sha != digest
            or baseline.get('source_sha256') != digest):
        return f'BLOCKER: x87 operand baseline source does not match the candidate: {path}'
    report_bytes = baseline_report.read_bytes()
    if hashlib.sha256(report_bytes).hexdigest() != baseline.get('report_sha256'):
        return f'BLOCKER: x87 operand baseline report bytes changed: {path}'
    baseline_evidence = json.loads(report_bytes)
    if baseline_evidence.get('status') != 'pass' or baseline_evidence.get('problems'):
        return f'BLOCKER: x87 operand baseline report is not a passing gate: {path}'
    identity_problems = bridge_report_identity_problems(baseline_evidence, digest, expected_va)
    if identity_problems:
        return ('BLOCKER: x87 operand baseline report is not bound: '
                + '; '.join(identity_problems) + f': {path}')
    profile, execution = baseline_evidence.get('profile'), baseline_evidence.get('execution_identity')
    try:
        execution_now = current_execution()
    except (Exception, SystemExit) as exc:
        return (f'BLOCKER: x87 operand current execution identity unavailable: '
                f'{type(exc).__name__}; evidence {path}')
    if not valid_execution_identity(execution_now):
        return f'BLOCKER: x87 operand current execution identity is malformed: {path}'
    if execution != execution_now:
        return f'BLOCKER: x87 operand evidence differs from current execution identity: {path}'
    if active_gate is not None:
        problems = bridge_report_identity_problems(
            active_gate, digest, expected_va, profile=profile,
            execution_identity=execution, baseline=baseline_evidence,
        )
        if problems:
            return ('BLOCKER: x87 operand proof differs from current C gate: '
                    + '; '.join(problems) + f': {path}')
    try:
        bridge, ftol = operand_mutant_sources(text, active_gate or baseline_evidence, probe)
    except ValueError as exc:
        return f'BLOCKER: x87 operand safeguard: {exc}; evidence {path}'
    expected = bridge + ftol
    if len(bridge) // 2 < _compiled_sites(active_gate or baseline_evidence):
        return f'BLOCKER: x87 compiled bridge calls lack individually parsed source calls; evidence {path}'
    rows = summary.get('mutants')
    if not isinstance(rows, list) or len(rows) != len(expected):
        return f'BLOCKER: x87 operand evidence has missing or extra mutant rows: {path}'
    by_key = {}
    for row in rows:
        if not isinstance(row, dict):
            return f'BLOCKER: x87 operand evidence has a malformed mutant row: {path}'
        key = (row.get('index'), row.get('operand'))
        if key in by_key:
            return f'BLOCKER: x87 operand evidence has duplicate mutant rows: {path}'
        by_key[key] = row
    if set(by_key) != {(row['index'], row['operand']) for row in expected}:
        return f'BLOCKER: x87 operand evidence does not cover every call operand: {path}'
    for expected_row in expected:
        row = by_key[(expected_row['index'], expected_row['operand'])]
        stem = mutant_stem(expected_row)
        if row.get('line') != expected_row['line']:
            return f'BLOCKER: x87 operand evidence line changed for {stem}: {path}'
        if row.get('source') != f'{stem}.c' or row.get('report') != f'{stem}.json':
            return f'BLOCKER: x87 operand evidence names changed for {stem}: {path}'
        source_path, report_path = path.parent / row['source'], path.parent / row['report']
        if not source_path.is_file() or not report_path.is_file():
            return f'BLOCKER: x87 operand evidence files are missing for {stem}: {path}'
        source_bytes = source_path.read_bytes()
        expected_bytes = expected_row['source'].encode('utf-8')
        if source_bytes != expected_bytes:
            return f'BLOCKER: x87 operand evidence source does not regenerate for {stem}: {path}'
        source_sha = hashlib.sha256(source_bytes).hexdigest()
        if row.get('source_sha256') != source_sha:
            return f'BLOCKER: x87 operand evidence source hash changed for {stem}: {path}'
        report_bytes = report_path.read_bytes()
        if hashlib.sha256(report_bytes).hexdigest() != row.get('report_sha256'):
            return f'BLOCKER: x87 operand evidence report bytes changed for {stem}: {path}'
        report = json.loads(report_bytes)
        outcome, reason = bridge_mutant_outcome(report, baseline_evidence)
        if outcome != 'killed' or row.get('outcome') != 'killed':
            return (f'BLOCKER: x87 operand evidence is not a kill for {stem}: '
                    f'{outcome}: {reason}; {path}')
        identity_problems = bridge_report_identity_problems(
            report, source_sha, expected_va, profile=profile, execution_identity=execution, baseline=baseline_evidence)
        if identity_problems:
            return ('BLOCKER: x87 operand report is not bound for '
                    f'{stem}: ' + '; '.join(identity_problems) + f': {path}')
    return None
