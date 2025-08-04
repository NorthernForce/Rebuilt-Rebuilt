#include <subsystems/superstructure/elevator/Elevator.h>

using namespace std;
using namespace units;
using namespace frc2;
using namespace rev::spark;

ElevatorIOSparkMax::ElevatorIOSparkMax(
    int busId, int id, SparkLowLevel::MotorType motorType, double kS, double kV,
    double kA, double kP, double kI, double kD, double kG,
    turns_per_second_t kCruiseVelocity,
    turns_per_second_squared_t kAcceleration, turns_per_second_cubed_t kJerk,
    meter_t kSprocketCircumference, double kGearRatio, bool kInverted,
    meter_t kUpperLimit)
    : m_motor(std::make_shared<SparkMax>(id, motorType)),

      m_position(GetPosition()),
      m_temperature(GetTemperature()),
      m_voltage(GetVoltage()),
      m_velocity(GetVelocity()),
      m_current(GetCurrent())
{
    SparkMaxConfig sparkMaxConfigs;

    auto &slot0Configs = sparkMaxConfigs.closedLoop;
    slot0Configs.Pid(kP, kI, kD, ClosedLoopSlot::kSlot0);
    this->kG = kG;

    sparkMaxConfigs.Inverted(kInverted);
    sparkMaxConfigs.SetIdleMode(SparkBaseConfig::IdleMode::kBrake);

    sparkMaxConfigs.encoder.PositionConversionFactor(
        (kGearRatio / kSprocketCircumference).value());

    sparkMaxConfigs.softLimit.ForwardSoftLimitEnabled(true);
    sparkMaxConfigs.softLimit.ForwardSoftLimit(kUpperLimit.value());
    sparkMaxConfigs.softLimit.ReverseSoftLimitEnabled(true);
    sparkMaxConfigs.softLimit.ReverseSoftLimit(0);

    sparkMaxConfigs.SmartCurrentLimit(40);

    m_motor->Configure(sparkMaxConfigs,
                       SparkBase::ResetMode::kResetSafeParameters,
                       SparkBase::PersistMode::kPersistParameters);
}

ElevatorIOSparkMax::ElevatorIOSparkMax(int busId, int id,
                                       SparkLowLevel::MotorType motorType,
                                       ElevatorConstants constants)
    : ElevatorIOSparkMax(busId, id, motorType, constants.kS, constants.kV,
                         constants.kA, constants.kP, constants.kI, constants.kD,
                         constants.kG, constants.kCruiseVelocity,
                         constants.kAcceleration, constants.kJerk,
                         constants.kSprocketCircumference, constants.kGearRatio,
                         constants.kInverted, constants.kUpperLimit)
{
}

void ElevatorIOSparkMax::SetTargetPosition(meter_t position)
{
    m_motor->GetClosedLoopController().SetReference(
        position.value(), SparkLowLevel::ControlType::kPosition);
}

void ElevatorIOSparkMax::SetLowerLimitEnable(bool enableLowerLimit)
{
    SparkMaxConfig config;
    config.softLimit.ReverseSoftLimit(enableLowerLimit);
    m_motor->Configure(config, SparkBase::ResetMode::kResetSafeParameters,
                       SparkBase::PersistMode::kPersistParameters);
}

void ElevatorIOSparkMax::SetSpeed(double speed, bool overrideLowerLimit)
{
    SetLowerLimitEnable(!overrideLowerLimit);
    m_motor->GetClosedLoopController().SetReference(
        speed + kG / frc::RobotController::GetInputVoltage(),
        SparkLowLevel::ControlType::kDutyCycle);
    SetLowerLimitEnable(overrideLowerLimit);
}

void ElevatorIOSparkMax::ResetPosition()
{
    m_motor->GetEncoder().SetPosition(0.0);
}

void ElevatorIOSparkMax::Stop()
{
    m_motor->StopMotor();
}

void ElevatorIOSparkMax::SetVoltage(volt_t voltage)
{
    m_motor->GetClosedLoopController().SetReference(
        voltage.value(), SparkLowLevel::ControlType::kVoltage);
}

turn_t ElevatorIOSparkMax::GetPosition() const
{
    return (turn_t)m_motor->GetEncoder().GetPosition();
}

celsius_t ElevatorIOSparkMax::GetTemperature() const
{
    return (celsius_t)m_motor->GetMotorTemperature();
}

volt_t ElevatorIOSparkMax::GetVoltage() const
{
    return (volt_t)m_motor->GetBusVoltage();
}

turns_per_second_t ElevatorIOSparkMax::GetVelocity() const
{
    return (revolutions_per_minute_t)m_motor->GetEncoder().GetVelocity();
}

ampere_t ElevatorIOSparkMax::GetCurrent() const
{
    return (ampere_t)m_motor->GetOutputCurrent();
}

bool ElevatorIOSparkMax::GetIsPresent() const
{
    return true;
}