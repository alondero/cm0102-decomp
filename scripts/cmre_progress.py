"""Export a local, offline recovery atlas using the existing cmre accounting.

Run with py -3.13. --workspace reads an authoritative checkout without copying
its game/evidence into a tooling worktree. No recovery records are written.
"""
from __future__ import annotations

import argparse
from datetime import datetime, timezone
import hashlib
import json
from pathlib import Path
import subprocess
import sys

SCHEMA = "cmre-progress/1"
ASSETS = Path(__file__).with_name("progress")


def module_label(paths: list[str]) -> str:
    """Candidate source ownership is uncertain; never publish absolute paths."""
    labels = set()
    for path in paths:
        parts = path.replace("\\", "/").split("/")
        code = [i for i, part in enumerate(parts) if part.lower() == "code"]
        label = "/".join(parts[code[-1] + 1:]) if code else parts[-1]
        if label and all(p not in (".", "..") and ":" not in p for p in label.split("/")):
            labels.add(label)
    if len(labels) > 1:
        return "Ambiguous module"
    return next(iter(labels), "Unknown module")


def snapshot(facts: dict, modules: dict[str, list[str]], *, generated_at: str, revision: str | None,
             source_kinds: dict[str, str]) -> dict:
    """One rectangle per inventory entry; byte weights never use ledger extents."""
    from cmre import shards

    accounting = shards.reconcile(facts)
    states = {f: state for shard in accounting["shards"]
              for state, ids in shard["functions"].items() for f in ids}
    source, lifted = set(facts["source"]), set(facts["lifted"])
    composed, integrated = set(facts["composed"]), set(facts["integrated"])
    rows = []
    for function, size in sorted(facts["inventory"]["functions"].items()):
        rows.append({"id": function, "bytes": size, "shard": shards.shard_of(int(function[3:], 16)),
                     "module": module_label(modules.get(function, [])), "state": states[function],
                     "source": function in source, "lifted": function in lifted,
                     "source_kind": source_kinds.get(function),
                     "composed": function in composed, "integrated": function in integrated})
    def measure(predicate):
        selected = [r for r in rows if predicate(r)]
        return {"functions": len(selected), "body_bytes": sum(r["bytes"] for r in selected)}
    totals = {"inventory": measure(lambda r: True),
              "source": measure(lambda r: r["source"]),
              "c": measure(lambda r: r["source_kind"] == "c"),
              "legacy": measure(lambda r: r["source_kind"] == "legacy"),
              "recovered": measure(lambda r: r["source"] or r["lifted"]),
              "composed": measure(lambda r: r["composed"]),
              "integrated": measure(lambda r: r["integrated"])}
    data = {"schema": SCHEMA, "generated_at": generated_at, "revision": revision,
            "input_sha256": facts["input_sha256"],
            "address_convention": "preferred-image VA; image base 0x00400000",
            "inventory_sha256": facts["inventory"]["sha256"], "totals": totals,
            "states": accounting["totals"]["states"],
            "outside_inventory": facts["outside"], "functions": rows}
    stable = {k: v for k, v in data.items() if k not in ("generated_at", "revision")}
    data["digest"] = hashlib.sha256(json.dumps(stable, sort_keys=True).encode()).hexdigest()
    return data


def capture() -> dict:
    from cmre import env, inventory, ledger, shards

    # inventory.load can reuse its cache. Check pinned inputs explicitly so a
    # missing or altered export cannot silently produce a fresh-looking atlas.
    for path, expected in env.INVENTORY_SHA256.items():
        if not path.is_file() or env.digest(path) != expected:
            raise ValueError(f"Missing or changed pinned inventory: {path}; see docs/ANALYSIS-SETUP.md")
    if not env.REGISTRY.is_file():
        raise ValueError(f"Missing local acceptance registry: {env.REGISTRY}; see docs/BUILDING.md")
    facts = shards.gather()
    # Legacy records are frozen. Derive C membership from the same captured
    # source set rather than rereading mutable acceptance records after gather.
    legacy = {f"fn_{e.va:08x}" for e in ledger.legacy_entries()}
    kinds = {function: "legacy" if function in legacy else "c" for function in facts["source"]}
    result = subprocess.run(["git", "rev-parse", "HEAD"], cwd=env.ROOT,
                            capture_output=True, text=True, check=False)
    revision = result.stdout.strip() if result.returncode == 0 else None
    return snapshot(facts, {f"fn_{va:08x}": paths for va, paths in inventory.load().modules.items()},
                    generated_at=datetime.now(timezone.utc).isoformat(), revision=revision, source_kinds=kinds)


def history_with(history: list[dict], data: dict) -> list[dict]:
    # Keep revisions of the inventory, but the chart compares only equal pins.
    point = {k: data[k] for k in ("generated_at", "revision", "input_sha256", "inventory_sha256",
                                 "digest", "totals")}
    if history and history[-1]["digest"] == data["digest"]:
        return history
    return [*history, point]


def render(data: dict, history: list[dict]) -> str:
    payload = json.dumps({"snapshot": data, "history": history}, separators=(",", ":"), ensure_ascii=True)
    # JSON sits in an HTML raw-text element: ordinary JSON escaping is insufficient.
    payload = payload.replace("&", "\\u0026").replace("<", "\\u003c").replace(">", "\\u003e")
    template = (ASSETS / "index.html").read_text(encoding="utf-8")
    return template.replace("/* PROGRESS_CSS */", (ASSETS / "style.css").read_text(encoding="utf-8")) \
        .replace("/* PROGRESS_JS */", (ASSETS / "app.js").read_text(encoding="utf-8")) \
        .replace("PROGRESS_DATA", payload)


def atomic_text(path: Path, value: str) -> None:
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_text(value, encoding="utf-8", newline="\n")
    temporary.replace(path)


def write_dashboard(output: Path | None = None) -> Path:
    from cmre import env
    from cmre.coordination import lock

    output = (output or env.ANALYSIS / "cmre/progress").resolve()
    output.mkdir(parents=True, exist_ok=True)
    with lock(output / ".export.lock"):
        data = capture()
        history_path = output / "history.json"
        history = json.loads(history_path.read_text(encoding="utf-8")) if history_path.exists() else []
        history = history_with(history, data)
        html = render(data, history)
        # A complete embedded snapshot makes index.html independently portable.
        atomic_text(output / "snapshot.json", json.dumps(data, indent=2) + "\n")
        atomic_text(history_path, json.dumps(history, indent=2) + "\n")
        atomic_text(output / "index.html", html)
    return output / "index.html"


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--workspace", type=Path, default=Path(__file__).resolve().parents[1],
                        help="read local progress from this checkout (default: this script's checkout)")
    parser.add_argument("--output", type=Path, help="output directory; defaults to the workspace's ignored analysis")
    args = parser.parse_args(argv)
    sys.path.insert(0, str(args.workspace.resolve()))
    try:
        print(f"Progress atlas: {write_dashboard(args.output)}")
    except (OSError, ValueError) as exc:
        print(str(exc), file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
