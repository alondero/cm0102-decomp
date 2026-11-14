"""Stage accounting and conversion lanes for the recovery pipeline.

The pipeline has three supply bands, ranked by how close the work already is to
acceptance:

``authored``
    ``recovery/c/fn_*.c`` exists, the lift is accepted, and the ledger has no C
    tier. The source is written; only the differential gate and publication
    remain.

``lifted``
    The lift is accepted and no C source exists. The translation is unwritten.

``unlifted``
    The function is modelled in the inventory but has no ledger record. Lift and
    translation both remain, and many of these fail the lift coverage gate until
    a fixture reaches their branches.

Two conversion lanes:

``triage``
    Run the C differential gate over a selection and record the outcome. It is
    deterministic, parallel across functions and writes a durable receipt. It
    accepts nothing.

``accept``
    Publish every triaged pass. Acceptance repeats the gate in a fresh process,
    so it is serial under the writer lock.

Planning is arithmetic over measured inputs: bytes per band, the ordered sizes,
and the measured cost of one gate run. It does not invent a throughput claim.
"""
from __future__ import annotations

import json
import math
import random
import time
from dataclasses import dataclass, field, asdict
from pathlib import Path

#: Measured on 2026-11-10. Accepting a 76-94 byte forwarder took 3-5 s; a 2.5 KB
#: routine took 187 s. Case count and branch coverage dominate, not bytes, so
#: these are planning defaults and stay configurable.
DEFAULT_GATE_SECONDS = {0: 4.0, 512: 15.0, 1024: 60.0, 2048: 120.0}
DEFAULT_ACTION_MINUTES = 22.5  # a complete five-action author loop, not one turn
DEFAULT_TRIAGE_JOBS = 8


@dataclass(frozen=True)
class Band:
    name: str
    functions: int
    body_bytes: int
    median_bytes: int
    blocker_families: dict[str, int] = field(default_factory=dict)


@dataclass(frozen=True)
class Survey:
    bands: dict[str, Band]
    sizes: dict[str, list[int]]
    # None supports arithmetic-only historical callers. Live surveys always
    # distinguish dispatchable supply from inventory awaiting evidence.
    dispatch_sizes: dict[str, list[int]] | None = None

    def total(self, *names: str) -> int:
        return sum(self.bands[name].body_bytes for name in names)


def _median(values: list[int]) -> int:
    if not values:
        return 0
    ordered = sorted(values)
    middle = len(ordered) // 2
    if len(ordered) % 2:
        return ordered[middle]
    return (ordered[middle - 1] + ordered[middle]) // 2


def family_of(blocker: str) -> str:
    text = (blocker or "").strip()
    return text.split()[0] if text else "(none)"


def build(entries, work_records, sources, unlifted) -> Survey:
    """Classify the live recovery state into the three supply bands.

    ``entries`` yields ``(va, body_bytes, has_c, has_lift)``.
    ``work_records`` maps ``fn_XXXXXXXX`` to a work-registry record.
    ``sources`` is the set of VAs with a ``recovery/c`` source.
    ``unlifted`` maps VA to body bytes for modelled functions without a ledger
    record.
    """
    sizes: dict[str, list[int]] = {"authored": [], "lifted": []}
    families: dict[str, dict[str, int]] = {"authored": {}, "lifted": {}}
    dispatch = {name: [] for name in ("authored", "lifted", "unlifted")}
    for va, size, has_c, has_lift in entries:
        if has_c or not has_lift:
            continue
        band = "authored" if va in sources else "lifted"
        record = work_records.get(f"fn_{va:08x}") or {}
        key = family_of(record.get("blocker", ""))
        sizes[band].append(size)
        families[band][key] = families[band].get(key, 0) + 1
        if record.get('state') not in ('evidence-needed', 'harness-blocker'):
            if band == 'lifted' or record.get('state') == 'ready':
                dispatch[band].append(size)
    sizes["unlifted"] = list(unlifted.values())
    bands = {}
    for name in ("authored", "lifted", "unlifted"):
        bands[name] = Band(name, len(sizes[name]), sum(sizes[name]),
                           _median(sizes[name]), families.get(name, {}))
    return Survey(bands=bands, sizes=sizes, dispatch_sizes=dispatch)


def survey(root: Path | None = None) -> Survey:
    """Read the live ledger, work registry, sources and inventory."""
    from . import inventory, ledger, queue, work

    del root  # paths come from env; kept as an explicit seam for tests
    entries = []
    functions = inventory.load().functions
    for entry in ledger.cmre_entries():
        record = entry.record
        if queue.compiler_generated(entry.va):
            continue  # metadata-proven funclets stay outside every C lane
        entries.append((entry.va, functions[entry.va].body_bytes,
                        bool(record.get("c")), bool(record.get("lifted"))))
    sources = {int(path.stem[3:], 16) for path in _c_dir().glob("fn_*.c")}
    unlifted = {}
    known = {entry.va for entry in ledger.cmre_entries()}
    for va, function in inventory.load().functions.items():
        if va not in known and not queue.compiler_generated(va):
            unlifted[va] = function.body_bytes
    return build(entries, work.read()["functions"], sources, unlifted)


def _c_dir() -> Path:
    from . import env
    return env.C_DIR


def gate_seconds(body_bytes: int, table: dict[int, float] | None = None) -> float:
    """Conservative single-gate cost for a function of this size."""
    table = table or DEFAULT_GATE_SECONDS
    cost = table[0]
    for threshold in sorted(table):
        if body_bytes >= threshold:
            cost = table[threshold]
    return float(cost)


def select(state: Survey, target_bytes: int, bands=("authored", "lifted", "unlifted")) -> list[dict]:
    """Largest-first selection reaching the target, band by band."""
    if target_bytes <= 0:
        raise ValueError("target_bytes must be positive")
    selected: list[dict] = []
    remaining = target_bytes
    for name in bands:
        for size in sorted((state.dispatch_sizes if state.dispatch_sizes is not None
                            else state.sizes).get(name, []), reverse=True):
            if remaining <= 0:
                break
            selected.append({"band": name, "body_bytes": size})
            remaining -= size
        if remaining <= 0:
            break
    return selected


def plan(state: Survey, target_bytes: int, window_minutes: float, *,
         action_minutes: float = DEFAULT_ACTION_MINUTES,
         gate_table: dict[int, float] | None = None,
         triage_jobs: int = DEFAULT_TRIAGE_JOBS,
         pass_rate: dict[str, float] | None = None) -> dict:
    """Size a session to a byte target from the measured stage costs.

    ``pass_rate`` is the measured fraction of a band that survives the gate.
    Omit it only when the rate is unknown; the plan then reports the optimistic
    selection and says so, because a held function that fails the gate
    contributes nothing.
    """
    if window_minutes <= 0:
        raise ValueError("window_minutes must be positive")
    selected = select(state, target_bytes) if pass_rate is None else _select_with_rate(
        state, target_bytes, pass_rate)
    total = sum(item["body_bytes"] for item in selected)
    expected = (total if pass_rate is None else math.floor(sum(
        item['body_bytes'] * float(pass_rate.get(item['band'], 0.0)) for item in selected)))
    gate_total = sum(gate_seconds(item["body_bytes"], gate_table) for item in selected)
    authored = sum(1 for item in selected if item["band"] == "authored")
    needs_author = len(selected) - authored
    author_slots = math.ceil(needs_author * action_minutes / window_minutes) if needs_author else 0
    jobs = max(1, min(triage_jobs, len(selected)))
    return {
        "target_bytes": target_bytes,
        "selected_bytes": total,
        "expected_accepted_bytes": expected,
        "selected_functions": len(selected),
        "functions_by_band": {
            name: sum(1 for item in selected if item["band"] == name)
            for name in ("authored", "lifted", "unlifted")},
        "shortfall_bytes": max(0, target_bytes - expected),
        "expected_pass_rate": pass_rate,
        "assumption": ("optimistic: assumes every selected function passes the gate"
                       if pass_rate is None else "scaled by the measured pass rate"),
        "author_slots": author_slots,
        "action_minutes": action_minutes,
        "triage_jobs": jobs,
        "triage_minutes": round(gate_total / 60.0 / jobs, 1),
        "accept_minutes": round(gate_total / 60.0, 1),
        "intake_per_hour": round(len(selected) * 60.0 / window_minutes, 1),
        "balance_rule": ("intake must match conversion one-for-one; a band whose"
                         " blockers are unresolved stalls the lane regardless of"
                         " author capacity"),
        "selected": selected,
    }


def _select_with_rate(state: Survey, target_bytes: int, pass_rate: dict[str, float]) -> list[dict]:
    """Select enough attempts that the expected yield reaches the target."""
    selected: list[dict] = []
    expected = 0.0
    for name in ("authored", "lifted", "unlifted"):
        rate = float(pass_rate.get(name, 0.0))
        if rate <= 0:
            continue
        for size in sorted((state.dispatch_sizes if state.dispatch_sizes is not None
                            else state.sizes).get(name, []), reverse=True):
            if expected >= target_bytes:
                break
            selected.append({"band": name, "body_bytes": size})
            expected += size * rate
        if expected >= target_bytes:
            break
    return selected


def receipt_path(root: Path, created: float | None = None) -> Path:
    stamp = time.strftime("%Y%m%dT%H%M%S", time.gmtime(created))
    return root / f"{stamp}-pipeline.json"


def write_receipt(root: Path, payload: dict) -> Path:
    from . import coordination
    root.mkdir(parents=True, exist_ok=True)
    path = receipt_path(root)
    coordination.atomic_json(path, payload)
    return path


def _bounded(result: dict, seconds: float) -> dict:
    """Project a gate result into a bounded triage row; never rewrite evidence."""
    diff = result.get("difftest") or {}
    mismatches = diff.get("mismatches") or []
    return {
        "status": result.get("status"),
        "coverage": diff.get("coverage"),
        "inconclusive": diff.get("inconclusive"),
        "case_count": diff.get("cases"),
        "first_mismatch": mismatches[0] if mismatches else None,
        "seconds": round(seconds, 1),
    }


def _triage_one(va: int) -> dict:
    from . import recover
    started = time.time()
    try:
        result = recover.test_c(va)
    except BaseException as error:  # a crash is a triage row, not a lane failure
        return {"va": f"{va:#010x}", "status": "harness-error",
                "error": f"{type(error).__name__}: {error}"[:300],
                "seconds": round(time.time() - started, 1)}
    return {"va": f"{va:#010x}", **_bounded(result, time.time() - started)}


def triage(vas: list[int], *, jobs: int = DEFAULT_TRIAGE_JOBS) -> dict:
    """Run the C gate over a selection in parallel; accept nothing."""
    from concurrent.futures import ProcessPoolExecutor

    if not vas:
        raise ValueError("triage needs at least one function")
    jobs = max(1, min(int(jobs), len(vas)))
    started = time.time()
    rows: list[dict] = []
    with ProcessPoolExecutor(max_workers=jobs) as pool:
        for row in pool.map(_triage_one, vas):
            rows.append(row)
    passes = [row for row in rows if row.get("status") == "pass"]
    return {
        "lane": "triage",
        "jobs": jobs,
        "select": len(vas),
        "pass": len(passes),
        "fail": len(rows) - len(passes),
        "seconds": round(time.time() - started, 1),
        "rows": sorted(rows, key=lambda row: row["va"]),
    }


def _accept_one(va: int) -> dict:
    from . import coordination, recover
    started = time.time()
    with coordination.writer():
        ok, result = recover.accept(va, "c")
    return {"va": f"{va:#010x}", "accepted": bool(ok),
            "status": result.get("status"), "seconds": round(time.time() - started, 1)}


def accept(vas: list[int], *, body_bytes: dict[int, int] | None = None) -> dict:
    """Publish each function through the normal acceptance gate, serially."""
    if not vas:
        raise ValueError("accept needs at least one function")
    started = time.time()
    rows = [_accept_one(va) for va in vas]
    accepted = [row for row in rows if row["accepted"]]
    sizes = body_bytes or {}
    return {
        "lane": "accept",
        "select": len(vas),
        "accepted": len(accepted),
        "rejected": len(rows) - len(accepted),
        "accepted_bytes": sum(sizes.get(int(row["va"], 16), 0) for row in accepted),
        "seconds": round(time.time() - started, 1),
        "rows": rows,
    }


def candidates(band: str = "authored", *, limit: int = 10, min_bytes: int = 0,
               max_bytes: int | None = None) -> list[dict]:
    """Order the chosen band largest-first for the conversion lane."""
    from . import inventory, ledger, queue, work
    functions = inventory.load().functions
    records = work.read()['functions']
    chosen: list[dict] = []
    for entry in ledger.cmre_entries():
        record = entry.record
        if record.get("c") or not record.get("lifted"):
            continue
        if queue.compiler_generated(entry.va) or work.dispatch_hold(entry.va, records):
            continue
        has_source = (_c_dir() / f"fn_{entry.va:08x}.c").is_file()
        if (band == "authored") != has_source:
            continue
        size = functions[entry.va].body_bytes
        if size < min_bytes or (max_bytes is not None and size > max_bytes):
            continue
        chosen.append({"va": entry.va, "body_bytes": size,
                       "band": "authored" if has_source else "lifted"})
    chosen.sort(key=lambda item: (-item["body_bytes"], item["va"]))
    return chosen[:limit]


def coverage_probe(va: int, *, arg_counts=(2, 4, 6, 8, 10), seeds=(0, 1),
                   cases: int = 120, budget: int = 200_000) -> dict:
    """Measure original-only branch coverage over a bounded input grid.

    This is a supply measurement, not an acceptance path: it runs only the
    original machine, so it says whether a function's low coverage is an
    argument-shape/seed problem (tunable) rather than a missing fixture or an
    unreachable branch (needs evidence).
    """
    from . import difftest, emu, image, lift

    img = image.primary()
    lifted = lift.discover(va)
    starts = set(lifted.insns)
    if not starts:
        raise ValueError(f"no lifted instructions for {va:#x}")
    region = (va, lifted.region_end)
    templates = difftest.arena_templates()
    rows = []
    for arg_count in arg_counts:
        for seed in seeds:
            machine = emu.Machine(img, watch=region)
            covered: set[int] = set()
            opts = difftest.Options(cases=cases, budget=budget, discovery=0,
                                    arg_count=arg_count, min_coverage=0.0, seed=seed,
                                    hints={}, inputs="v2")
            rng = random.Random(seed)
            for _ in range(cases):
                case = difftest.make_case(rng, opts, [], templates)
                covered |= machine.run(va, case, budget).covered
            rows.append({"arg_count": arg_count, "seed": seed,
                         "coverage": round(len(covered & starts) / len(starts), 4)})
    best = max(rows, key=lambda row: row["coverage"])
    return {"va": f"{va:#010x}", "cases": cases,
            "instructions": len(starts), "best": best, "grid": rows}


def command(args) -> int:
    """CLI entry point: ``cmre pipeline {survey,plan,drain}``."""
    action = args.action
    if action in ('cancel-unstarted', 'retry-precondition'):
        from .provider_queue import command as cancel_command
        return cancel_command(args)
    if action == 'report':
        from .provider_metrics import report
        print(json.dumps(report(args.directory), indent=2))
        return 0
    if action == 'scale':
        from .pipeline_controller import scale
        value = scale(args.directory, args.stage, args.workers,
            owners=args.owners.split(',') if args.owners else None,
            execution_workers=args.execution_workers, **{
            name: getattr(args, name) for name in ('low_functions', 'high_functions', 'low_bytes', 'high_bytes')
            if getattr(args, name) is not None})
        print(json.dumps(value, indent=2))
        return 0
    if action in ('replenish', 'inbox', 'adopted'):
        from . import pipeline_flow
        if action == 'adopted':
            value = pipeline_flow.acknowledge(args.directory, args.job, args.receipt)
        elif action == 'inbox':
            value = pipeline_flow.inbox(args.directory)
        else:
            from scripts.cmre_provider_campaign import _profile_entries, load
            report = pipeline_flow.prepare(args.output, pipeline_flow.snapshot(),
                _profile_entries(load(args.profiles)), slots=args.slots,
                duration_seconds=args.window_minutes * 60, limit=args.limit,
                buffer_bytes=args.buffer_kb * 1000, scalable=getattr(args, 'scalable', False))
            value = {k: v for k, v in report.items() if k != 'selection'}
        print(json.dumps(value, indent=2))
        return 0
    if action == "survey":
        state = survey()
        payload = {
            "bands": {name: asdict(band) for name, band in state.bands.items()},
            "total_unaccepted_bytes": state.total("authored", "lifted", "unlifted"),
        }
        if args.json:
            print(json.dumps(payload, indent=2))
        else:
            for name in ("authored", "lifted", "unlifted"):
                band = state.bands[name]
                print(f"{name:9s} {band.functions:5d} fn {band.body_bytes:9d} B"
                      f"  median {band.median_bytes:5d} B")
                for family, count in sorted(band.blocker_families.items(),
                                            key=lambda item: -item[1])[:6]:
                    print(f"          {count:4d}  {family}")
        return 0
    if action == "plan":
        state = survey()
        rates = None
        if getattr(args, "pass_rate", None):
            rates = {}
            for part in args.pass_rate.split(","):
                name, _, value = part.partition("=")
                if not name.strip() or not value.strip():
                    raise ValueError("--pass-rate needs band=value pairs")
                rates[name.strip()] = float(value)
        payload = plan(state, int(args.target_kb) * 1024, float(args.window_minutes),
                       action_minutes=float(args.action_minutes),
                       triage_jobs=int(args.jobs), pass_rate=rates)
        if args.json:
            print(json.dumps(payload, indent=2))
        else:
            for key in ("target_bytes", "selected_bytes", "expected_accepted_bytes", "selected_functions",
                        "functions_by_band", "shortfall_bytes", "expected_pass_rate",
                        "assumption", "author_slots", "triage_minutes",
                        "accept_minutes", "intake_per_hour"):
                print(f"{key:22s} {payload[key]}")
        return 0
    if action == "drain":
        chosen = candidates(args.band, limit=args.limit, min_bytes=args.min_bytes,
                            max_bytes=args.max_bytes)
        if not chosen:
            print("no candidates in that band", file=__import__("sys").stderr)
            return 1
        triaged = triage([item["va"] for item in chosen], jobs=args.jobs)
        passes = [int(row["va"], 16) for row in triaged["rows"] if row.get("status") == "pass"]
        payload = {"schema": "cmre-pipeline-drain/1", "band": args.band,
                   "selection": chosen, "triage": triaged, "accept": None}
        if args.accept and passes:
            sizes = {item["va"]: item["body_bytes"] for item in chosen}
            payload["accept"] = accept(passes, body_bytes=sizes)
        from . import env
        path = write_receipt(env.ANALYSIS / "cmre/pipeline", payload)
        summary = {"receipt": str(path), "select": triaged["select"],
                   "triage_pass": triaged["pass"], "triage_seconds": triaged["seconds"],
                   "accepted": (payload["accept"] or {}).get("accepted", 0),
                   "accepted_bytes": (payload["accept"] or {}).get("accepted_bytes", 0)}
        print(json.dumps(summary, indent=2))
        return 0
    return 2
