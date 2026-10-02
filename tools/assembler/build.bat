@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
cl /nologo /W4 /D_CRT_SECURE_NO_WARNINGS /Fe:asm.exe main.c parser.c codificador.c saidas.c
if errorlevel 1 goto :fim

asm.exe exemplo.asm

:fim
