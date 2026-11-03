"""Deterministic work queue over the pinned inventory.

Order: functions whose original callees are all recovered first ("ready"), then
by body size, then by VA. Accepted and blocked functions are excluded. The same
repository state always yields the same queue.
"""
from __future__ import annotations

import functools
import re
import zlib
from dataclasses import dataclass

from . import env, image, inventory, ledger, lift

GENERATED_MAX_BYTES = 64
_EBP_BASE = re.compile(r"\[ebp\b")
_EBP_WRITE = re.compile(r"^ebp\b")


@dataclass(frozen=True)
class Item:
    va: int
    name: str
    body_bytes: int
    ready: bool
    pending_callees: int
    module: str


def frontier(limit: int = 10, max_bytes: int | None = None, min_bytes: int = 0, module: str | None = None,
             functions: set[int] | None = None) -> list[Item]:
    inv = inventory.load()
    done = set(ledger.recovered())
    skip = done | set(ledger.blocked())
    text = image.primary().text
    items = []
    for va, f in inv.functions.items():
        if functions is not None and va not in functions:
            continue
        if va in skip or not (text.va <= va < text.end):
            continue
        if f.body_bytes < min_bytes or (max_bytes is not None and f.body_bytes > max_bytes):
            continue
        paths = inv.modules.get(va, [])
        mod = paths[0].split("\\code\\")[-1] if paths else ""
        if module and module.lower() not in mod.lower():
            continue
        pending = [c for c in inv.callees.get(va, []) if c not in done]
        items.append(Item(va, f.name, f.body_bytes, not pending, len(pending), mod))
    items.sort(key=lambda i: (not i.ready, i.body_bytes, i.va))
    return items[:limit]


def compiler_generated(va: int) -> str | None:
    """`unwind-funclet`, `seh-finally`, `import-thunk`, or None for code a C translation owns.

    Only image metadata proves the kind (`cmre.generated`); `generated_shape` is the
    unproven shape heuristic, kept to report entries whose shape and proof disagree.
    """
    from . import generated

    return generated.kind(va)


@functools.cache
def generated_shape(va: int) -> str | None:
    """Shape heuristic: EBP used as a memory base before being set, or one import JMP."""
    f = inventory.load().functions.get(va)
    if f is None or f.body_bytes > GENERATED_MAX_BYTES:
        return None
    try:
        insns = sorted(lift.discover(va).insns.values(), key=lambda i: i.va)
    except Exception:
        return None
    if len(insns) == 1 and insns[0].kind == "import_jmp":
        return "import-thunk"
    for ins in insns:
        if ins.mnemonic in ("push", "pop") and ins.op_str == "ebp" or _EBP_WRITE.match(ins.op_str):
            return None
        if _EBP_BASE.search(ins.op_str):
            return "unwind-funclet"
    return None


def in_shard(va: int, shard: tuple[int, int]) -> bool:
    """Stable 1-based shard K of N by function ID, so agents that never talk can split the queue."""
    k, n = shard
    if not 1 <= k <= n:
        raise ValueError(f"Invalid shard {k}/{n}: expected 1 <= K <= N")
    return zlib.crc32(f"fn_{va:08x}".encode()) % n == k - 1


def c_frontier(limit: int = 10, max_bytes: int | None = None, include_generated: bool = False,
               min_bytes: int = 0, module: str | None = None, include_held: bool = False,
               owner: str | None = None, shard: tuple[int, int] | None = None) -> list[Item]:
    """Tier-2 queue: accepted lifted functions without C, callee-complete first, then smallest.
    Compiler-generated funclets and thunks are excluded unless requested. Live claims of
    other agents are held work; `owner` sees its own claims."""
    inv = inventory.load()
    entries = ledger.cmre_entries()
    from . import work
    held = work.read()["functions"]
    claimed = {k for k, c in work.claims().items() if c["owner"] != owner}
    with_c = {e.va for e in entries if e.record.get("c")} | {e.va for e in ledger.legacy_entries()}
    items = []
    for e in entries:
        if e.va in with_c:
            continue
        if shard and not in_shard(e.va, shard):
            continue
        if not include_held and (work.held(e.va, held) or f"fn_{e.va:08x}" in claimed):
            continue
        f = inv.functions.get(e.va)
        size = f.body_bytes if f else e.size
        if size < min_bytes or (max_bytes is not None and size > max_bytes):
            continue
        if not include_generated and compiler_generated(e.va):
            continue
        paths = inv.modules.get(e.va, [])
        mod = paths[0].split("\\code\\")[-1] if paths else ""
        if module and module.lower() not in mod.lower():
            continue
        pending = [c for c in inv.callees.get(e.va, []) if c not in with_c]
        items.append(Item(e.va, f.name if f else "", size, not pending, len(pending), mod))
    items.sort(key=lambda i: (not i.ready, i.body_bytes, i.va))
    return items[:limit]


def claim_next(limit: int, owner: str, hours: float | None = None, **filters) -> list[Item]:
    """Select and claim the next C items for one agent; concurrent callers get disjoint items.

    The slow selection runs unlocked over a wider pool; only the claim check and write
    hold the work lock, which test recording also uses.
    """
    from . import work
    pool = c_frontier(limit * 4 + 20, owner=owner, **filters)
    from contextlib import ExitStack
    from . import coordination
    busy = set()
    while True:
        taken = {k for k, c in work.claims().items() if c['owner'] != owner}
        chosen = [i for i in pool if i.va not in busy and f'fn_{i.va:08x}' not in taken][:limit]
        if not chosen:
            return []
        with ExitStack() as fences:
            for va in sorted({i.va for i in chosen}):
                try:
                    fences.enter_context(work.attempt_fence(va))
                except coordination.BusyError:
                    busy.add(va)
                    break
            else:
                with work.locked():
                    taken = {k for k, c in work.claims().items() if c['owner'] != owner}
                    if any(f'fn_{i.va:08x}' in taken for i in chosen):
                        continue
                    work._claim_unlocked([i.va for i in chosen], owner, hours or work.CLAIM_HOURS)
                    return chosen


def coverage_summary() -> dict:
    inv = inventory.load()
    total = sum(f.body_bytes for f in inv.functions.values())
    rec = ledger.recovered()
    legacy = [e for e in rec.values() if e.legacy]
    new = [e for e in rec.values() if not e.legacy]
    c_tier = [e for e in new if e.record.get("c")]
    source = legacy + c_tier
    generated = [va for va in inv.functions if compiler_generated(va)]
    body = lambda es: sum(inv.functions[e.va].body_bytes for e in es if e.va in inv.functions)
    percent = lambda size: round(100 * size / total, 3) if total else 0.0
    return {
        "input_sha256": env.TARGET_SHA256,
        "inventory_functions": len(inv.functions),
        "inventory_body_bytes": total,
        "recovered_functions": len(rec),
        "recovered_body_bytes": body(rec.values()),
        "recovered_percent": percent(body(rec.values())),
        "legacy_functions": len(legacy),
        "lifted_functions": len(new),
        "c_tier_functions": len(c_tier),
        "c_tier_body_bytes": body(c_tier),
        "legacy_body_bytes": body(legacy),
        "source_functions": len(source),
        "source_body_bytes": body(source),
        "source_percent": percent(body(source)),
        "source_unmapped_functions": sum(e.va not in inv.functions for e in source),
        "compiler_generated_functions": len(generated),
        "compiler_generated_body_bytes": sum(inv.functions[va].body_bytes for va in generated),
        "blocked_functions": len(ledger.blocked()),
        "integrated_functions": sum(1 for e in rec.values() if e.record.get("status", {}).get("integrated")
                                    or e.record.get("integrated")),
    }
