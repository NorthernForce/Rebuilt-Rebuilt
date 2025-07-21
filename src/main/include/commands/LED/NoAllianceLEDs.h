// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/CommandHelper.h>

class LEDs;

/**
 * LED command that displays a rainbow animation when no alliance is selected
 */
class NoAllianceLEDs : public frc2::CommandHelper<frc2::Command, NoAllianceLEDs> {
 public:
  explicit NoAllianceLEDs(LEDs* leds);

  void Initialize() override;
  void End(bool interrupted) override;
  bool RunsWhenDisabled() const override;

 private:
  LEDs* m_leds;
};