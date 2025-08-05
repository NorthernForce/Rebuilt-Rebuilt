#pragma once

#include <constants/ElevatorConstants.h>
#include <pathplanner/lib/controllers/PPHolonomicDriveController.h>
#include <units/frequency.h>
#include <units/math.h>

#include <ctre/phoenix6/TalonFX.hpp>

namespace nfr
{
    class DriveConstants
    {
    public:
        static constexpr units::hertz_t kUpdateRate =
            200_Hz;  // Update rate for the swerve drive
        static constexpr units::meters_per_second_t kMaxTranslationSpeed =
            3.0_mps;  // Maximum translation speed
        static constexpr units::radians_per_second_t kMaxRotationSpeed =
            10.0_rad_per_s;  // Maximum rotation speed
        static constexpr std::array<double, 3> kOdometryStandardDeviation = {
            0.01, 0.01, 0.01};  // Standard deviation for odometry
        static constexpr std::array<double, 3> kVisionStandardDeviation = {
            0.1, 0.1, 9999999};  // Standard deviation for vision measurements
        static constexpr pathplanner::PIDConstants kTranslationPID =
            pathplanner::PIDConstants(0.5, 0.0, 0.0);
        static constexpr pathplanner::PIDConstants kRotationPID =
            pathplanner::PIDConstants(0.1, 0.0, 0.0);
    };
}  // namespace nfr
namespace UniversalElevatorConstants
{
    constexpr double kHomingSpeed = 0.25;
    constexpr units::meter_t kTolerance = 0.006_m;
    // TODO fix values
    enum SuperstructurePresets
    {
        L1,
        L2,
        L3,
        L4,
        CORAL_STATION,
        START
    };
}  // namespace UniversalElevatorConstants

namespace InnerElevatorConstants
{
    constexpr int kId = 2;
    constexpr int kSensorId = 0;

    constexpr double kGearBoxRatio = 12.0;
    constexpr double kSprocketTeeth = 16.0;
    constexpr units::meter_t kSprocketPitch = 0.25_in;
    constexpr units::meter_t kSprocketCircumference =
        kSprocketPitch * kSprocketTeeth;

    constexpr double kS = 0.017384;
    constexpr double kV = 0.726186;
    constexpr double kA = 0.015;
    constexpr double kP = 18;
    constexpr double kI = 0;
    constexpr double kD = 0;
    constexpr double kG = 0.21;
    constexpr units::turns_per_second_t kCruiseVelocity = 160_tps;
    constexpr units::turns_per_second_squared_t kAcceleration = 0_tr_per_s_sq;
    constexpr units::turns_per_second_cubed_t kJerk = 0_tr_per_s_cu;

    constexpr units::meter_t kLowerLimit = 0_m;
    constexpr units::meter_t kUpperLimit = 0.63_m;
    constexpr units::kilogram_t kElevatorMass = 2.72_kg;

    constexpr ElevatorConstants kConstants = ElevatorConstants(
        kS, kV, kA, kP, kI, kD, kG, kCruiseVelocity, kAcceleration, kJerk,
        kSprocketCircumference, kGearBoxRatio, true, kLowerLimit, kUpperLimit,
        kElevatorMass);
}  // namespace InnerElevatorConstants

namespace OuterElevatorConstants
{
    constexpr int kId = 3;
    constexpr int kSensorId = 1;

    constexpr double kGearBoxRatio = 16.0;
    constexpr double kSprocketTeeth = 22.0;
    constexpr units::meter_t kSprocketPitch = 0.25_in;
    constexpr units::meter_t kSprocketCircumference =
        kSprocketPitch * kSprocketTeeth;

    constexpr double kS = 0.052289;
    constexpr double kV = 0.504647;
    constexpr double kA = 0.015;
    constexpr double kP = 18;
    constexpr double kI = 0;
    constexpr double kD = 0;
    constexpr double kG = 0.31;
    constexpr units::turns_per_second_t kCruiseVelocity = 160_tps;
    constexpr units::turns_per_second_squared_t kAcceleration = 0_tr_per_s_sq;
    constexpr units::turns_per_second_cubed_t kJerk = 0_tr_per_s_cu;

    constexpr units::meter_t kLowerLimit = 0_m;
    constexpr units::meter_t kUpperLimit = 0.68_m;
    constexpr units::kilogram_t kElevatorMass = 2.72_kg;

    constexpr ElevatorConstants kConstants = ElevatorConstants(
        kS, kV, kA, kP, kI, kD, kG, kCruiseVelocity, kAcceleration, kJerk,
        kSprocketCircumference, kGearBoxRatio, true, kLowerLimit, kUpperLimit,
        kElevatorMass);
}  // namespace OuterElevatorConstants
