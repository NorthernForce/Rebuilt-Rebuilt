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

using namespace std;
using namespace units;
using namespace frc2;

class ElevatorIO;
class ElevatorIOTalonFX;
class ElevatorMoveToPositionCommand;
class ElevatorHomingCommand;

class Elevator : public SubsystemBase
{
  public:
    Elevator(string name, ElevatorIO &motor, ElevatorSensorIO &sensor,
             meter_t errorTolerance);
    void SetTargetPosition(meter_t position);
    void Set(double speed);
    void Stop();
    meter_t GetPosition();
    meter_t GetTargetPosition();
    bool IsAtTargetPosition();
    bool IsAtPosition(meter_t position);
    ElevatorIO &GetIO();
    ElevatorSensorIO &GetSensor();
    void Periodic() override;

    CommandPtr GetMoveToPositionCommand(meter_t position);
    CommandPtr GetHomingCommand(double homingSpeed);
    CommandPtr GetStopCommand();
    CommandPtr GetManualControlCommand(double speed);

    frc2::sysid::SysIdRoutine GetSysIdRoutine();
    CommandPtr GetSysIdQuasistaticForward();
    CommandPtr GetSysIdQuasistaticReverse();
    CommandPtr GetSysIdDynamicForward();
    CommandPtr GetSysIdDynamicReverse();

  private:
    string m_name;
    ElevatorIO &m_motor;
    ElevatorSensorIO &m_sensor;
    meter_t m_targetState;
    meter_t m_errorTolerance;
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
        turns_per_second_t kCruiseVelocity;
        turns_per_second_squared_t kAcceleration;
        turns_per_second_cubed_t kJerk;
        meter_t kSprocketCircumference;
        double kGearRatio;
        bool kInverted;
        meter_t kLowerLimit;
        meter_t kUpperLimit;
        kilogram_t kMass;
    };
    ElevatorIO() = default;
    virtual ~ElevatorIO() = 0;
    virtual void SetTargetPosition(meter_t position) = 0;
    virtual void SetSpeed(double speed, bool overrideLowerLimit) = 0;
    virtual void SetLowerLimitEnable(bool enableLowerLimit) = 0;
    virtual void ResetPosition() = 0;
    virtual void Stop() = 0;
    virtual void SetVoltage(volt_t) = 0;
    virtual void Update() = 0;

    virtual turn_t GetPosition() = 0;
    virtual celsius_t GetTemperature() = 0;
    virtual volt_t GetVoltage() = 0;
    virtual turns_per_second_t GetVelocity() = 0;
    virtual turns_per_second_t GetRotorVelocity() = 0;
    virtual ampere_t GetCurrent() = 0;
    virtual bool GetIsPresent() = 0;
};

class ElevatorIOTalonFX : public ElevatorIO
{
  public:
    ElevatorIOTalonFX(int id, double kS, double kV, double kA, double kP,
                      double kI, double kD, double kG,
                      turns_per_second_t cruiseVelocity,
                      turns_per_second_squared_t acceleration,
                      turns_per_second_cubed_t jerk,
                      meter_t sprocketCircumference, double gearRatio,
                      bool inverted, meter_t upperLimit);
    ElevatorIOTalonFX(int id, ElevatorConstants constants);
    void SetTargetPosition(meter_t position) override;
    void SetSpeed(double speed, bool overrideLowerLimit) override;
    void SetLowerLimitEnable(bool enableLowerLimit) override;
    void ResetPosition() override;
    void Stop() override;
    void SetVoltage(volt_t) override;
    void Update() override;

    turn_t GetPosition();
    celsius_t GetTemperature();
    volt_t GetVoltage();
    turns_per_second_t GetVelocity();
    turns_per_second_t GetRotorVelocity();
    ampere_t GetCurrent();
    bool GetIsPresent();

  private:
    shared_ptr<ctre::phoenix6::hardware::TalonFX> m_motor;
    ctre::phoenix6::StatusSignal<angle::turn_t> m_position;
    ctre::phoenix6::StatusSignal<temperature::celsius_t> m_temperature;
    ctre::phoenix6::StatusSignal<voltage::volt_t> m_voltage;
    ctre::phoenix6::StatusSignal<angular_velocity::turns_per_second_t>
        m_velocity;
    ctre::phoenix6::StatusSignal<angular_velocity::turns_per_second_t>
        m_rotorVelocity;
    ctre::phoenix6::StatusSignal<current::ampere_t> m_current;
    ctre::phoenix6::controls::MotionMagicExpoVoltage m_motionMagicVoltage;
    ctre::phoenix6::controls::DutyCycleOut m_dutyCycleOut;
    ctre::phoenix6::controls::VoltageOut m_voltageOut;
    bool m_isPresent;
    double kG;
};

class ElevatorMoveToPositionCommand
    : public CommandHelper<Command, ElevatorMoveToPositionCommand>
{
  public:
    ElevatorMoveToPositionCommand(Elevator *elevator, meter_t position);
    void Initialize() override;
    bool IsFinished() override;
    void End(bool interrupted) override;

  private:
    Elevator *m_elevator;
    meter_t m_position;
};

class ElevatorHomingCommand
    : public CommandHelper<Command, ElevatorHomingCommand>
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
    : public CommandHelper<Command, ElevatorHoldAtPositionCommand>
{
  public:
    ElevatorHoldAtPositionCommand(Elevator *elevator, meter_t position);
    void Initialize() override;
    bool IsFinished() override;
    void End(bool interrupted) override;

  private:
    Elevator *m_elevator;
    meter_t m_position;
};

class ElevatorManualControlCommand
    : public CommandHelper<Command, ElevatorManualControlCommand>
{
  public:
    ElevatorManualControlCommand(Elevator *elevator, double *speed);
    void Initialize() override;

  private:
    Elevator *m_elevator;
    double *m_speed;
};