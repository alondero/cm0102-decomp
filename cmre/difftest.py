"""Differential test: original function vs recovered build on identical cases.

Profiles
- exact: lifted assembly. Registers, EFLAGS, ordered memory accesses (address,
  size, value), final dirty memory and stop point must all match after mapping
  recovered addresses back to original VAs.
- abi: tier-2 C accepted before ADR 0002. Callee-saved registers, ESP, EAX/EDX,
  outcome and final memory outside the dead stack must match; ordered accesses are
  not compared because a compiler may legitimately reorder stack traffic. Budget
  stops on both sides are inconclusive; asymmetric exhaustion blocks acceptance.
- semantic: tier-2 C default (ADR 0002). Return value, callee-saved registers,
  ESP, FPU control word/depth, non-stack memory, import calls and a fault's data
  access; never register allocation, the faulting instruction or stack layout.

Every hinted input fixture and capture runs once first, as case number cases + i.
The random cases follow, derived from Random(seed) only, where seed defaults to the
function VA, so a run is reproducible bit for bit. Optional hints
(recovery/hints/fn_<va>.json) shape inputs; they never change what is compared.
"""
from __future__ import annotations

import functools
import hashlib
import json
import random
import struct
from dataclasses import dataclass
from pathlib import Path

import unicorn

import zlib

from . import emu, env, image, inventory, return_transport, semantic, x87_transport

POOL_FIXED = (0, 1, 2, 3, 4, 7, 8, 0x10, 0x20, 0x7F, 0x80, 0xFF, 0x100, 0x7FFF, 0x8000, 0xFFFF,
              0x10000, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 0xFFFFFFFE, 0xFFFF8000)
DEFAULTS = {"cases": 600, "budget": 200_000, "discovery": 24, "arg_count": 10, "min_coverage": 0.6}
POINTER_TAIL_RESERVE = 0x400
# Input policies. v1: lifted records and C accepted before v2. v2 (new C): entry
# EFLAGS keep DF clear, as the Win32 ABI guarantees at a call, and only writable
# globals read by the function's own instructions are randomized; callee-internal
# state such as .data string literals and C runtime variables keeps its image bytes.
INPUT_POLICIES = ("v1", "v2")
EFLAGS_DF = 0x400


@dataclass
class Options:
    cases: int
    budget: int
    discovery: int
    arg_count: int
    min_coverage: float
    seed: int
    hints: dict
    inputs: str = "v1"  # set by the caller from the record's policy, never by hints
    va: int = 0  # names the function whose local captures the hints may list

    @classmethod
    def for_function(cls, va: int, overrides: dict | None = None, *, tier: str = "lifted") -> "Options":
        if tier not in ("lifted", "c"):
            raise ValueError(f"Unknown hint tier: {tier}")
        hints = load_hints(va)
        c_hints = hints.pop("c_hints", {})
        if tier == "c":
            hints.update(c_hints)
        validate_hints(overrides or {})
        merged = {**DEFAULTS, **{k: hints[k] for k in DEFAULTS if k in hints}, **(overrides or {})}
        return cls(seed=hints.get("seed", va), hints=hints, va=va, **merged)


def load_hints(va: int) -> dict:
    path = env.HINTS_DIR / f"fn_{va:08x}.json"
    hints = json.loads(path.read_text(encoding="utf-8")) if path.exists() else {}
    validate_hints(hints)
    return hints


def validate_hints(hints: dict, *, worker: bool = False) -> None:
    """Validate inputs before execution; task workers cannot change test policy."""
    metadata = {"notes", "fixture_provenance", "input_probe", "input_probe_sha256", "fixture_evidence"}
    policy = set(DEFAULTS) | {"seed"}
    input_fields = set(emu.REG_NAMES[:8]) | metadata | {"args", "arg_count", "struct_size", "globals", "fixtures", "arena_size", "import_transcript", "captures"}
    allowed = policy | input_fields | {"auto", "lift_jump_tables", "original_x87", "lift_original_calls", "c_hints"}
    if not isinstance(hints, dict) or set(hints) - allowed:
        raise ValueError("hints must be an object containing only supported fields")
    if worker and set(hints) & (policy - {"arg_count"}):
        raise ValueError("worker hints cannot change acceptance policy")
    if 'lift_jump_tables' in hints:
        if worker:
            raise ValueError('worker hints cannot change lift bindings')
        from .lift import validate_table_hints
        validate_table_hints(hints['lift_jump_tables'])
    if 'original_x87' in hints:
        if worker:
            raise ValueError('worker hints cannot change original x87 bindings')
        from .x87_transport import validate_declarations
        validate_declarations(hints['original_x87'])
    if 'lift_original_calls' in hints:
        if worker:
            raise ValueError('worker hints cannot change original CALL bindings')
        from .build import validate_lift_calls
        validate_lift_calls(hints['lift_original_calls'])
    for key in policy | {"struct_size"}:
        if key not in hints:
            continue
        value = hints[key]
        if key == "min_coverage":
            valid = type(value) in (int, float) and 0 < value <= 1
        else:
            lower = 0 if key in {"discovery", "arg_count", "struct_size", "seed"} else 1
            upper = {"struct_size": hints.get("arena_size", emu.ARENA_SIZE), "arg_count": 1024, "seed": 0xFFFFFFFF}.get(key, 1_000_000_000)
            valid = type(value) is int and lower <= value <= upper
        if not valid:
            raise ValueError(f"invalid hint {key}: {value!r}")
    emu.arena_capacity(hints.get('arena_size', emu.ARENA_SIZE))
    if 'import_transcript' in hints:
        emu.imports.Transcript(hints['import_transcript'], emu.ARENA,
                               hints.get('arena_size', emu.ARENA_SIZE))
    kinds = {"any", "ptr", "small", "struct"}
    args = hints.get("args", [])
    if not isinstance(args, list) or any(type(v) is not str or v not in kinds for v in args):
        raise ValueError("args must be a list of supported input kinds")
    for key in emu.REG_NAMES[:8]:
        if key in hints and (type(hints[key]) is not str or hints[key] not in kinds):
            raise ValueError(f"invalid register input kind: {key}")
    if "auto" in hints and type(hints["auto"]) is not bool:
        raise ValueError("auto must be boolean")
    for key in metadata:
        if key in hints and not isinstance(hints[key], dict if key == "fixture_provenance" else str):
            raise ValueError(f"invalid hint metadata: {key}")
    globals_in = hints.get("globals", {})
    if not isinstance(globals_in, dict):
        raise ValueError("globals must be an object")
    for address, values in globals_in.items():
        try:
            valid = isinstance(address, str) and 0 <= int(address, 16) <= 0xFFFFFFFF
        except ValueError:
            valid = False
        if not valid or not isinstance(values, list) or not values or any(type(v) is not int or not 0 <= v <= 0xFFFFFFFF for v in values):
            raise ValueError("globals require VA keys and nonempty unsigned dword lists")
    fixtures = hints.get("fixtures", [])
    if not isinstance(fixtures, list):
        raise ValueError("fixtures must be a list")
    if fixtures:
        templates = (bytes(hints.get("arena_size", emu.ARENA_SIZE)),) * ARENA_TEMPLATES
        for fixture in fixtures:
            _fixture_case(fixture, templates, hints.get("arg_count", DEFAULTS["arg_count"]))
    captures = hints.get("captures", [])
    if (not isinstance(captures, list) or len(captures) > MAX_CAPTURES or len(set(captures)) != len(captures)
            or any(not valid_capture_name(name) for name in captures)):
        raise ValueError(f"captures must be at most {MAX_CAPTURES} distinct capture names (SHA-256 hex)")
    if "c_hints" in hints:
        c_hints = hints["c_hints"]
        if not isinstance(c_hints, dict) or set(c_hints) - input_fields:
            raise ValueError("c_hints can override input shapes and metadata only, not acceptance policy or bindings")
        # Validate the effective C inputs, including inherited arg_count. Bindings
        # and thresholds stay shared; a nested override cannot bypass worker rules.
        validate_hints({**{k: v for k, v in hints.items() if k != "c_hints"}, **c_hints}, worker=worker)


def _pointer(rng: random.Random) -> int:
    """Arena pointer, biased to the start: game objects are large (field offsets
    near 64 KiB), so `this`-style pointers need most of the arena after them."""
    if rng.random() < 0.6:
        return emu.ARENA + 4 * rng.randrange(0, 64)
    return emu.ARENA + 4 * rng.randrange(0, (emu.ARENA_SIZE - POINTER_TAIL_RESERVE) // 4)


def _value(rng: random.Random, kind: str = "any", struct_size: int = 0, arena_size: int = emu.ARENA_SIZE) -> int:
    if kind == "struct":
        return _struct_pointer(rng, struct_size, arena_size)
    if kind == "ptr" or (kind == "any" and rng.random() < 0.3):
        return _pointer(rng)
    if kind == "small" or (kind == "any" and rng.random() < 0.4):
        return rng.choice(POOL_FIXED) if rng.random() < 0.5 else rng.randrange(0, 300)
    return rng.getrandbits(32)


def _struct_pointer(rng: random.Random, size: int, arena_size: int = emu.ARENA_SIZE) -> int:
    """A pointer such that the entire [base, base+size) range sits inside the test arena."""
    if not 0 <= size <= arena_size:
        raise ValueError(f"struct size {size} does not fit inside the test arena")
    max_offset = min(arena_size - size, arena_size - 1)
    return emu.ARENA + 4 * rng.randrange(max_offset // 4 + 1)


def _arena(rng: random.Random) -> bytes:
    out = bytearray()
    for _ in range(emu.ARENA_SIZE // 4):
        r = rng.random()
        if r < 0.35:
            v = _pointer(rng)
        elif r < 0.7:
            v = rng.choice(POOL_FIXED) if r < 0.5 else rng.randrange(0, 64)
        else:
            v = rng.getrandbits(32)
        out += struct.pack("<I", v)
    return bytes(out)


ARENA_TEMPLATES = 8
ARENA_MUTATIONS = 64


TEMPLATE_SEED = 0x0C0FFEE


@functools.cache
def arena_templates() -> tuple[bytes, ...]:
    """Full random arenas, generated once per process from a fixed seed; each case
    mutates one with its own seeded RNG (generating 256 KiB per case is slow)."""
    rng = random.Random(TEMPLATE_SEED)
    return tuple(_arena(rng) for _ in range(ARENA_TEMPLATES))


def _mutated_arena(rng: random.Random, templates: list[bytes]) -> bytes:
    arena = bytearray(rng.choice(templates))
    for _ in range(ARENA_MUTATIONS):
        struct.pack_into("<I", arena, 4 * rng.randrange(emu.ARENA_SIZE // 4), _value(rng))
    return bytes(arena)


def _fixture_case(fixture: dict, templates: list[bytes], arg_count: int) -> emu.Case:
    """Replay captured input bytes, never an execution result or callee stub."""
    def u32(value):
        if type(value) is not int or not 0 <= value <= 0xFFFFFFFF:
            raise ValueError("fixture values must be unsigned dwords")
        return value

    try:
        required = {"regs", "stack", "arena_template", "arena_writes", "globals", "import_seed"}
        if not isinstance(fixture, dict) or not required.issubset(fixture) or set(fixture) - required - {"fpcw", "import_transcript"}:
            raise ValueError("fixture fields differ from the input fixture schema")
        fpcw = fixture.get("fpcw")
        if "fpcw" in fixture and (type(fpcw) is not int or not 0 <= fpcw <= 0xFFFF):
            raise ValueError("fixture FPCW must be an unsigned word")
        regs = {name: u32(value) for name, value in fixture["regs"].items()}
        if set(regs) != set(emu.REG_NAMES):
            raise ValueError("fixture must specify every initial register")
        stack = [u32(value) for value in fixture["stack"]]
        if len(stack) != arg_count:
            raise ValueError("fixture stack length differs from arg_count")
        template = fixture["arena_template"]
        if type(template) is not int or not 0 <= template < len(templates):
            raise ValueError("fixture arena template is out of range")
        arena = bytearray(templates[template])
        for key, value in fixture["arena_writes"].items():
            offset = int(key, 16)
            if offset % 4 or not 0 <= offset <= len(arena) - 4:
                raise ValueError("fixture arena write must be an aligned in-range dword")
            struct.pack_into("<I", arena, offset, u32(value))
        globals_in = {}
        for key, value in fixture["globals"].items():
            address = int(key, 16)
            section = image.primary().section_at(address)
            if section is None or not section.writable or address + 4 > section.end:
                raise ValueError("fixture globals must fit inside writable image sections")
            globals_in[address] = u32(value)
        transcript = fixture.get('import_transcript')
        if transcript is not None:
            emu.imports.Transcript(transcript, emu.ARENA, len(arena))
        return emu.Case(regs, stack, bytes(arena), globals_in, u32(fixture["import_seed"]), fpcw, transcript)
    except (KeyError, TypeError, AttributeError, ValueError, struct.error) as exc:
        raise ValueError(f"invalid input fixture: {exc}") from exc


# --- Captured entry inputs (ADR 0009) -------------------------------------------------------
# A capture is an input, never a result: the registers, stack words and memory
# pages observed in the unmodified pinned process when one function was entered.
# The store lives in this harness module so the harness identity covers the
# bytes a test reads. Captures hold original game memory: they stay under
# local/captures/, are never published, and are named by the SHA-256 of their
# canonical bytes. A missing capture makes a test unavailable, never passing.

CAPTURE_FORMAT = "cmre-capture/1"
CAPTURE_MAGIC = b"CMCAP1\n"
CAPTURE_STACK_WORDS = 64
CAPTURE_REG_NAMES = ("eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "eflags")


def capture_store() -> Path:
    return env.LOCAL_CAPTURES


@dataclass(frozen=True)
class Capture:
    name: str  # sha256 of the canonical bytes
    va: int
    regs: dict[str, int]
    fpcw: int
    stack: tuple[int, ...]
    pages: dict[int, bytes]
    import_seed: int
    meta: dict


def _capture_canonical(header: dict, pages: dict[int, bytes]) -> bytes:
    vas = sorted(pages)
    if any(type(va) is not int or va % emu.PAGE or len(pages[va]) != emu.PAGE for va in vas):
        raise ValueError("capture pages must be whole 4 KiB pages")
    head = json.dumps({**header, "format": CAPTURE_FORMAT, "page_vas": vas}, sort_keys=True,
                      separators=(",", ":")).encode()
    return CAPTURE_MAGIC + struct.pack("<I", len(head)) + head + b"".join(pages[va] for va in vas)


def capture_path(va: int, name: str) -> Path:
    return capture_store() / f"fn_{va:08x}" / f"{name}.cap"


def save_capture(va: int, regs: dict[str, int], fpcw: int, stack: list[int], pages: dict[int, bytes],
         import_seed: int, meta: dict) -> str:
    header = {"target": env.TARGET_SHA256, "va": va, "regs": {n: regs[n] for n in CAPTURE_REG_NAMES},
              "fpcw": fpcw, "stack": list(stack), "import_seed": import_seed, "meta": meta}
    blob = _capture_canonical(header, pages)
    name = env.digest_bytes(blob)
    path = capture_path(va, name)
    if not path.exists():
        path.parent.mkdir(parents=True, exist_ok=True)
        temporary = path.with_suffix(".tmp")
        temporary.write_bytes(zlib.compress(blob, 6))
        temporary.replace(path)
    return name


def valid_capture_name(name) -> bool:
    return type(name) is str and len(name) == 64 and all(c in "0123456789abcdef" for c in name)


def load_capture(va: int, name: str) -> Capture:
    """Load and verify one capture. Raises when it is absent or differs from its name."""
    if not valid_capture_name(name):
        raise ValueError("capture names are lowercase SHA-256 hex digests")
    path = capture_path(va, name)
    if not path.is_file():
        raise FileNotFoundError(f"capture {name[:12]} for fn_{va:08x} is not in the local capture store")
    blob = zlib.decompress(path.read_bytes())
    if env.digest_bytes(blob) != name or not blob.startswith(CAPTURE_MAGIC):
        raise ValueError(f"capture {name[:12]} differs from its recorded identity")
    size, = struct.unpack_from("<I", blob, len(CAPTURE_MAGIC))
    start = len(CAPTURE_MAGIC) + 4
    header = json.loads(blob[start:start + size])
    vas = header["page_vas"]
    body = blob[start + size:]
    if (header.get("format") != CAPTURE_FORMAT or header.get("target") != env.TARGET_SHA256
            or header.get("va") != va or len(body) != emu.PAGE * len(vas)):
        raise ValueError(f"capture {name[:12]} does not belong to fn_{va:08x} of the pinned target")
    pages = {page: body[i * emu.PAGE:(i + 1) * emu.PAGE] for i, page in enumerate(vas)}
    return Capture(name, va, header["regs"], header["fpcw"], tuple(header["stack"]), pages,
                   header["import_seed"], header["meta"])


def capture_names(va: int) -> list[str]:
    folder = capture_store() / f"fn_{va:08x}"
    return sorted(p.stem for p in folder.glob("*.cap")) if folder.is_dir() else []


MAX_CAPTURES = 96


@functools.lru_cache(maxsize=4096)
def _capture(va: int, name: str):
    return load_capture(va, name)


def _capture_case(va: int, name: str, arena_size: int, arg_count: int) -> emu.Case:
    """Replay one captured entry input (ADR 0009): observed registers, the first
    `arg_count` observed stack words, and the observed pages at their original
    addresses. The function's own frame uses the test stack as in every case."""
    try:
        found = _capture(va, name)
    except (OSError, ValueError, KeyError) as exc:
        raise ValueError(f"capture unavailable: {exc}") from exc
    stack = (list(found.stack) + [0] * arg_count)[:arg_count]
    return emu.Case(dict(found.regs), stack, bytes(arena_size), {}, found.import_seed, found.fpcw,
                    None, dict(found.pages))


def make_case(rng: random.Random, opts: Options, global_inputs: list[int], templates: list[bytes]) -> emu.Case:
    if opts.inputs not in INPUT_POLICIES:
        raise ValueError(f"unknown input policy {opts.inputs!r}")
    h = opts.hints
    kinds = h.get("args", [])
    struct_size = h.get("struct_size", 0)
    size = emu.arena_capacity(h.get("arena_size", emu.ARENA_SIZE))
    stack = [_value(rng, kinds[i] if i < len(kinds) else "any", struct_size, size) for i in range(opts.arg_count)]
    regs = {name: _value(rng, h.get(name, "any"), struct_size, size) for name in emu.REG_NAMES[:8]}
    regs["eflags"] = rng.choice((0x202, 0x246, 0x283, 0x602))
    chosen = {}
    if global_inputs and rng.random() < 0.6:
        for va in rng.sample(global_inputs, k=min(len(global_inputs), rng.randrange(1, 9))):
            chosen[va] = _value(rng)
    for va_text, values in sorted(h.get("globals", {}).items()):
        chosen[int(va_text, 16)] = rng.choice(values)
    case = emu.Case(regs, stack, _mutated_arena(rng, templates), chosen, rng.getrandbits(32))
    case.arena += bytes(size - len(case.arena))
    fixtures = h.get("fixtures", [])
    captures = h.get("captures", [])
    if (fixtures or captures) and rng.randrange(2) == 0:
        # Keep random/edge cases alongside original-side selected input fixtures.
        case = _hinted_case(opts, rng.choice(fixtures + captures), size,
                            [t + bytes(size - len(t)) for t in templates])
    return _entry_policy(case, opts)


def _hinted_case(opts: Options, chosen, size: int, padded: list[bytes]) -> emu.Case:
    """A hinted input fixture (dict) or local capture (name), without RNG draws."""
    if isinstance(chosen, str):
        return _capture_case(opts.va, chosen, size, opts.arg_count)
    return _fixture_case(chosen, padded, opts.arg_count)


def _entry_policy(case: emu.Case, opts: Options) -> emu.Case:
    if "import_transcript" in opts.hints:
        case.import_transcript = opts.hints["import_transcript"]
    if opts.inputs == "v2":
        # Apply the recorded entry policy to either input source, without new draws.
        case.regs["eflags"] &= ~EFLAGS_DF
    return case


def hinted_inputs(opts: Options) -> list:
    """Hinted input fixtures, then captures, in hint order: each is one gate case."""
    return list(opts.hints.get("fixtures", [])) + list(opts.hints.get("captures", []))


def gate_cases(opts: Options, global_inputs: set[int], templates: list[bytes]):
    """Yield (case number, case, discovering) in gate order.

    Every hinted input runs once first, as case number opts.cases + i, so a fixture's
    reach never depends on a random draw. Then the opts.cases random cases follow
    from Random(opts.seed) exactly as before: random case k keeps number k and its
    input, including its own random fixture draw. Discovery reads `global_inputs`
    live, so the caller adds the globals its discovery cases read."""
    if opts.inputs not in INPUT_POLICIES:
        raise ValueError(f"unknown input policy {opts.inputs!r}")
    size = emu.arena_capacity(opts.hints.get("arena_size", emu.ARENA_SIZE))
    padded = [t + bytes(size - len(t)) for t in templates]
    for i, chosen in enumerate(hinted_inputs(opts)):
        yield opts.cases + i, _entry_policy(_hinted_case(opts, chosen, size, padded), opts), False
    rng = random.Random(opts.seed)
    for index in range(opts.cases):
        yield index, make_case(rng, opts, sorted(global_inputs), templates), index < opts.discovery


def _global_reads(result: emu.Result, img: image.Image, region: tuple[int, int] | None = None) -> set[int]:
    """Writable-image dwords read before being written. With `region` (v2), only
    reads made by instructions inside it, per the run's read attribution."""
    written, found = set(), set()
    for index, (kind, addr, size, _) in enumerate(result.events):
        if kind == "c":
            continue
        s = img.section_at(addr)
        if s is None or not s.writable:
            continue
        a = addr & ~3
        if kind == "w":
            written.add(a)
        elif a not in written and (region is None or region[0] <= result.read_sites.get(index, -1) < region[1]):
            found.add(a)
    return found


def _discovery(machine: emu.Machine, opts: Options, index: int, region) -> tuple[int, int] | None:
    """Enable read attribution for a v2 discovery case; returns the region filter."""
    return _attribute(machine, opts, index < opts.discovery, region)


def _attribute(machine: emu.Machine, opts: Options, discovering: bool, region) -> tuple[int, int] | None:
    machine.attribute_reads = opts.inputs == "v2" and discovering
    return region if machine.attribute_reads else None


def _normalize(result: emu.Result, amap: emu.AddressMap | None, profile: str,
               returns: str = "int", fp: tuple = (), shape: semantic.IndirectCall | None = None):
    """Map execution locations; retain data addresses for paired comparison.

    Values are left untouched and compared with _equal. Raw data addresses compare
    before location mapping, so coincidental absolute pointers are not rewritten. `shape` is the
    original listing's argument shape for a recovered run's indirect stop."""
    if profile == "semantic":
        return _normalize_semantic(result, amap, returns, fp, shape)
    loc = amap.location if amap is not None else (lambda x: x)
    detail = result.detail
    if result.outcome == "fault":
        (access, address, size), eip = detail
        fault_loc = amap.fault_location if amap is not None else (lambda x: x)
        detail = ((access, address if access in DATA_FAULTS else fault_loc(address), size), fault_loc(eip))
    elif result.outcome in ("error", "budget"):
        detail = detail[:-1] + (loc(detail[-1]),)
    regs = result.regs
    if profile == "abi":
        regs = tuple(r for n, r in zip(emu.REG_NAMES, regs) if n in ABI_REGS)
        events = [e for e in result.events if e[0] == "c"]
    else:
        events = list(result.events)
    memory = {}
    floor = result.regs[4]
    for page, blob in result.memory.items():
        if amap is not None and amap.lo <= page < amap.hi:
            continue  # recovered image pages; their accesses are compared via events
        if profile == "abi" and emu.STACK <= page < emu.STACK + emu.STACK_SIZE:
            # Stack below the final ESP is dead: restore its pristine bytes.
            dead = max(0, min(len(blob), floor - page))
            base = result.pristine.get(page, bytes(len(blob)))
            blob = base[:dead] + blob[dead:]
            if blob == base:
                continue
        memory[page] = blob
    return (result.outcome, detail, regs, events, memory)


ABI_REGS = ("eax", "edx", "ebx", "esp", "ebp", "esi", "edi")
SEMANTIC_REGS = {"void": (), "int8": ("eax",), "int16": ("eax",), "int": ("eax",), "int64": ("eax", "edx"), "float": ()}
CALLEE_SAVED = ("ebx", "esp", "ebp", "esi", "edi")


def _normalize_semantic(result: emu.Result, amap: emu.AddressMap | None, returns: str, fp: tuple,
                        shape: semantic.IndirectCall | None = None):
    """Game-observable effects only (ADR 0002). A fault keeps its data access but
    not the registers or instruction at the fault; the stack is compiler layout.
    An indirect stop keeps its target and what the listing passes (ECX, pushed
    arguments), never the site or other registers."""
    loc = amap.location if amap is not None else (lambda x: x)
    canon = semantic.canonical
    detail, regs = result.detail, ()
    if result.outcome == "fault":
        (access, address, size), eip = detail
        if (access in DATA_FAULTS and return_transport.STACK_PROBE <= eip <= return_transport.STACK_PROBE_RET
                and semantic.GUARD_START <= address < emu.STACK):
            address = semantic.STACK_GUARD_MARK  # probe fault below the stack: offset is frame depth
        detail = (access, canon(address if access in DATA_FAULTS else loc(address)), size)
    elif result.outcome in semantic.INDIRECT_STOPS:
        target, _site, ecx, words, own = detail
        shape = own or shape or semantic.IndirectCall(False, 0)
        detail = (canon(target), canon(ecx) if shape.ecx else None,
                  tuple(canon(w) if w is not None else None for w in words[:shape.args or 0]))
    elif result.outcome in ("error", "budget"):
        detail = detail[:-1]
    elif result.outcome == "return":
        names = SEMANTIC_REGS[returns] + CALLEE_SAVED
        mask = semantic.RETURN_MASK.get(returns)
        regs = tuple(r & mask if n == "eax" and mask else canon(r) if n not in ("esp", "ebp") else r
                     for n, r in zip(emu.REG_NAMES, result.regs) if n in names) + tuple(fp)
    events = [(k, name, n, tuple(canon(v) for v in args)) for k, name, n, args in result.events if k == "c"]
    memory = {}
    for page, blob in result.memory.items():
        if (amap is not None and amap.lo <= page < amap.hi) or emu.STACK <= page < emu.STACK + emu.STACK_SIZE:
            continue
        memory[page] = semantic.canonical_blob(blob)
    return (result.outcome, detail, regs, events, memory)


def semantic_fp(uc, returns: str) -> tuple:
    """FPU state a caller can observe: control word, stack depth, and ST(0) for a float return."""
    from unicorn import x86_const as R

    fpsw = uc.reg_read(R.UC_X86_REG_FPSW)
    top = (fpsw >> 11) & 7
    out = (uc.reg_read(R.UC_X86_REG_FPCW), top)
    if returns == "float":
        mantissa, exponent = uc.reg_read(getattr(R, f"UC_X86_REG_FP{top}"))
        out += ((exponent << 64) | mantissa,)
    return out


def stop_shape(result: emu.Result) -> semantic.IndirectCall | None:
    """The original listing's argument shape at an indirect stop, for the recovered side."""
    return result.detail[4] if result.outcome in semantic.INDIRECT_STOPS else None


@functools.cache
def _entries() -> frozenset[int]:
    return frozenset(inventory.load().functions)


def _stop_indirect(original: emu.Machine, recovered: emu.Machine | None, region: tuple[int, int],
                   built, indirect: dict) -> None:
    """Arm indirect stops (semantic profile): the original at its listing's sites, the
    recovered build at its own compiled sites and any original site it enters."""
    words = max((shape.args or 0 for shape in indirect.values()), default=0)
    original.stop_indirect(indirect, _entries(), (region,), words)
    if recovered is not None:
        text = built.image.text
        sites = {va: None for va in semantic.indirect_sites(text.raw, text.va)} | indirect
        recovered.stop_indirect(sites, _entries(), (region, (text.va, text.end)), words)


def _equal(x: int, y: int, amap: emu.AddressMap | None) -> bool:
    """Original value x matches recovered value y if equal, or if y is the recovered
    address of exactly the code/data that x names in the original."""
    return x == y or (amap is not None and amap.value(y) == x)


def _equal_location(x: int, y: int, amap: emu.AddressMap | None) -> bool:
    """Keep an identical absolute address before testing its relocation relation."""
    return x == y or (amap is not None and amap.location(y) == x)


def _fault_parts(detail):
    if not isinstance(detail, tuple):
        return None
    if len(detail) == 2 and isinstance(detail[0], tuple) and len(detail[0]) == 3:
        return (*detail[0], detail[1])
    if len(detail) == 3 and all(type(part) is int for part in detail):
        return (*detail, None)  # Existing semantic profile omits the instruction.
    return None


def _mapping_proof_matches(proof, address: int, size: int) -> bool:
    """Check the identity and complete span of a Machine-produced page witness."""
    if (not isinstance(proof, dict) or proof.get('schema') != 'cmre-fault-mapping/1'
            or proof.get('raw_access') != unicorn.UC_MEM_WRITE_PROT
            or proof.get('projected_access') != unicorn.UC_MEM_WRITE_UNMAPPED
            or proof.get('relation') != 'candidate-original-unmapped-write'
            or proof.get('applied') is not False or proof.get('original_mapping') != 'unmapped'
            or type(proof.get('address')) is not int or type(proof.get('size')) is not int
            or proof.get('address') != address or proof.get('size') != size
            or type(address) is not int or type(size) is not int
            or not 0 <= address < 1 << 32 or size <= 0 or address + size > 1 << 32):
        return False
    pages = proof.get('pages')
    first = address & ~(emu.PAGE - 1)
    last = (address + size - 1) & ~(emu.PAGE - 1)
    if not isinstance(pages, list) or len(pages) != (last - first) // emu.PAGE + 1:
        return False
    for index, page in enumerate(pages):
        expected_page = first + index * emu.PAGE
        if (not isinstance(page, dict) or page.get('page') != expected_page
                or page.get('origin') != 'recovered-pe'
                or page.get('original_mapped') is not False
                or page.get('common_mapped') is not False):
            return False
        for key in ('initial_perms', 'current_perms'):
            perms = page.get(key)
            if (type(perms) is not int or not perms & unicorn.UC_PROT_READ
                    or perms & unicorn.UC_PROT_WRITE or perms & ~unicorn.UC_PROT_ALL):
                return False
        sections = page.get('recovered_sections')
        if not isinstance(sections, list) or not sections:
            return False
        for section in sections:
            if (not isinstance(section, dict) or section.get('writable') is not False
                    or type(section.get('va')) is not int or type(section.get('end')) is not int
                    or not 0 <= section['va'] < section['end'] <= 1 << 32):
                return False
            section_start = section['va'] & ~(emu.PAGE - 1)
            section_end = (section['end'] + emu.PAGE - 1) & ~(emu.PAGE - 1)
            if not section_start <= expected_page < section_end:
                return False
    return True


def _mapping_fault_pair(a, b, proof) -> bool:
    left, right = _fault_parts(a), _fault_parts(b)
    return bool(left and right and left[0] == unicorn.UC_MEM_WRITE_UNMAPPED
                and right[0] == unicorn.UC_MEM_WRITE_PROT
                and left[1:] == right[1:] and _mapping_proof_matches(proof, left[1], left[2]))


def _equal_fault_detail(a, b, amap, proof, *, strict_data_fault=False) -> bool:
    left, right = _fault_parts(a), _fault_parts(b)
    if left is None or right is None or left[2:] != right[2:]:
        return False
    if left[0] == right[0]:
        if strict_data_fault and left[0] in DATA_FAULTS:
            return left[1] == right[1]
        return (_equal_location(left[1], right[1], amap) if left[0] in DATA_FAULTS
                else left[1] == right[1])
    return _mapping_fault_pair(a, b, proof)


def _return_byte_pairs(events, amap) -> set[tuple[int, int]]:
    """(original byte, recovered byte) pairs of return addresses the recovered side
    pushed in this case. Bytes of a dead return address may be copied one at a time."""
    pairs = set()
    if amap is None or events is None:
        return pairs
    for e in events:
        if e[0] == "w" and e[2] == 4 and e[3] in amap.call_returns:
            o, r = amap.call_returns[e[3]], e[3]
            pairs |= {((o >> 8 * i) & 0xFF, (r >> 8 * i) & 0xFF) for i in range(4)}
    return pairs


def _equal_small(x: int, y: int, size: int, pairs) -> bool:
    return all(((x >> 8 * i) & 0xFF, (y >> 8 * i) & 0xFF) in pairs or (x >> 8 * i) & 0xFF == (y >> 8 * i) & 0xFF
               for i in range(size))


def _equal_blob(x: bytes, y: bytes, amap, pairs=frozenset()) -> bool:
    if x == y:
        return True
    if x is None or y is None or len(x) != len(y) or len(x) % 4:
        return False
    xs, ys = struct.unpack(f"<{len(x) // 4}I", x), struct.unpack(f"<{len(y) // 4}I", y)
    if all(_equal(p, q, amap) or _equal_small(p, q, 4, pairs) for p, q in zip(xs, ys)):
        return True
    # x86 stores need not be aligned. A relocated pointer can straddle two
    # comparison words; require its complete dword to map to the original.
    offset = 0
    while offset < len(x):
        if x[offset] == y[offset] or (x[offset], y[offset]) in pairs:
            offset += 1
            continue
        for start in range(max(0, offset - 3), min(offset, len(x) - 4) + 1):
            p = struct.unpack_from("<I", x, start)[0]
            q = struct.unpack_from("<I", y, start)[0]
            if _equal(p, q, amap):
                offset = start + 4
                break
        else:
            return False
    return True


def _diff(a, b, amap=None, *, fault_mapping=None, strict_data_fault=False) -> dict:
    """Empty dict when equivalent; otherwise the first difference, for the report."""
    if a[0] != b[0]:
        return {"field": "outcome", "original": str(a[0]), "recovered": str(b[0])}
    if a[1] != b[1] and not (a[0] == 'fault' and _equal_fault_detail(
            a[1], b[1], amap, fault_mapping, strict_data_fault=strict_data_fault)):
        return {"field": "detail", "original": str(a[1]), "recovered": str(b[1])}
    if len(a[2]) != len(b[2]) or not all(_equal(x, y, amap) for x, y in zip(a[2], b[2])):
        return {"field": "regs", "original": [hex(v) for v in a[2]], "recovered": [hex(v) for v in b[2]]}
    x, y = a[3], b[3]
    pairs = _return_byte_pairs(y, amap)
    if x is not None:
        for i, (p, q) in enumerate(zip(x, y)):
            if p[0] == "c" or q[0] == "c":
                same = p[:3] == q[:3] and len(p[3]) == len(q[3]) and all(_equal(u, v, amap) for u, v in zip(p[3], q[3]))
            else:
                same = (p[0] == q[0] and p[2] == q[2] and _equal_location(p[1], q[1], amap)
                        and (p[3] == q[3] or (p[2] == 4 and _equal(p[3], q[3], amap))
                             or _equal_small(p[3], q[3], p[2], pairs)))
            if not same:
                return {"field": "events", "index": i, "original": str(x[i:i + 3]), "recovered": str(y[i:i + 3]),
                        "lengths": [len(x), len(y)]}
        if len(x) != len(y):
            i = min(len(x), len(y))
            return {"field": "events", "index": i, "original": str(x[i:i + 3]), "recovered": str(y[i:i + 3]),
                    "lengths": [len(x), len(y)]}
    pages = sorted(p for p in set(a[4]) | set(b[4]) if not _equal_blob(a[4].get(p), b[4].get(p), amap, pairs))
    if pages:
        return {"field": "memory", "pages": [hex(p) for p in pages[:8]]}
    return {}


def _digest_view(n) -> str:
    outcome, detail, regs, events, memory = n
    return repr((outcome, detail, regs, events, {p: hashlib.sha256(b).hexdigest() for p, b in memory.items()}))


def _reads_own_code(result: emu.Result, region, data_spans) -> bool:
    """Original side read (or tried to write) its own instruction bytes (not copied data/tables)."""
    for e in result.events:
        if e[0] in ("r", "w") and region[0] <= e[1] < region[1] and not any(lo <= e[1] < hi for lo, hi in data_spans):
            return True
    return False


# CRT __chkstk (preferred-image VAs, SHA-256 0c78e7da...f86): `test [ecx], eax` probes the pages
# below ESP and discards the value, so those reads say nothing about frame layout.
STACK_PROBE_CODE = ((0x00945610, 0x00945640),)
STACK_LAYOUT_LIMIT = 0.02  # fraction of cases; above it the layout-dependent mismatches stay mismatches


def _reads_dead_stack_before(rerun: emu.Result, difference: dict, profile: str) -> bool:
    """The original loaded stack bytes wholly below the live ESP, other than a stack probe,
    before the first difference.

    Those bytes are leftovers of earlier or deeper frames. Their values follow the frame
    layout the compiler chose, which a faithful C function is free to change (ADR 0002).
    For an event difference the read must precede the original's differing import call;
    a difference in the outcome, fault detail, registers or memory takes any such read."""
    limit = (_raw_event_index(rerun.events, difference["index"], profile)
             if difference.get("field") == "events" else None)
    return any(not any(lo <= eip < hi for lo, hi in STACK_PROBE_CODE) and (limit is None or index < limit)
               for index, _address, eip in rerun.dead_stack_reads)


def _classify(na, nb, original: emu.Result, amap, region, data_spans, *, fault_mapping=None,
              layout_hazard=None, profile="semantic") -> tuple[str, dict]:
    """equal | code-as-data | stack-layout | mismatch. Relocated code cannot reproduce its own
    instruction bytes when the original reads them as data: a hazard, not a mismatch. With
    `layout_hazard` (C tier: a callable that re-runs the original with dead-stack tracking), a
    difference after the original read dead stack bytes is likewise a hazard: the case depends
    on the compiler's frame layout, not on the function's behaviour. A re-run that does not
    reproduce the first run exactly is ignored."""
    if original.outcome == 'error' and original.detail and str(original.detail[0]).startswith('import transcript'):
        return 'mismatch', {'field': 'import-transcript', 'original': original.detail}
    if nb[0] == 'error' and nb[1] and str(nb[1][0]).startswith('import transcript'):
        return 'mismatch', {'field': 'import-transcript', 'recovered': nb[1]}
    difference = _diff(na, nb, amap, fault_mapping=fault_mapping)
    if not difference:
        return "equal", {}
    if _reads_own_code(original, region, data_spans):
        return "code-as-data", difference
    if layout_hazard is not None:
        rerun = layout_hazard()
        same = ((rerun.outcome, rerun.detail, rerun.regs, rerun.events)
                == (original.outcome, original.detail, original.regs, original.events))
        if same and _reads_dead_stack_before(rerun, difference, profile):
            return "stack-layout", difference
    return "mismatch", difference


# ADR 0012: the shared budget stops code that never finishes; it is not a speed contest.
# When the C exhausts it on a case the original finished, that C case runs once more with
# this many times the budget. Results are then compared as usual; C still running is a
# budget-asymmetry mismatch, so a real hang still fails.
RECOVERED_BUDGET_FACTOR = 20


def _run_recovered(recovered, entry, case, budget, original_outcome, profile, allow=True):
    """(result, extended): the C run, extended once under ADR 0012 when only the C ran out.
    `allow` is False after an extended run of the same function still ran out: the verdict is
    settled, and extending every later case would make a hanging candidate ~10x slower to fail."""
    b = recovered.run(entry, case, budget)
    if allow and profile in ("abi", "semantic") and b.outcome == "budget" and original_outcome != "budget":
        return recovered.run(entry, case, budget * RECOVERED_BUDGET_FACTOR), True
    return b, False


CODE_AS_DATA_LIMIT = 0.02  # fraction of cases; above it the function is blocked for review


def run(va: int, region: tuple[int, int], insn_starts: set[int], built, call_returns: dict[int, int],
        opts: Options, profile: str, data_spans=(), returns: str = "int", indirect: dict | None = None) -> dict:
    img = image.primary()
    original = emu.Machine(img, watch=region, arena_size=opts.hints.get("arena_size", emu.ARENA_SIZE))
    recovered = emu.Machine(img, built.image, watch=region, escape=True,
                            arena_size=opts.hints.get("arena_size", emu.ARENA_SIZE))
    if profile == "semantic" and indirect is not None:
        _stop_indirect(original, recovered, region, built, indirect)
    text = built.image.text
    amap = emu.AddressMap(built.symbols, call_returns, text.va, text.end)
    templates = arena_templates()
    outcomes: dict[str, int] = {}
    mismatches = []
    inconclusive = 0
    extended_cases = 0
    extension_failed = False
    covered: set[int] = set()
    import_calls = 0
    code_as_data = []
    stack_layout = []
    mapping_faults = []
    global_inputs: set[int] = set()
    digest = hashlib.sha256()
    compare_fp = bool(getattr(built.image, 'x87_bindings', ()))
    input_cases = 0
    # Limits below keep opts.cases as their denominator: hinted-input cases add to the
    # counts only, so the gate never accepts what the random cases alone would reject.
    for index, case, discovering in gate_cases(opts, global_inputs, templates):
        input_cases += index >= opts.cases
        own = _attribute(original, opts, discovering, region)
        a = original.run(va, case, opts.budget)
        fp_a = x87_transport.snapshot(original.uc) if compare_fp else None
        sem_a = semantic_fp(original.uc, returns) if profile == "semantic" else ()
        b, extended = _run_recovered(recovered, built.entry, case, opts.budget, a.outcome, profile,
                                     allow=not extension_failed)
        extended_cases += extended
        extension_failed = extension_failed or (extended and b.outcome == "budget")
        fp_b = x87_transport.snapshot(recovered.uc) if compare_fp else None
        sem_b = semantic_fp(recovered.uc, returns) if profile == "semantic" else ()
        if discovering:
            global_inputs |= _global_reads(a, img, own)
        import_calls += sum(1 for e in a.events if e[0] == "c")
        na = _normalize(a, None, profile, returns, sem_a)
        nb = _normalize(b, amap, profile, returns, sem_b, stop_shape(a))
        if b.fault_mapping is not None:
            mapping_faults.append({'case': index, 'proof': b.fault_mapping,
                                   'relation_used': a.outcome == b.outcome == 'fault'
                                   and _mapping_fault_pair(na[1], nb[1], b.fault_mapping)})
        outcomes[a.outcome] = outcomes.get(a.outcome, 0) + 1
        digest.update(_digest_view(na).encode())
        if compare_fp:
            digest.update(repr(fp_a).encode())
        if profile in ("abi", "semantic") and "budget" in (a.outcome, b.outcome):
            covered |= a.covered
            inconclusive += 1
            if a.outcome != b.outcome and len(mismatches) < 5:
                mismatches.append({"case": index, "kind": "budget-asymmetry",
                                   "original": a.outcome, "recovered": b.outcome,
                                   **({"recovered_budget": opts.budget * RECOVERED_BUDGET_FACTOR}
                                      if extended else {}),
                                   "note": "Unresolved execution budget; investigate or raise the budget for both runs."})
            continue
        def tracked_rerun(case=case):
            # Deterministic: the same case on the same machine. Only differing cases pay for tracking.
            original.track_dead_stack_reads = True
            try:
                return original.run(va, case, opts.budget)
            finally:
                original.track_dead_stack_reads = False

        verdict, difference = _classify(na, nb, a, amap, region, data_spans, fault_mapping=b.fault_mapping,
                                        layout_hazard=tracked_rerun if profile == "semantic" else None,
                                        profile=profile)
        if compare_fp and (fp_difference := x87_transport.difference(fp_a, fp_b)):
            verdict, difference = 'mismatch', fp_difference
        if verdict == "stack-layout":
            # Waived: instructions reached only here are not evidence of coverage.
            stack_layout.append((index, difference))
            continue
        covered |= a.covered
        if verdict == "code-as-data":
            code_as_data.append(index)
            continue
        if verdict == "mismatch":
            if len(mismatches) < 5:
                mismatches.append({"case": index, **difference})
            elif len(mismatches) == 5:
                mismatches.append({"case": index, "note": "further mismatches omitted"})
    excess_layout = len(stack_layout) > STACK_LAYOUT_LIMIT * opts.cases
    if excess_layout:
        # Too many cases lean on dead stack to call them hazards: they stay mismatches.
        for index, difference in stack_layout[:max(0, 5 - len(mismatches))]:
            mismatches.append({"case": index, "kind": "stack-layout-excess", **difference})
    else:
        inconclusive += len(stack_layout)
    coverage = len(covered & insn_starts) / len(insn_starts) if insn_starts else 0.0
    return {
        "profile": profile,
        **({"returns": returns} if profile == "semantic" else {}),
        **({"inputs": opts.inputs} if opts.inputs != "v1" else {}),
        "cases": opts.cases,
        "input_cases": input_cases,
        "seed": opts.seed,
        "budget": opts.budget,
        **({'budget_policy': 'original-instructions/1',
            'native_step_guard': max(32, 4 * opts.budget + 16)}
           if getattr(recovered, 'logical_budget', False) else {}),
        "outcomes": dict(sorted(outcomes.items())),
        "inconclusive": inconclusive,
        **({"recovered_budget_factor": RECOVERED_BUDGET_FACTOR, "recovered_budget_extended": extended_cases}
           if extended_cases else {}),
        "mismatches": mismatches,
        "coverage": round(coverage, 4),
        "uncovered": sorted(f"{x:#010x}" for x in insn_starts - covered)[:40],
        "global_inputs": len(global_inputs),
        "import_calls": import_calls,
        "code_as_data_cases": len(code_as_data),
        "code_as_data_examples": code_as_data[:5],
        **({"stack_layout_cases": len(stack_layout), "stack_layout_examples": [i for i, _ in stack_layout[:5]]}
           if stack_layout else {}),
        **({'mapping_faults': mapping_faults} if mapping_faults else {}),
        "result_digest": digest.hexdigest(),
        "fp_registers_compared": list(x87_transport.FP_REGISTERS) if compare_fp else [],
    }


def _raw_event_index(events, comparison_index: int, profile: str) -> int:
    if profile not in ('abi', 'semantic'):
        return comparison_index
    imports = (index for index, event in enumerate(events) if event[0] == 'c')
    return next((index for offset, index in enumerate(imports) if offset == comparison_index), len(events))


def explain(va: int, region, insn_starts, built, call_returns, opts: Options, profile: str,
            case_index: int | None = None, window: int = 6, returns: str = "int",
            indirect: dict | None = None) -> dict:
    """Diagnostics for agents: where the original stops, or one case side by side.

    Replays the identical case sequence as run(), so case numbers match reports.
    """
    if type(window) is not int or not 1 <= window <= 10000:
        raise ValueError('explain window must be an integer between 1 and 10000')
    img = image.primary()
    original = emu.Machine(img, watch=region, arena_size=opts.hints.get("arena_size", emu.ARENA_SIZE))
    recovered = emu.Machine(img, built.image, watch=region, escape=True,
                            arena_size=opts.hints.get("arena_size", emu.ARENA_SIZE)) if built else None
    if profile == "semantic" and indirect is not None:
        _stop_indirect(original, recovered, region, built, indirect)
    text = built.image.text if built else None
    amap = emu.AddressMap(built.symbols, call_returns, text.va, text.end) if built else None
    templates = arena_templates()
    global_inputs: set[int] = set()
    stops: dict[str, int] = {}
    numbers = opts.cases + len(hinted_inputs(opts))
    if case_index is not None and not (type(case_index) is int and 0 <= case_index < numbers):
        raise ValueError(f"case {case_index} is not in this gate: random cases are 0..{opts.cases - 1}"
                         f" and hinted inputs {opts.cases}..{numbers - 1}")
    for index, case, discovering in gate_cases(opts, global_inputs, templates):
        if case_index is not None and index != case_index and index >= opts.cases:
            continue  # hinted inputs never feed later cases; run() resets the machine per case
        own = _attribute(original, opts, discovering, region)
        if index == case_index:
            original.attribute_reads = True
        a = original.run(va, case, opts.budget)
        if discovering:
            global_inputs |= _global_reads(a, img, own)
        if index == case_index:
            break
        if case_index is None:
            if a.outcome == "fault":
                (access, address, _), eip = a.detail
                key = f"fault eip={eip:#010x} {ACCESS.get(access, access)}"
                key = key if region[0] <= eip < region[1] else f"fault in callee eip={eip:#010x}"
            else:
                key = f"{a.outcome} {a.detail[0] if a.detail else ''}".strip()
            stops[key] = stops.get(key, 0) + 1
    if case_index is None:
        top = sorted(stops.items(), key=lambda kv: -kv[1])[:15]
        return {"stops": dict(top), "global_inputs": [f"{g:#010x}" for g in sorted(global_inputs)[:30]]}
    out = {"case": case_index, "regs_in": {k: hex(v) for k, v in case.regs.items()},
           "stack_in": [hex(v) for v in case.stack], "globals_in": {hex(k): hex(v) for k, v in case.globals.items()},
           "fpcw_in": case.fpcw,
           "original": {"outcome": a.outcome, "detail": str(a.detail), "regs": [hex(r) for r in a.regs]}}
    if recovered:
        compare_fp = bool(getattr(built.image, 'x87_bindings', ()))
        fp_a = x87_transport.snapshot(original.uc) if compare_fp else None
        sem_a = semantic_fp(original.uc, returns) if profile == "semantic" else ()
        recovered.attribute_reads = True
        b, extended = _run_recovered(recovered, built.entry, case, opts.budget, a.outcome, profile)
        sem_b = semantic_fp(recovered.uc, returns) if profile == "semantic" else ()
        na = _normalize(a, None, profile, returns, sem_a)
        nb = _normalize(b, amap, profile, returns, sem_b, stop_shape(a))
        d = _diff(na, nb, amap, fault_mapping=b.fault_mapping)
        if compare_fp:
            fp_b = x87_transport.snapshot(recovered.uc)
            d = x87_transport.difference(fp_a, fp_b) or d
            out['original']['fp'] = fp_a
        out["recovered"] = {"outcome": b.outcome, "detail": str(b.detail), "regs": [hex(r) for r in b.regs],
                            **({"budget_extended": True} if extended else {})}
        if b.fault_mapping is not None:
            out['recovered']['fault_mapping'] = b.fault_mapping
            out['recovered']['mapping_relation_used'] = (
                a.outcome == b.outcome == 'fault' and _mapping_fault_pair(na[1], nb[1], b.fault_mapping))
        if compare_fp:
            out['recovered']['fp'] = fp_b
        out["difference"] = d
        i = d.get("index", 0)
        ai = _raw_event_index(a.events, i, profile) if d.get('field') == 'events' else i
        bi = _raw_event_index(b.events, i, profile) if d.get('field') == 'events' else i
        if d.get('field') == 'events':
            out['event_context'] = {'comparison_index': i, 'original_raw_index': ai, 'recovered_raw_index': bi}
        out["events_original_raw"] = [str(e) for e in a.events[max(0, ai - window): ai + window]]
        out["events_recovered_raw"] = [str(e) for e in b.events[max(0, bi - window): bi + window]]
        out['read_sites_original'] = {str(j): f'{site:#010x}' for j, site in a.read_sites.items()
                                      if max(0, ai - window) <= j < ai + window}
        out['read_sites_recovered'] = {str(j): f'{site:#010x}' for j, site in b.read_sites.items()
                                       if max(0, bi - window) <= j < bi + window}
    return out


ACCESS = {19: "read-unmapped", 20: "write-unmapped", 21: "fetch-unmapped",
          22: "write-protected", 23: "read-protected", 24: "fetch-protected"}
DATA_FAULTS = (19, 20, 22, 23)


def _input_extents(va: int, insns: dict, arg_count: int) -> dict[tuple, int]:
    """Positive access spans attributed to entry inputs, following direct branches.

    Copy origins by value so overwrites cannot retarget an earlier alias. At a
    join retain possible input origins, but only agreed-on frame offsets. Unknown
    writes and indirect pointer loads end an alias; stack slots track saved values
    and argument overwrites.
    This only shapes cases: infer's original-side fault votes still establish ptrs.
    """
    import capstone
    from capstone import x86_const as X

    names = emu.REG_NAMES[:8]
    regs = {name: getattr(X, f"X86_REG_{name.upper()}") for name in names}
    parents = {reg: reg for reg in regs.values()}
    for name in ("eax", "ecx", "edx", "ebx"):
        for part in (name[1:], name[1] + "l", name[1] + "h"):
            parents[getattr(X, f"X86_REG_{part.upper()}")] = regs[name]
    for name, part in (("esp", "sp"), ("ebp", "bp"), ("esi", "si"), ("edi", "di")):
        parents[getattr(X, f"X86_REG_{part.upper()}")] = regs[name]
    esp = regs["esp"]
    initial = {reg: frozenset({("reg", name) if name != "esp" else ("stack", 0)}) for name, reg in regs.items()}
    initial.update({("slot", 4 * (i + 1)): frozenset({("arg", i)}) for i in range(arg_count)})
    states = {va: initial}

    def frame_offset(origins):
        if len(origins) == 1:
            kind, offset = next(iter(origins))
            if kind == "stack":
                return offset
        return None

    def slot_offset(op, state):
        if op.type == X.X86_OP_MEM and op.mem.index == 0:
            offset = frame_offset(state.get(op.mem.base, frozenset()))
            if offset is not None:
                return offset + op.mem.disp
        return None

    def source(op, state):
        if op.type == X.X86_OP_REG and op.size == 4:
            return state.get(op.reg, frozenset())
        offset = slot_offset(op, state)
        if offset is not None and op.size == 4:
            return state.get(("slot", offset), frozenset())
        return frozenset()

    def store_slot(state, offset, size, origins):
        # Explicit stores and implicit pushes share the same overlap rule.
        for key in list(state):
            if isinstance(key, tuple) and key[0] == "slot" and key[1] < offset + size and offset < key[1] + 4:
                state.pop(key)
        if size == 4:
            state[("slot", offset)] = origins

    pending = [va]
    while pending:
        address = pending.pop()
        ins = insns.get(address)
        if ins is None:
            continue
        before = states[address]
        after = dict(before)
        _, written = ins.regs_access()
        for reg in written:
            after.pop(parents.get(reg, reg), None)
        ops = ins.operands
        m = ins.mnemonic
        for op in ops:
            offset = slot_offset(op, before)
            if offset is None or not op.access & capstone.CS_AC_WRITE:
                continue
            origins = source(ops[1], before) if m == "mov" and op is ops[0] else frozenset()
            store_slot(after, offset, op.size, origins)
        if m == "mov" and len(ops) == 2 and ops[0].type == X.X86_OP_REG and ops[0].size == 4:
            after[ops[0].reg] = source(ops[1], before)
        # Track ESP/EBP frame offsets without treating frame addresses as inputs.
        offset = frame_offset(before.get(esp, frozenset()))
        if offset is not None:
            if m == "push":
                after[esp] = frozenset({("stack", offset - ops[0].size)})
                store_slot(after, offset - ops[0].size, ops[0].size, source(ops[0], before))
            elif m == "pop" and not (ops[0].type == X.X86_OP_REG and ops[0].reg == esp):
                after[esp] = frozenset({("stack", offset + ops[0].size)})
                if ops[0].type == X.X86_OP_REG and ops[0].size == 4:
                    after[ops[0].reg] = before.get(("slot", offset), frozenset())
            elif m in ("add", "sub") and ops[0].type == X.X86_OP_REG and ops[0].reg == esp and ops[1].type == X.X86_OP_IMM:
                after[esp] = frozenset({("stack", offset + ops[1].imm * (1 if m == "add" else -1))})
        if m == "lea" and ops[0].size == 4 and ops[1].mem.index == 0:
            offset = slot_offset(ops[1], before)
            if offset is not None:
                after[ops[0].reg] = frozenset({("stack", offset)})
        # Calls leave stack cleanup unknown. Data origins are candidate aliases,
        # gated by the runtime votes rather than claims about callee behaviour.
        successors = []
        if not ins.group(capstone.CS_GRP_RET):
            if ins.group(capstone.CS_GRP_JUMP):
                if ops[0].type == X.X86_OP_IMM:
                    successors.append(ops[0].imm & 0xFFFFFFFF)
                if m != "jmp":
                    successors.append(address + ins.size)
            else:
                successors.append(address + ins.size)
        for target in successors:
            if target not in insns:
                continue
            if target not in states:
                states[target] = dict(after)
            else:
                merged = {}
                for key in states[target].keys() | after.keys():
                    old = states[target].get(key, frozenset())
                    new = after.get(key, frozenset())
                    origins = frozenset(origin for origin in old | new if origin[0] != "stack" or old == new)
                    if origins:
                        merged[key] = origins
                if merged == states[target]:
                    continue
                states[target] = merged
            pending.append(target)
    extents = {}
    for address, state in states.items():
        ins = insns.get(address)
        if ins is None or ins.mnemonic == "lea":
            continue
        for op in ins.operands:
            if op.type != X.X86_OP_MEM or op.mem.disp <= 0 or op.mem.index != 0:
                continue
            for origin in state.get(op.mem.base, frozenset()):
                if origin[0] in ("reg", "arg"):
                    extents[origin] = max(extents.get(origin, 0), op.mem.disp + op.size)
    return extents


def infer(va: int, region, opts: Options) -> dict:
    """Infer pointer inputs from fault sites inside the function (original side only).

    An input is a pointer when fault_address == input + displacement of the faulting
    instruction's [base + disp] operand, in at least half the faults at that site.
    """
    import capstone
    from capstone import x86_const as X
    from . import inventory

    img = image.primary()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    machine = emu.Machine(img, watch=region, arena_size=opts.hints.get("arena_size", emu.ARENA_SIZE))
    rng = random.Random(opts.seed)
    templates = arena_templates()
    global_inputs: set[int] = set()
    sites: dict[int, int] = {}
    votes: dict[tuple, dict[tuple, int]] = {}
    for index in range(opts.cases):
        case = make_case(rng, opts, sorted(global_inputs), templates)
        own = _discovery(machine, opts, index, region)
        r = machine.run(va, case, opts.budget)
        if index < opts.discovery:
            global_inputs |= _global_reads(r, img, own)
        if r.outcome != "fault":
            continue
        (access, address, _), eip = r.detail
        if access not in DATA_FAULTS or not (region[0] <= eip < region[1]):
            continue
        ins = next(md.disasm(img.read_upto(eip, 16), eip, count=1), None)
        mem = next((o for o in ins.operands if o.type == X.X86_OP_MEM), None) if ins else None
        if mem is None or mem.mem.base == 0 or mem.mem.index != 0:
            continue
        disp = mem.mem.disp
        sites[eip] = sites.get(eip, 0) + 1
        inputs = [(("reg", n), v) for n, v in case.regs.items() if n not in ("esp", "eflags")]
        inputs += [(("arg", i), v) for i, v in enumerate(case.stack)]
        for key, value in inputs:
            if (value + disp) & 0xFFFFFFFF == address:
                votes.setdefault(eip, {})
                votes[eip][key] = votes[eip].get(key, 0) + 1
    hints = {k: v for k, v in opts.hints.items()}
    args = list(hints.get("args", []))
    # First pass: vote on whether each input is a pointer (matches a fault site).
    for eip, n in sorted(sites.items()):
        for (kind, which), count in sorted(votes.get(eip, {}).items()):
            if count * 2 < n:
                continue
            if kind == "reg":
                hints[which] = hints.get(which) or "ptr"
            else:
                args += ["any"] * (which + 1 - len(args))
                if args[which] != "struct":
                    args[which] = "ptr"
    # _pointer reserves 0x400 bytes at the arena's tail. Larger positive access
    # spans need size-aware placement, even when the arena itself is much larger.
    inv = inventory.load()
    f = inv.functions.get(va)
    if f:
        blob = img.read(va, f.body_bytes)
        insns = {ins.address: ins for ins in md.disasm(blob, va)}
        for (kind, which), size in _input_extents(va, insns, opts.arg_count).items():
            if not POINTER_TAIL_RESERVE < size <= emu.ARENA_SIZE:
                continue  # An object larger than the arena remains unshapeable.
            current = hints.get(which) if kind == "reg" else (args[which] if which < len(args) else None)
            if current not in ("ptr", "struct"):
                continue
            if kind == "reg":
                hints[which] = "struct"
            else:
                args[which] = "struct"
            hints["struct_size"] = max(hints.get("struct_size", 0), size)
    if args:
        hints["args"] = args
    hints["auto"] = True
    return hints
