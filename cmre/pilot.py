"""Local, measured recovery cohorts. Unknown model usage stays unknown."""
from __future__ import annotations

import json
import re
import time

from . import coordination, env, inventory, ledger, recover, work

TOKEN_FIELDS = ("input_tokens", "cache_read_tokens", "cache_write_tokens", "output_tokens")


def path(name):
    if not re.fullmatch(r"[a-zA-Z0-9][a-zA-Z0-9_-]{0,79}", name):
        raise ValueError("Pilot name must use letters, digits, hyphens or underscores")
    return env.ANALYSIS / "cmre/pilots" / f"{name}.json"


def accepted():
    return {e.va: e for e in ledger.cmre_entries() if e.record.get("c")}


def start(name, functions):
    inv = inventory.load()
    functions = sorted(set(functions))
    if not functions or any(va not in inv.functions for va in functions):
        raise ValueError("Select at least one function from the pinned inventory")
    if path(name).exists():
        raise ValueError("Pilot exists; choose another name")
    existing = accepted()
    legacy = {e.va for e in ledger.legacy_entries()}
    states = work.read()["functions"]
    data = {"schema": "cmre-pilot/1", "started": time.time(), "ended": None,
            "harness": recover.harness_digest(), "target": env.TARGET_SHA256,
            "usage_complete": False, "calls": {}, "functions": {}}
    for va in functions:
        key = f"fn_{va:08x}"
        state = states.get(key, {})
        data["functions"][key] = {"bytes": inv.functions[va].body_bytes,
                                  "already_accepted": va in existing or va in legacy,
                                  "attempts": state.get("attempts", 0), "failures": state.get("failures", 0)}
    coordination.atomic_json(path(name), data)
    return data


def record_usage(data, call_id, model, values, review_corrections):
    if data["ended"] is not None:
        raise ValueError("Finished pilot is immutable")
    if not call_id.strip() or not model.strip() or call_id in data["calls"]:
        raise ValueError("Use a unique provider call ID and an observed model name")
    if any(v is not None and (type(v) is not int or v < 0) for v in values.values()):
        raise ValueError("Token usage must be nonnegative integers or unknown")
    if review_corrections < 0:
        raise ValueError("Review corrections cannot be negative")
    data["calls"][call_id] = {"model": model, **values, "review_corrections": review_corrections}


def report(data):
    if "result" in data:
        return data["result"]
    current = accepted()
    states = work.read()["functions"]
    byte_count, count, attempts, failures = 0, 0, 0, 0
    for key, before in data["functions"].items():
        va = int(key[3:], 16)
        entry = current.get(va)
        source = env.C_DIR / f"{key}.c"
        if (entry and not before["already_accepted"] and source.exists()
                and entry.record["c"].get("source_sha256") == env.digest(source)):
            byte_count += before["bytes"]
            count += 1
        state = states.get(key, {})
        attempts += max(0, state.get("attempts", 0) - before["attempts"])
        failures += max(0, state.get("failures", 0) - before["failures"])
    seconds = max(0, (data["ended"] or time.time()) - data["started"])
    valid = data["harness"] == recover.harness_digest() and data["target"] == env.TARGET_SHA256
    calls = list(data["calls"].values())
    totals = {k: sum(c[k] for c in calls if c[k] is not None) for k in TOKEN_FIELDS}
    complete = data["usage_complete"] and bool(calls) and all(c[k] is not None for c in calls for k in TOKEN_FIELDS)
    return {"finished": data["ended"] is not None, "identity_matches": valid,
            "accepted_functions": count, "accepted_bytes": byte_count,
            "elapsed_seconds": round(seconds, 2), "test_invocations": attempts, "failed_invocations": failures,
            "recorded_calls": len(calls), "reported_tokens": totals, "usage_complete": complete,
            "review_corrections": sum(c["review_corrections"] for c in calls),
            "accepted_bytes_per_hour": round(byte_count * 3600 / seconds, 2) if valid and seconds else None,
            "tokens_per_accepted_byte": {k: round(v / byte_count, 3) if complete and valid and byte_count else None
                                         for k, v in totals.items()},
            "note": "Include failed work, coordinator and review calls. Rates are comparable only for matched cohorts and reconciled usage. Runtime/composition gates remain separate."}


def add_commands(sub):
    p = sub.add_parser("pilot", help="measure accepted bytes, elapsed time and actual model usage")
    commands = p.add_subparsers(dest="action", required=True)
    p = commands.add_parser("start")
    p.add_argument("name")
    p.add_argument("--functions", nargs="+", required=True)
    p.set_defaults(func=command)
    p = commands.add_parser("usage")
    p.add_argument("name")
    p.add_argument("--call-id", required=True)
    p.add_argument("--model", required=True)
    for key in TOKEN_FIELDS:
        p.add_argument("--" + key.replace("_", "-"), type=int)
    p.add_argument("--review-corrections", type=int, default=0)
    p.set_defaults(func=command)
    p = commands.add_parser("finish")
    p.add_argument("name")
    p.add_argument("--usage-complete", action="store_true", help="attest all model calls were reconciled against provider logs")
    p.set_defaults(func=command)
    p = commands.add_parser("report")
    p.add_argument("name")
    p.set_defaults(func=command)


def command(args):
    from .cli import _va
    with coordination.lock(env.ANALYSIS / "cmre/pilots.lock"):
        if args.action == "start":
            data = start(args.name, [_va(v) for v in args.functions])
        else:
            if not path(args.name).exists():
                raise ValueError("Pilot does not exist")
            data = json.loads(path(args.name).read_text(encoding="utf-8"))
            if args.action == "usage":
                record_usage(data, args.call_id, args.model,
                             {k: getattr(args, k) for k in TOKEN_FIELDS}, args.review_corrections)
            elif args.action == "finish":
                if data["ended"] is not None:
                    raise ValueError("Pilot is already finished")
                data["ended"] = time.time()
                data["usage_complete"] = args.usage_complete
                data["result"] = report(data)
            if args.action != "report":
                coordination.atomic_json(path(args.name), data)
    print(json.dumps(report(data), indent=2))
    return 0
