#pragma once
#include <frc/RobotController.h>
#include <frc2/command/Command.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/ParallelCommandGroup.h>
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/sysid/SysIdRoutine.h>
#include <subsystems/superstructure/elevator/Elevator.h>
#include <subsystems/superstructure/elevator/ElevatorSensor.h>
#include <units/math.h>

#include <ctre/phoenix6/TalonFX.hpp>
#include <memory>

#include "constants/Constants.h"

class Superstructure : public frc2::SubsystemBase
{
public:

    /**
    * @brief A loggable struct consisting of two length values for the heights of the inner and outer elevator stages
    */
    struct SuperstructureState
    {
    public:
        units::meter_t innerElevatorPosition;
        units::meter_t outerElevatorPosition;

        /**
        * @brief Implementation of the universal logging function
        */
        void Log(const nfr::LogContext& log) const;
    };

    /**
    * @brief A class to control two stages of an elevator
    * @param innerElevator Shared pointer to ElevatorIO for the inner elevator
    * @param outerElevator Shared pointer to ElevatorIO for the outer elevator
    */
    Superstructure(std::shared_ptr<Elevator> innerElevator,
                   std::shared_ptr<Elevator> outerElevator);

    /**
    * @brief Stops the superstructure
    */
    void Stop();

    /**
    * @brief Sets the target for the superstructure. 
    * Note: This does not actually move the superstructure and is only used for logging. To move the superstructure, use the GoTo- or HoldAt- Position commands
    * @param target The target SuperstructureState
    */
    void SetTarget(SuperstructureState target);
    
    /**
    * @return The current state of the superstructure
    */
    SuperstructureState GetState() const;

    /**
    * @return The target state of the superstructure
    */
    SuperstructureState GetTargetState() const;

    /**
    * @brief Converts a SuperstructurePreset to a SuperstructureState with the correct elevator heights
    * @param preset The SuperstructurePreset to be converted
    * @return The corresponding SuperstructureState
    */
    SuperstructureState GetPresetState(
        UniversalElevatorConstants::SuperstructurePresets preset) const;

    /**
    * @return True if the superstructure is at its target position
    */
    bool IsAtTarget() const;

    /**
    * @param position The position to check
    * @return True if the superstructure is at the supplied position
    */
    bool IsAtPosition(SuperstructureState position) const;

    /**
    * @return The inner elevator stage
    */
    Elevator* GetInnerElevator();

    /**
    * @return The outer elevator stage
    */
    Elevator* GetOuterElevator();

    /**
    * @brief Implementation of the universal logging function
    */
    void Log(const nfr::LogContext& log) const;

    /**
    * @brief Gets a GoToPosition command for the superstructure
    * This command ends as soon as the superstructure reaches the desired position
    * @param position The desired SuperstructureState
    * @return A GoToPosition command
    */
    frc2::CommandPtr GetGoToPositionCommand(SuperstructureState position);

    /**
    * @brief Gets a HoldAtPosition command for the superstructure
    * This command does not end
    * @param position The desired SuperstructureState
    * @return A HoldAtPosition command
    */
    frc2::CommandPtr GetHoldAtPositionCommand(SuperstructureState position);

    /**
    * @brief Gets a command to home the superstructure
    * This command is a parallel command group consisting of two elevator homing commands 
    * @param innerElevatorSpeed The homing speed for the inner elevator
    * @param outerElevatorSpeed The homing speed for the outer elevator
    * @return A homing command for the superstructure 
    */
    frc2::CommandPtr GetHomingCommand(double innerElevatorSpeed,
                                      double outerElevatorSpeed);

    /**
    * @brief Gets a command to control the superstructure with a joystick
    * This command is a parallel command group consisting of two elevator manual control commands
    * @param innerElevatorSpeed A function that returns a speed value to run the inner elevator at
    * @param outerElevatorSpeed A function that returns a speed value to run the outer elevator at
    */
    frc2::CommandPtr GetManualControlCommand(
        std::function<double()> innerElevatorSpeed,
        std::function<double()> outerElevatorSpeed);

private:
    std::shared_ptr<Elevator> m_innerElevator;
    std::shared_ptr<Elevator> m_outerElevator;
    SuperstructureState m_target;
};

/**
* @brief A ParallelCommandGroup consisting of two ElevatorMoveToPositionCommands
*/
class SuperstructureMoveToPositionCommand
    : public frc2::CommandHelper<frc2::ParallelCommandGroup,
                                 SuperstructureMoveToPositionCommand>
{
public:
    /**
    * @param superstructure A pointer to the superstructure being used
    * @param position The target SuperstructureState
    */
    SuperstructureMoveToPositionCommand(
        Superstructure* superstructure,
        Superstructure::SuperstructureState position);

private:
    Superstructure* m_superstructure;
    Superstructure::SuperstructureState m_position;
};

/**
* @brief A ParallelCommandGroup consisting of two ElevatorHoldAtPositionCommands
*/
class SuperstructureHoldAtPositionCommand
    : public frc2::CommandHelper<frc2::ParallelCommandGroup,
                                 SuperstructureHoldAtPositionCommand>
{
public:
    /**
    * @param superstructure A pointer to the superstructure being used
    * @param position The target SuperstructureState
    */
    SuperstructureHoldAtPositionCommand(
        Superstructure* superstructure,
        Superstructure::SuperstructureState position);

private:
    Superstructure* m_superstructure;
    Superstructure::SuperstructureState m_position;
};

/**
* @brief A ParallelCommandGroup consisting of two ElevatorHomingCommands
*/
class SuperstructureHomingCommand
    : public frc2::CommandHelper<frc2::ParallelCommandGroup,
                                 SuperstructureHomingCommand>
{
public:
    /**
    * @param superstructure A pointer to the superstructure being used
    * @param innerElevatorSpeed The speed to home the inner elevator at
    * @param outerElevatorSpeed The speed to home the outer elevator at
    */
    SuperstructureHomingCommand(Superstructure* superstructure,
                                double innerElevatorSpeed,
                                double outerElevatorSpeed);

private:
    Superstructure* m_superstructure;
};

/**
* @brief A ParallelCommandGroup consisting of two ElevatorManualControlCommands
*/
class SuperstructureManualControlCommand
    : public frc2::CommandHelper<frc2::ParallelCommandGroup,
                                 SuperstructureManualControlCommand>
{
public:
    /**
    * @param superstructure A pointer to the superstructure being used
    * @param innerElevatorSpeed A function that returns the speed value for the inner elevator
    * @param outerElevatorSpeed A function that returns the speed value for the outer elevator
    */
    SuperstructureManualControlCommand(
        Superstructure* superstructure,
        std::function<double()> innerElevatorSpeed,
        std::function<double()> outerElevatorSpeed);

private:
    Superstructure* m_superstructure;
};