param()
$ErrorActionPreference = 'Stop'
$workspace = Split-Path $PSScriptRoot -Parent
$toolRoot = Join-Path $workspace 'tools/ghidra'
$archive = Join-Path $toolRoot 'ghidra_12.1.4_PUBLIC_20260921.zip'
$install = Join-Path $toolRoot 'ghidra_12.1.4_PUBLIC'
$expected = 'ddac49f903da9d5bac833e5cc79395098b9c33cfd3279be5f31bd00387d2d4db'
if (Test-Path -LiteralPath $install) { throw "Installation exists; refusing to overwrite $install" }
New-Item -ItemType Directory -Force -Path $toolRoot | Out-Null
if (-not (Test-Path -LiteralPath $archive)) {
    & curl.exe -L --fail --output $archive 'https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_12.1.4_build/ghidra_12.1.4_PUBLIC_20260921.zip'
    if ($LASTEXITCODE -ne 0) { throw 'Ghidra download failed' }
}
if ((Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash.ToLowerInvariant() -ne $expected) {
    throw 'Ghidra archive hash mismatch'
}
Expand-Archive -LiteralPath $archive -DestinationPath $toolRoot
Write-Output "Ghidra ready: $install"
