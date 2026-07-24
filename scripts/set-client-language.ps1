[CmdletBinding()]
param(
    [ValidateSet("de", "es", "fr", "it", "pl", "ru", "uk", "us")]
    [string]$Language = "es",

    [string]$ClientRoot
)

$ErrorActionPreference = "Stop"

if ([string]::IsNullOrWhiteSpace($ClientRoot)) {
    $ClientRoot = Join-Path $PSScriptRoot "..\client"
}

$resolvedClientRoot = (Resolve-Path -LiteralPath $ClientRoot).Path
$persistentSymbols = Join-Path $resolvedClientRoot "Data\etc\ps.dat"
$languageTool = Join-Path $PSScriptRoot "client-language.js"
$fallbackInstaller = Join-Path $PSScriptRoot "install-localization-fallback.ps1"
$latinFontPatcher = Join-Path $PSScriptRoot "patch-latin-font.js"

if (-not (Test-Path -LiteralPath $persistentSymbols -PathType Leaf)) {
    throw "No se encontró $persistentSymbols."
}

if (Get-Process -Name "Nksp" -ErrorAction SilentlyContinue) {
    throw "Cierra completamente Last Chaos antes de cambiar el idioma."
}

$node = Get-Command node -ErrorAction Stop
$backup = "$persistentSymbols.bak"
if (-not (Test-Path -LiteralPath $backup)) {
    Copy-Item -LiteralPath $persistentSymbols -Destination $backup
}

$requiredFonts = @{
    de = "FontGerman0.tex"
    es = "FontSpain0.tex"
    fr = "FontFrance0.tex"
    it = "FontITALY0.tex"
    pl = "FontPOLAND0.tex"
    ru = "FontRussia0.tex"
    uk = "FontChineseT0.tex"
    us = "FontChineseT0.tex"
}

$requiredFont = Join-Path $resolvedClientRoot "Local\$Language\$($requiredFonts[$Language])"
$chineseFont = Join-Path $resolvedClientRoot "Local\us\FontChineseT0.tex"
$latinFont = Join-Path $resolvedClientRoot "Local\common\FontLatin0.tex"
$latinLanguages = @("de", "es", "fr", "it", "pl")
$fallbackFont = if ($Language -in $latinLanguages) { $latinFont } else { $chineseFont }

if ($Language -in $latinLanguages) {
    & $node.Source $latinFontPatcher $latinFont
    if ($LASTEXITCODE -ne 0) {
        throw "No se pudieron instalar los glifos latinos."
    }

    # Las variantes latinas usan la misma cuadrícula de PutTextExBrz.
    Copy-Item -LiteralPath $latinFont -Destination $requiredFont -Force
}

$requiresReplacement = -not (Test-Path -LiteralPath $requiredFont -PathType Leaf)
if (-not $requiresReplacement -and $Language -in $latinLanguages) {
    # Versiones anteriores del selector copiaban el atlas chino. Detectarlo y
    # sustituirlo porque PutTextExBrz usa una cuadrícula latina diferente.
    $requiresReplacement = (Get-FileHash -LiteralPath $requiredFont).Hash -eq
        (Get-FileHash -LiteralPath $chineseFont).Hash
}

if ($requiresReplacement) {
    if (-not (Test-Path -LiteralPath $fallbackFont -PathType Leaf)) {
        throw "Falta la fuente requerida y tampoco existe la fuente de respaldo: $fallbackFont."
    }
    Copy-Item -LiteralPath $fallbackFont -Destination $requiredFont -Force
}

$fallbackCount = & $fallbackInstaller `
    -TargetLanguage $Language `
    -ClientRoot $resolvedClientRoot

& $node.Source $languageTool set $persistentSymbols $Language
if ($LASTEXITCODE -ne 0) {
    throw "No se pudo actualizar el idioma del cliente."
}

$selected = & $node.Source $languageTool get $persistentSymbols
if ($LASTEXITCODE -ne 0) {
    throw "El archivo se escribió, pero no se pudo verificar."
}

Write-Host ""
Write-Host "Idioma configurado: $selected"
Write-Host "Recursos de respaldo instalados: $fallbackCount"
Write-Host "Ahora inicia el juego con Jugar.cmd."
