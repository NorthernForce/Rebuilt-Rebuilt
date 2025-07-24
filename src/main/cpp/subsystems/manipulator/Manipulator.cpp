#include "subsystems/Manipulator/Manipulator.h"
#include "RobotConstants.h"
#include "subsystems/Manipulator/Commands/Intake.h"
#include "subsystems/Manipulator/Commands/Outtake.h"

using namespace ctre::phoenix6;

Manipulator::Manipulator() : m_motor(ManipulatorConstants::kMotorId), m_sensor(ManipulatorConstants::kSensorId), config() {
        config.MotorOutput.NeutralMode = signals::NeutralModeValue::Brake;
        config.MotorOutput.Inverted = ManipulatorConstants::kMotorInverted
                ? signals::InvertedValue::Clockwise_Positive
                : signals::InvertedValue::CounterClockwise_Positive
        ;
        /*
        IS THIS RIGHT 40_A
        */
        config.CurrentLimits.StatorCurrentLimit = 40_A;
        config.CurrentLimits.StatorCurrentLimitEnable = true;
        config.Commutation.MotorArrangement = signals::MotorArrangementValue::Minion_JST;
        config.Commutation.AdvancedHallSupport = signals::AdvancedHallSupportValue::Enabled;
        m_motor.GetConfigurator().Apply(config);
        m_sensor = frc::DigitalInput{ManipulatorConstants::kSensorId};
        m_timer.Start();
}

void Manipulator::set(double speed) {
        m_motor.Set(speed);
}

void Manipulator::stop() {
        m_motor.Set(0);
}

bool Manipulator::hasCoralInSensor() {
        return !m_sensor.Get();
}

bool Manipulator::hasCoral() {
        return m_state == ManipulatorState::HAPPY;
}

void Manipulator::setCanIntake(bool canIntake) {
        m_canIntake = canIntake;
}

frc2::CommandPtr Manipulator::intake() {
    // ?
}

frc2::CommandPtr Manipulator::outtake() {
    // ?
}

frc2::CommandPtr Manipulator::slowOuttake() {
    // ?
}

ManipulatorState Manipulator::getState() {
        return m_state;
}

void Manipulator::setState(ManipulatorState state) {
        m_state = state;
        if (state == ManipulatorState::BRUTE_OUTTAKING) {
                m_timer.Restart();
        }
}

void Manipulator::Periodic() {
        switch (m_state) {
            case ManipulatorState::HUNGRY:
                set(m_canIntake ? ManipulatorConstants::kIntakeSpeed : 0);
                if (hasCoralInSensor()) m_state = ManipulatorState::PURGING;
                break;

            case ManipulatorState::PURGING:
                set(-ManipulatorConstants::kPurgeSpeed);
                if (!hasCoralInSensor()) {
                    m_state = ManipulatorState::REINTAKING;
                    m_timer.Reset();
                }
                break;

            case ManipulatorState::REINTAKING:
                set(0.2);
                if (m_timer.HasElapsed(units::second_t{0.5})) m_state = ManipulatorState::HUNGRY;
                if (hasCoralInSensor()) m_state = ManipulatorState::HAPPY;
                break;

            case ManipulatorState::HAPPY:
                stop();
                break;

            case ManipulatorState::OUTTAKING:
                set(ManipulatorConstants::kOuttakeSpeed);
                if (!hasCoralInSensor()) m_state = ManipulatorState::HUNGRY;
                break;

            case ManipulatorState::SLOW_OUTTAKING:
                set(ManipulatorConstants::kSlowOuttakeSpeed);
                if (!hasCoralInSensor()) m_state = ManipulatorState::HUNGRY;
                break;

            case ManipulatorState::BRUTE_OUTTAKING:
                set(ManipulatorConstants::kOuttakeSpeed);
                if (m_timer.HasElapsed(units::second_t{1.0})) m_state = ManipulatorState::HUNGRY;
                break;
        }
}