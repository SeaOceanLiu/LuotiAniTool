# Download DebugTrace from GitHub
# This script downloads or updates the DebugTrace submodule

param(
    [string]$RepositoryUrl = "https://github.com/SeaOceanLiu/DebugTrace.git",
    [string]$Branch = "main"
)

Write-Host "Starting DebugTrace download..." -ForegroundColor Green

# Check if DebugTrace directory exists
$debugTraceDir = Join-Path $PSScriptRoot "DebugTrace"

if (Test-Path $debugTraceDir) {
    Write-Host "DebugTrace directory already exists. Checking if it's a git repository..." -ForegroundColor Yellow

    # Check if it's a git repository
    $gitDir = Join-Path $debugTraceDir ".git"
    if (Test-Path $gitDir) {
        Write-Host "Updating existing DebugTrace repository..." -ForegroundColor Cyan

        # Update the existing repository
        Set-Location $debugTraceDir
        git pull origin $Branch
        if ($LASTEXITCODE -eq 0) {
            Write-Host "DebugTrace repository updated successfully!" -ForegroundColor Green
        } else {
            Write-Host "Failed to update DebugTrace repository." -ForegroundColor Red
        }
    } else {
        Write-Host "Directory exists but is not a git repository. Removing and re-cloning..." -ForegroundColor Yellow
        Remove-Item $debugTraceDir -Recurse -Force
        Set-Location $PSScriptRoot
        git clone $RepositoryUrl DebugTrace
        if ($LASTEXITCODE -eq 0) {
            Write-Host "DebugTrace repository cloned successfully!" -ForegroundColor Green
        } else {
            Write-Host "Failed to clone DebugTrace repository." -ForegroundColor Red
        }
    }
} else {
    Write-Host "DebugTrace directory does not exist. Cloning repository..." -ForegroundColor Cyan

    # Clone the repository
    Set-Location $PSScriptRoot
    git clone $RepositoryUrl DebugTrace
    if ($LASTEXITCODE -eq 0) {
        Write-Host "DebugTrace repository cloned successfully!" -ForegroundColor Green
    } else {
        Write-Host "Failed to clone DebugTrace repository." -ForegroundColor Red
    }
}

Write-Host "DebugTrace download process completed." -ForegroundColor Green