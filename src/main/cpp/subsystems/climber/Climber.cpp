#include "subsystems/climber/Climber.h"
#include "subsystems/climber/commands/ExtendClimber.h"
#include "subsystems/climber/commands/RetractClimber.h"
#include <ctre/phoenix6/TalonFX.hpp>
#include <frc2/command/Command.h>
#include <frc2/command/SubsystemBase.h>

Climber::Climber(int motorId, double climbSpeed, bool inverted)
    : m_motorId(motorId), m_climbSpeed(climbSpeed), m_motor(motorId) {
    
    ctre::phoenix6::configs::TalonFXConfiguration config{};
    config.MotorOutput.Inverted = inverted ? ctre::phoenix6::signals::InvertedValue::Clockwise_Positive
                                           : ctre::phoenix6::signals::InvertedValue::CounterClockwise_Positive;
    config.MotorOutput.NeutralMode = ctre::phoenix6::signals::NeutralModeValue::Brake;
    m_motor.GetConfigurator().Apply(config);
}

void Climber::Extend() {
    m_motor.SetControl(ctre::phoenix6::controls::DutyCycleOut{m_climbSpeed});
}

void Climber::Retract() {
    m_motor.SetControl(ctre::phoenix6::controls::DutyCycleOut{-m_climbSpeed});
}

void Climber::Stop() {
    m_motor.SetControl(ctre::phoenix6::controls::DutyCycleOut{0.0});
}

frc2::CommandPtr Climber::GetExtendCommand() {
    return ExtendClimber(this).ToPtr();
}

frc2::CommandPtr Climber::GetRetractCommand() {
    return RetractClimber(this).ToPtr();
}

void Climber::Periodic() {

}
