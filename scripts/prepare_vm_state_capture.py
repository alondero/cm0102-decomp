"""Prepare a bounded read-only primary state/artifact capture in the isolated guest."""
import argparse
import json
from pathlib import Path
import shutil
import sys
from prepare_runtime import ROOT, digest, SHA256


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--name', required=True)
    parser.add_argument('--guest-name', required=True, help='Name used by prepare_vm_guest_launch')
    parser.add_argument('--pid', type=int)
    parser.add_argument('--save', action='append', default=[], help='Exact private guest save basename, without extension')
    args = parser.parse_args()
    for name in (args.name, args.guest_name, *args.save):
        if not name.isascii() or not name.replace('-', '').isalnum() or not 1 <= len(name) <= 48:
            parser.error('ASCII letters/digits/hyphens names of length 1..48 required')
    if args.pid is not None and args.pid <= 0:
        parser.error('Positive guest PID required')
    area = ROOT / 'CM0102/Runtime'
    prep = area / ('vm-state-preparation-' + args.name)
    result = area / 'primary-vm-guest-results-a' / args.name
    source = area / 'primary-input-20261003-a'
    ps_path = source / (args.name + '.ps1')
    if any(p.exists() for p in (prep, result, ps_path)):
        parser.error('Fresh capture name required')
    launch_path = area / ('vm-launch-preparation-' + args.guest_name) / 'preparation.json'
    launch = json.loads(launch_path.read_text(encoding='utf-8'))
    assert launch['guest_root'] == 'C:\\CM0102-' + args.guest_name
    assert digest(source / 'cm0102.exe') == SHA256
    body = r'''$ErrorActionPreference = 'Stop'
$root = '__ROOT__'
$output = '\\vboxsvr\cmout\__NAME__'
if ((Get-ChildItem -LiteralPath $output -Force | Measure-Object).Count -ne 0) { throw 'Refusing nonempty capture output before any writes' }
$marker = [IO.File]::Open((Join-Path $output 'capture-started.txt'),[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None)
$writer = New-Object IO.StreamWriter($marker)
try { $writer.WriteLine([DateTime]::UtcNow.ToString('o')) } finally { $writer.Close() }
try {
  $found = @(Get-Process cm0102 -ErrorAction SilentlyContinue | Where-Object { $_.MainModule.FileName -eq (Join-Path $root 'cm0102.exe') })
  if ($found.Count -ne 1) { throw 'Expected exactly one private primary process' }
  $process = $found[0]
  __PID_CHECK__
  $hash = [Security.Cryptography.SHA256]::Create()
  $stream = [IO.File]::OpenRead($process.MainModule.FileName)
  try { $actual = ([BitConverter]::ToString($hash.ComputeHash($stream))).Replace('-', '').ToLowerInvariant() }
  finally { $stream.Close(); $hash.Clear() }
  if ($actual -ne '__PRIMARY_SHA__') { throw 'Primary executable hash differs' }
  & certutil -hashfile $process.MainModule.FileName SHA256 | Out-File (Join-Path $output 'executable-hash.txt') -Encoding utf8
  Get-WmiObject Win32_Process -Filter ('ProcessId=' + $process.Id) | Format-List ProcessId,ExecutablePath,CommandLine,CreationDate | Out-File (Join-Path $output 'process.txt') -Encoding utf8
  ('Actual module base: 0x{0:X8}' -f $process.MainModule.BaseAddress.ToInt32()) | Out-File (Join-Path $output 'process.txt') -Append -Encoding utf8
  if ($process.MainModule.BaseAddress.ToInt32() -ne 0x400000) { throw 'Unexpected module base; VA reads withheld' }
  Add-Type -TypeDefinition @'
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
public class PrimaryState {
  [DllImport("kernel32.dll",SetLastError=true)] static extern IntPtr OpenProcess(uint access,bool inherit,int id);
  [DllImport("kernel32.dll",SetLastError=true)] static extern bool ReadProcessMemory(IntPtr process,IntPtr address,byte[] buffer,UIntPtr count,out UIntPtr read);
  [DllImport("kernel32.dll",SetLastError=true)] static extern bool GetProcessTimes(IntPtr process,out long created,out long exited,out long kernel,out long user);
  [DllImport("kernel32.dll")] static extern uint WaitForSingleObject(IntPtr handle,uint timeout);
  [DllImport("kernel32.dll")] static extern bool CloseHandle(IntPtr handle);
  static void Alive(IntPtr handle) { if (WaitForSingleObject(handle,0)!=258) throw new Exception("Captured process exited"); }
  public static void Read(int id,long startTicks,string expectedPath,string output) {
    Process process=Process.GetProcessById(id);
    if (process.StartTime.ToUniversalTime().Ticks!=startTicks || !String.Equals(process.MainModule.FileName,expectedPath,StringComparison.OrdinalIgnoreCase)) throw new Exception("Process identity differs");
    IntPtr handle=OpenProcess(0x101410,false,id);
    if (handle==IntPtr.Zero) throw new Exception("OpenProcess failed");
    try {
      Alive(handle);
      long created,exited,kernel,user;
      if (!GetProcessTimes(handle,out created,out exited,out kernel,out user) || DateTime.FromFileTimeUtc(created).Ticks!=startTicks) throw new Exception("Held process creation time differs");
      Process opened=Process.GetProcessById(id);
      if (opened.StartTime.ToUniversalTime().Ticks!=startTicks || !String.Equals(opened.MainModule.FileName,expectedPath,StringComparison.OrdinalIgnoreCase) || opened.MainModule.BaseAddress.ToInt32()!=0x400000) throw new Exception("Process identity/base differs after open");
      uint[] addresses={0xAD6C30,0xDE1F5C,0xDE1F60,0xDE2798,0xAE2C88,0xAE2C90};
      string[] names={"lcg","mix","cursor","end","phase","date"};
      int[] sizes={4,4,4,4,1,8};
      File.WriteAllText(output,"Sequential unsuspended reads; no atomic snapshot or live code attestation; last_error is meaningful only on failure\n");
      for (int i=0;i<addresses.Length;i++) {
        Alive(handle);
        byte[] buffer=new byte[sizes[i]]; UIntPtr read;
        bool success=ReadProcessMemory(handle,new IntPtr((long)addresses[i]),buffer,new UIntPtr((uint)sizes[i]),out read);
        int error=Marshal.GetLastWin32Error();
        File.AppendAllText(output,String.Format("{0}\tVA 0x{1:X8}\t{2}\t{3:o}\tsuccess={4}; bytes={5}; last_error={6}\n",names[i],addresses[i],BitConverter.ToString(buffer).Replace("-", ""),DateTime.UtcNow,success,read.ToUInt32(),error));
        if (!success || read.ToUInt32()!=sizes[i]) throw new Exception("Read failed at "+names[i]);
      }
      Alive(handle);
    } finally { CloseHandle(handle); }
  }
}
'@
  [PrimaryState]::Read($process.Id,$process.StartTime.ToUniversalTime().Ticks,$process.MainModule.FileName,(Join-Path $output 'state.txt'))
  foreach ($name in @(__SAVES__)) {
    $file = Join-Path $root $name
    $destination = Join-Path $output $name
    if (Test-Path $destination) { throw 'Refusing existing artifact' }
    Copy-Item -LiteralPath $file -Destination $destination
    & certutil -hashfile $destination SHA256 | Out-File (Join-Path $output ($name+'.sha256.txt')) -Encoding utf8
  }
  Copy-Item -LiteralPath (Join-Path $root 'game.cfg') -Destination (Join-Path $output 'game.cfg')
  'PASS bounded primary state and requested artifacts; no replay acceptance' | Out-File (Join-Path $output 'capture-result.txt') -Encoding utf8
} catch {
  $_ | Out-String | Out-File (Join-Path $output 'failure.txt') -Encoding utf8
  throw
}
'''
    body = body.replace('__ROOT__', launch['guest_root']).replace('__NAME__', args.name)
    body = body.replace('__PRIMARY_SHA__', SHA256)
    body = body.replace('__PID_CHECK__', f"if ($process.Id -ne {args.pid}) {{ throw 'Guest PID differs' }}" if args.pid else '# Discover one process by verified private path; identity rechecked while holding its process handle.')
    body = body.replace('__SAVES__', ','.join("'" + n + ".sav'" for n in args.save))
    ps_path.write_text(body, encoding='utf-8')
    prep.mkdir()
    result.mkdir()
    for p in (ps_path, Path(__file__)):
        shutil.copyfile(p, prep / p.name)
    (prep / 'preparation.json').write_text(json.dumps(dict(status='prepared_only', command=sys.argv,
        python=sys.version, script_sha256=digest(Path(__file__)), support_sha256=digest(ROOT / 'scripts/prepare_runtime.py'),
        launch_preparation_sha256=digest(launch_path), guest_script_sha256=digest(ps_path),
        primary_sha256=SHA256, guest_root=launch['guest_root'], requested_pid=args.pid,
        scope='Sequential unsuspended primary memory reads; on-disk identity only; no engine parity'),indent=2)+'\n',encoding='utf-8')
    print('Prepared bounded guest capture:', ps_path.name)


if __name__ == '__main__':
    main()
