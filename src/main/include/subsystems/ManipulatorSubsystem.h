// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/CommandPtr.h>

/**
 * Manipulator subsystem for handling coral game pieces
 */
class ManipulatorSubsystem : public frc2::SubsystemBase {
 public:
  enum class State {
    IDLE,
    INTAKING,
    OUTTAKING,
    SLOW_OUTTAKING,
    PURGING
  };

  ManipulatorSubsystem();

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;

  /**
   * Intake coral
   */
  frc2::CommandPtr Intake();
  
  /**
   * Outtake coral  
   */
  frc2::CommandPtr Outtake();
  
  /**
   * Slow outtake coral
   */
  frc2::CommandPtr SlowOuttake();
  
  /**
   * Purge/eject coral
   */
  frc2::CommandPtr Purge();
  
  /**
   * Stop manipulator
   */
  frc2::CommandPtr Stop();
  
  /**
   * Check if manipulator has a game piece
   */
  bool HasGamePiece() const;
  
  /**
   * Get current manipulator state
   */
  State GetState() const;
  
  /**
   * Set manipulator state manually
   */
  void SetState(State state);

 private:
  State m_currentState = State::IDLE;
  bool m_hasGamePiece = false;
  
  // Simulation timer for game piece detection
  int m_intakeTimer = 0;
  int m_outtakeTimer = 0;
};