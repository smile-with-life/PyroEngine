@echo off
setlocal enabledelayedexpansion

set "HERE=%~dp0"
cd /d "%HERE%"

set "PY="
where python >nul 2>nul
if not errorlevel 1 set "PY=python"
if not defined PY (
  where py >nul 2>nul
  if not errorlevel 1 set "PY=py -3"
)

if not defined PY (
  echo Python not found.
  echo.
  where winget >nul 2>nul
  if not errorlevel 1 (
    echo Trying to install Python via winget...
    winget install -e --id Python.Python.3.12
    echo.
    where python >nul 2>nul
    if not errorlevel 1 set "PY=python"
    if not defined PY (
      where py >nul 2>nul
      if not errorlevel 1 set "PY=py -3"
    )
  )
)

if not defined PY (
  echo Python is required but still not found.
  echo Install Python 3.10+ and rerun install.bat.
  echo.
  echo If you have winget, you can run:
  echo   winget install -e --id Python.Python.3.12
  exit /b 1
)

set "VENV_DIR=.venv"
if not exist "%VENV_DIR%\Scripts\python.exe" (
  if exist "..\..\..\.venv\Scripts\python.exe" (
    set "VENV_DIR=..\..\..\.venv"
  ) else (
    %PY% -m venv "%VENV_DIR%"
    if errorlevel 1 exit /b 1
  )
)

call "%VENV_DIR%\Scripts\activate.bat"
if errorlevel 1 exit /b 1

%VENV_DIR%\Scripts\python.exe -m pip install --upgrade pip
if errorlevel 1 exit /b 1

%VENV_DIR%\Scripts\python.exe -m pip install -r requirements.txt
if errorlevel 1 exit /b 1

echo.
echo Installed. Run:
echo   %VENV_DIR%\Scripts\python.exe main.py --seed 1 --width 256 --depth 256 --max-height 80
