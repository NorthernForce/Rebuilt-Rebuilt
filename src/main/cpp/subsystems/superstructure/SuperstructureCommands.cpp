#include <subsystems/superstructure/Superstructure.h>
#include "subsystems/superstructure/elevator/Elevator.h"

using namespace std;
using namespace units;
using namespace frc2;

SuperstructureMoveToPositionCommand::SuperstructureMoveToPositionCommand(
    Superstructure *superstructure,
    Superstructure::SuperstructureState position)
{
    AddCommands(
        ElevatorMoveToPositionCommand(superstructure->GetInnerElevator(), position.innerElevatorPosition),
        ElevatorMoveToPositionCommand(superstructure->GetOuterElevator(), position.outerElevatorPosition));
    AddRequirements(superstructure);
}

SuperstructureHoldAtPositionCommand::SuperstructureHoldAtPositionCommand(
    Superstructure *superstructure,
    Superstructure::SuperstructureState position)
{
    AddCommands(
        ElevatorHoldAtPositionCommand(superstructure->GetInnerElevator(), position.innerElevatorPosition),
        ElevatorHoldAtPositionCommand(superstructure->GetOuterElevator(), position.outerElevatorPosition));
    AddRequirements(superstructure);
}

SuperstructureHomingCommand::SuperstructureHomingCommand(
    Superstructure *superstructure, double innerElevatorSpeed,
    double outerElevatorSpeed)
{
    AddRequirements(superstructure);
    m_superstructure = superstructure;
    AddCommands(ElevatorHomingCommand(m_superstructure->GetInnerElevator(),
                                      innerElevatorSpeed),
                ElevatorHomingCommand(m_superstructure->GetOuterElevator(),
                                      outerElevatorSpeed));
}

SuperstructureManualControlCommand::SuperstructureManualControlCommand(
    Superstructure *superstructure, function<double()> innerElevatorSpeed,
    function<double()> outerElevatorSpeed)
{
    AddRequirements(superstructure);
    m_superstructure = superstructure;
    AddCommands(ElevatorManualControlCommand(
                    m_superstructure->GetInnerElevator(), innerElevatorSpeed),
                ElevatorManualControlCommand(
                    m_superstructure->GetOuterElevator(), outerElevatorSpeed)

    );
}