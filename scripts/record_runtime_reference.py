"""Freeze the session's selected saves/screenshots plus input hashes locally.

This records observed evidence, not a determinism or behavioral-parity test.
"""
import argparse
from datetime import datetime, timezone
import json
from pathlib import Path
import platform
import shutil
import subprocess
import sys

from prepare_runtime import ROOT, SHA256, GDI_SHA256, digest

ARTIFACTS = [
    "reference-start.sav", "reference-start-july28.sav", "game.cfg",
    "formation-layout-repair.json", "after-next.png", "after-error-ok.png",
    "repaired-initializing.png", "new-game-complete.png", "restored-start.png",
    "holiday-progress.png", "fixtures-july28.png", "match-result.png",
    "match-statistics.png", "saved-july28.png",
]
KNOWN_SAVES = {
    "reference-start.sav": "efbd92e5b32d86ea9031aba1832bcf4a7f2296ccea9e01134d126ed869341cc8",
    "reference-start-july28.sav": "96dae5260c9bceb46a79c341e26c0bf1a27afed93da123236cb1f0e2df5f89e5",
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--pid", type=int, required=True)
    args = parser.parse_args()
    runtime = args.runtime.resolve()
    local_root = (ROOT / "CM0102/Runtime").resolve()
    out = args.output.resolve()
    if not runtime.is_relative_to(local_root) or not out.is_relative_to(local_root):
        raise ValueError("Runtime and output must remain under CM0102/Runtime")
    for name, expected in [("cm0102.exe", SHA256), ("cm0102_GDI.exe", GDI_SHA256)]:
        if digest(runtime / name) != expected:
            raise ValueError(f"Executable mismatch: {name}")
    for name in ARTIFACTS:
        if not (runtime / name).is_file():
            raise ValueError(f"Missing artifact: {name}")
    for name, expected in KNOWN_SAVES.items():
        if digest(runtime / name) != expected:
            raise ValueError(f"This recorder is pinned to the documented session saves: {name}")
    # pid is parsed as an integer; no user text is interpolated into shell code.
    command = f"$p=Get-Process -Id {args.pid}; $c=Get-CimInstance Win32_Process -Filter 'ProcessId={args.pid}'; [pscustomobject]@{{pid=$p.Id; executable=$p.MainModule.FileName; module_base=$p.MainModule.BaseAddress.ToInt64(); command_line=$c.CommandLine}} | ConvertTo-Json"
    process = json.loads(subprocess.check_output(["powershell", "-NoProfile", "-Command", command], text=True))
    if Path(process["executable"]).resolve() != runtime / "cm0102_GDI.exe":
        raise ValueError("PID does not belong to this runtime GDI executable")
    baseline = json.loads((ROOT / "CM0102/Inspection/game-manifest.json").read_text())
    inputs = []
    for row in baseline:
        path = runtime / row["path"]
        if digest(path) != row["sha256"]:
            raise ValueError(f"Runtime baseline file changed: {path}")
        inputs.append(row)
    repair = json.loads((runtime / "formation-layout-repair.json").read_text())
    for change in repair["files"]:
        path = runtime / "Data" / Path(change["source"]).name
        if digest(path) != change["sha256"]:
            raise ValueError(f"Repaired formation file changed: {path}")
        relative = path.relative_to(runtime).as_posix()
        if relative.casefold() not in {row["path"].casefold() for row in inputs}:
            inputs.append(dict(path=relative, size=path.stat().st_size, sha256=digest(path)))
    out.mkdir(parents=True, exist_ok=False)
    files = []
    for name in ARTIFACTS:
        before = digest(runtime / name)
        shutil.copy2(runtime / name, out / name)
        if digest(out / name) != before or digest(runtime / name) != before:
            raise ValueError(f"Artifact changed during capture: {name}")
        files.append(dict(path=name, size=(out / name).stat().st_size, sha256=before))
    record = dict(recorded_utc=datetime.now(timezone.utc).isoformat(), runtime=str(runtime),
        python=sys.version, platform=platform.platform(), command=sys.argv, process=process,
        script_sha256=digest(Path(__file__)),
        scenario=dict(seed_argument=1, database="Minimum (observed during original settings setup)",
            language="English", leagues="England through Conference; other countries inactive",
            manager="Reference Manager", club="Arsenal", nationality="English",
            start_date="2001-07-14 AM", end_date="2001-07-28 AM",
            progression="Restore starting save; Go on Holiday with default options; return on July 28",
            match=dict(date="2001-07-19", home="Trabzonspor", away="Arsenal", score=[1, 0],
                       shots=[9, 7], shots_on_target=[2, 5]),
            limitations=["One run only; determinism untested", "GDI only; primary runtime unvalidated",
                         "Holiday stopping time was manual; no deterministic input replay yet"]),
        runtime_baseline_files=inputs, artifacts=files)
    (out / "reference.json").write_text(json.dumps(record, indent=2) + "\n", encoding="utf-8")
    print(f"Frozen {len(files)} artifacts and {len(inputs)} input hashes in {out}")


if __name__ == "__main__":
    main()
