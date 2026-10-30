"""List a hash-locked original morphology region and referenced literal bytes."""
import argparse
import hashlib
import json
import sys
from pathlib import Path

import capstone
import dump_rng_slice as pe

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--start", required=True, type=lambda text: int(text, 0))
    parser.add_argument("--end", required=True, type=lambda text: int(text, 0),
                        help="exclusive preferred-image VA")
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    assert 0x6701F0 <= args.start < args.end <= 0x6820A0
    assert pe.digest(pe.TARGET) == pe.digest(pe.FALLBACK) == pe.SHA256
    data, sections = pe.load_pe(pe.TARGET)
    export_path = ROOT / "CM0102/Analysis/lang-loader-static-b"
    export = json.loads((export_path / "run.json").read_text(encoding="utf-8"))
    assert export["input_sha256"] == pe.SHA256
    assert all(pe.digest(export_path / path) == expected
               for path, expected in export["outputs"].items())
    blob = pe.read_va(data, sections, args.start, args.end - args.start)
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    instructions = list(decoder.disasm(blob, args.start))
    assert sum(item.size for item in instructions) == len(blob)
    lines, literals = [], {}
    for instruction in instructions:
        lines.append(f"{instruction.address:08x} {instruction.bytes.hex():20} "
                     f"{instruction.mnemonic} {instruction.op_str}")
        for token in instruction.op_str.replace("[", " ").replace("]", " ").replace(",", " ").split():
            if not token.startswith("0x"):
                continue
            address = int(token, 16)
            if not 0x970000 <= address < 0xAE0000:
                continue
            literal = bytearray()
            for index in range(128):
                byte = pe.read_va(data, sections, address + index, 1)[0]
                if not byte:
                    break
                literal.append(byte)
            literals[hex(address)] = literal.hex()
    output = args.output if args.output.is_absolute() else ROOT / args.output
    output.mkdir(exist_ok=False)
    (output / "instructions.txt").write_text("\n".join(lines) + "\n", encoding="utf-8")
    (output / "literals.json").write_text(json.dumps(literals, indent=2) + "\n", encoding="utf-8")
    report = dict(input="CM0102/Game/cm0102.exe", input_sha256=pe.SHA256,
                  address_convention="preferred-image VA; exclusive end",
                  command=sys.argv, python=sys.version, capstone=capstone.__version__,
                  script_sha256=pe.digest(Path(__file__)),
                  export_sha256=pe.digest(export_path / "run.json"),
                  region=dict(va=hex(args.start), size=len(blob),
                              sha256=hashlib.sha256(blob).hexdigest()),
                  artifacts={path.name: pe.digest(path) for path in output.iterdir()})
    (output / "report.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print("\n".join(lines))
    print(json.dumps({address: bytes.fromhex(value).decode("latin1")
                      for address, value in literals.items()}, ensure_ascii=True, indent=2))


if __name__ == "__main__":
    main()
