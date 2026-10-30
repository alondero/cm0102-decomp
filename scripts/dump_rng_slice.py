"""Dump the 0x0090D0A0 / 0x0090CFC0 RNG contract from a copied 3.9.68 executable.

This is evidence extraction, not recovered source. Addresses are preferred-image VAs.
"""
from __future__ import annotations

import hashlib
import json
import struct
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "CM0102/Analysis/baseline-c/cm0102.exe"
FALLBACK = ROOT / "CM0102/Game/cm0102.exe"
SHA256 = "0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86"
IMAGE_BASE = 0x00400000

VA_PRNG = 0x0090CFC0
VA_INIT = 0x0090D0A0
VA_SRAND = 0x009469FA
VA_RAND = 0x00946A04
VA_TIME = 0x00946EBB
VA_SPRINTF = 0x00944C9F
VA_ORCH = 0x005CCCD0
VA_TABLE = 0x00AA2454
VA_TABLE_END = 0x00AD4130
TABLE_COUNT = 51000
FMT_SEED = 0x00AD437C
DOUBLE_SCALE = 0x00970A70


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def load_pe(path: Path) -> tuple[bytes, list[dict]]:
    data = path.read_bytes()
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    sections_count = struct.unpack_from("<H", data, pe + 6)[0]
    optional_size = struct.unpack_from("<H", data, pe + 20)[0]
    optional = pe + 24
    sections = []
    for index in range(sections_count):
        pos = optional + optional_size + index * 40
        name, vsize, rva, size, offset = struct.unpack_from("<8sIIII", data, pos)
        sections.append(
            dict(
                name=name.rstrip(b"\0").decode("ascii"),
                rva=rva,
                va=IMAGE_BASE + rva,
                virtual_size=vsize,
                file_offset=offset,
                raw_size=size,
            )
        )
    return data, sections


def va_to_offset(sections: list[dict], va: int) -> int:
    for section in sections:
        if section["va"] <= va < section["va"] + section["raw_size"]:
            return section["file_offset"] + (va - section["va"])
    raise KeyError(hex(va))


def read_va(data: bytes, sections: list[dict], va: int, size: int) -> bytes:
    offset = va_to_offset(sections, va)
    return data[offset : offset + size]


def find_rel32_calls(data: bytes, sections: list[dict], target: int) -> list[int]:
    text = next(s for s in sections if s["name"] == ".text")
    blob = data[text["file_offset"] : text["file_offset"] + text["raw_size"]]
    hits = []
    start = 0
    while True:
        pos = blob.find(b"\xE8", start)
        if pos < 0 or pos + 5 > len(blob):
            break
        rel = struct.unpack_from("<i", blob, pos + 1)[0]
        call_va = text["va"] + pos
        if call_va + 5 + rel == target:
            hits.append(call_va)
        start = pos + 1
    return hits


def ascii_at(data: bytes, sections: list[dict], va: int) -> str:
    offset = va_to_offset(sections, va)
    end = data.index(b"\0", offset)
    return data[offset:end].decode("ascii", errors="replace")


def main() -> int:
    path = TARGET if TARGET.is_file() else FALLBACK
    if digest(path) != SHA256:
        raise SystemExit(f"hash mismatch: {path}")
    data, sections = load_pe(path)
    out = ROOT / "CM0102/Analysis/rng-slice"
    out.mkdir(parents=True, exist_ok=True)

    init_bytes = read_va(data, sections, VA_INIT, 0x86)
    prng_bytes = read_va(data, sections, VA_PRNG, 0xDD)
    srand_bytes = read_va(data, sections, VA_SRAND, 10)
    rand_bytes = read_va(data, sections, VA_RAND, 30)
    table = read_va(data, sections, VA_TABLE, TABLE_COUNT * 4)
    scale = struct.unpack("<d", read_va(data, sections, DOUBLE_SCALE, 8))[0]
    dwords = struct.unpack(f"<{TABLE_COUNT}I", table)
    counts = Counter(dwords)

    record = {
        "input": str(path.relative_to(ROOT)).replace("\\", "/"),
        "input_sha256": SHA256,
        "address_convention": "preferred-image VA, image base 0x00400000",
        "functions": {
            "prng": {"va": hex(VA_PRNG), "size": 0xDD, "sha256": hashlib.sha256(prng_bytes).hexdigest()},
            "init": {"va": hex(VA_INIT), "size": 0x86, "sha256": hashlib.sha256(init_bytes).hexdigest()},
            "srand": {"va": hex(VA_SRAND), "size": 10, "sha256": hashlib.sha256(srand_bytes).hexdigest()},
            "rand": {"va": hex(VA_RAND), "size": 30, "sha256": hashlib.sha256(rand_bytes).hexdigest()},
        },
        "globals": {
            "lcg_seed": "0x00ad6c30",
            "mix": "0x00de1f5c",
            "cursor": "0x00de1f60",
            "log_buffer": "0x00de1f64",
            "table_end_pointer": "0x00de2798",
            "table_base": hex(VA_TABLE),
            "table_last": hex(VA_TABLE_END),
            "table_count": TABLE_COUNT,
            "seed_format": ascii_at(data, sections, FMT_SEED),
        },
        "callers": {
            "init": [hex(va) for va in find_rel32_calls(data, sections, VA_INIT)],
            "prng": [hex(va) for va in find_rel32_calls(data, sections, VA_PRNG)],
            "srand": [hex(va) for va in find_rel32_calls(data, sections, VA_SRAND)],
            "rand": [hex(va) for va in find_rel32_calls(data, sections, VA_RAND)],
            "time": [hex(va) for va in find_rel32_calls(data, sections, VA_TIME)],
            "sprintf": [hex(va) for va in find_rel32_calls(data, sections, VA_SPRINTF)],
        },
        "table": {
            "file_offset": hex(va_to_offset(sections, VA_TABLE)),
            "unique_values": len(counts),
            "min": min(dwords),
            "max": max(dwords),
            "zero_count": counts[0],
            "first_16": [hex(v) for v in dwords[:16]],
        },
        "prng_large_bound_scale_qword": {"va": hex(DOUBLE_SCALE), "ieee754_double": scale},
        "time_iat": {
            "0x00967148": "KERNEL32.DLL::GetLocalTime",
            "0x0096714c": "KERNEL32.DLL::GetSystemTime",
            "0x00967150": "KERNEL32.DLL::GetTimeZoneInformation",
        },
        "notes": [
            "0x009469fa / 0x00946a04 match the MSVC LCG (seed * 0x343FD + 0x269EC3, then (seed>>16)&0x7FFF).",
            "0x00946ebb is a time() helper using GetLocalTime/GetSystemTime/GetTimeZoneInformation.",
            "0x00944c9f is a sprintf-style writer used for the seed log line.",
            "0x0090cfc0 walks a 51000-entry dword table whose start pointer is chosen by 0x0090d0a0.",
        ],
    }

    (out / "slice.json").write_text(json.dumps(record, indent=2) + "\n", encoding="utf-8")
    (out / "init.bin").write_bytes(init_bytes)
    (out / "prng.bin").write_bytes(prng_bytes)
    (out / "srand.bin").write_bytes(srand_bytes)
    (out / "rand.bin").write_bytes(rand_bytes)
    (out / "table.bin").write_bytes(table)

    orch_size = 4361
    orch = read_va(data, sections, VA_ORCH, orch_size)
    orch_calls = []
    i = 0
    while i + 5 <= len(orch):
        if orch[i] == 0xE8:
            rel = struct.unpack_from("<i", orch, i + 1)[0]
            site = VA_ORCH + i
            orch_calls.append({"from_va": hex(site), "to_va": hex(site + 5 + rel)})
            i += 5
            continue
        i += 1
    (out / "orchestration-calls.json").write_text(
        json.dumps({"function_va": hex(VA_ORCH), "body_bytes": orch_size, "rel32_calls": orch_calls}, indent=2)
        + "\n",
        encoding="utf-8",
    )
    print(f"Wrote {out}")
    print(f"init callers: {record['callers']['init']}")
    print(f"prng callers: {len(record['callers']['prng'])}")
    print(f"table unique={record['table']['unique_values']} zeros={record['table']['zero_count']}")
    print(f"scale double={scale}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
