"""Drive the disposable GDI window: capture, click, key. Does not touch CM0102/Game."""
from __future__ import annotations

import argparse
import ctypes
import ctypes.wintypes as wintypes
import sys
import time
from pathlib import Path

user32 = ctypes.windll.user32
user32.FindWindowW.argtypes = [wintypes.LPCWSTR, wintypes.LPCWSTR]
user32.FindWindowW.restype = wintypes.HWND
user32.PostMessageW.argtypes = [wintypes.HWND, wintypes.UINT, wintypes.WPARAM, wintypes.LPARAM]
user32.SetForegroundWindow.argtypes = [wintypes.HWND]
user32.GetWindowThreadProcessId.argtypes = [wintypes.HWND, ctypes.POINTER(wintypes.DWORD)]
user32.GetClientRect.argtypes = [wintypes.HWND, ctypes.c_void_p]
user32.ClientToScreen.argtypes = [wintypes.HWND, ctypes.c_void_p]
TITLE = "Championship Manager 01/02"


class POINT(ctypes.Structure):
    _fields_ = [("x", ctypes.c_long), ("y", ctypes.c_long)]


class RECT(ctypes.Structure):
    _fields_ = [("left", ctypes.c_long), ("top", ctypes.c_long), ("right", ctypes.c_long), ("bottom", ctypes.c_long)]


def find_window() -> int:
    hwnd = user32.FindWindowW(None, TITLE)
    if not hwnd:
        raise SystemExit(f"window not found: {TITLE}")
    pid = wintypes.DWORD()
    user32.GetWindowThreadProcessId(hwnd, ctypes.byref(pid))
    kernel32 = ctypes.windll.kernel32
    kernel32.OpenProcess.argtypes = [wintypes.DWORD, wintypes.BOOL, wintypes.DWORD]
    kernel32.OpenProcess.restype = wintypes.HANDLE
    kernel32.QueryFullProcessImageNameW.argtypes = [wintypes.HANDLE, wintypes.DWORD, wintypes.LPWSTR, ctypes.POINTER(wintypes.DWORD)]
    kernel32.CloseHandle.argtypes = [wintypes.HANDLE]
    process = kernel32.OpenProcess(0x1000, False, pid.value)
    if not process:
        raise SystemExit("Cannot verify game process path")
    try:
        path = ctypes.create_unicode_buffer(32768)
        length = wintypes.DWORD(len(path))
        if not kernel32.QueryFullProcessImageNameW(process, 0, path, ctypes.byref(length)):
            raise SystemExit("Cannot query game process path")
        runtime = Path(__file__).resolve().parents[1] / "CM0102/Runtime"
        if not Path(path.value).resolve().is_relative_to(runtime.resolve()):
            raise SystemExit("Refusing interaction outside CM0102/Runtime")
    finally:
        kernel32.CloseHandle(process)
    return hwnd


def client_size(hwnd: int) -> tuple[int, int]:
    rect = RECT()
    user32.GetClientRect(hwnd, ctypes.byref(rect))
    return rect.right - rect.left, rect.bottom - rect.top


def click_client(hwnd: int, x: int, y: int) -> None:
    width, height = client_size(hwnd)
    if not (0 <= x < width and 0 <= y < height):
        raise SystemExit("Click outside client area")
    user32.SetForegroundWindow(hwnd)
    time.sleep(0.2)
    pt = POINT(x, y)
    user32.ClientToScreen(hwnd, ctypes.byref(pt))
    user32.SetCursorPos(pt.x, pt.y)
    time.sleep(0.05)
    user32.mouse_event(0x0002, 0, 0, 0, 0)
    time.sleep(0.05)
    user32.mouse_event(0x0004, 0, 0, 0, 0)
    time.sleep(0.2)


def capture(hwnd: int, path: Path) -> None:
    import subprocess

    pid = wintypes.DWORD()
    user32.GetWindowThreadProcessId(hwnd, ctypes.byref(pid))
    script = Path(__file__).resolve().parent / "capture_game.ps1"
    subprocess.check_call(
        ["powershell", "-NoProfile", "-File", str(script), "-GameProcessId", str(pid.value), "-OutputPath", str(path)]
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=["info", "click", "capture", "text", "key"])
    parser.add_argument("--x", type=int)
    parser.add_argument("--y", type=int)
    parser.add_argument("--out", type=Path)
    parser.add_argument("--text")
    parser.add_argument("--key", choices=["enter", "tab", "escape", "space"])
    args = parser.parse_args()
    hwnd = find_window()
    w, h = client_size(hwnd)
    print(f"hwnd={hwnd} client={w}x{h}")
    if args.action == "info":
        return 0
    if args.action == "text":
        if args.text is None or not args.text.isascii():
            raise SystemExit("text requires --text with ASCII characters")
        for char in args.text:
            user32.PostMessageW(hwnd, 0x0102, ord(char), 1)
            time.sleep(0.03)
        return 0
    if args.action == "key":
        if args.key is None:
            raise SystemExit("key requires --key")
        key = {"enter": 13, "tab": 9, "escape": 27, "space": 32}[args.key]
        user32.PostMessageW(hwnd, 0x0100, key, 1)
        time.sleep(0.05)
        user32.PostMessageW(hwnd, 0x0101, key, 0xC0000001)
        return 0
    if args.action == "click":
        if args.x is None or args.y is None:
            raise SystemExit("click requires --x --y in client pixels")
        click_client(hwnd, args.x, args.y)
        return 0
    if args.out is None:
        raise SystemExit("capture requires --out")
    capture(hwnd, args.out)
    print(f"wrote {args.out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
