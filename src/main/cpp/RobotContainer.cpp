// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc/DriverStation.h>
#include <frc2/command/Commands.h>
#include <frc2/command/button/CommandXboxController.h>
#include <logging/LogTypes.h>

#include "constants/Constants.h"
#include "frc/MathUtil.h"
#include "frc/Preferences.h"
#include "frc/smartdashboard/SmartDashboard.h"
#include "generated/TunerConstants.h"

using namespace std;
using namespace nfr;

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

void RobotContainer::Log(const nfr::LogContext& log) const
{
    log["match_time"] << frc::DriverStation::GetMatchTime();
    log["drive"] << drive;
    
    // AdvantageScope 3D robot visualization
    // Based on config.json components in advantageScopeAssets/Robot_Ralph/
    LogRobotState(log["Robot3d"]);
}

void RobotContainer::LogRobotState(const nfr::LogContext& log) const
{
    // Get current robot pose for the base robot component
    frc::Pose3d robotPose = frc::Pose3d(drive->GetState().Pose);
    log["Robot"] << robotPose;
    
    // Component 0: Swerve drive modules - base robot frame
    // Uses the main robot pose as reference
    log["component_0"] << robotPose;
    
    // Component 1: Manipulator/Arm - positioned at front of robot
    // This would normally get position from manipulator subsystem
    // Using dummy values for now since manipulator isn't implemented
    frc::Pose3d manipulatorPose = robotPose + frc::Transform3d(
        frc::Translation3d(0.27_m, 0.05_m, 0.53_m),
        frc::Rotation3d(0_deg, 0_deg, 270_deg)
    );
    log["component_1"] << manipulatorPose;
    
    // Component 2: Robot base frame - matches main robot position  
    log["component_2"] << robotPose;
    
    // Component 3: Elevator - positioned above robot center
    // Using dummy values as elevator is not implemented yet
    double elevatorHeight = 0.30; // Dummy elevator height in meters
    frc::Pose3d elevatorPose = robotPose + frc::Transform3d(
        frc::Translation3d(0.31_m, -0.07_m, units::meter_t(elevatorHeight)),
        frc::Rotation3d(0_deg, 285_deg, 270_deg)
    );
    log["component_3"] << elevatorPose;
}
