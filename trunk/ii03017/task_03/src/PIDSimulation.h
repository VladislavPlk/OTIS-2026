#pragma once

#include <vector>
#include "Model.h"
#include "PIDController.h"

/** @brief Строка результатов моделирования на одном временном шаге. */
struct PIDRow
{
    int step;
    double time;
    double setpoint;
    double temperature;
    double error;
    double control;
};

/** @brief Режим остановки моделирования. */
enum class SimulationMode
{
    FixedSteps = 1,
    UntilSettled = 2
};

/** @brief Параметры замкнутой системы регулирования. */
struct SimulationConfig
{
    double setpoint = 100.0;
    double initialTemperature = 20.0;
    double dt = 0.1;
    int steps = 500;
    double epsilon = 0.5;
    int maxSteps = 10000;
};

/**
 * @brief Выполнить моделирование объекта из ЛР1 с ПИД-регулятором.
 * @param model Выбранная модель из task_01.
 * @param controller ПИД-регулятор.
 * @param config Параметры моделирования.
 * @param mode Фиксированное число шагов или остановка по точности.
 * @return Таблица значений, включая начальное состояние.
 */
std::vector<PIDRow> simulatePID(const Model& model, PIDController& controller,
                                const SimulationConfig& config,
                                SimulationMode mode);

/** @brief Сохранить результаты в CSV. */
bool savePIDCsv(const char* fileName, const std::vector<PIDRow>& rows);
