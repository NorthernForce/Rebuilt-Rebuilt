// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>

class NFRRobotContainer
{
  public:
    virtual ~NFRRobotContainer() = default;

    virtual void RobotPeriodic() = 0;
    virtual void DisabledInit() = 0;
    virtual void DisabledPeriodic() = 0;
    virtual void DisabledExit() = 0;
    virtual void AutonomousInit() = 0;
    virtual void AutonomousPeriodic() = 0;
    virtual void AutonomousExit() = 0;
    virtual void TeleopInit() = 0;
    virtual void TeleopPeriodic() = 0;
    virtual void TeleopExit() = 0;
    virtual void TestInit() = 0;
    virtual void TestPeriodic() = 0;
    virtual void TestExit() = 0;
    
    virtual frc2::CommandPtr GetAutonomousCommand() = 0;
  private:
    virtual void ConfigureBindings() = 0;
};
