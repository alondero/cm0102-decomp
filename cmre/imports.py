"""Differential-test stubs for the 145 imports. TEST SCAFFOLDING ONLY.

A stub only has to be identical on the original and recovered sides; it is not a
model of Windows. It records the call (name + stdcall arguments) as an ordered
event, returns a value derived from the case seed, and pops the documented
stdcall argument count. A successful FindFirstFileA/FindNextFileA/FileTimeTo*
call also writes its documented output structure (see `outputs`), so neither side
reads stack garbage from the buffer. Terminal imports end the case instead.
Never use these values as evidence of game behaviour.
"""
from __future__ import annotations

# Documented WINAPI/PASCAL (callee-pops) argument counts.
STDCALL_ARGS = {
    "ADVAPI32.dll!RegOpenKeyExA": 5, "ADVAPI32.dll!RegQueryValueExA": 6,
    "DDRAW.dll!DirectDrawCreate": 3, "DSOUND.dll!#1": 3,  # ordinal 1 = DirectSoundCreate
    "GDI32.dll!CreateFontA": 14, "GDI32.dll!DeleteObject": 1, "GDI32.dll!EndDoc": 1, "GDI32.dll!EndPage": 1,
    "GDI32.dll!GetDeviceCaps": 2, "GDI32.dll!GetTextMetricsA": 2, "GDI32.dll!SelectObject": 2,
    "GDI32.dll!StartDocA": 2, "GDI32.dll!StartPage": 1, "GDI32.dll!TextOutA": 5,
    "KERNEL32.dll!CloseHandle": 1, "KERNEL32.dll!CompareStringA": 6, "KERNEL32.dll!CompareStringW": 6,
    "KERNEL32.dll!CreateDirectoryA": 2, "KERNEL32.dll!CreateFileA": 7, "KERNEL32.dll!CreateMutexA": 3,
    "KERNEL32.dll!CreateProcessA": 10, "KERNEL32.dll!DeleteFileA": 1, "KERNEL32.dll!ExitProcess": 1,
    "KERNEL32.dll!FileTimeToLocalFileTime": 2, "KERNEL32.dll!FileTimeToSystemTime": 2, "KERNEL32.dll!FindClose": 1,
    "KERNEL32.dll!FindFirstFileA": 2, "KERNEL32.dll!FindNextFileA": 2, "KERNEL32.dll!FlushFileBuffers": 1,
    "KERNEL32.dll!FreeEnvironmentStringsA": 1, "KERNEL32.dll!FreeEnvironmentStringsW": 1,
    "KERNEL32.dll!FreeLibrary": 1, "KERNEL32.dll!GetACP": 0, "KERNEL32.dll!GetCPInfo": 2,
    "KERNEL32.dll!GetCommandLineA": 0, "KERNEL32.dll!GetCurrentDirectoryA": 2, "KERNEL32.dll!GetCurrentProcess": 0,
    "KERNEL32.dll!GetDiskFreeSpaceA": 5, "KERNEL32.dll!GetDriveTypeA": 1, "KERNEL32.dll!GetEnvironmentStrings": 0,
    "KERNEL32.dll!GetEnvironmentStringsW": 0, "KERNEL32.dll!GetEnvironmentVariableA": 3,
    "KERNEL32.dll!GetExitCodeProcess": 2, "KERNEL32.dll!GetFileAttributesA": 1, "KERNEL32.dll!GetFileType": 1,
    "KERNEL32.dll!GetFullPathNameA": 4, "KERNEL32.dll!GetLastError": 0, "KERNEL32.dll!GetLocalTime": 1,
    "KERNEL32.dll!GetLogicalDriveStringsA": 2, "KERNEL32.dll!GetModuleFileNameA": 3,
    "KERNEL32.dll!GetModuleHandleA": 1, "KERNEL32.dll!GetOEMCP": 0, "KERNEL32.dll!GetProcAddress": 2,
    "KERNEL32.dll!GetStartupInfoA": 1, "KERNEL32.dll!GetStdHandle": 1, "KERNEL32.dll!GetStringTypeA": 5,
    "KERNEL32.dll!GetStringTypeW": 4, "KERNEL32.dll!GetSystemInfo": 1, "KERNEL32.dll!GetSystemTime": 1,
    "KERNEL32.dll!GetSystemTimeAsFileTime": 1, "KERNEL32.dll!GetTimeZoneInformation": 1, "KERNEL32.dll!GetVersion": 0,
    "KERNEL32.dll!GetVersionExA": 1, "KERNEL32.dll!GlobalAlloc": 2, "KERNEL32.dll!GlobalFree": 1,
    "KERNEL32.dll!GlobalMemoryStatus": 1, "KERNEL32.dll!HeapAlloc": 3, "KERNEL32.dll!HeapCreate": 3,
    "KERNEL32.dll!HeapDestroy": 1, "KERNEL32.dll!HeapFree": 3, "KERNEL32.dll!HeapReAlloc": 4,
    "KERNEL32.dll!HeapSize": 3, "KERNEL32.dll!IsBadCodePtr": 1, "KERNEL32.dll!IsBadReadPtr": 2,
    "KERNEL32.dll!IsBadWritePtr": 2, "KERNEL32.dll!LCMapStringA": 6, "KERNEL32.dll!LCMapStringW": 6,
    "KERNEL32.dll!LoadLibraryA": 1, "KERNEL32.dll!MoveFileA": 2, "KERNEL32.dll!MultiByteToWideChar": 6,
    "KERNEL32.dll!RaiseException": 4, "KERNEL32.dll!ReadFile": 5, "KERNEL32.dll!RtlUnwind": 4,
    "KERNEL32.dll!SetCurrentDirectoryA": 1, "KERNEL32.dll!SetEndOfFile": 1, "KERNEL32.dll!SetEnvironmentVariableA": 2,
    "KERNEL32.dll!SetFilePointer": 4, "KERNEL32.dll!SetHandleCount": 1, "KERNEL32.dll!SetStdHandle": 2,
    "KERNEL32.dll!SetUnhandledExceptionFilter": 1, "KERNEL32.dll!TerminateProcess": 2,
    "KERNEL32.dll!UnhandledExceptionFilter": 1, "KERNEL32.dll!VirtualAlloc": 4, "KERNEL32.dll!VirtualFree": 3,
    "KERNEL32.dll!WaitForSingleObject": 2, "KERNEL32.dll!WideCharToMultiByte": 8, "KERNEL32.dll!WinExec": 2,
    "KERNEL32.dll!WriteFile": 5,
    "USER32.dll!BeginPaint": 2, "USER32.dll!ChangeDisplaySettingsA": 2, "USER32.dll!CloseWindow": 1,
    "USER32.dll!CreateWindowExA": 12, "USER32.dll!DefWindowProcA": 4, "USER32.dll!DestroyCursor": 1,
    "USER32.dll!DestroyWindow": 1, "USER32.dll!DispatchMessageA": 1, "USER32.dll!EndPaint": 2,
    "USER32.dll!GetCursor": 0, "USER32.dll!GetDesktopWindow": 0, "USER32.dll!GetKeyboardLayout": 1,
    "USER32.dll!GetMessageA": 4, "USER32.dll!GetSystemMetrics": 1, "USER32.dll!GetWindowRect": 2,
    "USER32.dll!LoadCursorA": 2, "USER32.dll!LoadIconA": 2, "USER32.dll!MessageBoxA": 4, "USER32.dll!MoveWindow": 6,
    "USER32.dll!PeekMessageA": 5, "USER32.dll!RegisterClassA": 1, "USER32.dll!SetCursor": 1,
    "USER32.dll!SetWindowLongA": 3, "USER32.dll!ShowCursor": 1, "USER32.dll!ShowWindow": 2,
    "USER32.dll!TranslateMessage": 1, "USER32.dll!UpdateWindow": 1,
    "WINSPOOL.DRV!EnumPrintersA": 7,
    "WSOCK32.dll!WSACleanup": 0, "WSOCK32.dll!WSAGetLastError": 0, "WSOCK32.dll!WSAStartup": 2,
    "WSOCK32.dll!accept": 3, "WSOCK32.dll!bind": 3, "WSOCK32.dll!closesocket": 1, "WSOCK32.dll!connect": 3,
    "WSOCK32.dll!gethostbyaddr": 3, "WSOCK32.dll!gethostname": 2, "WSOCK32.dll!htons": 1,
    "WSOCK32.dll!ioctlsocket": 3, "WSOCK32.dll!listen": 2, "WSOCK32.dll!recv": 4, "WSOCK32.dll!recvfrom": 6,
    "WSOCK32.dll!send": 4, "WSOCK32.dll!sendto": 6, "WSOCK32.dll!setsockopt": 5, "WSOCK32.dll!socket": 3,
    "comdlg32.dll!PrintDlgA": 1,
}

# Control does not come back normally (or unwinds frames): end the case here.
TERMINAL = {"KERNEL32.dll!ExitProcess", "KERNEL32.dll!RaiseException", "KERNEL32.dll!RtlUnwind",
            "KERNEL32.dll!TerminateProcess", "KERNEL32.dll!UnhandledExceptionFilter"}


def stub_code(name: str) -> bytes:
    """`ret 4*n` (or `ret`) executed after the hook records the call and sets EAX."""
    n = STDCALL_ARGS[name]
    return bytes([0xC2, (4 * n) & 0xFF, (4 * n) >> 8]) if n else b"\xC3"


def result(seed: int, index: int, arena: int, arena_size: int) -> int:
    """Seeded stub return: 0, 1, an arena pointer or a random dword (deterministic)."""
    h = (seed * 0x9E3779B1 + (index + 1) * 0x85EBCA6B) & 0xFFFFFFFF
    h ^= h >> 15
    h = (h * 0x2C1B3C6D) & 0xFFFFFFFF
    h ^= h >> 12
    kind = h & 3
    if kind == 0:
        return 0
    if kind == 1:
        return 1
    if kind == 2:
        return arena + ((h >> 4) % (arena_size - 0x400)) & ~3
    return h


# Documented output structures written by the file-search and file-time stubs.
FIND_DATA_SIZE = 320  # WIN32_FIND_DATAA
_FILETIME_MAX = (1 << 63) - 1  # FileTimeToSystemTime fails only for values with the top bit set
_TICKS_PER_DAY = 864000000000


def _mix(seed: int, index: int, salt: int) -> int:
    h = (seed * 0x9E3779B1 + (index + 1) * 0x85EBCA6B + salt * 0xC2B2AE35) & 0xFFFFFFFF
    h ^= h >> 15
    h = (h * 0x2C1B3C6D) & 0xFFFFFFFF
    h ^= h >> 12
    h = (h * 0x297A2D39) & 0xFFFFFFFF
    return h ^ (h >> 15)


def find_data(seed: int, index: int) -> bytes:
    """A valid WIN32_FIND_DATAA chosen only by (seed, call index): documented layout.

    Times are valid FILETIMEs, the name is NUL-terminated and fits 8.3, so
    cAlternateFileName stays empty. Test scaffolding, not a file system."""
    kind = _mix(seed, index, 1) % 8
    if kind < 2:
        name, attrs = (b".", b"..")[kind], 0x10
    else:
        name = b"F%07X.DAT" % (_mix(seed, index, 2) & 0xFFFFFFF)
        attrs = (0x10, 0x20, 0x01, 0x80)[_mix(seed, index, 3) % 4]
        if attrs == 0x10:
            name = name[:8]
    times = b"".join(
        (0x01C00000_00000000 + (_mix(seed, index, 4 + i) << 20) + _mix(seed, index, 8 + i)).to_bytes(8, "little")
        for i in range(3))
    size = 0 if attrs == 0x10 else _mix(seed, index, 12)
    out = (attrs.to_bytes(4, "little") + times + (0).to_bytes(4, "little") + size.to_bytes(4, "little")
           + bytes(8) + name + b"\0")
    return out + bytes(FIND_DATA_SIZE - len(out))


def systemtime(filetime: int) -> bytes | None:
    """SYSTEMTIME for a FILETIME, or None where FileTimeToSystemTime documents failure."""
    if not 0 <= filetime <= _FILETIME_MAX:
        return None
    days, rest = divmod(filetime, _TICKS_PER_DAY)
    seconds, ticks = divmod(rest, 10000000)
    # Proleptic Gregorian date from days since 1601-01-01 (a 400-year cycle is 146097 days).
    era, doe = divmod(days + 584694, 146097)
    yoe = (doe - doe // 1460 + doe // 36524 - doe // 146096) // 365
    doy = doe - (365 * yoe + yoe // 4 - yoe // 100)
    mp = (5 * doy + 2) // 153
    day = doy - (153 * mp + 2) // 5 + 1
    month = mp + 3 if mp < 10 else mp - 9
    year = yoe + era * 400 + (month <= 2)
    weekday = (days + 1) % 7  # 1601-01-01 was a Monday; SYSTEMTIME counts Sunday as 0
    hour, seconds = divmod(seconds, 3600)
    minute, second = divmod(seconds, 60)
    fields = (year, month, weekday, day,
              hour, minute, second, ticks // 10000)
    return b"".join(f.to_bytes(2, "little") for f in fields)


def outputs(name: str, args: tuple, value: int, seed: int, index: int, read) -> list[tuple[int, bytes]]:
    """Documented out-parameter writes for a successful call: [(address, bytes)].

    `read(address, size)` returns bytes, or None when unmapped. A failed call
    writes nothing (the API leaves its output unspecified). Only the file-search
    and file-time imports are covered; every other stub writes nothing."""
    if name == "KERNEL32.dll!FindFirstFileA":
        return [(args[1], find_data(seed, index))] if value != 0xFFFFFFFF else []
    if name == "KERNEL32.dll!FindNextFileA":
        return [(args[1], find_data(seed, index))] if value else []
    if not value or name not in ("KERNEL32.dll!FileTimeToLocalFileTime", "KERNEL32.dll!FileTimeToSystemTime"):
        return []
    raw = read(args[0], 8)
    if raw is None:
        return []
    if name == "KERNEL32.dll!FileTimeToLocalFileTime":
        return [(args[1], raw)]  # time-zone bias of zero: local time equals UTC, copied unchanged
    st = systemtime(int.from_bytes(raw, "little"))
    return [(args[1], st)] if st is not None else []


class Transcript:
    """Authored per-case import inputs, never a Windows or CRT implementation.

    Exact call names and arguments fail closed. Heap annotations additionally
    check the author's allocation/size/free claims; no allocation or file data
    is inferred. Every output byte and return value is supplied by the fixture.
    """

    HEAP_IMPORTS = {'alloc': 'KERNEL32.dll!HeapAlloc',
                    'size': 'KERNEL32.dll!HeapSize',
                    'free': 'KERNEL32.dll!HeapFree'}

    # Only the input-capacity release's explicit heap/file/drive boundaries.
    # STDCALL_ARGS describes stack cleanup, not permission to substitute any API.
    SUPPORTED_IMPORTS = frozenset(HEAP_IMPORTS.values()) | frozenset({
        'KERNEL32.dll!CreateFileA', 'KERNEL32.dll!ReadFile',
        'KERNEL32.dll!WriteFile', 'KERNEL32.dll!SetFilePointer',
        'KERNEL32.dll!GetFileType', 'KERNEL32.dll!CloseHandle',
        'KERNEL32.dll!FlushFileBuffers', 'KERNEL32.dll!SetEndOfFile',
        'KERNEL32.dll!GetLastError', 'KERNEL32.dll!GetStdHandle',
        'KERNEL32.dll!GetLogicalDriveStringsA', 'KERNEL32.dll!GetDriveTypeA',
    })

    def __init__(self, steps, arena, size):
        import copy
        self.steps = copy.deepcopy(steps)
        self.index = 0
        self.arena, self.size = arena, size
        self.blocks = {}
        if not isinstance(steps, list):
            raise ValueError('import transcript must be a list')
        for step in steps:
            if (not isinstance(step, dict) or set(step) - {'name','args','result','writes','heap'}
                    or not {'name','args','result'} <= set(step)):
                raise ValueError('invalid import transcript step')
            name, args = step['name'], step['args']
            if not isinstance(name, str) or name not in self.SUPPORTED_IMPORTS:
                raise ValueError('unsupported transcript import')
            if (not isinstance(args, list) or len(args) != STDCALL_ARGS[name]
                    or any(type(v) is not int or not 0 <= v <= 0xffffffff for v in args)
                    or type(step['result']) is not int or not 0 <= step['result'] <= 0xffffffff):
                raise ValueError('invalid transcript arguments or result')
            heap = step.get('heap')
            if name in self.HEAP_IMPORTS.values() and heap is None:
                raise ValueError('transcript heap imports require a coherence annotation')
            if heap is not None and (heap not in self.HEAP_IMPORTS or name != self.HEAP_IMPORTS[heap]):
                raise ValueError('invalid heap annotation')
            if not isinstance(step.get('writes', []), list):
                raise ValueError('invalid transcript writes')
            for write in step.get('writes', []):
                if (not isinstance(write, dict) or set(write) != {'arg','data'}
                        or type(write['arg']) is not int or not 0 <= write['arg'] < len(args)
                        or not isinstance(write['data'], str)):
                    raise ValueError('invalid transcript output')
                try:
                    blob = bytes.fromhex(write['data'])
                except ValueError as exc:
                    raise ValueError('invalid transcript output hex') from exc
                self._range(args[write['arg']], len(blob))

    def _range(self, address, size):
        if not self.arena <= address <= address + size <= self.arena + self.size:
            raise ValueError('transcript output/allocation outside arena')

    def call(self, name, args, write):
        if self.index >= len(self.steps):
            raise ValueError('import transcript exhausted')
        step = self.steps[self.index]
        if step['name'] != name or tuple(step['args']) != args:
            raise ValueError('import transcript name or arguments differ')
        result, heap = step['result'], step.get('heap')
        if heap == 'alloc' and result:
            size = args[2]
            self._range(result, size)
            if not size or any(result < p+n and p < result+size for (_,p),n in self.blocks.items()):
                raise ValueError('overlapping or empty transcript allocation')
            self.blocks[args[0],result] = size
        elif heap in ('size','free'):
            key = (args[0], args[2])
            if key not in self.blocks:
                raise ValueError('unknown transcript heap block')
            if heap == 'size' and result != self.blocks[key]:
                raise ValueError('transcript HeapSize differs from allocation')
            if heap == 'free' and result:
                del self.blocks[key]
        for output in step.get('writes', []):
            blob = bytes.fromhex(output['data'])
            self._range(args[output['arg']], len(blob))
            write(args[output['arg']], blob)
        self.index += 1
        return result
