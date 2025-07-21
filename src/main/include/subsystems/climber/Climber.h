#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/SubsystemBase.h>
#include <ctre/phoenix6/TalonFX.hpp>

class Climber : public frc2::SubsystemBase {
public:
    Climber(int motorId, double climbSpeed, bool inverted);

    void Extend();
    void Retract();
    void Stop();
    void Periodic() override;

    frc2::CommandPtr GetExtendCommand();
    frc2::CommandPtr GetRetractCommand();

private:
    int m_motorId;
    double m_climbSpeed;
    ctre::phoenix6::hardware::TalonFX m_motor;
};