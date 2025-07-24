#pragma once

#include <frc/DigitalInput.h>
#include <frc2/command/Command.h>
#include <frc2/command/CommandPtr.h>
#include <frc/Timer.h>
#include <memory>


#include <ctre/phoenix6/TalonFXS.hpp>
#include <ctre/phoenix6/configs/Configs.hpp>

enum class ManipulatorState
{
    HUNGRY,
    PURGING,
    REINTAKING,
    OUTTAKING,
    HAPPY,
    SLOW_OUTTAKING,
    BRUTE_OUTTAKING
};

class Manipulator : public frc2::Subsystem {
    public:
        Manipulator();
        void set(double speed);
        void stop();
        bool hasCoral();
        void setCanIntake(bool canIntake);
        bool hasCoralInSensor();
        void Periodic() override;
        frc2::CommandPtr intake();
        frc2::CommandPtr outtake();
        frc2::CommandPtr slowOuttake();
        ManipulatorState getState();
        void setState(ManipulatorState state);
    private:
        ctre::phoenix6::hardware::TalonFXS m_motor;
        frc::DigitalInput m_sensor;
        ctre::phoenix6::configs::TalonFXSConfiguration config;
        ManipulatorState m_state;
        bool m_canIntake;
        frc::Timer m_timer;
};