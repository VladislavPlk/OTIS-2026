#include "PIDController.h"
#include "PIDSimulation.h"
#include "Model1_3.h"
#include "Model2_5.h"
#include "Model3_7.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <utility>
#include <vector>
#include <string>

static double readDouble(const std::string& prompt)
{
    double value{};
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value && std::isfinite(value))
            return value;

        if (std::cin.eof())
            throw std::runtime_error("Ввод завершён");

        std::cout << "Ошибка ввода. Введите конечное число.\n";
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }
}

static int readInt(const std::string& prompt, int minimum)
{
    int value{};
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value && value >= minimum)
            return value;

        if (std::cin.eof())
            throw std::runtime_error("Ввод завершён");

        std::cout << "Ошибка ввода. Введите целое число не меньше "
                  << minimum << ".\n";
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }
}

static std::unique_ptr<Model> chooseModel()
{
    std::cout << "\nВыберите объект управления из ЛР1:\n"
              << "1 - Model 1.3 (линейная)\n"
              << "2 - Model 2.5 (нелинейная)\n"
              << "3 - Model 3.7 (дифференциальная)\n";

    const int choice = readInt("Ваш выбор: ", 1);

    if (choice == 1)
    {
        const double a1 = readDouble("a1 = ");
        const double a2 = readDouble("a2 = ");
        const double b = readDouble("b = ");
        return std::make_unique<Model1_3>(a1, a2, b);
    }
    if (choice == 2)
    {
        const double a = readDouble("a = ");
        const double b = readDouble("b = ");
        return std::make_unique<Model2_5>(a, b);
    }
    if (choice == 3)
    {
        const double a = readDouble("a = ");
        const double b = readDouble("b = ");
        return std::make_unique<Model3_7>(a, b);
    }

    throw std::invalid_argument("Нет модели с таким номером");
}

// Возвращает количество символов UTF-8, а не байтов.
// Это важно для правильного выравнивания русских заголовков в таблице.
static std::size_t utf8Length(const std::string& value)
{
    std::size_t length = 0;
    for (unsigned char ch : value)
    {
        if ((ch & 0xC0) != 0x80)
            ++length;
    }
    return length;
}

static std::string formatNumber(double value)
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(4) << value;
    return stream.str();
}

static void printTable(const std::vector<PIDRow>& rows)
{
    const std::vector<std::string> headers = {
        "Шаг", "Время", "Задание", "Температура", "Ошибка", "Управление"
    };

    std::vector<std::vector<std::string>> table;
    table.reserve(rows.size());

    std::vector<std::size_t> widths;
    widths.reserve(headers.size());

    for (const std::string& header : headers)
        widths.push_back(utf8Length(header));

    for (const PIDRow& row : rows)
    {
        std::vector<std::string> cells = {
            std::to_string(row.step),
            formatNumber(row.time),
            formatNumber(row.setpoint),
            formatNumber(row.temperature),
            formatNumber(row.error),
            formatNumber(row.control)
        };

        for (std::size_t i = 0; i < cells.size(); ++i)
            widths[i] = (std::max)(widths[i], cells[i].size());

        table.push_back(std::move(cells));
    }

    const auto printBorder = [&]()
    {
        std::cout << '+';
        for (std::size_t width : widths)
            std::cout << std::string(width + 2, '-') << '+';
        std::cout << '\n';
    };

    const auto printRow = [&](const std::vector<std::string>& cells,
                              bool headerRow = false)
    {
        std::cout << '|';
        for (std::size_t i = 0; i < cells.size(); ++i)
        {
            const std::size_t visibleLength = utf8Length(cells[i]);
            const std::size_t padding = widths[i] - visibleLength;

            std::cout << ' ';
            if (headerRow || i == 0)
                std::cout << cells[i] << std::string(padding, ' ');
            else
                std::cout << std::string(padding, ' ') << cells[i];
            std::cout << " |";
        }
        std::cout << '\n';
    };

    std::cout << '\n';
    printBorder();
    printRow(headers, true);
    printBorder();

    for (const auto& cells : table)
        printRow(cells);

    printBorder();
}


int main()
{
    try
    {
#ifdef _WIN32
        // UTF-8 output in Windows console.
        system("chcp 65001 > nul");
#endif
        std::cout << "OTIS-2026 | Лабораторная работа №3\n"
                  << "Система управления температурой с ПИД-регулятором\n";

        const std::unique_ptr<Model> model = chooseModel();

        SimulationConfig config;
        config.setpoint = readDouble("\nЗаданная температура: ");
        config.initialTemperature = readDouble("Начальная температура: ");
        config.dt = readDouble("Шаг моделирования dt: ");

        std::cout << "\nРежим моделирования:\n"
                  << "1 - фиксированное число шагов\n"
                  << "2 - до достижения заданной точности\n";
        const int modeChoice = readInt("Ваш выбор: ", 1);
        if (modeChoice != 1 && modeChoice != 2)
            throw std::invalid_argument("Неизвестный режим");

        SimulationMode mode;
        if (modeChoice == 1)
        {
            mode = SimulationMode::FixedSteps;
            config.steps = readInt("Количество шагов: ", 1);
        }
        else
        {
            mode = SimulationMode::UntilSettled;
            config.epsilon = readDouble("Допустимая ошибка epsilon: ");
            config.maxSteps = readInt("Максимальное число шагов: ", 1);
        }

        std::cout << "\nВведите коэффициенты ПИД-регулятора:\n";
        const double kp = readDouble("Kp = ");
        const double ki = readDouble("Ki = ");
        const double kd = readDouble("Kd = ");
        const double minOutput = readDouble("Минимум управляющего сигнала: ");
        const double maxOutput = readDouble("Максимум управляющего сигнала: ");

        PIDController controller(kp, ki, kd, config.dt,
                                  minOutput, maxOutput);

        const std::vector<PIDRow> rows =
            simulatePID(*model, controller, config, mode);

        std::cout << "\nИспользуемая модель: " << model->getName() << '\n';
        printTable(rows);

        if (!savePIDCsv("pid_results.csv", rows))
            throw std::runtime_error("Не удалось создать pid_results.csv");

        std::cout << "\nРезультаты сохранены в pid_results.csv\n";
        std::cout << "Построение графика...\n";
#ifdef _WIN32
        const int plotResult = std::system("python plot.py");
#else
        const int plotResult = std::system("python3 plot.py");
#endif
        if (plotResult != 0)
            std::cout << "График не построен. Установите matplotlib и запустите plot.py вручную.\n";
    }
    catch (const std::exception& error)
    {
        std::cerr << "\nОшибка: " << error.what() << '\n';
        return 1;
    }

    std::cout << "\nНажмите Enter для выхода...";
    std::cin.ignore();
    std::cin.get();
    return 0;
    
}
