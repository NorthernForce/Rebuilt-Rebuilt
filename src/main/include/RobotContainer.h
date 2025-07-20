// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include "subsystems/SwerveDrive.h"
#include <frc2/command/button/CommandXboxController.h>

class RobotContainer {
 public:
  RobotContainer();

  frc2::CommandPtr GetAutonomousCommand();

 private:
  void ConfigureBindings();
  nfr::SwerveDrive drive;
  std::optional<frc2::CommandPtr> resetModulesCommand;
  frc2::CommandXboxController driverController{0};
};
