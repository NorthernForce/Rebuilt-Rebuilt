#pragma once

#include <frc/RobotController.h>
#include <frc2/command/Command.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/sysid/SysIdRoutine.h>
#include <subsystems/superstructure/elevator/ElevatorSensor.h>
#include <units/math.h>

#include <ctre/phoenix6/SignalLogger.hpp>
#include <ctre/phoenix6/TalonFX.hpp>
#include <memory>
#include <string>

class ElevatorIO;
class ElevatorIOTalonFX;
class ElevatorMoveToPositionCommand;
class ElevatorHomingCommand;

class Elevator : public frc2::SubsystemBase
{
  public:
    Elevator(std::string name, std::shared_ptr<ElevatorIO> motor, std::shared_ptr<ElevatorSensorIO> sensor,
             units::meter_t errorTolerance);
    void SetTargetPosition(units::meter_t position);
    void Set(double speed);
    void Stop();
    units::meter_t GetPosition();
    units::meter_t GetTargetPosition();
    bool IsAtTargetPosition();
    bool IsAtPosition(units::meter_t position);
    ElevatorIO* GetIO();
    ElevatorSensorIO* GetSensor();
    void Periodic() override;

    frc2::CommandPtr GetMoveToPositionCommand(units::meter_t position);
    frc2::CommandPtr GetHomingCommand(double homingSpeed);
    frc2::CommandPtr GetStopCommand();
    frc2::CommandPtr GetManualControlCommand(double speed);

    frc2::sysid::SysIdRoutine GetSysIdRoutine();
    frc2::CommandPtr GetSysIdQuasistaticForward();
    frc2::CommandPtr GetSysIdQuasistaticReverse();
    frc2::CommandPtr GetSysIdDynamicForward();
    frc2::CommandPtr GetSysIdDynamicReverse();

  private:
    std::string m_name;
    std::shared_ptr<ElevatorIO> m_motor;
    std::shared_ptr<ElevatorSensorIO> m_sensor;
    units::meter_t m_targetState;
    units::meter_t m_errorTolerance;
    bool m_hasResetPosition = false;
    frc2::sysid::SysIdRoutine m_sysIdRoutine;
};

class ElevatorIO
{
  public:
    struct ElevatorConstants
    {
        double kS;
        double kV;
        double kA;
        double kP;
        double kI;
        double kD;
        double kG;
        units::turns_per_second_t kCruiseVelocity;
        units::turns_per_second_squared_t kAcceleration;
        units::turns_per_second_cubed_t kJerk;
        units::meter_t kSprocketCircumference;
        double kGearRatio;
        bool kInverted;
        units::meter_t kLowerLimit;
        units::meter_t kUpperLimit;
        units::kilogram_t kMass;
    };
    ElevatorIO() = default;
    virtual ~ElevatorIO() = 0;
    virtual void SetTargetPosition(units::meter_t position) = 0;
    virtual void SetSpeed(double speed, bool overrideLowerLimit) = 0;
    virtual void SetLowerLimitEnable(bool enableLowerLimit) = 0;
    virtual void ResetPosition() = 0;
    virtual void Stop() = 0;
    virtual void SetVoltage(units::volt_t) = 0;
    virtual void Update() = 0;

    virtual units::turn_t GetPosition() = 0;
    virtual units::celsius_t GetTemperature() = 0;
    virtual units::volt_t GetVoltage() = 0;
    virtual units::turns_per_second_t GetVelocity() = 0;
    virtual units::turns_per_second_t GetRotorVelocity() = 0;
    virtual units::ampere_t GetCurrent() = 0;
    virtual bool GetIsPresent() = 0;
};

class ElevatorIOTalonFX : public ElevatorIO
{
  public:
    ElevatorIOTalonFX(int id, double kS, double kV, double kA, double kP,
                      double kI, double kD, double kG,
                      units::turns_per_second_t cruiseVelocity,
                      units::turns_per_second_squared_t acceleration,
                      units::turns_per_second_cubed_t jerk,
                      units::meter_t sprocketCircumference, double gearRatio,
                      bool inverted, units::meter_t upperLimit);
    ElevatorIOTalonFX(int id, ElevatorConstants constants);
    void SetTargetPosition(units::meter_t position) override;
    void SetSpeed(double speed, bool overrideLowerLimit) override;
    void SetLowerLimitEnable(bool enableLowerLimit) override;
    void ResetPosition() override;
    void Stop() override;
    void SetVoltage(units::volt_t) override;
    void Update() override;

    units::turn_t GetPosition() override;
    units::celsius_t GetTemperature() override;
    units::volt_t GetVoltage() override;
    units::turns_per_second_t GetVelocity() override;
    units::turns_per_second_t GetRotorVelocity() override;
    units::ampere_t GetCurrent() override;
    bool GetIsPresent() override;

  private:
    std::shared_ptr<ctre::phoenix6::hardware::TalonFX> m_motor;
    ctre::phoenix6::StatusSignal<units::turn_t> m_position;
    ctre::phoenix6::StatusSignal<units::celsius_t> m_temperature;
    ctre::phoenix6::StatusSignal<units::volt_t> m_voltage;
    ctre::phoenix6::StatusSignal<units::turns_per_second_t>
        m_velocity;
    ctre::phoenix6::StatusSignal<units::turns_per_second_t>
        m_rotorVelocity;
    ctre::phoenix6::StatusSignal<units::ampere_t> m_current;
    ctre::phoenix6::controls::MotionMagicExpoVoltage m_motionMagicVoltage;
    ctre::phoenix6::controls::DutyCycleOut m_dutyCycleOut;
    ctre::phoenix6::controls::VoltageOut m_voltageOut;
    bool m_isPresent;
    double kG;
};

class ElevatorMoveToPositionCommand
    : public frc2::CommandHelper<frc2::Command, ElevatorMoveToPositionCommand>
{
  public:
    ElevatorMoveToPositionCommand(Elevator *elevator, units::meter_t position);
    void Initialize() override;
    bool IsFinished() override;
    void End(bool interrupted) override;

  private:
    Elevator *m_elevator;
    units::meter_t m_position;
};

class ElevatorHomingCommand
    : public frc2::CommandHelper<frc2::Command, ElevatorHomingCommand>
{
  public:
    ElevatorHomingCommand(Elevator *elevator, double speed);
    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End(bool interrupted) override;

  private:
    Elevator *m_elevator;
    double m_speed;
};

class ElevatorHoldAtPositionCommand
    : public frc2::CommandHelper<frc2::Command, ElevatorHoldAtPositionCommand>
{
  public:
    ElevatorHoldAtPositionCommand(Elevator *elevator, units::meter_t position);
    void Initialize() override;
    bool IsFinished() override;
    void End(bool interrupted) override;

  private:
    Elevator *m_elevator;
    units::meter_t m_position;
};

class ElevatorManualControlCommand
    : public frc2::CommandHelper<frc2::Command, ElevatorManualControlCommand>
{
  public:
    ElevatorManualControlCommand(Elevator *elevator, std::function<double()> speed);
    void Execute() override;

  private:
    Elevator *m_elevator;
    std::function<double()> m_speed;
};