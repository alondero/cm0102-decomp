"""Synthetic input-byte contracts: no game, VM, compiler or installed assets."""
from copy import deepcopy
from dataclasses import replace
import hashlib

import pytest

from cmre.fixture_kits import (ManagerRecordCopySpec, expand_manager_record_copy,
                               manager_record_copy_binding)


TARGET = "a" * 64
EVIDENCE = b"synthetic original-evidence stand-in; never engine evidence"


def fixture():
    return {
        "regs": {name: 0x12345678 for name in
                 ("eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "eflags")},
        "stack": [0x11223344],
        "arena_template": 2, "arena_writes": {
            "0x24": 0x1234abcd, "0X0030": 0x00020004, "0x60": 0x98765432,
            "0x90": 0xdeadbeef,
        }, "globals": {}, "import_seed": 69,
        "fpcw": 0x37f, "import_transcript": [],
    }


def spec():
    return ManagerRecordCopySpec(
        target_sha256=TARGET, root_va=0x1010, manager_callee_va=0x2020,
        lookup_callee_va=0x4040, copy_callee_va=0x3030, category=24, arena_size=0x100,
        lookup_word_offset=0x26, count_word_offset=0x32,
        capacity_word_offset=0x30, category_slot_offset=0x60,
        lookup_word=b"\xff\xff", count_word=b"\x00\x00",
        capacity_word=b"\x01\x00", category_pointer=b"\x44\x33\x22\x11",
    )


def expand(base=None, selected=None, **overrides):
    base = fixture() if base is None else base
    selected = spec() if selected is None else selected
    kwargs = dict(observed_target_sha256=TARGET, evidence=EVIDENCE,
                  reviewed_evidence_sha256=hashlib.sha256(EVIDENCE).hexdigest(),
                  reviewed_binding_sha256=manager_record_copy_binding(base, selected))
    kwargs.update(overrides)
    return expand_manager_record_copy(base, selected, **kwargs)


def test_exact_dwords_and_ordinary_input_metadata_preserved_without_mutation():
    base = fixture()
    before = deepcopy(base)
    expected = deepcopy(base)
    expected["arena_writes"].update({"0x24": 0xffffabcd, "0X0030": 1,
                                      "0x60": 0x11223344})
    assert expand(base) == expected
    assert base == before
    # Mutating the result must not alias any nested input data.
    changed = expand(base)
    changed["import_transcript"].append({"synthetic": 99})
    assert base == before


@pytest.mark.parametrize("category,slot", [(24, 0x60), (23, 0x5c), (42, 0xa8)])
def test_category_slots_are_supplied_independently(category, slot):
    selected = replace(spec(), category=category, category_slot_offset=slot)
    result = expand(selected=selected)
    assert result["arena_writes"][hex(slot)] == 0x11223344
    # Deliberately non-formula mapping is data; kit cannot infer a category map.
    remapped = replace(selected, category_slot_offset=0x80)
    assert expand(selected=remapped)["arena_writes"]["0x80"] == 0x11223344


@pytest.mark.parametrize("word", [b"\xff\xff", b"\x00\x80", b"\xff\x7f", b"\x00\x00"])
def test_signed_boundary_bit_patterns_are_never_interpreted_or_clamped(word):
    result = expand(selected=replace(spec(), lookup_word=word, count_word=word))
    assert result["arena_writes"]["0x24"].to_bytes(4, "little") == b"\xcd\xab" + word
    assert result["arena_writes"]["0X0030"].to_bytes(4, "little") == b"\x01\x00" + word


@pytest.mark.parametrize("field,value", [
    ("root_va", 0x1011), ("manager_callee_va", 0x2021),
    ("lookup_callee_va", 0x4041),
    ("copy_callee_va", 0x3031), ("category", 23),
    ("arena_size", 0x104), ("category_slot_offset", 0x64),
    ("lookup_word", b"\xfe\xff"),
])
def test_changed_reviewed_root_callee_layout_or_value_fails_closed(field, value):
    base, original = fixture(), spec()
    pin = manager_record_copy_binding(base, original)
    with pytest.raises(ValueError, match="binding changed"):
        expand(base, replace(original, **{field: value}), reviewed_binding_sha256=pin)


def test_changed_base_seed_and_original_provenance_fail_closed():
    base = fixture()
    pin = manager_record_copy_binding(base, spec())
    base["import_seed"] = 70
    with pytest.raises(ValueError, match="binding changed"):
        expand(base, reviewed_binding_sha256=pin)
    with pytest.raises(ValueError, match="binary SHA-256 changed"):
        expand(observed_target_sha256="b" * 64)
    with pytest.raises(ValueError, match="evidence SHA-256 changed"):
        expand(evidence=EVIDENCE + b" changed")


def test_no_template_zero_or_unknown_neighbor_fallback():
    base = fixture()
    del base["arena_writes"]["0x24"]
    with pytest.raises(ValueError, match="explicit neighboring bytes"):
        expand(base)


@pytest.mark.parametrize("changes", [
    {"lookup_word": b"\xff"}, {"category_pointer": b"\x01"},
    {"count_word_offset": 0x30}, {"lookup_word_offset": 0x25},
    {"category_slot_offset": 0x100}, {"arena_size": 0x101},
    {"copy_callee_va": 0}, {"category": True},
])
def test_invalid_explicit_layout_rejected(changes):
    with pytest.raises(ValueError):
        expand(selected=replace(spec(), **changes))


@pytest.mark.parametrize("writes", [
    {"0x24": 0, "0X0024": 1}, {"0x25": 0}, {"0x100": 0},
    {"0x24": -1}, {"0x24": True}, {"36": 0},
])
def test_ambiguous_or_invalid_dword_writes_rejected(writes):
    base = fixture()
    base["arena_writes"] = writes
    with pytest.raises(ValueError):
        expand(base)


def test_no_new_harness_fields_and_no_empty_provenance():
    base = fixture()
    base["comparison_override"] = True
    with pytest.raises(ValueError, match="ordinary input fixture"):
        expand(base)
    with pytest.raises(ValueError, match="evidence bytes"):
        expand(evidence=b"")
