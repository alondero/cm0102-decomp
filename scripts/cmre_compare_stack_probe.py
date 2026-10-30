"""Audit accepted C build bytes with/without GCC stack probing (HARNESS.md).

Run before changing compiler inputs or accepted sources. This is a byte audit,
not differential validation; the whole inventory still needs revalidation.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import sys
import tempfile
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from cmre import build, env, ledger, lift, recover


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    run_tool = env.run_tool
    enabled = False

    def compile_with_probe_setting(command, *args, **kwargs):
        if "-c" in command and "-m32" in command:
            command = [arg for arg in command if arg != "-mno-stack-arg-probe"]
            if enabled:
                command.append("-mno-stack-arg-probe")
        return run_tool(command, *args, **kwargs)

    rows = []
    with tempfile.TemporaryDirectory(prefix="cmre-probe-audit-") as tmp, patch.object(env, "run_tool", compile_with_probe_setting):
        for entry in ledger.cmre_entries():
            if not entry.record.get("c"):
                continue
            source = env.ROOT / entry.record["c"]["source"]
            for opt in recover.C_OPT_LEVELS:
                enabled = False
                before = build.build(source, lift.symbol(entry.va), Path(tmp), opt)
                enabled = True
                after = build.build(source, lift.symbol(entry.va), Path(tmp), opt)
                row = {"va": f"{entry.va:#010x}", "source_sha256": env.digest(source), "opt": opt,
                       "changed_sections": [a.name for a, b in zip(before.image.sections, after.image.sections) if a != b],
                       "sections_equal": before.image.sections == after.image.sections,
                       "symbols_equal": before.symbols == after.symbols,
                       "before_text_sha256": env.digest_bytes(before.image.text.raw),
                       "after_text_sha256": env.digest_bytes(after.image.text.raw)}
                rows.append(row)
                if not row["sections_equal"] or not row["symbols_equal"]:
                    print(f'CHANGED {entry.va:#010x} {opt}', flush=True)
    report = {"input_sha256": env.TARGET_SHA256, "toolchain": env.tool("gcc"), "builds": rows}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    changed = sum(not r["sections_equal"] or not r["symbols_equal"] for r in rows)
    print(f"Compared {len(rows)} builds; {changed} changed; receipt: {args.output}")


if __name__ == "__main__":
    main()
