@echo off
echo Checking Visual Studio Environment...

echo.
echo Checking environment variables:
echo VS170COMNTOOLS = %VS170COMNTOOLS%
echo VS160COMNTOOLS = %VS160COMNTOOLS%
echo VS150COMNTOOLS = %VS150COMNTOOLS%

echo.
echo Checking for Build Tools:
if exist "d:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat" (
    echo Found VS 2022 Build Tools
    echo Initializing environment...
    call "d:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
) else (
    echo VS 2022 Build Tools not found
)

echo.
echo Checking for MSBuild:
where msbuild

echo.
echo Done.
pause