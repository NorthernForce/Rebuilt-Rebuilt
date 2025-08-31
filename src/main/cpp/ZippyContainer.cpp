// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "ZippyContainer.h"

#include <frc/DriverStation.h>
#include <frc2/command/Commands.h>
#include <frc2/command/button/CommandXboxController.h>

#include "constants/Constants.h"
#include "frc/geometry/Pose3d.h"
#include "frc/smartdashboard/SmartDashboard.h"
#include "generated/TunerConstants.h"
#include "subsystems/drive/SwerveUtils.h"
#include "units/base.h"

using namespace std;
using namespace nfr;

ZippyContainer::ZippyContainer()
{
    // Create our swerve drivetrain with all its configuration
    // This big constructor call sets up:
    // - Hardware configuration (motor controllers, encoders)
    // - Update rate (how often to calculate new motor commands)
    // - Odometry settings (how accurately we track robot position)
    // - PID controllers for autonomous path following
    // - Maximum speeds for safety
    m_drive = std::make_unique<SwerveDrive>(
        TunerConstants::DrivetrainConstants, DriveConstants::kUpdateRate,
        DriveConstants::kOdometryStandardDeviation,
        DriveConstants::kVisionStandardDeviation,
        DriveConstants::kTranslationPID, DriveConstants::kRotationPID,
        DriveConstants::kMaxTranslationSpeed, DriveConstants::kMaxRotationSpeed,
        TunerConstants::FrontLeft, TunerConstants::FrontRight,
        TunerConstants::BackLeft, TunerConstants::BackRight);

    // Load saved swerve module offsets from previous calibration
    m_drive->SetModuleOffsets(getModuleOffsets());

    // Set up controller bindings and default commands
    ConfigureBindings();
}

void ZippyContainer::ConfigureBindings()
{
    m_drive->SetDefaultCommand(m_drive->DriveByJoystick(
        ProcessInput([&]() { return m_driverController.GetLeftX(); }),
        ProcessInput([&]() { return m_driverController.GetLeftY(); }),
        ProcessInput([&]() { return m_driverController.GetRightX(); }),
        true  // Field-centric driving
        ));
    m_driverController.Back().OnTrue(
        m_drive->RunOnce([&]() { m_drive->SeedFieldCentric(); }));
    m_resetModulesCommand = m_drive->RunOnce(
        [&]()
        {
            auto offsets =
                m_drive->ResetModuleOffsets({0_deg, 0_deg, 0_deg, 0_deg});
            SetModuleOffsets(offsets);
        });
    frc::SmartDashboard::PutData("Reset Swerve Modules",
                                 m_resetModulesCommand.value().get());
}

frc2::CommandPtr ZippyContainer::GetAutonomousCommand()
{
    return frc2::cmd::Print("No autonomous command configured");
}

void ZippyContainer::RobotInit()
{
}
void ZippyContainer::RobotPeriodic()
{
}
void ZippyContainer::DisabledInit()
{
}
void ZippyContainer::DisabledPeriodic()
{
}
void ZippyContainer::DisabledExit()
{
}
void ZippyContainer::AutonomousInit()
{
}
void ZippyContainer::AutonomousPeriodic()
{
}
void ZippyContainer::AutonomousExit()
{
}
void ZippyContainer::TeleopInit()
{
}
void ZippyContainer::TeleopPeriodic()
{
}
void ZippyContainer::TeleopExit()
{
}
void ZippyContainer::TestInit()
{
}
void ZippyContainer::TestPeriodic()
{
}
void ZippyContainer::TestExit()
{
}
void ZippyContainer::Log(const nfr::LogContext& log) const
{
    // Log important robot data for debugging and analysis
    // Match time helps correlate log data with what happened during the match
    log["match_time"] << frc::DriverStation::GetMatchTime();

    // Log drivetrain state (position, velocity, motor currents, etc.)
    // The drivetrain Log method will record detailed state information
    log["drive"] << m_drive;
}
void ZippyContainer::LogRobotState(const nfr::LogContext& log) const
{
    // Get current robot pose for the base robot component
    frc::Pose3d robotPose = frc::Pose3d(m_drive->GetState().Pose);

    // Main robot pose for AdvantageScope 3D visualization
    log["Robot"] << robotPose;

    // Component 0: Swerve drive modules - base robot frame
    // This represents the main chassis/drivetrain
    log["component_0"] << robotPose;

    // Component 1: Robot base frame - secondary base component
    // Position based on config.json zeroedPosition for component 1
    frc::Pose3d basePose =
        robotPose +
        frc::Transform3d(frc::Translation3d(-1.52_m, -0.4_m, -0.02_m),
                         frc::Rotation3d(0_deg, 0_deg, 90_deg));
    log["component_1"] << basePose;

    // Additional robot state information for debugging
    log["chassis_speeds"] << m_drive->GetState().Speeds;
    log["field_relative_heading"]
        << m_drive->GetState().Pose.Rotation().Degrees();
}
