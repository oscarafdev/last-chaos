@echo off
setlocal

cd /d "%~dp0"
powershell -NoProfile -ExecutionPolicy Bypass -File ".\scripts\set-client-language.ps1" -Language es

if errorlevel 1 (
  echo.
  echo No se pudo configurar el cliente en espanol.
  pause
  exit /b 1
)

call Jugar.cmd

