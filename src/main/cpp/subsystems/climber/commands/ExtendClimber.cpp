#include "ExtendClimber.h"
#include "Climber.h"

ExtendClimber::ExtendClimber(Climber* climber)
    : m_climber(climber)
{
    AddRequirements({climber});
}

void ExtendClimber::Initialize()
{
    m_climber->Extend();
}

void ExtendClimber::Execute()
{
    
}

void ExtendClimber::End(bool interrupted)
{
    m_climber->Stop();
}

bool ExtendClimber::IsFinished()
{
    return false; 
}
