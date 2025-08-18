// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>
#include <logging/Logger.h>

/**
 * @brief Container class that organizes all robot subsystems and controller
 * bindings
 *
 * This class is like the "command center" of the robot. It:
 * - Creates and manages all subsystems (drivetrain, arms, shooters, etc.)
 * - Sets up which controller buttons trigger which actions
 * - Provides the autonomous command for the robot to run
 * - Handles logging of robot state
 *
 * In the Command-Based programming pattern, this container keeps everything
 * organized and connected. Think of it as the main control panel that connects
 * driver inputs to robot actions.
 */
class NFRRobotContainer
{
  public:
    /**
     * @brief Constructor - creates subsystems and sets up controller bindings
     *
     * This runs once when the robot starts up. It creates our swerve drivetrain
     * and configures which buttons on the controller do what actions.
     */
    NFRRobotContainer();

    virtual ~NFRRobotContainer() = default;

    virtual void RobotInit() = 0;
    virtual void RobotPeriodic() = 0;
    virtual void DisabledInit() = 0;
    virtual void DisabledPeriodic() = 0;
    virtual void DisabledExit() = 0;
    virtual void AutonomousInit() = 0;
    virtual void AutonomousPeriodic() = 0;
    virtual void AutonomousExit() = 0;
    virtual void TeleopInit() = 0;
    virtual void TeleopPeriodic() = 0;
    virtual void TeleopExit() = 0;
    virtual void TestInit() = 0;
    virtual void TestPeriodic() = 0;
    virtual void TestExit() = 0;

    /**
     * @brief Gets the command to run during autonomous period
     *
     * This returns the autonomous command that should run during the 15-second
     * autonomous period at the start of each match. Currently returns a simple
     * "do nothing" command, but would normally return a complex autonomous
     * routine.
     *
     * @return CommandPtr to run during autonomous
     */
    virtual frc2::CommandPtr GetAutonomousCommand() = 0;

    /**
     * @brief Logs current robot state for debugging and analysis
     *
     * This method is called every 20ms to record important robot data like
     * drivetrain position, motor temperatures, etc. This data helps us debug
     * problems and analyze robot performance.
     *
     * @param log The logging context to write data to
     */
    virtual void Log(const nfr::LogContext &log) const;

private:
    /**
     * @brief Sets up controller button bindings and default commands
     *
     * This method configures:
     * - Which joystick axes control driving
     * - Which buttons trigger specific actions
     * - Default commands that run when no other commands are active
     */
    virtual void ConfigureBindings() = 0;
};
