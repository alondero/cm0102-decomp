"""Reconstruct the local 3.9.68 inspection baseline from the supplied media.

Requires the ISO mounted and its cabinet extracted with unshield -R.
Only accepts empty destination folders; original media are never modified.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import stat
import struct

ROOT = Path(__file__).resolve().parents[1]
CM = ROOT / "CM0102"


def sha256(path):
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def manifest(folder):
    return [{"path": p.relative_to(folder).as_posix(), "size": p.stat().st_size,
             "sha256": sha256(p)} for p in sorted(folder.rglob("*")) if p.is_file()]


def write_json(path, value):
    path.write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, required=True)
    args = parser.parse_args()
    base = CM / "Original-3.9.60"
    game = CM / "Game"
    payload = CM / "InstallMedia/BasePayload"
    patch = CM / "InstallMedia/Patch/Extracted"
    inspection = CM / "Inspection"
    for name, expected in {
        "setup.exe": "7ca2850ecb21b20ead2a03bd12ec97c53b088086b4225926a5181521bd7eedec",
        "update.dat": "1f8141d5baf0f8a99d7e1a3373ba2feeb9e0ddbdf4863b1ad29da2cada7fc918",
    }.items():
        if sha256(patch / name) != expected:
            raise SystemExit(f"This reconstruction supports only the recorded 3.9.68 {name}")
    inspection.mkdir(parents=True, exist_ok=True)
    for dest in (base, game):
        if dest.exists() and any(dest.iterdir()):
            raise SystemExit(f"Refusing to overwrite nonempty destination: {dest}")
    groups = {
        "CM3_Data": "Data", "SI_Data": "Data", "Editor": "Editor",
        "Hall_Of_Fame": "", "History": "History", "Pictures_cfg": "Pictures",
        "Pictures_Std": "Pictures", "Picture_Tools": "Pictures",
        # The cabinet group is named Tactics, but the game's formation loader
        # opens data\\352_v1.pct (and the other supplied .pct files).
        # See docs/RUNTIME.md. Never rerun over the preserved historical layout.
        "Sounds": "Sounds", "Tactics": "Data",
    }
    for group in payload.iterdir():
        target = "qstart" if group.name.startswith("~QS_") else groups[group.name]
        shutil.copytree(group, base / target, dirs_exist_ok=True)
    for name in ("cm0102.exe", "cm0102_GDI.exe", "langtool.exe", "ip.exe"):
        shutil.copy2(args.disc / name, base / name)
    shutil.copytree(args.disc / "MANUAL", base / "Manual")
    shutil.copytree(base, game, dirs_exist_ok=True)
    for file in game.rglob("*"):
        if file.is_file():
            file.chmod(stat.S_IREAD | stat.S_IWRITE)

    # The official setup.exe table at VA 0x410070 contains 46 pairs of
    # filename/directory pointers. Its PE sections map VA - 0x400000 to offset.
    # Its unpacker at VA 0x403430 reads 46 uint32 lengths, then raw file bytes.
    setup = (patch / "setup.exe").read_bytes()
    data = (patch / "update.dat").read_bytes()
    sizes = struct.unpack_from("<46I", data)
    if sum(sizes) + 184 != len(data):
        raise ValueError("Unexpected patch payload size")

    def cstring(va):
        off = va - 0x400000
        if not 0 <= off < len(setup):
            raise ValueError("Invalid patch string pointer")
        return setup[off:setup.index(0, off)].decode("ascii")

    records = []
    offset = 184
    for i, size in enumerate(sizes):
        name_va, dir_va = struct.unpack_from("<II", setup, 0x10070 + i * 8)
        name, directory = cstring(name_va), cstring(dir_va)
        if directory not in ("\\", "\\data\\", "\\editor\\") or Path(name).name != name:
            raise ValueError("Unexpected patch destination")
        rel = Path(directory.strip("\\").capitalize()) / name
        dest = game / rel
        blob = data[offset:offset + size]
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(blob)
        records.append({"path": rel.as_posix(), "offset": offset, "size": size,
                        "sha256": hashlib.sha256(blob).hexdigest()})
        offset += size
    # Loose patch resources accompany update.dat; retain every language readme.
    for source in patch.iterdir():
        if source.suffix.lower() in (".fnt", ".t2k", ".ttf"):
            shutil.copy2(source, game / "Data" / source.name)
        elif source.name.startswith("readme_"):
            shutil.copy2(source, game / source.name)
    shutil.copy2(patch / "readme_eng.txt", game / "readme.txt")
    # The official updater deletes old quick starts (see 0x403004). Preserve
    # those original database snapshots only in Original-3.9.60.
    for quickstart in (game / "qstart").glob("*.qst"):
        quickstart.unlink()
    for folder, filename in ((base, "original-manifest.json"), (game, "game-manifest.json")):
        write_json(inspection / filename, manifest(folder))
    write_json(inspection / "patch-payload-manifest.json", records)
    print(f"Prepared {game}; extracted and hashed {len(records)} patch payloads.")


if __name__ == "__main__":
    main()
