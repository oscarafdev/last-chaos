[CmdletBinding()]
param(
    [ValidateSet("Win32", "x64")]
    [string]$Platform = "x64",

    [string]$Configuration = "LCRelease",

    [string]$Target = "Nksp",

    [string]$PlatformToolset = "v145",

    [string]$WindowsSdkVersion = "10.0.26100.0"
)

$ErrorActionPreference = "Stop"
$projectRoot = Split-Path -Parent $PSScriptRoot
$solution = Join-Path $projectRoot "client\src\LCClient.sln"
$outputDirectory = (Join-Path $projectRoot "client\build\$PlatformToolset\$Platform") + "\"
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"

if (-not (Test-Path $vswhere)) {
    throw "No se encontró vswhere. Instala Visual Studio Build Tools con C++."
}

$installationPath = & $vswhere -latest -products * `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property installationPath

if (-not $installationPath) {
    throw "No se encontró un toolchain C++ de Visual Studio."
}

$msbuild = Join-Path $installationPath "MSBuild\Current\Bin\MSBuild.exe"
if (-not (Test-Path $msbuild)) {
    throw "No se encontró MSBuild en $installationPath."
}

& $msbuild $solution `
    /m `
    /t:$Target `
    /p:Configuration=$Configuration `
    /p:Platform=$Platform `
    /p:PlatformToolset=$PlatformToolset `
    /p:WindowsTargetPlatformVersion=$WindowsSdkVersion `
    /p:OutDir=$outputDirectory `
    /p:PreserveRuntimeBin=true `
    /p:CustomBuildStepUseInBuild=false `
    /p:PostBuildEventUseInBuild=false `
    /verbosity:minimal

if ($LASTEXITCODE -ne 0) {
    throw "La compilación del cliente falló con código $LASTEXITCODE."
}
