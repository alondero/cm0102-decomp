"""Bounded Win32 file and time observations for the i386 caller ABI.

Ranges come from the validated native fixture ABI. Storage snapshots and
changes describe reads at endpoints. Writes and complete effects require
separate evidence.
"""
from __future__ import annotations

SIZES = {"FindFirstFileA": 320, "FindNextFileA": 320,
         "FileTimeToLocalFileTime": 8, "FileTimeToSystemTime": 16}


def output_span(name, args):
    if name not in SIZES or len(args) != 2:
        raise ValueError("unsupported two-argument file/time import")
    if any(type(a) is not int or not 0 <= a <= 0xffffffff for a in args):
        raise ValueError("invalid raw u32 argument")
    address, size = args[1], SIZES[name]
    if address == 0 or address + size > 0x100000000:
        raise ValueError("invalid file/time output range")
    return address, size


def read_range(reader, address, size, purpose):
    """Preserve partial bytes and verify their reported address and extent."""
    if not 0 < address < address + size <= 0x100000000:
        raise ValueError("invalid file/time observation range")
    read = reader(address, size, purpose)
    if (read.address, read.requested_size, read.purpose) != (address, size, purpose):
        raise ValueError("file/time sample does not match requested range")
    return read


def read_output(reader, name, args, phase):
    address, size = output_span(name, args)
    return read_range(reader, address, size, f"{name}.output-{phase}")


def classify_output(name, eax, before, after):
    """Byte equality is not output definedness; failed buffers stay undefined."""
    if name not in SIZES or type(eax) is not int or not 0 <= eax <= 0xffffffff:
        raise ValueError("unsupported scalar/prototype")
    if not isinstance(before, bytes) or not isinstance(after, bytes) or (
            len(before) != SIZES[name] or len(after) != SIZES[name]):
        raise ValueError("classification requires exact bounded before/after bytes")
    success = eax != (0xffffffff if name == "FindFirstFileA" else 0)
    ranges, gaps = [], ["bounded-storage-only-not-complete-effects"]
    if success:
        if name.startswith("Find"):
            # Fixed public fields through nFileSizeLow, NOT dwReserved0/1,
            # alternate-name storage, unfilled name tails or trailing padding.
            ranges.append([0, 36])
            end = after.find(b"\0", 44, 304)
            if end < 0:
                gaps.append("filename-not-terminated")
            else:
                ranges.append([44, end + 1])
        else:
            ranges.append([0, SIZES[name]])
    changed = [i for i, (a, b) in enumerate(zip(before, after)) if a != b]
    return dict(scalar_result=eax, before_hex=before.hex(), after_hex=after.hex(),
                output_status="selected-defined-fields" if success else "undefined-on-failure",
                defined_ranges=ranges, changed_offsets=changed,
                unchanged_count=len(before) - len(changed), gaps=gaps, replayable=False)
