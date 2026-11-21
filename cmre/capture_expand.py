"""Offline work on stored captures (ADR 0009): expansion, selection, hints.

Tooling, not harness. Nothing here needs the game process and nothing here
decides a test result: expansion and selection run the original side only, and
`apply` ends by calling the unchanged lifted gate.
"""
from __future__ import annotations

import json
import random

from . import capture, emu, env

PAGE = capture.PAGE
MUTATION_VALUES = (0, 1, 2, 0xFF, 0x7F, 0x80, 0xFFFF, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF)
# Opt-in exploration of the seeded import-stub inputs (not observed API results).
SEED_MUTATION_PROBABILITY = 0.25
SEED_SCAFFOLDING = "seeded-import-stub-input (not observed)"


def _function(va: int):
    """Lifter's instruction starts and region: the same denominator the gate uses."""
    from . import lift
    lifted = lift.discover(va)
    return set(lifted.insns), (lifted.va, lifted.region_end)


def _machine(va: int, region):
    from . import difftest, emu, image
    opts = difftest.Options.for_function(va)
    return opts, emu.Machine(image.primary(), watch=region,
                             arena_size=opts.hints.get("arena_size", emu.ARENA_SIZE))


def _run(machine, va: int, opts, regs, stack, pages, seed, fpcw):
    from . import emu
    words = (list(stack) + [0] * opts.arg_count)[:opts.arg_count]
    case = emu.Case(dict(regs), words, bytes(machine.arena_size), {}, seed, fpcw, None, dict(pages))
    return machine.run(va, case, opts.budget)


def _input_reads(result) -> list[tuple[int, int]]:
    """(address, size) of bytes the run read before writing them, outside the test stack."""
    from . import emu
    written, found = set(), []
    for event in result.events:
        if event[0] == "c":
            continue
        kind, address, size, _ = event
        if emu.STACK <= address < emu.STACK + emu.STACK_SIZE:
            continue
        if kind == "w":
            written.update(range(address, address + size))
        elif size in (1, 2, 4) and not written.intersection(range(address, address + size)):
            found.append((address, size))
    return found


def _value(rng: random.Random, old: int, size: int) -> int:
    mask = (1 << (8 * size)) - 1
    choice = rng.randrange(6)
    if choice == 0:
        return rng.choice(MUTATION_VALUES) & mask
    if choice == 1:
        return (old + rng.choice((1, -1, 2, -2, 16, -16))) & mask
    if choice == 2:
        return old ^ (1 << rng.randrange(8 * size))
    if choice == 3:
        return rng.randrange(0, 64) & mask
    if choice == 4:
        return 0
    return rng.getrandbits(8 * size)


def _mutate(rng: random.Random, machine, parent: dict, *, mutate_seed: bool = False) -> tuple[dict, list]:
    """One to three small edits of a parent input, at bytes a previous run read.

    With `mutate_seed`, sometimes replace all edits with one mutation of the u32
    import seed. That seed only chooses seeded import-stub results: a scaffolding
    input, not an observation of a real API result or of captured memory.
    """
    regs, stack = dict(parent["regs"]), list(parent["stack"])
    pages = {page: bytearray(blob) for page, blob in parent["pages"].items()}
    seed = parent.get("seed", 0)
    notes = []
    if mutate_seed and rng.random() < SEED_MUTATION_PROBABILITY:
        old = seed & 0xFFFFFFFF
        seed = _value(rng, old, 4)
        if seed == old:
            seed ^= 1
        notes.append(["import_seed", old, seed])
    else:
        for _ in range(rng.choice((1, 1, 2, 3))):
            choice = rng.random()
            if choice < 0.15 and stack:
                index = rng.randrange(min(len(stack), 10))
                stack[index] = _value(rng, stack[index], 4)
                notes.append(["stack", index, stack[index]])
            elif choice < 0.25:
                name = rng.choice(("eax", "ecx", "edx", "ebx", "esi", "edi"))
                regs[name] = _value(rng, regs[name], 4)
                notes.append(["reg", name, regs[name]])
            elif parent["reads"]:
                address, size = rng.choice(parent["reads"])
                page, offset = address & ~(PAGE - 1), address & (PAGE - 1)
                if offset + size > PAGE:
                    continue
                if page not in pages:
                    if not any(lo <= page < hi for lo, hi in machine._writable_image):
                        continue  # code, constants and absent memory are not inputs
                    pages[page] = bytearray(machine.pristine[page])
                old = int.from_bytes(pages[page][offset:offset + size], "little")
                new = _value(rng, old, size)
                pages[page][offset:offset + size] = new.to_bytes(size, "little")
                notes.append(["mem", address, size, new])
    mutant = {"regs": regs, "stack": stack, "pages": {p: bytes(b) for p, b in pages.items()}, "seed": seed}
    return mutant, notes


# --- comparison-guided (input-to-state) mutation --------------------------------
# Random edits rarely hit the exact constant a `cmp` wants (a message id, an enum,
# a record type). Tracing each comparison's operand values on the original side
# and writing the other operand's value into the input byte that supplied this
# one reaches those branches directly. Still original-side input exploration
# only: the result is a derived capture, and the unchanged gate decides.
I2S_GP = ("eax", "ecx", "edx", "ebx", "esi", "edi")
I2S_MATCHES = 8          # input positions tried per traced operand value
I2S_SITE_VALUES = 8      # distinct operand pairs kept per comparison site
I2S_SWITCH_MAX = 64      # `cmp x, n` with n <= this also offers every value 0..n+1


def _disassembler():
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    return md


def _starts_from_bytes(va: int, code: bytes) -> set[int]:
    """Linear-sweep instruction starts of a code blob (synthetic tests, small tools)."""
    md = _disassembler()
    return {ins.address for ins in md.disasm(code, va)}


def _compare_sites(machine, starts) -> dict:
    """Decoded `cmp`/`test` instructions among the function's instruction starts."""
    md = _disassembler()
    sites = {}
    for address in sorted(starts):
        blob = b""
        for length in (16, 8, 4):
            try:
                blob = bytes(machine.uc.mem_read(address, length))
                break
            except Exception:
                continue
        ins = next(md.disasm(blob, address, 1), None)
        if ins is not None and ins.mnemonic in ("cmp", "test") and len(ins.operands) == 2:
            sites[address] = ins
    return sites


def _operand(uc, ins, op):
    """(kind, location, value, size) of one operand just before `ins` executes."""
    from capstone import x86_const as X
    from unicorn import x86_const as U
    size = op.size
    mask = (1 << (8 * size)) - 1
    if op.type == X.X86_OP_IMM:
        return ("imm", None, op.imm & mask, size)
    if op.type == X.X86_OP_REG:
        name = ins.reg_name(op.reg)
        return ("reg", name, uc.reg_read(getattr(U, f"UC_X86_REG_{name.upper()}")) & mask, size)
    if op.type == X.X86_OP_MEM:
        if op.mem.segment:
            return None
        address = op.mem.disp
        if op.mem.base:
            address += uc.reg_read(getattr(U, f"UC_X86_REG_{ins.reg_name(op.mem.base).upper()}"))
        if op.mem.index:
            address += op.mem.scale * uc.reg_read(getattr(U, f"UC_X86_REG_{ins.reg_name(op.mem.index).upper()}"))
        address &= 0xFFFFFFFF
        try:
            value = int.from_bytes(bytes(uc.mem_read(address, size)), "little")
        except Exception:
            return None
        return ("mem", address, value, size)
    return None


def _traced_run(machine, va: int, opts, entry: dict, sites: dict):
    """One original-side run of `entry` that also records each comparison's operands."""
    import unicorn
    observed, seen = [], {}

    def hook(uc, address, size, _):
        ins = sites.get(address)
        if ins is None:
            return
        ops = [_operand(uc, ins, op) for op in ins.operands]
        if None in ops:
            return
        key = tuple(o[2] for o in ops)
        bucket = seen.setdefault(address, set())
        if key in bucket or len(bucket) >= I2S_SITE_VALUES:
            return
        bucket.add(key)
        observed.append({"site": address, "mnemonic": ins.mnemonic, "ops": ops})

    handle = None
    if sites:
        handle = machine.uc.hook_add(unicorn.UC_HOOK_CODE, hook, begin=min(sites), end=max(sites))
    try:
        result = _run(machine, va, opts, entry["regs"], entry["stack"], entry["pages"],
                      entry.get("seed", 0), entry.get("fpcw"))
    finally:
        if handle is not None:
            machine.uc.hook_del(handle)
    return result, observed


def _editable_page(machine, parent: dict, page: int) -> bool:
    if page in parent["pages"]:
        return True
    if emu.STACK <= page < emu.STACK + emu.STACK_SIZE or emu.ARENA <= page < emu.ARENA + machine.arena_size:
        return False
    return any(lo <= page < hi for lo, hi in machine._writable_image)


def _read_input(machine, parent: dict, address: int, size: int) -> int | None:
    page, offset = address & ~(PAGE - 1), address & (PAGE - 1)
    if offset + size > PAGE or not _editable_page(machine, parent, page):
        return None
    blob = parent["pages"].get(page)
    if blob is None:
        blob = machine.pristine.get(page)
    if blob is None:
        return None
    return int.from_bytes(blob[offset:offset + size], "little")


def _stack_index(address: int, size: int, count: int) -> int | None:
    base = emu.STACK_TOP + 4
    if address % 4 or size != 4 or not base <= address < base + 4 * count:
        return None
    return (address - base) // 4


def _targets(observation: dict, side: int) -> list[int]:
    """Values that would change this comparison's outcome when written into operand `side`."""
    ops = observation["ops"]
    kind, _, value, size = ops[side]
    other = ops[1 - side]
    mask = (1 << (8 * size)) - 1
    if observation["mnemonic"] == "test":
        if other[0] == "imm" or other[1] != ops[side][1]:
            bits = other[2] & mask
            values = [value | bits, value & ~bits & mask]
        else:  # test r, r: zero versus non-zero
            values = [0, 1]
    else:
        wanted = other[2] & mask
        values = [wanted, (wanted + 1) & mask, (wanted - 1) & mask]
        if other[0] == "imm" and 0 < wanted <= I2S_SWITCH_MAX:
            values += list(range(wanted + 2))
    out = []
    for v in values:
        if v != value and v not in out:
            out.append(v)
    return out


def _positions(machine, parent: dict, side) -> list[tuple]:
    """Input positions that supplied this operand: exact for memory, by value for registers."""
    kind, location, value, size = side
    mask = (1 << (8 * size)) - 1
    if kind == "mem":
        index = _stack_index(location, size, len(parent["stack"]))
        if index is not None:
            return [("stack", index, size)]
        page = location & ~(PAGE - 1)
        if _editable_page(machine, parent, page) and _read_input(machine, parent, location, size) is not None:
            return [("mem", location, size)]
        if any(lo <= page < hi for lo, hi in machine._executable) or (
                page in machine.pristine and not _editable_page(machine, parent, page)
                and not emu.ARENA <= page < emu.ARENA + machine.arena_size):
            return []  # code, constants and harness memory are not inputs
    found = []
    for address, width in parent.get("reads", []):
        if width >= size:
            current = _read_input(machine, parent, address, width)
            if current is not None and current & mask == value:
                found.append(("mem", address, width))
    for index, word in enumerate(parent["stack"]):
        if word & mask == value:
            found.append(("stack", index, 4))
    for name in I2S_GP:
        if parent["regs"].get(name, 0) & mask == value:
            found.append(("reg", name, 4))
    return found[:I2S_MATCHES]


def _edit(machine, parent: dict, position, size: int, value: int):
    """Copy of `parent` with the low `size` bytes at `position` set to `value`, plus its note."""
    regs, stack = dict(parent["regs"]), list(parent["stack"])
    pages = dict(parent["pages"])
    kind, location, width = position
    mask = (1 << (8 * size)) - 1
    if kind == "stack":
        stack[location] = (stack[location] & ~mask | value) & 0xFFFFFFFF
        note = ["stack", location, stack[location]]
    elif kind == "reg":
        regs[location] = (regs.get(location, 0) & ~mask | value) & 0xFFFFFFFF
        note = ["reg", location, regs[location]]
    else:
        page, offset = location & ~(PAGE - 1), location & (PAGE - 1)
        blob = bytearray(pages.get(page) or machine.pristine[page])
        full = (1 << (8 * width)) - 1
        old = int.from_bytes(blob[offset:offset + width], "little")
        new = (old & ~mask | value) & full
        blob[offset:offset + width] = new.to_bytes(width, "little")
        pages[page] = bytes(blob)
        note = ["mem", location, width, new]
    return {"regs": regs, "stack": stack, "pages": pages, "seed": parent.get("seed", 0)}, [note]


def _i2s_mutants(machine, parent: dict, observations: list) -> list[tuple[dict, list]]:
    """Deterministic single edits that make traced comparisons go the other way."""
    out, seen = [], set()
    for observation in observations:
        for side in (0, 1):
            operand = observation["ops"][side]
            if operand[0] == "imm":
                continue
            values = _targets(observation, side)
            if not values:
                continue
            for position in _positions(machine, parent, operand):
                for value in values:
                    mutant, notes = _edit(machine, parent, position, operand[3], value)
                    key = repr(notes)
                    if key in seen:
                        continue
                    seen.add(key)
                    out.append((mutant, notes))
    return out


def expand(va: int, *, iterations: int = 600, target: float = 0.9, limit: int = 64,
           mutate_import_seed: bool = False, compare: bool = True) -> dict:
    """Coverage-guided mutation of this function's captures, original side only.

    A mutant is kept only when the original executes an instruction of the
    function that no stored input reached. It is saved as an ordinary capture
    whose metadata names its parent and edits: a derived test input, not an
    observation of the game. `mutate_import_seed` is an opt-in exploration of
    the seeded import-stub inputs; it adds no evidence about real imports.
    `compare` (default) first solves traced comparisons (input-to-state), then
    spends the remaining iterations on random edits.
    """
    starts, region = _function(va)
    names = capture.names_for(va)
    if not names or not starts:
        return {"va": f"{va:#010x}", "captures": len(names), "coverage": 0.0, "added": 0}
    opts, machine = _machine(va, region)
    corpus, covered = [], set()
    for name in names:
        found = capture.load(va, name)
        entry = {"name": name, "regs": found.regs, "stack": found.stack, "pages": found.pages,
                 "seed": found.import_seed, "fpcw": found.fpcw}
        result = _run(machine, va, opts, entry["regs"], entry["stack"], entry["pages"], entry["seed"], entry["fpcw"])
        entry["reads"] = _input_reads(result)
        covered |= result.covered & starts
        corpus.append(entry)
    before = len(covered) / len(starts)
    added, spent = 0, 0
    if compare:
        sites = _compare_sites(machine, starts)
        queue, tried = list(corpus), set()
        while queue and spent < iterations and len(covered) / len(starts) < target and added < limit:
            parent = queue.pop(0)
            _, observed = _traced_run(machine, va, opts, parent, sites)
            for mutant, notes in _i2s_mutants(machine, parent, observed):
                if spent >= iterations or added >= limit:
                    break
                key = (parent["name"], repr(notes))
                if key in tried:
                    continue
                tried.add(key)
                spent += 1
                result = _run(machine, va, opts, mutant["regs"], mutant["stack"], mutant["pages"],
                              mutant["seed"], parent["fpcw"])
                fresh = (result.covered & starts) - covered
                if not fresh:
                    continue
                meta = {"kind": "mutated", "parent": parent["name"], "edits": notes, "outcome": result.outcome,
                        "budget": opts.budget, "arg_count": opts.arg_count, "strategy": "compare"}
                name = capture.save(va, mutant["regs"], parent["fpcw"], mutant["stack"], mutant["pages"],
                                    mutant["seed"], meta)
                covered |= fresh
                added += 1
                entry = {**mutant, "name": name, "fpcw": parent["fpcw"], "reads": _input_reads(result)}
                corpus.append(entry)
                queue.append(entry)
    compared = added
    rng = random.Random(va)
    for _ in range(max(0, iterations - spent)):
        if len(covered) / len(starts) >= target or added >= limit:
            break
        parent = rng.choice(corpus)
        mutant, notes = _mutate(rng, machine, parent, mutate_seed=mutate_import_seed)
        if not notes:
            continue
        seed = mutant["seed"]
        result = _run(machine, va, opts, mutant["regs"], mutant["stack"], mutant["pages"],
                      seed, parent["fpcw"])
        fresh = (result.covered & starts) - covered
        if not fresh:
            continue
        meta = {"kind": "mutated", "parent": parent["name"], "edits": notes, "outcome": result.outcome,
                "budget": opts.budget, "arg_count": opts.arg_count}
        if seed != parent["seed"]:
            meta["scaffolding"] = SEED_SCAFFOLDING
        name = capture.save(va, mutant["regs"], parent["fpcw"], mutant["stack"], mutant["pages"],
                            seed, meta)
        covered |= fresh
        added += 1
        corpus.append({**mutant, "name": name, "fpcw": parent["fpcw"],
                       "reads": _input_reads(result)})
    return {"va": f"{va:#010x}", "captures": len(names), "added": added, "added_compare": compared,
            "coverage_before": round(before, 4), "coverage": round(len(covered) / len(starts), 4)}


def select(va: int, limit: int | None = None) -> list[str]:
    """Greedy cover: few stored inputs that keep every reached instruction; observed ones first."""
    from . import difftest
    limit = limit or difftest.MAX_CAPTURES
    starts, region = _function(va)
    opts, machine = _machine(va, region)
    reach, observed = {}, set()
    for name in capture.names_for(va):
        found = capture.load(va, name)
        result = _run(machine, va, opts, found.regs, found.stack, found.pages, found.import_seed, found.fpcw)
        reach[name] = result.covered & starts
        if found.meta.get("kind") != "mutated":
            observed.add(name)
    chosen, covered = [], set()
    while reach and len(chosen) < limit:
        best = max(sorted(reach), key=lambda n: (len(reach[n] - covered), n in observed))
        if chosen and not reach[best] - covered:
            break
        chosen.append(best)
        covered |= reach.pop(best)
    return sorted(chosen)


def apply(va: int, *, test: bool = True) -> dict:
    """List the selected captures in the function's hints, then run the unchanged lifted gate."""
    from . import difftest, recover
    path = env.HINTS_DIR / f"fn_{va:08x}.json"
    hints = json.loads(path.read_text(encoding="utf-8")) if path.exists() else {}
    names = select(va)
    if not names:
        return {"va": f"{va:#010x}", "status": "no-captures"}
    hints.pop("auto", None)  # a claimed hint: automatic inference no longer rewrites it
    hints["captures"] = names
    difftest.validate_hints(hints)
    temporary = path.with_suffix(".tmp")
    temporary.write_text(json.dumps(hints, indent=1) + "\n", encoding="utf-8")
    temporary.replace(path)
    out = {"va": f"{va:#010x}", "captures": len(names)}
    if test:
        report = recover.test_lifted(va)
        result = report.get("difftest", {})
        out.update(status=report["status"], coverage=result.get("coverage"), outcomes=result.get("outcomes"))
    return out


def stored() -> list[int]:
    """Functions that have at least one capture in the local store."""
    root = capture.store_dir()
    if not root.is_dir():
        return []
    return sorted(int(p.name[3:], 16) for p in root.iterdir()
                  if p.is_dir() and p.name.startswith("fn_") and any(p.glob("*.cap")))


def _worker(job):
    action, va, options = job
    try:
        return (expand if action == "expand" else apply)(va, **options)
    except Exception as exc:  # one function's failure must not hide the others
        return {"va": f"{va:#010x}", "status": "error", "error": f"{type(exc).__name__}: {exc}"}


def run_many(action: str, vas: list[int], jobs: int, **options) -> list[dict]:
    from concurrent.futures import ProcessPoolExecutor
    work = [(action, va, options) for va in vas]
    if jobs <= 1:
        return [_worker(item) for item in work]
    with ProcessPoolExecutor(max_workers=jobs) as pool:
        return list(pool.map(_worker, work, chunksize=1))
