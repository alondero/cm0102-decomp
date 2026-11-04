"""Recovery records: legacy ledgers (via registry.json) plus per-function cmre records.

One JSON file per function (recovery/ledger/fn_<va>.json) so parallel agents
never edit the same file. Legacy ledgers are read-only history.
"""
from __future__ import annotations

import glob
import json
from dataclasses import dataclass

from . import env, image

SCHEMA = "cmre-ledger/1"


@dataclass(frozen=True)
class Entry:
    va: int
    size: int
    sha256: str
    source: str  # ledger path
    category: str
    legacy: bool
    record: dict


def record_path(va: int):
    return env.LEDGER_DIR / f"fn_{va:08x}.json"


def blocked_path(va: int):
    return env.BLOCKED_DIR / f"fn_{va:08x}.json"


def registry() -> dict:
    return json.loads(env.REGISTRY.read_text(encoding="utf-8"))


def legacy_entries() -> list[Entry]:
    out = []
    for path in registry()["current"]:
        data = json.loads((env.ROOT / path).read_text(encoding="utf-8"))
        for f in data["functions"]:
            out.append(Entry(int(f["va"], 16), f["size"], f["sha256"], path, f.get("category", "application"),
                             True, f))
    return out


def cmre_entries() -> list[Entry]:
    out = []
    for p in sorted(glob.glob(str(env.LEDGER_DIR / "fn_*.json"))):
        r = json.loads(open(p, encoding="utf-8").read())
        out.append(Entry(int(r["va"], 16), r["extent"]["bytes"], r["extent"]["sha256"], env.rel(p),
                         r.get("category", "application"), False, r))
    return out


def recovered() -> dict[int, Entry]:
    out: dict[int, Entry] = {}
    for e in legacy_entries() + cmre_entries():
        out.setdefault(e.va, e)
    return out


def blocked() -> dict[int, dict]:
    out = {}
    for p in sorted(glob.glob(str(env.BLOCKED_DIR / "fn_*.json"))):
        r = json.loads(open(p, encoding="utf-8").read())
        out[int(r["va"], 16)] = r
    return out


def write_json(path, value) -> None:
    from .coordination import atomic_json

    atomic_json(path, value)


def verify() -> list[str]:
    """Integrity check of every current record. Returns a list of problems (empty = green)."""
    problems = []
    img = image.primary()
    seen: dict[int, str] = {}
    for path in registry()["current"]:
        data = json.loads((env.ROOT / path).read_text(encoding="utf-8"))
        if data.get("input_sha256") != env.TARGET_SHA256:
            problems.append(f"{path}: input_sha256 differs from pinned target")
    for e in legacy_entries() + cmre_entries():
        if e.va in seen:
            problems.append(f"{e.va:#010x} recorded twice: {seen[e.va]} and {e.source}")
        seen[e.va] = e.source
        if env.digest_bytes(img.read(e.va, e.size)) != e.sha256:
            problems.append(f"{e.source}: byte identity differs at {e.va:#010x}")
        if not e.legacy:
            r = e.record
            if r.get("schema") != SCHEMA:
                problems.append(f"{e.source}: schema {r.get('schema')!r}")
            if 'c_retractions' in r:
                from . import retract
                problems.extend(f"{e.source}: {problem}" for problem in retract.verify_history(r))
            for tier in ("lifted", "c"):
                t = r.get(tier)
                if not t:
                    continue
                src = env.ROOT / t["source"]
                if not src.exists() or env.digest(src) != t["source_sha256"]:
                    problems.append(f"{e.source}: {tier} source {t['source']} missing or changed since acceptance")
                for dependency, expected in t.get('source_dependencies', {}).items():
                    header = env.ROOT / dependency
                    if not header.is_file() or env.digest(header) != expected:
                        problems.append(f"{e.source}: {tier} dependency {dependency} missing or changed since acceptance")
    # Overlap check across all extents.
    spans = sorted((e.va, e.va + e.size, e.source) for e in legacy_entries() + cmre_entries())
    for (a0, a1, s0), (b0, b1, s1) in zip(spans, spans[1:]):
        if b0 < a1:
            problems.append(f"extent overlap {a0:#010x}..{a1:#010x} ({s0}) and {b0:#010x} ({s1})")
    for va in blocked():
        if record_path(va).exists():
            problems.append(f"fn_{va:08x} is both accepted and blocked; delete the blocked record")
    return problems
