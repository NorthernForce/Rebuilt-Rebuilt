#pragma once
#include <ctre/phoenix6/TalonFX.hpp>
#include <frc/RobotController.h>
#include <frc2/command/Command.h>
#include <frc2/command/ParallelCommandGroup.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/sysid/SysIdRoutine.h>
#include <memory>
#include <string>
#include <subsystems/superstructure/elevator/ElevatorSensor.h>
#include <units/math.h>
#include <subsystems/superstructure/elevator/Elevator.h>
#include "RobotConstants.h"

using namespace std;
using namespace units;
using namespace frc2;

class Superstructure: public frc2::SubsystemBase {
    public:
    struct SuperstructureState {
        meter_t innerElevatorPosition;
        meter_t outerElevatorPosition;
    };
    Superstructure(Elevator* innerElevator, Elevator* outerElevator);
    void Stop();
    void SetTarget(SuperstructureState target);
    SuperstructureState GetState();
    SuperstructureState GetTargetState();
    SuperstructureState GetPresetState(RobotConstants::ElevatorConstants::SuperstructurePresets preset);
    bool IsAtTarget();
    bool IsAtPosition(SuperstructureState position);
    Elevator* GetInnerElevator();
    Elevator* GetOuterElevator();
    CommandPtr GetGoToPositionCommand(SuperstructureState position);
    CommandPtr GetHoldAtPositionCommand(SuperstructureState position);
    CommandPtr GetHomingCommand(double innerElevatorSpeed, double outerElevatorSpeed);
    CommandPtr GetManualControlCommand(double* innerElevatorSpeed, double* outerElevatorSpeed);
    private:
    Elevator* m_innerElevator;
    Elevator* m_outerElevator;
    SuperstructureState m_target;
};

class SuperstructureGoToPositionCommand: public CommandHelper<Command, SuperstructureGoToPositionCommand>
{
    public:
    SuperstructureGoToPositionCommand(Superstructure* superstructure, Superstructure::SuperstructureState position);
    void Initialize();
    bool IsFinished();
    private:
    Superstructure* m_superstructure;
    Superstructure::SuperstructureState m_position;
};

class SuperstructureHoldAtPositionCommand: public CommandHelper<Command, SuperstructureHoldAtPositionCommand>
{
    public:
    SuperstructureHoldAtPositionCommand(Superstructure* superstructure, Superstructure::SuperstructureState position);
    void Initialize();
    bool IsFinished();
    private:
    Superstructure* m_superstructure;
    Superstructure::SuperstructureState m_position;
};

class SuperstructureHomingCommand: public CommandHelper<ParallelCommandGroup, SuperstructureHomingCommand>
{
    public:
    SuperstructureHomingCommand(Superstructure* superstructure, double innerElevatorSpeed, double outerElevatorSpeed);
    private:
    Superstructure* m_superstructure;
};

class SuperstructureManualControlCommand: public CommandHelper<ParallelCommandGroup, SuperstructureManualControlCommand>
{
    public:
    SuperstructureManualControlCommand(Superstructure* superstructure, double* innerElevatorSpeed, double* outerElevatorSpeed);
    private:
    Superstructure* m_superstructure;
};