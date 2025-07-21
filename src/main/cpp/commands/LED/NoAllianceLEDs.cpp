// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/LED/NoAllianceLEDs.h"
#include "subsystems/LEDs.h"
#include "Constants.h"

NoAllianceLEDs::NoAllianceLEDs(LEDs* leds) : m_leds{leds} {
  AddRequirements(leds);
}

void NoAllianceLEDs::Initialize() {
  m_leds->RainbowAnimation(Constants::LEDConstants::kRainbowBrightness,
                           Constants::LEDConstants::kRainbowSpeed);
}

void NoAllianceLEDs::End(bool interrupted) {
  m_leds->ClearAnimationBuffer();
}

bool NoAllianceLEDs::RunsWhenDisabled() const {
  return true;
}