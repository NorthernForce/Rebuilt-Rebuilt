#pragma once

#include <constants/ElevatorConstants.h>
#include <frc/RobotController.h>
#include <frc/simulation/ElevatorSim.h>
#include <frc2/command/Command.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/sysid/SysIdRoutine.h>
#include <logging/Logger.h>
#include <rev/SparkMax.h>
#include <rev/config/SparkMaxConfig.h>
#include <rev/sim/SparkMaxSim.h>
#include <subsystems/superstructure/elevator/ElevatorSensor.h>
#include <units/constants.h>
#include <units/math.h>

#include <ctre/phoenix6/SignalLogger.hpp>
#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/TalonFXS.hpp>
#include <memory>
#include <string>

class ElevatorIO;
class ElevatorIOTalonFX;
class ElevatorIOTalonFXS;
class ElevatorIOSparkMax;
class ElevatorIOTalonFXSim;
class ElevatorIOTalonFXSSim;
class ElevatorIOSparkMaxSim;
class ElevatorMoveToPositionCommand;
class ElevatorHomingCommand;

/**
* @brief A class to control a single stage of the elevator, including one motor and one sensor at the bottom limit of the stage
*/
class Elevator : public frc2::SubsystemBase
{
public:

    /**
    * @param name The name of this elevator stage
    * @param motor A shared pointer to any implementation of the abstract class ElevatorIO
    * @param sensor A shared pointer to any implementation of the abstract class ElevatorSensorIO
    * @param errorTolerance A length value used to determine whether the elevator is at an acceptable distance from its target (Recommended to be between 0.25 and 1 inch)
    */
    Elevator(std::string name, std::shared_ptr<ElevatorIO> motor,
             std::shared_ptr<ElevatorSensorIO> sensor,
             units::meter_t errorTolerance);

    /**
    * @brief Sets the target position of the elevator and starts the motor moving towards that target
    * @param position The desired position
    */
    void SetTargetPosition(units::meter_t position);

    /**
    * @brief Sets the motor moving at a supplied speed
    * @param speed The speed for the motor to move (0.0 - 1.0)
    */
    void Set(double speed);

    /**
    * @brief Stops the motor
    */
    void Stop();

    /**
    * @brief Gets the position of the elevator
    * @return The position of this elevator stage
    */
    units::meter_t GetPosition() const;

    /**
    * @brief Gets the target position of the elevator
    * @return The target position of thsi elevator stage
    */
    units::meter_t GetTargetPosition() const;

    /**
    * @brief Determines whether the elevator is within an acceptable distance from its target using the error tolerance variable
    * @return True if the elevator is at its target position
    */
    bool IsAtTargetPosition() const;

    /**
    * @brief Determines whether the elevator is within an acceptable distance from the supplied position using the error tolerance variable
    * @param position The reference position
    * @return True if the elevator is at the supplied position
    */
    bool IsAtPosition(units::meter_t position) const;

    /**
    * @brief Gets the ElevatorIO
    */
    ElevatorIO *GetIO();

    /**
    * @brief Gets the ElevatorSensorIO
    */
    ElevatorSensorIO *GetSensor() const;

    /**
    * @brief This function runs every 0.02 seconds
    */
    void Periodic() override;

    /**
    * @brief Implementation of the universal log function
    */
    void Log(const nfr::LogContext &log) const;

    /**
    * @brief Gets a command to move the elevator to the supplied position
    * This command sets the motor to move to a position using a PID loop and finishes when the elevator has reached that position
    * @param position The position to move to
    * @return An ElevatorMoveToPositionCommand
    */
    frc2::CommandPtr GetMoveToPositionCommand(units::meter_t position);

    /**
    * @brief Gets a command to hold the elevator at a supplied position
    * This command does not end and keeps trying to move the elevator to the position
    * @param position The position to hold at
    * @return An ElevatorHoldAtPositionCommand
    */
    frc2::CommandPtr GetHoldAtPositionCommand(units::meter_t position);

    /**
    * @brief Gets a command to home the elevator
    * This command disables the lower limit of the stage and moves it at the supplied speed until the sensor is triggered
    * @param homingSpeed The speed to home the elevator at
    * @return An ElevatorHomingCommand
    */
    frc2::CommandPtr GetHomingCommand(double homingSpeed);

    /**
    * @brief Gets a command to stop the elevator
    * This command stops all movement in the elevator stage
    * @return A command to stop the elevator
    */
    frc2::CommandPtr GetStopCommand();

    /**
    * @brief Gets a command to control the elevator stage with a joystick
    * @param speed A function that returns the desired speed value (0.0 - 1.0)
    * @return A command to manually control the elevator
    */
    frc2::CommandPtr GetManualControlCommand(std::function<double()> speed);

    /**
    * @brief Gets a sysid routine for the elevator stage
    * This routine runs the elevator at 4 volts for 4 seconds
    * @return A sysid routine for the elevator
    */
    frc2::sysid::SysIdRoutine GetSysIdRoutine();

    /**
    * @brief Gets a command to run a quasistatic sysid routine in the forward direction
    * In a quasistatic routine the motor is sped up gradually, so the voltage being used to accelerate the motor is effectively zero (hence: static)
    */
    frc2::CommandPtr GetSysIdQuasistaticForward();

    /**
    * @brief Gets a command to run a quasistatic sysid routine in the reverse direction
    * In a quasistatic routine the motor is sped up gradually, so the voltage being used to accelerate the motor is effectively zero (hence: static)
    */
    frc2::CommandPtr GetSysIdQuasistaticReverse();

    /**
    * @brief Gets a command to run a dynamic sysid routine in the forward direction
    * In a dynamic routine a step-voltage is applied to the motor, so acceleration behaviors can be determined
    */
    frc2::CommandPtr GetSysIdDynamicForward();

    /**
    * @brief Gets a command to run a dynamic sysid routine in the reverse direction
    * In a dynamic routine a step-voltage is applied to the motor, so acceleration behaviors can be determined
    */
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

/**
* @brief An abstract class to control an elevator motor
* This class has no actual functionality
*/
class ElevatorIO
{
public:
    ElevatorIO() = default;
    virtual ~ElevatorIO() = default;
    virtual void SetTargetPosition(units::meter_t position) = 0;
    virtual void SetSpeed(double speed, bool overrideLowerLimit) = 0;
    virtual void SetLowerLimitEnable(bool enableLowerLimit) = 0;
    virtual void ResetPosition() = 0;
    virtual void Stop() = 0;
    virtual void SetVoltage(units::volt_t) = 0;
    virtual void Refresh() = 0;

    virtual units::turn_t GetPosition() const = 0;
    virtual units::celsius_t GetTemperature() const = 0;
    virtual units::volt_t GetVoltage() const = 0;
    virtual units::turns_per_second_t GetVelocity() const = 0;
    virtual units::turns_per_second_t GetRotorVelocity() const = 0;
    virtual units::ampere_t GetCurrent() const = 0;
    virtual bool GetIsPresent() const = 0;
};

/**
* @brief A class to control an elevator using any motor with a TalonFX controller (Falcon500, KrakenX60, KrakenX44)
*/
class ElevatorIOTalonFX : public ElevatorIO
{
public:
    /**
    * @brief An expanded constructor with all values found in the ElevatorConstants struct as separate parameters
    * @param id The CAN id of the motor
    * @param kS
    * @param kV
    * @param kA
    * @param kP The Proportional value of the PID loop used to control this motor
    * @param kI The Integral value of the PID loop used to control this motor
    * @param kD The Derivative value of the PID loop used to control this motor
    * @param kG
    * @param kCruiseVelocity The cruise velocity value for Phoenix6's MotionMagic
    * @param kAcceleration The acceleration value for Phoenix6's MotionMagic
    * @param kJerk The acceleration value for Phoenix6's MotionMagic
    * @param kSprocketCircumference The circumference of the sprocket directly attached to the elevator (How far the elevator moves with one turn of the driving shaft)
    * @param kGearRatio The gear ratio from the output shaft of the motor to the driving shaft
    * @param kInverted true if the motor should run in reverse, false if not
    * @param kUpperLimit The maximum mechanical height that the elevator stage is capable of
    */
    ElevatorIOTalonFX(int id, double kS, double kV, double kA, double kP,
                      double kI, double kD, double kG,
                      units::turns_per_second_t kCruiseVelocity,
                      units::turns_per_second_squared_t kAcceleration,
                      units::turns_per_second_cubed_t kJerk,
                      units::meter_t kSprocketCircumference, double kGearRatio,
                      bool kInverted, units::meter_t kUpperLimit);

    /**
    * @brief A compressed constructor using the ElevatorConstants struct
    * This constructor calls the expanded constructor with the values contained in the "constants" parameter
    * @param id The CAN id of the motor
    * @param constants An ElevatorConstants struct containing all the constants corresponding to that elevator stage
    */
    ElevatorIOTalonFX(int id, ElevatorConstants constants);

    /**
    * @brief Starts moving the motor towards the desired position using a MotionMagicExpoVoltage loop
    * @param position The desired position for the motor to move to
    */
    void SetTargetPosition(units::meter_t position) override;

    /** 
    * @brief Sets the speed of the motor directly
    * @param speed The speed to move the motor at (0.0 - 1.0)
    * @param overrideLowerLimit Should only be true if the elevator is homing and the lower limit of the elevator is being reset with a functioning limit switch
    */
    void SetSpeed(double speed, bool overrideLowerLimit) override;

    /**
    * @brief Sets whether the lower soft limit of the motor should be enabled
    * @param enableLowerLimit true if the lower soft limit should be enabled, false if not
    */
    void SetLowerLimitEnable(bool enableLowerLimit) override;

    /**
    * @brief Sets the position of the elevator to zero
    * This should only be used if the elevator is at the lower hard limit, NOT the soft limit
    */
    void ResetPosition() override;

    /**
    * @brief Stops the motor
    */
    void Stop() override;

    /**
    * @brief Runs the motor at a desired velocity
    * @param voltage The voltage that should be supplied to the motor
    */
    void SetVoltage(units::volt_t voltage) override;

    /**
    * @brief Refreshes the StatusSignals for the motor
    */
    void Refresh() override;

    /** 
    * @brief Gets the position of the elevator
    */
    units::turn_t GetPosition() const override;

    /**
    * @brief Gets the temperature of the motor
    */
    units::celsius_t GetTemperature() const override;

    /**
    * @brief Gets the voltage being supplied to the motor
    */
    units::volt_t GetVoltage() const override;

    /**
    * @brief Gets the velocity of the driving shaft of the elevator
    */
    units::turns_per_second_t GetVelocity() const override;

    /**
    * @brief Gets the velocity of the motor's output shaft
    */
    units::turns_per_second_t GetRotorVelocity() const override;

    /**
    * @brief Gets the current being supplied to the motor
    */
    units::ampere_t GetCurrent() const override;

    /**
    * @return true if the motor is conected, false if not
    */
    bool GetIsPresent() const override;

private:
    std::shared_ptr<ctre::phoenix6::hardware::TalonFX> m_motor;
    ctre::phoenix6::StatusSignal<units::turn_t> m_position;
    ctre::phoenix6::StatusSignal<units::celsius_t> m_temperature;
    ctre::phoenix6::StatusSignal<units::volt_t> m_voltage;
    ctre::phoenix6::StatusSignal<units::turns_per_second_t> m_velocity;
    ctre::phoenix6::StatusSignal<units::turns_per_second_t> m_rotorVelocity;
    ctre::phoenix6::StatusSignal<units::ampere_t> m_current;
    ctre::phoenix6::controls::MotionMagicExpoVoltage m_motionMagicVoltage;
    ctre::phoenix6::controls::DutyCycleOut m_dutyCycleOut;
    ctre::phoenix6::controls::VoltageOut m_voltageOut;
    bool m_isPresent;
    double kG;
};

/**
* @brief A class that inherits ElevatorIOTalonFX and is used to simulate a TalonFX motor
*/
class ElevatorIOTalonFXSim : public ElevatorIOTalonFX
{
public:
    /**
    * @param id The CAN id of the motor
    * @param constants The ElevatorConstants corresponding to this motor
    * @param dcMotorType The type of motor being used. Many motors are already included under frc::DCMotor::[motor name]
    */
    ElevatorIOTalonFXSim(int id, ElevatorConstants constants,
                         frc::DCMotor dcMotorType);

    /**
    * @brief Updates the ElevatorSim with the voltages being supplied and the TalonFXSimState with the motor's position and velocity
    */
    void UpdateSim();

private:
    frc::sim::ElevatorSim m_elevatorSim;
    ctre::phoenix6::sim::TalonFXSimState m_simState;
    ElevatorConstants m_constants;
};

/**
* @brief A class to control any elevator using a TalonFXS motor controller
*/
class ElevatorIOTalonFXS : public ElevatorIO
{
public:
    /**
    * @brief An expanded constructor with all values found in the ElevatorConstants struct as separate parameters
    * @param id The CAN id of the motor
    * @param kS
    * @param kV
    * @param kA
    * @param kP The Proportional value of the PID loop used to control this motor
    * @param kI The Integral value of the PID loop used to control this motor
    * @param kD The Derivative value of the PID loop used to control this motor
    * @param kG
    * @param kCruiseVelocity The cruise velocity value for Phoenix6's MotionMagic
    * @param kAcceleration The acceleration value for Phoenix6's MotionMagic
    * @param kJerk The acceleration value for Phoenix6's MotionMagic
    * @param kSprocketCircumference The circumference of the sprocket directly attached to the elevator (How far the elevator moves with one turn of the driving shaft)
    * @param kGearRatio The gear ratio from the output shaft of the motor to the driving shaft
    * @param kInverted true if the motor should run in reverse, false if not
    * @param kUpperLimit The maximum mechanical height that the elevator stage is capable of
    */
    ElevatorIOTalonFXS(int id, double kS, double kV, double kA, double kP,
                       double kI, double kD, double kG,
                       units::turns_per_second_t kCruiseVelocity,
                       units::turns_per_second_squared_t kAcceleration,
                       units::turns_per_second_cubed_t kJerk,
                       units::meter_t kSprocketCircumference, double kGearRatio,
                       bool kInverted, units::meter_t kUpperLimit);

    /**
    * @brief A compressed constructor using the ElevatorConstants struct
    * This constructor calls the expanded constructor with the values contained in the "constants" parameter
    * @param id The CAN id of the motor
    * @param constants An ElevatorConstants struct containing all the constants corresponding to that elevator stage
    */
    ElevatorIOTalonFXS(int id, ElevatorConstants kConstants);
    
    /**
    * @brief Starts moving the motor towards the desired position using a MotionMagicExpoVoltage loop
    * @param position The desired position for the motor to move to
    */
    void SetTargetPosition(units::meter_t position) override;
    
    /** 
    * @brief Sets the speed of the motor directly
    * @param speed The speed to move the motor at (0.0 - 1.0)
    * @param overrideLowerLimit Should only be true if the elevator is homing and the lower limit of the elevator is being reset with a functioning limit switch
    */
    void SetSpeed(double speed, bool overrideLowerLimit) override;
    
    /**
    * @brief Sets whether the lower soft limit of the motor should be enabled
    * @param enableLowerLimit true if the lower soft limit should be enabled, false if not
    */
    void SetLowerLimitEnable(bool enableLowerLimit) override;
    
    /**
    * @brief Sets the position of the elevator to zero
    * This should only be used if the elevator is at the lower hard limit, NOT the soft limit
    */
    void ResetPosition() override;
    
    /**
    * @brief Stops the motor
    */
    void Stop() override;
    
    /**
    * @brief Runs the motor at a desired velocity
    * @param voltage The voltage that should be supplied to the motor
    */
    void SetVoltage(units::volt_t) override;
    
    /**
    * @brief Refreshes the StatusSignals for the motor
    */
    void Refresh() override;

    /** 
    * @brief Gets the position of the elevator
    */
    units::turn_t GetPosition() const override;

    /**
    * @brief Gets the temperature of the motor
    */
    units::celsius_t GetTemperature() const override;

    /**
    * @brief Gets the voltage being supplied to the motor
    */
    units::volt_t GetVoltage() const override;

    /**
    * @brief Gets the velocity of the driving shaft of the elevator
    */
    units::turns_per_second_t GetVelocity() const override;

    /**
    * @brief Gets the velocity of the motor's output shaft
    */
    units::turns_per_second_t GetRotorVelocity() const override;

    /**
    * @brief Gets the current being supplied to the motor
    */
    units::ampere_t GetCurrent() const override;

    /**
    * @return true if the motor is conected, false if not
    */
    bool GetIsPresent() const override;

private:
    std::shared_ptr<ctre::phoenix6::hardware::TalonFXS> m_motor;
    ctre::phoenix6::StatusSignal<units::turn_t> m_position;
    ctre::phoenix6::StatusSignal<units::celsius_t> m_temperature;
    ctre::phoenix6::StatusSignal<units::volt_t> m_voltage;
    ctre::phoenix6::StatusSignal<units::turns_per_second_t> m_velocity;
    ctre::phoenix6::StatusSignal<units::turns_per_second_t> m_rotorVelocity;
    ctre::phoenix6::StatusSignal<units::ampere_t> m_current;
    ctre::phoenix6::controls::MotionMagicExpoVoltage m_motionMagicVoltage;
    ctre::phoenix6::controls::DutyCycleOut m_dutyCycleOut;
    ctre::phoenix6::controls::VoltageOut m_voltageOut;
    bool m_isPresent;
    double kG;
};

/**
* @brief A class that inherits ElevatorIOTalonFXS and is used to simulate a TalonFXS motor controller
*/
class ElevatorIOTalonFXSSim : public ElevatorIOTalonFXS
{
public:
    /**
    * @param id The CAN id of the motor
    * @param constants The ElevatorConstants corresponding to this motor
    * @param dcMotorType The type of motor being used. Many motors are already included under frc::DCMotor::[motor name]
    */
    ElevatorIOTalonFXSSim(int id, ElevatorConstants constants,
                          frc::DCMotor dcMotorType);
    
    /**
    * @brief Updates the ElevatorSim with the voltages being supplied and the TalonFXSimState with the motor's position and velocity
    */
    void UpdateSim();

private:
    frc::sim::ElevatorSim m_elevatorSim;
    ctre::phoenix6::sim::TalonFXSSimState m_simState;
    ElevatorConstants m_constants;
};

/**
* @brief A class to control any elevator using a SparkMAX motor controller */
class ElevatorIOSparkMax : public ElevatorIO
{
public:
/**
    * @brief An expanded constructor with all values found in the ElevatorConstants struct as separate parameters
    * @param busId The CAN bus id
    * @param id The CAN id of the motor
    * @param motorType The selected motor type from the enum rev::spark::SparkLowLevel::MotorType
    * @param kS
    * @param kV
    * @param kA
    * @param kP The Proportional value of the PID loop used to control this motor
    * @param kI The Integral value of the PID loop used to control this motor
    * @param kD The Derivative value of the PID loop used to control this motor
    * @param kG
    * @param kCruiseVelocity The cruise velocity value for Phoenix6's MotionMagic
    * @param kAcceleration The acceleration value for Phoenix6's MotionMagic
    * @param kJerk The acceleration value for Phoenix6's MotionMagic
    * @param kSprocketCircumference The circumference of the sprocket directly attached to the elevator (How far the elevator moves with one turn of the driving shaft)
    * @param kGearRatio The gear ratio from the output shaft of the motor to the driving shaft
    * @param kInverted true if the motor should run in reverse, false if not
    * @param kUpperLimit The maximum mechanical height that the elevator stage is capable of
    */
    ElevatorIOSparkMax(int busId, int id,
                       rev::spark::SparkLowLevel::MotorType motorType,
                       double kS, double kV, double kA, double kP, double kI,
                       double kD, double kG,
                       units::turns_per_second_t kCruiseVelocity,
                       units::turns_per_second_squared_t kAcceleration,
                       units::turns_per_second_cubed_t kJerk,
                       units::meter_t kSprocketCircumference, double kGearRatio,
                       bool kInverted, units::meter_t kUpperLimit);
    
    /**
    * @brief A compressed constructor using the ElevatorConstants struct
    * This constructor calls the expanded constructor with the values contained in the "constants" parameter
    * @param busId The CAN bus id
    * @param id The CAN id of the motor
    * @param motorType The selected motor type from the enum rev::spark::SparkLowLevel::MotorType
    * @param constants An ElevatorConstants struct containing all the constants corresponding to that elevator stage
    */
    ElevatorIOSparkMax(int busId, int id,
                       rev::spark::SparkLowLevel::MotorType motorType,
                       ElevatorConstants kConstants);
    
    /**
    * @brief Starts moving the motor towards the desired position using a ClosedLoopController
    * @param position The desired position for the motor to move to
    */
    void SetTargetPosition(units::meter_t position) override;
    
    /** 
    * @brief Sets the speed of the motor directly
    * @param speed The speed to move the motor at (0.0 - 1.0)
    * @param overrideLowerLimit Should only be true if the elevator is homing and the lower limit of the elevator is being reset with a functioning limit switch
    */
    void SetSpeed(double speed, bool overrideLowerLimit) override;
    
    /**
    * @brief Sets whether the lower soft limit of the motor should be enabled
    * @param enableLowerLimit true if the lower soft limit should be enabled, false if not
    */
    void SetLowerLimitEnable(bool enableLowerLimit) override;
    
    /**
    * @brief Sets the position of the elevator to zero
    * This should only be used if the elevator is at the lower hard limit, NOT the soft limit
    */
    void ResetPosition() override;

    /**
    * @brief Stops the motor
    */
    void Stop() override;

    /**
    * @brief Runs the motor at a desired velocity
    * @param voltage The voltage that should be supplied to the motor
    */
    void SetVoltage(units::volt_t) override;

    /**
    * @brief Gets the SparkMAX that was created
    */
    rev::spark::SparkMax *GetSparkMax();

    /** 
    * @brief Gets the position of the elevator
    */
    units::turn_t GetPosition() const override;

    /**
    * @brief Gets the temperature of the motor
    */
    units::celsius_t GetTemperature() const override;

    /**
    * @brief Gets the voltage being supplied to the motor
    */
    units::volt_t GetVoltage() const override;

    /**
    * @brief Gets the velocity of the driving shaft of the elevator
    */
    units::turns_per_second_t GetVelocity() const override;

    /**
    * @brief Gets the velocity of the motor's output shaft
    */
    units::turns_per_second_t GetRotorVelocity() const override;

    /**
    * @brief Gets the current being supplied to the motor
    */
    units::ampere_t GetCurrent() const override;

    /**
    * @return true if the motor is conected, false if not
    */
    bool GetIsPresent() const override;

private:
    std::shared_ptr<rev::spark::SparkMax> m_motor;
    units::turn_t m_position;
    units::celsius_t m_temperature;
    units::volt_t m_voltage;
    units::turns_per_second_t m_velocity;
    units::ampere_t m_current;
    double kG;
};

/**
* @brief A class that inherits ElevatorIOSparkMax and is used to simulate a SparkMax motor controller
*/
class ElevatorIOSparkMaxSim : public ElevatorIOSparkMax
{
public:
    /**
    * @param busId The CAN bus id
    * @param id The CAN id of the motor
    * @param motorType The selected motor type from the enum rev::spark::SparkLowLevel::MotorType
    * @param constants The ElevatorConstants corresponding to this motor
    * @param dcMotorType The type of motor being used. Many motors are already included under frc::DCMotor::[motor name]
    */
    ElevatorIOSparkMaxSim(int busId, int id,
                          rev::spark::SparkLowLevel::MotorType motorType,
                          ElevatorConstants constants,
                          frc::DCMotor dcMotorType);

    /**
    * @brief Updates the ElevatorSim with the voltages being supplied and the TalonFXSimState with the motor's position and velocity
    */
    void UpdateSim();

private:
    frc::sim::ElevatorSim m_elevatorSim;
    rev::spark::SparkMaxSim m_sparkSim;
    ElevatorConstants m_constants;
};

/**
* @brief A command to move the elevator to a position and then finish
*/
class ElevatorMoveToPositionCommand
    : public frc2::CommandHelper<frc2::Command, ElevatorMoveToPositionCommand>
{
public:
    /**
    * @param elevator A pointer to the elevator being used
    * @param position The desired position to move to
    */
    ElevatorMoveToPositionCommand(Elevator *elevator, units::meter_t position);

    /**
    * @brief Sets the target position of the motor
    * The Initialize function runs once when the command begins
    */
    void Initialize() override;

    /**
    * @brief Checks if the elevator has reached the desired position
    * The IsFinished function determines if the command should stop running
    * @return true if the position of the elevator is within the error tolerance range of the target position
    */
    bool IsFinished() override;

    /**
    * @brief Stops the elevator
    * The end function runs once when the command ends
    */
    void End(bool interrupted) override;

private:
    Elevator *m_elevator;
    units::meter_t m_position;
};

/**
* @brief A command to move the elevator to a position and hold it there
* This command is mostly called in autonomous when it can only be called once and the elevator must remain at that position until another command is called
*/
class ElevatorHoldAtPositionCommand
    : public frc2::CommandHelper<frc2::Command, ElevatorHoldAtPositionCommand>
{
public:
    /**
    * @param elevator A pointer to the elevator being used
    * @param position The position to move to
    */
    ElevatorHoldAtPositionCommand(Elevator *elevator, units::meter_t position);

    /**
    * @brief Sets the target position of the motor
    * The Initialize function runs once when the command begins
    */
    void Initialize() override;

    /**
    * @brief Normally this function would determine when the command is supposed to end, but this command does not as it needs to keep moving the elevator to the position until another command is called
    */
    bool IsFinished() override;

private:
    Elevator *m_elevator;
    units::meter_t m_position;
};

/**
* @brief A command to move the elevator to its lower hard limit and reset its position to zero
*/
class ElevatorHomingCommand
    : public frc2::CommandHelper<frc2::Command, ElevatorHomingCommand>
{
public:
    /**
    * @param elevator A pointer to the elevator being used
    * @param speed The speed to home the elevator at
    */
    ElevatorHomingCommand(Elevator *elevator, double speed);

    /**
    * @brief Disables the lower soft limit and begins moving the motor downwards
    * The Initialize function runs once when the command begins
    */
    void Initialize() override;

    /**
    * @brief Gets whether the sensor has been triggered
    * The IsFinished function determines if the command should stop running
    * @return true if the sensor has been triggered and the command should stop, false if not
    */
    bool IsFinished() override;

    /**
    * @brief Stops the motor
    * The End function runs once when the command ends
    */
    void End(bool interrupted) override;

private:
    Elevator *m_elevator;
    double m_speed;
};

/**
* @brief A command to manually control the elevator with a joystick
*/
class ElevatorManualControlCommand
    : public frc2::CommandHelper<frc2::Command, ElevatorManualControlCommand>
{
public:
    /**
    * @param elevator A pointer to the elevator being used
    * @param speed A function that returns the speed value to move the elevator at
    */
    ElevatorManualControlCommand(Elevator *elevator,
                                 std::function<double()> speed);
    
    /**
    * @brief Sets the speed of the motor to the return value of the speed() function
    * The Execute function runs every update until the command ends
    */
    void Execute() override;

private:
    Elevator *m_elevator;
    std::function<double()> m_speed;
};