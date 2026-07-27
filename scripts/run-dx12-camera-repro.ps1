[CmdletBinding()]
param(
    [string]$FixturePath = (
        '.itconfig\dx12-camera-captures\camera-bug-piso.json'
    ),

    [string]$ScreenshotPath = '',

    [string]$ServerAddress = '127.0.0.1',

    [ValidateRange(0, 31)]
    [int]$ServerIndex = 0,

    [ValidateRange(0, 31)]
    [int]$ChannelIndex = 0,

    [ValidateRange(0, 7)]
    [int]$CharacterIndex = 0,

    [ValidateRange(0, 300)]
    [int]$WorldDelaySeconds = 5,

    [ValidateRange(1, 30)]
    [int]$CameraHoldSeconds = 5,

    [ValidateRange(30, 300)]
    [int]$TimeoutSeconds = 120
)

$ErrorActionPreference = 'Stop'
$workspace = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$fixtureCandidate = if ([IO.Path]::IsPathRooted($FixturePath)) {
    $FixturePath
}
else {
    Join-Path $workspace $FixturePath
}
$fixtureFile = (Resolve-Path $fixtureCandidate).Path
$fixture = Get-Content -LiteralPath $fixtureFile -Raw |
    ConvertFrom-Json

function Convert-ToInvariantArgument {
    param([Parameter(Mandatory)]$Value)

    return [Convert]::ToString(
        [double]$Value,
        [Globalization.CultureInfo]::InvariantCulture
    )
}

function Convert-PlacementArguments {
    param([Parameter(Mandatory)]$Placement)

    return @(
        Convert-ToInvariantArgument $Placement.position[0]
        Convert-ToInvariantArgument $Placement.position[1]
        Convert-ToInvariantArgument $Placement.position[2]
        Convert-ToInvariantArgument $Placement.orientation[0]
        Convert-ToInvariantArgument $Placement.orientation[1]
        Convert-ToInvariantArgument $Placement.orientation[2]
    )
}

$settingsPath = Join-Path $workspace '.itconfig\lastchaos-test.settings.psd1'
if (-not (Test-Path -LiteralPath $settingsPath)) {
    throw "Falta la configuración local de pruebas: $settingsPath"
}
$settings = Import-PowerShellDataFile $settingsPath
if ([string]::IsNullOrWhiteSpace($settings.Username) -or
    [string]::IsNullOrWhiteSpace($settings.Password)) {
    throw 'La configuración local no contiene Username y Password.'
}
if (Get-Process Nksp -ErrorAction SilentlyContinue) {
    throw 'Cierra el cliente que ya está abierto antes de iniciar la reproducción.'
}

$stamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$captureName = "repro-$stamp"
$captureFile = Join-Path $workspace (
    ".itconfig\dx12-camera-captures\camera-$captureName.json"
)
if ([string]::IsNullOrWhiteSpace($ScreenshotPath)) {
    $ScreenshotPath = Join-Path $workspace (
        ".itconfig\dx12-camera-replays\repro-$stamp.png"
    )
}
else {
    $ScreenshotPath = if ([IO.Path]::IsPathRooted($ScreenshotPath)) {
        [IO.Path]::GetFullPath($ScreenshotPath)
    }
    else {
        [IO.Path]::GetFullPath((Join-Path $workspace $ScreenshotPath))
    }
}
$screenshotDirectory = Split-Path -Parent $ScreenshotPath
[IO.Directory]::CreateDirectory($screenshotDirectory) | Out-Null

$arguments = @(
    'fkzktlfgod!'
    '+testautologin'
    $settings.Username
    $settings.Password
    '+testserver'
    $ServerIndex
    '+testchannel'
    $ChannelIndex
    '+testcharacter'
    $CharacterIndex
    '+testplayerplacement'
) + (Convert-PlacementArguments $fixture.player) + @(
    '+testviewpoint'
) + (Convert-PlacementArguments $fixture.viewpoint) + @(
    '+testcameraangle'
    (Convert-ToInvariantArgument $fixture.networkCameraAngle)
    '+testworldviewdelay'
    $WorldDelaySeconds
    '+testworldviewhold'
    $CameraHoldSeconds
    '+testcapture'
    $captureName
)

$env:LASTCHAOS_DX12_UI_COMPARE = 'replace'
$env:LASTCHAOS_DX12_3D_REPLACE_ALL = 'enabled'
& (Join-Path $PSScriptRoot 'set-client-server.ps1') `
    -Address $ServerAddress
$clientRoot = Join-Path $workspace 'client'
$executable = Join-Path $clientRoot 'Bin\Nksp.exe'
$buildRoot = Join-Path $clientRoot 'build\v145\x64'
foreach ($binary in @('Engine.dll', 'Nksp.exe')) {
    $source = Join-Path $buildRoot $binary
    if (-not (Test-Path -LiteralPath $source)) {
        throw "Falta la compilación moderna: $source"
    }
    $destination = Join-Path $clientRoot "Bin\$binary"
    if ((Test-Path -LiteralPath $destination) -and
        (Get-FileHash -LiteralPath $source).Hash -eq
            (Get-FileHash -LiteralPath $destination).Hash) {
        continue
    }
    Copy-Item `
        -LiteralPath $source `
        -Destination $destination `
        -Force
}
$process = Start-Process `
    -FilePath $executable `
    -ArgumentList $arguments `
    -WorkingDirectory $clientRoot `
    -PassThru

$deadline = (Get-Date).AddSeconds($TimeoutSeconds)
while ((Get-Date) -lt $deadline) {
    if ($process.HasExited) {
        throw "El cliente terminó antes de restaurar la cámara (código $($process.ExitCode))."
    }
    if (Test-Path -LiteralPath $captureFile) {
        break
    }
    Start-Sleep -Milliseconds 250
    $process.Refresh()
}
if (-not (Test-Path -LiteralPath $captureFile)) {
    throw "El cliente no confirmó la cámara en $TimeoutSeconds segundos."
}

Start-Sleep -Milliseconds 500
$captureScript = Join-Path $workspace '.itconfig\capture-lastchaos-window.ps1'
& $captureScript -Destination $ScreenshotPath | Out-Null

[pscustomobject]@{
    ProcessId = $process.Id
    Fixture = $fixtureFile
    VerifiedCamera = $captureFile
    Screenshot = $ScreenshotPath
}
