#include <subsystems/elevator/Elevator.h>

ElevatorHomingCommand::ElevatorHomingCommand(Elevator* elevator, double speed)
{
    AddRequirements({elevator});
    m_elevator = elevator;
    m_speed = speed;
}

void ElevatorHomingCommand::Initialize()
{
    m_elevator -> GetIO().SetLowerLimitEnable(false);
}

void ElevatorHomingCommand::Execute()
{
    m_elevator -> GetIO().SetSpeed(-m_speed, true);
}

bool ElevatorHomingCommand::IsFinished()
{
    return m_elevator -> GetSensor().IsAtLimit();
}

void ElevatorHomingCommand::End(bool interrupted)
{
    m_elevator -> GetIO().Stop();
    m_elevator -> GetIO().ResetPosition();
    m_elevator -> GetIO().SetLowerLimitEnable(true);
}