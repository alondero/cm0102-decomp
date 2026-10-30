"""Instruction-shape candidates and exact member deltas; never equivalence proof."""
from __future__ import annotations

from collections import defaultdict
import difflib
import hashlib
import json

import capstone
from capstone import x86_const as X

from . import coordination, env, inventory, ledger, lift, queue, work


def describe(insns):
    ordered = sorted(insns, key=lambda i: i.va)
    positions = {i.va: n for n, i in enumerate(ordered)}
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    shape, listing = [], []
    for n, original in enumerate(ordered):
        decoded = list(decoder.disasm(original.raw, original.va))
        if len(decoded) != 1:
            raise ValueError("Cannot decode an instruction family member")
        ins = decoded[0]
        operands = []
        for operand in ins.operands:
            if operand.type == X.X86_OP_REG:
                value = ("reg", ins.reg_name(operand.reg))
            elif operand.type == X.X86_OP_MEM:
                m = operand.mem
                value = ("mem", ins.reg_name(m.segment), ins.reg_name(m.base),
                         ins.reg_name(m.index), m.scale)
            elif operand.type == X.X86_OP_IMM:
                value = ("target", positions[original.target]) if original.target in positions else ("imm",)
            else:
                raise ValueError("Unsupported family operand")
            operands.append((operand.size, value))
        shape.append((original.kind, ins.mnemonic, operands))
        # Keep every constant, displacement, register and external address in the
        # delta; only internal branch destinations become stable ordinal labels.
        operand_text = original.op_str
        if original.target in positions:
            operand_text = f"instruction[{positions[original.target]}]"
        listing.append(f"{n:04d} {ins.mnemonic} {operand_text}")
    digest = hashlib.sha256(json.dumps(shape, separators=(",", ":")).encode()).hexdigest()
    return digest, listing


def index():
    from . import recover

    destination = env.ANALYSIS / "cmre/families/index.json"
    cache = json.loads(destination.read_text(encoding="utf-8")) if destination.exists() else {}
    version = {"harness": recover.harness_digest(), "indexer": env.digest(__file__)}
    old = cache.get("entries", {}) if cache.get("version") == version else {}
    entries = {}
    for e in ledger.cmre_entries():
        if queue.compiler_generated(e.va):
            continue
        key = f"fn_{e.va:08x}"
        hint = env.HINTS_DIR / f"{key}.json"
        identity = [e.record.get("extent", {}).get("sha256"), env.digest(hint) if hint.exists() else None]
        if key in old and old[key]["identity"] == identity:
            entries[key] = old[key]
            continue
        recovered = lift.discover(e.va)
        if recovered.problems:
            continue
        fingerprint, _ = describe(recovered.insns.values())
        entries[key] = {"identity": identity, "shape": fingerprint}
    coordination.atomic_json(destination, {"version": version, "entries": entries})
    return entries


def groups(min_members=2, module=None, include_held=False):
    inv = inventory.load()
    accepted = {e.va for e in ledger.cmre_entries() if e.record.get("c")}
    accepted |= {e.va for e in ledger.legacy_entries()}
    held = work.read()["functions"]
    grouped = defaultdict(list)
    for key, data in index().items():
        va = int(key[3:], 16)
        if module and not any(module.lower() in p.lower() for p in inv.modules.get(va, [])):
            continue
        if va not in accepted and not include_held and work.held(va, held):
            continue
        grouped[data["shape"]].append(va)
    result = []
    for fingerprint, members in grouped.items():
        pending = sorted(set(members) - accepted)
        if len(members) < min_members or not pending:
            continue
        representatives = sorted(set(members) & accepted)
        result.append({"family": fingerprint, "representative": representatives[0] if representatives else pending[0],
                       "accepted_representative": bool(representatives), "pending": pending,
                       "pending_bytes": sum(inv.functions[v].body_bytes for v in pending)})
    return sorted(result, key=lambda r: (-r["pending_bytes"], r["family"]))


def delta(left, right):
    a, b = lift.discover(left), lift.discover(right)
    if a.problems or b.problems:
        raise ValueError("Resolve lift problems before comparing family members")
    fa, la = describe(a.insns.values())
    fb, lb = describe(b.insns.values())
    return fa == fb, "\n".join(difflib.unified_diff(la, lb, fromfile=f"fn_{left:08x}",
                                                    tofile=f"fn_{right:08x}", lineterm=""))


def add_commands(sub):
    p = sub.add_parser("family", help="original instruction-shape groups; hypotheses, not acceptance")
    p.add_argument("va", nargs="?", help="representative for --peer")
    p.add_argument("--peer", help="show every instruction operand difference")
    p.add_argument("--module")
    p.add_argument("--include-held", action="store_true")
    p.add_argument("--min-members", type=int, default=2)
    p.add_argument("-n", type=int, default=10)
    p.add_argument("--json", action="store_true")
    p.set_defaults(func=command)


def command(args):
    from .cli import _va
    if args.va or args.peer:
        if not args.va or not args.peer:
            raise ValueError("Supply both a representative and --peer")
        same, changes = delta(_va(args.va), _va(args.peer))
        print(f"Same instruction shape: {same}; verify all differences against original evidence.")
        print(changes or "No operand differences; inspect each function's ABI, callers and dependencies.")
        return 0
    rows = groups(args.min_members, args.module, args.include_held)[:args.n]
    if args.json:
        print(json.dumps(rows, indent=2))
    else:
        for row in rows:
            ids = " ".join(f"fn_{v:08x}" for v in row["pending"][:8])
            print(f"{row['family'][:12]} {len(row['pending'])} pending {row['pending_bytes']}B "
                  f"representative=fn_{row['representative']:08x} accepted={row['accepted_representative']} {ids}")
        print("Shape matches are candidates only. Use --json for all member IDs; each needs its own gate.")
    return 0
