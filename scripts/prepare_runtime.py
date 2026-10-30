"""Copy the preserved 3.9.68 install into an empty disposable runtime folder."""
from __future__ import annotations

import argparse
import hashlib
import json
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "CM0102/Game"
SHA256 = "0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86"
GDI_SHA256 = "0e50572c292bdfb9de8b3e087dd29043a13bcd0985ed198c9011e5a0f694388b"


def digest(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    exe = SOURCE / "cm0102.exe"
    gdi = SOURCE / "cm0102_GDI.exe"
    if digest(exe) != SHA256 or digest(gdi) != GDI_SHA256:
        raise SystemExit("refusing to copy: preserved executable hash mismatch")
    out = args.output.resolve()
    if out.exists():
        raise SystemExit(f"refusing nonempty/existing path: {out}")
    shutil.copytree(SOURCE, out)
    cfg = out / "game.cfg"
    if cfg.exists():
        cfg.unlink()
    record = {
        "source": str(SOURCE.relative_to(ROOT)).replace("\\", "/"),
        "output": str(out.relative_to(ROOT)).replace("\\", "/") if out.is_relative_to(ROOT) else str(out),
        "cm0102_exe_sha256": digest(out / "cm0102.exe"),
        "cm0102_gdi_exe_sha256": digest(out / "cm0102_GDI.exe"),
        "file_count": sum(1 for p in out.rglob("*") if p.is_file()),
    }
    (out / "runtime-copy.json").write_text(json.dumps(record, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(record, indent=2))
    if record["cm0102_exe_sha256"] != SHA256 or record["cm0102_gdi_exe_sha256"] != GDI_SHA256:
        raise SystemExit("copy hash mismatch")
    return 0


if __name__ == "__main__":
    sys.exit(main())
