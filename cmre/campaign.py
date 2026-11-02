"""Bounded milestone planning and primary-runtime observation comparison.

These reports never write recovery ledgers or promote milestone acceptance.
Inventory edges describe known direct calls, not complete dependency closure.
"""
from __future__ import annotations

import json
import re
import time
from pathlib import Path

from . import env, inventory, ledger

SCHEMA = "cmre-campaign/1"
REPLAY_SCHEMA = "cmre-replay-observations/1"
MILESTONES = ("M2", "M3", "M4", "M5", "M6")


def _va(value: int | str) -> int:
    if isinstance(value, str):
        value = int(value.removeprefix("fn_"), 16)
    if type(value) is not int or not env.IMAGE_BASE <= value <= 0xFFFFFFFF:
        raise ValueError("function must be a preferred-image VA")
    return value


def _id(va: int) -> str:
    return f"fn_{va:08x}"


def create(name: str, functions: list[int | str], milestone: str = "M5") -> dict:
    """Create a deterministic manifest from an explicit, nonempty function set."""
    if not isinstance(name, str) or not name.strip():
        raise ValueError("campaign name is required")
    if milestone not in MILESTONES:
        raise ValueError(f"milestone must be one of {MILESTONES}")
    selected = sorted({_va(v) for v in functions})
    if not selected:
        raise ValueError("select at least one function explicitly")
    inv = inventory.load()
    missing = [_id(v) for v in selected if v not in inv.functions]
    if missing:
        raise ValueError(f"functions absent from pinned inventory: {missing}")
    return {
        "schema": SCHEMA,
        "name": name.strip(),
        "input_sha256": env.TARGET_SHA256,
        "address_convention": env.ADDRESS_CONVENTION,
        "milestone": milestone,
        "functions": [_id(v) for v in selected],
        "runtime_lane": {"milestone": "M2", "status": "evidence-required",
                         "next": "Compare repeated primary scenarios with observed consumed inputs."},
    }


def _validate(manifest: dict) -> dict:
    if not isinstance(manifest, dict) or manifest.get("schema") != SCHEMA:
        raise ValueError("unsupported campaign schema")
    if manifest.get("input_sha256") != env.TARGET_SHA256:
        raise ValueError("campaign target differs from pinned primary executable")
    if manifest.get("address_convention") != env.ADDRESS_CONVENTION:
        raise ValueError("campaign address convention differs")
    if not isinstance(manifest.get("functions"), list):
        raise ValueError("campaign functions must be an explicit list")
    # Revalidate identity and membership, ignoring any caller-supplied state claims.
    return create(manifest.get("name"), manifest["functions"], manifest.get("milestone"))


def save(path: str | Path, manifest: dict) -> None:
    """Write a new planning artifact, never replace an existing file."""
    value = _validate(manifest)
    path = Path(path).resolve()
    protected = [env.ROOT / "CM0102" / n for n in
                 ("Game", "Original-3.9.60", "InstallMedia", "Inspection")]
    protected += [env.RECOVERY / n for n in ("date", "lang", "crt", "rng", "ledger", "lifted", "blocked")]
    if any(path.is_relative_to(p.resolve()) for p in protected) or path == env.REGISTRY.resolve():
        raise ValueError("campaign output is inside preserved recovery or game material")
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("x", encoding="utf-8", newline="\n") as stream:
        json.dump(value, stream, indent=2)
        stream.write("\n")


def load(path: str | Path) -> dict:
    return _validate(json.loads(Path(path).read_text(encoding="utf-8")))


def _states(entry) -> dict:
    if entry is None:
        return {"lifted": False, "c": False, "validated": False,
                "composed": "unrecorded", "integrated": False}
    record = entry.record
    flags = record if entry.legacy else record.get("status", {})
    return {"lifted": bool(record.get("lifted")),
            "c": flags.get("decompiled") is True if entry.legacy else bool(record.get("c")),
            "validated": flags.get("validated") is True,
            "composed": "unrecorded", "integrated": flags.get("integrated") is True}


def status(manifest: dict) -> dict:
    """Read live records; preserve unknown imports, indirect calls and data."""
    manifest = _validate(manifest)
    inv, entries = inventory.load(), ledger.recovered()
    selected = {_va(v) for v in manifest["functions"]}
    states = {v: _states(e) for v, e in entries.items()}
    rows, candidates, edges = [], [], []
    for va in sorted(selected):
        entry = entries.get(va)
        state = _states(entry)
        callees = sorted(set(inv.callees.get(va, [])))
        pending = [v for v in callees if not states.get(v, {}).get("c")]
        boundaries = entry.record.get("boundaries", {}) if entry else {}
        imports = boundaries.get("imports")
        rows.append({"function": _id(va), "body_bytes": inv.functions[va].body_bytes,
                     "states": state, "ledger": entry.source if entry else None,
                     "known_direct_callees": [_id(v) for v in callees],
                     "callees_without_c": [_id(v) for v in pending],
                     "outside_slice": [_id(v) for v in callees if v not in selected],
                     "imports": {"status": "recorded" if imports is not None else "unknown",
                                 "boundaries": imports},
                     "original_data": "unaccounted",
                     "indirect_calls": "not-proven-complete"})
        edges.extend({"caller": _id(va), "callee": _id(v),
                      "callee_c_available": bool(states.get(v, {}).get("c")),
                      "execution_binding": "original-until-composition-proven"} for v in callees)
        if state["lifted"] and not state["c"]:
            candidates.append((bool(pending), inv.functions[va].body_bytes, va, len(pending)))
    selected_bytes = sum(r["body_bytes"] for r in rows)
    source_bytes = sum(r["body_bytes"] for r in rows if r["states"]["c"])
    return {"schema": "cmre-campaign-status/1", "name": manifest["name"],
            "input_sha256": env.TARGET_SHA256, "milestone": manifest["milestone"],
            "functions": rows,
            "counts": {key: sum(r["states"][key] is True for r in rows)
                       for key in ("lifted", "c", "validated", "integrated")},
            "source_coverage": {"selected_body_bytes": selected_bytes,
                                "accepted_c_legacy_body_bytes": source_bytes,
                                "accepted_c_legacy_percent": round(100 * source_bytes / selected_bytes, 3)
                                if selected_bytes else 0.0},
            "c_queue": [{"function": _id(va), "c_ready": not waiting,
                         "pending_callees": count} for waiting, _, va, count in sorted(candidates)],
            "known_direct_edges": edges,
            "composition": {"status": "not-validated", "closed": False,
                            "required": "Composed recovered-call build and differential evidence."},
            "runtime_lane": manifest["runtime_lane"],
            "milestone_accepted": False,
            "limitations": ["Record presence is not fresh revalidation.",
                            "Known direct calls exclude unresolved indirect/data/API dependencies.",
                            "C availability does not establish a recovered execution binding."]}


def plan(manifest: dict, workers: int = 2) -> dict:
    """Plan bounded packet work without preparing, claiming, publishing or accepting.

    Live leases reserve their function even when inputs drift. Existing current
    submissions take precedence over new translations; report only token-free
    task identifiers, never full manifests or bearer credentials.
    """
    from . import tasks, work

    if type(workers) is not int or workers < 1:
        raise ValueError("workers must be a positive integer within authorized runtime capacity")
    report = status(manifest)
    packets = tasks.status()
    work_records = work.read()['functions']
    result = {"schema": "cmre-campaign-plan/1", "name": report["name"],
              "input_sha256": env.TARGET_SHA256, "milestone": report["milestone"],
              "source_coverage": report["source_coverage"], "worker_limit": workers,
              "dispatch": [], "prepare": [], "active": [], "review": [], "blocked": [], "stale_tasks": [],
              "milestone_accepted": False,
              "limitations": ["Read-only advice; task claim and publication recheck current inputs.",
                              "A present ABI contract still needs analyst review and task prepare byte checks.",
                              "Worker limit must also fit user authorization and available runtime capacity.",
                              "Source-byte coverage is separate from composition, live integration and M6."]}
    dispatchable, preparable = [], []
    # Dependency priority follows the campaign C queue, never a global smallest-first queue.
    order = {row["function"]: i for i, row in enumerate(report["c_queue"])}
    for row in sorted(report["functions"], key=lambda r: order.get(r["function"], len(order))):
        function = row["function"]
        va = _va(function)
        related = sorted((p for p in packets if int(p["va"], 16) == va),
                         key=lambda p: p["created_at"], reverse=True)
        base = {"function": function, "body_bytes": row["body_bytes"]}
        hold = work.dispatch_hold(va, work_records)
        publishing = [p for p in related if p["status"] == "publishing"]
        if publishing:
            result["review"].extend({**base, "task_id": p["task_id"], "action": "reconcile-publication"}
                                    for p in publishing)
            continue
        live = [p for p in related if p.get("lease") and p["lease"]["expires_at"] > time.time()]
        if live:
            for p in live:
                item = {**base, "task_id": p["task_id"], "expires_at": p["lease"]["expires_at"]}
                if hold:
                    item['hold'] = hold
                try:
                    tasks.validate_current(p)
                except (ValueError, OSError) as exc:
                    item["input_error"] = str(exc)
                result["active"].append(item)
            continue
        if row["states"]["c"]:
            continue
        current = []
        for p in related:
            try:
                tasks.validate_current(p)
                current.append(p)
            except (ValueError, OSError) as exc:
                result["stale_tasks"].append({**base, "task_id": p["task_id"], "reason": str(exc)})
        if current:
            p = current[0]
            item = {**base, "task_id": p["task_id"]}
            if len(current) > 1:
                result["blocked"].append({**item, "reason": "multiple-current-packets-require-review"})
            elif p["status"] == "submitted":
                result["review"].append({**item, "action": "review-submission-before-serial-publication",
                                         **({'hold': hold} if hold else {})})
            elif hold:
                result["blocked"].append({**item, **hold})
            elif p["status"] in ("prepared", "claimed") and len(p["attempts"]) < tasks.MAX_ATTEMPTS:
                if not p.get("abi", {}).get("analyst_contract"):
                    result["blocked"].append({**item, "reason": "abi-evidence-required"})
                else:
                    dispatchable.append({**item, "action": "claim-existing-packet"})
            else:
                result["blocked"].append({**item, "reason": p["status"], "action": "inspect-preserved-attempts"})
        elif hold:
            result["blocked"].append({**base, **hold})
        elif not row["states"]["lifted"]:
            result["blocked"].append({**base, "reason": "accepted-lift-required"})
        elif row["body_bytes"] > tasks.MAX_BODY_BYTES:
            result["blocked"].append({**base, "reason": "outside-packet-lane"})
        elif not (env.C_DIR / "contracts" / f"fn_{va:08x}.json").is_file():
            result["blocked"].append({**base, "reason": "abi-evidence-required"})
        else:
            preparable.append({**base, "contract_status": "present-unverified",
                               "command": f"py -3.13 -m cmre task prepare {function}"})
    slots = max(0, workers - len(result["active"]))
    result["dispatch"] = dispatchable[:slots]
    result["prepare"] = preparable[:max(0, slots - len(result["dispatch"]))]
    return result


def _hash(value, field: str) -> str:
    if not isinstance(value, str) or re.fullmatch(r"[0-9a-f]{64}", value) is None:
        raise ValueError(f"{field} must be a lowercase SHA-256")
    return value


def _replay(value: dict) -> dict:
    if not isinstance(value, dict) or value.get("schema") != REPLAY_SCHEMA:
        raise ValueError("unsupported replay observation schema")
    if value.get("input_sha256") != env.TARGET_SHA256:
        raise ValueError("replay must identify the pinned primary executable")
    base = value.get("actual_module_base")
    if type(base) is not int or not 0 < base <= 0xFFFFFFFF:
        raise ValueError("replay requires actual_module_base as an unsigned address")
    for field in ("scenario_sha256", "environment_sha256", "initial_state_sha256",
                  "observer_evidence_sha256"):
        _hash(value.get(field), field)
    checkpoints = value.get("checkpoints")
    if not isinstance(checkpoints, list) or len(checkpoints) < 2:
        raise ValueError("replay requires initial and subsequent checkpoints")
    seen, previous = set(), -1
    for checkpoint in checkpoints:
        if not isinstance(checkpoint, dict):
            raise ValueError("checkpoint must be an object")
        name = checkpoint.get("id")
        if not isinstance(name, str) or not name or name in seen:
            raise ValueError("checkpoint IDs must be nonempty and unique")
        seen.add(name)
        count = checkpoint.get("consumed_input_count")
        if type(count) is not int or count < 0 or count < previous:
            raise ValueError("consumed input counts must be nonnegative and monotonic")
        previous = count
        if checkpoint.get("consumption") != "observed":
            raise ValueError("posted inputs are insufficient; consumption must be observed")
        for field in ("consumed_inputs_sha256", "state_sha256", "rng_sha256", "ack_evidence_sha256"):
            _hash(checkpoint.get(field), field)
    if checkpoints[0]["consumed_input_count"] != 0 or previous == 0:
        raise ValueError("replay must start before input and observe subsequent consumption")
    if checkpoints[0]["state_sha256"] != value["initial_state_sha256"]:
        raise ValueError("initial checkpoint does not match initial state identity")
    return value


def compare_replays(left: dict, right: dict) -> dict:
    """Find the first difference in declared, bounded checkpoint observations.

    Hash receipts identify local evidence; this tool cannot establish that an
    observer really measured consumption or that selected state is complete.
    """
    left, right = _replay(left), _replay(right)
    result = {"schema": "cmre-replay-comparison/1", "input_sha256": env.TARGET_SHA256,
              "milestone_accepted": False,
              "scope": "Declared consumed-input and selected state/RNG checkpoint hashes only.",
              "limitations": "Raw observer evidence and state completeness require independent review."}
    for field in ("scenario_sha256", "environment_sha256", "initial_state_sha256", "actual_module_base"):
        if left[field] != right[field]:
            return {**result, "status": "incomparable", "field": field}
    a, b = left["checkpoints"], right["checkpoints"]
    for index, (x, y) in enumerate(zip(a, b)):
        for field in ("id", "consumed_input_count", "consumed_inputs_sha256", "state_sha256", "rng_sha256"):
            if x[field] != y[field]:
                return {**result, "status": "diverged", "checkpoint_index": index,
                        "checkpoint": x["id"], "field": field, "left": x[field], "right": y[field]}
    if len(a) != len(b):
        return {**result, "status": "diverged", "checkpoint_index": min(len(a), len(b)),
                "field": "checkpoint_count", "left": len(a), "right": len(b)}
    return {**result, "status": "matching-observations", "checkpoints_compared": len(a)}


def _command(args) -> int:
    try:
        if args.campaign_action == "create":
            value = create(args.name, args.functions, args.milestone)
            save(args.output, value)
        elif args.campaign_action == "status":
            value = status(load(args.manifest))
        elif args.campaign_action == "plan":
            value = plan(load(args.manifest), args.workers)
        else:
            value = compare_replays(*(json.loads(Path(p).read_text(encoding="utf-8"))
                                      for p in (args.left, args.right)))
        print(json.dumps(value, indent=2))
        return 0 if value.get("status") not in ("incomparable", "diverged") else 1
    except (ValueError, OSError) as exc:
        print(json.dumps({"status": "error", "detail": str(exc)}))
        return 1


def add_commands(sub) -> None:
    parser = sub.add_parser("campaign", help="bounded milestone scope and live dependency status")
    actions = parser.add_subparsers(dest="campaign_action", required=True)
    make = actions.add_parser("create")
    make.add_argument("name")
    make.add_argument("functions", nargs="+")
    make.add_argument("--milestone", choices=MILESTONES, default="M5")
    make.add_argument("--output", required=True)
    make.set_defaults(func=_command)
    show = actions.add_parser("status")
    show.add_argument("manifest")
    show.set_defaults(func=_command)
    dispatch = actions.add_parser("plan", help="read-only bounded packet plan with live leases and review backlog")
    dispatch.add_argument("manifest")
    dispatch.add_argument("--workers", type=int, default=2)
    dispatch.set_defaults(func=_command)
    replay = sub.add_parser("replay-compare", help="compare primary consumed-input checkpoint observations")
    replay.add_argument("left")
    replay.add_argument("right")
    replay.set_defaults(func=_command, campaign_action="compare")
