@echo off
setlocal

echo 正在检测编译环境...

:: 获取项目根目录
set PROJECT_ROOT=%~dp0..
:: 规范化路径
pushd "%PROJECT_ROOT%"
set PROJECT_ROOT=%CD%
popd

:: 检测Visual Studio环境
set VSDETECTED=0

:: 检查Visual Studio Build Tools
if exist "d:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat" (
    echo 检测到Visual Studio 2022 Build Tools
    set VSDETECTED=1
    call "d:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
) else if exist "c:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat" (
    echo 检测到Visual Studio 2022 Build Tools (C盘)
    set VSDETECTED=1
    call "c:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
)

:: 如果没有检测到Visual Studio，则尝试检测MinGW
if %VSDETECTED%==0 (
    where gcc >nul 2>&1
    if %errorlevel% equ 0 (
        echo 检测到MinGW环境
        set COMPILER=MinGW
    ) else (
        echo 未检测到Visual Studio或MinGW环境，请安装任一环境后重试。
        pause
        exit /b 1
    )
) else (
    set COMPILER=MSVC
)

:: 设置默认配置为Debug
set CONFIGURATION=Debug
if "%1"=="Release" set CONFIGURATION=Release
if "%1"=="release" set CONFIGURATION=Release
if "%1"=="RELEASE" set CONFIGURATION=Release

echo 使用配置: %CONFIGURATION%

:: 切换到项目根目录
cd /d "%PROJECT_ROOT%"

:: 清理旧的构建缓存
if exist "build\CMakeCache.txt" del "build\CMakeCache.txt"
if exist "build\CMakeFiles" rd /s /q "build\CMakeFiles"

:: 创建构建目录
if not exist build mkdir build
cd build

:: 根据检测到的编译器选择生成器
if "%COMPILER%"=="MSVC" (
    echo 使用Visual Studio生成器
    cmake .. -G "Visual Studio 17 2022"
) else (
    echo 使用MinGW生成器
    cmake .. -G "MinGW Makefiles"
)

:: 检查CMake是否成功
if errorlevel 1 (
    echo CMake配置失败！
    cd ..
    pause
    exit /b 1
)

:: 编译项目
if "%COMPILER%"=="MSVC" (
        %CONFIGURATION% /m
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