#include "subsystems/Manipulator/Commands/Intake.h"

Intake::Intake(Manipulator* manipulator) : m_manipulator(manipulator)
{
    AddRequirements(manipulator);
}

void Intake::Initialize()
{
}

void Intake::Execute()
{
}

bool Intake::IsFinished()
{
    return m_manipulator->hasCoral();
}