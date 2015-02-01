@echo off
for %%* in (.) do set ProjectName=%%~n*
echo %ProjectName%
make
arm-none-eabi-strip %ProjectName%.elf

cd %~dp0\resources\ctpk_48x48icon\
call convert.bat
cd %~dp0
cd %~dp0\resources\ctpk_24x24icon\
call convert.bat
cd %~dp0

cd %~dp0\resources\cbmd_bannerImage\
call convert.bat
cd %~dp0\resources\

if exist banner.bin rm banner.bin (
if exist icon.bin rm icon.bin (
if exist banner.bnr rm banner.bnr (
if exist icon.icn rm icon.icn (

_buildscript.py

rename *.bnr *.bin
rename *.icn *.bin

cd %~dp0

resources\makerom.exe -f cci -rsf resources\gw_workaround.rsf -target d -exefslogo -elf %ProjectName%.elf -icon resources\icon.bin -banner resources\banner.bin -o %ProjectName%.3ds
resources\makerom.exe -f cia -o %ProjectName%.cia -elf %ProjectName%.elf -rsf resources\build_cia.rsf -icon resources\icon.bin -banner resources\banner.bin -exefslogo -target t
pause