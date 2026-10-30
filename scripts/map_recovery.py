"""Read-only Ghidra export of source-path candidates and decoded call sites.

Requires the accepted baseline-c project. Outputs are local derived evidence,
not recovered source or proof of original translation-unit ownership.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import subprocess
import sys
import zipfile

from analyze_baseline import ROOT, SHA256, ARCHIVE_SHA256, digest
from dump_rng_slice import load_pe, read_va


def rows(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def summarize(out, data, sections):
    candidates = {}
    references = rows(out / "source-references.tsv")
    for row in references:
        if not row["instruction"] or not row["function_va"]:
            continue
        address = int(row["from_va"], 16)
        original = bytes.fromhex(row["bytes"])
        if read_va(data, sections, address, len(original)) != original:
            raise ValueError(f"Instruction bytes disagree at {address:08x}")
        item = candidates.setdefault(row["function_va"], {"paths": set(), "references": []})
        item["paths"].add(row["path"].replace("/", "\\").casefold())
        item["references"].append(row)
    ledger = []
    for va, item in sorted(candidates.items()):
        paths = sorted(item["paths"])
        ledger.append(dict(function_va=va, candidate_paths=paths,
            ownership="single-path candidate" if len(paths) == 1 else "ambiguous multiple paths",
            evidence=item["references"], compiled=False, validated=False, integrated=False))
    (out / "module-candidates.json").write_text(json.dumps(ledger, indent=2) + "\n", encoding="utf-8")
    calls = rows(out / "calls.tsv")
    for row in calls:
        original = bytes.fromhex(row["bytes"])
        if read_va(data, sections, int(row["site_va"], 16), len(original)) != original:
            raise ValueError(f"Call bytes disagree: {row['site_va']}")
    orchestration = [row for row in calls if row["function_va"] == "005cccd0"]
    (out / "orchestration-calls.json").write_text(json.dumps(orchestration, indent=2) + "\n", encoding="utf-8")
    summary = dict(source_path_strings=len(rows(out / "source-paths.tsv")),
        source_references=len(references), functions_with_candidates=len(ledger),
        ambiguous_functions=sum(len(item["candidate_paths"]) > 1 for item in ledger),
        call_records=len(calls), unique_call_sites=len({row["site_va"] for row in calls}),
        unresolved_call_sites=len({row["site_va"] for row in calls if row["reference_type"] == "UNRESOLVED"}),
        orchestration_call_sites=len({row["site_va"] for row in orchestration}),
        caveat="Ghidra instruction/function boundaries remain analysis hypotheses; no recovery coverage percentage.")
    (out / "summary.json").write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    return summary


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--functions", nargs="+", default=["005cccd0", "0081b890"])
    args = parser.parse_args()
    # Validate before creating output or starting Ghidra.
    selected = [f"{int(value, 16):08x}" for value in args.functions]
    baseline = ROOT / "CM0102/Analysis/baseline-c"
    target = baseline / "cm0102.exe"
    if digest(target) != SHA256 or digest(ROOT / "CM0102/Game/cm0102.exe") != SHA256:
        raise SystemExit("Input hash mismatch")
    prior = json.loads((baseline / "run.json").read_text())
    if prior["exit_code"] != 0 or prior["input_sha256"] != SHA256:
        raise SystemExit("Baseline analysis was not successful")
    ghidra = ROOT / "tools/ghidra/ghidra_12.1.4_PUBLIC"
    archive = ghidra.parent / "ghidra_12.1.4_PUBLIC_20260921.zip"
    if digest(archive) != ARCHIVE_SHA256:
        raise SystemExit("Ghidra archive hash mismatch")
    with zipfile.ZipFile(archive) as bundle:
        for member in bundle.infolist():
            if member.is_dir():
                continue
            if digest(ghidra.parent / member.filename) != hashlib.sha256(bundle.read(member)).hexdigest():
                raise SystemExit(f"Ghidra installation mismatch: {member.filename}")
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    data, sections = load_pe(target)
    # Extract directly from the pinned binary; the community catalog is not an input.
    paths = []
    for match in re.finditer(rb"[A-Za-z]:\\[^\x00\r\n]{1,240}\.(?:cpp|hpp|c|h)\x00", data, re.I):
        offset = match.start()
        section = next(s for s in sections if s["file_offset"] <= offset < s["file_offset"] + s["raw_size"])
        va = section["va"] + offset - section["file_offset"]
        paths.append((va, match.group()[:-1].decode("ascii")))
    with (out / "source-paths.tsv").open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(["va", "path"])
        writer.writerows((f"{va:08x}", path) for va, path in paths)
    java = Path("C:/Program Files/Java/jdk-21.0.10")
    command = [str(ghidra / "support/analyzeHeadless.bat"), str(baseline), "cm0102",
        "-process", "cm0102.exe", "-readOnly", "-noanalysis", "-scriptPath", str(ROOT / "scripts/ghidra"),
        "-postScript", "ExportRecoveryMap.java", str(out), *selected]
    scripts = [Path(__file__).resolve(), ROOT / "scripts/ghidra/ExportRecoveryMap.java",
               ROOT / "scripts/analyze_baseline.py", ROOT / "scripts/dump_rng_slice.py"]
    record = dict(input=str(target.relative_to(ROOT)), input_sha256=SHA256,
        address_convention="preferred-image VA; image base 0x00400000",
        baseline_manifest_sha256=digest(baseline / "run.json"),
        ghidra_version="12.1.4", archive_sha256=ARCHIVE_SHA256,
        command=command, python=sys.version, platform=platform.platform(),
        java=subprocess.run([str(java / "bin/java.exe"), "-version"], capture_output=True, text=True).stderr,
        scripts={str(p.relative_to(ROOT)): digest(p) for p in scripts})
    env = os.environ.copy()
    env["JAVA_HOME"] = str(java)
    with (out / "headless.log").open("w", encoding="utf-8") as log:
        result = subprocess.run(command, env=env, stdout=log, stderr=subprocess.STDOUT)
    record["exit_code"] = result.returncode
    record["preserved_input_sha256_after"] = digest(ROOT / "CM0102/Game/cm0102.exe")
    record["success"] = False
    try:
        if result.returncode or not (out / "export-complete.txt").exists():
            raise RuntimeError("Ghidra export failed; see headless.log")
        if record["preserved_input_sha256_after"] != SHA256:
            raise RuntimeError("Preserved input changed")
        if any(digest(ROOT / name) != value for name, value in record["scripts"].items()):
            raise RuntimeError("Scripts changed during export")
        record["summary"] = summarize(out, data, sections)
        record["success"] = True
    finally:
        record["outputs"] = {p.name: digest(p) for p in sorted(out.iterdir()) if p.is_file() and p.name != "run.json"}
        (out / "run.json").write_text(json.dumps(record, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(record["summary"], indent=2))


if __name__ == "__main__":
    main()
