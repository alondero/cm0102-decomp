"""Data-only expansion of reviewed manager-record-copy entry inputs.

This first kit edits four explicit fields in an ordinary input fixture. It does
not execute code, select a random seed, infer a category table, construct engine
objects, or initialize missing bytes. The caller supplies a reviewed fixture,
layout, byte values and retained original-evidence artifact. Keep the reviewed
hashes separately: computing a binding hash is not an original-side observation.

Addresses ending in ``_va`` use preferred-image virtual addresses. Offsets use
the fixture arena. The caller must obtain ``observed_target_sha256`` from its
binary-identity gate; this module never opens the game. Output still needs the
existing hint validator and original/C gates before recovery acceptance.
"""
from __future__ import annotations

from copy import deepcopy
from dataclasses import asdict, dataclass
import hashlib
import json
import re


@dataclass(frozen=True)
class ManagerRecordCopySpec:
    target_sha256: str
    root_va: int
    manager_callee_va: int
    lookup_callee_va: int
    copy_callee_va: int
    category: int
    arena_size: int
    lookup_word_offset: int
    count_word_offset: int
    capacity_word_offset: int
    category_slot_offset: int
    lookup_word: bytes
    count_word: bytes
    capacity_word: bytes
    category_pointer: bytes


def _sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _hash(value: str) -> None:
    if not isinstance(value, str) or re.fullmatch(r"[0-9a-f]{64}", value) is None:
        raise ValueError("expected a lowercase SHA-256")


def _u32(value: int) -> None:
    if type(value) is not int or not 0 <= value <= 0xffffffff:
        raise ValueError("expected an unsigned DWORD")


def _fields(spec: ManagerRecordCopySpec) -> tuple[tuple[int, bytes], ...]:
    _hash(spec.target_sha256)
    for va in (spec.root_va, spec.manager_callee_va, spec.lookup_callee_va, spec.copy_callee_va):
        _u32(va)
        if va == 0:
            raise ValueError("original root/callee VAs must be explicit and nonzero")
    _u32(spec.category)
    if (type(spec.arena_size) is not int or not 4 <= spec.arena_size <= 0x100000000
            or spec.arena_size % 4):
        raise ValueError("arena_size must be an explicit DWORD-aligned extent")
    fields = ((spec.lookup_word_offset, spec.lookup_word),
              (spec.count_word_offset, spec.count_word),
              (spec.capacity_word_offset, spec.capacity_word),
              (spec.category_slot_offset, spec.category_pointer))
    occupied: set[int] = set()
    for (offset, data), width in zip(fields, (2, 2, 2, 4)):
        _u32(offset)
        if type(data) is not bytes or len(data) != width:
            raise ValueError(f"field at {offset:#x} needs exactly {width} explicit bytes")
        if offset % width or offset + width > spec.arena_size:
            raise ValueError("field offset is unaligned or outside the arena")
        addresses = set(range(offset, offset + width))
        if occupied & addresses:
            raise ValueError("manager-copy fields overlap")
        occupied |= addresses
    return fields


def manager_record_copy_binding(fixture: dict, spec: ManagerRecordCopySpec) -> str:
    """Hash a proposed exact recipe for independent review and later replay.

    This pins the complete base fixture, including its import seed/transcript,
    plus every root, callee, layout and value parameter. It supplies no evidence
    that the selected inputs reach a copy or that the callees are correct.
    """
    _fields(spec)
    values = {key: value.hex() if isinstance(value, bytes) else value
              for key, value in asdict(spec).items()}
    return _sha256(json.dumps({"fixture": fixture, "spec": values},
                              sort_keys=True, separators=(",", ":"),
                              allow_nan=False).encode("utf-8"))


def expand_manager_record_copy(
    fixture: dict, spec: ManagerRecordCopySpec, *,
    observed_target_sha256: str, evidence: bytes,
    reviewed_evidence_sha256: str, reviewed_binding_sha256: str,
) -> dict:
    """Return an ordinary fixture with only the four reviewed fields changed.

    Partial DWORD edits require an explicit existing DWORD in ``arena_writes``.
    Missing neighbors fail closed instead of borrowing template bytes or zeros.
    Complete DWORD replacement needs no prior value. No provenance/harness fields
    are inserted into the result, and the input fixture is never mutated.
    """
    fields = _fields(spec)
    for value in (observed_target_sha256, reviewed_evidence_sha256,
                  reviewed_binding_sha256):
        _hash(value)
    if observed_target_sha256 != spec.target_sha256:
        raise ValueError("original binary SHA-256 changed")
    if type(evidence) is not bytes or not evidence:
        raise ValueError("retained original evidence bytes are required")
    if _sha256(evidence) != reviewed_evidence_sha256:
        raise ValueError("original evidence SHA-256 changed")
    if manager_record_copy_binding(fixture, spec) != reviewed_binding_sha256:
        raise ValueError("reviewed fixture/root/callee/layout/value binding changed")
    required = {"regs", "stack", "arena_template", "arena_writes", "globals", "import_seed"}
    if (not isinstance(fixture, dict) or not required <= fixture.keys()
            or fixture.keys() - required - {"fpcw", "import_transcript"}):
        raise ValueError("expected an ordinary input fixture")
    writes = fixture["arena_writes"]
    if not isinstance(writes, dict):
        raise ValueError("arena_writes must be an object")
    offsets = {}
    for key, value in writes.items():
        if not isinstance(key, str) or re.fullmatch(r"0[xX][0-9a-fA-F]+", key) is None:
            raise ValueError("arena write keys must be hexadecimal offsets")
        offset = int(key, 16)
        _u32(value)
        if offset % 4 or not 0 <= offset <= spec.arena_size - 4 or offset in offsets:
            raise ValueError("arena write is unaligned, out of range or duplicated")
        offsets[offset] = key
    result = deepcopy(fixture)
    for offset, data in fields:
        aligned = offset & ~3
        key = offsets.get(aligned, hex(aligned))
        if len(data) == 4:
            value = int.from_bytes(data, "little")
        else:
            if aligned not in offsets:
                raise ValueError(f"partial write needs explicit neighboring bytes at {aligned:#x}")
            original = bytearray(result["arena_writes"][key].to_bytes(4, "little"))
            original[offset - aligned:offset - aligned + len(data)] = data
            value = int.from_bytes(original, "little")
        result["arena_writes"][key] = value
    return result
