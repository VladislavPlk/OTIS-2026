ЛАБОРАТОРНАЯ РАБОТА №3 — ИНСТРУКЦИЯ

ВАЖНО: этот архив содержит только папку task_03.
Исходники моделей намеренно НЕ включены, чтобы не дублировать ЛР1.

КАК РАЗМЕСТИТЬ:
1. Распакуйте папку task_03 в trunk/ii03017/.
2. Убедитесь, что рядом уже существуют:
   trunk/ii03017/task_01/src/Model.h
   trunk/ii03017/task_01/src/Model1_3.h/.cpp
   trunk/ii03017/task_01/src/Model2_5.h/.cpp
   trunk/ii03017/task_01/src/Model3_7.h/.cpp
3. Добавьте папку MinGW-w64in в PATH (в ней должны находиться g++.exe и mingw32-make.exe).
4. Откройте новый терминал в папке task_03.
5. Запустите run_all.bat. Скрипт явно выбирает генератор MinGW Makefiles и удаляет старую папку build, чтобы не использовать сохранённую конфигурацию NMake.

ЗАВИСИМОСТИ:
- CMake
- MinGW-w64 (g++ и mingw32-make должны быть доступны через PATH)
- C++17
- Git и доступ в Интернет при первой загрузке Google Test
- Doxygen + Graphviz
- Python 3 + matplotlib (для графика)

Если собираете вручную через CMD:
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_MAKE_PROGRAM=mingw32-make -DOTIS_BUILD_TESTS=OFF
cmake --build build

Если CMake не находит task_01, укажите путь явно:
cmake -S . -B build -DOTIS_TASK01_SRC="ПОЛНЫЙ_ПУТЬ_К_task_01/src"
