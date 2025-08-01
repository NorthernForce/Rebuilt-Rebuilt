#include "subsystems/drive/SwerveDrive.h"

#include <frc/DriverStation.h>
#include <frc/MathUtil.h>
#include <frc/RobotController.h>

#include "logging/LogTypes.h"

using namespace nfr;
using namespace ctre::phoenix6;
using namespace ctre::phoenix6::swerve;
using namespace ctre::phoenix6::hardware;
using namespace frc2::sysid;
using namespace frc2;
using namespace frc;
using namespace units;
using namespace std;
using namespace pathplanner;
using namespace choreo;

SwerveDrive::SwerveDrive(const SwerveDrivetrainConstants &driveConstants,
                         hertz_t updateRate,
                         std::array<double, 3> const &odometryStandardDeviation,
                         std::array<double, 3> const &visionStandardDeviation,
                         PIDConstants translationPID, PIDConstants rotationPID,
                         units::meters_per_second_t maxTranslationSpeed,
                         units::radians_per_second_t maxRotationSpeed,
                         const SwerveModuleConstants &frontLeftConstants,
                         const SwerveModuleConstants &frontRightConstants,
                         const SwerveModuleConstants &rearLeftConstants,
                         const SwerveModuleConstants &rearRightConstants)
    : SwerveDrivetrain(driveConstants, updateRate, odometryStandardDeviation,
                       visionStandardDeviation, frontLeftConstants,
                       frontRightConstants, rearLeftConstants,
                       rearRightConstants),
      maxTranslationSpeed(maxTranslationSpeed),
      maxRotationSpeed(maxRotationSpeed)
{
    ConfigurePathplanner(translationPID, rotationPID);
    ConfigureChoreo(translationPID, rotationPID);
    if (utils::IsSimulation())
    {
        StartSimThread();
    }
}

void SwerveDrive::ConfigurePathplanner(PIDConstants translationPID,
                                       PIDConstants rotationPID)
{
    auto config = RobotConfig::fromGUISettings();
    AutoBuilder::configure(
        [this]() { return GetState().Pose; }, [this](const Pose2d &pose)
        { ResetPose(pose); }, [this]() { return GetState().Speeds; },
        [this](const ChassisSpeeds &speeds,
               const DriveFeedforwards &feedforwards)
        {
            SetControl(pathplannerFollower.WithSpeeds(speeds)
                           .WithWheelForceFeedforwardsX(
                               feedforwards.robotRelativeForcesX)
                           .WithWheelForceFeedforwardsY(
                               feedforwards.robotRelativeForcesY));
        },
        make_shared<PPHolonomicDriveController>(translationPID, rotationPID),
        std::move(config),
        []
        {
            auto const alliance = DriverStation::GetAlliance().value_or(
                DriverStation::Alliance::kBlue);
            return alliance == DriverStation::Alliance::kRed;
        },
        this);
}

void SwerveDrive::StartSimThread()
{
    lastSimTime = utils::GetCurrentTime();
    simNotifier = make_unique<frc::Notifier>(
        [this]
        {
            units::second_t const currentTime = utils::GetCurrentTime();
            auto const deltaTime = currentTime - lastSimTime;
            lastSimTime = currentTime;

            /* use the measured time delta, get battery voltage from WPILib */
            UpdateSimState(deltaTime, RobotController::GetBatteryVoltage());
        });
    simNotifier->StartPeriodic(kSimLoopPeriod);
}

void SwerveDrive::ConfigureChoreo(PIDConstants translationPID,
                                  PIDConstants rotationPID)
{
    choreo.xController = make_unique<PIDController>(
        translationPID.kP, translationPID.kI, translationPID.kD);
    choreo.yController = make_unique<PIDController>(
        translationPID.kP, translationPID.kI, translationPID.kD);
    choreo.headingController = make_unique<PIDController>(
        rotationPID.kP, rotationPID.kI, rotationPID.kD);
    choreo.headingController->EnableContinuousInput(-M_PI, M_PI);
}

void SwerveDrive::FollowTrajectory(const SwerveSample &sample)
{
    const auto &pose = GetState().Pose;
    meters_per_second_t xFeedback{
        choreo.xController->Calculate(pose.X().value(), sample.x.value())};
    meters_per_second_t yFeedback{
        choreo.yController->Calculate(pose.Y().value(), sample.y.value())};
    radians_per_second_t headingFeedback{choreo.headingController->Calculate(
        pose.Rotation().Radians().value(), sample.heading.value())};
    SetControl(choreo.follower.WithSpeeds(
        ChassisSpeeds{xFeedback + sample.vx, yFeedback + sample.vy,
                      headingFeedback + sample.omega}));
}

CommandPtr SwerveDrive::GetSysIdRoutine()
{
    return cmd::Sequence(
        cmd::RunOnce([]() { SignalLogger::Start(); }),
        sysIdRoutineTranslation.Quasistatic(Direction::kForward),
        sysIdRoutineTranslation.Quasistatic(Direction::kReverse),
        sysIdRoutineTranslation.Dynamic(Direction::kForward),
        sysIdRoutineTranslation.Dynamic(Direction::kReverse),
        sysIdRoutineSteerGains.Quasistatic(Direction::kForward),
        sysIdRoutineSteerGains.Quasistatic(Direction::kReverse),
        sysIdRoutineSteerGains.Dynamic(Direction::kForward),
        sysIdRoutineSteerGains.Dynamic(Direction::kReverse),
        sysIdRoutineRotation.Quasistatic(Direction::kForward),
        sysIdRoutineRotation.Quasistatic(Direction::kReverse),
        sysIdRoutineRotation.Dynamic(Direction::kForward),
        sysIdRoutineRotation.Dynamic(Direction::kReverse),
        cmd::RunOnce([]() { SignalLogger::Stop(); }));
}

void SwerveDrive::Periodic()
{
    if (DriverStation::IsDisabled())
    {
        auto const allianceColor = DriverStation::GetAlliance().value_or(
            DriverStation::Alliance::kRed);
        SetOperatorPerspectiveForward(allianceColor ==
                                              DriverStation::Alliance::kRed
                                          ? kRedAlliancePerspectiveRotation
                                          : kBlueAlliancePerspectiveRotation);
    }
}

void SwerveDrive::AddVisionMeasurement(Pose2d pose, second_t timestamp)
{
    SwerveDrivetrain::AddVisionMeasurement(pose,
                                           utils::FPGAToCurrentTime(timestamp));
}

void SwerveDrive::SetModuleOffsets(const std::array<Rotation2d, 4> &offsets)
{
    for (size_t i = 0; i < offsets.size(); ++i)
    {
        auto &module = GetModule(i);
        auto &cancoder = module.GetEncoder();
        configs::CANcoderConfiguration config;
        cancoder.GetConfigurator().Refresh(config);
        config.MagnetSensor.MagnetOffset = offsets[i].Degrees();
        cancoder.GetConfigurator().Apply(config);
    }
}

std::array<Rotation2d, 4> SwerveDrive::ResetModuleOffsets(
    const std::array<frc::Rotation2d, 4> &targetOffsets)
{
    std::array<Rotation2d, 4> offsets;
    for (size_t i = 0; i < offsets.size(); ++i)
    {
        auto &module = GetModule(i);
        auto &cancoder = module.GetEncoder();
        const auto &currentAngle = module.GetCurrentState().angle;
        const auto &delta = currentAngle - targetOffsets[i];
        configs::CANcoderConfiguration config;
        cancoder.GetConfigurator().Refresh(config);
        const auto &currentOffset = config.MagnetSensor.MagnetOffset;
        auto newOffset = currentOffset + delta.Degrees();
        newOffset = (degree_t)frc::InputModulus(((degree_t)newOffset).value(),
                                                -180.0, 180.0);
        config.MagnetSensor.MagnetOffset = newOffset;
        cancoder.GetConfigurator().Apply(config);
        offsets[i] = Rotation2d{newOffset};
    }
    return offsets;
}

CommandPtr SwerveDrive::DriveByJoystick(function<double()> xAxis,
                                        function<double()> yAxis,
                                        function<double()> rotationAxis,
                                        bool fieldCentric)
{
    if (fieldCentric)
    {
        return ApplyRequest(
            [&, xAxis = std::move(xAxis), yAxis = std::move(yAxis),
             rotationAxis = std::move(rotationAxis)]() -> auto &&
            {
                return fieldCentricRequest
                    .WithVelocityX(xAxis() * maxTranslationSpeed)
                    .WithVelocityY(yAxis() * maxTranslationSpeed)
                    .WithRotationalRate(rotationAxis() * maxRotationSpeed);
            });
    }
    else
    {
        return ApplyRequest(
            [&, xAxis = std::move(xAxis), yAxis = std::move(yAxis),
             rotationAxis = std::move(rotationAxis)]() -> auto &&
            {
                return robotRelativeRequest
                    .WithVelocityX(xAxis() * maxTranslationSpeed)
                    .WithVelocityY(yAxis() * maxTranslationSpeed)
                    .WithRotationalRate(rotationAxis() * maxRotationSpeed);
            });
    }
}

void SwerveDrive::Log(const nfr::LogContext &log) const
{
    log["pose"] << GetState().Pose;
    log["speeds"] << GetState().Speeds;
    auto vx = GetState().Speeds.vx;
    auto vy = GetState().Speeds.vy;
    auto speed = math::sqrt(vx * vx + vy * vy);
    log["speed"] << speed;
}