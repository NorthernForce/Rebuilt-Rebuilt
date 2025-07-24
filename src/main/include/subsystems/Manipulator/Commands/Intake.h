#pragma once

#include <frc2/command/Command.h>

#include "subsystems/Manipulator/Manipulator.h"

class Intake : public frc2::Command
{
  public:
    explicit Intake(Manipulator* manipulator);
    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;

  private:
    Manipulator* m_manipulator;
};