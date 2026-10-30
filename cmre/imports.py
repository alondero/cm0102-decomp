"""Differential-test stubs for the 145 imports. TEST SCAFFOLDING ONLY.

A stub only has to be identical on the original and recovered sides; it is not a
model of Windows. It records the call (name + stdcall arguments) as an ordered
event, returns a value derived from the case seed, and pops the documented
stdcall argument count. Terminal imports end the case instead.
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
