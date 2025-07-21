// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/LED/EndgameLEDs.h"
#include "subsystems/LEDs.h"

EndgameLEDs::EndgameLEDs(LEDs* leds) : m_leds{leds} {
  AddRequirements(leds);
}

void EndgameLEDs::Initialize() {
  m_leds->FireAnimation();
}

void EndgameLEDs::End(bool interrupted) {
  m_leds->ClearAnimationBuffer();
}

bool EndgameLEDs::RunsWhenDisabled() const {
  return true;
}