"""Bind replay actions to one live, hash-checked disposable GDI process."""
import ctypes
from ctypes import wintypes
from pathlib import Path
import subprocess

from gdi_interact import find_window, user32
from prepare_runtime import ROOT, GDI_SHA256, digest


class VerifiedGame:
    def __init__(self, pid, runtime):
        self.pid = pid
        self.runtime = Path(runtime).resolve()
        runtime_root = ROOT / 'CM0102/Runtime'
        if pid <= 0 or self.runtime == runtime_root or not self.runtime.is_relative_to(runtime_root):
            raise ValueError('Positive PID and disposable runtime required')
        self.kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
        self.kernel32.OpenProcess.argtypes = [wintypes.DWORD, wintypes.BOOL, wintypes.DWORD]
        self.kernel32.OpenProcess.restype = wintypes.HANDLE
        self.kernel32.CloseHandle.argtypes = [wintypes.HANDLE]
        self.kernel32.QueryFullProcessImageNameW.argtypes = [wintypes.HANDLE, wintypes.DWORD, wintypes.LPWSTR, ctypes.POINTER(wintypes.DWORD)]
        self.kernel32.GetProcessTimes.argtypes = [wintypes.HANDLE] + [ctypes.POINTER(wintypes.FILETIME)] * 4
        self.kernel32.WaitForSingleObject.argtypes = [wintypes.HANDLE, wintypes.DWORD]
        self.handle = None

    def __enter__(self):
        self.handle = self.kernel32.OpenProcess(0x101000, False, self.pid)
        if not self.handle:
            raise ctypes.WinError(ctypes.get_last_error())
        try:
            self.hwnd = find_window()
            name = ctypes.create_unicode_buffer(32768)
            length = wintypes.DWORD(len(name))
            if not self.kernel32.QueryFullProcessImageNameW(self.handle, 0, name, ctypes.byref(length)):
                raise ctypes.WinError(ctypes.get_last_error())
            executable = Path(name.value).resolve()
            if executable != self.runtime / 'cm0102_GDI.exe' or digest(executable) != GDI_SHA256:
                raise ValueError('PID does not name the expected unpatched runtime GDI executable')
            creation, exit_time, kernel, user = (wintypes.FILETIME() for _ in range(4))
            if not self.kernel32.GetProcessTimes(self.handle, ctypes.byref(creation), ctypes.byref(exit_time), ctypes.byref(kernel), ctypes.byref(user)):
                raise ctypes.WinError(ctypes.get_last_error())
            self.identity = dict(pid=self.pid, hwnd=self.hwnd,
                executable=executable.relative_to(ROOT).as_posix(), executable_sha256=GDI_SHA256,
                creation_filetime=(creation.dwHighDateTime << 32) | creation.dwLowDateTime)
            self.check()
            return self
        except BaseException:
            self.__exit__(None, None, None)
            raise

    def check(self):
        pid = wintypes.DWORD()
        user32.GetWindowThreadProcessId(self.hwnd, ctypes.byref(pid))
        if pid.value != self.pid or self.kernel32.WaitForSingleObject(self.handle, 0) != 0x102:
            raise RuntimeError('Expected game process/window ended or changed')

    def __exit__(self, *unused):
        if self.handle:
            self.kernel32.CloseHandle(self.handle)
            self.handle = None


def tool_pins():
    return {name: digest(ROOT / 'scripts' / name) for name in
        ('replay_runtime_guard.py', 'gdi_interact.py', 'capture_game.ps1', 'prepare_runtime.py')}


def capture_tool():
    command = ['powershell', '-NoProfile', '-Command', '$PSVersionTable.PSVersion.ToString()']
    return dict(version_command=command, version=subprocess.check_output(command, text=True).strip(),
        capture_command=['powershell', '-NoProfile', '-File', 'scripts/capture_game.ps1',
            '-GameProcessId', '<recorded PID>', '-OutputPath', '<recorded PNG>'])
