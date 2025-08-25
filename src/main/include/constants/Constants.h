#pragma once

#include <frc/geometry/Rotation3d.h>
#include <frc/geometry/Transform3d.h>
#include <frc/geometry/Translation3d.h>
#include <pathplanner/lib/controllers/PPHolonomicDriveController.h>
#include <units/angle.h>
#include <units/frequency.h>
#include <units/length.h>
#include <units/time.h>

#include <string_view>

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

    class VisionConstants
    {
    public:
        // Maximum age for vision estimates to be considered valid
        static constexpr units::second_t kMaxEstimateAge = 0.1_s;

        // Timeout for considering vision system has recent estimates
        static constexpr units::second_t kEstimateTimeout = 0.5_s;

        // AprilTag field layout
        static constexpr const char* kAprilTagFieldLayout = "2024-crescendo.json";
    };

    class CameraConstants
    {
    public:
        // Camera transforms relative to robot center
        static constexpr frc::Transform3d kFrontLeftCameraTransform{
            frc::Translation3d{0.3_m, 0.2_m,
                               0.5_m},  // 30cm forward, 20cm left, 50cm up
            frc::Rotation3d{0_deg, -15_deg, 0_deg}  // Slight downward tilt
        };
        static constexpr frc::Transform3d kCenterCameraTransform{
            frc::Translation3d{0.0_m, 0.0_m, 0.5_m},  // Center of robot, 50cm up
            frc::Rotation3d{0_deg, -15_deg, 0_deg}    // Slight downward tilt
        };
        static constexpr frc::Transform3d kFrontRightCameraTransform{
            frc::Translation3d{0.3_m, -0.2_m,
                               0.5_m},  // 30cm forward, 20cm right, 50cm up
            frc::Rotation3d{0_deg, -15_deg, 0_deg}  // Slight downward tilt
        };
        static constexpr frc::Transform3d kCenterBackCameraTransform{
            frc::Translation3d{-0.3_m, 0.0_m, 0.5_m},  // 30cm back, center, 50cm up
            frc::Rotation3d{0_deg, -15_deg,
                            180_deg}  // Facing backwards with slight downward tilt
        };

        // PhotonVision camera names
        static constexpr std::string_view kFrontLeftCameraName =
            "front_left_camera";
        static constexpr std::string_view kCenterCameraName = "center_camera";

        // LimeLight camera names
        static constexpr std::string_view kFrontRightCameraName = "limelight-fl";
        static constexpr std::string_view kCenterBackCameraName = "limelight-ctr";
    };
}  // namespace nfr