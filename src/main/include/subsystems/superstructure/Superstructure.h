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
#include <string>

#include "constants/Constants.h"

class Superstructure : public frc2::SubsystemBase
{
  public:
    struct SuperstructureState
    {
        units::meter_t innerElevatorPosition;
        units::meter_t outerElevatorPosition;
    };
    Superstructure(std::shared_ptr<Elevator> innerElevator,
                   std::shared_ptr<Elevator> outerElevator);
    void Stop();
    void SetTarget(SuperstructureState target);
    SuperstructureState GetState();
    SuperstructureState GetTargetState();
    SuperstructureState GetPresetState(
        ElevatorConstants::SuperstructurePresets preset);
    bool IsAtTarget();
    bool IsAtPosition(SuperstructureState position);
    Elevator* GetInnerElevator();
    Elevator* GetOuterElevator();
    frc2::CommandPtr GetGoToPositionCommand(SuperstructureState position);
    frc2::CommandPtr GetHoldAtPositionCommand(SuperstructureState position);
    frc2::CommandPtr GetHomingCommand(double innerElevatorSpeed,
                                      double outerElevatorSpeed);
    frc2::CommandPtr GetManualControlCommand(
        std::function<double()> innerElevatorSpeed,
        std::function<double()> outerElevatorSpeed);

  private:
    std::shared_ptr<Elevator> m_innerElevator;
    std::shared_ptr<Elevator> m_outerElevator;
    SuperstructureState m_target;
};

class SuperstructureGoToPositionCommand
    : public frc2::CommandHelper<frc2::Command,
                                 SuperstructureGoToPositionCommand>
{
  public:
    SuperstructureGoToPositionCommand(
        Superstructure* superstructure,
        Superstructure::SuperstructureState position);
    void Initialize();
    bool IsFinished();

  private:
    Superstructure* m_superstructure;
    Superstructure::SuperstructureState m_position;
};

class SuperstructureHoldAtPositionCommand
    : public frc2::CommandHelper<frc2::Command,
                                 SuperstructureHoldAtPositionCommand>
{
  public:
    SuperstructureHoldAtPositionCommand(
        Superstructure* superstructure,
        Superstructure::SuperstructureState position);
    void Initialize();
    bool IsFinished();

  private:
    Superstructure* m_superstructure;
    Superstructure::SuperstructureState m_position;
};

class SuperstructureHomingCommand
    : public frc2::CommandHelper<frc2::ParallelCommandGroup,
                                 SuperstructureHomingCommand>
{
  public:
    SuperstructureHomingCommand(Superstructure* superstructure,
                                double innerElevatorSpeed,
                                double outerElevatorSpeed);

  private:
    Superstructure* m_superstructure;
};

class SuperstructureManualControlCommand
    : public frc2::CommandHelper<frc2::ParallelCommandGroup,
                                 SuperstructureManualControlCommand>
{
  public:
    SuperstructureManualControlCommand(
        Superstructure* superstructure,
        std::function<double()> innerElevatorSpeed,
        std::function<double()> outerElevatorSpeed);

  private:
    Superstructure* m_superstructure;
};