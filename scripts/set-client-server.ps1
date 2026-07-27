[CmdletBinding()]
param(
    [Parameter(Mandatory)]
    [ValidatePattern(
        '^(?:(?:25[0-5]|2[0-4]\d|1?\d?\d)\.){3}(?:25[0-5]|2[0-4]\d|1?\d?\d)$'
    )]
    [string]$Address,

    [ValidateRange(1, 65535)]
    [int]$Port = 4001,

    [string]$Name = "LastChaos",

    [string]$ClientRoot
)

$ErrorActionPreference = "Stop"

if ([string]::IsNullOrWhiteSpace($ClientRoot)) {
    $ClientRoot = Join-Path $PSScriptRoot "..\client"
}
$resolvedClientRoot = (Resolve-Path -LiteralPath $ClientRoot).Path
$outputPath = Join-Path $resolvedClientRoot "sl.dta"
$record = "${Name} ${Address} ${Port} 1000 800"
$recordBytes = [Text.Encoding]::ASCII.GetBytes($record)

$stream = [IO.MemoryStream]::new()
$writer = [IO.BinaryWriter]::new($stream)
try {
    $initialBytes = [byte[]](0x4C, 0x43, 0x32, 0x30, 0x31, 0x38)
    $writer.Write($initialBytes)

    $previousValue = 0x13572468
    $key = [byte]0x5A
    $writer.Write([int]$previousValue)
    $writer.Write($key)

    $encodedServerCount = $previousValue + 1
    $writer.Write([int]$encodedServerCount)
    $previousValue = $encodedServerCount

    $encodedLength = $previousValue + $recordBytes.Length
    $writer.Write([int]$encodedLength)

    foreach ($plainByte in $recordBytes) {
        $encodedByte = [byte](($plainByte + $key) -band 0xFF)
        $writer.Write($encodedByte)
        $key = $encodedByte
    }
    $writer.Flush()
    [IO.File]::WriteAllBytes($outputPath, $stream.ToArray())
}
finally {
    $writer.Dispose()
    $stream.Dispose()
}

Write-Output "Servidor del cliente configurado: ${Address}:${Port}"
