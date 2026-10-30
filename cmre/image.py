"""Read-only view of the pinned primary executable."""
from __future__ import annotations

import struct
from dataclasses import dataclass
from functools import cache

import pefile

from . import env


@dataclass(frozen=True)
class Section:
    name: str
    va: int
    virtual_size: int
    raw: bytes
    characteristics: int

    @property
    def end(self) -> int:
        return self.va + self.virtual_size

    @property
    def writable(self) -> bool:
        return bool(self.characteristics & 0x80000000)

    @property
    def executable(self) -> bool:
        return bool(self.characteristics & 0x20000000)


class Image:
    def __init__(self, data: bytes, base: int):
        pe = pefile.PE(data=data, fast_load=True)
        pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_IMPORT"]])
        self.base = base
        # Optional source-declared CALL transport, validated by the builder.
        self.return_bindings = ()
        self.jump_bindings = ()
        self.x87_bindings = ()
        self.headers = data[: pe.OPTIONAL_HEADER.SizeOfHeaders]
        self.sections = [
            Section(
                s.Name.rstrip(b"\0").decode("ascii"),
                base + s.VirtualAddress,
                max(s.Misc_VirtualSize, s.SizeOfRawData),
                s.get_data(),
                s.Characteristics,
            )
            for s in pe.sections
        ]
        # IAT slot VA -> "DLL!Name"
        self.imports: dict[int, str] = {}
        for entry in getattr(pe, "DIRECTORY_ENTRY_IMPORT", []):
            for imp in entry.imports:
                name = imp.name.decode() if imp.name else f"#{imp.ordinal}"
                self.imports[imp.address] = f"{entry.dll.decode()}!{name}"

    def section_at(self, va: int) -> Section | None:
        for s in self.sections:
            if s.va <= va < s.end:
                return s
        return None

    def read(self, va: int, size: int) -> bytes:
        s = self.section_at(va)
        if s is None or va + size > s.end:
            raise KeyError(f"{va:#x}+{size} outside image sections")
        start = va - s.va
        blob = s.raw[start : start + size]
        return blob + bytes(size - len(blob))  # uninitialised tail reads as zero

    def read_upto(self, va: int, size: int) -> bytes:
        """Read at most `size` bytes, stopping at the section end (for decode windows)."""
        s = self.section_at(va)
        if s is None:
            raise KeyError(f"{va:#x} outside image sections")
        return self.read(va, min(size, s.end - va))

    def dword(self, va: int) -> int:
        return struct.unpack("<I", self.read(va, 4))[0]

    @property
    def text(self) -> Section:
        return next(s for s in self.sections if s.name == ".text")


@cache
def primary() -> Image:
    return Image(env.target_bytes(), env.IMAGE_BASE)
