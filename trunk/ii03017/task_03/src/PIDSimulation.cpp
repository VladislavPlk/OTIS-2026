#include "PIDSimulation.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <stdexcept>

std::vector<PIDRow> simulatePID(const Model& model, PIDController& controller,
                                const SimulationConfig& config,
                                SimulationMode mode)
{
    if (!std::isfinite(config.setpoint) ||
        !std::isfinite(config.initialTemperature) ||
        !std::isfinite(config.dt) || config.dt <= 0.0)
        throw std::invalid_argument("Некорректное задание, температура или dt");

    if (config.steps < 1 || config.maxSteps < 1 ||
        !std::isfinite(config.epsilon) || config.epsilon <= 0.0)
        throw std::invalid_argument("Некорректные ограничения моделирования");

    const int limit = mode == SimulationMode::FixedSteps
        ? config.steps : config.maxSteps;

    std::vector<PIDRow> rows;
    rows.reserve(static_cast<std::size_t>(limit) + 1);

    double temperature = config.initialTemperature;
    double previousTemperature = temperature;
    double error = config.setpoint - temperature;

    rows.push_back({0, 0.0, config.setpoint, temperature, error, 0.0});

    for (int k = 1; k <= limit; ++k)
    {
        const double control = controller.update(config.setpoint, temperature);
        const double nextTemperature =
            model.calculateNext(temperature, previousTemperature, control, config.dt);

        if (!std::isfinite(nextTemperature))
            throw std::domain_error("Модель вернула некорректную температуру");

        previousTemperature = temperature;
        temperature = nextTemperature;
        error = config.setpoint - temperature;

        rows.push_back({k, k * config.dt, config.setpoint,
                        temperature, error, control});

        if (mode == SimulationMode::UntilSettled &&
            std::abs(error) <= config.epsilon)
            break;
    }

    return rows;
}

bool savePIDCsv(const char* fileName, const std::vector<PIDRow>& rows)
{
    std::ofstream file(fileName);
    if (!file)
        return false;

    file << "step,time,setpoint,temperature,error,control\n";
    file << std::fixed << std::setprecision(6);

    for (const PIDRow& row : rows)
        file << row.step << ',' << row.time << ',' << row.setpoint << ','
             << row.temperature << ',' << row.error << ',' << row.control << '\n';

    return static_cast<bool>(file);
}
