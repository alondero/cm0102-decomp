"""Pinned-inventory boundary reconciliation: exceptions and their evidenced dispositions.

Read-only and local. Reconciles the pinned function inventory with accepted
extents, compiler-generated proofs (`cmre.generated`) and every `.text` byte.
An exception is `open` until mechanical evidence from the pinned image or an
accepted record disposes of it. Reports hold VAs, function IDs, counts and
evidence VAs only: no instruction bytes or listings. Entries without an inventory
entry are shown in the shard of their own VA, provisionally: an inventory
revision decides their ownership.
"""
from __future__ import annotations

import bisect
from collections import Counter
import json
import re
import struct

from . import env

SCHEMA = "cmre-boundaries/1"
# MSVC alignment fill: one-byte NOP/INT3 and the multi-byte no-op forms it emits.
FILL = sorted((bytes.fromhex(h) for h in (
    "90", "cc", "8bc0", "8bc9", "8bff", "8d4000", "8d4900", "8d642400", "8d742600", "8d7600",
    "8d9b00000000", "8da42400000000", "8db42600000000", "8dbf00000000")), key=len, reverse=True)
LIMITATIONS = [
    "Dispositions are mechanical: alignment fill, referenced data, jump tables and metadata proofs.",
    "Unresolved .text intervals may hold code that the pinned inventory omits, or data; both stay open.",
    "Shards of entries outside the inventory are provisional until an explicit inventory revision.",
    "This report is accounting, not recovery evidence; only `cmre accept` recovers.",
]


def _id(va: int) -> str:
    return f"fn_{va:08x}"


def is_fill(raw: bytes) -> bool:
    p = 0
    while p < len(raw):
        token = next((t for t in FILL if raw.startswith(t, p)), None)
        if token is None:
            return False
        p += len(token)
    return True


def _references(img, va: int) -> list[int]:
    """Absolute 32-bit references in any section and rel32 CALL/JMP sites in .text."""
    needle = struct.pack("<I", va)
    out = []
    for s in img.sections:
        out += [s.va + m.start() for m in re.finditer(re.escape(needle), s.raw)]
    text = img.text
    for m in re.finditer(rb"(?=[\xe8\xe9](....))", text.raw, re.S):
        site = text.va + m.start()
        if (site + 5 + struct.unpack("<i", m.group(1))[0]) & 0xFFFFFFFF == va:
            out.append(site)
    return sorted(out)


def _extent_disposition(img, entry, fn, lifted) -> tuple[str, bool, dict]:
    """Why an accepted extent differs from the Ghidra instruction-body count."""
    lo, hi = entry.va, entry.va + entry.size
    if lifted is None:
        return "undiscoverable", True, {}
    end = max([i.va + i.size for i in lifted.insns.values()]
              + [t.va + 4 * len(t.targets) for t in lifted.tables.values()])
    code = sum(i.size for i in lifted.insns.values() if i.va < hi)
    tables = sum(4 * len(t.targets) for t in lifted.tables.values() if t.va < hi)
    refs = {v for i in lifted.insns.values() for v in i.substitutions}
    fill = data = external = unexplained = 0
    for run in lifted.data:
        if run.va >= hi:
            continue
        raw = run.raw[: hi - run.va]
        if run.va in refs:
            data += len(raw)
            end = max(end, run.va + len(raw))  # e.g. a switch index table after the last instruction
        elif is_fill(raw):
            fill += len(raw)
        elif _references(img, run.va):
            external += len(raw)  # e.g. SEH filter/except blocks named by a scope table
        else:
            unexplained += len(raw)
    tail = img.read(end, hi - end) if end < hi else b""
    if tail and is_fill(tail):
        fill, end = fill + len(tail), hi
    detail = {"extent": entry.size, "body": fn.body_bytes, "code": code, "tables": tables,
              "fill": fill, "referenced_data": data, "externally_referenced": external,
              "unexplained": unexplained, "discovered_end": f"{end:#010x}"}
    if end != hi:
        return "accepted extent differs from current discovery", True, detail
    if code != fn.body_bytes:
        return "decoded instructions differ from inventory body", True, detail
    if unexplained:
        return "unreferenced non-fill bytes inside extent", True, detail
    parts = [k for k in ("tables", "fill", "referenced_data", "externally_referenced") if detail[k]]
    return "extent = body + " + " + ".join(parts), False, detail


def _unproven_disposition(references: list[int]) -> tuple[str, bool]:
    """A generated shape without EH or import metadata is ordinary code; funclets are never called."""
    if references:
        return "shape only; called or referenced directly, so ordinary C owns it", False
    return "shape only; no metadata or references, so ordinary C owns it", False


def _legacy_disposition(entry, fn) -> tuple[str, bool, dict] | None:
    """A legacy record that decoded its whole extent explains the body difference; else discover."""
    decoded = entry.record.get("instruction_bytes") if entry.legacy else None
    if decoded is None or decoded != entry.size:
        return None
    detail = {"legacy": True, "ledger": entry.source, "extent": entry.size, "body": fn.body_bytes,
              "instruction_bytes": decoded}
    return (f"legacy record decodes its whole extent; inventory body "
            f"{'under' if decoded > fn.body_bytes else 'over'}counts", False, detail)


def gather() -> dict:
    from . import generated, image, inventory, ledger, lift, queue, shards

    img = image.primary()
    text = img.text
    inv = inventory.load()
    accepted = ledger.recovered()
    exceptions: list[dict] = []

    def add(kind, va, disposition, open_, **evidence):
        exceptions.append({"kind": kind, "va": f"{va:#010x}", "function": _id(va),
                           "shard": shards.shard_of(va), "inventory": va in inv.functions,
                           "disposition": disposition, "open": open_, "evidence": evidence})

    discovered: dict[int, object] = {}

    def discover(va):
        if va not in discovered:
            try:
                discovered[va] = lift.discover(va)
            except BaseException:  # noqa: BLE001 - discover exits on unrecoverable regions
                discovered[va] = None
        return discovered[va]

    # Accepted entries absent from the inventory.
    for e in sorted(accepted.values(), key=lambda e: e.va):
        if e.va in inv.functions:
            continue
        refs = _references(img, e.va)
        owners = sorted({_id(o) for o in map(generated.owner, refs) if o is not None})
        manual = bool(e.record.get("manual_boundary_evidence"))
        add("outside-inventory", e.va,
            "accepted with manual boundary; pinned inventory omits the entry" if refs and manual
            else "no reference or boundary evidence", not (refs and manual),
            ledger=e.source, references=[f"{r:#010x}" for r in refs], referencing_entries=owners,
            manual_boundary_evidence=manual)
    # Accepted extents that differ from the inventory body.
    for e in sorted(accepted.values(), key=lambda e: e.va):
        fn = inv.functions.get(e.va)
        if fn is None or e.size == fn.body_bytes:
            continue
        disposition, open_, detail = (_legacy_disposition(e, fn)
                                      or _extent_disposition(img, e, fn, discover(e.va)))
        add("extent-body", e.va, disposition, open_, **{"legacy": e.legacy, **detail})
    # Inventory bodies that run past the next entry.
    for a, b in zip(inv.entries_sorted, inv.entries_sorted[1:]):
        over = a + inv.functions[a].body_bytes - b
        if over > 0:
            own = accepted.get(a)
            disjoint = own is not None and own.va + own.size <= b
            add("inventory-body-overlap", a,
                "inventory body overcounts; accepted extent ends at the next entry" if disjoint
                else "overlapping bodies unresolved", not disjoint,
                next_entry=_id(b), overlap_bytes=over,
                accepted_extent=own.size if own else None)
    # Compiler-generated: proof, parent and regeneration path.
    proofs = generated.proofs()
    parents = generated.parents()
    for va in sorted(set(inv.functions) - set(proofs)):
        if queue.generated_shape(va):
            refs = _references(img, va)
            add("generated-unproven", va, *_unproven_disposition(refs),
                shape=queue.generated_shape(va), references=[f"{r:#010x}" for r in refs])
    generated_rows = {}
    for va, p in proofs.items():
        parent = parents.get(p.site) if p.site is not None else None
        row = {"kind": p.kind, "regeneration": generated.REGENERATION[p.kind],
               "metadata": f"{p.metadata:#010x}" if p.metadata is not None else None,
               "site": f"{p.site:#010x}" if p.site is not None else None,
               "parent": _id(parent) if parent is not None else p.import_name}
        generated_rows[_id(va)] = row
        if p.kind != "import-thunk" and parent is None:
            add("generated-parent-outside-inventory", va,
                "registration site lies in code no inventory entry owns", True,
                metadata=row["metadata"], site=row["site"])
    # Every .text byte: owned by an entry, alignment fill, handler stub, or unresolved.
    owned = bytearray(text.virtual_size)

    def mark(lo, hi):
        lo, hi = max(lo, text.va), min(hi, text.end)
        if lo < hi:
            owned[lo - text.va:hi - text.va] = b"\1" * (hi - lo)

    for e in accepted.values():
        mark(e.va, e.va + e.size)
    for va, fn in inv.functions.items():
        if va in accepted:
            continue
        lifted = discover(va)
        if lifted is None:
            mark(va, va + fn.body_bytes)
            continue
        for i in lifted.insns.values():
            mark(i.va, i.va + i.size)
        for t in lifted.tables.values():
            mark(t.va, t.va + 4 * len(t.targets))
    stubs = {stub: info for info, (stub, _) in generated._cxx().items()}
    sites = {site for _, site in generated._cxx().values()} | set(generated._seh().values())
    raw = text.raw + bytes(text.virtual_size - len(text.raw))
    bytes_by = Counter()
    intervals = Counter()
    p = 0
    while p < len(owned):
        if owned[p]:
            p += 1
            continue
        q = p
        while q < len(owned) and not owned[q]:
            q += 1
        lo, hi = text.va + p, text.va + q
        inside = [s for s in stubs if lo <= s < hi]
        rest = bytearray(raw[p:q])
        for s in inside:
            rest[s - lo:s - lo + 10] = b"\x90" * 10
        # Zero bytes are section padding only after the last code; elsewhere they may be data.
        body = bytes(rest).rstrip(b"\x00") if hi == text.end else bytes(rest)
        if not body or is_fill(body):
            kind = "handler-stubs" if inside else "fill"
        else:
            kind = "unresolved"
            registered = sorted(s for s in sites if lo <= s < hi)
            add("unowned-text", lo,
                "contains a registered EH prologue: code the pinned inventory omits" if registered
                else "unresolved: code or data outside every entry", True,
                end=f"{hi:#010x}", bytes=hi - lo, handler_stubs=len(inside),
                eh_registration_sites=[f"{s:#010x}" for s in registered])
        bytes_by[kind] += hi - lo
        intervals[kind] += 1
        p = q
    stub_parents = Counter("parent" if parents.get(site) is not None else "outside-inventory"
                           for _, site in generated._cxx().values())
    return {"schema": SCHEMA, "input_sha256": env.TARGET_SHA256,
            "inventory": {"sha256": {env.rel(k): v for k, v in env.INVENTORY_SHA256.items()},
                          "functions": len(inv.functions),
                          "body_bytes": sum(f.body_bytes for f in inv.functions.values())},
            "text": {"start": f"{text.va:#010x}", "end": f"{text.end:#010x}", "bytes": text.virtual_size,
                     "owned_by_entries": sum(owned),
                     "unowned": {k: {"intervals": intervals[k], "bytes": bytes_by[k]} for k in sorted(intervals)},
                     "handler_stubs": {"count": len(stubs), "registration": dict(sorted(stub_parents.items()))}},
            "generated": generated_rows,
            "exceptions": exceptions,
            "limitations": LIMITATIONS}


def summary(report: dict, shard: int | None = None) -> dict:
    rows = [e for e in report["exceptions"] if shard is None or e["shard"] == shard]
    kinds = sorted({e["kind"] for e in rows})
    count = lambda xs: dict(sorted(Counter(e["kind"] for e in xs).items()))
    gen = [g for f, g in report["generated"].items()
           if shard is None or _shard_of_id(f) == shard]
    return {"exceptions": len(rows), "open": sum(e["open"] for e in rows),
            "by_kind": {k: {"total": count(rows).get(k, 0),
                            "open": count([e for e in rows if e["open"]]).get(k, 0)} for k in kinds},
            "generated": {"functions": len(gen),
                          "parent_verified": sum(g["parent"] is not None for g in gen),
                          "by_kind": dict(sorted(Counter(g["kind"] for g in gen).items()))}}


def _shard_of_id(function: str) -> int:
    from . import shards

    return shards.shard_of(int(function[3:], 16))


def command(args) -> int:
    report = gather()
    if args.shard is not None:
        report["exceptions"] = [e for e in report["exceptions"] if e["shard"] == args.shard]
        report["generated"] = {f: g for f, g in report["generated"].items() if _shard_of_id(f) == args.shard}
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        t, inv = report["text"], report["inventory"]
        print(f"SHA-256 {report['input_sha256']}; inventory {inv['functions']} functions, {inv['body_bytes']} body bytes")
        print(f".text {t['start']}..{t['end']} {t['bytes']}B: owned by entries {t['owned_by_entries']}B; "
              + "; ".join(f"{k} {v['intervals']} intervals {v['bytes']}B" for k, v in t["unowned"].items()))
        print(f"handler stubs {t['handler_stubs']['count']}: registration {t['handler_stubs']['registration']}")
        for k in ([args.shard] if args.shard else [None, *range(1, 9)]):
            s = summary(report, k)
            g = s["generated"]
            print(f"{'all' if k is None else f'shard {k}/8'}: exceptions {s['exceptions']} open {s['open']} | "
                  f"generated {g['functions']} parent-verified {g['parent_verified']} | "
                  + " ".join(f"{kind} {v['open']}/{v['total']}" for kind, v in s["by_kind"].items()))
        if args.ids:
            for e in report["exceptions"]:
                if e["open"] or args.all:
                    print(f"  {'OPEN' if e['open'] else 'ok  '} {e['kind']} {e['function']} s{e['shard']}: {e['disposition']}")
    if args.write:
        from .coordination import atomic_json
        from .shards import digest

        path = env.ANALYSIS / "cmre/boundaries" / f"{digest(report)}.json"
        atomic_json(path, report)
        print(f"report: {env.rel(path)}")
    return 0


def add_commands(sub) -> None:
    p = sub.add_parser("boundaries", help="inventory boundary and compiler-generated reconciliation (read-only)")
    p.add_argument("--shard", type=int, choices=range(1, 9), metavar="K", help="one shard, 1..8")
    p.add_argument("--ids", action="store_true", help="list open exceptions")
    p.add_argument("--all", action="store_true", help="with --ids: include disposed exceptions")
    p.add_argument("--json", action="store_true")
    p.add_argument("--write", action="store_true", help="save under CM0102/Analysis/cmre/boundaries/<digest>.json")
    p.set_defaults(func=command)
