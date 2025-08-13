// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>
#include <logging/Logger.h>

#include "subsystems/LEDSubsystem.h"
#include "subsystems/drive/SwerveDrive.h"

class RobotContainer
{
public:
    RobotContainer();

    frc2::CommandPtr GetAutonomousCommand();

    void Log(const nfr::LogContext &log) const;

private:
    void ConfigureBindings();
    void InitializeRobotSpecificLEDStates();
    std::unique_ptr<nfr::SwerveDrive> drive{nullptr};
    std::unique_ptr<nfr::LEDSubsystem> leds{nullptr};
    std::optional<frc2::CommandPtr> resetModulesCommand;
    frc2::CommandXboxController driverController{0};
};
