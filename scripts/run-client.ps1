[CmdletBinding()]
param(
    [string]$ClientRoot
)

$ErrorActionPreference = "Stop"

if ([string]::IsNullOrWhiteSpace($ClientRoot)) {
    $ClientRoot = Join-Path $PSScriptRoot "..\client"
}

$resolvedClientRoot = (Resolve-Path -LiteralPath $ClientRoot).Path
$executable = Join-Path $resolvedClientRoot "Bin\Nksp.exe"
$serverList = Join-Path $resolvedClientRoot "sl.dta"
$launcher = Join-Path $resolvedClientRoot "multi.bat"

if (-not (Test-Path -LiteralPath $executable -PathType Leaf)) {
    throw "No se encontró $executable. Ejecuta scripts\build-client.ps1 primero."
}

if (-not (Test-Path -LiteralPath $serverList -PathType Leaf)) {
    throw "No se encontró $serverList. El cliente no puede resolver el LoginServer."
}

if (-not (Test-Path -LiteralPath $launcher -PathType Leaf)) {
    throw "No se encontro $launcher."
}

# cmd.exe reproduce la forma exacta de lanzamiento que espera el cliente legado.
Start-Process `
    -FilePath $env:ComSpec `
    -ArgumentList "/d", "/c", "multi.bat" `
    -WorkingDirectory $resolvedClientRoot `
    -WindowStyle Hidden
