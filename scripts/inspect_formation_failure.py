"""Export the formation open-failure branch in both preserved executable copies.

Requires capstone. This establishes a local diagnostic correspondence, not
whole-function or cross-executable behavioral equivalence.
"""
import hashlib
import json
from pathlib import Path
import sys

import capstone
from dump_rng_slice import ROOT, SHA256, load_pe, read_va, ascii_at
from prepare_runtime import GDI_SHA256


def main():
    out = ROOT / "CM0102/Analysis/formation-failure"
    out.mkdir(parents=True, exist_ok=False)
    record = dict(python=sys.version, capstone=capstone.__version__, command=sys.argv,
                  script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                  address_convention="preferred-image VA; both image bases 0x00400000", inputs=[])
    for name, expected, start, strings in [
        ("cm0102.exe", SHA256, 0x5b5d10, [0x98f928, 0x9cc3dc, 0x9cc76c]),
        ("cm0102_GDI.exe", GDI_SHA256, 0x5b5e40, [0x98e928, 0x9cb3dc, 0x9cb76c]),
    ]:
        source = ROOT / "CM0102/Game" / name
        if hashlib.sha256(source.read_bytes()).hexdigest() != expected:
            raise ValueError(f"Wrong input: {name}")
        # All analysis uses a byte-identical local copy.
        copied = out / name
        copied.write_bytes(source.read_bytes())
        data, sections = load_pe(copied)
        code = read_va(data, sections, start, 0x113)
        disassembler = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        instructions = [dict(va=f"{i.address:08x}", bytes=i.bytes.hex(), mnemonic=i.mnemonic, operands=i.op_str)
                        for i in disassembler.disasm(code, start)]
        if sum(len(bytes.fromhex(i["bytes"])) for i in instructions) != len(code):
            raise ValueError("Incomplete disassembly")
        item = dict(path=str(source.relative_to(ROOT)), sha256=expected,
                    start_va=f"{start:08x}", size=len(code),
                    slice_sha256=hashlib.sha256(code).hexdigest(), instructions=instructions,
                    strings={f"{va:08x}": ascii_at(data, sections, va) for va in strings})
        record["inputs"].append(item)
    record["same_mnemonic_sequence"] = [i["mnemonic"] for i in record["inputs"][0]["instructions"]] == [i["mnemonic"] for i in record["inputs"][1]["instructions"]]
    record["same_diagnostic_strings"] = list(record["inputs"][0]["strings"].values()) == list(record["inputs"][1]["strings"].values())
    (out / "evidence.json").write_text(json.dumps(record, indent=2) + "\n", encoding="utf-8")
    print({k: record[k] for k in ["same_mnemonic_sequence", "same_diagnostic_strings"]})
    print(record["inputs"][1]["strings"])


if __name__ == "__main__":
    main()
