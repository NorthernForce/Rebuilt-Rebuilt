// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc2/command/Commands.h>
#include <frc2/command/button/CommandXboxController.h>

#include "generated/TunerConstants.h"
#include "constants/Constants.h"
#include "frc/MathUtil.h"
#include "frc/Preferences.h"
#include "frc/smartdashboard/SmartDashboard.h"

using namespace std;
using namespace nfr;

std::array<frc::Rotation2d, 4> getModuleOffsets() {
    return {
        (units::degree_t)frc::Preferences::GetDouble("FrontLeftOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("FrontRightOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("BackLeftOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("BackRightOffset", 0.0)
    };
}

void setModuleOffsets(const std::array<frc::Rotation2d, 4>& offsets) {
    frc::Preferences::SetDouble("FrontLeftOffset", offsets[0].Degrees().value());
    frc::Preferences::SetDouble("FrontRightOffset", offsets[1].Degrees().value());
    frc::Preferences::SetDouble("BackLeftOffset", offsets[2].Degrees().value());
    frc::Preferences::SetDouble("BackRightOffset", offsets[3].Degrees().value());
}

RobotContainer::RobotContainer()
{
    drive = make_unique<SwerveDrive>(
        TunerConstants::DrivetrainConstants,
        DriveConstants::kUpdateRate,
        DriveConstants::kOdometryStandardDeviation,
        DriveConstants::kVisionStandardDeviation,
        DriveConstants::kTranslationPID,
        DriveConstants::kRotationPID,
        DriveConstants::kMaxTranslationSpeed,
        DriveConstants::kMaxRotationSpeed,
        TunerConstants::FrontLeft,
        TunerConstants::FrontRight,
        TunerConstants::BackLeft,
        TunerConstants::BackRight);
    drive->setModuleOffsets(getModuleOffsets());
    ConfigureBindings();
}

std::function<double()> processInput(std::function<double()> input) {
    return [input = std::move(input)]() mutable {
        auto x = input();
        x = frc::ApplyDeadband(x, 0.10);
        x *= abs(x); // Square the input for finer control at low speeds
        return x;
    };
}

void RobotContainer::ConfigureBindings() {
    frc2::CommandXboxController driverController{0};
    frc2::CommandXboxController operatorController{1};
    drive->SetDefaultCommand(drive->driveByJoystick(
        processInput([&driverController]() { return driverController.GetLeftX(); }),
        processInput([&driverController]() { return driverController.GetLeftY(); }),
        processInput([&driverController]() { return driverController.GetRightX(); }),
        true // Field-centric driving
    ));
    resetModulesCommand = drive->RunOnce([&]() {
        auto offsets = drive->resetModuleOffsets({0_deg, 0_deg, 0_deg, 0_deg});
        setModuleOffsets(offsets);
    });
    frc::SmartDashboard::PutData("Reset Swerve Modules", resetModulesCommand.value().get());
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand()
{
    return frc2::cmd::Print("No autonomous command configured");
}
