# LuotiAniTool 项目清理脚本
Write-Host "================================" -ForegroundColor Green
Write-Host "LuotiAniTool 项目清理脚本" -ForegroundColor Green
Write-Host "================================" -ForegroundColor Green
Write-Host ""

# 获取项目根目录
$ProjectRoot = Join-Path $PSScriptRoot ".."
$ProjectRoot = Resolve-Path $ProjectRoot

Write-Host "项目根目录: $ProjectRoot" -ForegroundColor Yellow
Write-Host ""

# 删除构建目录
Write-Host "正在删除构建目录..." -ForegroundColor Cyan
$BuildDirs = @("build", "build_temp")

foreach ($dir in $BuildDirs) {
    $FullPath = Join-Path $ProjectRoot $dir
    if (Test-Path $FullPath) {
        Remove-Item -Path $FullPath -Recurse -Force
        Write-Host "已删除 $dir 目录" -ForegroundColor Green
    } else {
        Write-Host "$dir 目录不存在" -ForegroundColor Gray
    }
}

# 删除构建脚本目录中的构建产物
Write-Host "正在删除构建脚本目录中的构建产物..." -ForegroundColor Cyan
$BuildScriptDirs = @(
    "Debug",
    "Release",
    "UIControls\Debug",
    "UIControls\Release",
    "DebugTrace.dir",
    "UIControls.dir",
    "LuotiAniTool.dir"
)

foreach ($dir in $BuildScriptDirs) {
    $FullPath = Join-Path $ProjectRoot "build_scripts" $dir
    if (Test-Path $FullPath) {
        Remove-Item -Path $FullPath -Recurse -Force
        Write-Host "已删除 build_scripts\$dir 目录" -ForegroundColor Green
    } else {
        Write-Host "build_scripts\$dir 目录不存在" -ForegroundColor Gray
    }
}

# 删除CMake缓存文件
Write-Host "正在删除CMake缓存文件..." -ForegroundColor Cyan
$CMakeFiles = @(
    "CMakeCache.txt",
    "CMakeFiles"
)

foreach ($file in $CMakeFiles) {
    $FullPath = Join-Path $ProjectRoot "build_scripts" $file
    if (Test-Path $FullPath) {
        if (Test-Path -Path $FullPath -PathType Container) {
            Remove-Item -Path $FullPath -Recurse -Force
            Write-Host "已删除 build_scripts\$file 目录" -ForegroundColor Green
        } else {
            Remove-Item -Path $FullPath -Force
            Write-Host "已删除 build_scripts\$file 文件" -ForegroundColor Green
        }
    } else {
        Write-Host "build_scripts\$file 不存在" -ForegroundColor Gray
    }
}

# 删除UIControls目录中的CMake文件
Write-Host "正在删除UIControls目录中的CMake文件..." -ForegroundColor Cyan
$UIControlsCMakeFiles = @(
    "UIControls\CMakeFiles",
    "UIControls\UIControls.dir",
    "UIControls\cmake_install.cmake"
)

foreach ($file in $UIControlsCMakeFiles) {
    $FullPath = Join-Path $ProjectRoot "build_scripts" $file
    if (Test-Path $FullPath) {
        if (Test-Path -Path $FullPath -PathType Container) {
            Remove-Item -Path $FullPath -Recurse -Force
            Write-Host "已删除 build_scripts\$file 目录" -ForegroundColor Green
        } else {
            Remove-Item -Path $FullPath -Force
            Write-Host "已删除 build_scripts\$file 文件" -ForegroundColor Green
        }
    } else {
        Write-Host "build_scripts\$file 不存在" -ForegroundColor Gray
    }
}

# 删除Visual Studio项目文件（但保留脚本）
Write-Host "正在删除Visual Studio项目文件..." -ForegroundColor Cyan
$VSSolutionFiles = Get-ChildItem -Path (Join-Path $ProjectRoot "build_scripts") -Include "*.vcxproj", "*.vcxproj.filters", "*.sln" -File
$VSProjectFiles = Get-ChildItem -Path (Join-Path $ProjectRoot "build_scripts" "UIControls") -Include "*.vcxproj", "*.vcxproj.filters" -File -ErrorAction SilentlyContinue

foreach ($file in $VSSolutionFiles) {
    Remove-Item -Path $file.FullName -Force
    Write-Host "已删除 $($file.Name)" -ForegroundColor Green
}

foreach ($file in $VSProjectFiles) {
    Remove-Item -Path $file.FullName -Force
    Write-Host "已删除 build_scripts\UIControls\$($file.Name)" -ForegroundColor Green
}

Write-Host ""
Write-Host "清理完成！所有编译中间结果和最终结果已删除。" -ForegroundColor Green
Write-Host ""