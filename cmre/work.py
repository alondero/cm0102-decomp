"""C queue memory, not recovery evidence. Only cmre accept establishes recovery."""
from __future__ import annotations

from datetime import datetime, timedelta, timezone
from contextlib import contextmanager, ExitStack
import json
import time

from . import coordination, env

STATES = ("ready", "needs-review", "translation-defect", "harness-blocker", "evidence-needed")
# A claim only covers selection until the agent writes recovery/c/fn_X.c, which then holds it.
CLAIM_HOURS = 3.0


@contextmanager
def locked():
    """Serialize short queue updates from independent workers, with a bounded wait."""
    deadline = time.monotonic() + 5
    while True:
        stack = ExitStack()
        try:
            stack.enter_context(coordination.lock(env.ANALYSIS / "cmre/work.lock"))
            break
        except coordination.BusyError:
            stack.close()
            if time.monotonic() >= deadline:
                raise
            time.sleep(0.02)
    with stack:
        yield


def path():
    return env.HINTS_DIR / "work-state.json"


def read():
    if not path().exists():
        return {"schema": "cmre-work/1", "functions": {}}
    data = json.loads(path().read_text(encoding="utf-8"))
    if data.get("schema") != "cmre-work/1":
        raise ValueError("Unsupported work-state schema")
    return data


def identity(va):
    from . import recover

    files = {"source": env.C_DIR / f"fn_{va:08x}.c",
             "hints": env.HINTS_DIR / f"fn_{va:08x}.json",
             "header": env.RECOVERY / "include/cmre.h"}
    return {"target": env.TARGET_SHA256, "harness": recover.harness_digest(),
            **{k: env.digest(p) if p.is_file() else None for k, p in files.items()}}


def update(va, state, reason, retry_when="", blocker="", *, report=None):
    if state not in STATES or not reason.strip():
        raise ValueError("A supported state and a nonempty reason are required")
    if state in ("harness-blocker", "evidence-needed") and not retry_when.strip():
        raise ValueError("Blocked work requires an explicit retry condition")
    from . import inventory
    if va not in inventory.load().functions:
        raise ValueError("Function is absent from the pinned inventory")
    with locked():
        data = read()
        key = f"fn_{va:08x}"
        old = data["functions"].get(key, {})
        attempts = old.get("attempts", 0) + (report is not None)
        failures = old.get("failures", 0) + (report is not None and report["status"] != "pass")
        record = {"state": state, "reason": reason, "retry_when": retry_when,
                  "blocker": blocker, "identity": identity(va), "attempts": attempts,
                  "failures": failures, "updated": datetime.now(timezone.utc).isoformat()}
        if report is not None:
            record["last_report"] = {"key": report.get("key"), "status": report["status"],
                                     "profile": report.get("profile")}
            if report["status"] != "pass":
                record["first_failure"] = old.get("first_failure", {"reason": reason, "report": record["last_report"]})
        elif old.get("last_report"):
            record["last_report"] = old["last_report"]
        if "first_failure" in old:
            record["first_failure"] = old["first_failure"]
        data["functions"][key] = record
        coordination.atomic_json(path(), data)
    return record


def record_test(va, report):
    problems = list(report.get("problems", []))
    for build in report.get("builds", {}).values():
        problems += build.get("difftest", {}).get("mismatches", [])
        if build.get("detail"):
            problems.append(build["detail"])
    reason = json.dumps(problems[0], ensure_ascii=False)[:1200] if problems else report["status"]
    source = env.C_DIR / f"fn_{va:08x}.c"
    if report.get("source_sha256") and (not source.exists() or env.digest(source) != report["source_sha256"]):
        return update(va, "evidence-needed", "Source changed after the reported test",
                      "Retest the current source and review its matching report", report=report)
    if problems and all(isinstance(p, dict) and p.get("kind") == "budget-asymmetry" for p in problems):
        return update(va, "evidence-needed", reason,
                      "Investigate the asymmetric stop or establish an adequate equal budget for both executions",
                      "execution-budget", report=report)
    return update(va, "needs-review" if report["status"] == "pass" else "translation-defect",
                  reason, "Review listing and current report" if report["status"] == "pass"
                  else "Resolve the recorded discrepancy; five failed attempts is the session ceiling",
                  report=report)


def retry(va, reason, changed=False):
    old = read()["functions"].get(f"fn_{va:08x}")
    if changed and (not old or old["identity"] == identity(va)):
        raise ValueError("Inputs are unchanged; resolve the recorded retry condition first")
    return update(va, "ready", reason)


def _now():
    return datetime.now(timezone.utc)


def _claims_path():
    # Ephemeral coordination between concurrent agents (any client), not queue memory.
    return env.ANALYSIS / "cmre/claims.json"


def _live_claims():
    if not _claims_path().exists():
        return {}
    data = json.loads(_claims_path().read_text(encoding="utf-8"))
    if data.get("schema") != "cmre-claims/1":
        raise ValueError("Unsupported claims schema")
    now = _now()
    return {k: v for k, v in data["claims"].items() if datetime.fromisoformat(v["expires"]) > now}


def claims():
    """Live claims by function ID: {"owner", "expires"}."""
    return _live_claims()


def _write_claims(live):
    coordination.atomic_json(_claims_path(), {"schema": "cmre-claims/1", "claims": live})


def _claim_unlocked(vas, owner, hours):
    live = _live_claims()
    taken = [f"fn_{va:08x} is claimed by {live[k]['owner']}" for va in vas
             if (k := f"fn_{va:08x}") in live and live[k]["owner"] != owner]
    if taken:
        raise ValueError("; ".join(taken))
    expires = (_now() + timedelta(hours=hours)).isoformat()
    for va in vas:
        live[f"fn_{va:08x}"] = {"owner": owner, "expires": expires}
    _write_claims(live)
    return {f"fn_{va:08x}": live[f"fn_{va:08x}"] for va in vas}


def claim(vas, owner, hours=CLAIM_HOURS):
    """Claim functions for one agent, all or nothing; renewing your own claim is allowed."""
    if not owner.strip() or hours <= 0:
        raise ValueError("A claim requires an owner name and a positive duration")
    with locked():
        return _claim_unlocked(list(vas), owner, hours)


def release(owner, vas=None):
    with locked():
        keys = None if vas is None else {f"fn_{va:08x}" for va in vas}
        live = {k: v for k, v in _live_claims().items()
                if v["owner"] != owner or (keys is not None and k not in keys)}
        _write_claims(live)


def held(va, records):
    record = records.get(f"fn_{va:08x}")
    if record:
        return record["state"] != "ready"
    # An existing unaccepted source deserves review, not silent overwriting.
    return (env.C_DIR / f"fn_{va:08x}.c").is_file()


def import_wip():
    from . import ledger

    accepted = {e.va for e in ledger.cmre_entries() if e.record.get("c")}
    accepted |= {e.va for e in ledger.legacy_entries()}
    known = read()["functions"]
    counts = {"needs-review": 0, "evidence-needed": 0}
    for source in sorted(env.C_DIR.glob("fn_????????.c")):
        va = int(source.stem[3:], 16)
        if va in accepted or source.stem in known:
            continue
        head = source.read_text(encoding="utf-8").splitlines()[:8]
        wip = next((line.strip() for line in head if "WIP:" in line), None)
        state = "evidence-needed" if wip else "needs-review"
        update(va, state, wip or "Existing unaccepted source; pass and review status unknown",
               "Inspect existing source and report; classify the first unresolved obligation")
        counts[state] += 1
    return counts


def add_commands(sub):
    parser = sub.add_parser("work", help="persistent C queue reasons; never accepts source")
    commands = parser.add_subparsers(dest="action", required=True)
    p = commands.add_parser("list")
    p.add_argument("--state", choices=STATES)
    p.add_argument("--blocker")
    p.add_argument("-n", type=int, default=20)
    p.add_argument("--json", action="store_true")
    p.set_defaults(func=command)
    p = commands.add_parser("set")
    p.add_argument("va")
    p.add_argument("--state", choices=STATES, required=True)
    p.add_argument("--reason", required=True)
    p.add_argument("--retry-when", default="")
    p.add_argument("--blocker", default="")
    p.set_defaults(func=command)
    p = commands.add_parser("retry")
    p.add_argument("va")
    p.add_argument("--reason", required=True)
    p.add_argument("--changed", action="store_true")
    p.set_defaults(func=command)
    commands.add_parser("import-wip").set_defaults(func=command)
    p = commands.add_parser("claim", help="reserve functions for one concurrent agent")
    p.add_argument("vas", nargs="+")
    p.add_argument("--owner", required=True)
    p.add_argument("--hours", type=float, default=CLAIM_HOURS)
    p.set_defaults(func=command)
    p = commands.add_parser("release", help="return an agent's claimed functions (all by default)")
    p.add_argument("vas", nargs="*")
    p.add_argument("--owner", required=True)
    p.set_defaults(func=command)
    commands.add_parser("claims", help="list live claims").set_defaults(func=command)


def command(args):
    from .cli import _va
    if args.action == "set":
        result = update(_va(args.va), args.state, args.reason, args.retry_when, args.blocker)
    elif args.action == "retry":
        result = retry(_va(args.va), args.reason, args.changed)
    elif args.action == "import-wip":
        result = import_wip()
    elif args.action == "claim":
        result = claim([_va(v) for v in args.vas], args.owner, args.hours)
    elif args.action == "release":
        release(args.owner, [_va(v) for v in args.vas] or None)
        return 0
    elif args.action == "claims":
        for key, c in sorted(claims().items()):
            print(f"{key} {c['owner']} until {c['expires']}")
        return 0
    else:
        from . import ledger
        accepted = {e.va for e in ledger.cmre_entries() if e.record.get("c")}
        accepted |= {e.va for e in ledger.legacy_entries()}
        rows = [(k, v) for k, v in sorted(read()["functions"].items())
                if int(k[3:], 16) not in accepted
                and (not args.state or v["state"] == args.state)
                and (not args.blocker or v["blocker"] == args.blocker)]
        if args.json:
            print(json.dumps(dict(rows[:args.n]), indent=2))
        else:
            print(f"{len(rows)} pending recorded functions")
            for key, r in rows[:args.n]:
                stale = r["identity"] != identity(int(key[3:], 16))
                print(f"{key} {r['state']} failures={r['failures']} changed={stale} "
                      f"blocker={r['blocker'] or '-'} {r['reason'][:180]}")
        return 0
    print(json.dumps(result, indent=2))
    return 0
