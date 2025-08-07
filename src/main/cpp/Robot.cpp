// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

using namespace std;

Robot::Robot()
{
}

void Robot::RobotInit()
{
    map<string, shared_ptr<NFRRobotContainer>> robots = { {"023C3578", make_shared<RalphContainer>()} };
    NFRRobotChooser chooser = NFRRobotChooser(make_shared<RalphContainer>(), robots);
    m_container = chooser.GetNFRRobotContainer();
}

void Robot::RobotPeriodic()
{
    frc2::CommandScheduler::GetInstance().Run();
    m_container->RobotPeriodic();
}

void Robot::DisabledInit()
{
    m_container->DisabledInit();
}

void Robot::DisabledPeriodic()
{
    m_container->DisabledPeriodic();
}

void Robot::DisabledExit()
{
    m_container->DisabledExit();
}

void Robot::AutonomousInit()
{
    m_container->AutonomousInit();
    m_autonomousCommand = m_container->GetAutonomousCommand();

    if (m_autonomousCommand)
    {
        m_autonomousCommand->Schedule();
    }
}

void Robot::AutonomousPeriodic()
{
    m_container->AutonomousPeriodic();
}

void Robot::AutonomousExit()
{
    m_container->AutonomousExit();
}

void Robot::TeleopInit()
{
    m_container->TeleopInit();
    if (m_autonomousCommand)
    {
        m_autonomousCommand->Cancel();
    }
}

void Robot::TeleopPeriodic()
{
    m_container->TeleopPeriodic();
}

void Robot::TeleopExit()
{
    m_container->TeleopExit();
}

void Robot::TestInit()
{
    m_container->TestInit();
    frc2::CommandScheduler::GetInstance().CancelAll();
}

void Robot::TestPeriodic()
{
    m_container->TestPeriodic();
}

void Robot::TestExit()
{
    m_container->TestExit();
}

#ifndef RUNNING_FRC_TESTS
int main()
{
    return frc::StartRobot<Robot>();
}
#endif
