#pragma once

#include <util/NFRRobotContainer.h>
#include <subsystems/drive/SwerveDrive.h>

class ZippyContainer : public NFRRobotContainer
{
public:
    ZippyContainer();
    virtual ~ZippyContainer() = default;

    void RobotInit() override;
    void RobotPeriodic() override;
    void DisabledInit() override;
    void DisabledPeriodic() override;
    void DisabledExit() override;
    void AutonomousInit() override;
    void AutonomousPeriodic() override;
    void AutonomousExit() override;
    void TeleopInit() override;
    void TeleopPeriodic() override;
    void TeleopExit() override;
    void TestInit() override;
    void TestPeriodic() override;
    void TestExit() override;

    frc2::CommandPtr GetAutonomousCommand() override;

    /**
    * @brief Logs current robot state for debugging and analysis
    *
    * This method is called every 20ms to record important robot data like
    * drivetrain position, motor temperatures, etc. This data helps us debug
    * problems and analyze robot performance.
    *
    * @param log The logging context to write data to
    */
    void Log(const nfr::LogContext &log) const override;
private:
    /**
     * @brief Sets up controller button bindings and default commands
     *
     * This method configures:
     * - Which joystick axes control driving
     * - Which buttons trigger specific actions
     * - Default commands that run when no other commands are active
     */
    void ConfigureBindings() override;

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
    std::unique_ptr<nfr::SwerveDrive> m_drive{nullptr};

    /**
     * @brief Command to reset swerve module positions
     *
     * Swerve modules need periodic calibration to know their absolute position.
     * This command resets their encoders to known good positions.
     * std::optional means this may or may not contain a value.
     */
    std::optional<frc2::CommandPtr> m_resetModulesCommand;

    /**
     * @brief Xbox controller for driver input
     *
     * CommandXboxController integrates with the command system, allowing us to
     * easily bind buttons to commands. The number (0) indicates this is the
     * first controller connected to the driver station.
     */
    frc2::CommandXboxController m_driverController{0};
};