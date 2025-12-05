# ESP32-S3 Macropad - Wokwi Simulation Quick Start

Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  ESP32-S3 Macropad - Wokwi Setup" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

# Check if PlatformIO is available
if (-not (Get-Command "pio" -ErrorAction SilentlyContinue)) {
    Write-Host "[ERROR] PlatformIO CLI not found!" -ForegroundColor Red
    Write-Host "Please install PlatformIO: https://platformio.org/install/cli" -ForegroundColor Yellow
    exit 1
}

# Check if wokwi-cli is available
$hasWokwiCli = Get-Command "wokwi-cli" -ErrorAction SilentlyContinue

Write-Host "[1/3] Building firmware..." -ForegroundColor Green
pio run -e esp32-s3-devkitc-1

if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Build failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "[2/3] Firmware built successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "Firmware location:" -ForegroundColor Yellow
Write-Host "  .pio/build/esp32-s3-devkitc-1/firmware.elf" -ForegroundColor White
Write-Host "  .pio/build/esp32-s3-devkitc-1/firmware.bin" -ForegroundColor White
Write-Host ""

if ($hasWokwiCli) {
    Write-Host "[3/3] Starting Wokwi simulation..." -ForegroundColor Green
    Write-Host ""
    wokwi-cli --timeout 60000 .
} else {
    Write-Host "[3/3] Simulation Options:" -ForegroundColor Green
    Write-Host ""
    Write-Host "Option 1: Install Wokwi CLI" -ForegroundColor Yellow
    Write-Host "  npm install -g wokwi-cli" -ForegroundColor White
    Write-Host "  Then run: wokwi-cli --timeout 60000 ." -ForegroundColor White
    Write-Host ""
    Write-Host "Option 2: VS Code Extension" -ForegroundColor Yellow
    Write-Host "  1. Install 'Wokwi Simulator' extension" -ForegroundColor White
    Write-Host "  2. Press F1 and run 'Wokwi: Start Simulator'" -ForegroundColor White
    Write-Host ""
    Write-Host "Option 3: Wokwi Web Interface" -ForegroundColor Yellow
    Write-Host "  1. Go to https://wokwi.com/projects/new/esp32-s3-devkitc-1" -ForegroundColor White
    Write-Host "  2. Copy diagram.json contents to Wokwi editor" -ForegroundColor White
    Write-Host "  3. Upload firmware.bin" -ForegroundColor White
    Write-Host ""
}

Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "Documentation: docs/WOKWI.md" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
