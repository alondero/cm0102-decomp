"""Copy preserved formation assets to Data on a disposable runtime copy only.

The GDI formation loader requests data\\352_v1.pct. The historical extraction
placed the cabinet's Tactics group in Tactics. Preserve that baseline; record
this additive runtime layout correction separately.
"""
import argparse
import json
from pathlib import Path
import shutil
import sys

from prepare_runtime import ROOT, SOURCE, SHA256, GDI_SHA256, digest


def repair(runtime: Path):
    runtime = runtime.resolve()
    runtime_root = (ROOT / "CM0102/Runtime").resolve()
    if runtime == runtime_root or not runtime.is_relative_to(runtime_root):
        raise ValueError("Destination must be a copy beneath CM0102/Runtime")
    for name, expected in [("cm0102.exe", SHA256), ("cm0102_GDI.exe", GDI_SHA256)]:
        if digest(runtime / name) != expected:
            raise ValueError(f"Runtime executable mismatch: {name}")
    manifest_path = ROOT / "CM0102/Inspection/game-manifest.json"
    manifest = json.loads(manifest_path.read_text())
    entries = [r for r in manifest if r["path"].lower().startswith(("tactics/", "data/")) and r["path"].lower().endswith(".pct")]
    if len(entries) != 40 or len({Path(row["path"]).name.lower() for row in entries}) != 40:
        raise ValueError("Expected the 40 preserved formation files")
    changes = []
    for row in entries:
        source = SOURCE / row["path"]
        destination = runtime / "Data" / source.name
        if not destination.resolve().is_relative_to(runtime):
            raise ValueError("Destination escapes runtime copy")
        if digest(source) != row["sha256"]:
            raise ValueError(f"Preserved asset mismatch: {source}")
        if destination.exists() and digest(destination) != row["sha256"]:
            raise ValueError(f"Refusing to overwrite different file: {destination}")
        changes.append(dict(source=str(source.relative_to(ROOT)), destination=str(destination.relative_to(ROOT)),
                            sha256=row["sha256"], already_present=destination.exists()))
    evidence = runtime / "formation-layout-repair.json"
    # Keep the first transformation record, including its original preconditions.
    if evidence.exists():
        if not all(change["already_present"] for change in changes):
            raise ValueError("Prior repair manifest exists but assets are missing")
        print("Formation layout already repaired; original manifest retained")
        return
    for change in changes:
        if not change["already_present"]:
            shutil.copy2(ROOT / change["source"], ROOT / change["destination"])
        if digest(ROOT / change["destination"]) != change["sha256"]:
            raise ValueError("Copied asset mismatch")
    record = dict(reason="GDI formation.cpp line 2272: data\\352_v1.pct open failed",
                  python=sys.version, command=sys.argv, script_sha256=digest(Path(__file__)),
                  baseline_manifest_sha256=digest(manifest_path), files=changes)
    evidence.write_text(json.dumps(record, indent=2) + "\n", encoding="utf-8")
    print(f"Verified {len(changes)} formation assets in runtime Data; recorded {evidence}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, required=True)
    repair(parser.parse_args().runtime)
