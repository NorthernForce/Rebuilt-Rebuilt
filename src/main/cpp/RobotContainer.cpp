// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc2/command/Commands.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/DriverStation.h>

RobotContainer::RobotContainer() {
  // Initialize subsystems here when they're implemented
  
  // Configure autonomous chooser
  m_chooser.SetDefaultOption("Simple Leave", "SimpleLeave");
  m_chooser.AddOption("Do Nothing", "DoNothing");
  
  frc::SmartDashboard::PutData("Auto Choices", &m_chooser);
  
  ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {
  // Placeholder for control bindings
  // Will be implemented when subsystems are added
  
  // Example bindings (commented out until subsystems exist):
  // m_driverController.Back().OnTrue(frc2::cmd::Print("Reset orientation"));
  // m_driverController.A().WhileTrue(frc2::cmd::Print("Climb extend"));
  // m_driverController.B().WhileTrue(frc2::cmd::Print("Climb retract"));
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
  std::string selected = m_chooser.GetSelected();
  
  if (selected == "SimpleLeave") {
    // Simple autonomous that moves forward for 2 seconds
    return frc2::cmd::Print("Simple Leave Auto - Moving forward").ToPtr();
  } else {
    // Default do nothing
    return frc2::cmd::Print("Do Nothing Auto").ToPtr();
  }
}

void RobotContainer::Periodic() {
  // Periodic updates for robot container
  // This will include vision updates, field display updates, etc.
}

void RobotContainer::AutonomousInit() {
  // Initialize autonomous mode
  // Set up any auto-specific configurations
}
