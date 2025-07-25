#include <subsystems/superstructure/Superstructure.h>

Superstructure::SuperstructureState GetPresetState(
    ElevatorConstants::SuperstructurePresets preset);

Superstructure::Superstructure(Elevator* innerElevator, Elevator* outerElevator)
    : m_innerElevator(innerElevator), m_outerElevator(outerElevator)
{
    m_target = GetPresetState(ElevatorConstants::SuperstructurePresets::START);
}

void Superstructure::Stop()
{
    m_innerElevator->GetIO().Stop();
    m_outerElevator->GetIO().Stop();
}

void Superstructure::SetTarget(SuperstructureState target)
{
    m_target = target;
}

Superstructure::SuperstructureState Superstructure::GetState()
{
    return SuperstructureState(m_innerElevator->GetPosition(),
                               m_outerElevator->GetPosition());
}

Superstructure::SuperstructureState Superstructure::GetTargetState()
{
    return m_target;
}

Superstructure::SuperstructureState Superstructure::GetPresetState(
    ElevatorConstants::SuperstructurePresets preset)
{
    // TODO fix values
    switch (preset)
    {
        case ElevatorConstants::L1:
            return Superstructure::SuperstructureState(0_m, 0_m);
        case ElevatorConstants::L2:
            return Superstructure::SuperstructureState(0_m, 0_m);
        case ElevatorConstants::L3:
            return Superstructure::SuperstructureState(0_m, 0_m);
        case ElevatorConstants::L4:
            return Superstructure::SuperstructureState(0_m, 0_m);
        case ElevatorConstants::CORAL_STATION:
            return Superstructure::SuperstructureState(0_m, 0_m);
        case ElevatorConstants::START:
            return Superstructure::SuperstructureState(0_m, 0_m);
    }
}

bool Superstructure::IsAtTarget()
{
    return m_innerElevator->IsAtTargetPosition() &&
           m_outerElevator->IsAtTargetPosition();
}

bool Superstructure::IsAtPosition(SuperstructureState position)
{
    return m_innerElevator->IsAtPosition(position.innerElevatorPosition) &&
           m_outerElevator->IsAtPosition(position.outerElevatorPosition);
}

Elevator* Superstructure::GetInnerElevator()
{
    return m_innerElevator;
}

Elevator* Superstructure::GetOuterElevator()
{
    return m_outerElevator;
}

CommandPtr Superstructure::GetGoToPositionCommand(SuperstructureState position)
{
    return SuperstructureGoToPositionCommand(this, position).ToPtr();
}

CommandPtr Superstructure::GetHoldAtPositionCommand(
    SuperstructureState position)
{
    return SuperstructureHoldAtPositionCommand(this, position).ToPtr();
}

CommandPtr Superstructure::GetHomingCommand(double innerElevatorSpeed,
                                            double outerElevatorSpeed)
{
    return SuperstructureHomingCommand(this, innerElevatorSpeed,
                                       outerElevatorSpeed)
        .ToPtr();
}

CommandPtr Superstructure::GetManualControlCommand(double* innerElevatorSpeed,
                                                   double* outerElevatorSpeed)
{
    return SuperstructureManualControlCommand(this, innerElevatorSpeed,
                                              outerElevatorSpeed)
        .ToPtr();
}