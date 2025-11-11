@echo off
REM Windows build script for RTLIL Library using CMake
REM This script generates Visual Studio solution and builds the library

echo ================================================================
echo RTLIL Library - Windows Build Script
echo ================================================================
echo.

REM Check if CMake is available
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake not found in PATH
    echo Please install CMake from https://cmake.org/download/
    echo and add it to your system PATH
    pause
    exit /b 1
)

REM Get CMake version
echo Checking CMake version...
cmake --version
echo.

REM Create build directory
if not exist build mkdir build
cd build

REM Detect Visual Studio version
echo Detecting Visual Studio...
echo.

REM Try to detect the latest Visual Studio installation
set VS_GENERATOR=

REM Check for VS 2022
if exist "C:\Program Files\Microsoft Visual Studio\2022" (
    set VS_GENERATOR=Visual Studio 17 2022
    echo Found Visual Studio 2022
    goto :found_vs
)

REM Check for VS 2019
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019" (
    set VS_GENERATOR=Visual Studio 16 2019
    echo Found Visual Studio 2019
    goto :found_vs
)

REM Check for VS 2017
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017" (
    set VS_GENERATOR=Visual Studio 15 2017
    echo Found Visual Studio 2017
    goto :found_vs
)

REM If no VS found, ask user
echo Could not auto-detect Visual Studio installation.
echo.
echo Please select your Visual Studio version:
echo   1. Visual Studio 2022
echo   2. Visual Studio 2019
echo   3. Visual Studio 2017
echo   4. Use default generator
echo.
set /p VS_CHOICE="Enter choice (1-4): "

if "%VS_CHOICE%"=="1" set VS_GENERATOR=Visual Studio 17 2022
if "%VS_CHOICE%"=="2" set VS_GENERATOR=Visual Studio 16 2019
if "%VS_CHOICE%"=="3" set VS_GENERATOR=Visual Studio 15 2017
if "%VS_CHOICE%"=="4" set VS_GENERATOR=

:found_vs

echo.
echo ================================================================
echo Generating Visual Studio Solution...
echo ================================================================
echo.

REM Generate solution
if "%VS_GENERATOR%"=="" (
    echo Using default CMake generator...
    cmake ..
) else (
    echo Using generator: %VS_GENERATOR%
    cmake .. -G "%VS_GENERATOR%" -A x64
)

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: CMake generation failed!
    echo Please check the error messages above.
    pause
    exit /b 1
)

echo.
echo ================================================================
echo Building RTLIL Library (Release Configuration)...
echo ================================================================
echo.

REM Build Release configuration
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed!
    echo Please check the error messages above.
    pause
    exit /b 1
)

echo.
echo ================================================================
echo Build completed successfully!
echo ================================================================
echo.
echo Output files:
echo   Library:  build\lib\Release\rtlil.lib
echo   Examples: build\bin\Release\simple_example.exe
echo             build\bin\Release\complex_example.exe
echo.
echo To open in Visual Studio, run:
echo   start RTLILLibrary.sln
echo.
echo To run the examples:
echo   bin\Release\simple_example.exe
echo   bin\Release\complex_example.exe
echo.

REM Ask if user wants to run tests
set /p RUN_TEST="Do you want to run the complex example now? (Y/N): "
if /i "%RUN_TEST%"=="Y" (
    echo.
    echo Running complex_example...
    echo ================================================================
    bin\Release\complex_example.exe
    echo.
    echo JSON output saved to: bin\Release\complex_example.json
)

echo.
pause
