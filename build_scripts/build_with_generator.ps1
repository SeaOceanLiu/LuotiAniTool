# PowerShell构建脚本 - 支持指定生成器
param(
    [Parameter(Mandatory=$false)]
    [ValidateSet("vs2022", "vs2019", "vs2017", "mingw")]
    [string]$Generator = "vs2022",

    [Parameter(Mandatory=$false)]
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug"
)

# 获取项目根目录
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectRoot = Join-Path $ScriptDir ".." | Resolve-Path

Write-Host "使用生成器: $Generator" -ForegroundColor Cyan
Write-Host "使用配置: $Configuration" -ForegroundColor Cyan

# 检测Visual Studio环境
$vsDetected = $false
$vsVersion = ""
$vsPath = ""

if (Test-Path env:VS170COMNTOOLS) {
    Write-Host "检测到Visual Studio 2022环境" -ForegroundColor Green
    $vsDetected = $true
    $vsVersion = "2022"
    $vsPath = $env:VS170COMNTOOLS
} elseif (Test-Path env:VS160COMNTOOLS) {
    Write-Host "检测到Visual Studio 2019环境" -ForegroundColor Green
    $vsDetected = $true
    $vsVersion = "2019"
    $vsPath = $env:VS160COMNTOOLS
} elseif (Test-Path env:VS150COMNTOOLS) {
    Write-Host "检测到Visual Studio 2017环境" -ForegroundColor Green
    $vsDetected = $true
    $vsVersion = "2017"
    $vsPath = $env:VS150COMNTOOLS
} else {
    # 检查Visual Studio Build Tools
    $buildToolsPaths = @(
        "d:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\",
        "c:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\",
        "d:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\",
        "c:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\"
    )

    foreach ($path in $buildToolsPaths) {
        if (Test-Path "$path\VsDevCmd.bat") {
            $version = if ($path -like "*2022*") { "2022" } elseif ($path -like "*2019*") { "2019" } else { "Unknown" }
            Write-Host "检测到Visual Studio $version Build Tools" -ForegroundColor Green
            $vsDetected = $true
            $vsVersion = $version
            $vsPath = $path
            break
        }
    }
}

# 检查环境是否满足要求
switch ($Generator) {
    {$_ -in "vs2022", "vs2019", "vs2017"} {
        if (-not $vsDetected) {
            Write-Host "未检测到Visual Studio环境，请安装相应的Visual Studio版本或Build Tools后重试。" -ForegroundColor Red
            Pause
            exit 1
        }

        # 检查请求的版本是否与检测到的版本匹配
        if ($Generator -eq "vs2022" -and $vsVersion -ne "2022") {
            Write-Host "请求的Visual Studio 2022版本与检测到的版本($vsVersion)不匹配。" -ForegroundColor Yellow
        } elseif ($Generator -eq "vs2019" -and $vsVersion -ne "2019") {
            Write-Host "请求的Visual Studio 2019版本与检测到的版本($vsVersion)不匹配。" -ForegroundColor Yellow
        } elseif ($Generator -eq "vs2017" -and $vsVersion -ne "2017") {
            Write-Host "请求的Visual Studio 2017版本与检测到的版本($vsVersion)不匹配。" -ForegroundColor Yellow
        }

        # 初始化Visual Studio环境
        & "$vsPath\VsDevCmd.bat" -arch=x64 | Out-Null
    }
    "mingw" {
        try {
            $gccResult = Get-Command gcc -ErrorAction Stop
        } catch {
            Write-Host "未检测到MinGW环境，请安装MinGW后重试。" -ForegroundColor Red
            Pause
            exit 1
        }
    }
}

# 切换到项目根目录
Set-Location $ProjectRoot

# 清理旧的构建缓存
if (Test-Path "build\CMakeCache.txt") {
    Remove-Item "build\CMakeCache.txt" -Force
}
if (Test-Path "build\CMakeFiles") {
    Remove-Item "build\CMakeFiles" -Recurse -Force
}

# 创建构建目录
if (-not (Test-Path build)) {
    New-Item -ItemType Directory -Name build | Out-Null
}
Set-Location build

# 根据参数选择生成器
switch ($Generator) {
    "vs2022" {
        Write-Host "使用Visual Studio 2022生成器" -ForegroundColor Yellow
        cmake .. -G "Visual Studio 17 2022"
    }
    "vs2019" {
        Write-Host "使用Visual Studio 2019生成器" -ForegroundColor Yellow
        cmake .. -G "Visual Studio 16 2019"
    }
    "vs2017" {
        Write-Host "使用Visual Studio 2017生成器" -ForegroundColor Yellow
        cmake .. -G "Visual Studio 15 2017"
    }
    "mingw" {
        Write-Host "使用MinGW生成器" -ForegroundColor Yellow
        cmake .. -G "MinGW Makefiles"
    }
}

# 检查CMake是否成功
if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake配置失败！" -ForegroundColor Red
    Set-Location ..
    Pause
    exit 1
}

# 编译项目
switch ($Generator) {
    {$_ -in "vs2022", "vs2019", "vs2017"} {
        msbuild LuotiAniTool.sln /p:Configuration=$Configuration /m
    }
    "mingw" {
        mingw32-make -j$env:NUMBER_OF_PROCESSORS
    }
}

# 检查编译是否成功
if ($LASTEXITCODE -ne 0) {
    Write-Host "编译失败！" -ForegroundColor Red
    Set-Location ..
    Pause
    exit 1
}

Set-Location ..

Write-Host ""
Write-Host "构建完成！" -ForegroundColor Green
Pause