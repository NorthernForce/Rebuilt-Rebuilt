// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/util/Color.h>

namespace Constants {
  namespace LEDConstants {
    // CAN ID for the CANdle LED controller
    constexpr int kCANId = 30;
    
    // Number of LEDs in the strip
    constexpr int kLEDCount = 72;
    
    // Team colors
    const frc::Color kTeamColor{238.0/255.0, 10.0/255.0, 154.0/255.0}; // Pink/Magenta
    
    // LED update rate in seconds
    constexpr double kLEDRate = 0.1;
    
    // Animation speeds and brightness values
    constexpr double kRainbowBrightness = 0.75;
    constexpr double kRainbowSpeed = 0.1;
    constexpr double kStrobeBrightness = 0.5;
    constexpr double kFireBrightness = 0.5;
    constexpr double kFireSpeed = 0.5;
  }
}