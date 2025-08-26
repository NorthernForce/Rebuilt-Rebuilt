#pragma once

#include <constants/ElevatorConstants.h>
#include <pathplanner/lib/controllers/PPHolonomicDriveController.h>
#include <units/frequency.h>
#include <units/math.h>

#include <ctre/phoenix6/TalonFX.hpp>

namespace nfr
{
    /**
     * @brief Configuration constants for the robot's swerve drivetrain
     *
     * This class contains all the important numbers that control how our swerve
     * drivetrain behaves. These values are tuned through testing and should be
     * adjusted carefully - small changes can dramatically affect robot
     * performance.
     *
     * @note All values use WPILib's unit system for type safety. This prevents
     * mistakes like mixing up meters and inches, or seconds and milliseconds.
     */
    class DriveConstants
    {
    public:
        /**
         * @brief How often (frequency) to update swerve drive calculations
         *
         * Higher frequencies give smoother control but use more CPU. 200 Hz
         * (200 times per second) is a good balance for swerve drive.
         */
        static constexpr units::hertz_t kUpdateRate = 200_Hz;

        /**
         * @brief Maximum speed the robot can translate (move without rotating)
         *
         * This limits how fast the robot can drive in any direction. Set based
         * on what drivers can safely control and mechanical capabilities. 3.0
         * m/s ≈ 6.7 mph - fast but controllable for most FRC robots.
         */
        static constexpr units::meters_per_second_t kMaxTranslationSpeed =
            3.0_mps;

        /**
         * @brief Maximum speed the robot can rotate (spin in place)
         *
         * This limits how fast the robot can turn. 10 rad/s ≈ 1.6 rotations per
         * second
         * - fast enough for quick turns but not so fast that drivers lose
         * control.
         */
        static constexpr units::radians_per_second_t kMaxRotationSpeed =
            10.0_rad_per_s;

        /**
         * @brief Uncertainty values for odometry (position tracking using wheel
         * encoders)
         *
         * These numbers tell the robot how much to trust its wheel-based
         * position estimates. Lower values = "trust this measurement more"
         * [x_uncertainty, y_uncertainty, rotation_uncertainty]
         *
         * 0.01 = very confident in wheel-based position tracking (swerve is
         * accurate!)
         */
        static constexpr std::array<double, 3> kOdometryStandardDeviation = {
            0.01, 0.01, 0.01};

        /**
         * @brief Uncertainty values for vision-based position measurements
         *
         * These numbers tell the robot how much to trust camera-based position
         * estimates. [x_uncertainty, y_uncertainty, rotation_uncertainty]
         *
         * 0.1 = less confident in vision than wheels (cameras can be fooled by
         * lighting) 9999999 = essentially ignore vision rotation data (cameras
         * aren't great at this)
         */
        static constexpr std::array<double, 3> kVisionStandardDeviation = {
            0.1, 0.1, 9999999};

        /**
         * @brief PID controller gains for translation during autonomous
         *
         * PID controllers automatically correct errors to reach target
         * positions.
         * - P (Proportional): How strongly to correct based on current error
         * - I (Integral): How strongly to correct based on accumulated error
         * over time
         * - D (Derivative): How strongly to correct based on rate of error
         * change
         *
         * These values control how the robot follows autonomous paths.
         * Higher P = more aggressive correction, but can cause oscillation
         */
        static constexpr pathplanner::PIDConstants kTranslationPID =
            pathplanner::PIDConstants(0.5, 0.0, 0.0);

        /**
         * @brief PID controller gains for rotation during autonomous
         *
         * Similar to translation PID, but for turning to target headings.
         * Lower values than translation because rotation is typically easier to
         * control.
         */
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
