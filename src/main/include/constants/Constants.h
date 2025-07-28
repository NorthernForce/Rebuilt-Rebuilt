#pragma once

#include <pathplanner/lib/controllers/PPHolonomicDriveController.h>
#include <units/frequency.h>
#include <units/time.h>
#include <frc/geometry/Transform3d.h>
#include <frc/geometry/Translation3d.h>
#include <frc/geometry/Rotation3d.h>
#include <units/angle.h>
#include <units/length.h>

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
    static const frc::Transform3d kFrontLeftCameraTransform;
    static const frc::Transform3d kCenterCameraTransform;
    static const frc::Transform3d kFrontRightCameraTransform;
    static const frc::Transform3d kCenterBackCameraTransform;
    
    // PhotonVision camera names
    static constexpr const char* kFrontLeftCameraName = "front_left_camera";
    static constexpr const char* kCenterCameraName = "center_camera";
    
    // LimeLight camera names
    static constexpr const char* kFrontRightCameraName = "limelight-fl";
    static constexpr const char* kCenterBackCameraName = "limelight-ctr";
};
}  // namespace nfr