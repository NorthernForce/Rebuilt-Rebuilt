// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RalphContainer.h"

#include <frc/DriverStation.h>
#include <frc/Preferences.h>
#include <frc/RobotBase.h>
#include <frc/Timer.h>
#include <frc2/command/Commands.h>
#include <frc2/command/button/CommandXboxController.h>

#include "constants/Constants.h"
#include "frc/geometry/Pose3d.h"
#include "frc/smartdashboard/SmartDashboard.h"
#include "generated/TunerConstants.h"
#include "subsystems/apriltag/LimeLightCameraIO.h"
#include "subsystems/apriltag/PhotonVisionCameraIO.h"
#include "subsystems/apriltag/PhotonVisionCameraSimIO.h"
#include "subsystems/drive/SwerveUtils.h"
#include "units/base.h"

using namespace std;
using namespace nfr;

std::vector<CameraConfig> RalphContainer::CreateCameraConfigurations()
{
    std::vector<CameraConfig> configs;

    if (frc::RobotBase::IsReal())
    {
        // Real robot - create PhotonVision and LimeLight cameras

        // PhotonVision cameras
        configs.emplace_back(
            "FrontLeft", std::string(CameraConstants::kFrontLeftCameraName),
            CameraConstants::kFrontLeftCameraTransform,
            []()
            {
                return std::make_unique<PhotonVisionCameraIO>(
                    std::string(CameraConstants::kFrontLeftCameraName),
                    CameraConstants::kFrontLeftCameraTransform);
            });

        configs.emplace_back(
            "Center", std::string(CameraConstants::kCenterCameraName),
            CameraConstants::kCenterCameraTransform,
            []()
            {
                return std::make_unique<PhotonVisionCameraIO>(
                    std::string(CameraConstants::kCenterCameraName),
                    CameraConstants::kCenterCameraTransform);
            });

        // LimeLight cameras
        configs.emplace_back(
            "FrontRight", std::string(CameraConstants::kFrontRightCameraName),
            CameraConstants::kFrontRightCameraTransform,
            []()
            {
                return std::make_unique<LimeLightCameraIO>(
                    std::string(CameraConstants::kFrontRightCameraName),
                    CameraConstants::kFrontRightCameraTransform);
            });

        configs.emplace_back(
            "CenterBack", std::string(CameraConstants::kCenterBackCameraName),
            CameraConstants::kCenterBackCameraTransform,
            []()
            {
                return std::make_unique<LimeLightCameraIO>(
                    std::string(CameraConstants::kCenterBackCameraName),
                    CameraConstants::kCenterBackCameraTransform);
            });
    }
    else
    {
        // Simulation - use PhotonVision simulation for all cameras including
        // LimeLight
        configs.emplace_back(
            "FrontLeft-Sim", std::string(CameraConstants::kFrontLeftCameraName),
            CameraConstants::kFrontLeftCameraTransform,
            []()
            {
                return std::make_unique<PhotonVisionCameraSimIO>(
                    std::string(CameraConstants::kFrontLeftCameraName),
                    CameraConstants::kFrontLeftCameraTransform);
            });

        // Use PhotonVision simulation for LimeLight cameras too
        configs.emplace_back(
            "LimeLight-Sim",
            std::string(CameraConstants::kFrontRightCameraName),
            CameraConstants::kFrontRightCameraTransform,
            []()
            {
                return std::make_unique<PhotonVisionCameraSimIO>(
                    std::string(CameraConstants::kFrontRightCameraName),
                    CameraConstants::kFrontRightCameraTransform);
            });

        configs.emplace_back(
            "Center-Sim", std::string(CameraConstants::kCenterCameraName),
            CameraConstants::kCenterCameraTransform,
            []()
            {
                return std::make_unique<PhotonVisionCameraSimIO>(
                    std::string(CameraConstants::kCenterCameraName),
                    CameraConstants::kCenterCameraTransform);
            });
    }

    return configs;
}

RalphContainer::RalphContainer()
    : m_localizer(CreateCameraConfigurations(),
                  VisionConstants::kEstimateTimeout)
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

void RalphContainer::ConfigureBindings()
{
    // Set up the default driving command that runs whenever no other command is
    // using the drivetrain This command reads joystick inputs and translates
    // them to robot movement
    m_drive->SetDefaultCommand(m_drive->DriveByJoystick(
        ProcessInput(
            [&]() {
                return m_driverController.GetLeftX();
            }),  // Side-to-side movement (strafe)
        ProcessInput(
            [&]() {
                return m_driverController.GetLeftY();
            }),  // Forward/backward movement
        ProcessInput([&]()
                     { return m_driverController.GetRightX(); }),  // Rotation
        true  // Field-centric driving (explained below)
        ));

    // Field-centric vs Robot-centric driving:
    // - Field-centric: "Forward" always means away from our alliance wall,
    // regardless of robot orientation
    // - Robot-centric: "Forward" means the direction the robot is currently
    // facing Field-centric is much easier for drivers to control!

    // Bind controller buttons to specific actions
    // Back button: Reset the field-centric heading to current robot direction
    // This is useful if the gyroscope drifts or we need to re-orient our
    // reference frame
    m_driverController.Back().OnTrue(
        m_drive->RunOnce([&]() { m_drive->SeedFieldCentric(); }));

    // Create a command to reset swerve module offsets and put it on
    // SmartDashboard This allows drivers/programmers to recalibrate swerve
    // modules from the dashboard
    m_resetModulesCommand = m_drive->RunOnce(
        [&]()
        {
            // Reset all modules to 0 degrees and save the new offsets
            auto offsets =
                m_drive->ResetModuleOffsets({0_deg, 0_deg, 0_deg, 0_deg});
            SetModuleOffsets(offsets);
        });
    // Put this command on SmartDashboard so it can be triggered from the driver
    // station
    frc::SmartDashboard::PutData("Reset Swerve Modules",
                                 m_resetModulesCommand.value().get());
}

frc2::CommandPtr RalphContainer::GetAutonomousCommand()
{
    // TODO: Replace this with an actual autonomous command
    // For now, just print a message and do nothing
    // Real autonomous commands might:
    // - Drive to a specific position
    // - Score game pieces
    // - Follow a pre-planned path
    // - Use vision to align with targets
    return frc2::cmd::Print("No autonomous command configured");
}

void RalphContainer::RobotInit()
{
}

void RalphContainer::RobotPeriodic()
{
    // Update localizer with current robot pose for reference
    m_localizer.UpdateWithReferencePose(m_drive->GetState().Pose);

    // Add vision measurements to drivetrain pose estimator
    const auto& estimatedPoses = m_localizer.GetEstimatedPoses();
    for (const auto& estimatedPose : estimatedPoses)
    {
        // Only use recent estimates to avoid stale data
        // Check that the estimate is from this periodic cycle (very recent)
        units::second_t currentTime = frc::Timer::GetFPGATimestamp();
        units::second_t estimateAge = currentTime - estimatedPose.timestamp;

        if (estimateAge < VisionConstants::kMaxEstimateAge)
        {
            m_drive->AddVisionMeasurement(estimatedPose.pose,
                                          estimatedPose.timestamp);
        }
    }
}

void RalphContainer::DisabledInit()
{
}
void RalphContainer::DisabledPeriodic()
{
}
void RalphContainer::DisabledExit()
{
}
void RalphContainer::AutonomousInit()
{
}
void RalphContainer::AutonomousPeriodic()
{
}
void RalphContainer::AutonomousExit()
{
}
void RalphContainer::TeleopInit()
{
}
void RalphContainer::TeleopPeriodic()
{
}
void RalphContainer::TeleopExit()
{
}
void RalphContainer::TestInit()
{
}
void RalphContainer::TestPeriodic()
{
}
void RalphContainer::TestExit()
{
}
void RalphContainer::Log(const nfr::LogContext& log) const
{
    // Log important robot data for debugging and analysis
    // Match time helps correlate log data with what happened during the match
    log["match_time"] << frc::DriverStation::GetMatchTime();

    // Log drivetrain state (position, velocity, motor currents, etc.)
    // The drivetrain Log method will record detailed state information
    log["drive"] << m_drive;
    log["localizer"] << m_localizer;

    // AdvantageScope 3D robot visualization
    // Based on config.json components in advantageScopeAssets/Robot_Ralph/
    LogRobotState(log["Robot3d"]);
}

void RalphContainer::LogRobotState(const nfr::LogContext& log) const
{
    // Get current robot pose for the base robot component
    frc::Pose3d robotPose = frc::Pose3d(m_drive->GetState().Pose);

    // Main robot pose for AdvantageScope 3D visualization
    log["Robot"] << robotPose;

    // Component 0: Swerve drive modules - base robot frame
    // This represents the main chassis/drivetrain
    log["component_0"] << robotPose;

    // Component 1: Manipulator/Arm - positioned at front of robot
    // This would normally get actual position from manipulator subsystem
    // Using dummy values as manipulator subsystem doesn't exist yet
    // Position based on config.json zeroedPosition for component 1
    frc::Pose3d manipulatorPose =
        robotPose + frc::Transform3d(frc::Translation3d(0.27_m, 0.05_m, 0.53_m),
                                     frc::Rotation3d(0_deg, 0_deg, 270_deg));
    log["component_1"] << manipulatorPose;

    // Component 2: Robot base frame - secondary base component
    // Position based on config.json zeroedPosition for component 2
    frc::Pose3d basePose =
        robotPose +
        frc::Transform3d(frc::Translation3d(-1.52_m, -0.4_m, -0.02_m),
                         frc::Rotation3d(0_deg, 0_deg, 90_deg));
    log["component_2"] << basePose;

    // Component 3: Elevator - positioned above robot center
    // Using dummy values as elevator is not implemented yet
    // Position based on config.json zeroedPosition for component 3
    double elevatorHeight = 0.30;  // Dummy elevator height in meters
    frc::Pose3d elevatorPose =
        robotPose +
        frc::Transform3d(
            frc::Translation3d(0.31_m, -0.07_m, units::meter_t(elevatorHeight)),
            frc::Rotation3d(0_deg, 285_deg, 270_deg));
    log["component_3"] << elevatorPose;

    // Additional robot state information for debugging
    log["chassis_speeds"] << m_drive->GetState().Speeds;
    log["field_relative_heading"]
        << m_drive->GetState().Pose.Rotation().Degrees();
}
