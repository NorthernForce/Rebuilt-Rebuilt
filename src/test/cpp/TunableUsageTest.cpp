#include <constants/TunableConstants.h>
#include <tuning/TuningManager.h>

#include "gtest/gtest.h"

class TunableUsageTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Clear any existing tunables
        nfr::TuningManager::GetInstance().Clear();
        
        // Reset tunable constants to their defaults
        nfr::TunableDriveConstants::kTranslationP.Set(0.5);
        nfr::TunableDriveConstants::kTranslationI.Set(0.0);
        nfr::TunableDriveConstants::kTranslationD.Set(0.0);
        nfr::TunableExampleConstants::kMotorSpeed.Set(0.5);
    }

    void TearDown() override
    {
        nfr::TuningManager::GetInstance().Clear();
    }
};

TEST_F(TunableUsageTest, DriveConstantsUsage)
{
    // Test that we can use tunable constants just like regular constants
    double updateRate = nfr::TunableDriveConstants::kUpdateRateHz;
    EXPECT_DOUBLE_EQ(200.0, updateRate);
    
    // Test units usage
    units::meters_per_second_t maxSpeed = nfr::TunableDriveConstants::kMaxTranslationSpeed;
    EXPECT_DOUBLE_EQ(3.0, maxSpeed.value());
    
    // Test that we can modify values - note: in test environment, 
    // the value is stored locally until NetworkTables updates it
    nfr::TunableDriveConstants::kUpdateRateHz.Set(250.0);
    double newUpdateRate = nfr::TunableDriveConstants::kUpdateRateHz.Get();
    EXPECT_DOUBLE_EQ(250.0, newUpdateRate);
    
    // Test array getters
    auto odometryStdDev = nfr::TunableDriveConstants::GetOdometryStandardDeviation();
    EXPECT_DOUBLE_EQ(0.01, odometryStdDev[0]);
    EXPECT_DOUBLE_EQ(0.01, odometryStdDev[1]);
    EXPECT_DOUBLE_EQ(0.01, odometryStdDev[2]);
}

TEST_F(TunableUsageTest, ExampleConstantsUsage)
{
    // Test various types
    double motorSpeed = nfr::TunableExampleConstants::kMotorSpeed;
    EXPECT_DOUBLE_EQ(0.5, motorSpeed);
    
    bool motorEnabled = nfr::TunableExampleConstants::kMotorEnabled;
    EXPECT_TRUE(motorEnabled);
    
    int encoderCPR = nfr::TunableExampleConstants::kEncoderCPR;
    EXPECT_EQ(2048, encoderCPR);
    
    // Test units
    units::meter_t armLength = nfr::TunableExampleConstants::kArmLength;
    EXPECT_DOUBLE_EQ(0.8, armLength.value());
    
    units::degree_t armAngle = nfr::TunableExampleConstants::kArmAngle;
    EXPECT_DOUBLE_EQ(45.0, armAngle.value());
}

TEST_F(TunableUsageTest, ArithmeticWithTunables)
{
    // Test that tunables work seamlessly in calculations
    double speed = nfr::TunableExampleConstants::kMotorSpeed;
    double scaledSpeed = speed * 0.8;  // Scale down by 80%
    EXPECT_DOUBLE_EQ(0.4, scaledSpeed);
    
    // Test with units
    units::meter_t length = nfr::TunableExampleConstants::kArmLength;
    units::meter_t doubleLength = length * 2.0;
    EXPECT_DOUBLE_EQ(1.6, doubleLength.value());
}

TEST_F(TunableUsageTest, ConditionalLogicWithTunables)
{
    // Test that tunables work in conditional statements
    if (nfr::TunableExampleConstants::kMotorEnabled)
    {
        // Motor is enabled by default, so this should execute
        EXPECT_TRUE(true);
    }
    else
    {
        // Should not reach here
        EXPECT_FALSE(true);
    }
    
    // Test comparison
    if (nfr::TunableExampleConstants::kMotorSpeed > 0.3)
    {
        // Motor speed is 0.5 by default, so this should execute
        EXPECT_TRUE(true);
    }
    else
    {
        EXPECT_FALSE(true);
    }
}

TEST_F(TunableUsageTest, FunctionParametersWithTunables)
{
    // Test passing tunables to functions that expect the base type
    auto testFunction = [](double speed, bool enabled, units::meter_t length) -> double {
        return enabled ? speed * length.value() : 0.0;
    };
    
    double result = testFunction(
        nfr::TunableExampleConstants::kMotorSpeed,
        nfr::TunableExampleConstants::kMotorEnabled,
        nfr::TunableExampleConstants::kArmLength
    );
    
    // 0.5 * 0.8 = 0.4
    EXPECT_DOUBLE_EQ(0.4, result);
}

TEST_F(TunableUsageTest, UpdateMechanismTest)
{
    // Test that update mechanism works
    double originalSpeed = nfr::TunableExampleConstants::kMotorSpeed;
    EXPECT_DOUBLE_EQ(0.5, originalSpeed);
    
    // Simulate changing the value and verify it's immediately accessible
    nfr::TunableExampleConstants::kMotorSpeed.Set(0.7);
    
    // The value should be immediately available through Get()
    double newSpeed = nfr::TunableExampleConstants::kMotorSpeed.Get();
    EXPECT_DOUBLE_EQ(0.7, newSpeed);
    
    // And also through implicit conversion
    double implicitSpeed = nfr::TunableExampleConstants::kMotorSpeed;
    EXPECT_DOUBLE_EQ(0.7, implicitSpeed);
}

// Demo function showing how to use tunables in a PID controller setup
double CalculatePIDOutput(double error, double previousError, double integral)
{
    double kP = nfr::TunableDriveConstants::kTranslationP;
    double kI = nfr::TunableDriveConstants::kTranslationI;
    double kD = nfr::TunableDriveConstants::kTranslationD;
    
    double derivative = error - previousError;
    integral += error;
    
    return kP * error + kI * integral + kD * derivative;
}

TEST_F(TunableUsageTest, PIDControllerExample)
{
    // Test that tunables work seamlessly in control algorithms
    double output = CalculatePIDOutput(1.0, 0.8, 0.1);
    
    // With default values: kP=0.5, kI=0.0, kD=0.0
    // output = 0.5 * 1.0 + 0.0 * 0.1 + 0.0 * 0.2 = 0.5
    EXPECT_DOUBLE_EQ(0.5, output);
    
    // Change PID values and test again
    nfr::TunableDriveConstants::kTranslationP.Set(1.0);
    nfr::TunableDriveConstants::kTranslationI.Set(0.1);
    nfr::TunableDriveConstants::kTranslationD.Set(0.05);
    
    double newOutput = CalculatePIDOutput(1.0, 0.8, 0.1);
    
    // New output = 1.0 * 1.0 + 0.1 * 0.1 + 0.05 * 0.2 = 1.0 + 0.01 + 0.01 = 1.02
    EXPECT_DOUBLE_EQ(1.02, newOutput);
}