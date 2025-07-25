#include <subsystems/superstructure/elevator/Elevator.h>

ElevatorIOTalonFX::ElevatorIOTalonFX(int id, double kS, double kV, double kA,
                                     double kP, double kI, double kD, double kG,
                                     turns_per_second_t kCruiseVelocity,
                                     turns_per_second_squared_t kAcceleration,
                                     turns_per_second_cubed_t kJerk,
                                     meter_t kSprocketCircumference,
                                     double kGearRatio, bool kInverted,
                                     meter_t kUpperLimit)
    : m_motor(std::make_shared<ctre::phoenix6::hardware::TalonFX>(id)),

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
    ctre::phoenix6::configs::TalonFXConfiguration talonFXConfigs;

    auto &slot0Configs = talonFXConfigs.Slot0;
    slot0Configs.kS = kS;
    slot0Configs.kV = kV;
    slot0Configs.kA = kA;
    slot0Configs.kP = kP;
    slot0Configs.kI = kI;
    slot0Configs.kD = kD;
    slot0Configs.kG = kG;
    this->kG = kG;

    auto &motionMagicConfigs = talonFXConfigs.MotionMagic;
    motionMagicConfigs.MotionMagicCruiseVelocity = 160_tps;
    motionMagicConfigs.MotionMagicAcceleration = 160_tr_per_s_sq;
    motionMagicConfigs.MotionMagicJerk = kJerk;

    talonFXConfigs.MotorOutput.Inverted =
        (kInverted ? ctre::phoenix6::signals::InvertedValue::Clockwise_Positive
                   : ctre::phoenix6::signals::InvertedValue::
                         CounterClockwise_Positive);
    talonFXConfigs.MotorOutput.NeutralMode =
        ctre::phoenix6::signals::NeutralModeValue::Brake;

    talonFXConfigs.Feedback.RotorToSensorRatio = 1;
    talonFXConfigs.Feedback.SensorToMechanismRatio =
        (scalar_t)(kGearRatio / kSprocketCircumference).value();

    talonFXConfigs.SoftwareLimitSwitch.ForwardSoftLimitEnable = true;
    talonFXConfigs.SoftwareLimitSwitch.ForwardSoftLimitThreshold =
        (turn_t)kUpperLimit.value();
    talonFXConfigs.SoftwareLimitSwitch.ReverseSoftLimitEnable = true;
    talonFXConfigs.SoftwareLimitSwitch.ReverseSoftLimitThreshold = 0_tr;
    talonFXConfigs.CurrentLimits.StatorCurrentLimit = 40_A;
    talonFXConfigs.CurrentLimits.StatorCurrentLimitEnable = true;

    m_motor->GetConfigurator().Apply(talonFXConfigs);
}

ElevatorIOTalonFX::ElevatorIOTalonFX(int id, ElevatorConstants constants)
    : ElevatorIOTalonFX(id, constants.kS, constants.kV, constants.kA,
                        constants.kP, constants.kI, constants.kD, constants.kG,
                        constants.kCruiseVelocity, constants.kAcceleration,
                        constants.kJerk, constants.kSprocketCircumference,
                        constants.kGearRatio, constants.kInverted,
                        constants.kUpperLimit)
{
}

void ElevatorIOTalonFX::SetTargetPosition(meter_t position)
{
    m_motor->SetControl(
        m_motionMagicVoltage.WithPosition((turn_t)position.value()));
}

void ElevatorIOTalonFX::SetLowerLimitEnable(bool enableLowerLimit)
{
    ctre::phoenix6::configs::TalonFXConfiguration config;
    m_motor->GetConfigurator().Refresh(config);
    config.SoftwareLimitSwitch.ReverseSoftLimitEnable = enableLowerLimit;
    m_motor->GetConfigurator().Apply(config);
}

void ElevatorIOTalonFX::SetSpeed(double speed, bool overrideLowerLimit)
{
    m_motor->SetControl(m_dutyCycleOut.WithOutput(
        speed + kG / frc::RobotController::GetInputVoltage()));
}

void ElevatorIOTalonFX::ResetPosition()
{
    m_motor->SetPosition(0_tr);
}

void ElevatorIOTalonFX::Stop()
{
    m_motor->StopMotor();
}

void ElevatorIOTalonFX::SetVoltage(volt_t voltage)
{
    m_motor->SetControl(m_voltageOut.WithOutput(voltage));
}

void ElevatorIOTalonFX::Update()
{
    ctre::phoenix6::BaseStatusSignal::RefreshAll(m_temperature, m_position,
                                                 m_current, m_velocity,
                                                 m_rotorVelocity, m_voltage);
}

turn_t ElevatorIOTalonFX::GetPosition()
{
    return m_motor->GetPosition().GetValue();
}

celsius_t ElevatorIOTalonFX::GetTemperature()
{
    return m_motor->GetDeviceTemp().GetValue();
}

volt_t ElevatorIOTalonFX::GetVoltage()
{
    return m_motor->GetMotorVoltage().GetValue();
}

turns_per_second_t ElevatorIOTalonFX::GetVelocity()
{
    return m_motor->GetVelocity().GetValue();
}

turns_per_second_t ElevatorIOTalonFX::GetRotorVelocity()
{
    return m_motor->GetRotorVelocity().GetValue();
}

ampere_t ElevatorIOTalonFX::GetCurrent()
{
    return m_motor->GetTorqueCurrent().GetValue();
}

bool ElevatorIOTalonFX::GetIsPresent()
{
    return m_motor->IsConnected();
}