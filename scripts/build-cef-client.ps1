[CmdletBinding()]
param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release",

    [switch]$SkipDeploy,

    [switch]$DeployRebuiltClient
)

$ErrorActionPreference = "Stop"

$cefVersion = "150.0.14+g7c1aa68+chromium-150.0.7871.129"
$cefArchiveName = "cef_binary_${cefVersion}_windows64_minimal.tar.bz2"
$cefArchiveSha1 = "63272475eee07ec21cbc8774e2e7e18957115a55"
$cefDownloadUrl = "https://cef-builds.spotifycdn.com/$($cefArchiveName.Replace('+', '%2B'))"

$projectRoot = Split-Path -Parent $PSScriptRoot
$cacheRoot = Join-Path $projectRoot ".itconfig\cef"
$archivePath = Join-Path $cacheRoot $cefArchiveName
$cefRoot = Join-Path $cacheRoot "cef_binary_${cefVersion}_windows64_minimal"
$buildRoot = Join-Path $projectRoot ".itconfig\build\cef"
$sourceRoot = Join-Path $projectRoot "client\src\CefWebPage"
$runtimeRoot = Join-Path $projectRoot "client\Bin"
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"

New-Item -ItemType Directory -Force -Path $cacheRoot | Out-Null

if (-not (Test-Path $archivePath)) {
    Write-Host "Descargando CEF $cefVersion..."
    & curl.exe -L --fail --output $archivePath $cefDownloadUrl
    if ($LASTEXITCODE -ne 0) {
        throw "No se pudo descargar CEF."
    }
}

$actualSha1 = (Get-FileHash $archivePath -Algorithm SHA1).Hash.ToLowerInvariant()
if ($actualSha1 -ne $cefArchiveSha1) {
    throw "El SHA-1 de CEF no coincide. Esperado: $cefArchiveSha1; actual: $actualSha1"
}

if (-not (Test-Path (Join-Path $cefRoot "cmake\FindCEF.cmake"))) {
    Write-Host "Extrayendo CEF..."
    & tar.exe -xf $archivePath -C $cacheRoot
    if ($LASTEXITCODE -ne 0) {
        throw "No se pudo extraer CEF."
    }
}

if (-not (Test-Path $vswhere)) {
    throw "No se encontró vswhere. Instala Visual Studio Build Tools con C++."
}

$installationPath = & $vswhere -latest -products * `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property installationPath
$installationVersion = & $vswhere -latest -products * -property installationVersion
if (-not $installationPath) {
    throw "No se encontró un toolchain C++ de Visual Studio."
}

$cmake = Join-Path $installationPath "Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
if (-not (Test-Path $cmake)) {
    throw "No se encontró CMake en la instalación de Visual Studio."
}

$visualStudioMajor = [int]($installationVersion.Split(".")[0])
$visualStudioYear = if ($visualStudioMajor -ge 18) { "2026" } else { "2022" }
$generator = "Visual Studio $visualStudioMajor $visualStudioYear"

& $cmake -S $sourceRoot -B $buildRoot `
    -G $generator -A x64 `
    "-DCEF_ROOT=$cefRoot" `
    "-DUSE_SANDBOX=OFF"
if ($LASTEXITCODE -ne 0) {
    throw "Falló la configuración CMake de CEF."
}

& $cmake --build $buildRoot --config $Configuration `
    --target CWebPage CefSubprocess --parallel
if ($LASTEXITCODE -ne 0) {
    throw "Falló la compilación de CEF."
}

if ($SkipDeploy) {
    return
}

$backupRoot = Join-Path $projectRoot ".itconfig\backups"
New-Item -ItemType Directory -Force -Path $backupRoot | Out-Null
$currentWebPage = Join-Path $runtimeRoot "CWebPage.dll"
if ((Test-Path $currentWebPage) -and
    -not (Test-Path (Join-Path $backupRoot "CWebPage.legacy.dll"))) {
    Copy-Item $currentWebPage (Join-Path $backupRoot "CWebPage.legacy.dll")
}

$outputRoot = Join-Path $buildRoot $Configuration
Copy-Item (Join-Path $outputRoot "CWebPage.dll") $runtimeRoot -Force
Copy-Item (Join-Path $outputRoot "CefSubprocess.exe") $runtimeRoot -Force

$binaryFiles = @(
    "chrome_elf.dll",
    "d3dcompiler_47.dll",
    "dxcompiler.dll",
    "dxil.dll",
    "libcef.dll",
    "libEGL.dll",
    "libGLESv2.dll",
    "v8_context_snapshot.bin",
    "vk_swiftshader.dll",
    "vk_swiftshader_icd.json",
    "vulkan-1.dll"
)
foreach ($file in $binaryFiles) {
    Copy-Item (Join-Path $cefRoot "Release\$file") $runtimeRoot -Force
}

Copy-Item (Join-Path $cefRoot "Resources\*") $runtimeRoot -Recurse -Force
Copy-Item (Join-Path $cefRoot "LICENSE.txt") `
    (Join-Path $runtimeRoot "CEF-LICENSE.txt") -Force

if ($DeployRebuiltClient) {
    $clientBuildRoot = Join-Path $projectRoot "client\build\v145\x64"
    $clientFiles = @(
        "Nksp.exe",
        "Engine.dll",
        "EntitiesMP.dll",
        "GameMP.dll",
        "Shaders.dll"
    )

    foreach ($file in $clientFiles) {
        $builtFile = Join-Path $clientBuildRoot $file
        if (-not (Test-Path $builtFile)) {
            throw "Falta $builtFile. Ejecuta scripts\build-client.ps1 primero."
        }

        $runtimeFile = Join-Path $runtimeRoot $file
        $backupFile = Join-Path $backupRoot "$file.legacy"
        if ((Test-Path $runtimeFile) -and -not (Test-Path $backupFile)) {
            Copy-Item $runtimeFile $backupFile
        }
        Copy-Item $builtFile $runtimeFile -Force
    }
}

Write-Host "CEF desplegado en $runtimeRoot"
