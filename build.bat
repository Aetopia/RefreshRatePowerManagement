@echo off
cd "%~dp0"
gcc -mwindows -s WinMain.c -o RefreshRatePowerManagement.exe
upx --best --ultra-brute RefreshRatePowerManagement.exe>nul 2>&1