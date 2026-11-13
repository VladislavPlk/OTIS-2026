#include "PIDController.h"
#include "PIDSimulation.h"
#include "Model1_3.h"
#include "Model2_5.h"
#include "Model3_7.h"

#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>

TEST(PIDController, ProportionalResponse)
{
    PIDController pid(2.0, 0.0, 0.0, 0.1, -100.0, 100.0);
    EXPECT_DOUBLE_EQ(pid.update(10.0, 7.0), 6.0);
}

TEST(PIDController, OutputIsLimited)
{
    PIDController pid(100.0, 0.0, 0.0, 0.1, -5.0, 5.0);
    EXPECT_DOUBLE_EQ(pid.update(10.0, 0.0), 5.0);
}

TEST(PIDController, FirstDerivativeSampleDoesNotKick)
{
    PIDController pid(1.0, 0.0, 10.0, 0.1);
    EXPECT_DOUBLE_EQ(pid.update(2.0, 0.0), 2.0);
}

TEST(PIDController, ResetClearsPreviousError)
{
    PIDController pid(1.0, 0.0, 1.0, 1.0);
    pid.update(10.0, 0.0);
    pid.reset();
    EXPECT_DOUBLE_EQ(pid.update(2.0, 0.0), 2.0);
}

TEST(PIDController, RejectsNonPositiveTimeStep)
{
    EXPECT_THROW((PIDController(1.0, 1.0, 1.0, 0.0)), std::invalid_argument);
}

TEST(PIDController, RejectsInvalidLimits)
{
    EXPECT_THROW((PIDController(1.0, 1.0, 1.0, 0.1, 5.0, -5.0)),
                 std::invalid_argument);
}

TEST(PIDSimulation, FixedStepModeReturnsInitialRowAndRequestedSteps)
{
    Model3_7 plant(0.0, 1.0);
    PIDController pid(0.2, 0.0, 0.0, 0.1);
    SimulationConfig config;
    config.setpoint = 5.0;
    config.initialTemperature = 0.0;
    config.dt = 0.1;
    config.steps = 20;

    const auto rows = simulatePID(plant, pid, config, SimulationMode::FixedSteps);
    ASSERT_EQ(rows.size(), 21u);
    EXPECT_EQ(rows.front().step, 0);
    EXPECT_EQ(rows.back().step, 20);
}

TEST(PIDSimulation, SettledModeStopsWithinTolerance)
{
    Model3_7 plant(0.0, 1.0);
    PIDController pid(0.2, 0.5, 0.0, 0.1, -100.0, 100.0);
    SimulationConfig config;
    config.setpoint = 5.0;
    config.initialTemperature = 0.0;
    config.dt = 0.1;
    config.epsilon = 0.2;
    config.maxSteps = 20000;

    const auto rows = simulatePID(plant, pid, config, SimulationMode::UntilSettled);
    ASSERT_GT(rows.size(), 1u);
    EXPECT_LE(std::abs(rows.back().error), config.epsilon);
    EXPECT_LT(rows.size(), static_cast<std::size_t>(config.maxSteps + 1));
}

TEST(PIDSimulation, RejectsInvalidTimeStep)
{
    Model3_7 plant(0.0, 1.0);
    PIDController pid(1.0, 0.0, 0.0, 0.1);
    SimulationConfig config;
    config.dt = 0.0;

    EXPECT_THROW(simulatePID(plant, pid, config, SimulationMode::FixedSteps),
                 std::invalid_argument);
}

TEST(PIDSimulation, CanUseAllThreeModelsFromTaskOne)
{
    Model1_3 linear(0.5, 0.1, 0.01);
    Model2_5 nonlinear(0.8, 2.0);
    Model3_7 differential(0.0, 0.1);
    SimulationConfig config;
    config.steps = 10;
    config.dt = 0.1;

    PIDController pid1(0.1, 0.01, 0.0, config.dt);
    PIDController pid2(0.1, 0.01, 0.0, config.dt);
    PIDController pid3(0.1, 0.01, 0.0, config.dt);

    EXPECT_EQ(simulatePID(linear, pid1, config, SimulationMode::FixedSteps).size(), 11u);
    EXPECT_EQ(simulatePID(nonlinear, pid2, config, SimulationMode::FixedSteps).size(), 11u);
    EXPECT_EQ(simulatePID(differential, pid3, config, SimulationMode::FixedSteps).size(), 11u);
}
