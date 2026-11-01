@echo off
chcp 65001 >nul

echo ========================================
echo          СБОРКА ПРОЕКТА
echo ========================================

if not exist build (
    mkdir build
)

cd build

echo.
echo [1/2] Конфигурация проекта (CMake)...
cmake ..

if errorlevel 1 (
    echo.
    echo ОШИБКА: CMake не смог настроить проект.
    pause
    exit /b 1
)

echo.
echo [2/2] Компиляция кода...
cmake --build . --config Debug

if errorlevel 1 (
    echo.
    echo ОШИБКА: в коде есть ошибки, смотри текст выше.
    pause
    exit /b 1
)

echo.
echo ========================================
echo СБОРКА УСПЕШНО ЗАВЕРШЕНА!
echo ========================================
echo Программа: build\Debug\SimulationModels.exe

pause