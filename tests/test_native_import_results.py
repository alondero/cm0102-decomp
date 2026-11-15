"""Actual OS import observations, never authored surrogate API results."""
import json
import struct
import uuid

import pytest

from cmre.import_observe import MemoryRead
from native_import_results import (ROOT, Unavailable, bounded_range, build_native_results,
                                   output_observations, observe_native_results, verify_caller)


def test_range_limit_preserves_requested_range_and_partial_error():
    requests = []

    def partial(address, size, purpose):
        requests.append((address, size))
        return MemoryRead(address, size, b"ab", purpose, "ReadProcessMemory:299")

    read = bounded_range(partial, 0x1000, 1000, "output", limit=8)
    assert requests == [(0x1000, 8)]
    assert read.requested_size == 1000 and read.data == b"ab"
    assert "observation-limit:8" in read.error
    assert "ReadProcessMemory:299" in read.error
    assert not read.complete


def test_invalid_range_never_calls_reader():
    with pytest.raises(ValueError):
        bounded_range(lambda *args: pytest.fail("must not read"), 0xfffffff0, 32, "invalid")


def test_readfile_short_error_and_overlapped_are_incomplete():
    def reader(address, size, purpose):
        return MemoryRead(address, size, struct.pack("<I", 2) if size == 4 else b"x" * size, purpose)

    for eax, overlapped, gap in [(1, 0, "short-read"), (0, 0, "api-error-or-pending"),
                                 (1, 0x4444, "overlapped-unsupported")]:
        reads, gaps = output_observations(reader, "ReadFile", [7, 0x2000, 8, 0x3000, overlapped], eax)
        assert gap in gaps
        assert {r.purpose for r in reads} >= {"ReadFile.byte-count", "ReadFile.buffer"}


def test_unreadable_bytecount_does_not_invent_count_or_data():
    def reader(address, size, purpose):
        return MemoryRead(address, size, b"", purpose, "ReadProcessMemory:299")

    reads, gaps = output_observations(reader, "ReadFile", [7, 0x2000, 8, 0x3000, 0], 1)
    assert "byte-count-unreadable" in gaps
    assert all(not read.complete and read.data == b"" for read in reads)


def test_heap_null_does_not_read_null_output():
    reads, gaps = output_observations(lambda *args: pytest.fail("null read"), "HeapAlloc", [1, 8, 32], 0)
    assert reads == ()
    assert "allocation-failed" in gaps


def test_abi_requires_native_u32_pushes_and_verified_iat():
    slot = 0x402000
    call = b"\xff\x15" + struct.pack("<I", slot)

    def evidence(pushes, import_name="KERNEL32.dll!HeapAlloc"):
        fields = dict(iat=slot, pushes=0x401000, call=0x401000 + len(pushes),
                      continuation=0x401000 + len(pushes) + 6)
        return verify_caller("HeapAlloc", fields, lambda a, n: pushes + call, {slot: import_name})

    assert evidence(b"\x6a\x20\x6a\x08\x50")["expected_endpoint_delta"] == 16
    with pytest.raises(ValueError, match="u32"):
        evidence(b"\x6a\x20\x6a\x08\x66\x50")  # push AX, not a u32 arg
    with pytest.raises(ValueError):
        evidence(b"\x6a\x20\x50")  # missing argument
    with pytest.raises(ValueError, match="PE"):
        evidence(b"\x6a\x20\x6a\x08\x50", "OTHER.dll!HeapAlloc")


@pytest.fixture(scope="module")
def native_results():
    directory = ROOT / "local/native-import-results" / uuid.uuid4().hex
    try:
        return build_native_results(directory)
    except Unavailable as exc:
        pytest.skip(str(exc))


@pytest.mark.parametrize("scenario", ["exact", "short", "limit", "error"])
def test_real_windows_results_at_actual_continuations(native_results, scenario):
    result = observe_native_results(native_results, scenario)
    assert result["child_returncode"] == 0
    assert result["cleanup"]["all_debugger_bytes_restored"]
    assert result["cleanup"]["detached"]
    protocol = result["protocol"]
    assert protocol["cleanup_complete"] and protocol["replayable"] is False
    assert "import_transcript" not in result
    records = protocol["invocations"]
    assert [r["import_name"] for r in records] == ["KERNEL32.dll!HeapAlloc", "KERNEL32.dll!ReadFile"]
    child = json.loads(result["stdout"])
    for record, name, argc in zip(records, ("HeapAlloc", "ReadFile"), (3, 5)):
        pre, post = record["entry"], record["return"]
        assert post["registers"]["esp"] == pre["registers"]["esp"] + 4 + 4 * argc
        assert post["registers"]["eip"] == record["return_site"]
        assert record["return_site"] == result["manifest"][name]["continuation"]
        assert pre["registers"]["eip"] == result["resolved_imports"][name]["target"]
        assert result["resolved_imports"][name]["mapped_module"].lower().endswith(".dll")
        assert record["replayable"] is False
    heap, read = records
    assert heap["return"]["registers"]["eax"] == child["heap_pointer"] != 0
    heap_buffer = next(r for r in heap["return"]["memory"] if r["purpose"] == "HeapAlloc.buffer")
    assert heap_buffer["address"] == child["heap_pointer"]
    assert heap_buffer["requested_size"] == 32
    assert heap_buffer["data_hex"] == child["heap_hex"]
    assert len(bytes.fromhex(heap_buffer["data_hex"])) == 32
    assert read["return"]["registers"]["eax"] == child["read_eax"]
    count = next(r for r in read["return"]["memory"] if r["purpose"] == "ReadFile.byte-count")
    assert struct.unpack("<I", bytes.fromhex(count["data_hex"]))[0] == child["read_count"]
    buffer = next(r for r in read["return"]["memory"] if r["purpose"] == "ReadFile.buffer")
    observed = bytes.fromhex(buffer["data_hex"])
    assert observed == bytes.fromhex(child["read_hex"])[:len(observed)]
    before = next(r for r in read["entry"]["memory"] if r["purpose"] == "ReadFile.byte-count-before")
    assert bytes.fromhex(before["data_hex"]) == struct.pack("<I", 0xdeadbeef)
    expected_count = 0 if scenario == "error" else len(bytes.fromhex(result["authored_payload_hex"]))
    assert child["read_count"] == expected_count
    if scenario == "exact":
        assert observed == bytes.fromhex(result["authored_payload_hex"])
        assert read["state"] == heap["state"] == "paired-endpoints"
    else:
        assert read["state"] == "returned-incomplete"
        assert read["gaps"]
    if scenario == "short":
        assert observed[:5] == b"short" and observed[5:] == b"\xa5" * 19
    elif scenario == "limit":
        assert buffer["requested_size"] == 128 and len(observed) == 64
        assert "observation-limit:64" in buffer["error"]
    elif scenario == "error":
        assert read["return"]["registers"]["eax"] == 0
        assert "api-error-or-pending" in read["gaps"]
    assert len(result["steps"]) >= 4
    assert all(step["trap_flag_after"] == 0 for step in result["steps"])
    expected_entries = {item["target"] for item in result["resolved_imports"].values()}
    assert {step["address"] for step in result["steps"] if step["rearmed"]} >= expected_entries
    for binding in result["resolved_imports"].values():
        restored = [item for item in result["cleanup"]["restored_bytes"]
                    if item["address"] == binding["target"]]
        assert len(restored) == 1 and restored[0]["original_hex"] == restored[0]["observed_hex"]


@pytest.mark.parametrize("point", ["step-prepared", "step-stop"])
def test_real_import_interrupted_step_restores_and_detaches(native_results, point):
    report = {}
    with pytest.raises(RuntimeError, match="injected failure"):
        observe_native_results(native_results, "exact", fail_at=point, report=report)
    assert report["cleanup"]["detached"] and report["cleanup"]["all_debugger_bytes_restored"]
    assert report["cleanup"]["interrupted_step"]["trap_flag_after"] == 0
    assert report["child_returncode"] == 0
    assert report["protocol"]["invocations"][0]["state"] == "incomplete"
    assert report["protocol"]["cleanup_complete"]
    assert json.loads(report["stdout"])["heap_pointer"] != 0
