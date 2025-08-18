#include "ZippyContainer.h"

#include <frc2/command/Commands.h>
#include <frc2/command/button/CommandXboxController.h>

#include "constants/Constants.h"
#include "frc/MathUtil.h"
#include "frc/Preferences.h"
#include "frc/smartdashboard/SmartDashboard.h"
#include "generated/TunerConstants.h"

using namespace std;
using namespace nfr;

std::array<frc::Rotation2d, 4> getModuleOffsets()
{
    return {
        (units::degree_t)frc::Preferences::GetDouble("FrontLeftOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("FrontRightOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("BackLeftOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("BackRightOffset", 0.0)};
}

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

ZippyContainer::ZippyContainer()
    : m_drive(TunerConstants::DrivetrainConstants, DriveConstants::kUpdateRate,
              DriveConstants::kOdometryStandardDeviation,
              DriveConstants::kVisionStandardDeviation,
              DriveConstants::kTranslationPID, DriveConstants::kRotationPID,
              DriveConstants::kMaxTranslationSpeed,
              DriveConstants::kMaxRotationSpeed, TunerConstants::FrontLeft,
              TunerConstants::FrontRight, TunerConstants::BackLeft,
              TunerConstants::BackRight)
{
    m_drive.SetModuleOffsets(getModuleOffsets());
    ConfigureBindings();
}

std::function<double()> ProcessInput(std::function<double()> input)
{
    return [input = std::move(input)]() mutable
    {
        auto x = input();
        x = frc::ApplyDeadband(x, 0.10);
        x *= abs(x);  // Square the input for finer control at low speeds
        return x;
    };
}

void ZippyContainer::ConfigureBindings()
{
    m_drive.SetDefaultCommand(m_drive.DriveByJoystick(
        ProcessInput([&]() { return m_driverController.GetLeftX(); }),
        ProcessInput([&]() { return m_driverController.GetLeftY(); }),
        ProcessInput([&]() { return m_driverController.GetRightX(); }),
        true  // Field-centric driving
        ));
    m_driverController.Back().OnTrue(
        m_drive.RunOnce([&]() { m_drive.SeedFieldCentric(); }));
    m_resetModulesCommand = m_drive.RunOnce(
        [&]()
        {
            auto offsets =
                m_drive.ResetModuleOffsets({0_deg, 0_deg, 0_deg, 0_deg});
            SetModuleOffsets(offsets);
        });
    frc::SmartDashboard::PutData("Reset Swerve Modules",
                                 m_resetModulesCommand.value().get());
}

frc2::CommandPtr ZippyContainer::GetAutonomousCommand()
{
    return frc2::cmd::Print("No autonomous command configured");
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