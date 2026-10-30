"""Verify preserved files and every official update.dat payload without writing."""
import json
from pathlib import Path
import struct
import sys

from prepare_game import CM, sha256


def main():
    failures = []
    checked = 0
    for folder, filename in ((CM / "Game", "game-manifest.json"),
                             (CM / "Original-3.9.60", "original-manifest.json")):
        entries = json.loads((CM / "Inspection" / filename).read_text(encoding="utf-8"))
        expected = {entry["path"].lower() for entry in entries}
        for entry in entries:
            path = folder / entry["path"]
            if not path.is_file() or path.stat().st_size != entry["size"] or sha256(path) != entry["sha256"]:
                failures.append(str(path))
            checked += 1
        extra = [p.relative_to(folder).as_posix() for p in folder.rglob("*")
                 if p.is_file() and p.relative_to(folder).as_posix().lower() not in expected]
        print(f"{folder.name}: {len(entries)} baseline files; additional runtime files: {extra}")
    payload = CM / "InstallMedia/Patch/Extracted/update.dat"
    entries = json.loads((CM / "Inspection/patch-payload-manifest.json").read_text())
    with payload.open("rb") as stream:
        sizes = struct.unpack("<46I", stream.read(184))
        for entry, size in zip(entries, sizes):
            if stream.tell() != entry["offset"] or size != entry["size"]:
                failures.append(f"Patch boundary: {entry['path']}")
            if stream.read(size) != (CM / "Game" / entry["path"]).read_bytes():
                failures.append(f"Patch bytes: {entry['path']}")
        if stream.read(1) or len(entries) != 46:
            failures.append("Patch payload count/length")
    for failure in failures:
        print(f"FAIL: {failure}")
    print(f"Checked {checked} baseline files and 46 patch payloads: {'FAIL' if failures else 'PASS'}")
    return bool(failures)


if __name__ == "__main__":
    sys.exit(main())
