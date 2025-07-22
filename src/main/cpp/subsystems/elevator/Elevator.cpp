#include "subsystems/elevator/Elevator.h"

Elevator::Elevator(string name, ElevatorIO& motor, meter_t errorTolerance): m_motor(motor){
    m_name = name;
    m_errorTolerance = errorTolerance;
}

void Elevator::SetTargetPosition(meter_t position)
{
    m_targetState = position;
    m_motor.SetTargetPosition(position);
}

void Elevator::Stop()
{
    m_motor.Stop();
}

frc2::CommandPtr Elevator::GetMoveToPositionCommand(meter_t position)
{
    return ElevatorMoveToPositionCommand(this, position).ToPtr();
}

frc2::CommandPtr Elevator::GetHomingCommand(double homingSpeed)
{
    return ElevatorHomingCommand(this, homingSpeed).ToPtr();
}

frc2::CommandPtr Elevator::GetStopCommand()
{
    return Run([&] {Stop(); });
}

void Elevator::Periodic()
{
    m_motor.Update();
}

meter_t Elevator::GetPosition()
{
    return meter_t(m_motor.GetPosition().value());
}

meter_t Elevator::GetTargetPosition()
{
    return m_targetState;
}

bool Elevator::IsAtTargetPosition()
{
    return units::math::abs(GetPosition() - GetTargetPosition()) <= m_errorTolerance;
}

bool Elevator::IsAtPosition(meter_t position)
{
    return units::math::abs(GetPosition() - position) <= m_errorTolerance;
}

ElevatorIO& Elevator::GetIO()
{
    return m_motor;
}