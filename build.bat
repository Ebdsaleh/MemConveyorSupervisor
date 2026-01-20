@echo off
setlocal enabledelayedexpansion
CLS
:: --- PRE-FLIGHT CHECK ---
if not exist "CMakeLists.txt" (
    echo [!] ERROR: CMakeLists.txt not found in this folder.
    echo [!] Current Path: %CD%
    echo [!] Please move CMakeLists.txt to the root folder.
    pause
    exit /b
)

echo [BUILD] Initializing CMake Laboratory (MSVC Edition)...

:: 1. Clear previous build attempts
if exist build ( rd /s /q build )
mkdir build
cd build

:: 2. Configure using the Visual Studio 2022 Generator
:: This automatically finds your Developer Command Prompt settings
cmake -G "Visual Studio 17 2022" -A x64 ..

if %ERRORLEVEL% EQU 0 (
    echo [BUILD] Compiling MemConveyorSupervisor with MSVC...
    :: Use the --config Release flag for a high-performance build
    cmake --build . --config Release
) else (
    echo [ERROR] CMake configuration failed! Check your Developer Command Prompt.
    pause
    exit /b
)

:: --- 3. RUN TEST ---
cd ..
:: Check the root Release folder instead of the build folder
if exist "Release\MemConveyor.exe" (
    echo [SUCCESS] Bridge Established.
    echo ----------------------------------------
    "Release\MemConveyor.exe"
) else (
    echo [!] ERROR: Executable not found at %CD%\Release\MemConveyor.exe
)

pause