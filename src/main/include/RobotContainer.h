// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>
#include <logging/Logger.h>
#include <subsystems/drive/SwerveDrive.h>
#ifdef PI
#undef PI
#endif
#include "subsystems/superstructure/Superstructure.h"

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
class RobotContainer
{
public:
    /**
     * @brief Constructor - creates subsystems and sets up controller bindings
     *
     * This runs once when the robot starts up. It creates our swerve drivetrain
     * and configures which buttons on the controller do what actions.
     */
    RobotContainer();

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
    frc2::CommandPtr GetAutonomousCommand();

    /**
     * @brief Logs current robot state for debugging and analysis
     *
     * This method is called every 20ms to record important robot data like
     * drivetrain position, motor temperatures, etc. This data helps us debug
     * problems and analyze robot performance.
     *
     * @param log The logging context to write data to
     */
    void Log(const nfr::LogContext &log) const;

private:
    /**
     * @brief Sets up controller button bindings and default commands
     *
     * This method configures:
     * - Which joystick axes control driving
     * - Which buttons trigger specific actions
     * - Default commands that run when no other commands are active
     */
    void ConfigureBindings();

    /**
     * @brief Our robot's swerve drivetrain subsystem
     *
     * Swerve drive allows the robot to move in any direction instantly without
     * turning first. Each wheel can rotate and steer independently, giving
     * incredible maneuverability compared to traditional tank drive.
     *
     * Uses std::unique_ptr for automatic memory management - the pointer owns
     * the SwerveDrive object and will automatically delete it when destroyed.
     */
    std::unique_ptr<nfr::SwerveDrive> drive{nullptr};

    /**
     * @brief Command to reset swerve module positions
     *
     * Swerve modules need periodic calibration to know their absolute position.
     * This command resets their encoders to known good positions.
     * std::optional means this may or may not contain a value.
     */
    std::optional<frc2::CommandPtr> resetModulesCommand;

    /**
     * @brief Xbox controller for driver input
     *
     * CommandXboxController integrates with the command system, allowing us to
     * easily bind buttons to commands. The number (0) indicates this is the
     * first controller connected to the driver station.
     */
    frc2::CommandXboxController driverController{0};

    /**
     * @brief Xbox controller for manipulator input
     * 
     * CommandXboxController integrates with the command system, allowing us to
     * easily bind buttons to commands. The number (1) indicates this is the
     * second controller connected to the driver station.
     */
    frc2::CommandXboxController manipulatorController{0};
    
    /**
     * @brief Our robot's superstructure subsystem
     *
     * This superstructure consists of two elevator stages that can be individually
     * controlled, allowing us to elevate the game pieces to any level we want.
     */
    Superstructure m_superstructure;
};
