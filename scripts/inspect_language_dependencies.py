"""Verify instruction-backed next-slice language evidence; no recovery promotion.

Requires a completed read-only map_recovery.py export. Does not alter Ghidra,
the executable, recovery sources or accepted ledgers.
"""
from __future__ import annotations

import argparse
from collections import Counter
import csv
import hashlib
import json
from pathlib import Path
import platform
import struct
import sys

import capstone
import dump_rng_slice as pe

ROOT = Path(__file__).resolve().parents[1]
SELECTED = {
    0x66E790: "filename/memory-mode setup and keyboard-layout language selection candidate",
    0x66E9C0: "translation table teardown candidate",
    0x66EA10: "three-owned-string row destructor candidate",
    0x66EA50: "available-language membership check candidate",
    0x66EB40: "language selection and table loading candidate",
    0x6701F0: "language morphology candidate; pseudocode signature unreliable",
    0x6820A0: "table-based morphology replacement candidate A",
    0x682150: "table-based morphology replacement candidate B",
}


def read_rows(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_json(path, value):
    path.write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--export", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    analysis = (ROOT / "CM0102/Analysis").resolve()
    source, out = args.export.resolve(), args.output.resolve()
    if out == analysis or not out.is_relative_to(analysis):
        parser.error("output must be a fresh directory beneath CM0102/Analysis")
    if source == analysis or not source.is_relative_to(analysis):
        parser.error("export must be beneath CM0102/Analysis")
    if pe.digest(pe.TARGET) != pe.SHA256 or pe.digest(pe.FALLBACK) != pe.SHA256:
        raise ValueError("Pinned original/copy hash mismatch")
    export_report = source / "run.json"
    run = json.loads(export_report.read_text(encoding="utf-8"))
    if not run["success"] or run["input_sha256"] != pe.SHA256:
        raise ValueError("Export was not successful against the pinned image")
    for name, expected in run["outputs"].items():
        if pe.digest(source / name) != expected:
            raise ValueError(f"Export artifact changed: {name}")
    inventory = ROOT / "CM0102/Analysis/baseline-c/exports/functions.tsv"
    functions = {int(r["entry_va"], 16): r for r in read_rows(inventory)}
    rows = read_rows(source / "selected-disassembly.tsv")
    data, sections = pe.load_pe(pe.TARGET)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    records, calls, checked = [], [], 0
    for va, contract in SELECTED.items():
        body = [r for r in rows if int(r["function_va"], 16) == va]
        if not body or int(body[0]["instruction_va"], 16) != va:
            raise ValueError(f"Missing selected entry: {va:08x}")
        identity = hashlib.sha256()
        for row in body:
            site = int(row["instruction_va"], 16)
            blob = bytes.fromhex(row["bytes"])
            if pe.read_va(data, sections, site, len(blob)) != blob:
                raise ValueError(f"Instruction bytes differ: {site:08x}")
            instructions = list(md.disasm(blob, site))
            if len(instructions) != 1 or instructions[0].size != len(blob):
                raise ValueError(f"Not exactly one decoded instruction: {site:08x}")
            identity.update(struct.pack("<II", site, len(blob)) + blob)
            checked += 1
            instruction = instructions[0]
            if instruction.mnemonic == "call":
                operand = instruction.operands[0]
                target = (f"0x{operand.imm:08x}" if operand.type == capstone.x86.X86_OP_IMM else None)
                calls.append(dict(function_va=f"0x{va:08x}", site_va=f"0x{site:08x}",
                                  bytes=blob.hex(), target_va=target, instruction=row["instruction"]))
        counts = Counter(r["target_va"] for r in calls if r["function_va"] == f"0x{va:08x}")
        records.append(dict(va=f"0x{va:08x}", contract=contract,
                            ghidra_body_bytes=int(functions[va]["body_bytes"]),
                            checked_instruction_records=len(body),
                            instruction_address_length_bytes_sha256=identity.hexdigest(),
                            call_counts=dict(counts),
                            status=dict(identified=True, decompiled=False, compiled=False,
                                        validated=False, integrated=False)))
    # Loader's array-constructor callback is absent from Ghidra's function list.
    # Exact 13-byte return-terminated code, excluding three following NOPs.
    callback = pe.read_va(data, sections, 0x66F4C0, 13)
    expected = bytes.fromhex("8bc133c98908894804894808c3")
    if callback != expected:
        raise ValueError("Row constructor candidate bytes changed")
    out.mkdir(parents=True, exist_ok=False)
    write_json(out / "functions.json", records)
    write_json(out / "calls.json", calls)
    indices = list(pe.read_va(data, sections, 0x66E9A0, 26))
    targets = list(struct.unpack("<" + "I" * (max(indices) + 1),
                                pe.read_va(data, sections, 0x66E960, 4 * (max(indices) + 1))))
    setup_sites = {int(r["instruction_va"], 16) for r in rows if int(r["function_va"], 16) == 0x66E790}
    if any(target not in setup_sites for target in targets):
        raise ValueError("Setup table target is not a verified selected instruction")
    write_json(out / "setup-tables.json", dict(
        byte_table_va="0x0066e9a0", values=indices,
        target_table_va="0x0066e960", targets=[f"0x{v:08x}" for v in targets],
        selection="(keyboard-layout low word & 0x3ff) - 4; unsigned range check <=25",
        imported_call_iat_va="0x009671d8", imported_call_site_va="0x0066e7c0",
        purpose="Static dispatch evidence only; no compiled recovery"))
    write_json(out / "row-constructor.json", dict(
        va="0x0066f4c0", size=13, bytes=callback.hex(), sha256=hashlib.sha256(callback).hexdigest(),
        disassembly=[f"{i.address:08x} {i.mnemonic} {i.op_str}" for i in md.disasm(callback, 0x66F4C0)],
        ghidra_function_present=0x66F4C0 in functions,
        contract="ECX row pointer; returns same pointer in EAX; zeros only dwords +0/+4/+8",
        compiled=False, validated=False, integrated=False))
    if pe.digest(pe.TARGET) != pe.SHA256 or pe.digest(pe.FALLBACK) != pe.SHA256:
        raise ValueError("Post-inspection input/copy hash mismatch")
    write_json(out / "report.json", dict(
        status="pass", command=sys.argv, python=sys.version, platform=platform.platform(),
        capstone=capstone.__version__, input=str(pe.TARGET), input_sha256=pe.SHA256,
        address_convention="preferred-image VA; image base 0x00400000",
        export_report=dict(path=str(export_report.relative_to(ROOT)), sha256=pe.digest(export_report)),
        sources={str(p.relative_to(ROOT)): pe.digest(p) for p in (Path(__file__).resolve(),
                 ROOT / "scripts/dump_rng_slice.py", inventory)},
        checked_instructions=checked, candidate_functions=len(records),
        artifacts={p.name: pe.digest(p) for p in out.iterdir() if p.is_file()},
        limitations=["Candidate contracts are static observations, not compiled or validated recovery",
                     "Pseudocode text is unvalidated and can omit ABI/retained-stack arguments",
                     "Instruction identities do not assert contiguous extents or complete control flow",
                     "Indirect calls, exception behavior, loader failures and morphology remain unrecovered"]))
    print(f"PASS: {len(records)} candidates, {checked} checked instruction records; {out}")


if __name__ == "__main__":
    main()
