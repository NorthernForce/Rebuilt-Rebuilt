#pragma once

#include <frc2/command/CommandHelper.h>
#include <frc2/command/Command.h>

class Climber;

class ExtendClimber : public frc2::CommandHelper<frc2::Command, ExtendClimber> {
private:
    Climber* m_climber;

public:
    explicit ExtendClimber(Climber* climber);

    void Initialize() override;
    void Execute() override;
    void End(bool interrupted) override;
    bool IsFinished() override;
};
