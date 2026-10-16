#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

#include "InputSignal.h"
#include "Model.h"
#include "Model1_3.h"
#include "Model2_5.h"
#include "Model3_7.h"
#include "StabilityAnalyzer.h"
#include "Simulation.h"

static int readInt(const std::string& prompt, int minValue)
{
    int value;

    while (true)
    {
        std::cout << prompt;

        if (std::cin >> value && value >= minValue)
            return value;

        std::cout << "Ошибка ввода. Повторите.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

static double readDouble(const std::string& prompt)
{
    double value;

    while (true)
    {
        std::cout << prompt;

        if (std::cin >> value && std::isfinite(value))
            return value;

        std::cout << "Ошибка ввода. Повторите.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

static SignalType readSignalType()
{
    const int choice = readInt(
        "\nТип входного воздействия:\n"
        "1 - ступенчатое\n"
        "2 - импульсное\n"
        "3 - гармоническое\n"
        "Ваш выбор: ", 1);

    if (choice < 1 || choice > 3)
        return readSignalType();

    return static_cast<SignalType>(choice);
}

static std::unique_ptr<Model> createModel()
{
    const int choice = readInt(
        "\nВыберите модель:\n"
        "1 - Model 1.3\n"
        "2 - Model 2.5\n"
        "3 - Model 3.7\n"
        "Ваш выбор: ", 1);

    switch (choice)
    {
    case 1:
    {
        const double a1 = readDouble("a1 = ");
        const double a2 = readDouble("a2 = ");
        const double b = readDouble("b = ");
        return std::make_unique<Model1_3>(a1, a2, b);
    }

    case 2:
    {
        const double a = readDouble("a = ");
        const double b = readDouble("b = ");
        return std::make_unique<Model2_5>(a, b);
    }

    case 3:
    {
        const double a = readDouble("a = ");
        const double b = readDouble("b = ");
        return std::make_unique<Model3_7>(a, b);
    }

    default:
        std::cout << "Нет такой модели. Повторите выбор.\n";
        return createModel();
    }
}


static void printResultsTable(std::ostream& out, const std::vector<Row>& rows)
{
    out << "\n" << std::setw(8) << "tau" << std::setw(15) << "u"
        << std::setw(15) << "y" << '\n';
    out << std::string(38, '-') << '\n';
    for (const Row& row : rows)
        out << std::setw(8) << row.tau
            << std::setw(15) << std::fixed << std::setprecision(6) << row.u
            << std::setw(15) << row.y << '\n';
}

static bool saveResultsCsv(const std::string& path, const std::vector<Row>& rows)
{
    std::ofstream file(path);
    if (!file)
        return false;
    file << "tau,u,y\n";
    for (const Row& row : rows)
        file << row.tau << ',' << row.u << ',' << row.y << '\n';
    return static_cast<bool>(file);
}

#ifndef OTIS_TESTING

int main()
{   
    #ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    #endif
    std::cout << "============================================\n";
    std::cout << " OTIS-2026 | Лабораторная работа №1\n";
    std::cout << " Вариант 3\n";
    std::cout << "============================================\n";

    std::unique_ptr<Model> model = createModel();

    // Для линейной модели 1.3 проверяем корни характеристического уравнения
    // до запуска симуляции и предупреждаем о неустойчивости.
    if (const auto* linear = dynamic_cast<const Model1_3*>(model.get()))
    {
        // Проверяем устойчивость линейной модели по её коэффициентам.
        const StabilityResult stability = analyzeModel1_3Stability(linear->getA1(), linear->getA2());
        std::cout << "\nАнализ устойчивости: " << stability.message << '\n';
        std::cout << "Корни характеристического уравнения: z1 = ("
                  << stability.root1Real << ", " << stability.root1Imag << "i), z2 = ("
                  << stability.root2Real << ", " << stability.root2Imag << "i)\n";
    }

    const SignalType signal = readSignalType();
    const double amplitude = readDouble("Амплитуда A = ");
    const int n = readInt("Количество шагов n = ", 1);
    const double y0 = readDouble("Начальное значение y0 = ");

    double dt = 1.0;

    // dt используется только Model 3.7, но вводим его для единого интерфейса.
    if (std::string(model->getName()).find("Model 3.7") != std::string::npos)
        dt = readDouble("Шаг интегрирования dt = ");

    const std::vector<Row> rows = simulate(*model, signal, amplitude, n, y0, dt);

    std::cout << "\nМодель: " << model->getName() << '\n';
    printResultsTable(std::cout, rows);
    if (!saveResultsCsv("results.csv", rows))
        std::cerr << "Не удалось сохранить results.csv.\n";
    else
        std::cout << "\nРезультаты сохранены в results.csv\n";

    system("python plot.py");

    system("pause");

    return 0;
}

#endif // OTIS_TESTING
