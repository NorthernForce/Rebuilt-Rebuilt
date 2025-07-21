// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/LED/AutoLEDs.h"
#include "subsystems/LEDs.h"
#include "Constants.h"

AutoLEDs::AutoLEDs(LEDs* leds) : m_leds{leds} {
  AddRequirements(leds);
}

void AutoLEDs::Initialize() {
  m_leds->RainbowAnimation(Constants::LEDConstants::kRainbowBrightness, 
                           Constants::LEDConstants::kRainbowSpeed);
}

void AutoLEDs::End(bool interrupted) {
  m_leds->ClearAnimationBuffer();
}

bool AutoLEDs::RunsWhenDisabled() const {
  return true;
}