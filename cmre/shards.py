"""Per-shard reconciliation of the whole pinned inventory: read-only, local, deterministic.

Every pinned entry belongs to exactly one of eight shards by `RULE`, whatever its
state: unlifted, blocked, held, accepted or compiler-generated. Only accepted
C/legacy source reconciles an entry, so an empty C queue, a lift or a generated
classification never closes a shard. Reports hold function IDs, states and
inventory body-byte counts only (no instructions, bytes or listings). They are
progress accounting, not recovery evidence; only `cmre accept` recovers.
"""
from __future__ import annotations

from collections import Counter
import json
import zlib

from . import env

SHARDS = 8
RULE = "1 + crc32(ASCII lowercase 'fn_%08x' % preferred-image VA) % 8"
FACTS_SCHEMA = "cmre-shard-facts/1"
REPORT_SCHEMA = "cmre-shard-report/2"
# Mutually exclusive; only `source` reconciles an entry (see `reconcile`).
STATES = ("source", "generated", "lift-blocked", "held", "c-queue", "unlifted")
LIMITATIONS = [
    "Ownership is fixed at eight shards over the pinned inventory; compare reports to see inventory revisions.",
    "Compiler-generated dispositions are unverified until bound to a parent and regeneration path.",
    "Accepted entries outside the inventory are open exceptions, excluded from owned bytes.",
    "Holds recorded against accepted source bytes keep a shard open.",
    "Held and C-queue states come from the work registry; live claims are scheduling state and are excluded.",
    "composed_reported counts passing experimental composition reports whose sources, includes, header "
    "and execution identity are unchanged; it is not acceptance, revalidation or live integration.",
    "Record presence is not fresh revalidation; the session check remains the green gate.",
]


def shard_of(va: int) -> int:
    return 1 + zlib.crc32(f"fn_{va:08x}".encode("ascii")) % SHARDS


def _id(va: int) -> str:
    return f"fn_{va:08x}"


def _va(function: str) -> int:
    return int(function[3:], 16)


def canonical(value) -> bytes:
    return json.dumps(value, sort_keys=True, separators=(",", ":")).encode("utf-8")


def digest(value) -> str:
    return env.digest_bytes(canonical(value))


def _composed(target: str) -> set[int]:
    """Functions in a passing composition report whose recorded inputs are all still current:
    sources, their included files, the shared header and the execution identity."""
    out, digests, execution = set(), {}, None

    def current(path) -> str | None:
        if path not in digests:
            digests[path] = env.digest(path) if path.is_file() else None
        return digests[path]

    for path in sorted((env.ANALYSIS / "cmre/compositions").glob("*.json")):
        try:
            report = json.loads(path.read_text(encoding="utf-8"))
        except (OSError, ValueError):
            continue
        if (report.get("schema") != "cmre-composition-report/1" or report.get("input_sha256") != target
                or report.get("status") != "pass"
                or report.get("observed_composed_calls_at_both_optimizations") is not True):
            continue
        if execution is None:
            from . import recover
            execution = recover.execution_identity()
        functions = [_va(f) for f in report.get("campaign", {}).get("functions", [])]
        sources = report.get("source_sha256", {})
        included = [(env.ROOT / name, digest) for d in report.get("source_dependencies", {}).values()
                    for name, digest in d.get("files", {}).items()]
        fresh = lambda va: sources.get(f"{_id(va)}.c") == (current(env.C_DIR / f"{_id(va)}.c") or False)
        if (functions and report.get("execution_identity") == execution
                and report.get("header_sha256") == current(env.ROOT / "recovery/include/cmre.h")
                and all(fresh(va) for va in functions)
                and all(current(p) == digest for p, digest in included)):
            out.update(functions)
    return out


def gather() -> dict:
    """Current local facts: pinned inventory, ledgers, work registry and composition reports."""
    from . import inventory, ledger, queue, work

    inv = inventory.load()
    cmre = ledger.cmre_entries()
    legacy = ledger.legacy_entries()
    source = {e.va for e in legacy} | {e.va for e in cmre if e.record.get("c")}
    lifted = {e.va for e in cmre}
    blocked = set(ledger.blocked())
    records = work.read()["functions"]
    on_disk = {_va(p.stem) for p in env.C_DIR.glob("fn_????????.c")}
    candidates = (set(inv.functions) | lifted | blocked | on_disk | {_va(k) for k in records}) - source
    # The same rule as the C selector, so a report never disagrees with `cmre next --c`.
    held = {va: records.get(_id(va), {}).get("state", "unaccepted-source")
            for va in candidates if work.held(va, records)}
    generated = {va: kind for va in inv.functions if va not in source
                 and (kind := queue.compiler_generated(va))}
    ready = {va for va in lifted - source - set(held) - set(generated)
             if all(c in source for c in inv.callees.get(va, []))}
    # Acceptance follows review, so a pending-review note on accepted source is spent. Other
    # holds stay open when recorded against the accepted bytes (legacy has no source identity).
    accepted_c = {e.va: e.record["c"].get("source_sha256") for e in cmre if e.record.get("c")}
    accepted_holds = {va: r["state"] for key, r in records.items()
                      if (va := _va(key)) in source and r["state"] not in ("ready", "needs-review")
                      and (va not in accepted_c or r.get("identity", {}).get("source") == accepted_c[va])}
    integrated = {va for va, e in ledger.recovered().items()  # the `cmre status` rule
                  if e.record.get("status", {}).get("integrated") or e.record.get("integrated")}
    outside: dict[str, list[str]] = {}
    for kind, vas in (("accepted-source", source), ("lifted", lifted - source),
                      ("lift-blocked", blocked), ("held", set(held))):
        for va in sorted(vas - set(inv.functions)):
            outside.setdefault(_id(va), []).append(kind)
    inside = lambda vas: sorted(_id(va) for va in vas if va in inv.functions)
    return {"schema": FACTS_SCHEMA, "input_sha256": env.TARGET_SHA256,
            "inventory": {"sha256": {env.rel(p): h for p, h in env.INVENTORY_SHA256.items()},
                          "functions": {_id(va): f.body_bytes for va, f in sorted(inv.functions.items())}},
            "source": inside(source), "lifted": inside(lifted), "lift_blocked": inside(blocked),
            "held": {_id(va): s for va, s in sorted(held.items()) if va in inv.functions},
            "generated": {_id(va): k for va, k in sorted(generated.items())},
            "accepted_holds": {_id(va): s for va, s in sorted(accepted_holds.items()) if va in inv.functions},
            "c_ready": inside(ready), "composed": inside(_composed(env.TARGET_SHA256)),
            "integrated": inside(integrated), "outside": dict(sorted(outside.items()))}


def _normal(facts: dict) -> dict:
    """Order-independent facts, so equal states always give equal reports."""
    out = dict(facts)
    for key in ("source", "lifted", "lift_blocked", "c_ready", "composed", "integrated"):
        out[key] = sorted(set(facts[key]))
    for key in ("held", "generated", "accepted_holds"):
        out[key] = dict(sorted(facts[key].items()))
    out["outside"] = {k: sorted(v) for k, v in sorted(facts["outside"].items())}
    out["inventory"] = {"sha256": dict(sorted(facts["inventory"]["sha256"].items())),
                        "functions": dict(sorted(facts["inventory"]["functions"].items()))}
    return out


def reconcile(facts: dict, shard: int | None = None) -> dict:
    """Partition every pinned entry into its shard and one state; pure over `facts`."""
    if facts.get("schema") != FACTS_SCHEMA:
        raise ValueError("Unsupported shard facts schema")
    if shard is not None and not 1 <= shard <= SHARDS:
        raise ValueError(f"Invalid shard {shard}: expected 1..{SHARDS}")
    facts = _normal(facts)
    sizes = facts["inventory"]["functions"]
    source, lifted, blocked = set(facts["source"]), set(facts["lifted"]), set(facts["lift_blocked"])
    held, generated, accepted_holds = facts["held"], facts["generated"], facts["accepted_holds"]
    ready, composed, integrated = set(facts["c_ready"]), set(facts["composed"]), set(facts["integrated"])
    by_shard = {k: {s: [] for s in STATES} for k in range(1, SHARDS + 1)}

    def state(f: str) -> str:
        if f in source:
            return "source"
        if f in generated:
            return "generated"
        if f in lifted:  # exactly the C selector's rule; a block record does not hold lifted work
            return "held" if f in held else "c-queue"
        return "lift-blocked" if f in blocked else "held" if f in held else "unlifted"

    for function in sizes:
        by_shard[shard_of(_va(function))][state(function)].append(function)
    measure = lambda ids: {"functions": len(ids), "body_bytes": sum(sizes[f] for f in ids)}

    def section(k: int) -> dict:
        ids = by_shard[k]
        owned = [f for s in STATES for f in ids[s]]
        outside = [{"function": f, "kinds": kinds} for f, kinds in facts["outside"].items()
                   if shard_of(_va(f)) == k]
        holds = {f: accepted_holds[f] for f in ids["source"] if f in accepted_holds}
        open_ = {s: len(ids[s]) for s in STATES[1:] if ids[s]}
        if holds:
            open_["accepted-hold"] = len(holds)
        if outside:
            open_["outside-inventory"] = len(outside)
        total, accepted = measure(owned), measure(ids["source"])
        return {"shard": k, "owned": total,
                "states": {s: measure(ids[s]) for s in STATES},
                "source_percent": round(100 * accepted["body_bytes"] / total["body_bytes"], 3)
                if total["body_bytes"] else 0.0,
                "lift_inclusive": measure([f for f in owned if f in source or f in lifted]),
                "composed_reported": measure([f for f in owned if f in composed]),
                "integrated": measure([f for f in owned if f in integrated]),
                "c_queue_ready": measure([f for f in ids["c-queue"] if f in ready]),
                "held_by_state": dict(sorted(Counter(held[f] for f in ids["held"]).items())),
                "accepted_holds": holds,
                "generated_by_kind": dict(sorted(Counter(generated[f] for f in ids["generated"]).items())),
                "generated_disposition": "unverified",
                "outside_inventory": outside,
                "open": open_,
                "complete": total["functions"] > 0 and not open_,
                "functions": ids}

    sections = [section(k) for k in range(1, SHARDS + 1)]
    report = {"schema": REPORT_SCHEMA, "input_sha256": facts["input_sha256"], "rule": RULE,
              "shard_count": SHARDS, "facts_sha256": digest(facts),
              "inventory": {"sha256": facts["inventory"]["sha256"], **measure(list(sizes))},
              "shards": sections if shard is None else [sections[shard - 1]],
              "limitations": LIMITATIONS}
    if shard is None:
        add = lambda key: {m: sum(s[key][m] for s in sections) for m in ("functions", "body_bytes")}
        report["totals"] = {"owned": add("owned"),
                            "states": {st: {m: sum(s["states"][st][m] for s in sections)
                                            for m in ("functions", "body_bytes")} for st in STATES},
                            **{key: add(key) for key in ("lift_inclusive", "composed_reported",
                                                         "integrated", "c_queue_ready")},
                            "accepted_holds": sum(len(s["accepted_holds"]) for s in sections),
                            "outside_inventory": sum(len(s["outside_inventory"]) for s in sections),
                            "complete_shards": sum(s["complete"] for s in sections)}
    return report


def _owned(section: dict) -> set[str]:
    return {f for ids in section["functions"].values() for f in ids}


def compare(old: dict, new: dict) -> dict:
    """Per-shard changes between two reports, including inventory additions and removals."""
    if old.get("schema") != REPORT_SCHEMA or new.get("schema") != REPORT_SCHEMA:
        raise ValueError("Unsupported shard report schema")
    if old["input_sha256"] != new["input_sha256"]:
        raise ValueError("Reports describe different target binaries")
    if (old["rule"], old["shard_count"]) != (new["rule"], new["shard_count"]):
        raise ValueError("Reports use different shard ownership rules")
    before = {s["shard"]: s for s in old["shards"]}
    after = {s["shard"]: s for s in new["shards"]}
    rows = []
    for k in sorted(before.keys() & after.keys()):
        b, a = before[k], after[k]
        was, now = _owned(b), _owned(a)
        rows.append({"shard": k, "owned": {"before": b["owned"], "after": a["owned"]},
                     "added": sorted(now - was), "removed": sorted(was - now),
                     "source": {m: a["states"]["source"][m] - b["states"]["source"][m]
                                for m in ("functions", "body_bytes")},
                     "complete": {"before": b["complete"], "after": a["complete"]}})
    changed = (old["inventory"]["sha256"] != new["inventory"]["sha256"]
               or any(r["added"] or r["removed"] for r in rows))
    return {"inventory_changed": changed, "shards": rows}


def _line(s: dict) -> str:
    st = s["states"]
    return (f"shard {s['shard']}/{SHARDS}: owned {s['owned']['functions']} fn {s['owned']['body_bytes']}B"
            f" | source {st['source']['functions']} fn {st['source']['body_bytes']}B ({s['source_percent']}%)"
            f" | lift-incl {s['lift_inclusive']['functions']} | composed {s['composed_reported']['functions']}"
            f" | integrated {s['integrated']['functions']}"
            f" | c-queue {st['c-queue']['functions']} (ready {s['c_queue_ready']['functions']})"
            f" | held {st['held']['functions']} | accepted-hold {len(s['accepted_holds'])}"
            f" | generated {st['generated']['functions']}"
            f" | lift-blocked {st['lift-blocked']['functions']} | unlifted {st['unlifted']['functions']}"
            f" | outside {len(s['outside_inventory'])} | {'COMPLETE' if s['complete'] else 'INCOMPLETE'}")


def command(args) -> int:
    from pathlib import Path

    report = reconcile(gather(), args.shard)
    delta = compare(json.loads(Path(args.since).read_text(encoding="utf-8")), report) if args.since else None
    saved = None
    if args.write:
        from .coordination import atomic_json

        path = env.ANALYSIS / "cmre/shards" / f"{digest(report)}.json"
        atomic_json(path, report)
        saved = env.rel(path)
    if args.json:  # one document; the extra keys are outside the saved report and its digest
        print(json.dumps({**report, **({"comparison": delta} if delta is not None else {}),
                          **({"saved": saved} if saved else {})}, indent=2))
        return 0
    inv = report["inventory"]
    print(f"SHA-256 {report['input_sha256']}; inventory {inv['functions']} functions, "
          f"{inv['body_bytes']} body bytes; facts {report['facts_sha256'][:16]}")
    for s in report["shards"]:
        print(_line(s))
        if args.ids:
            for state in STATES[1:]:
                if s["functions"][state]:
                    print(f"  {state} {' '.join(s['functions'][state])}")
            for f, state in s["accepted_holds"].items():
                print(f"  accepted-hold {f} {state}")
            for e in s["outside_inventory"]:
                print(f"  outside-inventory {e['function']} {','.join(e['kinds'])}")
    if delta is not None:
        print("inventory CHANGED" if delta["inventory_changed"] else "inventory unchanged")
        for r in delta["shards"]:
            print(f"  shard {r['shard']}: source {r['source']['functions']:+d} fn {r['source']['body_bytes']:+d}B"
                  f"; owned {r['owned']['before']['functions']}->{r['owned']['after']['functions']}"
                  f"; added {len(r['added'])} removed {len(r['removed'])}")
    if saved:
        print(f"report: {saved}")
    return 0


def add_commands(sub) -> None:
    p = sub.add_parser("shards", help="per-shard inventory reconciliation and completion (read-only)")
    p.add_argument("--shard", type=int, help=f"one shard, 1..{SHARDS}")
    p.add_argument("--ids", action="store_true", help="list unfinished function IDs by state")
    p.add_argument("--json", action="store_true")
    p.add_argument("--write", action="store_true", help="save under CM0102/Analysis/cmre/shards/<digest>.json")
    p.add_argument("--since", help="compare with an earlier saved report")
    p.set_defaults(func=command)
