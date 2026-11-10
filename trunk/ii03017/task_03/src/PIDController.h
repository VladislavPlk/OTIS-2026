#pragma once

/**
 * @brief Discrete incremental PID controller with actuator and anti-windup limits.
 *
 * The controller uses:
 * delta_u(k) = Kp*(e(k)-e(k-1)) + Ki*e(k)*dt
 *            + Kd/dt*(e(k)-2*e(k-1)+e(k-2)).
 */
class PIDController
{
public:
    /**
     * @param kp Proportional coefficient.
     * @param ki Integral coefficient.
     * @param kd Derivative coefficient.
     * @param dt Sampling interval (must be positive).
     * @param minOutput Minimum actuator output.
     * @param maxOutput Maximum actuator output.
     * @param integralLimit Maximum absolute accumulated integral error.
     */
    PIDController(double kp, double ki, double kd, double dt,
                  double minOutput = -100.0, double maxOutput = 100.0,
                  double integralLimit = 1000.0);

    /** @brief Calculate the next actuator output. */
    double update(double setpoint, double measuredValue);

    /** @brief Clear controller history and set the output to initialOutput. */
    void reset(double initialOutput = 0.0);

    /** @brief Return the latest actuator output. */
    double output() const;

private:
    double kp_, ki_, kd_, dt_;
    double minOutput_, maxOutput_, integralLimit_;
    double integralError_;
    double previousError_;
    double previousPreviousError_;
    double output_;
    bool hasHistory_;
};
