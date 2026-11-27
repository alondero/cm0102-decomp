"""Replay one retained C gate mismatch with diagnostic-only observation hooks."""

from __future__ import annotations

from itertools import zip_longest
import json
from pathlib import Path
import random
import struct
import tempfile

import capstone
import unicorn

from . import acceptance_request, build, difftest, emu, env, image, lift, recover, semantic, x87_transport


WRITE_LIMIT = 16_384
TRANSFER_CONTEXT_LIMIT = 16


class Trace:
    """Attribute gate events to the executing instruction and record external calls."""

    def __init__(self, machine, owner, original_owner, original_text, entries, registers=(), *, abi_stack=False):
        self.machine = machine
        self.owner = owner
        self.original_owner = original_owner
        self.original_text = original_text
        self.entries = entries
        self.abi_stack = abi_stack
        self.decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        self.decoder.detail = True
        self.registers = {name: reg for name, reg in zip(emu.REG_NAMES, emu.REGS) if name in registers}
        self.clear()
        machine.uc.hook_add(unicorn.UC_HOOK_CODE, self._instruction)

    def clear(self):
        self.calls = []
        self.writes = []
        self.transfers = []
        self.site = None
        self.size = 0
        self.cursor = 0
        self.writes_truncated = False
        self.register_values = {}
        self.register_changes = {}

    def flush(self):
        events = self.machine.events
        for index in range(self.cursor, len(events)):
            kind, address, size, value = events[index]
            if kind == "c":
                self.calls.append((index, {"kind": "call", "site": self.site, "target": address,
                                           "args": value, "import_index": len(self.calls)}))
            elif kind == "w" and (self.abi_stack or not emu.STACK <= address < emu.STACK_TOP):
                if len(self.writes) < WRITE_LIMIT:
                    self.writes.append((index, {"kind": "write", "site": self.site, "address": address,
                                                "size": size, "value": value}))
                else:
                    self.writes_truncated = True
        self.cursor = len(events)
        for name, reg in self.registers.items():
            value = self.machine.uc.reg_read(reg)
            if name in self.register_values and value != self.register_values[name]:
                self.register_changes[name] = {"site": self.site, "value": value}
            self.register_values[name] = value

    def _instruction(self, uc, pc, size, _):
        self.flush()
        if (self.site is not None and self.owner[0] <= self.site < self.owner[1]
                and self.original_text[0] <= pc < self.original_text[1]
                and pc in self.entries and not self.original_owner[0] <= pc < self.original_owner[1]
                and len(self.transfers) < TRANSFER_CONTEXT_LIMIT):
            insn = next(self.decoder.disasm(bytes(uc.mem_read(self.site, self.size)), self.site, count=1), None)
            if insn and (insn.group(capstone.CS_GRP_CALL) or insn.group(capstone.CS_GRP_JUMP)):
                self.transfers.append({"site": self.site, "target": pc})
        self.site, self.size = pc, size

    def finish(self, result):
        self.flush()
        if result.outcome == "fault" and self.writes:
            (access, address, size), site = result.detail
            _, last = self.writes[-1]
            if (access in {unicorn.UC_MEM_WRITE_UNMAPPED, unicorn.UC_MEM_WRITE_PROT}
                    and last["site"] == site and last["address"] < address + size
                    and address < last["address"] + last["size"]):
                self.writes.pop()  # Unicorn observes attempts as well as completed stores.


def _changed_bytes(a, b, machine_a, machine_b, amap, profile):
    """Locate bytes that differ in the gate's normalized final memory."""
    changed = {}
    for page in set(a) | set(b):
        an = a.get(page)
        bn = b.get(page)
        if an is None and page in machine_a.pristine:
            an = machine_a._initial_page(page)
        if bn is None and page in machine_b.pristine:
            bn = machine_b._initial_page(page)
        if an is None or bn is None:
            continue
        if profile == "semantic":
            an, bn = semantic.canonical_blob(an), semantic.canonical_blob(bn)
        if difftest._equal_blob(an, bn, amap):
            continue
        offsets = {i for i, (left, right) in enumerate(zip(an, bn)) if left != right}
        if amap is not None and (amap.exact or amap.call_returns):
            # Retain the gate's equivalence for unaligned relocated dwords
            # when the page also contains genuinely differing bytes.
            for offset in sorted(offsets):
                if offset not in offsets:
                    continue
                for start in range(max(0, offset - 3), min(offset, len(an) - 4) + 1):
                    left = struct.unpack_from("<I", an, start)[0]
                    right = struct.unpack_from("<I", bn, start)[0]
                    if difftest._equal(left, right, amap):
                        offsets.difference_update(range(start, start + 4))
                        break
        changed.update((page + i, (an[i], bn[i])) for i in offsets)
    return changed


def _first_call(trace_a, trace_b, na, nb, amap):
    for index, (left, right) in enumerate(zip_longest(na[3], nb[3])):
        if left and right and left[:3] == right[:3] and all(
            difftest._equal(a, b, amap) for a, b in zip(left[3], right[3])
        ):
            continue
        return (trace_a.calls[index] if left else None, trace_b.calls[index] if right else None)
    return None


def _writes(trace, changed, side):
    """Find the last observed write to each disagreeing byte, independent of store width."""
    owners = {}
    for index, event in trace.writes:
        for offset in range(event["size"]):
            address = event["address"] + offset
            if address in changed:
                owners[address] = (index, {**event, "different_byte": address, "final_byte": changed[address][side]})
    return owners


def _first_write(trace_a, trace_b, changed):
    if not changed or trace_a.writes_truncated or trace_b.writes_truncated:
        return None
    original = _writes(trace_a, changed, 0)
    recovered = _writes(trace_b, changed, 1)
    if not original and not recovered:
        return None
    # The original provides execution order when it wrote a disagreeing byte.
    first = original or recovered
    address = min(first, key=lambda addr: (first[addr][0], addr))
    return original.get(address), recovered.get(address)


def _earlier(call, write):
    if call is None:
        return write
    if write is None:
        return call
    # With different code the two observations can cross. In that case
    # use the matched import ordinal and retain both witnesses.
    common = [(c[0], w[0]) for c, w in zip(call, write) if c and w]
    return write if common and all(w < c for c, w in common) else call


def _point(pair):
    left, right = (point[1] if point else None for point in pair)
    return {"kind": (left or right)["kind"], "original": left, "recovered": right}


def _terminal(result, view, trace, profile, returns):
    kind = "return" if result.outcome == "return" else "stop"
    site = trace.site
    if result.outcome in {"fault", "error", "budget"}:
        site = result.detail[-1]
    if result.outcome in semantic.INDIRECT_STOPS:
        kind, site = "call", result.detail[1]
    names = _register_names(profile, returns)
    return {"kind": kind, "site": site, "outcome": result.outcome, "detail": view[1],
            "registers": dict(zip(names, view[2])),
            "last_register_changes": trace.register_changes}


def _register_names(profile, returns):
    if profile == "abi":
        return difftest.ABI_REGS
    selected = difftest.SEMANTIC_REGS[returns] + difftest.CALLEE_SAVED
    return tuple(name for name in emu.REG_NAMES if name in selected) + ("fpcw", "fp_depth", "st0")


def _replay(va, lifted, built, opts, profile, returns, mismatch):
    original_image = image.primary()
    region = lifted.va, lifted.region_end
    c_text = built.image.text
    c_region = c_text.va, c_text.end
    original_text = original_image.text.va, original_image.text.end
    arena_size = opts.hints.get("arena_size", emu.ARENA_SIZE)
    original = emu.Machine(original_image, watch=region, arena_size=arena_size)
    recovered = emu.Machine(original_image, built.image, watch=region, escape=True, arena_size=arena_size)
    if profile == "semantic":
        difftest._stop_indirect(original, recovered, region, built, recover._indirect_calls(lifted))

    rng = random.Random(opts.seed)
    templates = difftest.arena_templates()
    globals_seen = set()
    index = mismatch["case"]
    for prior in range(index + 1):
        case = difftest.make_case(rng, opts, sorted(globals_seen), templates)
        own = difftest._discovery(original, opts, prior, region)
        if prior == index:
            break
        result = original.run(va, case, opts.budget)
        if prior < opts.discovery:
            globals_seen |= difftest._global_reads(result, original_image, own)

    entries = difftest._entries()
    registers = ()
    if mismatch.get("field") == "regs":
        names = _register_names(profile, returns)
        registers = tuple(name for name, a, b in zip(names, mismatch["original"], mismatch["recovered"]) if a != b)
    abi_stack = profile == "abi"
    trace_a = Trace(original, region, region, original_text, entries, registers, abi_stack=abi_stack)
    trace_b = Trace(recovered, c_region, region, original_text, entries, registers, abi_stack=abi_stack)
    original.track_dead_stack_reads = mismatch.get("kind") == "stack-layout-excess"
    a = original.run(va, case, opts.budget)
    trace_a.finish(a)
    if mismatch.get("kind") == "budget-asymmetry":
        b = recovered.run(built.entry, case, mismatch.get("recovered_budget", opts.budget))
    else:
        b = recovered.run(built.entry, case, opts.budget)
        if b.outcome == "budget" and a.outcome != "budget":
            trace_b.clear()
            b = recovered.run(built.entry, case, opts.budget * difftest.RECOVERED_BUDGET_FACTOR)
    trace_b.finish(b)
    amap = emu.AddressMap(built.symbols, {}, *c_region)
    fp_a = difftest.semantic_fp(original.uc, returns) if profile == "semantic" else ()
    fp_b = difftest.semantic_fp(recovered.uc, returns) if profile == "semantic" else ()
    na = difftest._normalize(a, None, profile, returns, fp_a)
    nb = difftest._normalize(b, amap, profile, returns, fp_b, difftest.stop_shape(a))

    if mismatch.get("kind") == "budget-asymmetry":
        actual = {"original": a.outcome, "recovered": b.outcome}
    else:
        _, actual = difftest._classify(na, nb, a, amap, region, recover._data_spans(lifted),
                                       fault_mapping=b.fault_mapping, profile=profile)
        if built.image.x87_bindings:
            actual = x87_transport.difference(x87_transport.snapshot(original.uc),
                                               x87_transport.snapshot(recovered.uc)) or actual
    expected = {k: v for k, v in mismatch.items() if k not in {"case", "kind", "note", "recovered_budget"}}
    if (mismatch.get("kind") == "stack-layout-excess"
            and not difftest._reads_dead_stack_before(a, expected, profile)):
        raise ValueError(f"case {index} did not reproduce the gate's frame-layout evidence")
    if json.loads(json.dumps(actual)) != expected:
        raise ValueError(f"case {index} did not reproduce the gate mismatch: {actual!r}")

    changed = _changed_bytes(na[4], nb[4], original, recovered, amap, profile)
    # The gate masks compiler stack layout. Keep caller-owned stack changes as
    # context, without comparing private storage or compiler-generated spill slots.
    caller_page = emu.STACK_TOP
    caller_bytes = _changed_bytes({p: v for p, v in a.memory.items() if p == caller_page},
                                  {p: v for p, v in b.memory.items() if p == caller_page},
                                  original, recovered, amap, profile)
    terminal_a = _terminal(a, na, trace_a, profile, returns)
    terminal_b = _terminal(b, nb, trace_b, profile, returns)
    first_call = _first_call(trace_a, trace_b, na, nb, amap)
    first_write = _first_write(trace_a, trace_b, changed)
    # A call missing from the side that ran out of instructions is unknown.
    # An observed call missing from a completed return is a concrete discrepancy.
    observed_call = first_call and all(
        point or result.outcome == "return" for point, result in zip(first_call, (a, b))
    )
    budget = mismatch.get("kind") == "budget-asymmetry"
    first = (first_call if observed_call else None) if budget else _earlier(first_call, first_write)
    if first:
        divergence = _point(first)
    else:
        kind = "call" if "call" in {terminal_a["kind"], terminal_b["kind"]} else (
            "return" if a.outcome == b.outcome == "return" else "stop")
        divergence = {"kind": kind, "original": terminal_a, "recovered": terminal_b}
    if budget and not observed_call:
        classification = "harness-boundary"
        reason = "The instruction budget stopped one execution before a comparable divergence was established."
    elif (mismatch.get("kind") == "stack-layout-excess" or mismatch.get("field") == "import-transcript"
          or any(r.outcome in {"escape", "import", "error"} for r in (a, b))):
        classification = "harness-boundary"
        reason = "The reproduced mismatch involves frame layout, import modeling, or execution transport."
    else:
        classification = "candidate-source-defect"
        reason = "The replay reproduced an observable difference under the gate profile."
    caller_write = _first_write(trace_a, trace_b, caller_bytes)
    return {"classification": classification, "reason": reason, "divergence": divergence,
            "original_stop": terminal_a, "recovered_stop": terminal_b,
            "caller_stack_context": _point(caller_write) if caller_write else None,
            "first_import_difference": _point(first_call) if first_call else None,
            "first_final_memory_write": _point(first_write) if first_write else None,
            "original_external_transfers": trace_a.transfers, "recovered_external_transfers": trace_b.transfers,
            "write_trace_truncated": trace_a.writes_truncated or trace_b.writes_truncated}


def _report_path(va, source):
    candidates = (env.RUNS / f"fn_{va:08x}").glob("c-*/report.json")
    sha = env.digest(source)
    for path in sorted(candidates, key=lambda p: p.stat().st_mtime_ns, reverse=True):
        gate = json.loads(path.read_bytes())
        if gate.get("source_sha256") == sha and gate.get("status") == "fail":
            return path
    raise ValueError("No retained failing C gate for this source; supply --report and its matching --source.")


def run(va: int, report: Path | None = None, *, source: Path | None = None, opt: str | None = None) -> dict:
    if report is None:
        source = source or env.C_DIR / f"fn_{va:08x}.c"
        report = _report_path(va, source)
    raw = report.read_bytes()
    gate = json.loads(raw)
    if gate.get("tier") != "c" or gate.get("va") != f"{va:#010x}":
        raise ValueError("Expected a retained C gate report for this function.")
    source = (source or env.ROOT / gate["source"]).resolve()
    if env.digest(source) != gate["source_sha256"]:
        raise ValueError("Candidate source SHA-256 differs from the retained C gate.")
    if gate["execution_identity"] != recover.execution_identity():
        raise ValueError("Local execution identity differs from the retained C gate.")

    builds = gate["builds"]
    if opt is None:
        available = [key for key in reversed(recover.C_OPT_LEVELS) if "difftest" in builds.get(key, {})]
        opt = next((key for key in available if builds[key]["difftest"]["mismatches"]),
                   available[0] if available else None)
    if opt not in recover.C_OPT_LEVELS or "difftest" not in builds.get(opt, {}):
        raise ValueError("The selected optimization has no differential run.")
    diff = builds[opt]["difftest"]
    opts = difftest.Options.for_function(va, {key: diff[key] for key in ("cases", "budget")}, tier="c")
    opts.inputs = diff.get("inputs", "v1")
    if acceptance_request.digest_json(vars(opts)) != gate["effective_options_sha256"]:
        raise ValueError("Effective C options differ from the retained C gate; restore its hint snapshot.")

    case = min((row for row in diff["mismatches"] if "field" in row or row.get("kind") == "budget-asymmetry"),
               key=lambda row: row["case"], default=None)
    out = {
        "schema": "cmre-localize/1", "function": f"fn_{va:08x}",
        "target_sha256": env.TARGET_SHA256, "original_addresses": env.ADDRESS_CONVENTION,
        "recovered_addresses": f"linked image VA, image base {env.LINK_BASE:#010x}",
        "gate_report": str(report.resolve()), "gate_sha256": env.digest_bytes(raw),
        "source": str(source), "source_sha256": gate["source_sha256"],
        "harness_sha256": gate["harness_sha256"], "effective_options_sha256": gate["effective_options_sha256"],
        "optimization": opt, "profile": gate["profile"], "coverage": diff["coverage"], "gate_mismatch": case,
    }
    if case is None:
        input_gap = (not gate["problems"] and diff["coverage"] < opts.min_coverage
                     and diff["inconclusive"] < opts.cases / 2
                     and diff["code_as_data_cases"] <= difftest.CODE_AS_DATA_LIMIT * opts.cases)
        return {**out, "classification": "input-gap" if input_gap else None, "divergence": None,
                "reason": "The corpus missed the coverage threshold." if input_gap else
                "The selected differential run has no retained mismatching case."}

    lifted = lift.discover(va)
    with tempfile.TemporaryDirectory(prefix="cmre-localize-") as tmp:
        built = build.build(source, lift.symbol(va), Path(tmp), opt)
        result = _replay(va, lifted, built, opts, gate["profile"], gate.get("returns", "int"), case)
    if env.digest(source) != gate["source_sha256"]:
        raise ValueError("Candidate source changed during replay.")
    return {**out, **result}


def command(args) -> int:
    try:
        result = run(args.va, args.report, source=args.source, opt=args.opt)
    except (OSError, build.BuildError) as exc:
        raise ValueError(str(exc)) from exc
    scratch = env.ANALYSIS / "cmre/scratch"
    scratch.mkdir(parents=True, exist_ok=True)
    path = Path(tempfile.mkdtemp(dir=scratch, prefix=f"localize-{result['function']}-")) / "report.json"
    path.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(f"{result['classification'] or 'unclassified'}: {path}")
    return 0


def add_commands(sub, parse_va) -> None:
    parser = sub.add_parser("localize", help="diagnose the first observable divergence in a retained C gate")
    parser.add_argument("va", type=parse_va, metavar="fn_ID")
    parser.add_argument("--report", type=Path, help="retained C gate report JSON")
    parser.add_argument("--source", type=Path, help="exact candidate source from the gate")
    parser.add_argument("--opt", choices=recover.C_OPT_LEVELS, help="build to replay, for example --opt=-O0")
    parser.set_defaults(func=command)
