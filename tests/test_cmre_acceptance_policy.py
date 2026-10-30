"""Negative contracts for observable ABI boundaries and C evidence quality."""
from types import SimpleNamespace

import pytest

from cmre import difftest, emu, env, recover


@pytest.mark.parametrize("change", ["argument", "order", "missing"])
def test_abi_rejects_import_event_changes(change):
    events = [("c", "first", 1, (12,)), ("c", "second", 1, (34,))]
    altered = {"argument": [("c", "first", 1, (13,)), events[1]],
               "order": list(reversed(events)), "missing": events[:1]}[change]
    def normalized(es):
        return difftest._normalize(emu.Result("return", (), (0,) * 9, es, {}, set()), None, "abi")
    assert difftest._diff(normalized(events), normalized(altered))["field"] == "events"


def test_abi_allows_internal_access_reordering():
    call = ("c", "same", 1, (12,))
    def normalized(es):
        return difftest._normalize(emu.Result("return", (), (0,) * 9, es, {}, set()), None, "abi")
    assert not difftest._diff(normalized([("r", 16, 4, 1), call]),
                             normalized([call, ("w", 20, 4, 1)]))


@pytest.mark.parametrize("profile", ["abi", "semantic"])
@pytest.mark.parametrize("coverage,excluded", [(0.59, 0), (1.0, 3)])
def test_c_rejects_insufficient_evidence(tmp_path, monkeypatch, coverage, excluded, profile):
    monkeypatch.setattr(env, "C_DIR", tmp_path)
    monkeypatch.setattr(env, "RUNS", tmp_path / "runs")
    (tmp_path / "fn_004671b0.c").write_text("u32 cm_fn_004671b0(void) { return 0; }\n")
    ret = SimpleNamespace(va=0x004671B0, raw=b"\xc3", target=None)
    monkeypatch.setattr(recover.lift, "discover", lambda va: SimpleNamespace(va=va, region_end=va+1,
                                                                          insns={va: ret}, tables={}, name="test"))
    monkeypatch.setattr(recover.build, "build", lambda *args: SimpleNamespace(
        image=SimpleNamespace(return_bindings=(), jump_bindings=(), x87_bindings=())))
    monkeypatch.setattr(difftest, "run", lambda *args, **kwargs: {"mismatches": [], "inconclusive": 0,
                                                                 "coverage": coverage, "code_as_data_cases": excluded})
    report = recover.test_c(0x004671b0, {"cases": 100}, fresh=True, profile=profile)
    assert report["problems"] == [] and report["status"] == "fail"


@pytest.mark.parametrize("hints", [{"casess": 1}, {"cases": 0}, {"cases": True},
                                    {"min_coverage": float("nan")}, {"ecx": "pointer"},
                                    {"args": "ptr"}, {"globals": {"0x400000": []}},
                                    {"fixtures": [{}]}, {"struct_size": emu.ARENA_SIZE + 1}])
def test_malformed_hints_fail_before_running(hints):
    with pytest.raises(ValueError):
        difftest.validate_hints(hints)


def test_worker_hints_cannot_relax_policy():
    with pytest.raises(ValueError, match="policy"):
        difftest.validate_hints({"min_coverage": 0.1}, worker=True)
    difftest.validate_hints({"ecx": "struct", "struct_size": 128, "arg_count": 12}, worker=True)


def test_execution_identity_changes_with_actual_inventory_and_toolchain_bytes(tmp_path, monkeypatch):
    target = tmp_path / "game"
    inventory = tmp_path / "inventory"
    compiler = tmp_path / "compiler"
    for path in (target, inventory, compiler):
        path.write_bytes(b"one")

    monkeypatch.setattr(env, "TARGET", target)
    monkeypatch.setattr(env, "INVENTORY_SHA256", {inventory: "declared-pin"})
    monkeypatch.setattr(env, "toolchain_identity", lambda: {"gcc": {"version": "unchanged", "sha256": "old"}})
    monkeypatch.setattr(env, "tool", lambda name: str(compiler))
    baseline = recover.execution_identity()
    for path in (target, inventory, compiler):
        path.write_bytes(b"two")
        assert recover.execution_identity() != baseline
        path.write_bytes(b"one")


def test_lifted_request_cache_skips_build_but_fresh_rebuilds(tmp_path, monkeypatch):
    monkeypatch.setattr(env, "RUNS", tmp_path)
    report = recover.test_lifted(0x004671b0, {"cases": 20}, fresh=True)
    assert report["status"] == "pass"
    def forbidden(*args):
        raise RuntimeError("build invoked")
    monkeypatch.setattr(recover.build, "lift_and_build", forbidden)
    assert recover.test_lifted(0x004671b0, {"cases": 20}) == report
    with pytest.raises(RuntimeError, match="build invoked"):
        recover.test_lifted(0x004671b0, {"cases": 20}, fresh=True)
    artifact = tmp_path / "fn_004671b0" / report["key"] / "fn_004671b0.S"
    artifact.write_text("damaged")
    with pytest.raises(RuntimeError, match="build invoked"):
        recover.test_lifted(0x004671b0, {"cases": 20})


def test_c_report_hashes_actual_crlf_candidate_bytes(tmp_path, monkeypatch):
    monkeypatch.setattr(env, "RUNS", tmp_path / "runs")
    candidate = tmp_path / "candidate.c"
    candidate.write_bytes(b'#include "cmre.h"\r\nCM_THISCALL void *cm_fn_004671b0(void *self) { return (u8 *)self + 0x20; }\r\n')
    result = recover.test_c(0x004671b0, {"cases": 20}, fresh=True, source=candidate)
    assert result["status"] == "pass"
    assert result["source_sha256"] == env.digest(candidate)


@pytest.mark.parametrize("directive", ['#include "helper.h"', '#define H "helper.h"\n#include H',
                                       '#inc\\\nlude "helper.h"'])
def test_c_rejects_external_compiler_selected_includes_before_cache(tmp_path, monkeypatch, directive):
    monkeypatch.setattr(env, "RUNS", tmp_path / "runs")
    source = tmp_path / "candidate.c"
    (tmp_path / "helper.h").write_text("#define OFFSET 0x20\n")
    source.write_text(directive + '\n#include "cmre.h"\nCM_THISCALL void *cm_fn_004671b0(void *self) { return (u8 *)self + OFFSET; }\n')
    report = recover.test_c(0x004671b0, {"cases": 20}, source=source)
    assert report["status"] == "fail"
    assert "unsupported dependency" in report["problems"][0]
    assert not (tmp_path / "runs").exists()


def test_compiler_selected_support_header_changes_dependency_identity(tmp_path, monkeypatch):
    monkeypatch.setattr(env, "ROOT", tmp_path)
    include = tmp_path / "recovery/include"
    include.mkdir(parents=True)
    header = include / "helper.h"
    header.write_text("#define VALUE 1\n")
    source = tmp_path / "candidate.c"
    source.write_text('#include "helper.h"\nint answer(void) { return VALUE; }\n')
    first = recover.c_dependencies(source)
    header.write_text("#define VALUE 2\n")
    second = recover.c_dependencies(source)
    assert first["files"]["recovery/include/helper.h"] != second["files"]["recovery/include/helper.h"]
    assert first["preprocessed"] != second["preprocessed"]


def test_lifted_refresh_refuses_to_erase_existing_c_tier(tmp_path, monkeypatch):
    from cmre import ledger
    monkeypatch.setattr(env, "LEDGER_DIR", tmp_path)
    path = ledger.record_path(0x004671b0)
    path.write_text('{"lifted": {}, "c": {"source_sha256": "keep"}}\n')
    before = path.read_bytes()
    ok, report = recover.accept(0x004671b0, refresh=True)
    assert not ok and report["status"] == "has-c-tier"
    assert path.read_bytes() == before
