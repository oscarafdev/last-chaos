[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidatePattern("^[a-z]{2}$")]
    [string]$TargetLanguage,

    [string]$SourceLanguage = "us",

    [string]$ClientRoot
)

$ErrorActionPreference = "Stop"

if ([string]::IsNullOrWhiteSpace($ClientRoot)) {
    $ClientRoot = Join-Path $PSScriptRoot "..\client"
}

$resolvedClientRoot = (Resolve-Path -LiteralPath $ClientRoot).Path
$sourceRoot = Join-Path $resolvedClientRoot "Local\$SourceLanguage"
$targetRoot = Join-Path $resolvedClientRoot "Local\$TargetLanguage"

if (-not (Test-Path -LiteralPath $sourceRoot -PathType Container)) {
    throw "No se encontró la localización de respaldo: $sourceRoot."
}

if (-not (Test-Path -LiteralPath $targetRoot -PathType Container)) {
    throw "No se encontró la localización de destino: $targetRoot."
}

$installed = 0
$merged = 0

function Copy-MissingLocalizationFile {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Source,

        [Parameter(Mandatory = $true)]
        [string]$Destination
    )

    if (Test-Path -LiteralPath $Destination) {
        return
    }

    $destinationDirectory = Split-Path -Parent $Destination
    if (-not (Test-Path -LiteralPath $destinationDirectory)) {
        New-Item -ItemType Directory -Path $destinationDirectory | Out-Null
    }

    Copy-Item -LiteralPath $Source -Destination $Destination
    $script:installed += 1
}

# La interfaz busca estas carpetas antes de continuar con el arranque.
foreach ($folder in @("Loading", "SignBoard")) {
    $sourceFolder = Join-Path $sourceRoot $folder
    if (-not (Test-Path -LiteralPath $sourceFolder -PathType Container)) {
        continue
    }

    Get-ChildItem -LiteralPath $sourceFolder -File -Recurse | ForEach-Object {
        $relativePath = $_.FullName.Substring($sourceFolder.Length + 1)
        $destination = Join-Path (Join-Path $targetRoot $folder) $relativePath
        Copy-MissingLocalizationFile -Source $_.FullName -Destination $destination
    }
}

# Mapas y otras texturas localizadas ubicadas en la raíz.
Get-ChildItem -LiteralPath $sourceRoot -File -Filter "*.tex" | ForEach-Object {
    if ($_.Name -like "Font*") {
        return
    }
    Copy-MissingLocalizationFile `
        -Source $_.FullName `
        -Destination (Join-Path $targetRoot $_.Name)
}

$sourceStringRoot = Join-Path $sourceRoot "String"
$targetStringRoot = Join-Path $targetRoot "String"

# Este archivo no lleva sufijo de idioma.
$chatFilter = Join-Path $sourceStringRoot "CharacterChatFilter.dat"
if (Test-Path -LiteralPath $chatFilter -PathType Leaf) {
    Copy-MissingLocalizationFile `
        -Source $chatFilter `
        -Destination (Join-Path $targetStringRoot "CharacterChatFilter.dat")
}

# Los .lod y filtros restantes llevan el sufijo del país. Se renombran, pero
# nunca sustituyen un archivo traducido que ya exista.
Get-ChildItem -LiteralPath $sourceStringRoot -File | Where-Object {
    $_.Name -match "_$([regex]::Escape($SourceLanguage))\.(lod|dat)$"
} | ForEach-Object {
    $targetName = $_.Name -replace `
        "_$([regex]::Escape($SourceLanguage))(?=\.(lod|dat)$)", `
        "_$TargetLanguage"
    Copy-MissingLocalizationFile `
        -Source $_.FullName `
        -Destination (Join-Path $targetStringRoot $targetName)
}

# El código de país también selecciona tres tablas globales fuera de Local.
# Esta distribución solo incluye las variantes USA/RUS.
$countryData = @{
    es = @{
        MakeItem = @{ Source = "MakeItem_usa.lod"; Target = "MakeItem_spn.lod" }
        Lacarette = @{ Source = "lacarette_USA.lod"; Target = "lacarette_spn.lod" }
        Event = @{ Source = "event_usa.lod"; Target = "event_spn.lod"; Country = 14 }
    }
}

if ($countryData.ContainsKey($TargetLanguage)) {
    foreach ($definition in $countryData[$TargetLanguage].Values) {
        $sourceData = Join-Path $resolvedClientRoot "Data\$($definition.Source)"
        $targetData = Join-Path $resolvedClientRoot "Data\$($definition.Target)"
        if (-not (Test-Path -LiteralPath $sourceData -PathType Leaf)) {
            throw "Falta la tabla de respaldo $sourceData."
        }
        Copy-MissingLocalizationFile -Source $sourceData -Destination $targetData

        if ($definition.ContainsKey("Country")) {
            $stream = [System.IO.File]::Open(
                $targetData,
                [System.IO.FileMode]::Open,
                [System.IO.FileAccess]::Write,
                [System.IO.FileShare]::Read
            )
            try {
                $writer = [System.IO.BinaryWriter]::new($stream)
                $writer.Write([int]$definition.Country)
                $writer.Flush()
            }
            finally {
                if ($null -ne $writer) {
                    $writer.Dispose()
                }
                $stream.Dispose()
            }
        }
    }
}

# USA no trae la tabla textual de Lacarette. La rusa sí tiene el esquema
# requerido; se usa solo si el paquete de destino tampoco proporciona uno.
$targetLacaretteStrings = Join-Path $targetStringRoot "strLacarette_$TargetLanguage.lod"
if (-not (Test-Path -LiteralPath $targetLacaretteStrings -PathType Leaf)) {
    $lacaretteStructure = Join-Path $resolvedClientRoot "Local\ru\String\strLacarette_ru.lod"
    if (Test-Path -LiteralPath $lacaretteStructure -PathType Leaf) {
        Copy-MissingLocalizationFile `
            -Source $lacaretteStructure `
            -Destination $targetLacaretteStrings
    }
}

# TableLoader presupone que cada fila contiene todos sus campos. Los paquetes
# antiguos pueden traer huecos o índices distintos y hacen que el runtime 2018
# aborte. Se usa USA como estructura compatible y se superponen traducciones.
$lodFieldCounts = [ordered]@{
    "strItem"           = 2
    "strSetItem"        = 1
    "strOption"         = 1
    "strRareOption"     = 1
    "strNpcName"        = 2
    "strNPCHelp"        = 2
    "strNPCShop"        = 2
    "strQuest"          = 4
    "strSkill"          = 3
    "strSSkill"         = 2
    "strAction"         = 2
    "strCombo"          = 1
    "strAffinity"       = 1
    "strItemCollection" = 2
}
$lodMerger = Join-Path $PSScriptRoot "merge-localization-lod.js"
$node = Get-Command node -ErrorAction Stop

foreach ($entry in $lodFieldCounts.GetEnumerator()) {
    $sourceLod = Join-Path $sourceStringRoot "$($entry.Key)_$SourceLanguage.lod"
    $targetLod = Join-Path $targetStringRoot "$($entry.Key)_$TargetLanguage.lod"
    if (-not (Test-Path -LiteralPath $sourceLod -PathType Leaf) -or
        -not (Test-Path -LiteralPath $targetLod -PathType Leaf)) {
        continue
    }

    $originalLod = "$targetLod.original"
    if (-not (Test-Path -LiteralPath $originalLod -PathType Leaf)) {
        Copy-Item -LiteralPath $targetLod -Destination $originalLod
    }

    & $node.Source $lodMerger $sourceLod $originalLod $targetLod $entry.Value | Out-Null
    if ($LASTEXITCODE -ne 0) {
        throw "No se pudo compatibilizar $targetLod."
    }
    $merged += 1
}

Write-Output "$installed archivos instalados; $merged tablas LOD compatibilizadas"
