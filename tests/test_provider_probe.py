"""Asset free tests for :mod:`cmre.provider_probe`.

The tests use tiny synthetic fixtures and a fake image; no game assets, no
emulator and no harness imports are involved.
"""

from __future__ import annotations

import importlib.util
import pathlib
import sys

import pytest

_ROOT = pathlib.Path(__file__).resolve().parents[1]
_MODULE_PATH = _ROOT / "cmre" / "provider_probe.py"


def _load_module():
    module = None
    try:
        from cmre import provider_probe as module  # type: ignore
    except Exception:
        module = None
    if module is not None:
        try:
            if pathlib.Path(module.__file__).resolve() == _MODULE_PATH.resolve():
                return module
        except Exception:
            pass
    spec = importlib.util.spec_from_file_location("provider_probe", _MODULE_PATH)
    loaded = importlib.util.module_from_spec(spec)
    sys.modules["provider_probe"] = loaded
    spec.loader.exec_module(loaded)
    return loaded


provider_probe = _load_module()

PatchError = provider_probe.PatchError
RequestError = provider_probe.RequestError
GlobalWriteError = provider_probe.GlobalWriteError


# fixtures


def make_base():
    return {
        "import_seed": {"seed_id": 7, "thunks": [1, 2, 3]},
        "import_transcript": {"calls": []},
        "arena_template": {"origin": "0x1000", "size": 4096},
        "regs": {
            "eax": 0,
            "ecx": 0,
            "edx": 0,
            "ebx": 0,
            "esp": 0,
            "ebp": 0,
            "esi": 0,
            "edi": 0,
            "eflags": 0,
        },
        "stack": [0, 0, 0, 0],
        "arena_writes": {"0x1000": 1},
        "globals": {"0x2000": 2},
        "notes": "keep me",
    }


class FakeSection:
    def __init__(self, start, end, writable=True, executable=False):
        self.start = start
        self.end = end
        self.writable = writable
        self.executable = executable


class FakeImage:
    def __init__(self, sections, raise_on_missing=False):
        self.sections = list(sections)
        self.raise_on_missing = raise_on_missing

    def section_at(self, address):
        for section in self.sections:
            if section.start <= address < section.end:
                return section
        if self.raise_on_missing:
            raise LookupError(f"no section at 0x{address:x}")
        return None


# apply_patch


def test_apply_patch_rejects_unknown_patch_key():
    base = make_base()
    with pytest.raises(PatchError):
        provider_probe.apply_patch(base, {"memory": {"0x1000": 1}})
    assert base == make_base()


def test_apply_patch_refuses_import_model_changes():
    for forbidden in ("import_seed", "import_transcript"):
        with pytest.raises(PatchError):
            provider_probe.apply_patch(make_base(), {forbidden: {"calls": ["x"]}})


def test_apply_patch_rejects_unknown_register():
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"regs": {"eip": 1}})


def test_apply_patch_register_values_must_be_u32_non_bool():
    for bad in (True, False, -1, 0x1_0000_0000, 1.5, "1", None):
        with pytest.raises(PatchError):
            provider_probe.apply_patch(make_base(), {"regs": {"eax": bad}})


def test_apply_patch_merges_registers_and_keeps_others():
    base = make_base()
    patched = provider_probe.apply_patch(base, {"regs": {"eax": 0xDEADBEEF}})
    assert patched["regs"]["eax"] == 0xDEADBEEF
    assert patched["regs"]["ecx"] == 0
    assert base["regs"]["eax"] == 0


def test_apply_patch_stack_replaces_existing_stack():
    base = make_base()
    patched = provider_probe.apply_patch(base, {"stack": [1, 2, 3]})
    assert patched["stack"] == [1, 2, 3]
    assert base["stack"] == [0, 0, 0, 0]


def test_apply_patch_stack_bounds_and_types():
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"stack": list(range(65))})
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"stack": [True]})
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"stack": [0x1_0000_0000]})
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"stack": "1234"})
    ok = provider_probe.apply_patch(make_base(), {"stack": list(range(64))})
    assert len(ok["stack"]) == 64


def test_apply_patch_merges_address_mappings_and_serialises_hex():
    base = make_base()
    patched = provider_probe.apply_patch(
        base,
        {"arena_writes": {"4096": 7, "0x1008": 9}, "globals": {"0x2004": 5}},
    )
    assert patched["arena_writes"] == {"0x1000": 7, "0x1008": 9}
    assert patched["globals"] == {"0x2000": 2, "0x2004": 5}
    assert base["arena_writes"] == {"0x1000": 1}
    assert base["globals"] == {"0x2000": 2}


def test_apply_patch_rejects_conflicting_address_aliases():
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"globals": {"0x2000": 1, "8192": 2}})
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"arena_writes": {"0x10": 1, "16": 2}})


def test_apply_patch_allows_identical_address_aliases():
    patched = provider_probe.apply_patch(make_base(), {"globals": {"0x2000": 1, "8192": 1}})
    assert patched["globals"] == {"0x2000": 1}


def test_apply_patch_rejects_malformed_addresses_and_values():
    for bad in ("0xzz", "", "  ", "010x1", None, True):
        with pytest.raises(PatchError):
            provider_probe.apply_patch(make_base(), {"globals": {bad: 1}})
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"globals": {"0x2000": True}})
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"globals": {"0x2000": -1}})


def test_apply_patch_preserves_prefix_and_import_model():
    base = make_base()
    patched = provider_probe.apply_patch(base, {"regs": {"eax": 1}})
    assert patched["import_seed"] == base["import_seed"]
    assert patched["import_transcript"] == base["import_transcript"]
    assert patched["arena_template"] == base["arena_template"]
    assert patched["notes"] == "keep me"
    assert patched["stack"] == base["stack"]
    assert patched["arena_writes"] == base["arena_writes"]
    assert patched["globals"] == base["globals"]


def test_apply_patch_clones_inputs():
    base = make_base()
    patch = {"regs": {"eax": 5}, "globals": {"0x2000": 9}}
    snapshot = make_base()
    patched = provider_probe.apply_patch(base, patch)

    assert patched is not base
    assert base == snapshot
    assert patch == {"regs": {"eax": 5}, "globals": {"0x2000": 9}}

    patched["regs"]["eax"] = 99
    patched["globals"]["0x2000"] = 99
    patched["import_seed"]["seed_id"] = 99
    assert base == snapshot
    assert patch["regs"]["eax"] == 5
    assert patch["globals"]["0x2000"] == 9


def test_apply_patch_mapping_entry_limit():
    # the base fixture already holds one global, so 4095 patched entries are
    # the largest accepted patch for a 4096 entry mapping.
    ok = provider_probe.apply_patch(
        make_base(), {"globals": {f"0x{0x3000 + i * 4:x}": i for i in range(4095)}}
    )
    assert len(ok["globals"]) == 4096
    with pytest.raises(PatchError):
        provider_probe.apply_patch(
            make_base(), {"globals": {f"0x{0x3000 + i * 4:x}": i for i in range(4096)}}
        )


def test_apply_patch_json_size_budget():
    base = make_base()
    base["blob"] = "x" * (1024 * 1024 + 16)
    with pytest.raises(PatchError):
        provider_probe.apply_patch(base, {"regs": {"eax": 1}})


def test_apply_patch_validate_callback_runs_after_checks():
    seen = []

    def callback(final):
        seen.append(final)
        return True

    patched = provider_probe.apply_patch(make_base(), {"regs": {"eax": 3}}, validate=callback)
    assert len(seen) == 1
    assert seen[0]["regs"]["eax"] == 3
    assert seen[0] is patched


def test_apply_patch_validate_callback_not_called_when_checks_fail():
    seen = []

    def callback(final):
        seen.append(final)
        return True

    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"regs": {"eax": True}}, validate=callback)
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"nope": 1}, validate=callback)
    assert seen == []


def test_apply_patch_validate_callback_can_reject():
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), {"regs": {"eax": 1}}, validate=lambda final: False)


def test_apply_patch_rejects_non_dict_inputs():
    with pytest.raises(PatchError):
        provider_probe.apply_patch([], {})
    with pytest.raises(PatchError):
        provider_probe.apply_patch(make_base(), [])


# validate_request


ALLOWED = [0x401000, 0x401100]


def make_request(**overrides):
    request = {
        "function": "0x401000",
        "patch": {},
        "budget": 1000,
        "shapes": {"0x401000": {"ecx": False, "stack_words": 0}},
        "name": "probe_1",
    }
    request.update(overrides)
    return request


def test_validate_request_normalises_function_and_shape_keys():
    result = provider_probe.validate_request(make_request(), allowed_functions=ALLOWED)
    assert result["function"] == 0x401000
    assert result["shapes"] == {"4198400": {"ecx": False, "stack_words": 0}}
    assert result["budget"] == 1000
    assert result["name"] == "probe_1"
    assert result["patch"] == {}


def test_validate_request_rejects_unknown_and_missing_keys():
    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(extra=1), allowed_functions=ALLOWED
        )
    request = make_request()
    del request["name"]
    with pytest.raises(RequestError):
        provider_probe.validate_request(request, allowed_functions=ALLOWED)
    request = make_request()
    del request["shapes"]
    with pytest.raises(RequestError):
        provider_probe.validate_request(request, allowed_functions=ALLOWED)


def test_validate_request_function_must_be_allowed():
    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(function="0x402000"), allowed_functions=ALLOWED
        )


def test_validate_request_budget_rules():
    request = make_request()
    del request["budget"]
    defaulted = provider_probe.validate_request(request, allowed_functions=ALLOWED)
    assert defaulted["budget"] == 200000

    for bad in (0, -1, True, False, "100", 1.5, None):
        with pytest.raises(RequestError):
            provider_probe.validate_request(make_request(budget=bad), allowed_functions=ALLOWED)

    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(budget=200001), allowed_functions=ALLOWED
        )
    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(budget=100), allowed_functions=ALLOWED, max_budget=10
        )


def test_validate_request_shape_arity():
    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(shapes={"0x401000": {"ecx": False}}), allowed_functions=ALLOWED
        )
    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(
                shapes={"0x401000": {"ecx": False, "stack_words": 0, "edx": True}}
            ),
            allowed_functions=ALLOWED,
        )


def test_validate_request_shape_value_rules():
    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(shapes={"0x401000": {"ecx": 1, "stack_words": 0}}),
            allowed_functions=ALLOWED,
        )
    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(shapes={"0x401000": {"ecx": False, "stack_words": 65}}),
            allowed_functions=ALLOWED,
        )
    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(shapes={"0x401000": {"ecx": False, "stack_words": True}}),
            allowed_functions=ALLOWED,
        )


def test_validate_request_shape_functions_must_be_allowed():
    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(
                shapes={
                    "0x401000": {"ecx": False, "stack_words": 0},
                    "0x402000": {"ecx": False, "stack_words": 0},
                }
            ),
            allowed_functions=ALLOWED,
        )


def test_validate_request_requires_explicit_root_shape():
    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(shapes={"0x401100": {"ecx": False, "stack_words": 0}}),
            allowed_functions=ALLOWED,
        )


def test_validate_request_rejects_conflicting_shape_aliases():
    with pytest.raises(RequestError):
        provider_probe.validate_request(
            make_request(
                shapes={
                    "0x401000": {"ecx": False, "stack_words": 0},
                    "4198400": {"ecx": True, "stack_words": 0},
                }
            ),
            allowed_functions=ALLOWED,
        )


def test_validate_request_name_regex():
    for good in ("a", "probe_1", "0abc", "a" * 64):
        result = provider_probe.validate_request(
            make_request(name=good), allowed_functions=ALLOWED
        )
        assert result["name"] == good
    for bad in ("", "Probe", "-abc", "_abc", "a" * 65, "a.b", "a b", 1, None):
        with pytest.raises(RequestError):
            provider_probe.validate_request(make_request(name=bad), allowed_functions=ALLOWED)


def test_validate_request_validates_patch():
    result = provider_probe.validate_request(
        make_request(patch={"regs": {"eax": 7}, "globals": {"0x2000": 3}}),
        allowed_functions=ALLOWED,
    )
    assert result["patch"]["regs"] == {"eax": 7}
    assert result["patch"]["globals"] == {"0x2000": 3}

    with pytest.raises((RequestError, PatchError)):
        provider_probe.validate_request(
            make_request(patch={"regs": {"nope": 1}}), allowed_functions=ALLOWED
        )
    with pytest.raises((RequestError, PatchError)):
        provider_probe.validate_request(
            make_request(patch={"import_seed": {}}), allowed_functions=ALLOWED
        )


# validate_global_writes


def test_validate_global_writes_accepts_data_section():
    fixture = {"globals": {"0x1000": 1, "0x100c": 2}}
    image = FakeImage([FakeSection(0x1000, 0x1010)])
    assert provider_probe.validate_global_writes(fixture, image) is None


def test_validate_global_writes_rejects_byte_overlap():
    image = FakeImage([FakeSection(0x1000, 0x1010)])
    with pytest.raises(GlobalWriteError):
        provider_probe.validate_global_writes({"globals": {"0x100d": 1}}, image)
    # exactly filling the section is fine
    provider_probe.validate_global_writes({"globals": {"0x100c": 1}}, image)


def test_validate_global_writes_rejects_straddling_two_sections():
    image = FakeImage([FakeSection(0x1000, 0x1008), FakeSection(0x1008, 0x1010)])
    with pytest.raises(GlobalWriteError):
        provider_probe.validate_global_writes({"globals": {"0x1006": 1}}, image)


def test_validate_global_writes_rejects_code_and_readonly_sections():
    code = FakeImage([FakeSection(0x1000, 0x1010, writable=True, executable=True)])
    with pytest.raises(GlobalWriteError):
        provider_probe.validate_global_writes({"globals": {"0x1000": 1}}, code)

    readonly = FakeImage([FakeSection(0x1000, 0x1010, writable=False)])
    with pytest.raises(GlobalWriteError):
        provider_probe.validate_global_writes({"globals": {"0x1000": 1}}, readonly)


def test_validate_global_writes_rejects_unmapped_addresses():
    image = FakeImage([FakeSection(0x1000, 0x1010)])
    with pytest.raises(GlobalWriteError):
        provider_probe.validate_global_writes({"globals": {"0x2000": 1}}, image)

    raising = FakeImage([FakeSection(0x1000, 0x1010)], raise_on_missing=True)
    with pytest.raises(GlobalWriteError):
        provider_probe.validate_global_writes({"globals": {"0x2000": 1}}, raising)


def test_validate_global_writes_rejects_malformed_addresses_and_values():
    image = FakeImage([FakeSection(0x1000, 0x1010)])
    with pytest.raises(GlobalWriteError):
        provider_probe.validate_global_writes({"globals": {"nope": 1}}, image)
    with pytest.raises(GlobalWriteError):
        provider_probe.validate_global_writes({"globals": {"0x1000": True}}, image)
    with pytest.raises(GlobalWriteError):
        provider_probe.validate_global_writes({"globals": {"0x1000": -5}}, image)


def test_validate_global_writes_without_globals_is_noop():
    image = FakeImage([FakeSection(0x1000, 0x1010)])
    assert provider_probe.validate_global_writes({}, image) is None
    assert provider_probe.validate_global_writes({"globals": {}}, image) is None


def test_validate_global_writes_does_not_mutate_fixture():
    fixture = {"globals": {"0x1000": 1}, "import_transcript": {"calls": []}}
    snapshot = {"globals": {"0x1000": 1}, "import_transcript": {"calls": []}}
    image = FakeImage([FakeSection(0x1000, 0x1010)])
    provider_probe.validate_global_writes(fixture, image)
    assert fixture == snapshot


# module hygiene


def test_module_only_uses_stdlib_imports():
    for value in vars(provider_probe).values():
        name = getattr(value, "__name__", None)
        if isinstance(value, type(sys)) and name:
            assert name in sys.stdlib_module_names, name


def test_module_exposes_no_executor_entry_points():
    for forbidden in ("run", "execute", "emulate", "step", "main"):
        assert not hasattr(provider_probe, forbidden)
