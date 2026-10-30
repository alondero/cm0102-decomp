<#
.SYNOPSIS
Run REA with this project's Ghidra and a hash-checked analysis copy.
.EXAMPLE
.\scripts\rea.ps1 prepare
.EXAMPLE
.\scripts\rea.ps1 doctor --provider ghidra --json
#>
param(
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]] $ReaArguments
)

$ErrorActionPreference = 'Stop'
$workspace = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$targetHash = '0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86'
$original = Join-Path $workspace 'CM0102/Game/cm0102.exe'
$analysisCopy = Join-Path $workspace 'CM0102/Analysis/rea/input/cm0102.exe'
$ghidra = Join-Path $workspace 'tools/ghidra/ghidra_12.1.4_PUBLIC'
$java = 'C:\Program Files\Java\jdk-21.0.10'

if ((Get-FileHash -LiteralPath $original -Algorithm SHA256).Hash.ToLowerInvariant() -ne $targetHash) {
    throw 'Pinned CM 01/02 3.9.68 input hash mismatch; refusing REA startup.'
}
if (!(Test-Path -LiteralPath $analysisCopy)) {
    New-Item -ItemType Directory -Path (Split-Path -Parent $analysisCopy) -Force | Out-Null
    Copy-Item -LiteralPath $original -Destination $analysisCopy
}
if ((Get-FileHash -LiteralPath $analysisCopy -Algorithm SHA256).Hash.ToLowerInvariant() -ne $targetHash) {
    throw 'REA analysis copy hash mismatch; preserve it for diagnosis and prepare a new copy.'
}

if ($ReaArguments.Count -eq 1 -and $ReaArguments[0] -eq 'prepare') {
    Write-Output $analysisCopy
    exit 0
}

foreach ($prerequisite in @((Join-Path $ghidra 'support/analyzeHeadless.bat'), (Join-Path $java 'bin/javac.exe'))) {
    if (!(Test-Path -LiteralPath $prerequisite -PathType Leaf)) {
        throw "REA prerequisite missing: $prerequisite. See docs/agent/REA.md."
    }
}
$reaCommand = Get-Command rea.cmd -CommandType Application -ErrorAction Stop
$settings = @{
    GHIDRA_INSTALL_DIR = $ghidra
    JAVA_HOME = $java
    REA_ANALYSIS_PROVIDER = 'ghidra'
    REA_GHIDRA_STARTUP_TIMEOUT_MS = '900000'
}
$previous = @{}
try {
    foreach ($name in $settings.Keys) {
        $previous[$name] = [Environment]::GetEnvironmentVariable($name, 'Process')
        [Environment]::SetEnvironmentVariable($name, $settings[$name], 'Process')
    }
    & $reaCommand.Source @ReaArguments
    $reaExitCode = $LASTEXITCODE
}
finally {
    foreach ($name in $previous.Keys) {
        [Environment]::SetEnvironmentVariable($name, $previous[$name], 'Process')
    }
}
exit $reaExitCode
