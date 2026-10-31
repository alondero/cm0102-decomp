"""Compiler-generated code is reported separately from the C translation queue."""
import pytest

from cmre import queue


@pytest.mark.parametrize("va, reason", [
    (0x0095C8D0, "unwind-funclet"),   # lea ecx, [ebp - N] ; jmp destructor
    (0x00953B90, "unwind-funclet"),   # mov eax, [ebp - N] ; push eax ; call delete ; pop ecx ; ret
    (0x00959D80, "unwind-funclet"),   # mov ecx, [ebp - N] ; jmp destructor
    (0x00944C10, "import-thunk"),     # jmp [import]
])
def test_compiler_generated_shapes(va, reason):
    assert queue.compiler_generated(va) == reason


@pytest.mark.parametrize("va", [
    0x007E6130,  # scalar deleting destructor: ordinary C
    0x005D5A30,  # mov al, [ecx + N] ; ret
    0x00401000,  # sets up its own frame before using EBP
])
def test_ordinary_functions_are_not_compiler_generated(va):
    assert queue.compiler_generated(va) is None


def test_c_queue_excludes_compiler_generated_by_default():
    vas = {i.va for i in queue.c_frontier(100000, max_bytes=16)}
    assert 0x0095C8D0 not in vas
    assert 0x0095C8D0 in {i.va for i in queue.c_frontier(100000, max_bytes=16, include_generated=True)}


def test_coverage_reports_compiler_generated_separately():
    summary = queue.coverage_summary()
    assert summary["compiler_generated_functions"] > 3000
    assert summary["compiler_generated_body_bytes"] > 0


def test_classification_requires_image_metadata():
    # Reads its caller's EBP but is in no unwind map or scope table and is never referenced.
    assert queue.compiler_generated(0x009452B2) is None
    # Null-filter entries of `_except_handler3` scope tables, not C++ unwind actions.
    assert queue.compiler_generated(0x00946323) == "seh-finally"
    assert queue.compiler_generated(0x00946419) == "seh-finally"
