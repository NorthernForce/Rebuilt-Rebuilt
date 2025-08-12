#pragma once

#include <utils/NFRRobotContainer.h>
#include <subsystems/SwerveDrive.h>

class ZippyContainer : public NFRRobotContainer {
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
    private:
        void ConfigureBindings() override;
        nfr::SwerveDrive m_drive;
        std::optional<frc2::CommandPtr> m_resetModulesCommand;
        frc2::CommandXboxController m_driverController{0};
};