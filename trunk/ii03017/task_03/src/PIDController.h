#pragma once

/**
 * @brief Дискретный ПИД-регулятор с ограничением выхода и защитой от windup.
 *
 * Используется позиционная форма:
 * u(k) = Kp*e(k) + Ki*I(k) + Kd*D(k).
 */
class PIDController
{
public:
    /**
     * @param kp Пропорциональный коэффициент.
     * @param ki Интегральный коэффициент.
     * @param kd Дифференциальный коэффициент.
     * @param dt Шаг дискретизации, должен быть положительным.
     * @param minOutput Нижняя граница управляющего сигнала.
     * @param maxOutput Верхняя граница управляющего сигнала.
     * @param integralLimit Ограничение интегральной суммы по модулю.
     */
    PIDController(double kp, double ki, double kd, double dt,
                  double minOutput = -100.0, double maxOutput = 100.0,
                  double integralLimit = 1000.0);

    /** @brief Рассчитать управляющее воздействие по заданию и измерению. */
    double update(double setpoint, double measuredValue);

    /** @brief Сбросить внутреннее состояние регулятора. */
    void reset();

    /** @brief Получить последнее управляющее воздействие. */
    double output() const;

private:
    double kp_;
    double ki_;
    double kd_;
    double dt_;
    double minOutput_;
    double maxOutput_;
    double integralLimit_;
    double integral_{0.0};
    double previousError_{0.0};
    double output_{0.0};
    bool hasPreviousError_{false};
};
