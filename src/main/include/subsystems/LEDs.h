// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/CommandPtr.h>
#include <frc/util/Color.h>
#include <ctre/phoenix/led/CANdle.h>
#include <memory>

/**
 * LED subsystem that controls a CANdle LED controller with various animations and patterns.
 * Provides different visual states for game conditions like alliance color, piece detection,
 * autonomous mode, etc.
 */
class LEDs : public frc2::SubsystemBase {
 public:
  /**
   * Game states that can be represented with different LED patterns
   */
  enum class GameState {
    NONE,
    NO_ALLIANCE,
    BLUE_ALLIANCE, 
    RED_ALLIANCE,
    AUTO,
    TELEOP,
    ENDGAME,
    HASPIECE,
    WANTSPIECE,
    READYPLACE
  };

  LEDs();

  /**
   * Basic LED control methods
   */
  void ResetLEDs();
  void SetColor(const frc::Color& color, int start = 0, int length = -1);
  void RainbowAnimation(double brightness, double animationSpeed);
  void StrobeAnimation(const frc::Color& color);
  void FireAnimation();
  void ClearAnimationBuffer();

  /**
   * Game-specific LED patterns
   */
  void HasPiece();
  void EveryOther(const frc::Color& allianceColor);
  void FeedParticleEffect(double brightness, int tick);

  /**
   * Command factory methods that return command objects for different LED behaviors
   */
  frc2::CommandPtr ReadyToPlace();
  frc2::CommandPtr Hungry();
  frc2::CommandPtr Happy();
  frc2::CommandPtr NoAlliance();
  frc2::CommandPtr RedAlliance();
  frc2::CommandPtr BlueAlliance();
  frc2::CommandPtr Auto();
  frc2::CommandPtr Endgame();

  void Periodic() override;

 private:
  std::unique_ptr<ctre::phoenix::led::CANdle> m_candle;
  int m_ledCount;
  int m_currentAnimationTick;

  void SetEveryOtherColor(const frc::Color& color1, const frc::Color& color2);
  std::vector<int> ParticleTranslation(int tick);
};