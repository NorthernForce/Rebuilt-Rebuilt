// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/CommandHelper.h>
#include <frc/Timer.h>

class LEDs;

/**
 * LED command that blinks the team color on and off
 */
class Blinking : public frc2::CommandHelper<frc2::Command, Blinking> {
 public:
  explicit Blinking(LEDs* leds);

  void Initialize() override;
  void Execute() override;
  void End(bool interrupted) override;
  bool RunsWhenDisabled() const override;

 private:
  LEDs* m_leds;
  frc::Timer m_timer;
  bool m_ledState;
};