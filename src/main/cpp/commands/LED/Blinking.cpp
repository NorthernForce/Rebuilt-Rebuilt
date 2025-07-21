// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/LED/Blinking.h"
#include "subsystems/LEDs.h"
#include "Constants.h"

#include <units/time.h>

Blinking::Blinking(LEDs* leds) : m_leds{leds}, m_ledState{false} {
  AddRequirements(leds);
}

void Blinking::Initialize() {
  m_timer.Restart();
  m_ledState = false;
}

void Blinking::Execute() {
  if (m_timer.AdvanceIfElapsed(units::second_t{Constants::LEDConstants::kLEDRate})) {
    m_ledState = !m_ledState;
    if (m_ledState) {
      m_leds->SetColor(Constants::LEDConstants::kTeamColor);
    } else {
      m_leds->ResetLEDs();
    }
  }
}

void Blinking::End(bool interrupted) {
  m_leds->ClearAnimationBuffer();
}

bool Blinking::RunsWhenDisabled() const {
  return true;
}