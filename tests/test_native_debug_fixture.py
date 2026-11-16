"""Live asset-free Windows tests; binaries/evidence stay under ignored local/."""
import json
import struct
import subprocess
import uuid

import pytest

from native_debug_fixture import ROOT, Unavailable, _line_with_timeout, build_fixture, observe


@pytest.fixture(scope="module")
def native_fixture():
    # Do not use tmp_path: retained native evidence belongs in ignored local/.
    directory = ROOT / "local/native-debug-fixture" / uuid.uuid4().hex
    try:
        exe = build_fixture(directory)
    except Unavailable as exc:
        pytest.skip(str(exc))
    return exe


def run(native_fixture, scenario):
    result = observe(native_fixture, scenario)
    (native_fixture.parent / f"{scenario}.json").write_text(
        json.dumps(result, indent=2), encoding="utf-8")
    return result


def returns(result):
    return [event for event in result["events"] if event["kind"] == "return"]


@pytest.mark.parametrize("scenario,count", [
    ("ordinary", 1), ("nested", 3), ("shared", 3), ("threads", 4),
    ("unreadable", 1), ("thread-exit", 0),
])
def test_live_markers_and_cleanup(native_fixture, scenario, count):
    result = run(native_fixture, scenario)
    assert len(returns(result)) == count
    assert result["cleanup"] == {"restored": True, "detached": True}
    assert result["child_returncode"] == 0
    assert json.loads(result["stdout"]) == {"cleanup_ok": True}
    assert result["exceptions"] == []
    assert result["open_frames_at_done"] == []
    for event in returns(result):
        assert event["eax"] == event["token"] + event["depth"] + 17
        assert event["edx"] == 0x2468ACE0
        if scenario == "unreadable":
            assert event["output_status"] == "unreadable"
            assert event["output"] is None
        else:
            assert event["output_status"] == "read"
            assert struct.unpack("<I", bytes.fromhex(event["output"]))[0] == (
                event["token"] ^ 0x13579BDF)
    if scenario == "nested":
        assert [event["kind"] for event in result["events"]] == [
            "entry", "entry", "entry", "return", "return", "return"]
        assert len({event["esp"] for event in returns(result)}) == 3
    if scenario == "shared":
        assert len({event["caller"] for event in returns(result)}) == 1
    if scenario == "threads":
        assert len({event["tid"] for event in returns(result)}) == 2
        assert len(result["thread_exits"]) >= 2
        active, overlap = set(), False
        for event in result["events"]:
            key = event["tid"], event["esp"]
            if event["kind"] == "entry":
                active.add(key)
            else:
                active.remove(key)
            overlap |= len({tid for tid, _ in active}) == 2
        assert overlap
    if scenario == "thread-exit":
        abandoned = [frame for event in result["thread_exits"] for frame in event["open_frames"]]
        assert len(abandoned) == 1
        assert abandoned[0]["mode"] == ord("x")


def test_live_unhandled_fault(native_fixture):
    result = run(native_fixture, "fault")
    assert len(result["events"]) == 1
    assert result["events"][0]["kind"] == "entry"
    assert returns(result) == []
    faults = [event for event in result["exceptions"] if event["code"] == 0xC0000005]
    assert [event["first_chance"] for event in faults] == [1, 0]
    assert result["process_exit_code"] == 0xC0000005
    assert result["child_returncode"] != 0


def test_live_cleanup_on_driver_error(native_fixture):
    report = {}
    with pytest.raises(RuntimeError, match="injected failure"):
        observe(native_fixture, "ordinary", fail_at_done=True, report=report)
    assert report["cleanup"] == {"restored": True, "detached": True}
    assert report["child_returncode"] == 0
    assert json.loads(report["stdout"]) == {"cleanup_ok": True}


def test_external_breakpoint_build_contract(native_fixture):
    """Production observer can own ordinary NOP bytes, not fixture-owned INT3s."""
    exe = build_fixture(native_fixture.parent / "external", external_breakpoints=True)
    with pytest.raises(ValueError, match="matching authored fixture build receipt"):
        observe(exe, "ordinary")
    child = subprocess.Popen([str(exe)], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
    try:
        manifest = json.loads(_line_with_timeout(child.stdout, 5))
        raw = exe.read_bytes()
        pe = struct.unpack_from("<I", raw, 0x3c)[0]
        section_count = struct.unpack_from("<H", raw, pe + 6)[0]
        optional_size = struct.unpack_from("<H", raw, pe + 20)[0]
        base = struct.unpack_from("<I", raw, pe + 24 + 28)[0]

        def byte_at(va):
            rva = va - base
            for index in range(section_count):
                section = pe + 24 + optional_size + index * 40
                size, address, raw_size, offset = struct.unpack_from("<4I", raw, section + 8)
                if address <= rva < address + min(size, raw_size):
                    return raw[offset + rva - address]
            raise AssertionError(f"fixture VA not file-backed: {va:#x}")

        assert byte_at(manifest["entry"]) == 0x90
        assert byte_at(manifest["return"]) == 0x90
        assert byte_at(manifest["done"]) == 0xCC
        assert byte_at(manifest["probe"]) == 0x90
    finally:
        child.kill()  # still inert at its initial stdin gate, never debug-attached
        child.communicate(timeout=5)
