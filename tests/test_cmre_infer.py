"""Inference contracts for tier-1 acceptance hints.

Real-image and synthetic instruction fixtures exercise input attribution and
arena placement. Synthetic bytes run only in an in-memory copy of the image.
"""
import pytest
import random
from copy import copy
from dataclasses import replace

from cmre import difftest, recover, emu, image, inventory

NEAR_MISS = 0x006CD5F0  # preferred-image VA; SHA-256 pinned by cmre.env


def infer_code(monkeypatch, code, hints=None):
    """Run infer's real fault voting against explicit x86 test instructions."""
    img = copy(image.primary())
    section = img.section_at(NEAR_MISS)
    offset = NEAR_MISS - section.va
    patched = replace(section, raw=section.raw[:offset] + code + section.raw[offset + len(code):])
    img.sections = [patched if s is section else s for s in img.sections]
    inv = copy(inventory.load())
    inv.functions = dict(inv.functions)
    inv.functions[NEAR_MISS] = replace(inv.functions[NEAR_MISS], body_bytes=len(code))
    monkeypatch.setattr(image, "primary", lambda: img)
    monkeypatch.setattr(inventory, "load", lambda: inv)
    opts = difftest.Options.for_function(NEAR_MISS, {"cases": 120})
    opts.hints = hints or {}
    return difftest.infer(NEAR_MISS, (NEAR_MISS, NEAR_MISS + len(code)), opts)

def test_infer_records_are_marked_auto():
    # Anything the inference writes must carry "auto": true so an agent can claim it.
    assert recover.infer_hints(NEAR_MISS).get("auto") is True


def test_infer_is_deterministic_per_function_va():
    assert recover.infer_hints(NEAR_MISS) == recover.infer_hints(NEAR_MISS)


def test_struct_pointer_function_keeps_the_dereference_inside_the_arena():
    hints = recover.infer_hints(NEAR_MISS)
    assert hints["struct_size"] == 0xF596  # [esi+0xf592] is a four-byte read
    opts = difftest.Options.for_function(NEAR_MISS, {})
    opts.hints = hints
    for seed in range(40):
        case = difftest.make_case(random.Random(seed), opts, [], difftest.arena_templates())
        assert emu.ARENA <= case.regs["ecx"]
        assert case.regs["ecx"] + hints["struct_size"] <= emu.ARENA + emu.ARENA_SIZE


def test_struct_pointer_only_emitted_when_max_disp_exceeds_safe_head():
    # A pure-register, no-large-offset function (0x00450A70 = 4 bytes) must not get struct hints.
    h = recover.infer_hints(0x00450A70)
    assert "struct_size" not in h


def test_RED_struct_promotion_for_thiscall_with_aliased_esi_base():
    # Both return paths read through ESI copied from entry ECX. The later path
    # reaches offset 0xf592, and its dword needs 0xf596 bytes including its width.
    h = recover.infer_hints(NEAR_MISS)
    assert h.get("ecx") == "struct", f"expected ecx: struct, got {h}"
    assert h["struct_size"] == 0xF596


def test_RED_struct_promotion_for_argument_with_aliased_register_chain(monkeypatch):
    # mov esi,[esp+0xc]; mov edi,esi; mov eax,[edi+0x3f592]; ret 0xc.
    # With no pushes, [esp+0xc] really is args[2]. The large read uses its alias.
    h = infer_code(monkeypatch, bytes.fromhex("8b74240c89f78b8792f50300c20c00"))
    assert h.get("args", [None, None, None])[2] == "struct", f"expected args[2]: struct, got {h}"
    assert h["struct_size"] == 0x3F596
    opts = difftest.Options.for_function(NEAR_MISS)
    opts.hints = h
    for seed in range(40):
        case = difftest.make_case(random.Random(seed), opts, [], difftest.arena_templates())
        assert emu.ARENA <= case.stack[2]
        assert case.stack[2] + h["struct_size"] <= emu.ARENA + emu.ARENA_SIZE


def test_stack_argument_origin_accounts_for_saved_registers(monkeypatch):
    # push esi; push edi; mov edi,[esp+0xc]; mov esi,edi;
    # mov eax,[esi+0x3f592]; pop edi; pop esi; ret 4.
    h = infer_code(monkeypatch, bytes.fromhex("56578b7c240c89fe8b8692f503005f5ec20400"))
    assert h["args"] == ["struct"]


def test_later_return_path_retains_its_own_aliases(monkeypatch):
    # push esi; mov esi,ecx; test eax,eax; jne later; mov eax,[esi+0x103];
    # pop esi; ret; later: mov eax,[esi+0x3f592]; pop esi; ret.
    h = infer_code(monkeypatch, bytes.fromhex("5689ce85c075088b86030100005ec38b8692f503005ec3"))
    assert h["ecx"] == "struct"
    assert h["struct_size"] == 0x3F596


def test_register_copies_snapshot_the_source_before_it_is_overwritten(monkeypatch):
    # mov esi,ecx; mov ecx,edx; mov eax,[esi+0x3f592]; ret.
    h = infer_code(monkeypatch, bytes.fromhex("89ce89d18b8692f50300c3"))
    assert h["ecx"] == "struct"
    assert "edx" not in h


def test_loaded_pointer_is_not_an_alias_of_its_container(monkeypatch):
    # mov esi,ecx; mov esi,[esi]; mov eax,[esi+0x3f592]; ret.
    h = infer_code(monkeypatch, bytes.fromhex("89ce8b368b8692f50300c3"))
    assert h.get("ecx") == "ptr"
    assert "struct_size" not in h


def test_overwritten_argument_tracks_the_stored_register_source(monkeypatch):
    # mov [esp+4],edx; mov esi,[esp+4]; mov eax,[esi+0x3f592]; ret.
    h = infer_code(monkeypatch, bytes.fromhex("895424048b7424048b8692f50300c3"), {"args": ["ptr"]})
    assert h["edx"] == "struct"
    assert h["args"] == ["ptr"]


def test_join_promotes_each_proven_input_origin(monkeypatch):
    # test eax,eax; je second; mov esi,ecx; jmp join; second: mov esi,edx;
    # join: mov eax,[esi+0x3f592]; ret. Both inputs are already proven pointers.
    h = infer_code(monkeypatch, bytes.fromhex("85c0740489ceeb0289d68b8692f50300c3"),
                   {"ecx": "ptr", "edx": "ptr"})
    assert h["ecx"] == h["edx"] == "struct"
    assert h["struct_size"] == 0x3F596


def test_word_push_invalidates_overlapping_saved_pointer(monkeypatch):
    # push ecx; add esp,4; push word 0; sub esp,2; pop esi;
    # mov eax,[esi+0x3f592]; ret. The word push replaces saved ECX's upper half.
    h = infer_code(monkeypatch, bytes.fromhex("5183c4046668000083ec025e8b8692f50300c3"), {"ecx": "ptr"})
    assert h["ecx"] == "ptr"
    assert "struct_size" not in h


@pytest.mark.parametrize("size", [0, 1, 0xF596, emu.ARENA_SIZE - 1, emu.ARENA_SIZE])
def test_struct_pointer_handles_arena_boundaries(size):
    rng = random.Random(1)
    for _ in range(40):
        base = difftest._struct_pointer(rng, size)
        assert emu.ARENA <= base < emu.ARENA + emu.ARENA_SIZE
        assert base + size <= emu.ARENA + emu.ARENA_SIZE


def test_struct_larger_than_arena_is_explicitly_unshapeable():
    with pytest.raises(ValueError, match="arena"):
        difftest._struct_pointer(random.Random(1), emu.ARENA_SIZE + 1)
