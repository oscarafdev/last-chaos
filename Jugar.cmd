@echo off
setlocal

cd /d "%~dp0client"

rem Perfil autoritativo de DirectX 12 usado por el cliente modernizado.
set "LASTCHAOS_DX12_UI_COMPARE=replace"
set "LASTCHAOS_DX12_3D_REPLACE_ALL=enabled"

call multi.bat

if errorlevel 1 (
  echo.
  echo No se pudo iniciar Last Chaos.
  pause
)
