#include <subsystems/elevator/Elevator.h>

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