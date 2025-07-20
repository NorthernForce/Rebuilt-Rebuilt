#include "Climber.h"
#include "ExtendClimber.h"
#include "RetractClimber.h"
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

// I created an empty destructor to ensure proper cleanup, even though it does nothing in this case.

Climber::~Climber() {
    
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

frc2::Command* Climber::GetExtendCommand() {
    return new ExtendClimber(this);
}

frc2::Command* Climber::GetRetractCommand() {
    return new RetractClimber(this);
}

void Climber::Periodic() {

}
