@echo off
setlocal

:: 获取项目根目录
set PROJECT_ROOT=%~dp0..
:: 规范化路径
pushd "%PROJECT_ROOT%"
set PROJECT_ROOT=%CD%
popd

echo ================================
echo LuotiAniTool 项目清理脚本
echo ================================
echo.

:: 删除构建目录
echo 正在删除构建目录...
if exist "%PROJECT_ROOT%\build" (
    rd /s /q "%PROJECT_ROOT%\build"
    echo 已删除 build 目录
) else (
    echo build 目录不存在
)

if exist "%PROJECT_ROOT%\build_temp" (
    rd /s /q "%PROJECT_ROOT%\build_temp"
    echo 已删除 build_temp 目录
) else (
    echo build_temp 目录不存在
)

:: 删除构建脚本目录中的构建产物
echo 正在删除构建脚本目录中的构建产物...
if exist "%PROJECT_ROOT%\build_scripts\Debug" (
    rd /s /q "%PROJECT_ROOT%\build_scripts\Debug"
    echo 已删除 build_scripts\Debug 目录
) else (
    echo build_scripts\Debug 目录不存在
)

if exist "%PROJECT_ROOT%\build_scripts\Release" (
    rd /s /q "%PROJECT_ROOT%\build_scripts\Release"
    echo 已删除 build_scripts\Release 目录
) else (
    echo build_scripts\Release 目录不存在
)

if exist "%PROJECT_ROOT%\build_scripts\UIControls\Debug" (
    rd /s /q "%PROJECT_ROOT%\build_scripts\UIControls\Debug"
    echo 已删除 build_scripts\UIControls\Debug 目录
) else (
    echo build_scripts\UIControls\Debug 目录不存在
)

if exist "%PROJECT_ROOT%\build_scripts\UIControls\Release" (
    rd /s /q "%PROJECT_ROOT%\build_scripts\UIControls\Release"
    echo 已删除 build_scripts\UIControls\Release 目录
) else (
    echo build_scripts\UIControls\Release 目录不存在
)

if exist "%PROJECT_ROOT%\build_scripts\DebugTrace.dir" (
    rd /s /q "%PROJECT_ROOT%\build_scripts\DebugTrace.dir"
    echo 已删除 build_scripts\DebugTrace.dir 目录
) else (
    echo build_scripts\DebugTrace.dir 目录不存在
)

if exist "%PROJECT_ROOT%\build_scripts\UIControls.dir" (
    rd /s /q "%PROJECT_ROOT%\build_scripts\UIControls.dir"
    echo 已删除 build_scripts\UIControls.dir 目录
) else (
    echo build_scripts\UIControls.dir 目录不存在
)

if exist "%PROJECT_ROOT%\build_scripts\LuotiAniTool.dir" (
    rd /s /q "%PROJECT_ROOT%\build_scripts\LuotiAniTool.dir"
    echo 已删除 build_scripts\LuotiAniTool.dir 目录
) else (
    echo build_scripts\LuotiAniTool.dir 目录不存在
)

:: 删除CMake缓存文件
echo 正在删除CMake缓存文件...
if exist "%PROJECT_ROOT%\build_scripts\CMakeCache.txt" (
    del "%PROJECT_ROOT%\build_scripts\CMakeCache.txt"
    echo 已删除 build_scripts\CMakeCache.txt
) else (
    echo build_scripts\CMakeCache.txt 文件不存在
)

if exist "%PROJECT_ROOT%\build_scripts\CMakeFiles" (
    rd /s /q "%PROJECT_ROOT%\build_scripts\CMakeFiles"
    echo 已删除 build_scripts\CMakeFiles 目录
) else (
    echo build_scripts\CMakeFiles 目录不存在
)

:: 删除UIControls目录中的CMake文件
if exist "%PROJECT_ROOT%\build_scripts\UIControls\CMakeFiles" (
    rd /s /q "%PROJECT_ROOT%\build_scripts\UIControls\CMakeFiles"
    echo 已删除 build_scripts\UIControls\CMakeFiles 目录
) else (
    echo build_scripts\UIControls\CMakeFiles 目录不存在
)

if exist "%PROJECT_ROOT%\build_scripts\UIControls\UIControls.dir" (
    rd /s /q "%PROJECT_ROOT%\build_scripts\UIControls\UIControls.dir"
    echo 已删除 build_scripts\UIControls\UIControls.dir 目录
) else (
    echo build_scripts\UIControls\UIControls.dir 目录不存在
)

if exist "%PROJECT_ROOT%\build_scripts\UIControls\cmake_install.cmake" (
    del "%PROJECT_ROOT%\build_scripts\UIControls\cmake_install.cmake"
    echo 已删除 build_scripts\UIControls\cmake_install.cmake
) else (
    echo build_scripts\UIControls\cmake_install.cmake 文件不存在
)

:: 删除Visual Studio项目文件（但保留脚本）
echo 正在删除Visual Studio项目文件...
for %%f in (
    "%PROJECT_ROOT%\build_scripts\*.vcxproj"
    "%PROJECT_ROOT%\build_scripts\*.vcxproj.filters"
    "%PROJECT_ROOT%\build_scripts\*.sln"
) do (
    if exist "%%f" (
        del "%%f"
        echo 已删除 %%~nxf
    )
)

:: 删除UIControls目录中的Visual Studio项目文件
for %%f in (
    "%PROJECT_ROOT%\build_scripts\UIControls\*.vcxproj"
    "%PROJECT_ROOT%\build_scripts\UIControls\*.vcxproj.filters"
) do (
    if exist "%%f" (
        del "%%f"
        echo 已删除 build_scripts\UIControls\%%~nxf
    )
)

:: 删除未知的目录（除了保留的目录和文件）
echo 正在删除未知目录...
for /d %%d in ("%PROJECT_ROOT%\build_scripts\*") do (
    set "DIRNAME=%%~nxd"
    if /i not "%%~nxd"=="UIControls" (
        if /i not "%%~nxd"=="." (
            if /i not "%%~nxd"==".." (
                rd /s /q "%%d" >nul 2>&1
                if not errorlevel 1 (
                    echo 已删除 %%~nxd 目录
                )
            )
        )
    )
)

echo.
echo 清理完成！所有编译中间结果和最终结果已删除。
echo.