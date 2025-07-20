#pragma once

#include <frc2/command/CommandHelper.h>
#include <frc2/command/Command.h>

class Climber;

class RetractClimber : public frc2::CommandHelper<frc2::Command, RetractClimber> {
private:
    Climber* m_climber;

public:
    explicit RetractClimber(Climber* climber);

    void Initialize() override;
    void Execute() override;
    void End(bool interrupted) override;
    bool IsFinished() override;
};