// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystems/ManipulatorSubsystem.h"

#include <frc2/command/Commands.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "Constants.h"

ManipulatorSubsystem::ManipulatorSubsystem() {
  SetName("ManipulatorSubsystem");
}

void ManipulatorSubsystem::Periodic() {
  // Simulate game piece detection based on current state
  switch (m_currentState) {
    case State::INTAKING:
      m_intakeTimer++;
      // Simulate picking up game piece after 1 second (50 cycles at 50Hz)
      if (m_intakeTimer > 50 && !m_hasGamePiece) {
        m_hasGamePiece = true;
        m_currentState = State::IDLE;
        m_intakeTimer = 0;
      }
      break;
      
    case State::OUTTAKING:
    case State::SLOW_OUTTAKING:
    case State::PURGING:
      m_outtakeTimer++;
      // Simulate releasing game piece after 0.5 seconds (25 cycles)
      if (m_outtakeTimer > 25 && m_hasGamePiece) {
        m_hasGamePiece = false;
        m_currentState = State::IDLE;
        m_outtakeTimer = 0;
      }
      break;
      
    case State::IDLE:
      m_intakeTimer = 0;
      m_outtakeTimer = 0;
      break;
  }
  
  // Update dashboard
  frc::SmartDashboard::PutBoolean("Has Game Piece", m_hasGamePiece);
  frc::SmartDashboard::PutString("Manipulator State", [this]() {
    switch (m_currentState) {
      case State::IDLE: return "IDLE";
      case State::INTAKING: return "INTAKING";
      case State::OUTTAKING: return "OUTTAKING";
      case State::SLOW_OUTTAKING: return "SLOW_OUTTAKING";
      case State::PURGING: return "PURGING";
      default: return "UNKNOWN";
    }
  }());
}

frc2::CommandPtr ManipulatorSubsystem::Intake() {
  return frc2::cmd::StartEnd(
    [this] { 
      SetState(State::INTAKING);
    },
    [this] { 
      SetState(State::IDLE);
    },
    {this}
  ).ToPtr();
}

frc2::CommandPtr ManipulatorSubsystem::Outtake() {
  return frc2::cmd::StartEnd(
    [this] { 
      SetState(State::OUTTAKING);
    },
    [this] { 
      SetState(State::IDLE);
    },
    {this}
  ).ToPtr();
}

frc2::CommandPtr ManipulatorSubsystem::SlowOuttake() {
  return frc2::cmd::StartEnd(
    [this] { 
      SetState(State::SLOW_OUTTAKING);
    },
    [this] { 
      SetState(State::IDLE);
    },
    {this}
  ).ToPtr();
}

frc2::CommandPtr ManipulatorSubsystem::Purge() {
  return frc2::cmd::StartEnd(
    [this] { 
      SetState(State::PURGING);
    },
    [this] { 
      SetState(State::IDLE);
    },
    {this}
  ).ToPtr();
}

frc2::CommandPtr ManipulatorSubsystem::Stop() {
  return frc2::cmd::RunOnce([this] {
    SetState(State::IDLE);
  }, {}).ToPtr();
}

bool ManipulatorSubsystem::HasGamePiece() const {
  return m_hasGamePiece;
}

ManipulatorSubsystem::State ManipulatorSubsystem::GetState() const {
  return m_currentState;
}

void ManipulatorSubsystem::SetState(State state) {
  m_currentState = state;
}