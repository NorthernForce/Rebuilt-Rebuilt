// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc2/command/Commands.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/DriverStation.h>
#include <frc/MathUtil.h>

#include "FieldConstants.h"

RobotContainer::RobotContainer() {
  // Configure default commands
  m_drive.SetDefaultCommand(m_drive.DriveByJoystick(
    [this] { return ProcessJoystickInput(-m_driverController.GetLeftY()); },
    [this] { return ProcessJoystickInput(-m_driverController.GetLeftX()); },
    [this] { return ProcessJoystickInput(-m_driverController.GetRightX()); }
  ));
  
  // Configure autonomous chooser
  m_chooser.SetDefaultOption("Simple Leave", "SimpleLeave");
  m_chooser.AddOption("Do Nothing", "DoNothing");
  m_chooser.AddOption("Drive to Reef", "DriveToReef");
  
  frc::SmartDashboard::PutData("Auto Choices", &m_chooser);
  
  ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {
  // Driver controller bindings
  
  // Reset robot orientation (back button)
  m_driverController.Back().OnTrue(m_drive.ResetOrientation());
  
  // Reef positioning (bumpers)
  m_driverController.LeftBumper().WhileTrue(m_drive.DriveToNearestReef());
  m_driverController.RightBumper().WhileTrue(m_drive.DriveToNearestReef());
  
  // Quick movements for fine positioning (D-pad)
  m_driverController.POVUp().WhileTrue(m_drive.GoForward(0.3));
  m_driverController.POVDown().WhileTrue(m_drive.GoBackward(0.3));
  m_driverController.POVLeft().WhileTrue(m_drive.StrafeLeft(0.3));
  m_driverController.POVRight().WhileTrue(m_drive.StrafeRight(0.3));
  
  // Manipulator controls
  m_driverController.RightTrigger().WhileTrue(m_manipulator.Outtake());
  m_driverController.LeftTrigger().WhileTrue(m_manipulator.Intake());
  
  // Autonomous mode toggle (Y button)
  m_driverController.Y().OnTrue(frc2::cmd::RunOnce([this] {
    m_autonomousMode = !m_autonomousMode;
    frc::SmartDashboard::PutBoolean("Autonomous Mode", m_autonomousMode);
  }));
  
  // Emergency stop (X button)
  m_driverController.X().OnTrue(frc2::cmd::RunOnce([this] {
    m_drive.Stop().Schedule();
    m_manipulator.Stop().Schedule();
  }));
  
  // Operator controller bindings
  
  // Additional manipulator controls
  m_operatorController.RightTrigger().WhileTrue(m_manipulator.Outtake());
  m_operatorController.LeftTrigger().WhileTrue(m_manipulator.Intake());
  m_operatorController.A().WhileTrue(m_manipulator.SlowOuttake());
  m_operatorController.B().WhileTrue(m_manipulator.Purge());
  
  // Additional drive controls for operator
  m_operatorController.LeftBumper().WhileTrue(m_drive.StrafeLeft(0.4));
  m_operatorController.RightBumper().WhileTrue(m_drive.StrafeRight(0.4));
  
  // Superstructure controls (4 levels)
  m_operatorController.POVUp().OnTrue(m_superstructure.GetMoveToL4Command());
  m_operatorController.POVRight().OnTrue(m_superstructure.GetMoveToL3Command());
  m_operatorController.POVDown().OnTrue(m_superstructure.GetMoveToL2Command());
  m_operatorController.POVLeft().OnTrue(m_superstructure.GetMoveToL1Command());
  
  // Climber controls (right stick Y-axis for manual control)
  m_driverController.Start().OnTrue(m_climber.GetExtendCommand());
  m_driverController.Back().OnTrue(m_climber.GetRetractCommand());
  
  // Algae extractor controls
  m_operatorController.X().OnTrue(m_algaeExtractor.GetFullExtractionCycle());
  m_operatorController.Y().OnTrue(m_algaeExtractor.GetReturnCommand());
  
  // LED pattern controls (for demonstration)
  frc2::Trigger([this] { return m_manipulator.HasGamePiece(); })
    .OnTrue(frc2::cmd::RunOnce([this] { m_leds.SetPattern(LEDSubsystem::LEDPattern::GAME_PIECE_DETECTED); }))
    .OnFalse(frc2::cmd::RunOnce([this] { m_leds.SetPattern(LEDSubsystem::LEDPattern::SOLID_ALLIANCE); }));
  
  // Set alliance color based on DriverStation
  frc2::Trigger([] { return frc::DriverStation::GetAlliance() == frc::DriverStation::Alliance::kRed; })
    .OnTrue(frc2::cmd::RunOnce([this] { m_leds.SetAllianceColor(true); }))
    .OnFalse(frc2::cmd::RunOnce([this] { m_leds.SetAllianceColor(false); }));
  
  // Vision controls
  m_operatorController.Start().OnTrue(frc2::cmd::RunOnce([this] {
    m_vision.SetVisionEnabled(!m_vision.IsVisionEnabled());
  }));
}

double RobotContainer::ProcessJoystickInput(double input) {
  // Apply deadband
  input = frc::ApplyDeadband(input, Constants::Controller::kJoystickDeadband);
  
  // Square input for better fine control while keeping sign
  return input * std::abs(input);
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
  std::string selected = m_chooser.GetSelected();
  
  if (selected == "SimpleLeave") {
    // Simple autonomous that moves forward for 2 seconds
    return m_drive.GoForward(1.0).WithTimeout(2_s);
  } else if (selected == "DriveToReef") {
    // Drive to a reef position
    return frc2::cmd::Sequence(
      frc2::cmd::Print("Starting autonomous - driving to reef"),
      m_drive.DriveToPose(FieldConstants::ReefPositions::kA).WithTimeout(5_s),
      frc2::cmd::Print("Arrived at reef position")
    );
  } else {
    // Default do nothing
    return frc2::cmd::Print("Do Nothing Auto").ToPtr();
  }
}

void RobotContainer::Periodic() {
  // Update dashboard with robot status
  frc::SmartDashboard::PutBoolean("Autonomous Mode", m_autonomousMode);
  frc::SmartDashboard::PutString("Alliance", 
    frc::DriverStation::GetAlliance() == frc::DriverStation::Alliance::kBlue ? "Blue" : "Red");
  
  // Update robot pose for dashboard (this would be sent via NetworkTables to React app)
  auto pose = m_drive.GetPose();
  frc::SmartDashboard::PutNumberArray("Robot Pose", {
    pose.X().value(),
    pose.Y().value(), 
    pose.Rotation().Degrees().value()
  });
  
  // Cross-subsystem coordination
  
  // Update AprilTags subsystem with current robot pose from drive
  m_aprilTags.SetReferencePose(m_drive.GetPose());
  
  // Get vision-based pose corrections and apply to drive if available
  auto visionPose = m_aprilTags.GetRobotPose();
  if (visionPose.has_value() && m_aprilTags.GetVisibleTagCount() >= 2) {
    // Only use vision pose if we see multiple tags (more reliable)
    m_drive.AddVisionMeasurement(visionPose->pose, visionPose->timestamp);
  }
  
  // Update LED patterns based on robot state
  if (frc::DriverStation::IsAutonomous() || m_autonomousMode) {
    m_leds.SetPattern(LEDSubsystem::LEDPattern::AUTONOMOUS_MODE);
  } else if (m_vision.IsGamePieceDetected()) {
    m_leds.SetPattern(LEDSubsystem::LEDPattern::GAME_PIECE_DETECTED);
  } else if (frc::DriverStation::IsDisabled()) {
    m_leds.SetPattern(LEDSubsystem::LEDPattern::RAINBOW);
  } else {
    m_leds.SetPattern(LEDSubsystem::LEDPattern::SOLID_ALLIANCE);
  }
  
  // Update alliance color for LEDs
  auto alliance = frc::DriverStation::GetAlliance();
  if (alliance.has_value()) {
    m_leds.SetAllianceColor(alliance == frc::DriverStation::Alliance::kRed);
  }
  
  // Superstructure status for dashboard
  auto currentLevel = m_superstructure.GetCurrentLevel();
  std::string levelStr;
  switch (currentLevel) {
    case SuperstructureSubsystem::ScoringLevel::L1: levelStr = "L1"; break;
    case SuperstructureSubsystem::ScoringLevel::L2: levelStr = "L2"; break;
    case SuperstructureSubsystem::ScoringLevel::L3: levelStr = "L3"; break;
    case SuperstructureSubsystem::ScoringLevel::L4: levelStr = "L4"; break;
    case SuperstructureSubsystem::ScoringLevel::HOME: levelStr = "HOME"; break;
    case SuperstructureSubsystem::ScoringLevel::CORAL_STATION: levelStr = "CORAL_STATION"; break;
  }
  frc::SmartDashboard::PutString("Superstructure/CurrentLevel", levelStr);
  
  // If in autonomous mode and not in auto period, allow manual override
  if (m_autonomousMode && frc::DriverStation::IsTeleopEnabled()) {
    // Check if driver is giving manual input
    bool hasManualInput = 
      std::abs(m_driverController.GetLeftY()) > Constants::Controller::kJoystickDeadband ||
      std::abs(m_driverController.GetLeftX()) > Constants::Controller::kJoystickDeadband ||
      std::abs(m_driverController.GetRightX()) > Constants::Controller::kJoystickDeadband;
    
    if (hasManualInput) {
      // Temporarily disable autonomous mode for manual control
      frc::SmartDashboard::PutBoolean("Manual Override Active", true);
    } else {
      frc::SmartDashboard::PutBoolean("Manual Override Active", false);
    }
  }
}

void RobotContainer::AutonomousInit() {
  // Initialize autonomous mode
  m_autonomousMode = true;
  frc::SmartDashboard::PutBoolean("Autonomous Mode", true);
  
  // Reset robot pose to starting position (field-relative)
  auto alliance = frc::DriverStation::GetAlliance().value_or(frc::DriverStation::Alliance::kBlue);
  if (alliance == frc::DriverStation::Alliance::kBlue) {
    m_drive.ResetPose(frc::Pose2d{1.5_m, 1.5_m, frc::Rotation2d{0_deg}});
  } else {
    m_drive.ResetPose(frc::Pose2d{15_m, 6.5_m, frc::Rotation2d{180_deg}});
  }
}
