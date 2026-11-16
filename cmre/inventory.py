"""Pinned Ghidra inventories: function extents, decoded calls and module candidates.

Parsed once and cached as JSON beside the runs folder, keyed by input hashes.
"""
from __future__ import annotations

import csv
import json
from dataclasses import dataclass
from functools import cache

from . import env


@dataclass(frozen=True)
class Function:
    va: int
    name: str
    body_bytes: int
    signature: str
    noreturn: bool


@dataclass
class Inventory:
    functions: dict[int, Function]
    callees: dict[int, list[int]]  # direct internal call/jump targets that are function entries
    modules: dict[int, list[str]]  # candidate original source paths
    entries_sorted: list[int]

    def next_entry_after(self, va: int) -> int:
        import bisect

        from . import image

        i = bisect.bisect_right(self.entries_sorted, va)
        return self.entries_sorted[i] if i < len(self.entries_sorted) else image.primary().text.end


def _check_pins() -> None:
    for path, expected in env.INVENTORY_SHA256.items():
        if not path.exists():
            raise SystemExit(f"Missing pinned inventory {env.rel(path)}; see docs/ANALYSIS-SETUP.md")
        if env.digest(path) != expected:
            raise SystemExit(f"Pinned inventory hash differs: {env.rel(path)}")


def load() -> Inventory:
    # Validate actual bytes on every call, including in-process cache hits.
    _check_pins()
    identity = (env.ANALYSIS, env.FUNCTIONS_TSV, env.CALLS_TSV, env.MODULES_JSON,
                tuple(env.INVENTORY_SHA256.items()))
    return _load_cached(identity)


@cache
def _load_cached(identity: tuple) -> Inventory:
    # Full declared digests, not truncated prefixes, distinguish cache versions.
    cache_key = env.digest_bytes(json.dumps(list(env.INVENTORY_SHA256.values())).encode())
    cache_path = env.ANALYSIS / "cmre" / f"inventory-v2-{cache_key}.json"
    if cache_path.exists():
        raw = json.loads(cache_path.read_text(encoding="utf-8"))
    else:
        functions = {}
        with env.FUNCTIONS_TSV.open(encoding="utf-8") as f:
            for r in csv.DictReader(f, delimiter="\t"):
                functions[r["entry_va"]] = [r["name"], int(r["body_bytes"]), r["signature"]]
        callees: dict[str, set] = {}
        with env.CALLS_TSV.open(encoding="utf-8") as f:
            for r in csv.DictReader(f, delimiter="\t"):
                t = r["target_address"]
                if r["function_va"] and t in functions and t != r["function_va"]:
                    callees.setdefault(r["function_va"], set()).add(t)
        modules = {
            m["function_va"]: m["candidate_paths"]
            for m in json.loads(env.MODULES_JSON.read_text(encoding="utf-8"))
        }
        raw = {
            "functions": functions,
            "callees": {k: sorted(v) for k, v in sorted(callees.items())},
            "modules": modules,
        }
        cache_path.parent.mkdir(parents=True, exist_ok=True)
        cache_path.write_text(json.dumps(raw), encoding="utf-8", newline="\n")
    functions = {
        int(k, 16): Function(int(k, 16), n, b, s, s.startswith("noreturn"))
        for k, (n, b, s) in raw["functions"].items()
    }
    return Inventory(
        functions=functions,
        callees={int(k, 16): [int(x, 16) for x in v] for k, v in raw["callees"].items()},
        modules={int(k, 16): v for k, v in raw["modules"].items()},
        entries_sorted=sorted(functions),
    )


# Preserve the public functools cache controls used by callers and tests.
load.cache_clear = _load_cached.cache_clear
load.cache_info = _load_cached.cache_info
load.cache_parameters = _load_cached.cache_parameters
