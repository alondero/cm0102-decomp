"""Authored native file/time observations, not original CRT or game parity."""
import json
import struct
import uuid

import pytest

from cmre.import_observe import MemoryRead
from native_crt_import_results import (
    ROOT, NativeCrtResultsProfile, Unavailable, build_native_crt_results, classify_output,
    observe_native_crt_results, output_ranges, verify_caller,
)


def test_fixed_ranges_do_not_follow_native_handles_or_unknown_prototypes():
    assert output_ranges("FindFirstFileA", [0x1000, 0x2000]) == ((0x2000, 320),)
    assert output_ranges("FindNextFileA", [0xffffffff, 0x2000]) == ((0x2000, 320),)
    assert output_ranges("FileTimeToLocalFileTime", [0x1000, 0x2000]) == ((0x2000, 8),)
    assert output_ranges("FileTimeToSystemTime", [0x1000, 0x2000]) == ((0x2000, 16),)
    for name, args in [("Other", [1, 2]), ("FindNextFileA", [1]),
                       ("FindNextFileA", [1, 0]), ("FindNextFileA", [1, 0xffffff00])]:
        with pytest.raises(ValueError):
            output_ranges(name, args)


def test_find_success_does_not_define_padding_reserved_or_filename_tail():
    before = b"\xa5" * 320
    after = bytearray(before)
    after[44:50] = b"a.bin\0"
    info = classify_output("FindFirstFileA", 123, before, bytes(after))
    assert info["defined_ranges"] == [[0, 36], [44, 50]]
    assert info["changed_offsets"] == list(range(44, 50))
    assert info["unchanged_count"] == 314
    assert info["replayable"] is False
    assert info["before_hex"] == before.hex() and info["after_hex"] == after.hex()


@pytest.mark.parametrize("name,eax,size", [
    ("FindFirstFileA", 0xffffffff, 320), ("FindNextFileA", 0, 320),
    ("FileTimeToLocalFileTime", 0, 8), ("FileTimeToSystemTime", 0, 16),
])
def test_failure_bytes_remain_raw_undefined_even_when_unchanged(name, eax, size):
    info = classify_output(name, eax, b"\xa5" * size, b"\xa5" * size)
    assert info["defined_ranges"] == []
    assert info["output_status"] == "undefined-on-failure"
    assert info["changed_offsets"] == [] and info["unchanged_count"] == size
    assert info["scalar_result"] == eax


def test_partial_or_unterminated_find_output_fails_closed():
    with pytest.raises(ValueError):
        classify_output("FindNextFileA", 1, b"x" * 320, b"x" * 319)
    info = classify_output("FindNextFileA", 1, b"x" * 320, b"x" * 320)
    assert info["defined_ranges"] == [[0, 36]]
    assert "filename-not-terminated" in info["gaps"]


def test_time_output_definitions_depend_on_actual_scalar_not_expected_timezone():
    for name, size in [("FileTimeToLocalFileTime", 8), ("FileTimeToSystemTime", 16)]:
        info = classify_output(name, 1, b"\xa5" * size, b"\0" * size)
        assert info["defined_ranges"] == [[0, size]]
        assert info["scalar_result"] == 1


def test_partial_native_read_retained_without_classification():
    profile = NativeCrtResultsProfile.__new__(NativeCrtResultsProfile)
    profile.result = {"output_classification": {}}
    fact = dict(name="FindNextFileA", args=[123, 0x2000],
                before=MemoryRead(0x2000, 320, b"\xa5" * 320, "before").report())
    ctx = type("Context", (), {"Eax": 0})()
    reads, gaps = profile.outputs(fact, ctx, lambda a, n, p:
                                  MemoryRead(a, n, b"\xa5", p, "ReadProcessMemory:299"))
    assert reads[0].requested_size == 320 and reads[0].data == b"\xa5"
    assert not reads[0].complete
    assert "output-classification-unavailable-partial-read" in gaps
    assert profile.result["output_classification"] == {}


def test_native_caller_requires_two_u32_pushes_and_real_import_binding():
    slot, start = 0x402000, 0x401000
    raw = b"\x68\x00\x20\x40\x00\x50\xff\x15" + struct.pack("<I", slot)
    fields = dict(iat=slot, pushes=start, call=start + 6, continuation=start + 12)
    assert verify_caller("FindFirstFileA", fields, lambda a, n: raw,
                         {slot: "KERNEL32.dll!FindFirstFileA"})["expected_endpoint_delta"] == 12
    with pytest.raises(ValueError):
        verify_caller("FindFirstFileA", fields, lambda a, n: raw, {slot: "OTHER!FindFirstFileA"})
    with pytest.raises(ValueError):
        verify_caller("FindFirstFileA", fields, lambda a, n: b"\x66\x50" + raw[2:],
                      {slot: "KERNEL32.dll!FindFirstFileA"})


@pytest.fixture(scope="module")
def native_crt_results():
    directory = ROOT / "local/native-crt-import-results" / uuid.uuid4().hex
    try:
        return build_native_crt_results(directory)
    except Unavailable as exc:
        pytest.skip(f"unavailable: {exc}")


@pytest.mark.parametrize("scenario", ["success", "exhausted", "error"])
def test_real_file_and_time_api_results(native_crt_results, scenario):
    result = observe_native_crt_results(native_crt_results, scenario)
    assert result["child_returncode"] == 0
    assert result["cleanup"]["detached"] and result["cleanup"]["all_debugger_bytes_restored"]
    assert result["protocol"]["cleanup_complete"] and not result["protocol"]["replayable"]
    assert "import_transcript" not in result
    child = json.loads(result["stdout"])
    names = ["FindFirstFileA", "FindNextFileA", "FileTimeToLocalFileTime", "FileTimeToSystemTime"]
    records = result["protocol"]["invocations"]
    assert [r["import_name"] for r in records] == ["KERNEL32.dll!" + n for n in names]
    for record, name in zip(records, names):
        pre, post = record["entry"], record["return"]
        assert not record["replayable"]
        assert post["registers"]["esp"] == pre["registers"]["esp"] + 12
        assert post["registers"]["eip"] == result["manifest"][name]["continuation"]
        assert pre["registers"]["eip"] == result["resolved_imports"][name]["target"]
        scalar = post["registers"]["eax"]
        assert scalar == child[name]["eax"]
        before = next(r for r in pre["memory"] if r["purpose"] == name + ".output-before")
        after = next(r for r in post["memory"] if r["purpose"] == name + ".output-after")
        assert before["complete"] and after["complete"]
        assert after["data_hex"] == child[name]["hex"]
        info = classify_output(name, scalar, bytes.fromhex(before["data_hex"]),
                               bytes.fromhex(after["data_hex"]))
        assert info == result["output_classification"][name]
        assert record["state"] == "returned-incomplete"  # explicit observation-only gap
    first, next_record, local, system = records
    assert (first["return"]["registers"]["eax"] != 0xffffffff) == (scenario != "error")
    assert bool(next_record["return"]["registers"]["eax"]) == (scenario == "success")
    # Host timezone is deliberately not an expected-value oracle.
    assert bool(system["return"]["registers"]["eax"]) == (scenario != "error")
    first_input = next(r for r in first["entry"]["memory"] if r["purpose"].endswith(".input"))
    assert bytes.fromhex(first_input["data_hex"]) == (
        b"missingx-*.bin\0" if scenario == "error" else b"authored-*.bin\0")
    first_before = next(r for r in first["entry"]["memory"] if r["purpose"].endswith(".output-before"))
    assert bytes.fromhex(first_before["data_hex"]) == b"\xa5" * 320
    if scenario == "success":
        found = set()
        for record in (first, next_record):
            data = bytes.fromhex(next(r for r in record["return"]["memory"]
                                      if r["purpose"].endswith(".output-after"))["data_hex"])
            found.add(data[44:304].split(b"\0", 1)[0].decode("ascii"))
        assert found == set(result["authored_files"])
    if scenario != "error":
        assert local["return"]["registers"]["eax"] != 0
        inputs = next(r for r in system["entry"]["memory"] if r["purpose"].endswith(".input"))
        local_out = next(r for r in local["return"]["memory"] if r["purpose"].endswith(".output-after"))
        assert inputs["data_hex"] == local_out["data_hex"]
        utc_input = next(r for r in local["entry"]["memory"] if r["purpose"].endswith(".input"))
        first_data = bytes.fromhex(child["FindFirstFileA"]["hex"])
        assert bytes.fromhex(utc_input["data_hex"]) == first_data[20:28]
    assert all(step["trap_flag_after"] == 0 for step in result["steps"])
    for binding in result["resolved_imports"].values():
        restored = [r for r in result["cleanup"]["restored_bytes"] if r["address"] == binding["target"]]
        assert len(restored) == 1 and restored[0]["original_hex"] == restored[0]["observed_hex"]
