@echo off
setlocal EnableExtensions
cd /d "%~dp0"

echo =========================================
echo OTIS Task 03 - GCC / GNU Make build
echo =========================================

rem Paths confirmed on this computer:
set "MINGW_BIN=C:\Users\user\gcc\bin"
set "CXX_PATH=%MINGW_BIN%\g++.exe"
set "MAKE_PATH=%MINGW_BIN%\make.exe"

rem Add GCC tools to PATH so compiler runtime tools can be found.
set "PATH=%MINGW_BIN%;%PATH%"

where cmake >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake is not found in PATH.
    goto :error
)

if not exist "%CXX_PATH%" (
    echo ERROR: g++.exe was not found:
    echo %CXX_PATH%
    goto :error
)

if not exist "%MAKE_PATH%" (
    echo ERROR: make.exe was not found:
    echo %MAKE_PATH%
    goto :error
)

echo Compiler: %CXX_PATH%
echo Make:     %MAKE_PATH%
"%MAKE_PATH%" --version
if errorlevel 1 goto :error

echo.
echo [1/4] Configure project with MinGW Makefiles...
rem Remove the previous CMake cache (it may contain the NMake generator).
if exist build rmdir /s /q build

cmake -S . -B build -G "MinGW Makefiles" ^
  -DCMAKE_CXX_COMPILER="%CXX_PATH%" ^
  -DCMAKE_MAKE_PROGRAM="%MAKE_PATH%" ^
  -DOTIS_BUILD_TESTS=ON

if errorlevel 1 goto :error

echo.
echo [2/4] Build application and Google Test...
cmake --build build --config Release
if errorlevel 1 goto :error

echo.
echo [3/4] Run unit tests...
ctest --test-dir build --output-on-failure
if errorlevel 1 goto :error

echo.
echo [4/4] Generate Doxygen documentation...
where doxygen >nul 2>&1
if errorlevel 1 (
    echo WARNING: Doxygen is not found. Build and tests passed; documentation skipped.
) else (
    doxygen Doxyfile
    if errorlevel 1 goto :error
)

echo.
echo SUCCESS: build and tests completed.
echo Application: build\src\otis_task_03.exe
if exist doc\html\index.html echo Documentation: doc\html\index.html
pause
exit /b 0

:error
echo.
echo ERROR: operation failed. Read the messages above.
pause
exit /b 1
