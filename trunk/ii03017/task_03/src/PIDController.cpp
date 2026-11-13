#include "PIDController.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

PIDController::PIDController(double kp, double ki, double kd, double dt,
                             double minOutput, double maxOutput,
                             double integralLimit)
    : kp_(kp), ki_(ki), kd_(kd), dt_(dt),
      minOutput_(minOutput), maxOutput_(maxOutput),
      integralLimit_(integralLimit)
{
    if (!std::isfinite(kp_) || !std::isfinite(ki_) ||
        !std::isfinite(kd_) || !std::isfinite(dt_) || dt_ <= 0.0)
        throw std::invalid_argument("Коэффициенты PID должны быть конечными, dt > 0");

    if (!std::isfinite(minOutput_) || !std::isfinite(maxOutput_) ||
        minOutput_ >= maxOutput_ || !std::isfinite(integralLimit_) ||
        integralLimit_ < 0.0)
        throw std::invalid_argument("Некорректные ограничения регулятора");
}

double PIDController::update(double setpoint, double measuredValue)
{
    if (!std::isfinite(setpoint) || !std::isfinite(measuredValue))
        throw std::invalid_argument("Задание и измерение должны быть конечными");

    const double error = setpoint - measuredValue;
    const double proposedIntegral = std::clamp(
        integral_ + error * dt_, -integralLimit_, integralLimit_);

    // На первом шаге производная равна нулю: это исключает стартовый скачок.
    const double derivative = hasPreviousError_
        ? (error - previousError_) / dt_
        : 0.0;

    const double rawOutput =
        kp_ * error + ki_ * proposedIntegral + kd_ * derivative;
    output_ = std::clamp(rawOutput, minOutput_, maxOutput_);

    // Anti-windup: не накапливаем интеграл, если он усиливает насыщение.
    if (const bool pushesFurtherIntoSaturation =
            (rawOutput > maxOutput_ && error > 0.0) ||
            (rawOutput < minOutput_ && error < 0.0);
        !pushesFurtherIntoSaturation)
        integral_ = proposedIntegral;

    previousError_ = error;
    hasPreviousError_ = true;
    return output_;
}

void PIDController::reset()
{
    integral_ = 0.0;
    previousError_ = 0.0;
    output_ = 0.0;
    hasPreviousError_ = false;
}

double PIDController::output() const
{
    return output_;
}
