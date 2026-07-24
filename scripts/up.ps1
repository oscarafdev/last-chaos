$ErrorActionPreference = "Stop"
$projectRoot = Split-Path -Parent $PSScriptRoot

Push-Location $projectRoot
try {
    if (-not (Test-Path ".env")) {
        Copy-Item ".env.example" ".env"
        Write-Warning "Se creó .env con credenciales de desarrollo. Cámbialas antes de exponer servicios."
    }
    docker compose up --build -d
    docker compose ps
}
finally {
    Pop-Location
}
