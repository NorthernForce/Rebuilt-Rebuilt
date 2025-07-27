#include <subsystems/superstructure/Superstructure.h>

SuperstructureGoToPositionCommand::SuperstructureGoToPositionCommand(
    Superstructure *superstructure,
    Superstructure::SuperstructureState position)
{
    AddRequirements({superstructure});
    m_superstructure = superstructure;
    m_position = position;
}

void SuperstructureGoToPositionCommand::Initialize()
{
    m_superstructure->SetTarget(m_position);
    m_superstructure->GetInnerElevator()->SetTargetPosition(
        m_position.innerElevatorPosition);
    m_superstructure->GetOuterElevator()->SetTargetPosition(
        m_position.outerElevatorPosition);
}

bool SuperstructureGoToPositionCommand::IsFinished()
{
    return m_superstructure->IsAtPosition(m_position);
}

SuperstructureHoldAtPositionCommand::SuperstructureHoldAtPositionCommand(
    Superstructure *superstructure,
    Superstructure::SuperstructureState position)
{
    AddRequirements({superstructure});
    m_superstructure = superstructure;
    m_position = position;
}

void SuperstructureHoldAtPositionCommand::Initialize()
{
    m_superstructure->SetTarget(m_position);
    m_superstructure->GetInnerElevator()->SetTargetPosition(
        m_position.innerElevatorPosition);
    m_superstructure->GetOuterElevator()->SetTargetPosition(
        m_position.outerElevatorPosition);
}

bool SuperstructureHoldAtPositionCommand::IsFinished()
{
    return false;
}

SuperstructureHomingCommand::SuperstructureHomingCommand(
    Superstructure *superstructure, double innerElevatorSpeed,
    double outerElevatorSpeed)
{
    AddRequirements({superstructure});
    m_superstructure = superstructure;
    AddCommands(ElevatorHomingCommand(m_superstructure->GetInnerElevator(),
                                      innerElevatorSpeed),
                ElevatorHomingCommand(m_superstructure->GetOuterElevator(),
                                      outerElevatorSpeed));
}

SuperstructureManualControlCommand::SuperstructureManualControlCommand(
    Superstructure *superstructure, double *innerElevatorSpeed,
    double *outerElevatorSpeed)
{
    AddRequirements({superstructure});
    m_superstructure = superstructure;
    AddCommands(ElevatorManualControlCommand(m_superstructure->GetInnerElevator(), innerElevatorSpeed),
    ElevatorManualControlCommand(m_superstructure->GetOuterElevator(), outerElevatorSpeed)

    );
}