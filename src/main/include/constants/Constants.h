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