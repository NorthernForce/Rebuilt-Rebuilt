#include <subsystems/elevator/Elevator.h>

ElevatorMoveToPositionCommand::ElevatorMoveToPositionCommand(Elevator *elevator,
                                                             meter_t position) {
  AddRequirements({elevator});
  m_elevator = elevator;
  m_position = position;
}

void ElevatorMoveToPositionCommand::Initialize() {
  m_elevator->SetTargetPosition(m_position);
}

bool ElevatorMoveToPositionCommand::IsFinished() {
  return m_elevator->IsAtTargetPosition();
}

void ElevatorMoveToPositionCommand::End(bool interrupted) {
  m_elevator->Stop();
}