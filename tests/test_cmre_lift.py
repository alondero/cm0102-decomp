"""Lifter/build regressions on real functions from the pinned primary executable."""
import os
from pathlib import Path

import pytest

from cmre import build, env, image, lift


@pytest.fixture
def workdir(tmp_path):
    return tmp_path


def test_labels_survive_linking_and_round_trip(workdir):
    # PE/COFF GAS drops labels starting with "L"; labels must stay in the symbol table.
    lifted, text, built = build.lift_and_build(0x004671B0, workdir)
    assert built is not None, lifted.problems
    assert "v_004671b0" in built.symbols
    assert build.round_trip(lifted, built) == []


def test_immediate_self_reference_uses_offset(workdir):
    # 0x00414C20 pushes an address inside its own region; a bare symbol would be a memory read.
    lifted, text, built = build.lift_and_build(0x00414C20, workdir)
    assert built is not None, lifted.problems
    assert "push offset v_00414cf0" in text


def test_jump_table_entries_become_labels(workdir):
    lifted, text, built = build.lift_and_build(0x0053AC40, workdir)
    assert built is not None, lifted.problems
    assert "jmp dword ptr [eax*4 + v_0053acc8]" in text
    assert "    .long v_0053ac83" in text


def test_data_after_code_copies_to_region_end():
    # 0x00449590 indexes a byte table that follows its jump table; its length is unknown,
    # so the rest of the region must be copied, not one byte.
    lifted = lift.discover(0x00449590)
    assert not lifted.problems
    run = next(d for d in lifted.data if d.va == 0x004496A8)
    assert run.va + len(run.raw) == lifted.region_end


def test_build_ignores_hostile_path(workdir, monkeypatch):
    # Git Bash puts devkitPro's `as` first; gcc then fails silently unless the toolchain PATH is pinned.
    monkeypatch.setenv("PATH", "C:/devkitPro/msys2/usr/bin" + os.pathsep + os.environ["PATH"])
    lifted, text, built = build.lift_and_build(0x004671B0, workdir)
    assert built is not None, lifted.problems


def test_decode_window_is_clamped_at_section_end():
    text = image.primary().text
    assert len(image.primary().read_upto(text.end - 3, 16)) == 3


def test_problems_block_instead_of_guessing():
    lifted = lift.discover(0x004671B0)
    assert lifted.problems == []
    assert lifted.instruction_bytes == lifted.ghidra_body_bytes


def test_render_is_deterministic():
    assert lift.render(lift.discover(0x00449590)) == lift.render(lift.discover(0x00449590))
