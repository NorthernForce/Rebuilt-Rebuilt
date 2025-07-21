// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>
#include "subsystems/vision/Vision.h"

class RobotContainer {
public:
    RobotContainer();
    frc2::CommandPtr GetAutonomousCommand();

private:
    Vision m_vision;
    
    frc2::CommandXboxController m_driverController{0};
    
    void ConfigureBindings();
};
