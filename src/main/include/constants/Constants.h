#pragma once

#include <pathplanner/lib/controllers/PPHolonomicDriveController.h>
#include <units/frequency.h>

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
class DashboardConstants
{
  public:
    static constexpr int kPort = 5800;  // Port for the dashboard server
    static constexpr std::string_view kDistSubdirectory =
        "npm-dash";  // Subdirectory for dashboard files
};
}  // namespace nfr