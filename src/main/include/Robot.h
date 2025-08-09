// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include <frc2/command/CommandPtr.h>

#include <optional>

#include "RobotContainer.h"

/**
 * @brief Main robot class that manages all robot operations
 * 
 * This class inherits from TimedRobot, which automatically calls different methods
 * based on what the robot is doing. In FRC, robots have several "modes":
 * 
 * - **Disabled**: Robot is powered but motors/actuators are disabled for safety
 * - **Autonomous**: Robot runs by itself for 15 seconds at start of match
 * - **Teleop**: Human drivers control the robot (2 minutes 15 seconds)
 * - **Test**: Special mode for testing individual systems safely
 * 
 * Each mode has three phases: Init (runs once when entering), Periodic (runs every
 * 20ms while in that mode), and Exit (runs once when leaving that mode).
 * 
 * @note This follows WPILib's TimedRobot pattern - you don't call these methods
 * directly, the robot framework calls them automatically based on driver station input.
 */
class Robot : public frc::TimedRobot
{
public:
    /**
     * @brief Constructor - sets up logging and initializes robot
     * 
     * This runs once when the robot code starts up. It sets up our custom
     * logging system to help debug problems during matches and testing.
     */
    Robot();

    /**
     * @brief Runs continuously every 20ms regardless of robot mode
     * 
     * This is where we put code that should ALWAYS run, like:
     * - Updating our command scheduler (manages all robot commands)
     * - Logging robot data for debugging
     * - Updating dashboard/SmartDashboard values
     */
    void RobotPeriodic() override;

    // === DISABLED MODE METHODS ===
    // These run when the robot is powered but disabled (safest state)
    
    /** @brief Runs once when robot enters disabled mode */
    void DisabledInit() override;
    /** @brief Runs every 20ms while robot is disabled */
    void DisabledPeriodic() override;
    /** @brief Runs once when robot exits disabled mode */
    void DisabledExit() override;

    // === AUTONOMOUS MODE METHODS ===
    // These run during the 15-second autonomous period at start of match
    
    /**
     * @brief Runs once when autonomous period starts
     * 
     * This is where we start our autonomous command (the pre-programmed
     * sequence the robot should follow during autonomous).
     */
    void AutonomousInit() override;
    /** @brief Runs every 20ms during autonomous period */
    void AutonomousPeriodic() override;
    /** @brief Runs once when autonomous period ends */
    void AutonomousExit() override;

    // === TELEOP MODE METHODS ===
    // These run when human drivers control the robot
    
    /**
     * @brief Runs once when teleop (driver control) period starts
     * 
     * This cancels any autonomous commands that might still be running
     * and prepares for human driver control.
     */
    void TeleopInit() override;
    /** @brief Runs every 20ms during teleop period */
    void TeleopPeriodic() override;
    /** @brief Runs once when teleop period ends */
    void TeleopExit() override;

    // === TEST MODE METHODS ===
    // These run in test mode for safely testing individual systems
    
    /**
     * @brief Runs once when test mode starts
     * 
     * Cancels all commands for safety - test mode should only run
     * specific, controlled tests.
     */
    void TestInit() override;
    /** @brief Runs every 20ms during test mode */
    void TestPeriodic() override;
    /** @brief Runs once when test mode ends */
    void TestExit() override;

private:
    /**
     * @brief Stores the autonomous command while it's running
     * 
     * std::optional means this might or might not contain a value.
     * During autonomous, this holds our autonomous command so we can
     * cancel it later if needed. Outside autonomous, it's empty.
     */
    std::optional<frc2::CommandPtr> m_autonomousCommand;

    /**
     * @brief Container that holds all our subsystems and sets up controls
     * 
     * This object contains our drivetrain, any other subsystems (like arms,
     * shooters, etc.), and sets up which controller buttons do what.
     * Think of it as the "brain" that connects everything together.
     */
    RobotContainer m_container;
};
