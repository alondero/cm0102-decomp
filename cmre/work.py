"""C queue memory, not recovery evidence. Only cmre accept establishes recovery."""
from __future__ import annotations

from datetime import datetime, timedelta, timezone
from contextlib import contextmanager, ExitStack
import json
import time

from . import coordination, env

STATES = ("ready", "needs-review", "translation-defect", "harness-blocker", "evidence-needed")
HARD_HOLDS = ("harness-blocker", "evidence-needed")
_UNSET = object()
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


@contextmanager
def hold_fence(va, *, wait=False):
    """Serialize a function's hard-hold transitions with new publication."""
    deadline = time.monotonic() + 5
    while True:
        fence = coordination.lock(env.ANALYSIS / f'cmre/hold-fences/fn_{va:08x}.lock')
        try:
            fence.__enter__()
            break
        except coordination.BusyError:
            if not wait or time.monotonic() >= deadline:
                raise
            time.sleep(0.02)
    try:
        yield
    finally:
        fence.__exit__(None, None, None)


def attempt_fence(va):
    """Serialize one function's attempt reservations without blocking others."""
    return coordination.lock(env.ANALYSIS / f'cmre/attempt-fences/fn_{va:08x}.lock')


@contextmanager
def attempt_fences(vas):
    """Acquire distinct function fences in a stable order before the work lock."""
    with ExitStack() as stack:
        for va in sorted(set(vas)):
            stack.enter_context(attempt_fence(va))
        yield


def dispatch_hold(va, records):
    """Return scheduling hold details; ordinary failed attempts may be retried."""
    record = records.get(f'fn_{va:08x}')
    if record and record.get('state') in HARD_HOLDS:
        return {'reason': 'work-state-hold', 'work_state': record['state'],
                'retry_when': record.get('retry_when', ''), 'blocker': record.get('blocker', '')}
    if ((not record or record.get('state') != 'ready')
            and (env.C_DIR / f'fn_{va:08x}.c').is_file()):
        return {'reason': 'existing-source-needs-review',
                'retry_when': 'Inspect the existing unaccepted source before explicitly retrying'}
    return None


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


def describe(va, records=None):
    """One read-only view; queue history never substitutes for acceptance."""
    from . import ledger

    name = f'fn_{va:08x}'
    state = (read()['functions'] if records is None else records).get(name)
    legacy = next((entry for entry in ledger.legacy_entries() if entry.va == va), None)
    path = ledger.record_path(va)
    acceptance = {'state': 'unaccepted'}
    if legacy:
        acceptance = {'state': 'legacy', 'ledger': legacy.source,
                      'symbol': legacy.record.get('symbol')}
    elif path.is_file():
        record = json.loads(path.read_text(encoding='utf-8'))
        c = record.get('c')
        acceptance = {'state': 'accepted-lift', 'ledger': env.rel(path)}
        if c:
            source = env.C_DIR / f'{name}.c'
            digest = env.digest(source) if source.is_file() else None
            matches = bool(digest and digest == c.get('source_sha256'))
            acceptance.update(state='accepted-c' if matches else 'c-source-drift',
                              source=env.rel(source), source_sha256=digest,
                              accepted_source_sha256=c.get('source_sha256'))
    return {'function': name, 'acceptance': acceptance, 'work': state}


def update(va, state, reason, retry_when="", blocker="", *, report=None, event_id=None,
           preserve_hold=False, expected_previous=_UNSET, _remaining_holds=_UNSET, _resolution=None):
    if event_id is not None and (not isinstance(event_id, str) or not event_id or report is None):
        raise ValueError('Report events require a nonempty identity and report')
    if state not in STATES or not reason.strip():
        raise ValueError("A supported state and a nonempty reason are required")
    if state in ("harness-blocker", "evidence-needed") and not retry_when.strip():
        raise ValueError("Blocked work requires an explicit retry condition")
    from . import inventory
    if va not in inventory.load().functions:
        raise ValueError("Function is absent from the pinned inventory")
    with hold_fence(va, wait=True), locked():
        data = read()
        key = f"fn_{va:08x}"
        old = data["functions"].get(key, {})
        if expected_previous is not _UNSET and data['functions'].get(key) != expected_previous:
            raise ValueError('Work state changed during retry; inspect the current hold')
        if event_id is not None and event_id in old.get('report_events', {}):
            return old
        attempts = old.get("attempts", 0) + (report is not None)
        failures = old.get("failures", 0) + (report is not None and report["status"] != "pass")
        old_hard = old.get('state') in HARD_HOLDS
        generation = old.get('hold_generation', int(old_hard))
        record = {"state": state, "reason": reason, "retry_when": retry_when,
                  "blocker": blocker, "identity": identity(va), "attempts": attempts,
                  "failures": failures, "updated": datetime.now(timezone.utc).isoformat(),
                  "hold_generation": generation + (state in HARD_HOLDS and not old_hard)}
        if old_hard and _remaining_holds is _UNSET and (preserve_hold or state in HARD_HOLDS):
            for field in ('state', 'reason', 'retry_when', 'blocker', 'identity'):
                record[field] = old.get(field, '')
            extra = list(old.get('additional_holds', []))
            if state in HARD_HOLDS:
                new_hold = {'state': state, 'reason': reason, 'retry_when': retry_when, 'blocker': blocker}
                current = {field: old.get(field, '') for field in ('state', 'reason', 'retry_when', 'blocker')}
                if new_hold != current and new_hold not in extra:
                    extra.append(new_hold)
                    record['hold_generation'] = generation + 1
            if extra:
                record['additional_holds'] = extra
        elif old_hard and _remaining_holds is _UNSET:
            raise ValueError('Hard hold requires an explicit work retry')
        elif _remaining_holds is not _UNSET:
            if _remaining_holds:
                record['additional_holds'] = list(_remaining_holds)
            if old_hard and state in HARD_HOLDS:
                record['hold_generation'] = generation + 1
        if old.get('hold_resolutions'):
            record['hold_resolutions'] = list(old['hold_resolutions'])
        if old_hard and _resolution is not None:
            record.setdefault('hold_resolutions', []).append({'state': old['state'],
                'reason': old['reason'], 'retry_when': old.get('retry_when', ''), 'resolution': _resolution})
        if report is not None:
            record["last_report"] = {"key": report.get("key"), "status": report["status"],
                                     "profile": report.get("profile")}
            if report["status"] != "pass":
                record["first_failure"] = old.get("first_failure", {"reason": reason, "report": record["last_report"]})
        elif old.get("last_report"):
            record["last_report"] = old["last_report"]
        if "first_failure" in old:
            record["first_failure"] = old["first_failure"]
        if old.get('report_events') or event_id is not None:
            record['report_events'] = dict(old.get('report_events', {}))
            if event_id is not None:
                record['report_events'][event_id] = report['status']
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
                      "Retest the current source and review its matching report", report=report, preserve_hold=True)
    if problems and all(isinstance(p, dict) and p.get("kind") == "budget-asymmetry" for p in problems):
        return update(va, "evidence-needed", reason,
                      "Investigate the asymmetric stop or establish an adequate equal budget for both executions",
                      "execution-budget", report=report, preserve_hold=True)
    return update(va, "needs-review" if report["status"] == "pass" else "translation-defect",
                  reason, "Review listing and current report" if report["status"] == "pass"
                  else "Resolve the recorded discrepancy; five failed attempts is the session ceiling",
                  report=report, preserve_hold=True)


def retry(va, reason, changed=False):
    with locked():
        old = read()["functions"].get(f"fn_{va:08x}")
    if changed and (not old or old["identity"] == identity(va)):
        raise ValueError("Inputs are unchanged; resolve the recorded retry condition first")
    remaining = list(old.get('additional_holds', [])) if old else []
    if remaining:
        following = remaining.pop(0)
        return update(va, following['state'], following['reason'], following['retry_when'],
                      following.get('blocker', ''), expected_previous=old,
                      _remaining_holds=remaining, _resolution=reason)
    return update(va, "ready", reason, expected_previous=old, _remaining_holds=[], _resolution=reason)


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
    from . import tasks
    records = read()['functions']
    for va in vas:
        hold = dispatch_hold(va, records)
        if hold:
            raise ValueError(f"Function has a scheduling hold: {hold['reason']}; {hold['retry_when']}")
        if tasks.attempts_used(va, records.get(f'fn_{va:08x}', {})) >= tasks.MAX_ATTEMPTS:
            raise ValueError(f'fn_{va:08x} exhausted its five-attempt budget')
    live = _live_claims()
    taken = [f"fn_{va:08x} is claimed by {live[k]['owner']}" for va in vas
             if (k := f"fn_{va:08x}") in live and live[k]["owner"] != owner]
    if taken:
        raise ValueError("; ".join(taken))
    for va in vas:
        for path in (env.ANALYSIS / 'cmre/tasks').glob(f'c-{va:08x}-*/task.json'):
            manifest = json.loads(path.read_text(encoding='utf-8'))
            if manifest.get('status') in ('submitted', 'publishing'):
                raise ValueError(f"fn_{va:08x} has a pending {manifest['status']} task; reconcile it before claiming")
            lease = manifest.get('lease')
            if lease is not None and tasks._lease_expiry(lease) > time.time() and lease.get('owner') != owner:
                raise ValueError(f'fn_{va:08x} already has a live task lease')
    expires = (_now() + timedelta(hours=hours)).isoformat()
    for va in vas:
        live[f"fn_{va:08x}"] = {"owner": owner, "expires": expires}
    _write_claims(live)
    return {f"fn_{va:08x}": live[f"fn_{va:08x}"] for va in vas}


def claim(vas, owner, hours=CLAIM_HOURS):
    """Claim functions for one agent, all or nothing; renewing your own claim is allowed."""
    if not owner.strip() or hours <= 0:
        raise ValueError("A claim requires an owner name and a positive duration")
    vas = list(vas)
    with attempt_fences(vas), locked():
        return _claim_unlocked(vas, owner, hours)


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


def blocker_families(records):
    """Catalogue recorded blocker names without inferring aliases or retry policy."""
    result = {}
    for name, record in sorted(records.items()):
        family = record.get('blocker') or 'unclassified'
        row = result.setdefault(family, {'functions': [], 'retry_conditions': []})
        row['functions'].append(name)
        condition = record.get('retry_when', '')
        if condition and condition not in row['retry_conditions']:
            row['retry_conditions'].append(condition)
    for row in result.values():
        row['retry_conditions'].sort()
    return dict(sorted(result.items()))


def add_commands(sub):
    parser = sub.add_parser("work", help="persistent C queue reasons; never accepts source")
    commands = parser.add_subparsers(dest="action", required=True)
    p = commands.add_parser('show', help='one function: acceptance, queue history and every hold')
    p.add_argument('va')
    p.add_argument('--json', action='store_true')
    p.set_defaults(func=command)
    p = commands.add_parser('families', help='catalogue recorded blocker families')
    p.add_argument('--json', action='store_true')
    p.set_defaults(func=command)
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
    if args.action == 'show':
        result = describe(_va(args.va))
        if args.json:
            print(json.dumps(result, indent=2))
        else:
            record = result['work'] or {}
            print(f"{result['function']} acceptance={result['acceptance']['state']} "
                  f"work-history={record.get('state', 'unrecorded')} failures={record.get('failures', 0)}")
            for key, value in result['acceptance'].items():
                if key != 'state':
                    print(f'  {key}: {value}')
            for number, hold in enumerate([record, *record.get('additional_holds', [])]):
                if not hold:
                    continue
                print(f"  {'primary' if number == 0 else 'additional'} queue record: "
                      + json.dumps({key: hold[key] for key in ('state', 'blocker', 'reason', 'retry_when')
                                    if hold.get(key)}, ensure_ascii=True))
        return 0
    if args.action == 'families':
        result = blocker_families(read()['functions'])
        if args.json:
            print(json.dumps(result, indent=2))
        else:
            for family, row in result.items():
                print(f"{family}: {len(row['functions'])} functions")
                for condition in row['retry_conditions']:
                    print(f'  retry: {condition}')
        return 0
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
