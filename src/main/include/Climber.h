#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/CommandHelper.h>
#include <ctre/phoenix6/TalonFX.hpp>
#include <units/angle.h>

class Climber : public frc2::SubsystemBase {
public:
    Climber(int motorId, double climbSpeed, bool inverted);
    ~Climber() override; // virtual destructor is here

    void Extend();
    void Retract();
    void Stop();
    void Periodic() override;

    frc2::Command* GetExtendCommand();
    frc2::Command* GetRetractCommand();

private:
    int m_motorId;
    double m_climbSpeed;
    ctre::phoenix6::hardware::TalonFX m_motor;

    class ExtendClimberCommand : public frc2::CommandHelper<frc2::Command, ExtendClimberCommand> {
    public:
        ExtendClimberCommand(Climber* climber);
        void Initialize() override;
        void Execute() override;
        void End(bool interrupted) override;
        bool IsFinished() override;

    private:
        Climber* m_climber;
    };

    class RetractClimberCommand : public frc2::CommandHelper<frc2::Command, RetractClimberCommand> {
    public:
        RetractClimberCommand(Climber* climber);
        void Initialize() override;
        void Execute() override;
        void End(bool interrupted) override;
        bool IsFinished() override;

    private:
        Climber* m_climber;
    };
};