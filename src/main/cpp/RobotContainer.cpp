// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <logging/LogTypes.h>

#include <frc2/command/Commands.h>
#include <frc/DriverStation.h>

RobotContainer::RobotContainer()
{
    ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {}

frc2::CommandPtr RobotContainer::GetAutonomousCommand()
{
    return frc2::cmd::Print("No autonomous command configured");
}

void RobotContainer::Log(const nfr::LogContext &log) const
{
    log["match_time"] << frc::DriverStation::GetMatchTime();
}
