@echo off
REM Quarto expects `rsvg-convert` on PATH for SVG→PDF in handouts.
set "PY=%LOCALAPPDATA%\Microsoft\WindowsApps\python3.13.exe"
if not exist "%PY%" set "PY=python"
"%PY%" "%~dp0rsvg-convert.py" %*
