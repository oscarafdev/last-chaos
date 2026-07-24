[CmdletBinding()]
param(
    [ValidateSet("usa", "rus", "brz", "mex", "ger", "uk")]
    [string]$Locale = "usa"
)

$ErrorActionPreference = "Stop"
$projectRoot = Split-Path -Parent $PSScriptRoot

Push-Location $projectRoot
try {
    $env:LC_LOCALE = $Locale
    docker compose build server
}
finally {
    Pop-Location
}
