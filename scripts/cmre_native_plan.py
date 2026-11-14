"""Read-only native coordination dispatch lint; not a scheduler or sandbox.

Run with py -3.13 scripts/cmre_native_plan.py PLAN.json. Exit 2 rejects a plan.
The policy contract is docs/agent/archive/NATIVE-COORDINATION.md (retired 2026-11-13; see FARM.md).
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
MODELS = {
    "expert": "openai/gpt-6.1-sol",
    "senior": "opencode-go/deepseek-v4.1-flash",
    "routine": "minimax-coding-plan/MiniMax-M3.1-Flash",
}
ROLE_CLASSES = {
    "architecture": {"expert"}, "arbitration": {"expert"},
    "integration": {"expert"}, "acceptance": {"expert"},
    "validation": {"expert"}, "preflight": {"expert"},
    "investigation": {"senior", "expert"}, "inputs": {"senior", "expert"},
    "review": {"senior", "expert"}, "emitted_review": {"senior", "expert"},
    "authoring": set(MODELS), "corrections": set(MODELS), "reporting": set(MODELS),
}


def text(value):
    return (isinstance(value, str) and 0 < len(value) <= 500 and bool(value.strip())
            and not any(c in value for c in "\r\n\x00"))


def require(condition, message):
    if not condition:
        raise ValueError(message)


def pin(value, root):
    require(isinstance(value, dict) and set(value) == {"path", "sha256"}
            and text(value["path"]) and isinstance(value["sha256"], str), "Invalid file pin")
    path = root / value["path"]
    require(path.is_file() and hashlib.sha256(path.read_bytes()).hexdigest() == value["sha256"],
            "Missing or changed pinned file: " + value["path"])
    return path.resolve()


def validate_plan(data, *, root=None):
    """Validate declared identities/ownership, never actual caller identity or fidelity."""
    from scripts.cmre_session import load_parent

    root = Path(ROOT if root is None else root).resolve()
    require(isinstance(data, dict) and set(data) == {
        "schema", "mode", "authorization", "parent_receipt", "workers", "roles",
        "steward", "writer_slots", "tasks"}, "Invalid native plan fields")
    require(data["schema"] == "cmre-native-coordination/1"
            and data["mode"] == "coordination-only" and text(data["authorization"]),
            "Native plan requires coordination-only mode and user authorization reference")
    parent = pin(data["parent_receipt"], root)
    try:
        load_parent(parent)
    except (AttributeError, TypeError, KeyError) as exc:
        raise ValueError("Malformed inherited preflight receipt") from exc
    require(type(data["writer_slots"]) is int and data["writer_slots"] == 1,
            "Exactly one serialized canonical writer/test slot is required")
    rows = data["workers"]
    require(isinstance(rows, list) and 1 <= len(rows) <= 20, "Require 1..20 workers")
    workers = {}
    for row in rows:
        require(isinstance(row, dict) and set(row) == {
            "id", "class", "model", "role", "preflight", "parent_receipt"}, "Invalid worker fields")
        require(text(row["id"]) and row["id"] not in workers and row["id"] != "parent",
                "Worker IDs must be unique and not parent")
        require(isinstance(row["class"], str) and row["class"] in MODELS
                and row["model"] == MODELS[row["class"]], "Unauthorized worker model/class")
        require(row["role"] == "WORKER" and row["preflight"] == "inherit"
                and row["parent_receipt"] == data["parent_receipt"],
                "Every worker must explicitly inherit the same pinned preflight as ROLE=WORKER")
        workers[row["id"]] = row
    roles = data["roles"]
    require(isinstance(roles, dict) and set(roles) == set(ROLE_CLASSES),
            "Assign every substantive role; parent is coordination only")
    for role, owner in roles.items():
        require(isinstance(owner, str) and owner in workers
                and workers[owner]["class"] in ROLE_CLASSES[role], "Wrong model class for role: " + role)
    steward = data["steward"]
    require(isinstance(steward, str) and steward in workers
            and workers[steward]["class"] == "expert", "Require an expert steward")
    require(all(roles[r] == steward for r in ("integration", "acceptance", "validation", "preflight")),
            "One expert steward owns preflight, integration, acceptance and validation")
    require(roles["authoring"] != roles["review"] and roles["authoring"] != steward,
            "Authoring role requires an independent reviewer and non-author steward")
    tasks = data["tasks"]
    require(isinstance(tasks, list) and 1 <= len(tasks) <= 20, "Require 1..20 bounded tasks")
    ids, paths = set(), set()
    for task in tasks:
        require(isinstance(task, dict) and set(task) == {
            "id", "kind", "author", "reviewer", "steward", "writable_paths", "bounds", "state", "pins"},
            "Invalid task fields")
        require(text(task["id"]) and task["id"] not in ids, "Task IDs must be unique")
        ids.add(task["id"])
        kind = task["kind"]
        require(isinstance(kind, str) and kind in {"routine", "hard", "expert"}, "Invalid task class")
        author, reviewer = task["author"], task["reviewer"]
        require(isinstance(author, str) and isinstance(reviewer, str)
                and author in workers and reviewer in workers, "Task owners must be assigned workers")
        require(workers[author]["class"] in {
            "routine": set(MODELS), "hard": {"senior", "expert"}, "expert": {"expert"}}[kind],
            "Task author class is too low")
        require(workers[reviewer]["class"] in {"senior", "expert"}
                and author != reviewer and author != steward and task["steward"] == steward,
                "Require independent senior/expert review and a separate non-author expert steward")
        bounds = task["bounds"]
        require(isinstance(bounds, dict) and set(bounds) == {
            "max_messages", "max_seconds", "remaining_failures"}, "Task requires structured bounds")
        for field, limit in (("max_messages", 30), ("max_seconds", 86400), ("remaining_failures", 5)):
            require(type(bounds[field]) is int and (0 if field == "remaining_failures" else 1)
                    <= bounds[field] <= limit, "Invalid task bound: " + field)
        writable = task["writable_paths"]
        require(isinstance(writable, list) and writable, "Task requires exact private writable paths")
        for name in writable:
            require(text(name) and not any(c in name for c in "*?[]"), "Use exact paths, not patterns")
            path = (root / name).resolve()
            private_root = (root / "CM0102/Analysis/cmre").resolve()
            require(private_root in path.parents, "Private paths must stay below CM0102/Analysis/cmre")
            require(not path.is_dir() and path != root.resolve()
                    and all(path != other and path not in other.parents and other not in path.parents
                            for other in paths), "Private task paths must not overlap or name directories")
            paths.add(path)
        require(task["state"] in ("planned", "ready"), "Task state must be planned or ready")
        require(isinstance(task["pins"], dict), "Task pins must be an object")
        if task["state"] == "ready":
            require(set(task["pins"]) == {"source", "hints", "report", "verdict"},
                    "Ready bundles require source/hints/report/verdict pins")
            for value in task["pins"].values():
                pin(value, root)
            verdict = json.loads((root / task["pins"]["verdict"]["path"]).read_text(encoding="utf-8-sig"))
            require(isinstance(verdict, dict) and verdict.get("schema") == "cmre-native-review/1"
                    and verdict.get("author") == author and verdict.get("reviewer") == reviewer
                    and verdict.get("task") == task["id"] and verdict.get("verdict") == "approve"
                    and verdict.get("pins") == {k: task["pins"][k] for k in ("source", "hints", "report")},
                    "Review verdict must approve this exact author/reviewer/task/source/hints/report bundle")
        else:
            require(not task["pins"], "Planned tasks have no acceptance-ready pins")
    return {"mode": data["mode"], "workers": len(workers), "tasks": len(tasks),
            "ready": sum(t["state"] == "ready" for t in tasks), "steward": steward,
            "note": "Declared plan only; no dispatch, permissions, model identity or acceptance enforced."}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("plan", type=Path)
    args = parser.parse_args(argv)
    try:
        payload = args.plan.read_bytes()
        require(len(payload) <= 65536, "Plan exceeds 64 KiB")
        print(json.dumps(validate_plan(json.loads(payload.decode("utf-8-sig"))), indent=2))
    except (OSError, ValueError, TypeError) as exc:
        print(str(exc), file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.path.insert(0, str(ROOT))
    raise SystemExit(main())
