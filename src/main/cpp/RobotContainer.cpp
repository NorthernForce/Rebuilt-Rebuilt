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
    : drive(TunerConstants::DrivetrainConstants, DriveConstants::kUpdateRate,
            DriveConstants::kOdometryStandardDeviation,
            DriveConstants::kVisionStandardDeviation,
            DriveConstants::kTranslationPID, DriveConstants::kRotationPID,
            DriveConstants::kMaxTranslationSpeed,
            DriveConstants::kMaxRotationSpeed, TunerConstants::FrontLeft,
            TunerConstants::FrontRight, TunerConstants::BackLeft,
            TunerConstants::BackRight),
      m_superstructure(
          make_shared<Elevator>(string("Inner Elevator"),
                                make_shared<ElevatorIOTalonFX>(
                                    InnerElevatorConstants::kId,
                                    InnerElevatorConstants::kConstants),
                                make_shared<ElevatorSensorIOLimitSwitch>(
                                    InnerElevatorConstants::kSensorId),
                                ElevatorConstants::kTolerance),
          make_shared<Elevator>(string("Outer Elevator"),
                                make_shared<ElevatorIOTalonFX>(
                                    OuterElevatorConstants::kId,
                                    OuterElevatorConstants::kConstants),
                                make_shared<ElevatorSensorIOLimitSwitch>(
                                    OuterElevatorConstants::kSensorId),
                                ElevatorConstants::kTolerance))
{
    drive.SetModuleOffsets(getModuleOffsets());
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
    drive.SetDefaultCommand(drive.DriveByJoystick(
        ProcessInput([&]() { return driverController.GetLeftX(); }),
        ProcessInput([&]() { return driverController.GetLeftY(); }),
        ProcessInput([&]() { return driverController.GetRightX(); }),
        true  // Field-centric driving
        ));
    driverController.Back().OnTrue(
        drive.RunOnce([&]() { drive.SeedFieldCentric(); }));
    resetModulesCommand = drive.RunOnce(
        [&]()
        {
            auto offsets =
                drive.ResetModuleOffsets({0_deg, 0_deg, 0_deg, 0_deg});
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
    log["superstructure"] << m_superstructure;
}
