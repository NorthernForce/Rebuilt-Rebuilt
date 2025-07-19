#pragma once

#include <pathplanner/lib/controllers/PPHolonomicDriveController.h>
#include <units/time.h>

namespace nfr
{
    class DriveConstants
    {
    public:
        static constexpr units::hertz_t kUpdateRate = 200_Hz; // Update rate for the swerve drive
        static constexpr std::array<double, 3> kOdometryStandardDeviation = {0.01, 0.01, 0.01}; // Standard deviation for odometry
        static constexpr std::array<double, 3> kVisionStandardDeviation = {0.1, 0.1, 9999999}; // Standard deviation for vision measurements
        static constexpr pathplanner::PIDConstants kTranslationPID = pathplanner::PIDConstants(0.5, 0.0, 0.0);
        static constexpr pathplanner::PIDConstants kRotationPID = pathplanner::PIDConstants(0.1, 0.0, 0.0);
    };
}