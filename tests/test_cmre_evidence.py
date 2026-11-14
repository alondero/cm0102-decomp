"""Reusable navigation packets must expire rather than present stale evidence."""
import json
from types import SimpleNamespace
from pathlib import Path

import pytest

from cmre import env


@pytest.mark.parametrize('location', ['ledger', 'function'])
def test_legacy_provenance_names_frozen_source_without_claiming_a_c_binding(tmp_path, monkeypatch, location):
    from cmre import drafts, evidence, inventory, ledger, lift

    source = tmp_path / 'cm_date.c'
    source.write_text('authored legacy reconstruction')
    adapter = tmp_path / 'cm_date.S'
    adapter.write_text('authored entry adapter')
    frozen = tmp_path / 'legacy.json'
    references = {'source': str(source), 'entry_source': str(adapter)}
    record = {'symbol': 'cm_date_assign', **(references if location == 'function' else {})}
    frozen.write_text(json.dumps(references if location == 'ledger' else {'functions': [record]}))
    ghidra = tmp_path / 'ghidra'
    ghidra.mkdir()
    (ghidra / 'fn_00401100.c').write_text('unvalidated draft')
    monkeypatch.setattr(drafts, 'DRAFTS', ghidra)
    monkeypatch.setattr(drafts, 'accepted_prototype', lambda va: None)
    monkeypatch.setattr(drafts, 'prototype', lambda va: 'cm_fn_00401100(void)')
    monkeypatch.setattr(inventory, 'load', lambda: SimpleNamespace(functions={0x401100: object()}))
    monkeypatch.setattr(lift, 'discover', lambda va: SimpleNamespace(callees=[0x401100]))
    monkeypatch.setattr(ledger, 'legacy_entries', lambda: [SimpleNamespace(
        va=0x401100, source=str(frozen), record=record)])
    text = evidence.provenance(0x401000)
    assert 'frozen legacy recovery' in text and 'cm_date_assign' in text
    assert 'cm_date.c' in text and env.digest(source) in text
    assert 'cm_date.S' in text and env.digest(adapter) in text
    assert 'binding not inferred' in text
    assert 'unvalidated Ghidra' not in text


@pytest.mark.parametrize('location', ['ledger', 'function'])
def test_packet_dependencies_include_authored_legacy_source_and_header(tmp_path, monkeypatch, location):
    from cmre import drafts, evidence, lift

    registry, frozen = tmp_path / 'registry.json', tmp_path / 'legacy.json'
    source, header = tmp_path / 'cm_date.c', tmp_path / 'cm_date.h'
    adapter = tmp_path / 'cm_date.S'
    source.write_text('authored reconstruction')
    header.write_text('legacy interface')
    adapter.write_text('legacy entry adapter')
    references = {'source': str(source), 'header': str(header), 'entry_source': str(adapter)}
    frozen.write_text(json.dumps(references if location == 'ledger' else {'functions': [references]}))
    registry.write_text(json.dumps({'current': [str(frozen)]}))
    monkeypatch.setattr(env, 'REGISTRY', registry)
    monkeypatch.setattr(drafts, 'DRAFTS', tmp_path / 'no-drafts')
    monkeypatch.setattr(lift, 'discover', lambda va: SimpleNamespace(callees=[]))
    paths = evidence.dependencies(0x401000, None)
    assert source in paths and header in paths and adapter in paths and frozen in paths


def test_packet_reuses_rendering_but_invalidates_changed_or_new_dependencies(tmp_path, monkeypatch):
    from cmre import evidence

    source, absent = tmp_path / "source.c", tmp_path / "callee.c"
    source.write_text("old")
    monkeypatch.setattr(evidence, "dependencies", lambda va, peer: [source, absent])
    monkeypatch.setattr(evidence, "packet_root", lambda: tmp_path / "packets")
    calls = []

    def render(va, peer):
        calls.append(va)
        return "listing: " + source.read_text()

    monkeypatch.setattr(evidence, "render", render)
    first, reused = evidence.packet(0x401000)
    assert not reused
    assert evidence.packet(0x401000) == (first, True)
    assert len(calls) == 1
    source.write_text("new")
    second, reused = evidence.packet(0x401000)
    assert not reused and second != first
    assert first.read_text() == "listing: old"
    absent.write_text("new callee definition")
    assert evidence.packet(0x401000)[1] is False
    assert len(calls) == 3


def test_modified_packet_is_regenerated(tmp_path, monkeypatch):
    from cmre import evidence

    monkeypatch.setattr(evidence, "dependencies", lambda va, peer: [])
    monkeypatch.setattr(evidence, "packet_root", lambda: tmp_path)
    monkeypatch.setattr(evidence, "render", lambda va, peer: "original instructions")
    path, _ = evidence.packet(0x401000)
    path.write_text("altered")
    assert evidence.packet(0x401000) == (path, False)
    assert path.read_text() == "original instructions"


def test_manifest_detects_renderer_changes(tmp_path, monkeypatch):
    from cmre import evidence

    implementation = tmp_path / "renderer.py"
    implementation.write_text("version1")
    monkeypatch.setattr(evidence, "dependencies", lambda va, peer: [implementation])
    monkeypatch.setattr(evidence, "packet_root", lambda: tmp_path / "packets")
    monkeypatch.setattr(evidence, "render", lambda va, peer: "packet")
    path, _ = evidence.packet(0x401000)
    manifest = json.loads(path.with_suffix(".json").read_text())
    assert manifest["identity"]["target"] == env.TARGET_SHA256
    assert manifest["identity"]["address_convention"] == env.ADDRESS_CONVENTION
    assert manifest["identity"]["dependencies"][str(implementation)] == env.digest(implementation)
    implementation.write_text("version2")
    assert evidence.packet(0x401000)[0] != path


def test_caller_windows_preserve_partial_register_and_return_instructions(monkeypatch):
    from cmre import evidence, image, semantic

    img = object.__new__(image.Image)
    img.sections = [image.Section(".text", 0x401000, 13,
                                  bytes.fromhex("e8fb0f000066b8010050c39090"), 0x60000020)]
    monkeypatch.setattr(image, "primary", lambda: img)
    monkeypatch.setattr(semantic, "_call_sites", lambda: {0x402000: (0x401000,)})
    text = evidence.caller_windows(0x402000)
    assert "00401005" in text and "mov ax, 1" in text
    assert "push eax" in text and "ret" in text
    assert "indirect" in text and "not a return-width proof" in text


def test_invalid_call_site_is_explicit_and_never_disassembled_as_evidence(monkeypatch):
    from cmre import evidence, image, semantic

    img = object.__new__(image.Image)
    img.sections = [image.Section(".text", 0x401000, 5, b"\x90" * 5, 0x60000020)]
    monkeypatch.setattr(image, "primary", lambda: img)
    monkeypatch.setattr(semantic, "_call_sites", lambda: {0x402000: (0x401000,)})
    assert "UNRESOLVED" in evidence.caller_windows(0x402000)


def test_peer_packet_requires_matching_instruction_shape(monkeypatch):
    from cmre import evidence, families

    monkeypatch.setattr(families, "delta", lambda va, peer: (False, "different topology"))
    with pytest.raises(ValueError, match="instruction shape"):
        evidence.render(0x401000, 0x402000)


def test_packet_refuses_inputs_changed_during_rendering(tmp_path, monkeypatch):
    from cmre import evidence

    source = tmp_path / "source.c"
    source.write_text("old")
    monkeypatch.setattr(evidence, "dependencies", lambda va, peer: [source])
    monkeypatch.setattr(evidence, "packet_root", lambda: tmp_path / "packets")

    def render(va, peer):
        source.write_text("changed during rendering")
        return "stale"

    monkeypatch.setattr(evidence, "render", render)
    with pytest.raises(ValueError, match="changed during rendering"):
        evidence.packet(0x401000)
    assert not list((tmp_path / "packets").glob("*.txt"))


def test_packet_rejects_changed_target_before_rendering(tmp_path, monkeypatch):
    from cmre import evidence

    target = tmp_path / "target.exe"
    target.write_bytes(b"wrong target")
    monkeypatch.setattr(env, "TARGET", target)
    monkeypatch.setattr(evidence, "dependencies", lambda va, peer: [target])
    monkeypatch.setattr(evidence, "render", lambda va, peer: pytest.fail("rendered wrong binary"))
    with pytest.raises(ValueError, match="Pinned target hash mismatch"):
        evidence.packet(0x401000)


def test_brief_packet_cli_prints_paths_without_repeating_the_artifact(tmp_path, monkeypatch, capsys):
    from cmre import cli, drafts, evidence

    seen = []
    path = tmp_path / "packet.txt"

    def prepare(va, peer):
        seen.append((va, peer))
        return path, True

    monkeypatch.setattr(evidence, "packet", prepare)
    monkeypatch.setattr(drafts, "brief", lambda *args, **kwargs: pytest.fail("listing repeated"))
    assert cli.main(["brief", "fn_00401000", "--packet", "--peer", "fn_00402000"]) == 0
    assert seen == [(0x401000, 0x402000)]
    output = capsys.readouterr().out
    assert "packet reused:" in output and len(output.splitlines()) == 3


def test_plain_brief_rejects_peer_and_packet_rejects_no_asm(capsys):
    from cmre import cli

    assert cli.main(["brief", "fn_00401000", "--peer", "fn_00402000"]) == 2
    assert "--peer requires --packet" in capsys.readouterr().err
    assert cli.main(["brief", "fn_00401000", "--packet", "--no-asm"]) == 2
    assert "retains the original listing" in capsys.readouterr().err


def test_invalid_manifest_json_shape_is_regenerated(tmp_path, monkeypatch):
    from cmre import evidence

    monkeypatch.setattr(evidence, "dependencies", lambda va, peer: [])
    monkeypatch.setattr(evidence, "packet_root", lambda: tmp_path)
    monkeypatch.setattr(evidence, "render", lambda va, peer: "original")
    path, _ = evidence.packet(0x401000)
    path.with_suffix(".json").write_text("null")
    assert evidence.packet(0x401000) == (path, False)


def test_family_packet_retains_member_jump_tables_and_embedded_data(monkeypatch):
    from cmre import evidence, drafts, families, lift

    monkeypatch.setattr(families, "delta", lambda left, right: (True, "member instruction delta"))
    monkeypatch.setattr(evidence, "packet", lambda va, peer=None: (Path("representative.txt"), True))
    monkeypatch.setattr(drafts, "brief", lambda va, asm: "member contracts\n")
    monkeypatch.setattr(evidence, "provenance", lambda va: "")
    monkeypatch.setattr(evidence, "caller_windows", lambda va: "")
    monkeypatch.setattr(lift, "discover", lambda va: SimpleNamespace(
        tables={1: SimpleNamespace(va=0x401080, targets=[0x401010, 0x401030])},
        data=[SimpleNamespace(va=0x401090, raw=b"\x00\xff\x02")]))
    text = evidence.render(0x401000, 0x402000)
    assert "table 00401080 -> 00401010 00401030" in text
    assert "data 00401090 00ff02" in text
