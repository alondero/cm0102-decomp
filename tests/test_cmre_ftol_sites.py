"""ADR 0006 condition 4: per-site __ftol mutants (cmre.operand_evidence)."""
import tempfile
from pathlib import Path

import pytest

from cmre import build, env, operand_evidence as oe

# The type probe compiles with the pinned toolchain; without it these checks are unavailable.
needs_gcc = pytest.mark.skipif(not Path(env.tool('gcc')).exists(), reason='pinned MinGW gcc unavailable')


def probe(text, kind='types'):
    with tempfile.TemporaryDirectory() as tmp:
        if kind == 'fist':
            return build.assembly_probe(text, Path(tmp))
        return build.syntax_probe(text, Path(tmp))


def operands(text, names=()):
    return [text[s['operand'][0]:s['operand'][1]] for s in oe.ftol_cast_sites(text, names)]


@pytest.mark.parametrize('expression,operand', [
    ('(long long)v', 'v'),
    ('(long long)v * 2', 'v'),
    ('(long long) p->f[i].g', 'p->f[i].g'),
    ('(long long)scale(x, y)', 'scale(x, y)'),
    ('(long long)(a + b) - 1', '(a + b)'),
    ('(long long)(long double)x * 3', '(long double)x'),
    ('(long long)(f80)x', '(f80)x'),
    ('(long long)-x', '-x'),
    ('(long long)x++', 'x++'),
    ('(long long)1.5L', '1.5L'),
    ('(signed long long int)x', 'x'),
    ('(s64)x', 'x'),
])
def test_cast_operand_is_the_cast_expression_it_binds(expression, operand):
    assert operands(f'void f(void) {{ y = {expression}; }}\n', {'f80'}) == [operand]


def test_a_local_long_long_typedef_spells_the_same_cast():
    text = 'typedef long long ll;\nint f(long double v) { return (int)(ll)v; }\n'
    assert operands(text) == ['v']


def test_a_typedef_with_a_trailing_attribute_is_a_type_name():
    # fn_004de5d0: `(s32)*(volatile alias_s16 *)e_home` broke the probe.
    text = ('typedef s16 alias_s16 __attribute__((may_alias));\n'
            'u32 f(u32 e) { return (u32)(long long)*(volatile alias_s16 *)e; }\n')
    assert operands(text) == ['*(volatile alias_s16 *)e']


def test_casts_nest_and_chain():
    text = 'int f(long double t) { return (s16)(long long)((long long)t * 2.0L); }\n'
    assert operands(text) == ['((long long)t * 2.0L)', 't']


def test_prototypes_sizeof_calls_and_noncode_are_not_casts():
    text = ('void g(long long);\n'
            '/* (long long)comment */\n'
            'const char *s = "(long long)s";\n'
            'int f(void) { return sizeof(long long) * n + g(long long_value); }\n')
    assert operands(text) == []


def test_a_cast_after_return_or_case_is_still_a_cast():
    text = 'long long f(long double v) { switch (k) { case 1: return (long long)v; } return 0; }\n'
    assert operands(text) == ['v']


@pytest.mark.parametrize('text,needle', [
    ('#define FTOL(x) ((long long)(x))\nint f(long double v) { return FTOL(v); }\n', 'preprocessor'),
    ("int f(void) { return (long long)'a'; }\n", 'literal'),
    ('int f(void) { return (long long)sizeof v; }\n', 'sizeof'),
    ('int f(void) { return (long long)(v; }\n', 'unterminated'),
])
def test_unparseable_casts_fail_closed(text, needle):
    with pytest.raises(ValueError, match=needle):
        oe.ftol_cast_sites(text)


SOURCE = ('#include "cmre.h"\n'
          's32 cm_fn_00401000(u32 a, s32 n)\n'
          '{\n'
          '    long double v = (long double)a * 1.5L;\n'
          '    long long wide = (long long)n;          /* integer widening: not an __ftol site */\n'
          '    return (s32)((long long)v + wide);  /* __ftol at 0x00401010 */\n'
          '}\n')


@needs_gcc
def test_only_floating_operand_casts_become_mutants():
    rows = oe.ftol_mutant_sources(SOURCE, probe, 1)
    assert [(row['index'], row['line'], row['operand']) for row in rows] == [(0, 6, 'ftol')]
    assert '(long long)((v) + 1.0L)' in rows[0]['source']
    assert '(long long)n;' in rows[0]['source']


def test_no_original_site_needs_no_mutant_and_no_probe():
    assert oe.ftol_mutant_sources(SOURCE, None, 0) == []


@needs_gcc
def test_fewer_floating_casts_than_original_sites_fails_closed():
    with pytest.raises(ValueError, match='1 floating 64-bit cast.*2 original __ftol'):
        oe.ftol_mutant_sources(SOURCE, probe, 2)


@needs_gcc
def test_a_site_still_calling_the_original_helper_needs_no_mutant():
    text = SOURCE.replace('return (s32)', 'return (s32)cm_fn_00945640() + (s32)') \
        .replace('#include "cmre.h"\n', '#include "cmre.h"\nextern long long cm_fn_00945640(void);\n')
    assert len(oe.ftol_mutant_sources(text, probe, 2)) == 1


@needs_gcc
def test_implicit_float_to_integer_conversion_fails_closed():
    text = SOURCE.replace('return (s32)((long long)v + wide);', 's32 r = v; return r + (s32)(long long)v;')
    with pytest.raises(ValueError, match='implicit float-to-integer'):
        oe.ftol_mutant_sources(text, probe, 1)


@pytest.mark.parametrize('result', [(1, ''), (0, 'x.c:1:1: error: static assertion failed: "cmre-ftol-float-0"'),
                                    (1, 'x.c:3:5: error: expected ; before }')])
def test_probe_outcomes_that_do_not_explain_themselves_fail_closed(result):
    # (1, ''): on 24 Nov a probe run without the toolchain on PATH exited 1 silently.
    sites = oe.ftol_cast_sites(SOURCE)
    with pytest.raises(ValueError, match='unsupported'):
        oe.ftol_probe_result(*result, sites)


def test_an_empty_bracket_declaration_of_the_helper_cancels_no_site():
    # Review: `long long cm_fn_00945640();` (103 uses in 60 sources) matched the call pattern.
    text = ('long long cm_fn_00945640();\n'
            'extern long long __cdecl cm_fn_00945640 ( );\n'
            'u32 f(void) { u32 a = cm_fn_00945640(); return a + (u32)cm_fn_00945640(); }\n')
    assert oe.ftol_sites_in_source(text, 3) == 1


PADDED = ('#include "cmre.h"\n'
          'enum E { E0, E1 };\n'
          'typedef unsigned int uint;\n'
          '#define TOI(x) ((s32)(x))\n'
          'struct S { s32 d; };\n'
          's32 cm_fn_00401000(s32 c, long double v, long double w)\n'
          '{ s32 r; if (c) r = (s32)(long long)v; else r = (s32)(long long)v;\n'
          '  return r + (s32)(%s); }\n')


@needs_gcc
@pytest.mark.parametrize('second', [
    '(int)w', '(s32)w', '(unsigned long long)w', '(u8)(w * 2.0L)', '(enum E)w', '(uint)w',
    '(long unsigned)w', '(s32 const)w', '(__typeof__(c))w', 'TOI(w)', '(int)_Generic(w, default: w)',
    '(struct S){w}.d', '({ enum E e = w; e; })',
    '({ s32 q; __asm__("fistpl %0" : "=m"(q) : "t"(w) : "st"); q; })',
    '({ s32 q; __asm__("nop; fistpl %0" : "=m"(q) : "t"(w) : "st"); q; })'])
def test_every_float_to_integer_conversion_must_be_a_mutated_cast(second):
    # Review: two padded (long long) casts of v passed while the second site, spelled
    # any way the source parse cannot see, went unobserved. The marker build shows them all.
    with pytest.raises(ValueError, match=r'conversion\(s\) in the -O0 build are not floating'
                                         r'|implicit float-to-integer conversion'):
        oe.ftol_mutant_sources(PADDED % second, probe, 2)


@needs_gcc
@pytest.mark.parametrize('body,needle', [
    # GCC folds these at -O0, so a count of emitted stores balanced the unmutated (int)w.
    ('(s32)(long long)v + (s32)(long long)0.5L + (int)w', 'floating constant at line 3'),
    ('(s32)(long long)v + (s32)(long long)(0.25L * 2) + (int)w', 'floating constant at line 3'),
    ('(s32)(long long)v + (s32)(long long)v + (int)w', r'1 float-to-integer conversion\(s\)'),
])
def test_folded_casts_cannot_balance_an_unmutated_conversion(body, needle):
    text = ('#include "cmre.h"\n'
            's32 cm_fn_00401000(long double v, long double w)\n'
            f'{{ return {body}; }}\n')
    with pytest.raises(ValueError, match=needle):
        oe.ftol_mutant_sources(text, probe, 2)


@needs_gcc
def test_a_cast_of_an_assigned_local_is_not_a_constant():
    # fn_00696940: __builtin_constant_p of a local is not an integer constant expression.
    text = ('#include "cmre.h"\n'
            'volatile long long converted;\n'
            'void cm_fn_00401000(long double v)\n'
            '{ long double t, squared; t = v; squared = t * t; converted = (long long)squared; }\n')
    assert len(oe.ftol_mutant_sources(text, probe, 1)) == 1


@needs_gcc
def test_casts_folded_together_are_not_a_false_block():
    text = ('#include "cmre.h"\n'
            's32 cm_fn_00401000(long double v)\n'
            '{ return (s32)(long long)v + (s32)(long long)v; }\n')
    assert len(oe.ftol_mutant_sources(text, probe, 2)) == 2


@needs_gcc
def test_integer_casts_and_float_narrowing_stay_allowed():
    # Review: -Wfloat-conversion also reports long double -> double stores, a false block.
    text = ('#include "cmre.h"\n'
            '#define LOW(x) ((u8)(x))\n'
            'double g;\n'
            's32 cm_fn_00401000(u32 a, s32 n)\n'
            '{ long double v = (long double)a * 1.5L; g = v * 2.0L; float h = v;\n'
            '  return (s32)(long long)v + (s32)n + (int)\'a\' + (int)sizeof(v) + LOW(n) + (s32)(h > 0); }\n')
    assert len(oe.ftol_mutant_sources(text, probe, 1)) == 1


@needs_gcc
def test_a_comment_cannot_forge_a_probe_diagnostic():
    text = SOURCE.replace('/* __ftol at 0x00401010 */',
                          '/* x.c:1:1: error: static assertion failed: "cmre-ftol-float-0" */')
    assert len(oe.ftol_mutant_sources(text, probe, 1)) == 1


def test_sites_that_all_call_the_original_helper_need_no_mutant_proof():
    text = 'u32 f(void) { return (u32)cm_fn_00945640() + (u32)cm_fn_00945640 (); }\n'
    gate = {'builds': {}, 'ftol_sites': ['0x00401010', '0x00401020']}
    assert not oe.ftol_mutants_required(text, gate)
    assert oe.ftol_mutants_required(text, {**gate, 'ftol_sites': gate['ftol_sites'] + ['0x00401030']})
    assert oe.operand_mutant_sources(text, gate) == ([], [])


def test_gate_census_triggers_the_proof():
    assert oe.operand_proof_needed(b'int f(void);', {'builds': {}, 'ftol_sites': ['0x00401010']})
    assert not oe.operand_proof_needed(b'int f(void);', {'builds': {}, 'ftol_sites': []})
    assert oe.mutant_stem({'index': 3, 'operand': 'ftol'}) == 'ftol-003'
    assert oe.mutant_stem({'index': 3, 'operand': 'base'}) == 'call-003-base'
