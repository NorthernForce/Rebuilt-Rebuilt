// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/CommandHelper.h>
#include <frc/util/Color.h>

class LEDs;

/**
 * LED command that displays alternating alliance and team colors
 */
class EveryOther : public frc2::CommandHelper<frc2::Command, EveryOther> {
 public:
  EveryOther(LEDs* leds, const frc::Color& color);

  void Initialize() override;
  void End(bool interrupted) override;
  bool RunsWhenDisabled() const override;

 private:
  LEDs* m_leds;
  frc::Color m_color;
};