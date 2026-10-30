"""Record read-only Windows desktop/input availability for the disposable replay."""
import argparse
import ctypes
from ctypes import wintypes
from datetime import datetime,timezone
import json
import os
from pathlib import Path
import platform
import sys

from prepare_runtime import ROOT,digest


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    output=args.output.resolve()
    if not output.is_relative_to(ROOT/'CM0102/Runtime'):
        parser.error('output must remain beneath CM0102/Runtime')
    if output.exists():raise ValueError('Refusing existing desktop evidence')
    user32=ctypes.WinDLL('user32',use_last_error=True)
    kernel32=ctypes.WinDLL('kernel32',use_last_error=True)
    user32.GetThreadDesktop.argtypes=[wintypes.DWORD];user32.GetThreadDesktop.restype=wintypes.HANDLE
    user32.GetProcessWindowStation.restype=wintypes.HANDLE
    user32.OpenInputDesktop.argtypes=[wintypes.DWORD,wintypes.BOOL,wintypes.DWORD]
    user32.OpenInputDesktop.restype=wintypes.HANDLE
    user32.CloseDesktop.argtypes=[wintypes.HANDLE]
    user32.GetUserObjectInformationW.argtypes=[wintypes.HANDLE,ctypes.c_int,ctypes.c_void_p,wintypes.DWORD,ctypes.POINTER(wintypes.DWORD)]
    kernel32.GetCurrentThreadId.restype=wintypes.DWORD
    user32.GetForegroundWindow.restype=wintypes.HWND
    user32.GetCursorPos.argtypes=[ctypes.POINTER(wintypes.POINT)]
    def name(handle):
        buffer=ctypes.create_unicode_buffer(256);needed=wintypes.DWORD()
        ctypes.set_last_error(0)
        ok=user32.GetUserObjectInformationW(handle,2,buffer,ctypes.sizeof(buffer),ctypes.byref(needed))
        return dict(success=bool(ok),name=buffer.value if ok else None,last_error=ctypes.get_last_error())
    ctypes.set_last_error(0)
    desktop=user32.OpenInputDesktop(0,False,1)
    input_desktop=dict(open_succeeded=bool(desktop),last_error=ctypes.get_last_error())
    if desktop:
        try:input_desktop['information']=name(desktop)
        finally:user32.CloseDesktop(desktop)
    point=wintypes.POINT();ctypes.set_last_error(0)
    cursor_ok=user32.GetCursorPos(ctypes.byref(point))
    cursor=dict(success=bool(cursor_ok),last_error=ctypes.get_last_error())
    session=wintypes.DWORD()
    kernel32.ProcessIdToSessionId(os.getpid(),ctypes.byref(session))
    record=dict(recorded_utc=datetime.now(timezone.utc).isoformat(),command=sys.argv,
        python=sys.version,platform=platform.platform(),script_sha256=digest(Path(__file__)),
        pid=os.getpid(),session_id=session.value,
        window_station=name(user32.GetProcessWindowStation()),
        thread_desktop=name(user32.GetThreadDesktop(kernel32.GetCurrentThreadId())),
        input_desktop=input_desktop,cursor_read=cursor,foreground_hwnd=user32.GetForegroundWindow(),
        scope='Read-only host observation; no desktop switch or compatibility/system change')
    output.parent.mkdir(parents=True,exist_ok=True)
    output.write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(record,indent=2))


if __name__=='__main__':main()
