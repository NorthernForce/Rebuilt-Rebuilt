// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/LED/EveryOther.h"
#include "subsystems/LEDs.h"

EveryOther::EveryOther(LEDs* leds, const frc::Color& color) 
  : m_leds{leds}, m_color{color} {
  AddRequirements(leds);
}

void EveryOther::Initialize() {
  m_leds->EveryOther(m_color);
}

void EveryOther::End(bool interrupted) {
  m_leds->ClearAnimationBuffer();
}

bool EveryOther::RunsWhenDisabled() const {
  return true;
}