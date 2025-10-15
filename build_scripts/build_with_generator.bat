@echo off
setlocal enabledelayedexpansion

echo 正在检测编译环境...

:: 获取项目根目录
set PROJECT_ROOT=%~dp0..
:: 规范化路径
pushd "%PROJECT_ROOT%"
set PROJECT_ROOT=%CD%
popd

:: 检测Visual Studio环境
set VSDETECTED=0
set VSCOMNTOOLS=

if exist "%VS170COMNTOOLS%" (
    echo 检测到Visual Studio 2022环境
    set VSDETECTED=1
    set VSCOMNTOOLS=%VS170COMNTOOLS%
    call "%VS170COMNTOOLS%VsDevCmd.bat"
) else if exist "%VS160COMNTOOLS%" (
    echo 检测到Visual Studio 2019环境
    set VSDETECTED=1
    set VSCOMNTOOLS=%VS160COMNTOOLS%
    call "%VS160COMNTOOLS%VsDevCmd.bat"
) else if exist "%VS150COMNTOOLS%" (
    echo 检测到Visual Studio 2017环境
    set VSDETECTED=1
    set VSCOMNTOOLS=%VS150COMNTOOLS%
    call "%VS150COMNTOOLS%VsDevCmd.bat"
) else (
    :: 检查Visual Studio Build Tools
    if exist "d:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat" (
        echo 检测到Visual Studio 2022 Build Tools
        set VSDETECTED=1
        set VSCOMNTOOLS=d:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\
        call "d:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
    ) else if exist "c:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat" (
        echo 检测到Visual Studio 2022 Build Tools
        set VSDETECTED=1
        set VSCOMNTOOLS=c:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\
        call "c:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
    ) else if exist "d:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat" (
        echo 检测到Visual Studio 2019 Build Tools
        set VSDETECTED=1
        set VSCOMNTOOLS=d:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\
        call "d:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"
    ) else if exist "c:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat" (
        echo 检测到Visual Studio 2019 Build Tools
        set VSDETECTED=1
        set VSCOMNTOOLS=c:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\
        call "c:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"
    )
)

:: 设置默认配置为Debug
set CONFIGURATION=Debug
if "%2"=="Release" set CONFIGURATION=Release
if "%2"=="release" set CONFIGURATION=Release
if "%2"=="RELEASE" set CONFIGURATION=Release

echo 使用配置: %CONFIGURATION%

:: 切换到项目根目录
cd /d "%PROJECT_ROOT%"

:: 清理旧的构建缓存
if exist "build\CMakeCache.txt" del "build\CMakeCache.txt"
if exist "build\CMakeFiles" rd /s /q "build\CMakeFiles"

:: 创建构建目录
if not exist build mkdir build
cd build

:: 根据参数选择生成器
if "%1"=="vs2022" (
    if %VSDETECTED%==0 (
        echo 未检测到Visual Studio环境，请安装Visual Studio 2022或Build Tools后重试。
        cd ..
        pause
        exit /b 1
    )
    echo 使用Visual Studio 2022生成器
    :: 初始化Visual Studio环境
    if exist "%VSCOMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat" (
        call "%VSCOMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat" x64
    )
    cmake .. -G "Visual Studio 17 2022"
) else if "%1"=="vs2019" (
    if %VSDETECTED%==0 (
        echo 未检测到Visual Studio环境，请安装Visual Studio 2019或Build Tools后重试。
        cd ..
        pause
        exit /b 1
    )
    echo 使用Visual Studio 2019生成器
    :: 初始化Visual Studio环境
    if exist "%VSCOMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat" (
        call "%VSCOMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat" x64
    )
    cmake .. -G "Visual Studio 16 2019"
) else if "%1"=="vs2017" (
    if %VSDETECTED%==0 (
        echo 未检测到Visual Studio环境，请安装Visual Studio 2017或Build Tools后重试。
        cd ..
        pause
        exit /b 1
    )
    echo 使用Visual Studio 2017生成器
    :: 初始化Visual Studio环境
    if exist "%VSCOMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat" (
        call "%VSCOMNTOOLS%..\..\VC\Auxiliary\Build\vcvarsall.bat" x64
    )
    cmake .. -G "Visual Studio 15 2017"
) else if "%1"=="mingw" (
    where gcc >nul 2>&1
    if errorlevel 1 (
        echo 未检测到MinGW环境，请安装MinGW后重试。
        cd ..
        pause
        exit /b 1
    )
    echo 使用MinGW生成器
    cmake .. -G "MinGW Makefiles"
) else (
    echo 用法: build_with_generator.bat [vs2022^|vs2019^|vs2017^|mingw] [Debug^|Release]
    echo.
    echo 示例:
    echo   build_with_generator.bat vs2022 Debug
    echo   build_with_generator.bat mingw Release
    cd ..
    exit /b 1
)

:: 检查CMake是否成功
if errorlevel 1 (
    echo CMake配置失败！
    cd ..
    pause
    exit /b 1
)

:: 编译项目
if "%1"=="vs2022" (
    msbuild LuotiAniTool.sln /p:Configuration=%CONFIGURATION% /m
) else if "%1"=="vs2019" (
    msbuild LuotiAniTool.sln /p:Configuration=%CONFIGURATION% /m
) else if "%1"=="vs2017" (
    msbuild LuotiAniTool.sln /p:Configuration=%CONFIGURATION% /m
) else (
    mingw32-make -j%NUMBER_OF_PROCESSORS%
)

:: 检查编译是否成功
if errorlevel 1 (
    echo 编译失败！
    cd ..
    pause
    exit /b 1
)

cd ..

echo.
echo 构建完成！
pause