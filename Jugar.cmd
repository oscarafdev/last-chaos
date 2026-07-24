@echo off
setlocal

cd /d "%~dp0client"
call multi.bat

if errorlevel 1 (
  echo.
  echo No se pudo iniciar Last Chaos.
  pause
)
