@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if not exist "%LOCALAPPDATA%\TempMSVC" mkdir "%LOCALAPPDATA%\TempMSVC" 2>nul
set "TMP=%LOCALAPPDATA%\TempMSVC"
set "TEMP=%LOCALAPPDATA%\TempMSVC"
cl /nologo /W4 /D_CRT_SECURE_NO_WARNINGS /I. /Icore /Iloaders /Fe:sim.exe main.c runner.c tracer.c core\alu.c core\control.c core\mux.c loaders\stim_loader.c loaders\test_loader.c loaders\mem_loader.c loaders\loader_factory.c
if errorlevel 1 goto :fim

if exist "..\assembler\program.stim" (
    sim.exe ..\assembler\program.stim --verbose
)

:fim
