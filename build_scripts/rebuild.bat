@echo off
setlocal

:: 获取项目根目录
set PROJECT_ROOT=%~dp0..
:: 规范化路径
pushd "%PROJECT_ROOT%"
set PROJECT_ROOT=%CD%
popd

echo ================================
echo LuotiAniTool 项目重新构建脚本
echo ================================
echo.

echo 正在清理构建目录...
call "%PROJECT_ROOT%\build_scripts\clean.bat"

echo.
echo 正在重新构建项目...
echo.

:: 切换到项目根目录并调用构建脚本
cd /d "%PROJECT_ROOT%"
call build_scripts\build.bat %1

echo.
echo 重新构建完成！
pause