@echo off
for %%* in (.) do set ProjectName=%%~n*
echo %ProjectName%
make clean
rm -fr %ProjectName%.3ds %ProjectName%.cia
rm resources\banner.bin >nul 2>nul
rm resources\icon.bin >nul 2>nul
rm resources\banner.bnr >nul 2>nul
rm resources\icon.icn >nul 2>nul
call build.bat