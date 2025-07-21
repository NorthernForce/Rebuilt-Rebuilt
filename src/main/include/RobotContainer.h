// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <memory>

#include "Constants.h"
#include "subsystems/DriveSubsystem.h"
#include "subsystems/ManipulatorSubsystem.h"

class RobotContainer {
 public:
  RobotContainer();

  frc2::CommandPtr GetAutonomousCommand();
  void Periodic();
  
  // Autonomous mode selection
  void AutonomousInit();

 private:
  void ConfigureBindings();
  
  // Apply deadband and square inputs for better control
  static double ProcessJoystickInput(double input);
  
  // Controllers
  frc2::CommandXboxController m_driverController{Constants::Controller::kDriverControllerPort};
  frc2::CommandXboxController m_operatorController{Constants::Controller::kOperatorControllerPort};
  
  // Subsystems
  DriveSubsystem m_drive;
  ManipulatorSubsystem m_manipulator;
  
  // Autonomous mode tracking
  bool m_autonomousMode = false;
  
  // Autonomous chooser
  frc::SendableChooser<std::string> m_chooser;
};
