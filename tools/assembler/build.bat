@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if not exist "%LOCALAPPDATA%\TempMSVC" mkdir "%LOCALAPPDATA%\TempMSVC" 2>nul
set "TMP=%LOCALAPPDATA%\TempMSVC"
set "TEMP=%LOCALAPPDATA%\TempMSVC"
cl /nologo /W4 /D_CRT_SECURE_NO_WARNINGS /Fe:asm.exe main.c parser.c codificador.c saidas.c
if errorlevel 1 goto :fim

asm.exe exemplo.asm
asm.exe exemplo.asm -o program.stim

:fim
