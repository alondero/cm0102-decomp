"""Hash-keyed navigation packets. Original instructions remain the evidence.

Packets never change recovery records, infer signatures or establish acceptance.
Absent dependencies are tracked too, so a newly written callee expires a packet.
"""
from __future__ import annotations

import json
from pathlib import Path

import capstone

from . import coordination, drafts, env, families, image, inventory, lift, semantic


def packet_root():
    return env.ANALYSIS / "cmre/evidence"


def dependencies(va: int, peer: int | None) -> list[Path]:
    paths = {env.TARGET, env.FUNCTIONS_TSV, env.CALLS_TSV, env.MODULES_JSON, env.REGISTRY}
    if peer is not None:
        representative, _ = packet(peer)
        paths.update((representative, representative.with_suffix(".json")))
    # Reporting and draft-repair implementation, without changing harness identity.
    paths.update(Path(__file__).parent.glob("*.py"))
    paths.update((env.ROOT / "recovery/include").rglob("*.*"))
    paths.update(Path(env.tool(name)) for name in ("gcc", "as", "ld", "nm"))
    if env.REGISTRY.exists():
        for name in json.loads(env.REGISTRY.read_text(encoding='utf-8'))['current']:
            frozen = env.ROOT / name
            paths.add(frozen)
            data = json.loads(frozen.read_text(encoding='utf-8'))
            paths.update(drafts.legacy_sources(data).values())
            for function in data.get('functions', []):
                paths.update(drafts.legacy_sources(data, function).values())
    for owner in {va, peer} - {None}:
        callees = set(lift.discover(owner).callees)
        raw = drafts.DRAFTS / f"fn_{owner:08x}.c"
        if raw.exists():
            # Include function references present only in pseudocode as well.
            _, references = drafts._functions(raw.read_text(encoding="utf-8"))
            callees.update(references)
        for address in {owner, *callees}:
            name = f"fn_{address:08x}"
            paths.update((env.C_DIR / f"{name}.c", env.HINTS_DIR / f"{name}.json",
                          env.LEDGER_DIR / f"{name}.json", drafts.DRAFTS / f"{name}.c"))
        paths.add(drafts.DRAFTS / f"fn_{owner:08x}.globals.tsv")
    return sorted(paths)


def caller_windows(va: int) -> str:
    """Every indexed direct CALL and a bounded original, file-backed continuation.

    Windows are navigation aids, not a dataflow analysis: branch successors,
    pass-through wrappers and indirect callers need separate review.
    """
    img = image.primary()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    sites = semantic._call_sites().get(va, ())
    lines = [f"Direct caller windows: {len(sites)}; at most 24 following instructions each.",
             "Not exhaustive control flow; not a return-width proof. Review wrappers and indirect callers."]
    for site in sites:
        section = img.section_at(site)
        if section is None or not section.executable:
            lines.append(f"UNRESOLVED call site {site:#010x}: no executable section")
            continue
        start = site - section.va
        raw = section.raw[start:start + 5 + 24 * 15]
        decoded = list(md.disasm(raw, site, count=25))
        if (not decoded or decoded[0].size != 5 or decoded[0].bytes[0] != 0xE8
                or decoded[0].operands[0].imm != va):
            lines.append(f"UNRESOLVED call site {site:#010x}: indexed target differs from original E8 bytes")
            continue
        lines.append(f"caller at {site:#010x}:")
        stopped = False
        for ins in decoded:
            lines.append(f"{ins.address:08x}  {ins.bytes.hex()}  {ins.mnemonic} {ins.op_str}".rstrip())
            if ins.address != site and any(ins.group(g) for g in (capstone.CS_GRP_CALL,
                    capstone.CS_GRP_JUMP, capstone.CS_GRP_RET, capstone.CS_GRP_IRET)):
                lines.append("  window ends at control transfer; follow original successors as needed")
                stopped = True
                break
        if not stopped:
            lines.append("  window ends at instruction limit or file-backed section end; remaining flow unshown")
    return "\n".join(lines) + "\n"


def provenance(va: int) -> str:
    from . import ledger

    lines = ["Callee declaration provenance (a declaration is not ABI evidence):"]
    inv = inventory.load()
    legacy = {entry.va: entry for entry in ledger.legacy_entries()}
    for callee in sorted(lift.discover(va).callees):
        name = f"fn_{callee:08x}"
        source = env.C_DIR / f"{name}.c"
        ghidra = drafts.DRAFTS / f"{name}.c"
        if callee in legacy:
            entry = legacy[callee]
            lines.append(f'{name}: {drafts.legacy_provenance(entry)}')
            continue
        if drafts.accepted_prototype(callee):
            record = env.LEDGER_DIR / f"{name}.json"
            data = json.loads(record.read_text(encoding="utf-8")) if record.exists() else {}
            matches = data.get("c", {}).get("source_sha256") == env.digest(source)
            state = "accepted C (matching source hash)" if matches else "current C; acceptance not established here"
            path = source
        elif ghidra.exists() and f"cm_fn_{callee:08x}" in drafts.prototype(callee):
            state, path = "unvalidated Ghidra draft or inventory fallback", ghidra
        else:
            state, path = "unvalidated inventory signature", env.FUNCTIONS_TSV
        lines.append(f"{name}: {state}; {env.rel(path)} sha256={env.digest(path)}")
        if callee not in inv.functions:
            lines.append("  no inventory function entry; contract remains unresolved")
    return "\n".join(lines) + "\n"


def render(va: int, peer: int | None) -> str:
    shared = ""
    auxiliary = ""
    if peer is not None:
        same, delta = families.delta(peer, va)
        if not same:
            raise ValueError("Peer must have the same instruction shape; inspect separate full briefs")
        representative, _ = packet(peer)
        shared = (f"Shared representative: {env.rel(representative)}\n"
                  "Read its complete listing once, then every member delta. Each member needs its own review and gate.\n"
                  + (delta or "No operand differences; review this member's ABI, callers and dependencies.") + "\n")
        member = lift.discover(va)
        rows = [f"table {t.va:08x} -> {' '.join(f'{x:08x}' for x in t.targets)}"
                for t in member.tables.values()]
        rows += [f"data {d.va:08x} {d.raw.hex()}" for d in member.data]
        auxiliary = "Member jump tables and embedded data (not shared):\n" + "\n".join(rows) + "\n"
    return (f"SHA-256 {env.TARGET_SHA256}; {env.ADDRESS_CONVENTION}\n"
            "Navigation packet; Ghidra drafts and declarations are inputs, never evidence or acceptance.\n"
            "In the brief, C means a source file exists; acceptance is stated separately in provenance.\n"
            + shared + drafts.brief(va, asm=peer is None) + auxiliary + "\n" + provenance(va)
            + "\n" + caller_windows(va))


def packet(va: int, peer: int | None = None) -> tuple[Path, bool]:
    if peer == va:
        raise ValueError("Choose a different family representative")
    paths = dependencies(va, peer)
    fingerprint = {str(p): env.digest(p) if p.is_file() else None for p in paths}
    identity = {"schema": "cmre-evidence/1", "target": env.TARGET_SHA256,
                "address_convention": env.ADDRESS_CONVENTION, "va": va, "peer": peer,
                "dependencies": fingerprint}
    if env.TARGET in paths and fingerprint[str(env.TARGET)] != env.TARGET_SHA256:
        raise ValueError("Pinned target hash mismatch; refusing reusable evidence")
    key = env.digest_bytes(json.dumps(identity, sort_keys=True).encode())
    path = packet_root() / f"fn_{va:08x}-{key}.txt"
    manifest = path.with_suffix(".json")
    if path.exists() and manifest.exists():
        try:
            saved = json.loads(manifest.read_text(encoding="utf-8"))
            if (isinstance(saved, dict) and saved.get("identity") == identity
                    and saved.get("content_sha256") == env.digest(path)):
                return path, True
        except (OSError, ValueError):
            pass
    content = render(va, peer)
    # Refuse publication if source evidence changed during packet preparation.
    if fingerprint != {str(p): env.digest(p) if p.is_file() else None for p in dependencies(va, peer)}:
        raise ValueError("Evidence inputs changed during rendering; retry after edits finish")
    coordination.atomic_bytes(path, content.encode("utf-8"))
    coordination.atomic_json(manifest, {"identity": identity, "content_sha256": env.digest(path)})
    return path, False
