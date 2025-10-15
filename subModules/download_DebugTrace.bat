@echo off
REM Download DebugTrace from GitHub
REM This script downloads or updates the DebugTrace submodule

echo Starting DebugTrace download...

set SCRIPT_DIR=%~dp0
set DEBUG_TRACE_DIR=%SCRIPT_DIR%DebugTrace

REM Check if DebugTrace directory exists
if exist "%DEBUG_TRACE_DIR%" (
    echo DebugTrace directory already exists. Checking if it's a git repository...

    REM Check if it's a git repository
    if exist "%DEBUG_TRACE_DIR%\.git" (
        echo Updating existing DebugTrace repository...

        REM Update the existing repository
        cd /d "%DEBUG_TRACE_DIR%"
        git pull origin main
        if %ERRORLEVEL% EQU 0 (
            echo DebugTrace repository updated successfully!
        ) else (
            echo Failed to update DebugTrace repository.
        )
    ) else (
        echo Directory exists but is not a git repository. Removing and re-cloning...
        rd /s /q "%DEBUG_TRACE_DIR%"
        cd /d "%SCRIPT_DIR%"
        git clone https://github.com/SeaOceanLiu/DebugTrace.git DebugTrace
        if %ERRORLEVEL% EQU 0 (
            echo DebugTrace repository cloned successfully!
        ) else (
            echo Failed to clone DebugTrace repository.
        )
    )
) else (
    echo DebugTrace directory does not exist. Cloning repository...

    REM Clone the repository
    cd /d "%SCRIPT_DIR%"
    git clone https://github.com/SeaOceanLiu/DebugTrace.git DebugTrace
    if %ERRORLEVEL% EQU 0 (
        echo DebugTrace repository cloned successfully!
    ) else (
        echo Failed to clone DebugTrace repository.
    )
)

echo DebugTrace download process completed.
pause