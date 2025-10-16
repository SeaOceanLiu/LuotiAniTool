# LuotiAniTool 项目重新构建脚本
Write-Host "================================" -ForegroundColor Green
Write-Host "LuotiAniTool 项目重新构建脚本" -ForegroundColor Green
Write-Host "================================" -ForegroundColor Green
Write-Host ""

# 获取项目根目录
$ProjectRoot = Join-Path $PSScriptRoot ".."
$ProjectRoot = Resolve-Path $ProjectRoot

Write-Host "项目根目录: $ProjectRoot" -ForegroundColor Yellow
Write-Host ""

Write-Host "正在清理构建目录..." -ForegroundColor Cyan
& "$ProjectRoot\build_scripts\clean.ps1"

Write-Host ""
Write-Host "正在重新构建项目..." -ForegroundColor Cyan
Write-Host ""

# 切换到项目根目录并调用构建脚本
Set-Location -Path $ProjectRoot
& "$ProjectRoot\build_scripts\build.ps1" $args[0]

Write-Host ""
Write-Host "重新构建完成！" -ForegroundColor Green
Write-Host ""
Write-Host "按任意键退出..." -ForegroundColor Yellow
$host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")