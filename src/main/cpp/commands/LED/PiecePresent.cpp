// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/LED/PiecePresent.h"
#include "subsystems/LEDs.h"

PiecePresent::PiecePresent(LEDs* leds) : m_leds{leds} {
  AddRequirements(leds);
}

void PiecePresent::Initialize() {
  m_leds->HasPiece();
}

void PiecePresent::End(bool interrupted) {
  m_leds->ClearAnimationBuffer();
}

bool PiecePresent::RunsWhenDisabled() const {
  return true;
}