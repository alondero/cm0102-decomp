"""Asset-free validation and fixture patch helpers for a scoped original-only probe broker.

This module deliberately contains **no** emulator execution logic.  It only
provides pure, side effect free helpers:

* :func:`apply_patch` - build a patched fixture from a base fixture.
* :func:`validate_request` - normalise and validate a probe request.
* :func:`validate_global_writes` - check that global writes land inside a
  single writable, non executable data section of a caller supplied image.

Only the standard library is used and no game harness is imported at module
import time.
"""

from __future__ import annotations

import copy
import json
import re
from collections.abc import Mapping
from typing import Any, Callable, Optional

__all__ = [
    "ProbeError",
    "PatchError",
    "RequestError",
    "RequestPatchError",
    "GlobalWriteError",
    "REG_NAMES",
    "PATCH_KEYS",
    "REQUEST_KEYS",
    "SHAPE_KEYS",
    "MAX_MAPPING_ENTRIES",
    "MAX_STACK_WORDS",
    "MAX_BUDGET",
    "MAX_JSON_BYTES",
    "U32_MAX",
    "apply_patch",
    "validate_request",
    "validate_global_writes",
]


# constants

REG_NAMES = (
    "eax",
    "ecx",
    "edx",
    "ebx",
    "esp",
    "ebp",
    "esi",
    "edi",
    "eflags",
)

PATCH_KEYS = ("regs", "stack", "arena_writes", "globals")

#: Fixture fields that describe the original import model.  A patch may never
#: touch them - imported results are not probe controllable.
FORBIDDEN_PATCH_KEYS = ("import_seed", "import_transcript")

REQUEST_KEYS = ("function", "patch", "budget", "shapes", "name")
REQUIRED_REQUEST_KEYS = ("function", "shapes", "name")
SHAPE_KEYS = ("ecx", "stack_words")

MAX_MAPPING_ENTRIES = 4096
MAX_STACK_WORDS = 64
MAX_BUDGET = 200000
MAX_JSON_BYTES = 1024 * 1024
U32_MAX = 0xFFFFFFFF

_NAME_RE = re.compile(r"[a-z0-9][a-z0-9_-]{0,63}")


# errors


class ProbeError(ValueError):
    """Base class for every probe validation failure."""


class PatchError(ProbeError):
    """Raised when a fixture patch is malformed or not permitted."""


class RequestError(ProbeError):
    """Raised when a probe request is malformed or not permitted."""


class RequestPatchError(RequestError, PatchError):
    """Raised when the ``patch`` member of a probe request is invalid.

    It derives from both :class:`RequestError` and :class:`PatchError` so
    callers can catch whichever flavour they consider natural.
    """


class GlobalWriteError(ProbeError):
    """Raised when a global write does not fit a writable data section."""


# small helpers


def _require_u32(value: Any, label: str, err: type[ProbeError] = PatchError) -> int:
    """Return ``value`` when it is a u32 integer, otherwise raise ``err``."""

    if isinstance(value, bool) or not isinstance(value, int):
        raise err(f"{label} must be a u32 integer, got {value!r}")
    if value < 0 or value > U32_MAX:
        raise err(f"{label} must be within 0..0x{U32_MAX:X}, got {value!r}")
    return value


def _parse_address(
    value: Any, label: str, err: type[ProbeError] = PatchError
) -> int:
    """Parse an address.

    Strings are parsed with ``int(text, 0)`` (strict: ``0x`` prefixed hex or
    plain decimal, no octal surprises), integers are accepted as is.  The
    result is a u32.
    """

    if isinstance(value, bool):
        raise err(f"{label} must be an address, got {value!r}")
    if isinstance(value, int):
        address = value
    elif isinstance(value, str):
        text = value.strip()
        if not text:
            raise err(f"{label} must be a non empty address string")
        try:
            address = int(text, 0)
        except ValueError:
            raise err(f"{label} {value!r} is not a valid address") from None
    else:
        raise err(f"{label} must be an address, got {value!r}")
    if address < 0 or address > U32_MAX:
        raise err(f"{label} must be within 0..0x{U32_MAX:X}, got {address!r}")
    return address


def _canonical_address(address: int) -> str:
    """Serialise an address as a canonical lower case hex string."""

    return f"0x{address:x}"


def _json_size(obj: Any) -> int:
    """Return the UTF-8 size of ``obj`` when serialised as strict JSON.

    Arbitrary objects and non finite numbers are rejected: a fixture must be
    representable as JSON data at this trusted boundary.
    """

    try:
        text = json.dumps(
            obj, sort_keys=True, allow_nan=False, separators=(",", ":")
        )
    except (TypeError, ValueError) as exc:
        raise PatchError(f"fixture is not strict JSON: {exc}") from exc
    return len(text.encode("utf-8"))


def _normalize_address_mapping(
    mapping: Any, label: str, err: type[ProbeError] = PatchError
) -> dict[int, int]:
    """Normalise an address keyed mapping to ``{int address: u32 value}``.

    Conflicting aliases (two spellings of the same address with different
    values) are rejected.
    """

    if not isinstance(mapping, Mapping):
        raise err(f"{label} must be a mapping")
    normalized: dict[int, int] = {}
    for key, value in mapping.items():
        address = _parse_address(key, f"{label} key", err=err)
        number = _require_u32(value, f"{label}[{key!r}]", err=err)
        if address in normalized and normalized[address] != number:
            raise err(
                f"{label} contains conflicting address aliases for "
                f"{_canonical_address(address)}"
            )
        normalized[address] = number
    if len(normalized) > MAX_MAPPING_ENTRIES:
        raise err(f"{label} exceeds the {MAX_MAPPING_ENTRIES} entry limit")
    return normalized


def _merge_address_mapping(
    base_value: Any, patch_value: Any, label: str, err: type[ProbeError] = PatchError
) -> dict[str, int]:
    """Merge the patch mapping into the base mapping and serialise as hex."""

    patch_normalized = _normalize_address_mapping(patch_value, f"patch[{label!r}]", err)
    if base_value is None:
        base_normalized: dict[int, int] = {}
    else:
        base_normalized = _normalize_address_mapping(base_value, f"base[{label!r}]", err)
    merged = dict(base_normalized)
    merged.update(patch_normalized)
    if len(merged) > MAX_MAPPING_ENTRIES:
        raise err(f"{label} exceeds the {MAX_MAPPING_ENTRIES} entry limit")
    return {_canonical_address(addr): merged[addr] for addr in sorted(merged)}


# public API


def apply_patch(
    base_fixture: dict,
    patch: dict,
    *,
    validate: Optional[Callable[[dict], Any]] = None,
) -> dict:
    """Return a patched copy of ``base_fixture``.

    Only the keys ``regs``, ``stack``, ``arena_writes`` and ``globals`` may
    appear in ``patch``.  ``import_seed`` and ``import_transcript`` are
    forbidden - the original import model is never probe controllable.

    Neither ``base_fixture`` nor ``patch`` is mutated.
    """

    if not isinstance(base_fixture, dict):
        raise PatchError("base_fixture must be a dict")
    if not isinstance(patch, dict):
        raise PatchError("patch must be a dict")

    for key in patch:
        if not isinstance(key, str):
            raise PatchError(f"patch keys must be strings, got {key!r}")
        if key in FORBIDDEN_PATCH_KEYS:
            raise PatchError(f"patch must not change {key}")
        if key not in PATCH_KEYS:
            raise PatchError(f"unknown patch key {key!r}")

    final = copy.deepcopy(base_fixture)

    if "regs" in patch:
        regs_patch = patch["regs"]
        if not isinstance(regs_patch, Mapping):
            raise PatchError("patch['regs'] must be a mapping")
        base_regs = final.get("regs")
        if base_regs is None:
            merged_regs: dict[str, Any] = {}
        elif isinstance(base_regs, Mapping):
            merged_regs = dict(base_regs)
        else:
            raise PatchError("base fixture 'regs' must be a mapping")
        for name, value in regs_patch.items():
            if name not in REG_NAMES:
                raise PatchError(f"unknown register {name!r}")
            merged_regs[name] = _require_u32(value, f"regs[{name!r}]")
        final["regs"] = copy.deepcopy(merged_regs)

    if "stack" in patch:
        stack = patch["stack"]
        if isinstance(stack, (str, bytes, bytearray)) or not isinstance(stack, (list, tuple)):
            raise PatchError("patch['stack'] must be a list of u32 words")
        if len(stack) > MAX_STACK_WORDS:
            raise PatchError(f"patch['stack'] exceeds the {MAX_STACK_WORDS} word limit")
        final["stack"] = [
            _require_u32(word, f"stack[{index}]") for index, word in enumerate(stack)
        ]

    aggregate = 0
    for key in ("arena_writes", "globals"):
        if key in patch:
            final[key] = _merge_address_mapping(final.get(key), patch[key], key)
        if key in final:
            aggregate += _json_size(final[key])
    if aggregate > MAX_JSON_BYTES:
        raise PatchError(f"address mappings exceed the {MAX_JSON_BYTES} byte JSON budget")
    if _json_size(final) > MAX_JSON_BYTES:
        raise PatchError(f"fixture exceeds the {MAX_JSON_BYTES} byte JSON budget")

    if validate is not None:
        outcome = validate(final)
        if outcome is False:
            raise PatchError("patch was rejected by the validation callback")

    return final


def _normalize_allowed_functions(allowed_functions: Any) -> set[int]:
    """Return the set of allowed function addresses."""

    if isinstance(allowed_functions, (str, bytes)) or not isinstance(
        allowed_functions, (list, tuple, set, frozenset)
    ):
        raise RequestError("allowed_functions must be a sequence of integers")
    allowed: set[int] = set()
    for entry in allowed_functions:
        allowed.add(
            _parse_address(entry, "allowed_functions entry", err=RequestError)
        )
    return allowed


def _normalize_shapes(shapes: Any, allowed: set[int]) -> dict[str, dict[str, Any]]:
    """Return the normalised ``{decimal VA string: shape}`` mapping."""

    if not isinstance(shapes, Mapping):
        raise RequestError("shapes must be a mapping")
    normalized: dict[str, dict[str, Any]] = {}
    for key, shape in shapes.items():
        address = _parse_address(key, "shape key", err=RequestError)
        if address not in allowed:
            raise RequestError(f"shape function 0x{address:x} is not in allowed_functions")
        if not isinstance(shape, Mapping):
            raise RequestError(f"shape for 0x{address:x} must be a mapping")
        unknown = sorted(str(name) for name in shape if name not in SHAPE_KEYS)
        if unknown:
            raise RequestError(
                f"shape for 0x{address:x} has unknown keys: {', '.join(unknown)}"
            )
        absent = [name for name in SHAPE_KEYS if name not in shape]
        if absent:
            raise RequestError(
                f"shape for 0x{address:x} is missing keys: {', '.join(absent)}"
            )
        ecx = shape["ecx"]
        if not isinstance(ecx, bool):
            raise RequestError(f"shape for 0x{address:x} 'ecx' must be a bool")
        stack_words = shape["stack_words"]
        if isinstance(stack_words, bool) or not isinstance(stack_words, int):
            raise RequestError(f"shape for 0x{address:x} 'stack_words' must be an integer")
        if stack_words < 0 or stack_words > MAX_STACK_WORDS:
            raise RequestError(
                f"shape for 0x{address:x} 'stack_words' must be within "
                f"0..{MAX_STACK_WORDS}"
            )
        normalized_shape = {"ecx": ecx, "stack_words": stack_words}
        normalized_key = str(address)
        if normalized_key in normalized and normalized[normalized_key] != normalized_shape:
            raise RequestError(f"conflicting shape aliases for function 0x{address:x}")
        normalized[normalized_key] = normalized_shape
    return normalized


def validate_request(
    request: dict,
    *,
    allowed_functions: list[int],
    max_budget: int = MAX_BUDGET,
) -> dict:
    """Validate and normalise a probe request.

    The returned request carries an integer ``function``, decimal string shape
    keys and the validated (normalised) patch.  The root shape of the probed
    function must be supplied explicitly - no ABI is guessed.
    """

    if not isinstance(request, dict):
        raise RequestError("request must be a dict")

    allowed = _normalize_allowed_functions(allowed_functions)

    if isinstance(max_budget, bool) or not isinstance(max_budget, int):
        raise RequestError("max_budget must be an integer")
    if max_budget <= 0:
        raise RequestError("max_budget must be positive")

    unknown = sorted(str(key) for key in request if key not in REQUEST_KEYS)
    if unknown:
        raise RequestError(f"unknown request keys: {', '.join(unknown)}")
    absent = [key for key in REQUIRED_REQUEST_KEYS if key not in request]
    if absent:
        raise RequestError(f"missing request keys: {', '.join(absent)}")

    function = _parse_address(request["function"], "function", err=RequestError)
    if function not in allowed:
        raise RequestError(f"function 0x{function:x} is not in allowed_functions")

    budget = request.get("budget", min(MAX_BUDGET, max_budget))
    if isinstance(budget, bool) or not isinstance(budget, int):
        raise RequestError("budget must be an integer")
    if budget <= 0:
        raise RequestError("budget must be positive")
    if budget > max_budget:
        raise RequestError(f"budget {budget} exceeds max_budget {max_budget}")

    shapes = _normalize_shapes(request["shapes"], allowed)
    if str(function) not in shapes:
        raise RequestError(
            f"root shape for function 0x{function:x} must be supplied explicitly"
        )

    name = request["name"]
    if not isinstance(name, str) or not _NAME_RE.fullmatch(name):
        raise RequestError("name must match [a-z0-9][a-z0-9_-]{0,63}")

    raw_patch = request.get("patch", {})
    if not isinstance(raw_patch, dict):
        raise RequestError("patch must be a dict")
    try:
        prepared = apply_patch({}, raw_patch)
    except PatchError as exc:
        raise RequestPatchError(f"invalid patch: {exc}") from exc
    normalized_patch = {key: prepared[key] for key in PATCH_KEYS if key in prepared}

    return {
        "function": function,
        "patch": normalized_patch,
        "budget": budget,
        "shapes": shapes,
        "name": name,
    }


def _section_field(section: Any, name: str, default: Any = None, required: bool = False) -> Any:
    """Read ``name`` from a section object (attribute or mapping access)."""

    value: Any = default
    found = False
    if isinstance(section, Mapping):
        if name in section:
            value = section[name]
            found = True
    else:
        try:
            value = getattr(section, name)
            found = True
        except AttributeError:
            found = False
    if not found and required:
        raise GlobalWriteError(f"section is missing the {name!r} attribute")
    return value


def validate_global_writes(fixture: dict, image: Any) -> None:
    """Check every global write of ``fixture`` against the supplied image.

    Each four byte global write must lie wholly inside a single writable and
    non executable section.  Code sections, read only data, unmapped
    addresses and writes that straddle a section boundary are rejected.

    The fixture is never modified and the original code and import models are
    never touched - globals are data only.
    """

    if not isinstance(fixture, dict):
        raise GlobalWriteError("fixture must be a dict")

    globals_map = fixture.get("globals")
    if globals_map is None:
        return
    if not isinstance(globals_map, Mapping):
        raise GlobalWriteError("fixture['globals'] must be a mapping")

    for key, value in globals_map.items():
        address = _parse_address(key, "globals key", err=GlobalWriteError)
        _require_u32(value, f"globals[{key!r}]", err=GlobalWriteError)

        try:
            section = image.section_at(address)
        except GlobalWriteError:
            raise
        except Exception as exc:
            raise GlobalWriteError(
                f"global address {_canonical_address(address)} is not mapped in any section"
            ) from exc
        if section is None:
            raise GlobalWriteError(
                f"global address {_canonical_address(address)} is not mapped in any section"
            )

        writable = bool(_section_field(section, "writable", default=False))
        executable = bool(_section_field(section, "executable", default=False))
        end = _section_field(section, "end", required=True)
        start = _section_field(section, "start", default=None)
        if start is None:
            start = _section_field(section, "va", default=None)

        if not writable:
            raise GlobalWriteError(
                f"global address {_canonical_address(address)} is not in a writable section"
            )
        if executable:
            raise GlobalWriteError(
                f"global address {_canonical_address(address)} is in an executable section"
            )
        if not isinstance(end, int) or isinstance(end, bool):
            raise GlobalWriteError("section end must be an integer")
        if start is not None and (not isinstance(start, int) or isinstance(start, bool)):
            raise GlobalWriteError("section start must be an integer")
        if start is not None and address < start:
            raise GlobalWriteError(
                f"global address {_canonical_address(address)} is outside its section"
            )
        if address + 4 > end:
            raise GlobalWriteError(
                f"global write at {_canonical_address(address)} straddles or leaves "
                f"the end of its section"
            )
