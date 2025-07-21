// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystems/LEDs.h"
#include "Constants.h"
#include "commands/LED/AutoLEDs.h"
#include "commands/LED/Blinking.h"
#include "commands/LED/EveryOther.h"
#include "commands/LED/EndgameLEDs.h"
#include "commands/LED/NoAllianceLEDs.h"
#include "commands/LED/PiecePresent.h"
#include "commands/LED/Runway.h"

#include <frc/MathUtil.h>
#include <ctre/phoenix/led/CANdleConfiguration.h>
#include <ctre/phoenix/led/RainbowAnimation.h>
#include <ctre/phoenix/led/StrobeAnimation.h>
#include <ctre/phoenix/led/FireAnimation.h>

using namespace Constants::LEDConstants;

LEDs::LEDs() 
  : m_candle{std::make_unique<ctre::phoenix::led::CANdle>(kCANId)},
    m_ledCount{kLEDCount},
    m_currentAnimationTick{0} {
  
  // Configure the CANdle
  ctre::phoenix::led::CANdleConfiguration config;
  config.stripType = ctre::phoenix::led::LEDStripType::RGB;
  config.brightnessScalar = 0.75;
  m_candle->ConfigAllSettings(config);
}

void LEDs::ResetLEDs() {
  m_candle->SetLEDs(0, 0, 0, 0, 0, m_ledCount);
}

void LEDs::SetColor(const frc::Color& color, int start, int length) {
  if (length == -1) {
    length = m_ledCount;
  }
  
  int red = static_cast<int>(255 * color.red);
  int green = static_cast<int>(255 * color.green);
  int blue = static_cast<int>(255 * color.blue);
  
  m_candle->SetLEDs(red, green, blue, 0, start, length);
}

void LEDs::RainbowAnimation(double brightness, double animationSpeed) {
  ctre::phoenix::led::RainbowAnimation rainbowAnim{brightness, animationSpeed, m_ledCount};
  m_candle->Animate(rainbowAnim);
}

void LEDs::StrobeAnimation(const frc::Color& color) {
  int red = static_cast<int>(255 * color.red);
  int green = static_cast<int>(255 * color.green);
  int blue = static_cast<int>(255 * color.blue);
  
  ctre::phoenix::led::StrobeAnimation strobeAnim{red, green, blue, 0, kStrobeBrightness, m_ledCount};
  m_candle->Animate(strobeAnim);
}

void LEDs::FireAnimation() {
  ctre::phoenix::led::FireAnimation fireAnim{kFireBrightness, kFireSpeed, m_ledCount, kFireSpeed, kFireSpeed};
  m_candle->Animate(fireAnim);
}

void LEDs::ClearAnimationBuffer() {
  for (int i = 0; i < m_candle->GetMaxSimultaneousAnimationCount(); i++) {
    m_candle->ClearAnimation(i);
  }
}

void LEDs::HasPiece() {
  // Set to magenta/purple when piece is detected
  m_candle->SetLEDs(255, 0, 255, 0, 0, m_ledCount);
}

void LEDs::SetEveryOtherColor(const frc::Color& color1, const frc::Color& color2) {
  for (int i = 0; i < m_ledCount; i++) {
    if (i % 2 == 0) {
      SetColor(color1, i, 1);
    } else {
      SetColor(color2, i, 1);
    }
  }
}

void LEDs::EveryOther(const frc::Color& allianceColor) {
  SetEveryOtherColor(allianceColor, kTeamColor);
}

std::vector<int> LEDs::ParticleTranslation(int tick) {
  if (tick == 0) {
    return {62 - 6};
  } else if (tick > 0 && tick < 31) {
    int pos1 = static_cast<int>(frc::InputModulus((62 - 6) - tick, 0, 62));
    int pos2 = static_cast<int>(frc::InputModulus((62 - 6) + tick, 0, 62));
    return {pos1, pos2};
  } else if (tick == 31) {
    return {31 + 6};
  }
  
  return {};
}

void LEDs::FeedParticleEffect(double brightness, int tick) {
  ResetLEDs();
  
  auto particles = ParticleTranslation(tick);
  for (int pos : particles) {
    SetColor(kTeamColor, pos + 8, 1);
  }
  
  // Add additional particles at different phases
  auto nextParticle0 = ParticleTranslation(static_cast<int>(frc::InputModulus(tick + 4, 0, 32)));
  for (int pos : nextParticle0) {
    SetColor(kTeamColor, pos + 8, 1);
  }
  
  auto nextParticle1 = ParticleTranslation(static_cast<int>(frc::InputModulus(tick + 8, 0, 32)));
  for (int pos : nextParticle1) {
    SetColor(kTeamColor, pos + 8, 1);
  }
  
  auto nextParticle2 = ParticleTranslation(static_cast<int>(frc::InputModulus(tick + 12, 0, 32)));
  for (int pos : nextParticle2) {
    SetColor(kTeamColor, pos + 8, 1);
  }
}

// Command factory methods
frc2::CommandPtr LEDs::ReadyToPlace() {
  return std::make_unique<Blinking>(this);
}

frc2::CommandPtr LEDs::Hungry() {
  return std::make_unique<Runway>(this);
}

frc2::CommandPtr LEDs::Happy() {
  return std::make_unique<PiecePresent>(this);
}

frc2::CommandPtr LEDs::NoAlliance() {
  return std::make_unique<NoAllianceLEDs>(this);
}

frc2::CommandPtr LEDs::RedAlliance() {
  return std::make_unique<EveryOther>(this, frc::Color::kRed);
}

frc2::CommandPtr LEDs::BlueAlliance() {
  return std::make_unique<EveryOther>(this, frc::Color::kBlue);
}

frc2::CommandPtr LEDs::Auto() {
  return std::make_unique<AutoLEDs>(this);
}

frc2::CommandPtr LEDs::Endgame() {
  return std::make_unique<EndgameLEDs>(this);
}

void LEDs::Periodic() {
  // This method will be called once per scheduler run
}