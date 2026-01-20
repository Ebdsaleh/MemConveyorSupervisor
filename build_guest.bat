@echo off
set WATCOM=C:\WATCOM
set PATH=%WATCOM%\binnt64;%PATH%
set INCLUDE=%WATCOM%\h

echo [BUILD] Compiling 16-bit Guest for 486DX...
:: wcl is the Watcom Compile & Link command
:: -bt=dos means target DOS
:: -ms means "Small Memory Model" (Perfect for 16MB RAM)
wcl -bt=dos -ms src\guest_test.c -fe=GUEST.EXE

if exist GUEST.EXE (
    echo [SUCCESS] GUEST.EXE created. Ready for the simulation.
)
pause