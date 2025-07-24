#pragma once

#include <frc2/command/Command.h>
#include "subsystems/Manipulator/Manipulator.h"

class Outtake : public frc2::CommandHelper<frc2::Command, Outtake> {
    public:
        explicit Outtake(Manipulator* manipulator);
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
    private:
        Manipulator* m_manipulator;
};