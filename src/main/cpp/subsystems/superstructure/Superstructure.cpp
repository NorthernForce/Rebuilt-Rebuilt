#include <subsystems/superstructure/Superstructure.h>

using namespace std;
using namespace units;
using namespace frc2;

Superstructure::Superstructure(shared_ptr<Elevator> innerElevator,
                               shared_ptr<Elevator> outerElevator)
    : m_innerElevator(innerElevator), m_outerElevator(outerElevator)
{
    m_target = GetPresetState(
        UniversalElevatorConstants::SuperstructurePresets::START);
}

void Superstructure::Stop()
{
    m_innerElevator->Stop();
    m_outerElevator->Stop();
}

void Superstructure::SetTarget(SuperstructureState target)
{
    m_target = target;
}

Superstructure::SuperstructureState Superstructure::GetState() const
{
    return SuperstructureState(m_innerElevator->GetPosition(),
                               m_outerElevator->GetPosition());
}

Superstructure::SuperstructureState Superstructure::GetTargetState() const
{
    return m_target;
}

void Superstructure::Log(const nfr::LogContext& log) const
{
    log["innerElevator"] << m_innerElevator;
    log["outerElevator"] << m_outerElevator;
    log["state"] << GetState();
    log["targetState"] << GetTargetState();
    log["isAtTarget"] << IsAtTarget();
}

Superstructure::SuperstructureState Superstructure::GetPresetState(
    UniversalElevatorConstants::SuperstructurePresets preset) const
{
    // TODO fix values
    switch (preset)
    {
        case UniversalElevatorConstants::L1:
            return Superstructure::SuperstructureState(0_m, 0_m);
        case UniversalElevatorConstants::L2:
            return Superstructure::SuperstructureState(0_m, 0_m);
        case UniversalElevatorConstants::L3:
            return Superstructure::SuperstructureState(0_m, 0_m);
        case UniversalElevatorConstants::L4:
            return Superstructure::SuperstructureState(0_m, 0_m);
        case UniversalElevatorConstants::CORAL_STATION:
            return Superstructure::SuperstructureState(0_m, 0_m);
        case UniversalElevatorConstants::START:
            return Superstructure::SuperstructureState(0_m, 0_m);
        default:
            return Superstructure::SuperstructureState(0_m, 0_m);
    }
}

bool Superstructure::IsAtTarget() const
{
    return m_innerElevator->IsAtTargetPosition() &&
           m_outerElevator->IsAtTargetPosition();
}

bool Superstructure::IsAtPosition(SuperstructureState position) const
{
    return m_innerElevator->IsAtPosition(position.innerElevatorPosition) &&
           m_outerElevator->IsAtPosition(position.outerElevatorPosition);
}

Elevator* Superstructure::GetInnerElevator()
{
    return m_innerElevator.get();
}

Elevator* Superstructure::GetOuterElevator()
{
    return m_outerElevator.get();
}

CommandPtr Superstructure::GetGoToPositionCommand(SuperstructureState position)
{
    return SuperstructureMoveToPositionCommand(this, position).ToPtr();
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

CommandPtr Superstructure::GetManualControlCommand(
    function<double()> innerElevatorSpeed,
    function<double()> outerElevatorSpeed)
{
    return SuperstructureManualControlCommand(this, innerElevatorSpeed,
                                              outerElevatorSpeed)
        .ToPtr();
}

void Superstructure::SuperstructureState::Log(const nfr::LogContext& log) const
{
    log["innerElevatorPosition"] << this->innerElevatorPosition.value();
    log["outerElevatorPosition"] << this->outerElevatorPosition.value();
}