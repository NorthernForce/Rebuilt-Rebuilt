#include "subsystems/Manipulator/Commands/Outtake.h"

Outtake::Outtake(Manipulator* manipulator) : m_manipulator(manipulator) {
    AddRequirements(manipulator);
}

void Outtake::Initialize() {
    m_manipulator->setState(ManipulatorState::OUTTAKING);
}

void Outtake::Execute() {

}

bool Outtake::IsFinished() {
    return !m_manipulator->hasCoralInSensor();
}