param(
    [Parameter(Mandatory = $true)] [string] $AuthoritativeCheckout,
    [Parameter(Mandatory = $true)] [ValidateRange(1, 8)] [int] $Shard,
    [Parameter(Mandatory = $true)] [ValidatePattern('^[A-Za-z0-9][A-Za-z0-9_.-]*$')] [string] $Owner,
    [ValidateSet('Preview', 'Claim')] [string] $Action = 'Preview'
)

$ErrorActionPreference = 'Stop'
$checkout = (Resolve-Path -LiteralPath $AuthoritativeCheckout).Path
$target = Join-Path $checkout 'CM0102/Game/cm0102.exe'
$expectedHash = '0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86'
if (-not (Test-Path -LiteralPath $target -PathType Leaf)) { throw 'Pinned game executable not found in the authoritative checkout.' }
if ((Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash.ToLowerInvariant() -ne $expectedHash) { throw 'Pinned game executable hash does not match.' }
if (-not (Test-Path -LiteralPath (Join-Path $checkout 'cmre/__main__.py') -PathType Leaf)) { throw 'Authoritative cmre checkout not found.' }

Push-Location -LiteralPath $checkout
try {
    if ($Action -eq 'Claim') {
        & py -3.13 -m cmre next --c --claim $Owner --shard "$Shard/8" -n 3
    } else {
        & py -3.13 -m cmre next --c --shard "$Shard/8" -n 3
    }
    if ($LASTEXITCODE -ne 0) { throw "cmre exited with code $LASTEXITCODE" }
} finally {
    Pop-Location
}
