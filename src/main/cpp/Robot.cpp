// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

#include <frc/DriverStation.h>
#include <frc2/command/CommandScheduler.h>

#include "logging/LogTypes.h"
#include "logging/Logger.h"
#include "logging/NTLogManager.h"
#include "logging/WPILogManager.h"
#include "util/GitMetadataLoader.h"

bool isCompetition()
{
    return frc::DriverStation::IsFMSAttached();
}

Robot::Robot()
{
    nfr::logger.AddOutput(std::make_shared<nfr::WPILogManager>());
    if (!isCompetition())
    {
        nfr::logger.AddOutput(std::make_shared<nfr::NTLogManager>());
    }
    nfr::logger["git"] << getGitMetadata();
}

void Robot::RobotPeriodic()
{
    frc2::CommandScheduler::GetInstance().Run();
    m_container.Periodic();
    nfr::logger["robot"] << m_container;
    nfr::logger.Flush();
}

void Robot::DisabledInit()
{
}

void Robot::DisabledPeriodic()
{
}

void Robot::DisabledExit()
{
}

void Robot::AutonomousInit()
{
    m_autonomousCommand = m_container.GetAutonomousCommand();

    if (m_autonomousCommand)
    {
        m_autonomousCommand->Schedule();
    }
}

void Robot::AutonomousPeriodic()
{
}

void Robot::AutonomousExit()
{
}

void Robot::TeleopInit()
{
    if (m_autonomousCommand)
    {
        m_autonomousCommand->Cancel();
    }
}

void Robot::TeleopPeriodic()
{
}

void Robot::TeleopExit()
{
}

void Robot::TestInit()
{
    frc2::CommandScheduler::GetInstance().CancelAll();
}

void Robot::TestPeriodic()
{
}

void Robot::TestExit()
{
}

#ifndef RUNNING_FRC_TESTS
int main()
{
    return frc::StartRobot<Robot>();
}
#endif
