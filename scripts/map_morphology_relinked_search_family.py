"""Hash-locked Date/morphology family inventory and decoded call context (primary 3.9.68).

Uses the accepted Ghidra inventories without changing their project or exports.
Names are observed contracts or explicit candidates, not recovered source names.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import platform
import struct
import sys
from pathlib import Path

import capstone
import dump_rng_slice as pe

ROOT = Path(__file__).resolve().parents[1]
CONTRACTS = {
    0x549F70: "day/month/year constructor returning this",
    0x54A110: "default date construction",
    0x54A130: "day/month/year construction with diagnostics and optional weekday adjustment",
    0x54A2D0: "weekday adjustment into destination",
    0x54A3F0: "weekday calculation",
    0x54A470: "numeric date/four-digit year formatting with prior-frame month byte",
    0x54A650: "numeric date/two-digit year formatting with language-10 branch",
    0x54A950: "full weekday plus numeric date formatting with prior-frame month byte",
    0x54AE10: "short weekday/month plus ordinal day/year formatting",
    0x54B2F0: "ordinal day/short month/year formatting with independent reverse day scan",
    0x54B550: "short weekday/month plus ordinal day, no year formatting",
    0x54BA20: "full weekday/month plus ordinal day/year formatting",
    0x54BEF0: "full weekday/month plus ordinal day, no year formatting",
    0x54C3B0: "ordinal day/full month/year formatting",
    0x54C5B0: "add signed-word days",
    0x54C770: "add unsigned-byte weeks",
    0x54C8E0: "subtract signed-word days",
    0x54CAB0: "subtract unsigned-byte weeks",
    0x54CC30: "absolute year/day component difference",
    0x54CDB0: "signed day distance with asymmetric year counting",
    0x54CF70: "day of month in AX",
    0x54CFB0: "zero-based month or -1",
    0x54CFE0: "leap test on unsigned year word",
    0x54D040: "time-of-day name formatting",
    0x54D130: "language-dependent date format selection, AL result",
    0x54D170: "weekday name formatting",
    0x54D210: "short month name formatting",
    0x54D3B0: "full month name formatting",
}
SUPPORT = {0x417C10: "assignment returning a date value", 0x90D130: "diagnostic version pointer",
           0x66F4D0: "current language dword", 0x66F4E0: "localization/formatting wrapper",
           0x90C310: "language-dependent ordinal number string",
           0x66FBB0: "translation lookup, memory and file paths",
           0x682060: "unsigned-byte language key comparator",
           0x66E790: "database filename/memory mode and keyboard-layout language setup",
           0x66E9C0: "translation table teardown and current-language reset",
           0x66EA10: "row destruction preserving later-field reads and incidental EAX",
           0x66EA50: "available-language membership and setup diagnostic",
           0x66F4C0: "row constructor clearing three pointers only, manual boundary",
           0x66EB40: "language database selection and table loading",
           0x90C140: "file byte-order probe preserving incoming argument-slot writes",
           0x6820A0: "four-pointer morphology table A replacement, incidental EAX",
           0x682150: "four-pointer morphology table B replacement, incidental EAX",
           0x946464: "bsearch-style CRT support, separate from application coverage",
           0x945610: "register-ABI compiler stack probe; separate CRT support",
           0x945A60: "substring CRT front entry with recovered character-search closure",
           0x946540: "character CRT entry with recovered shared unaligned-match exit",
           0x6701F0: "complete morphology application body; recovered compiler probe, original data/CRT fixtures explicit"}
# Explicit instruction sites, not inferred disassembly of embedded jump tables.
# Each pair selects a no-time and with-time format string.
FORMAT_REFERENCES = {
    0x54A470: ((0x54A58C, 0x9B8560), (0x54A62F, 0x9B84A0)),
    0x54A650: ((0x54A7CD, 0x9B8624), (0x54A92B, 0x9B85AC)),
    0x54A950: ((0x54ABC7, 0x9B86F8), (0x54ADB5, 0x9B867C)),
    0x54AE10: ((0x54B091, 0x9B881C), (0x54B289, 0x9B87A0)),
    0x54B2F0: ((0x54B437, 0x9B8900), (0x54B51F, 0x9B8898)),
    0x54B550: ((0x54B7CA, 0x9B89B0), (0x54B9BB, 0x9B894C)),
    0x54BA20: ((0x54BCA3, 0x9B8AA0), (0x54BE8E, 0x9B89F8)),
    0x54BEF0: ((0x54C16C, 0x9B8B84), (0x54C350, 0x9B8B0C)),
    0x54C3B0: ((0x54C4E1, 0x9B8C4C), (0x54C588, 0x9B8BD8)),
}


def write_json(path, value):
    path.write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    out = args.output.resolve()
    analysis = (ROOT / "CM0102/Analysis").resolve()
    if out == analysis or not out.is_relative_to(analysis):
        parser.error("output must be a new directory beneath CM0102/Analysis")
    inventory = ROOT / "CM0102/Analysis/baseline-c/exports/functions.tsv"
    call_map = ROOT / "CM0102/Analysis/recovery-map-b/calls.tsv"
    ownership = ROOT / "CM0102/Analysis/recovery-map-b/module-candidates.json"
    ledger = ROOT / "recovery/date/ledger.json"
    lang_ledger = ROOT / "recovery/lang/ledger.json"
    lookup_ledger = ROOT / "recovery/lang/lookup-ledger.json"
    numeric_ledger = ROOT / "recovery/date/numeric-ledger.json"
    text_ledger = ROOT / "recovery/date/text-ledger.json"
    lifecycle_ledger = ROOT / "recovery/lang/lifecycle-ledger.json"
    loader_ledger = ROOT / "recovery/lang/loader-ledger.json"
    morph_tables_ledger = ROOT / "recovery/lang/morph-tables-ledger.json"
    morph_ledger = ROOT / "recovery/lang/morph-relinked-ledger.json"
    probe_ledger = ROOT / "recovery/crt/stack-probe-ledger.json"
    search_ledger = ROOT / "recovery/crt/search-ledger.json"
    if pe.digest(pe.TARGET) != pe.SHA256 or pe.digest(pe.FALLBACK) != pe.SHA256:
        raise SystemExit("Pinned original/copy hash mismatch")
    data, sections = pe.load_pe(pe.TARGET)
    with inventory.open(encoding="utf-8") as stream:
        functions = {int(r["entry_va"], 16): r for r in csv.DictReader(stream, delimiter="\t")}
    candidates = {int(r["function_va"], 16): r for r in json.loads(ownership.read_text(encoding="utf-8"))}
    recovered = {int(r["va"], 16): r for r in json.loads(ledger.read_text(encoding="utf-8"))["functions"]}
    for source_ledger in (lang_ledger, lookup_ledger, numeric_ledger, text_ledger, lifecycle_ledger, loader_ledger, morph_tables_ledger, morph_ledger, probe_ledger, search_ledger):
        contents = json.loads(source_ledger.read_text(encoding="utf-8"))
        if contents["input_sha256"] != pe.SHA256:
            raise ValueError(f"Recovery ledger input differs: {source_ledger}")
        for record in contents["functions"]:
            va = int(record["va"], 16)
            if va in recovered:
                raise ValueError(f"Conflicting recovery ledger entry: {va:08x}")
            recovered[va] = record
    for va, record in recovered.items():
        actual = pe.read_va(data, sections, va, record["size"])
        if hashlib.sha256(actual).hexdigest() != record["sha256"]:
            raise ValueError(f"Recovery ledger byte identity differs: {va:08x}")
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    calls = []
    tail_transfers = []
    selected = set(CONTRACTS) | set(SUPPORT)
    with call_map.open(encoding="utf-8") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            owner = int(row["function_va"], 16) if row["function_va"] else None
            try:
                target = int(row["target_address"], 16)
            except ValueError:
                target = None
            if owner not in selected and target not in selected:
                continue
            va = int(row["site_va"], 16)
            expected = bytes.fromhex(row["bytes"])
            actual = pe.read_va(data, sections, va, len(expected))
            if actual != expected:
                raise SystemExit(f"Call bytes differ at VA {va:08x}")
            decoded = list(md.disasm(actual, va))
            if len(decoded) != 1:
                raise SystemExit(f"Inventory entry is not a single instruction at VA {va:08x}")
            operand = decoded[0].operands[0]
            tail = (decoded[0].mnemonic == "jmp" and operand.type == capstone.x86.X86_OP_IMM
                    and row["instruction"].startswith("JMP ")
                    and row["reference_type"] == "UNCONDITIONAL_CALL")
            if decoded[0].mnemonic != "call" and not tail:
                raise SystemExit(f"Inventory entry is neither a call nor a verified direct tail jump at VA {va:08x}")
            if operand.type == capstone.x86.X86_OP_IMM and operand.imm != target:
                raise SystemExit(f"Direct call target differs at VA {va:08x}")
            context = pe.read_va(data, sections, va, 48)
            if tail:
                tail_transfers.append(dict(row,
                    classification="decoded direct JMP; inventory labels reference UNCONDITIONAL_CALL",
                    after_transfer=[f"{i.address:08x} {i.mnemonic} {i.op_str}"
                                    for i in md.disasm(context, va)]))
                continue
            calls.append(dict(row, after_call=[f"{i.address:08x} {i.mnemonic} {i.op_str}"
                                               for i in md.disasm(context, va)]))
    morphology_calls = []
    parent_blob = pe.read_va(data, sections, 0x6701F0, recovered[0x6701F0]["size"])
    parent_instructions = list(md.disasm(parent_blob, 0x6701F0))
    assert sum(i.size for i in parent_instructions) == 64465
    assert len(parent_instructions) == 20658
    inventory_sites = {int(r["site_va"], 16) for r in calls}
    for i in parent_instructions:
        if i.mnemonic != "call": continue
        assert len(i.operands) == 1 and i.operands[0].type == capstone.x86.X86_OP_IMM
        morphology_calls.append(dict(function_va="0x006701f0", site_va=f"0x{i.address:08x}",
            bytes=i.bytes.hex(), target_va=f"0x{i.operands[0].imm:08x}",
            instruction=i.mnemonic + " " + i.op_str,
            present_in_selected_ghidra_call_inventory=i.address in inventory_sites))
    records = []
    for va, contract in (CONTRACTS | SUPPORT).items():
        f = functions.get(va)
        status = recovered.get(va)
        if f is None and not (status and status.get("manual_boundary_evidence")):
            raise ValueError(f"Selected function lacks inventory or manual boundary evidence: {va:08x}")
        records.append({
            "va": f"0x{va:08x}", "contract": contract,
            "scope": "Date family candidate" if va in CONTRACTS else "support outside contiguous family",
            "ghidra_body_bytes": int(f["body_bytes"]) if f else None,
            "manual_boundary_evidence": status.get("manual_boundary_evidence") if status else None,
            "category": status.get("category", "application") if status else "application candidate",
            "status": ({k: status[k] for k in ("identified", "decompiled", "compiled", "validated", "integrated")}
                       if status else dict(identified=True, decompiled=False, compiled=False, validated=False, integrated=False)),
            "recovered_bytes": ({k: status[k] for k in ("size", "sha256")} if status else None),
            "ownership_evidence": candidates.get(va),
            "direct_callers_in_inventory": sum(r["target_address"].lower() == f"{va:08x}" for r in calls),
            "outgoing_calls_in_inventory": sum(r["function_va"].lower() == f"{va:08x}" for r in calls),
        })
        if status:
            blob = pe.read_va(data, sections, va, status["size"])
            if hashlib.sha256(blob).hexdigest() != status["sha256"]:
                raise SystemExit(f"Ledger byte identity differs at VA {va:08x}")
    out.mkdir(parents=True, exist_ok=False)
    write_json(out / "functions.json", records)
    write_json(out / "calls.json", calls)
    write_json(out / "tail-transfers.json", tail_transfers)
    write_json(out / "morphology-call-inventory.json", morphology_calls)
    formats = []
    for owner, references in FORMAT_REFERENCES.items():
        for site, target in references:
            blob = pe.read_va(data, sections, site, 5)
            decoded = list(md.disasm(blob, site))
            if (len(decoded) != 1 or decoded[0].mnemonic != "push"
                    or decoded[0].operands[0].type != capstone.x86.X86_OP_IMM
                    or decoded[0].operands[0].imm != target):
                raise SystemExit(f"Format-string reference differs at VA {site:08x}")
            formats.append(dict(function_va=f"0x{owner:08x}", site_va=f"0x{site:08x}",
                                bytes=blob.hex(), string_va=f"0x{target:08x}",
                                string=pe.ascii_at(data, sections, target)))
    write_json(out / "format-evidence.json", formats)
    search = json.loads(search_ledger.read_text(encoding="utf-8"))
    write_json(out / "crt-search-closure.json", {
        "ledger": str(search_ledger.relative_to(ROOT)).replace("\\", "/"),
        "ledger_sha256": pe.digest(search_ledger),
        "original_regions": search["original_regions"],
        "byte_accounting": search["byte_accounting"],
        "ownership": search["dependencies"]["ownership"],
        "internal_character_entry": search["dependencies"]["recovered_internal_character_entry"],
        "external_code_or_data": search["dependencies"]["external_code_or_data"],
    })
    write_json(out / "tables.json", {
        "address_convention": "preferred-image VA, image base 0x00400000",
        "common_month_offsets": {"va": "0x009b8448", "values": list(struct.unpack("<12h", pe.read_va(data, sections, 0x9B8448, 24)))},
        "leap_month_offsets": {"va": "0x009b8460", "values": list(struct.unpack("<12h", pe.read_va(data, sections, 0x9B8460, 24)))},
        "version": {"va": "0x00ad4394", "value": pe.ascii_at(data, sections, 0xAD4394)},
        "source_path": {"va": "0x009b8478", "value": pe.ascii_at(data, sections, 0x9B8478)},
    })
    if pe.digest(pe.TARGET) != pe.SHA256 or pe.digest(pe.FALLBACK) != pe.SHA256:
        raise SystemExit("Post-analysis original/copy hash mismatch")
    write_json(out / "report.json", {
        "status": "pass", "command": sys.argv, "python": sys.version,
        "platform": platform.platform(), "capstone": capstone.__version__,
        "input": str(pe.TARGET), "input_sha256": pe.SHA256,
        "address_convention": "preferred-image VA, image base 0x00400000",
        "sources": {str(p.relative_to(ROOT)).replace("\\", "/"): pe.digest(p)
                    for p in (Path(__file__), ROOT / "scripts/dump_rng_slice.py", inventory, call_map, ownership,
                              ledger, lang_ledger, lookup_ledger, numeric_ledger, text_ledger, lifecycle_ledger, loader_ledger, morph_tables_ledger, morph_ledger, probe_ledger, search_ledger)},
        "family_functions": len(CONTRACTS), "support_functions": len(SUPPORT),
        "decoded_call_records": len(calls),
        "complete_morphology_call_records": len(morphology_calls),
        "morphology_calls_absent_from_selected_ghidra_inventory": sum(not r["present_in_selected_ghidra_call_inventory"] for r in morphology_calls),
        "decoded_tail_transfer_records": len(tail_transfers),
        "artifacts": {p.name: pe.digest(p) for p in out.iterdir() if p.is_file()},
        "limitations": ["Candidate module ownership and formatting semantics are not full recovery",
                        "Ghidra body byte counts are not assumed to be contiguous function extents",
                        "Caller context is bounded linear decoding, not complete control-flow/liveness analysis",
                        "Call inventory can omit undecoded and unresolved indirect incoming calls"],
    })
    print(f"PASS: {len(CONTRACTS)} family functions, {len(SUPPORT)} support functions, {len(calls)} calls, {len(tail_transfers)} tail transfers; {out}")


if __name__ == "__main__":
    main()
