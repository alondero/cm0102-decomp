param([Parameter(Mandatory=$true)][int]$GameProcessId, [Parameter(Mandatory=$true)][string]$OutputPath)
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
Add-Type @'
using System;
using System.Runtime.InteropServices;
public class GameCapture {
 [StructLayout(LayoutKind.Sequential)] public struct RECT { public int Left, Top, Right, Bottom; }
 [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
 [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr dc, uint flags);
}
'@
$handle = (Get-Process -Id $GameProcessId).MainWindowHandle
if ($handle -eq 0) { throw 'Game has no visible main window' }
$rect = New-Object GameCapture+RECT
if (-not [GameCapture]::GetWindowRect($handle,[ref]$rect)) { throw 'Cannot read game window bounds' }
$bitmap = New-Object System.Drawing.Bitmap(($rect.Right-$rect.Left),($rect.Bottom-$rect.Top))
$graphics = [System.Drawing.Graphics]::FromImage($bitmap)
try {
    $dc = $graphics.GetHdc()
    try {
        if (-not [GameCapture]::PrintWindow($handle,$dc,2)) { throw 'Window capture failed' }
    } finally { $graphics.ReleaseHdc($dc) }
    $bitmap.Save([System.IO.Path]::GetFullPath($OutputPath))
} finally { $graphics.Dispose(); $bitmap.Dispose() }
