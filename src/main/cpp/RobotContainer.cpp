// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc/DriverStation.h>
#include <frc2/command/Commands.h>
#include <frc2/command/button/CommandXboxController.h>
#include <logging/LogTypes.h>

#include "constants/Constants.h"
#include "frc/MathUtil.h"
#include "frc/Preferences.h"
#include "frc/smartdashboard/SmartDashboard.h"
#include "generated/TunerConstants.h"

using namespace std;
using namespace nfr;

/**
 * @brief Loads swerve module steering offsets from robot preferences
 *
 * Swerve modules need to know their absolute angular position to steer
 * correctly. These offsets compensate for mechanical differences and are
 * determined through calibration. They're stored in robot preferences so they
 * persist between reboots.
 *
 * @return Array of 4 rotation offsets [FrontLeft, FrontRight, BackLeft,
 * BackRight]
 */
std::array<frc::Rotation2d, 4> getModuleOffsets()
{
    return {
        (units::degree_t)frc::Preferences::GetDouble("FrontLeftOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("FrontRightOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("BackLeftOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("BackRightOffset", 0.0)};
}

/**
 * @brief Saves swerve module steering offsets to robot preferences
 *
 * After calibrating the swerve modules, we save the offsets so they persist
 * between robot reboots. This avoids having to recalibrate every time.
 *
 * @param offsets Array of 4 rotation offsets to save
 */
void SetModuleOffsets(const std::array<frc::Rotation2d, 4>& offsets)
{
    frc::Preferences::SetDouble("FrontLeftOffset",
                                offsets[0].Degrees().value());
    frc::Preferences::SetDouble("FrontRightOffset",
                                offsets[1].Degrees().value());
    frc::Preferences::SetDouble("BackLeftOffset", offsets[2].Degrees().value());
    frc::Preferences::SetDouble("BackRightOffset",
                                offsets[3].Degrees().value());
}

RobotContainer::RobotContainer()
{
    // Create our swerve drivetrain with all its configuration
    // This big constructor call sets up:
    // - Hardware configuration (motor controllers, encoders)
    // - Update rate (how often to calculate new motor commands)
    // - Odometry settings (how accurately we track robot position)
    // - PID controllers for autonomous path following
    // - Maximum speeds for safety
    drive = std::make_unique<SwerveDrive>(
        TunerConstants::DrivetrainConstants, DriveConstants::kUpdateRate,
        DriveConstants::kOdometryStandardDeviation,
        DriveConstants::kVisionStandardDeviation,
        DriveConstants::kTranslationPID, DriveConstants::kRotationPID,
        DriveConstants::kMaxTranslationSpeed, DriveConstants::kMaxRotationSpeed,
        TunerConstants::FrontLeft, TunerConstants::FrontRight,
        TunerConstants::BackLeft, TunerConstants::BackRight);

    // Load saved swerve module offsets from previous calibration
    drive->SetModuleOffsets(getModuleOffsets());

    // Set up controller bindings and default commands
    ConfigureBindings();
}

/**
 * @brief Processes joystick input to improve driving feel
 *
 * Raw joystick values can be noisy and sensitive. This function:
 * 1. Applies deadband - ignores small inputs near zero (reduces drift)
 * 2. Squares the input - gives finer control at low speeds, more aggressive at
 * high speeds
 *
 * The squaring technique is common in FRC - it makes precise movements easier
 * while still allowing full speed when needed.
 *
 * @param input Function that returns current joystick value (-1.0 to 1.0)
 * @return Processed function that returns cleaned-up joystick value
 */
std::function<double()> ProcessInput(std::function<double()> input)
{
    return [input = std::move(input)]() mutable
    {
        auto x = input();
        x = frc::ApplyDeadband(x, 0.10);  // Ignore inputs smaller than 10%
        x *= abs(x);  // Square the input for finer control at low speeds
        return x;
    };
}

void RobotContainer::ConfigureBindings()
{
    // Set up the default driving command that runs whenever no other command is
    // using the drivetrain This command reads joystick inputs and translates
    // them to robot movement
    drive->SetDefaultCommand(drive->DriveByJoystick(
        ProcessInput(
            [&]() {
                return driverController.GetLeftX();
            }),  // Side-to-side movement (strafe)
        ProcessInput(
            [&]() {
                return driverController.GetLeftY();
            }),  // Forward/backward movement
        ProcessInput([&]()
                     { return driverController.GetRightX(); }),  // Rotation
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
    driverController.Back().OnTrue(
        drive->RunOnce([&]() { drive->SeedFieldCentric(); }));

    // Create a command to reset swerve module offsets and put it on
    // SmartDashboard This allows drivers/programmers to recalibrate swerve
    // modules from the dashboard
    resetModulesCommand = drive->RunOnce(
        [&]()
        {
            // Reset all modules to 0 degrees and save the new offsets
            auto offsets =
                drive->ResetModuleOffsets({0_deg, 0_deg, 0_deg, 0_deg});
            SetModuleOffsets(offsets);
        });
    // Put this command on SmartDashboard so it can be triggered from the driver
    // station
    frc::SmartDashboard::PutData("Reset Swerve Modules",
                                 resetModulesCommand.value().get());
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand()
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

void RobotContainer::Log(const nfr::LogContext& log) const
{
    // Log important robot data for debugging and analysis
    // Match time helps correlate log data with what happened during the match
    log["match_time"] << frc::DriverStation::GetMatchTime();

    // Log drivetrain state (position, velocity, motor currents, etc.)
    // The drivetrain Log method will record detailed state information
    log["drive"] << drive;
}
