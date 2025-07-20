// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"
#include "subsystems/climber/Climber.h"
#include "RobotConstants.h"
#include <frc2/command/Commands.h>
#include <frc2/command/button/CommandXboxController.h>

RobotContainer::RobotContainer()
    : m_climber(RobotConstants::ClimberConstants::kId,
                RobotConstants::ClimberConstants::kClimbSpeed,
                RobotConstants::ClimberConstants::kInverted),
      m_driverController(0)
{
    ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {
    m_driverController.A().WhileTrue(m_climber.GetExtendCommand());
    m_driverController.B().WhileTrue(m_climber.GetRetractCommand());
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
    return frc2::cmd::Print("No autonomous command configured");
}
