"""Read mapped RNG state from a verified unpatched disposable GDI process."""
import argparse
import ctypes
from ctypes import wintypes
from datetime import datetime,timezone
import json
from pathlib import Path
import platform
import struct
import sys
from prepare_runtime import ROOT,GDI_SHA256,digest


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--pid',type=int,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--phase',action='store_true',help='Also capture the separately mapped Date/phase globals')
    args=parser.parse_args();out=args.output.resolve()
    if out.exists()or not out.is_relative_to(ROOT/'CM0102/Runtime'):
        parser.error('fresh output beneath Runtime required')
    mapping_path=ROOT/'CM0102/Analysis/rng-gdi-map-b/report.json'
    mapping=json.loads(mapping_path.read_text(encoding='utf-8'))
    assert mapping['status']=='pass'and mapping['gdi_sha256']==GDI_SHA256
    assert digest(ROOT/'scripts/map_rng_gdi.py')==mapping['script_sha256']
    kernel32=ctypes.WinDLL('kernel32',use_last_error=True)
    kernel32.OpenProcess.argtypes=[wintypes.DWORD,wintypes.BOOL,wintypes.DWORD];kernel32.OpenProcess.restype=wintypes.HANDLE
    kernel32.CloseHandle.argtypes=[wintypes.HANDLE]
    kernel32.QueryFullProcessImageNameW.argtypes=[wintypes.HANDLE,wintypes.DWORD,wintypes.LPWSTR,ctypes.POINTER(wintypes.DWORD)]
    kernel32.ReadProcessMemory.argtypes=[wintypes.HANDLE,ctypes.c_void_p,ctypes.c_void_p,ctypes.c_size_t,ctypes.POINTER(ctypes.c_size_t)]
    kernel32.CreateToolhelp32Snapshot.argtypes=[wintypes.DWORD,wintypes.DWORD];kernel32.CreateToolhelp32Snapshot.restype=wintypes.HANDLE
    class Module(ctypes.Structure):
        _fields_=[('dwSize',wintypes.DWORD),('th32ModuleID',wintypes.DWORD),('th32ProcessID',wintypes.DWORD),
            ('GlblcntUsage',wintypes.DWORD),('ProccntUsage',wintypes.DWORD),('modBaseAddr',ctypes.c_void_p),
            ('modBaseSize',wintypes.DWORD),('hModule',wintypes.HMODULE),('szModule',wintypes.WCHAR*256),('szExePath',wintypes.WCHAR*260)]
    kernel32.Module32FirstW.argtypes=[wintypes.HANDLE,ctypes.POINTER(Module)]
    kernel32.Module32NextW.argtypes=[wintypes.HANDLE,ctypes.POINTER(Module)]
    process=kernel32.OpenProcess(0x1010,False,args.pid)
    if not process:raise ctypes.WinError(ctypes.get_last_error())
    try:
        name=ctypes.create_unicode_buffer(32768);length=wintypes.DWORD(len(name))
        if not kernel32.QueryFullProcessImageNameW(process,0,name,ctypes.byref(length)):raise ctypes.WinError(ctypes.get_last_error())
        executable=Path(name.value).resolve()
        assert executable.name.lower()=='cm0102_gdi.exe' and executable.is_relative_to(ROOT/'CM0102/Runtime')
        assert digest(executable)==GDI_SHA256
        snapshot=kernel32.CreateToolhelp32Snapshot(0x18,args.pid)
        if snapshot==ctypes.c_void_p(-1).value:raise ctypes.WinError(ctypes.get_last_error())
        try:
            module=Module();module.dwSize=ctypes.sizeof(Module)
            found=bool(kernel32.Module32FirstW(snapshot,ctypes.byref(module)));base=None
            while found:
                if Path(module.szExePath).resolve()==executable:base=module.modBaseAddr;break
                found=bool(kernel32.Module32NextW(snapshot,ctypes.byref(module)))
            if base is None:raise ValueError('Game module base not found')
        finally:kernel32.CloseHandle(snapshot)
        def memory(va,size):
            buffer=ctypes.create_string_buffer(size);read=ctypes.c_size_t()
            if not kernel32.ReadProcessMemory(process,va,buffer,size,ctypes.byref(read))or read.value!=size:
                raise ctypes.WinError(ctypes.get_last_error())
            return buffer.raw
        state={}
        for label,primary in (('lcg',0xad6c30),('mix',0xde1f5c),('cursor',0xde1f60),('table_end',0xde2798)):
            preferred=int(mapping['address_correspondences'][f'0x{primary:08x}'],16)
            actual=base+preferred-0x400000;raw=memory(actual,4)
            state[label]=dict(primary_preferred_va=f'0x{primary:08x}',gdi_preferred_va=f'0x{preferred:08x}',
                runtime_va=f'0x{actual:08x}',bytes=raw.hex(),uint32=struct.unpack('<I',raw)[0])
        cursor=state['cursor']['uint32']
        state['cursor_word']=dict(runtime_va=f'0x{cursor:08x}',bytes=memory(cursor,4).hex())
        phase_mapping_sha256=None
        if args.phase:
            phase_path=ROOT/'CM0102/Analysis/phase-gdi-map-b/report.json'
            phase_mapping=json.loads(phase_path.read_text(encoding='utf-8'))
            assert phase_mapping['status']=='pass'and phase_mapping['input_gdi_sha256']==GDI_SHA256
            assert digest(ROOT/'CM0102/Analysis/map-phase-gdi.py')==phase_mapping['script_sha256']
            phase_mapping_sha256=digest(phase_path)
            for label,primary,size in (('phase',0xae2c88,1),('date',0xae2c90,8)):
                preferred=int(phase_mapping['address_correspondences'][hex(primary)],16)
                actual=base+preferred-0x400000
                state[label]=dict(primary_preferred_va=hex(primary),gdi_preferred_va=hex(preferred),
                    runtime_va=hex(actual),bytes=memory(actual,size).hex())
        record=dict(recorded_utc=datetime.now(timezone.utc).isoformat(),command=sys.argv,python=sys.version,
            platform=platform.platform(),script_sha256=digest(Path(__file__)),pid=args.pid,
            executable=executable.relative_to(ROOT).as_posix(),executable_sha256=GDI_SHA256,
            actual_module_base=f'0x{base:08x}',mapping_sha256=digest(mapping_path),phase_mapping_sha256=phase_mapping_sha256,state=state,
            scope='Read-only bounded RNG snapshot; no process pause/write or complete world-state comparison')
        out.parent.mkdir(parents=True,exist_ok=True)
        out.write_text(json.dumps(record,indent=2)+'\n',encoding='utf-8')
        print('Captured mapped RNG state; actual module base',hex(base),{name:row.get('uint32')for name,row in state.items()})
    finally:kernel32.CloseHandle(process)


if __name__=='__main__':main()
