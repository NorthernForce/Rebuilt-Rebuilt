#include "subsystems/superstructure/elevator/Elevator.h"

using namespace std;
using namespace units;
using namespace frc2;

Elevator::Elevator(std::string name, std::shared_ptr<ElevatorIO> motor,
                   std::shared_ptr<ElevatorSensorIO> sensor,
                   meter_t errorTolerance)
    : m_motor(motor), m_sensor(sensor), m_sysIdRoutine(GetSysIdRoutine())
{
    m_name = name;
    m_errorTolerance = errorTolerance;
}

void Elevator::SetTargetPosition(meter_t position)
{
    m_targetState = position;
    m_motor->SetTargetPosition(position);
}

void Elevator::Set(double speed)
{
    m_motor->SetSpeed(speed, false);
}

void Elevator::Stop()
{
    m_motor->Stop();
}

CommandPtr Elevator::GetMoveToPositionCommand(meter_t position)
{
    return ElevatorMoveToPositionCommand(this, position).ToPtr();
}

CommandPtr Elevator::GetHomingCommand(double homingSpeed)
{
    return ElevatorHomingCommand(this, homingSpeed).ToPtr();
}

CommandPtr Elevator::GetManualControlCommand(function<double()> speed)
{
    return Run([&] { Set(speed()); });
}

CommandPtr Elevator::GetStopCommand()
{
    return Run([&] { Stop(); });
}

void Elevator::Periodic()
{
    m_motor->Refresh();
    if (m_sensor->IsAtLimit() && !m_hasResetPosition)
    {
        m_motor->ResetPosition();
        m_hasResetPosition = true;
    }
    else
    {
        m_hasResetPosition = false;
    }
}

meter_t Elevator::GetPosition() const
{
    return meter_t(m_motor->GetPosition().value());
}

meter_t Elevator::GetTargetPosition() const
{
    return m_targetState;
}

bool Elevator::IsAtTargetPosition() const
{
    return units::math::abs(GetPosition() - GetTargetPosition()) <=
           m_errorTolerance;
}

bool Elevator::IsAtPosition(meter_t position) const
{
    return units::math::abs(GetPosition() - position) <= m_errorTolerance;
}

void Elevator::Log(const nfr::LogContext& log) const
{
    log["position"] << GetPosition().value();
    log["targetPosition"] << GetTargetPosition().value();
    log["isAtTarget"] << IsAtTargetPosition();
    log["isAtLimit"] << GetSensor()->IsAtLimit();
}

ElevatorIO* Elevator::GetIO()
{
    return m_motor.get();
}

ElevatorSensorIO* Elevator::GetSensor() const
{
    return m_sensor.get();
}

frc2::sysid::SysIdRoutine Elevator::GetSysIdRoutine()
{
    return frc2::sysid::SysIdRoutine(
        frc2::sysid::Config(
            std::nullopt, 4_V, 4_s,
            [](frc::sysid::State state)
            {
                ctre::phoenix6::SignalLogger::WriteString(
                    "SysId_State",
                    frc::sysid::SysIdRoutineLog::StateEnumToString(state));
            }),
        frc2::sysid::Mechanism{[this](units::volt_t output)
                               { m_motor->SetVoltage(output); },
                               {},
                               this});
}

CommandPtr Elevator::GetSysIdQuasistaticForward()
{
    return m_sysIdRoutine.Quasistatic(frc2::sysid::Direction::kForward);
}

CommandPtr Elevator::GetSysIdQuasistaticReverse()
{
    return m_sysIdRoutine.Quasistatic(frc2::sysid::Direction::kReverse);
}

CommandPtr Elevator::GetSysIdDynamicForward()
{
    return m_sysIdRoutine.Dynamic(frc2::sysid::Direction::kForward);
}

CommandPtr Elevator::GetSysIdDynamicReverse()
{
    return m_sysIdRoutine.Dynamic(frc2::sysid::Direction::kReverse);
}