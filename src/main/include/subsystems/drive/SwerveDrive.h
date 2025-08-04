#pragma once

#include <choreo/Choreo.h>
#include <frc/Notifier.h>
#include <frc/controller/PIDController.h>
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/sysid/SysIdRoutine.h>
#include <logging/Logger.h>
#include <pathplanner/lib/auto/AutoBuilder.h>
#include <pathplanner/lib/controllers/PPHolonomicDriveController.h>
#include <units/time.h>

#include <ctre/phoenix6/SignalLogger.hpp>
#include <ctre/phoenix6/swerve/SwerveDrivetrain.hpp>

namespace nfr
{
    class SwerveDrive : public frc2::SubsystemBase,
                        public ctre::phoenix6::swerve::SwerveDrivetrain<
                            ctre::phoenix6::hardware::TalonFX,
                            ctre::phoenix6::hardware::TalonFX,
                            ctre::phoenix6::hardware::CANcoder>
    {
    private:
        static constexpr units::second_t kSimLoopPeriod = 5_ms;
        std::unique_ptr<frc::Notifier> simNotifier;
        units::second_t lastSimTime;
        static constexpr frc::Rotation2d kBlueAlliancePerspectiveRotation{
            0_deg};
        static constexpr frc::Rotation2d kRedAlliancePerspectiveRotation{
            180_deg};
        ctre::phoenix6::swerve::requests::SysIdSwerveTranslation
            translationCharacterization;
        ctre::phoenix6::swerve::requests::SysIdSwerveSteerGains
            steerCharacterization;
        ctre::phoenix6::swerve::requests::SysIdSwerveRotation
            rotationCharacterization;
        frc2::sysid::SysIdRoutine sysIdRoutineTranslation{
            frc2::sysid::Config{
                std::nullopt,  // Ramp rate of 1 volt / second
                4_V,
                std::nullopt,  // 10 second timeout,
                [](frc::sysid::State state)
                {
                    ctre::phoenix6::SignalLogger::WriteString(
                        "SysIdTranslation_State",
                        frc::sysid::SysIdRoutineLog::StateEnumToString(state));
                }},
            frc2::sysid::Mechanism{
                [this](units::volt_t output)
                { SetControl(translationCharacterization.WithVolts(output)); },
                {},
                this}};
        frc2::sysid::SysIdRoutine sysIdRoutineSteerGains{
            frc2::sysid::Config{
                std::nullopt,  // Ramp rate of 1 volt / second
                4_V,
                std::nullopt,  // 10 second timeout,
                [](frc::sysid::State state)
                {
                    ctre::phoenix6::SignalLogger::WriteString(
                        "SysIdSteerGains_State",
                        frc::sysid::SysIdRoutineLog::StateEnumToString(state));
                }},
            frc2::sysid::Mechanism{
                [this](units::volt_t output)
                { SetControl(steerCharacterization.WithVolts(output)); },
                {},
                this}};
        frc2::sysid::SysIdRoutine sysIdRoutineRotation{
            frc2::sysid::Config{
                std::nullopt,  // Ramp rate of 1 volt / second
                4_V,
                std::nullopt,  // 10 second timeout,
                [](frc::sysid::State state)
                {
                    ctre::phoenix6::SignalLogger::WriteString(
                        "SysIdRotation_State",
                        frc::sysid::SysIdRoutineLog::StateEnumToString(state));
                }},
            frc2::sysid::Mechanism{
                [this](units::volt_t output)
                {
                    SetControl(rotationCharacterization.WithRotationalRate(
                        output * (1_rad_per_s / 1_V)));
                    ctre::phoenix6::SignalLogger::WriteValue(
                        "Rotation_Rate", output * (1_rad_per_s / 1_V));
                },
                {},
                this}};
        ctre::phoenix6::swerve::requests::ApplyRobotSpeeds pathplannerFollower =
            ctre::phoenix6::swerve::requests::ApplyRobotSpeeds();
        struct
        {
            ctre::phoenix6::swerve::requests::ApplyFieldSpeeds follower =
                ctre::phoenix6::swerve::requests::ApplyFieldSpeeds()
                    .WithForwardPerspective(
                        ctre::phoenix6::swerve::requests::
                            ForwardPerspectiveValue::BlueAlliance);
            std::unique_ptr<frc::PIDController> xController;
            std::unique_ptr<frc::PIDController> yController;
            std::unique_ptr<frc::PIDController> headingController;
        } choreo;
        void ConfigurePathplanner(pathplanner::PIDConstants translationPID,
                                  pathplanner::PIDConstants rotationPID);
        void ConfigureChoreo(pathplanner::PIDConstants translationPID,
                             pathplanner::PIDConstants rotationPID);
        void StartSimThread();
        ctre::phoenix6::swerve::requests::FieldCentric fieldCentricRequest =
            ctre::phoenix6::swerve::requests::FieldCentric()
                .WithForwardPerspective(
                    ctre::phoenix6::swerve::requests::ForwardPerspectiveValue::
                        OperatorPerspective);
        ctre::phoenix6::swerve::requests::RobotCentric robotRelativeRequest =
            ctre::phoenix6::swerve::requests::RobotCentric();
        units::meters_per_second_t maxTranslationSpeed;
        units::radians_per_second_t maxRotationSpeed;

    public:
        using SwerveModuleConstants =
            ctre::phoenix6::swerve::SwerveModuleConstants<
                ctre::phoenix6::configs::TalonFXConfiguration,
                ctre::phoenix6::configs::TalonFXConfiguration,
                ctre::phoenix6::configs::CANcoderConfiguration>;
        SwerveDrive(const ctre::phoenix6::swerve::SwerveDrivetrainConstants
                        &drivetrainConstants,
                    units::hertz_t updateRate,
                    std::array<double, 3> const &odometryStandardDeviation,
                    std::array<double, 3> const &visionStandardDeviation,
                    pathplanner::PIDConstants translationPID,
                    pathplanner::PIDConstants rotationPID,
                    units::meters_per_second_t maxTranslationSpeed,
                    units::radians_per_second_t maxRotationSpeed,
                    const SwerveModuleConstants &frontLeftConstants,
                    const SwerveModuleConstants &frontRightConstants,
                    const SwerveModuleConstants &backLeftConstants,
                    const SwerveModuleConstants &backRightConstants);
        frc2::sysid::SysIdRoutine &GetSysIdTranslation()
        {
            return sysIdRoutineTranslation;
        }
        frc2::sysid::SysIdRoutine &GetSysIdSteerGains()
        {
            return sysIdRoutineSteerGains;
        }
        frc2::sysid::SysIdRoutine &GetSysIdRotation()
        {
            return sysIdRoutineRotation;
        }
        frc2::CommandPtr GetSysIdRoutine();
        template <typename RequestSupplier>
            requires std::is_lvalue_reference_v<
                         std::invoke_result_t<RequestSupplier>> &&
                     requires(RequestSupplier req,
                              ctre::phoenix6::swerve::SwerveDrivetrain<
                                  ctre::phoenix6::hardware::TalonFX,
                                  ctre::phoenix6::hardware::TalonFX,
                                  ctre::phoenix6::hardware::CANcoder> &drive) {
                         drive.SetControl(req());
                     }
        frc2::CommandPtr ApplyRequest(RequestSupplier request)
        {
            return Run([this, request = std::move(request)]
                       { return SetControl(request()); });
        }

        /**
         * \brief Returns a command that applies the specified control request
         * to this swerve drivetrain.
         *
         * \param request Function returning the request to apply
         * \returns Command to run
         */
        template <typename RequestSupplier>
            requires(!std::is_lvalue_reference_v<
                        std::invoke_result_t<RequestSupplier>>) &&
                    requires(RequestSupplier req,
                             ctre::phoenix6::swerve::SwerveDrivetrain<
                                 ctre::phoenix6::hardware::TalonFX,
                                 ctre::phoenix6::hardware::TalonFX,
                                 ctre::phoenix6::hardware::CANcoder> &drive) {
                        drive.SetControl(req());
                    }
        frc2::CommandPtr ApplyRequest(RequestSupplier request)
        {
            return Run([this, request = std::move(request)]
                       { return SetControl(request()); });
        }
        void Periodic() override;
        void AddVisionMeasurement(frc::Pose2d pose,
                                  units::second_t timestamp) override;
        void FollowTrajectory(const choreo::SwerveSample &sample);
        void SetModuleOffsets(const std::array<frc::Rotation2d, 4> &offsets);
        std::array<frc::Rotation2d, 4> ResetModuleOffsets(
            const std::array<frc::Rotation2d, 4> &targetOffsets);
        frc2::CommandPtr DriveByJoystick(std::function<double()> xAxis,
                                         std::function<double()> yAxis,
                                         std::function<double()> rotationAxis,
                                         bool fieldRelative = true);
        void Log(const nfr::LogContext &log) const;
    };
}  // namespace nfr