// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/LED/Runway.h"
#include "subsystems/LEDs.h"
#include "Constants.h"

#include <units/time.h>

Runway::Runway(LEDs* leds) : m_leds{leds}, m_tick{0} {
  AddRequirements(leds);
}

void Runway::Initialize() {
  m_timer.Restart();
  m_tick = 0;
}

void Runway::Execute() {
  if (m_timer.AdvanceIfElapsed(units::second_t{Constants::LEDConstants::kLEDRate})) {
    m_leds->FeedParticleEffect(0.75, m_tick);
    m_tick = (m_tick + 1) % 32;  // Reset after completing the cycle
  }
}

void Runway::End(bool interrupted) {
  m_leds->ClearAnimationBuffer();
}

bool Runway::RunsWhenDisabled() const {
  return true;
}