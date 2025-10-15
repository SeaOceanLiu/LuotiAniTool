@echo off
setlocal

:: 获取项目根目录
set PROJECT_ROOT=%~dp0..
:: 规范化路径
pushd "%PROJECT_ROOT%"
set PROJECT_ROOT=%CD%
popd

echo 正在清理构建目录...

:: 删除构建目录
if exist "%PROJECT_ROOT%\build" (
    echo 删除现有构建目录...
    rd /s /q "%PROJECT_ROOT%\build"
)

echo.
echo 正在重新构建项目...
echo.

:: 切换到项目根目录并调用构建脚本
cd /d "%PROJECT_ROOT%"
call build_scripts\build.bat %1

echo.
echo 重新构建完成！
pause