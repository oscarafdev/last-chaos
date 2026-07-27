param(
    [Parameter(Position = 0)]
    [ValidatePattern('^[A-Za-z0-9_-]{1,48}$')]
    [string]$Name = 'last'
)

$projectRoot = Split-Path -Parent $PSScriptRoot
$configurationDirectory = Join-Path $projectRoot '.itconfig'
$requestPath = Join-Path $configurationDirectory 'dx12-camera-capture.request'

if (-not (Test-Path -LiteralPath $configurationDirectory -PathType Container)) {
    throw "No se encontró la carpeta .itconfig en $projectRoot"
}

Set-Content -LiteralPath $requestPath -Value $Name -Encoding Ascii -NoNewline
Write-Host "Solicitud enviada. Mantén el juego abierto en la vista defectuosa."
Write-Host "La captura aparecerá en .itconfig\\dx12-camera-captures\\camera-$Name.json"
