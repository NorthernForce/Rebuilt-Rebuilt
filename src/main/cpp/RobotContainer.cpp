// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc/DriverStation.h>
#include <frc/RobotBase.h>
#include <frc2/command/Commands.h>
#include <frc2/command/button/CommandXboxController.h>
#include <logging/LogTypes.h>

#include "constants/Constants.h"
#include "frc/MathUtil.h"
#include "frc/Preferences.h"
#include "frc/smartdashboard/SmartDashboard.h"
#include "generated/TunerConstants.h"
#include "subsystems/apriltag/LimeLightCameraIO.h"
#include "subsystems/apriltag/PhotonVisionCameraIO.h"
#include "subsystems/apriltag/PhotonVisionCameraSimIO.h"

using namespace std;
using namespace nfr;

std::vector<CameraConfig> CreateCameraConfigurations()
{
    std::vector<CameraConfig> configs;

    if (frc::RobotBase::IsReal())
    {
        // Real robot - create PhotonVision and LimeLight cameras

        // PhotonVision cameras
        configs.emplace_back(
            "FrontLeft", CameraConstants::kFrontLeftCameraName,
            CameraConstants::kFrontLeftCameraTransform,
            []()
            {
                return std::make_unique<PhotonVisionCameraIO>(
                    CameraConstants::kFrontLeftCameraName,
                    CameraConstants::kFrontLeftCameraTransform);
            });

        configs.emplace_back("Center", CameraConstants::kCenterCameraName,
                             CameraConstants::kCenterCameraTransform,
                             []()
                             {
                                 return std::make_unique<PhotonVisionCameraIO>(
                                     CameraConstants::kCenterCameraName,
                                     CameraConstants::kCenterCameraTransform);
                             });

        // LimeLight cameras
        configs.emplace_back(
            "FrontRight", CameraConstants::kFrontRightCameraName,
            CameraConstants::kFrontRightCameraTransform,
            []()
            {
                return std::make_unique<LimeLightCameraIO>(
                    CameraConstants::kFrontRightCameraName,
                    CameraConstants::kFrontRightCameraTransform);
            });

        configs.emplace_back(
            "CenterBack", CameraConstants::kCenterBackCameraName,
            CameraConstants::kCenterBackCameraTransform,
            []()
            {
                return std::make_unique<LimeLightCameraIO>(
                    CameraConstants::kCenterBackCameraName,
                    CameraConstants::kCenterBackCameraTransform);
            });
    }
    else
    {
        // Simulation - use PhotonVision simulation for all cameras including
        // LimeLight
        configs.emplace_back(
            "FrontLeft-Sim", CameraConstants::kFrontLeftCameraName,
            CameraConstants::kFrontLeftCameraTransform,
            []()
            {
                return std::make_unique<PhotonVisionCameraSimIO>(
                    CameraConstants::kFrontLeftCameraName,
                    CameraConstants::kFrontLeftCameraTransform);
            });

        // Use PhotonVision simulation for LimeLight cameras too
        configs.emplace_back(
            "LimeLight-Sim", CameraConstants::kFrontRightCameraName,
            CameraConstants::kFrontRightCameraTransform,
            []()
            {
                return std::make_unique<PhotonVisionCameraSimIO>(
                    CameraConstants::kFrontRightCameraName,
                    CameraConstants::kFrontRightCameraTransform);
            });

        configs.emplace_back(
            "Center-Sim", CameraConstants::kCenterCameraName,
            CameraConstants::kCenterCameraTransform,
            []()
            {
                return std::make_unique<PhotonVisionCameraSimIO>(
                    CameraConstants::kCenterCameraName,
                    CameraConstants::kCenterCameraTransform);
            });
    }

    return configs;
}

std::array<frc::Rotation2d, 4> getModuleOffsets()
{
    return {
        (units::degree_t)frc::Preferences::GetDouble("FrontLeftOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("FrontRightOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("BackLeftOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("BackRightOffset", 0.0)};
}

void SetModuleOffsets(const std::array<frc::Rotation2d, 4>& offsets)
{
    frc::Preferences::SetDouble("FrontLeftOffset",
                                offsets[0].Degrees().value());
    frc::Preferences::SetDouble("FrontRightOffset",
                                offsets[1].Degrees().value());
    frc::Preferences::SetDouble("BackLeftOffset", offsets[2].Degrees().value());
    frc::Preferences::SetDouble("BackRightOffset",
                                offsets[3].Degrees().value());
}

RobotContainer::RobotContainer()
    : localizer(CreateCameraConfigurations(), VisionConstants::kEstimateTimeout)
{
    drive = std::make_unique<SwerveDrive>(
        TunerConstants::DrivetrainConstants, DriveConstants::kUpdateRate,
        DriveConstants::kOdometryStandardDeviation,
        DriveConstants::kVisionStandardDeviation,
        DriveConstants::kTranslationPID, DriveConstants::kRotationPID,
        DriveConstants::kMaxTranslationSpeed, DriveConstants::kMaxRotationSpeed,
        TunerConstants::FrontLeft, TunerConstants::FrontRight,
        TunerConstants::BackLeft, TunerConstants::BackRight);
    drive->SetModuleOffsets(getModuleOffsets());
    ConfigureBindings();
}

std::function<double()> ProcessInput(std::function<double()> input)
{
    return [input = std::move(input)]() mutable
    {
        auto x = input();
        x = frc::ApplyDeadband(x, 0.10);
        x *= abs(x);  // Square the input for finer control at low speeds
        return x;
    };
}

void RobotContainer::ConfigureBindings()
{
    drive->SetDefaultCommand(drive->DriveByJoystick(
        ProcessInput([&]() { return driverController.GetLeftX(); }),
        ProcessInput([&]() { return driverController.GetLeftY(); }),
        ProcessInput([&]() { return driverController.GetRightX(); }),
        true  // Field-centric driving
        ));
    driverController.Back().OnTrue(
        drive->RunOnce([&]() { drive->SeedFieldCentric(); }));
    resetModulesCommand = drive->RunOnce(
        [&]()
        {
            auto offsets =
                drive->ResetModuleOffsets({0_deg, 0_deg, 0_deg, 0_deg});
            SetModuleOffsets(offsets);
        });
    frc::SmartDashboard::PutData("Reset Swerve Modules",
                                 resetModulesCommand.value().get());
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand()
{
    return frc2::cmd::Print("No autonomous command configured");
}

void RobotContainer::Periodic()
{
    // Update localizer with current robot pose for reference
    localizer.UpdateWithReferencePose(drive->GetState().Pose);

    // Add vision measurements to drivetrain pose estimator
    const auto& estimatedPoses = localizer.GetEstimatedPoses();
    for (const auto& estimatedPose : estimatedPoses)
    {
        // Only use recent estimates to avoid stale data
        // Check that the estimate is from this periodic cycle (very recent)
        units::second_t currentTime = frc::Timer::GetFPGATimestamp();
        units::second_t estimateAge = currentTime - estimatedPose.timestamp;

        if (estimateAge < VisionConstants::kMaxEstimateAge)
        {
            drive->AddVisionMeasurement(estimatedPose.pose,
                                       estimatedPose.timestamp);
        }
    }
}

void RobotContainer::Log(const nfr::LogContext& log) const
{
    log["match_time"] << frc::DriverStation::GetMatchTime();
    log["drive"] << drive;
    log["localizer"] << localizer;
}
