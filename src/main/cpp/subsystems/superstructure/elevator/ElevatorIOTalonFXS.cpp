#include <subsystems/superstructure/elevator/Elevator.h>

using namespace std;
using namespace units;
using namespace frc2;

ElevatorIOTalonFXS::ElevatorIOTalonFXS(
    int id, double kS, double kV, double kA, double kP, double kI, double kD,
    double kG, turns_per_second_t kCruiseVelocity,
    turns_per_second_squared_t kAcceleration, turns_per_second_cubed_t kJerk,
    meter_t kSprocketCircumference, double kGearRatio, bool kInverted,
    meter_t kUpperLimit)
    : m_motor(std::make_shared<ctre::phoenix6::hardware::TalonFXS>(id)),

      m_position(m_motor->GetPosition()),
      m_temperature(m_motor->GetDeviceTemp()),
      m_current(m_motor->GetTorqueCurrent()),
      m_velocity(m_motor->GetVelocity()),
      m_rotorVelocity(m_motor->GetRotorVelocity()),
      m_voltage(m_motor->GetMotorVoltage()),
      m_isPresent(m_motor->IsConnected()),
      m_motionMagicVoltage(0_tr),
      m_dutyCycleOut(0),
      m_voltageOut(0_V)
{
    ctre::phoenix6::configs::TalonFXSConfiguration talonFXSConfigs;

    auto &slot0Configs = talonFXSConfigs.Slot0;
    slot0Configs.kS = kS;
    slot0Configs.kV = kV;
    slot0Configs.kA = kA;
    slot0Configs.kP = kP;
    slot0Configs.kI = kI;
    slot0Configs.kD = kD;
    slot0Configs.kG = kG;
    this->kG = kG;

    auto &motionMagicConfigs = talonFXSConfigs.MotionMagic;
    motionMagicConfigs.MotionMagicCruiseVelocity = kCruiseVelocity;
    motionMagicConfigs.MotionMagicAcceleration = kAcceleration;
    motionMagicConfigs.MotionMagicJerk = kJerk;

    talonFXSConfigs.MotorOutput.Inverted =
        (kInverted ? ctre::phoenix6::signals::InvertedValue::Clockwise_Positive
                   : ctre::phoenix6::signals::InvertedValue::
                         CounterClockwise_Positive);
    talonFXSConfigs.MotorOutput.NeutralMode =
        ctre::phoenix6::signals::NeutralModeValue::Brake;

    talonFXSConfigs.ExternalFeedback.RotorToSensorRatio = 1;
    talonFXSConfigs.ExternalFeedback.SensorToMechanismRatio =
        (scalar_t)(kGearRatio / kSprocketCircumference).value();

    talonFXSConfigs.SoftwareLimitSwitch.ForwardSoftLimitEnable = true;
    talonFXSConfigs.SoftwareLimitSwitch.ForwardSoftLimitThreshold =
        (turn_t)kUpperLimit.value();
    talonFXSConfigs.SoftwareLimitSwitch.ReverseSoftLimitEnable = true;
    talonFXSConfigs.SoftwareLimitSwitch.ReverseSoftLimitThreshold = 0_tr;
    talonFXSConfigs.CurrentLimits.StatorCurrentLimit = 40_A;
    talonFXSConfigs.CurrentLimits.StatorCurrentLimitEnable = true;

    m_motor->GetConfigurator().Apply(talonFXSConfigs);
}

ElevatorIOTalonFXS::ElevatorIOTalonFXS(int id, ElevatorConstants constants)
    : ElevatorIOTalonFXS(id, constants.kS, constants.kV, constants.kA,
                         constants.kP, constants.kI, constants.kD, constants.kG,
                         constants.kCruiseVelocity, constants.kAcceleration,
                         constants.kJerk, constants.kSprocketCircumference,
                         constants.kGearRatio, constants.kInverted,
                         constants.kUpperLimit)
{
}

void ElevatorIOTalonFXS::SetTargetPosition(meter_t position)
{
    m_motor->SetControl(
        m_motionMagicVoltage.WithPosition((turn_t)position.value()));
}

void ElevatorIOTalonFXS::SetLowerLimitEnable(bool enableLowerLimit)
{
    ctre::phoenix6::configs::TalonFXSConfiguration config;
    m_motor->GetConfigurator().Refresh(config);
    config.SoftwareLimitSwitch.ReverseSoftLimitEnable = enableLowerLimit;
    m_motor->GetConfigurator().Apply(config);
}

void ElevatorIOTalonFXS::SetSpeed(double speed, bool overrideLowerLimit)
{
    SetLowerLimitEnable(!overrideLowerLimit);
    m_motor->SetControl(m_dutyCycleOut.WithOutput(
        speed + kG / frc::RobotController::GetInputVoltage()));
    SetLowerLimitEnable(overrideLowerLimit);
}

void ElevatorIOTalonFXS::ResetPosition()
{
    m_motor->SetPosition(0_tr);
}

void ElevatorIOTalonFXS::Stop()
{
    m_motor->StopMotor();
}

void ElevatorIOTalonFXS::SetVoltage(volt_t voltage)
{
    m_motor->SetControl(m_voltageOut.WithOutput(voltage));
}

void ElevatorIOTalonFXS::Update()
{
    ctre::phoenix6::BaseStatusSignal::RefreshAll(m_temperature, m_position,
                                                 m_current, m_velocity,
                                                 m_rotorVelocity, m_voltage);
}

turn_t ElevatorIOTalonFXS::GetPosition() const
{
    return m_motor->GetPosition().GetValue();
}

celsius_t ElevatorIOTalonFXS::GetTemperature() const
{
    return m_motor->GetDeviceTemp().GetValue();
}

volt_t ElevatorIOTalonFXS::GetVoltage() const
{
    return m_motor->GetMotorVoltage().GetValue();
}

turns_per_second_t ElevatorIOTalonFXS::GetVelocity() const
{
    return m_motor->GetVelocity().GetValue();
}

turns_per_second_t ElevatorIOTalonFXS::GetRotorVelocity() const
{
    return m_motor->GetRotorVelocity().GetValue();
}

ampere_t ElevatorIOTalonFXS::GetCurrent() const
{
    return m_motor->GetTorqueCurrent().GetValue();
}

bool ElevatorIOTalonFXS::GetIsPresent() const
{
    return m_motor->IsConnected();
}