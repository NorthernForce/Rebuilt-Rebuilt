// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <memory>

#include "Constants.h"

// Forward declarations for subsystems (will be added later)
class DriveSubsystem;
class ManipulatorSubsystem;
class SuperstructureSubsystem;
class ClimberSubsystem;

class RobotContainer {
 public:
  RobotContainer();

  frc2::CommandPtr GetAutonomousCommand();
  void Periodic();
  
  // Autonomous mode selection
  void AutonomousInit();

 private:
  void ConfigureBindings();
  
  // Controllers
  frc2::CommandXboxController m_driverController{Constants::Controller::kDriverControllerPort};
  frc2::CommandXboxController m_operatorController{Constants::Controller::kOperatorControllerPort};
  
  // Subsystems (will be implemented later)
  // std::unique_ptr<DriveSubsystem> m_drive;
  // std::unique_ptr<ManipulatorSubsystem> m_manipulator;
  // std::unique_ptr<SuperstructureSubsystem> m_superstructure;
  // std::unique_ptr<ClimberSubsystem> m_climber;
  
  // Autonomous chooser
  frc::SendableChooser<std::string> m_chooser;
};
