[CmdletBinding()]
param(
    [string]$ServerAddress = "144.217.7.136",

    [ValidateRange(1, 65535)]
    [int]$ServerPort = 4001,

    [string]$OutputDirectory
)

$ErrorActionPreference = "Stop"
$projectRoot = Split-Path -Parent $PSScriptRoot
$clientRoot = Join-Path $projectRoot "client"

if ([string]::IsNullOrWhiteSpace($OutputDirectory)) {
    $OutputDirectory = Join-Path $projectRoot ".itconfig\dist"
}

$timestamp = Get-Date -Format "yyyyMMdd-HHmmss"
$packageName = "LastChaos-DX12-Beta-$timestamp"
$stagingRoot = Join-Path $OutputDirectory $packageName
$stagingClient = Join-Path $stagingRoot "client"
$archivePath = Join-Path $OutputDirectory "$packageName.zip"

New-Item -ItemType Directory -Path $stagingClient -Force | Out-Null

$runtimeDirectories = @("Bin", "Classes", "Data", "Fonts", "Local", "Shaders")
foreach ($directory in $runtimeDirectories) {
    $source = Join-Path $clientRoot $directory
    $destination = Join-Path $stagingClient $directory
    $robocopyArguments = @(
        $source,
        $destination,
        "/E",
        "/R:2",
        "/W:1",
        "/NFL",
        "/NDL",
        "/NJH",
        "/NJS",
        "/NP",
        "/XF",
        "*.pdb",
        "*.lib",
        "*.exp",
        "*.log",
        "ps.dat",
        "ps.dat.bak",
        "Player0.plr",
        "pass.txt"
    )
    & robocopy @robocopyArguments | Out-Null
    if ($LASTEXITCODE -ge 8) {
        throw "No se pudo copiar el directorio de runtime $directory (robocopy $LASTEXITCODE)."
    }
}

$runtimeFiles = @("lccnct.dta", "luncher.dat", "ModEXT.txt", "multi.bat", "sl.dta", "vtm.brn")
foreach ($file in $runtimeFiles) {
    Copy-Item -LiteralPath (Join-Path $clientRoot $file) -Destination $stagingClient -Force
}
[IO.File]::WriteAllBytes((Join-Path $stagingClient "nksp.loc"), [byte[]]::new(0))

$profileSource = Join-Path $clientRoot "Data\Etc\ps.dat"
$profileDestination = Join-Path $stagingClient "Data\Etc\ps.dat"
Copy-Item -LiteralPath $profileSource -Destination $profileDestination -Force

& node (Join-Path $PSScriptRoot "client-language.js") set $profileDestination es | Out-Null
if ($LASTEXITCODE -ne 0) {
    throw "No se pudo configurar el idioma espanol en el perfil beta."
}
& node (Join-Path $PSScriptRoot "client-language.js") sanitize $profileDestination | Out-Null
if ($LASTEXITCODE -ne 0) {
    throw "No se pudo eliminar la cuenta guardada del perfil beta."
}

& (Join-Path $PSScriptRoot "set-client-server.ps1") `
    -Address $ServerAddress `
    -Port $ServerPort `
    -ClientRoot $stagingClient | Out-Null

$jugar = @'
@echo off
setlocal
cd /d "%~dp0client"
set "LASTCHAOS_DX12_UI_COMPARE=replace"
set "LASTCHAOS_DX12_3D_REPLACE_ALL=enabled"
call multi.bat
if errorlevel 1 (
  echo.
  echo No se pudo iniciar Last Chaos.
  pause
)
'@
[IO.File]::WriteAllText((Join-Path $stagingRoot "Jugar-Espanol.cmd"), $jugar, [Text.Encoding]::ASCII)

$readme = @"
LAST CHAOS - BETA DIRECTX 12

1. Extrae completamente el archivo ZIP.
2. Ejecuta Jugar-Espanol.cmd.
3. Crea tu cuenta en https://lc.somositconfig.com

Servidor: ${ServerAddress}:${ServerPort}
El paquete no contiene cuentas ni contrasenas guardadas.
"@
[IO.File]::WriteAllText((Join-Path $stagingRoot "LEEME.txt"), $readme, [Text.Encoding]::UTF8)

foreach ($forbiddenFile in @("pass.txt", "Player0.plr")) {
    if (Get-ChildItem -LiteralPath $stagingRoot -Recurse -File -Filter $forbiddenFile) {
        throw "El paquete contiene el archivo persistente prohibido: $forbiddenFile"
    }
}

$decodedProfile = & node (Join-Path $PSScriptRoot "client-language.js") get $profileDestination
if ($LASTEXITCODE -ne 0 -or $decodedProfile -notmatch "^es ") {
    throw "No se pudo verificar el idioma del perfil beta."
}
$savedAccount = & node (Join-Path $PSScriptRoot "client-language.js") account $profileDestination |
    ConvertFrom-Json
if ($LASTEXITCODE -ne 0 -or $savedAccount.saveID -ne 0 -or $savedAccount.username -ne "") {
    throw "El perfil beta todavia contiene una cuenta guardada."
}

Push-Location $OutputDirectory
try {
    & tar.exe -a -cf $archivePath $packageName
    if ($LASTEXITCODE -ne 0) {
        throw "No se pudo comprimir el paquete beta."
    }
}
finally {
    Pop-Location
}

$hash = (Get-FileHash -Algorithm SHA256 -LiteralPath $archivePath).Hash
[pscustomobject]@{
    Package = $archivePath
    Sha256 = $hash
    Staging = $stagingRoot
}
