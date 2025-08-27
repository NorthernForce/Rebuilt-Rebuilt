// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

#include "RalphContainer.h"
#include "util/NFRRobotChooser.h"

using namespace std;
#include <frc/DriverStation.h>
#include <frc2/command/CommandScheduler.h>

#include <iostream>

#include "logging/Logger.h"
#include "util/GitMetadataLoader.h"

/**
 * @brief Checks if robot is connected to competition Field Management System
 * (FMS)
 *
 * At competitions, robots connect to the FMS which controls match timing and
 * robot enable/disable. This function helps us change logging behavior between
 * practice (more debug info) and competition (less network traffic).
 *
 * @return true if connected to FMS (at competition), false if practicing
 */
bool isCompetition()
{
    return frc::DriverStation::IsFMSAttached();
}

Robot::Robot()
{
    nfr::logger.EnableWPILogging();
    if (!isCompetition())
    {
        nfr::logger.EnableNTLogging();
        std::cout << "Running in non-competition mode. Enabling NT logging."
                  << std::endl;
    }
    else
    {
        std::cout << "Running in competition mode. No NT logging enabled."
                  << std::endl;
    }

    // Log information about which version of our code is running
    // This helps us know exactly what code was deployed to the robot
    nfr::logger["git"] << getGitMetadata();
}

void Robot::RobotInit()
{
    map<string, shared_ptr<NFRRobotContainer>> robots = {
        {"023C3578", make_shared<RalphContainer>()}};
    NFRRobotChooser chooser =
        NFRRobotChooser(make_shared<RalphContainer>(), robots);
    m_container = chooser.GetNFRRobotContainer();
}

void Robot::RobotPeriodic()
{
    // Run the command scheduler - this manages all active commands
    // Commands are like "drive forward", "shoot ball", etc.
    // The scheduler makes sure they run properly and don't conflict
    frc2::CommandScheduler::GetInstance().Run();
    m_container->RobotPeriodic();

    // Log current robot state for debugging and analysis
    // This includes drivetrain position, sensor values, etc.
    nfr::logger["robot"] << m_container;

    // Actually write all pending log data
    // Logs are buffered for performance, this forces them to be written
    nfr::logger.Flush();
}

void Robot::DisabledInit()
{
    m_container->DisabledInit();
    // Robot just entered disabled mode - currently nothing special to do
    // This is where you'd add code that should run once when disabling
}

void Robot::DisabledPeriodic()
{
    m_container->DisabledPeriodic();
    // Robot is disabled - currently nothing special to do
    // This runs every 20ms while disabled
    // You might use this to display diagnostic info on dashboard
}

void Robot::DisabledExit()
{
    m_container->DisabledExit();
    // Robot is about to leave disabled mode - currently nothing special to do
    // This is where you'd add code to prepare for enabling
}

void Robot::AutonomousInit()
{
    // Get the autonomous command from our container
    // This is the pre-programmed sequence the robot should follow
    // during the 15-second autonomous period
    m_autonomousCommand = m_container->GetAutonomousCommand();

    // If we have an autonomous command, start it running
    if (m_autonomousCommand)
    {
        m_autonomousCommand->Schedule();
    }

    m_container->AutonomousInit();
    // Note: If no autonomous command is set, robot will just sit still
    // This is safer than having a default that might not work
}

void Robot::AutonomousPeriodic()
{
    m_container->AutonomousPeriodic();
    // Autonomous periodic - currently nothing special to do
    // The autonomous command is running automatically via the scheduler
    // in RobotPeriodic(), so we don't need to do anything here
}

void Robot::AutonomousExit()
{
    m_container->AutonomousExit();
    // Autonomous period is ending - currently nothing special to do
    // The autonomous command will be cancelled automatically in TeleopInit()
}

void Robot::TeleopInit()
{
    // Cancel any autonomous command that might still be running
    // This ensures we have full control during teleop period
    if (m_autonomousCommand)
    {
        m_autonomousCommand->Cancel();
    }
    m_container->TeleopInit();
    // After this, the default commands (like joystick driving) will take over
}

void Robot::TeleopPeriodic()
{
    m_container->TeleopPeriodic();
    // Teleop periodic - currently nothing special to do
    // Driver controls are handled automatically by the command scheduler
    // All the driving, button actions, etc. are set up in RobotContainer
}

void Robot::TeleopExit()
{
    m_container->TeleopExit();
    // Teleop period is ending - currently nothing special to do
    // This might be used to save settings or prepare for next mode
}

void Robot::TestInit()
{
    // Cancel all commands for safety during test mode
    // Test mode should only run specific, controlled tests
    frc2::CommandScheduler::GetInstance().CancelAll();
    m_container->TestInit();
}

void Robot::TestPeriodic()
{
    m_container->TestPeriodic();
    // Test periodic - currently nothing special to do
    // In test mode, you might run specific tests like:
    // - Individual motor tests
    // - Sensor calibration
    // - System identification (SysID) for tuning PID controllers
}

void Robot::TestExit()
{
    m_container->TestExit();
    // Test mode is ending - currently nothing special to do
    // This is where you might save test results or reset systems
}

// This is the main entry point for our robot program
// The RUNNING_FRC_TESTS check excludes this when running unit tests
#ifndef RUNNING_FRC_TESTS
int main()
{
    // Start the robot with our Robot class
    // This hands control over to WPILib, which will create a Robot instance
    // and call its methods based on driver station input
    return frc::StartRobot<Robot>();
}
#endif
