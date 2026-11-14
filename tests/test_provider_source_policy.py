"""Semantic source-policy rejection and provenance-bound withdrawal."""
from cmre.source_policy import inline_assembly_lines


def test_assembly_macros_are_code_but_literals_and_comments_are_not():
    assert inline_assembly_lines('/* __asm__ */\nchar *s="asm";\n#define A __asm__\n') == [3]
    assert inline_assembly_lines('int f(void) { __asm__ volatile ("nop"); return 1; }') == [1]


def test_pipeline_rejects_assembly_before_build_or_test(tmp_path):
    from cmre.provider_runtime import Executor
    source = tmp_path / 'fn.c'
    source.write_text('int f(void) { __asm__("nop"); return 1; }')
    result = Executor(tmp_path)._c_bundle({}, {'candidate': str(source)}, [], {}, tmp_path)
    assert result['blocker'] == 'plain-c-required'


def test_line_splicing_precedes_comment_and_keyword_recognition():
    for newline in ('\n', '\r\n'):
        assert inline_assembly_lines('// comment \\' + newline + '__asm__("nop");' + newline) == []
        assert inline_assembly_lines('/\\' + newline + '/ __asm__("nop");' + newline) == []
        assert inline_assembly_lines('int f(void) { __as\\' + newline + 'm__("nop"); }') == [1]
        assert inline_assembly_lines('// comment' + newline + '__asm__("nop");') == [2]
