#include "subsystems/climber/commands/RetractClimber.h"
#include "subsystems/climber/Climber.h"

RetractClimber::RetractClimber(Climber* climber)
    : m_climber(climber)
{
    AddRequirements({climber});
}

void RetractClimber::Initialize()
{
    m_climber->Retract();
}

void RetractClimber::Execute()
{
    
}

void RetractClimber::End(bool interrupted)
{
    m_climber->Stop();
}

bool RetractClimber::IsFinished()
{
    return false;
}