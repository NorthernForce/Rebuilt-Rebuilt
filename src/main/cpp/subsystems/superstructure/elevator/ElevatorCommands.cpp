#include <subsystems/superstructure/elevator/Elevator.h>

ElevatorHoldAtPositionCommand::ElevatorHoldAtPositionCommand(Elevator *elevator,
                                                             meter_t position)
{
    AddRequirements({elevator});
    m_elevator = elevator;
    m_position = position;
}

void ElevatorHoldAtPositionCommand::Initialize()
{
    m_elevator->GetIO().SetTargetPosition(m_position);
}

bool ElevatorHoldAtPositionCommand::IsFinished()
{
    return false;
}

void ElevatorHoldAtPositionCommand::End(bool interrupted)
{
    m_elevator->GetIO().Stop();
}

ElevatorHomingCommand::ElevatorHomingCommand(Elevator *elevator, double speed)
{
    AddRequirements({elevator});
    m_elevator = elevator;
    m_speed = speed;
}

void ElevatorHomingCommand::Initialize()
{
    m_elevator->GetIO().SetLowerLimitEnable(false);
}

void ElevatorHomingCommand::Execute()
{
    m_elevator->GetIO().SetSpeed(-m_speed, true);
}

bool ElevatorHomingCommand::IsFinished()
{
    return m_elevator->GetSensor().IsAtLimit();
}

void ElevatorHomingCommand::End(bool interrupted)
{
    m_elevator->GetIO().Stop();
    m_elevator->GetIO().ResetPosition();
    m_elevator->GetIO().SetLowerLimitEnable(true);
}

ElevatorMoveToPositionCommand::ElevatorMoveToPositionCommand(Elevator *elevator,
                                                             meter_t position)
{
    AddRequirements({elevator});
    m_elevator = elevator;
    m_position = position;
}

void ElevatorMoveToPositionCommand::Initialize()
{
    m_elevator->SetTargetPosition(m_position);
}

bool ElevatorMoveToPositionCommand::IsFinished()
{
    return m_elevator->IsAtTargetPosition();
}

void ElevatorMoveToPositionCommand::End(bool interrupted)
{
    m_elevator->Stop();
}

ElevatorManualControlCommand::ElevatorManualControlCommand(Elevator *elevator,
                                                           function<double()> speed)
{
    AddRequirements({elevator});
    m_elevator = elevator;
    m_speed = speed;
}

void ElevatorManualControlCommand::Execute()
{
    m_elevator->GetIO().SetSpeed(m_speed(), false);
}