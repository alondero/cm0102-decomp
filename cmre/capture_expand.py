"""Offline work on stored captures (ADR 0009): expansion, selection, hints.

Tooling, not harness. Nothing here needs the game process and nothing here
decides a test result: expansion and selection run the original side only, and
`apply` ends by calling the unchanged lifted gate.
"""
from __future__ import annotations

import json
import random

from . import capture, env

PAGE = capture.PAGE
MUTATION_VALUES = (0, 1, 2, 0xFF, 0x7F, 0x80, 0xFFFF, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF)


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


def _mutate(rng: random.Random, machine, parent: dict) -> tuple[dict, list]:
    """One to three small edits of a parent input, at bytes a previous run read."""
    regs, stack = dict(parent["regs"]), list(parent["stack"])
    pages = {page: bytearray(blob) for page, blob in parent["pages"].items()}
    notes = []
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
    return {"regs": regs, "stack": stack, "pages": {p: bytes(b) for p, b in pages.items()}}, notes


def expand(va: int, *, iterations: int = 600, target: float = 0.9, limit: int = 64) -> dict:
    """Coverage-guided mutation of this function's captures, original side only.

    A mutant is kept only when the original executes an instruction of the
    function that no stored input reached. It is saved as an ordinary capture
    whose metadata names its parent and edits: a derived test input, not an
    observation of the game.
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
    rng = random.Random(va)
    added = 0
    for _ in range(iterations):
        if len(covered) / len(starts) >= target or added >= limit:
            break
        parent = rng.choice(corpus)
        mutant, notes = _mutate(rng, machine, parent)
        if not notes:
            continue
        result = _run(machine, va, opts, mutant["regs"], mutant["stack"], mutant["pages"],
                      parent["seed"], parent["fpcw"])
        fresh = (result.covered & starts) - covered
        if not fresh:
            continue
        meta = {"kind": "mutated", "parent": parent["name"], "edits": notes, "outcome": result.outcome,
                "budget": opts.budget, "arg_count": opts.arg_count}
        name = capture.save(va, mutant["regs"], parent["fpcw"], mutant["stack"], mutant["pages"],
                            parent["seed"], meta)
        covered |= fresh
        added += 1
        corpus.append({**mutant, "name": name, "seed": parent["seed"], "fpcw": parent["fpcw"],
                       "reads": _input_reads(result)})
    return {"va": f"{va:#010x}", "captures": len(names), "added": added,
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
