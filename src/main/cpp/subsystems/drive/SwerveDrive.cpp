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
    // Get current robot position from odometry
    const auto &pose = GetState().Pose;
    
    // Calculate correction velocities using PID controllers
    // PID controllers automatically correct errors between where we are vs where we should be
    
    // X-direction correction: if we're too far left/right, this creates velocity to correct it
    meters_per_second_t xFeedback{
        choreo.xController->Calculate(pose.X().value(), sample.x.value())};
    
    // Y-direction correction: if we're too far forward/back, this creates velocity to correct it    
    meters_per_second_t yFeedback{
        choreo.yController->Calculate(pose.Y().value(), sample.y.value())};
    
    // Heading correction: if we're facing wrong direction, this creates rotation to correct it
    radians_per_second_t headingFeedback{choreo.headingController->Calculate(
        pose.Rotation().Radians().value(), sample.heading.value())};
    
    // Combine feedforward (planned velocity) + feedback (error correction)
    // Feedforward: sample.vx, sample.vy, sample.omega (what the path planner calculated)
    // Feedback: xFeedback, yFeedback, headingFeedback (corrections for any errors)
    SetControl(choreo.follower.WithSpeeds(
        ChassisSpeeds{xFeedback + sample.vx, yFeedback + sample.vy,
                      headingFeedback + sample.omega}));
}

CommandPtr SwerveDrive::GetSysIdRoutine()
{
    // System Identification (SysId) is a process that automatically characterizes
    // the robot's response to different inputs, allowing us to calculate optimal
    // PID controller gains. This runs a comprehensive test sequence.
    
    return cmd::Sequence(
        // Start logging data for analysis
        cmd::RunOnce([]() { SignalLogger::Start(); }),
        
        // Test translation motors (drive wheels) in both directions
        // Quasistatic = slowly ramp up voltage to measure steady-state response
        sysIdRoutineTranslation.Quasistatic(Direction::kForward),
        sysIdRoutineTranslation.Quasistatic(Direction::kReverse),
        // Dynamic = quick voltage steps to measure transient response  
        sysIdRoutineTranslation.Dynamic(Direction::kForward),
        sysIdRoutineTranslation.Dynamic(Direction::kReverse),
        
        // Test steer motors (wheel turning) in both directions
        sysIdRoutineSteerGains.Quasistatic(Direction::kForward),
        sysIdRoutineSteerGains.Quasistatic(Direction::kReverse),
        sysIdRoutineSteerGains.Dynamic(Direction::kForward),
        sysIdRoutineSteerGains.Dynamic(Direction::kReverse),
        
        // Test robot rotation (all wheels working together to spin robot)
        sysIdRoutineRotation.Quasistatic(Direction::kForward),
        sysIdRoutineRotation.Quasistatic(Direction::kReverse),
        sysIdRoutineRotation.Dynamic(Direction::kForward),
        sysIdRoutineRotation.Dynamic(Direction::kReverse),
        
        // Stop logging and save data for analysis
        cmd::RunOnce([]() { SignalLogger::Stop(); }));
}

void SwerveDrive::Periodic()
{
    // This method runs every 20ms automatically
    
    // When robot is disabled, set the field orientation based on alliance color
    // This ensures "forward" points toward the correct goal
    if (DriverStation::IsDisabled())
    {
        // Get our alliance color (red or blue) from the driver station
        auto const allianceColor = DriverStation::GetAlliance().value_or(
            DriverStation::Alliance::kRed);
        
        // Set field perspective based on alliance:
        // - Red alliance: "forward" = 180° (away from red alliance wall)
        // - Blue alliance: "forward" = 0° (away from blue alliance wall)
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
    // Apply calibration offsets to each swerve module
    // Each module's CANcoder (absolute encoder) needs an offset to account for
    // mechanical differences in how the wheels are installed
    
    for (size_t i = 0; i < offsets.size(); ++i)
    {
        auto &module = GetModule(i);  // Get swerve module (0=FL, 1=FR, 2=BL, 3=BR)
        auto &cancoder = module.GetEncoder();  // Get the absolute position encoder
        
        // Read current encoder configuration
        configs::CANcoderConfiguration config;
        cancoder.GetConfigurator().Refresh(config);
        
        // Set the magnetic offset to compensate for installation differences
        config.MagnetSensor.MagnetOffset = offsets[i].Degrees();
        
        // Apply the new configuration to the encoder
        cancoder.GetConfigurator().Apply(config);
    }
}

std::array<Rotation2d, 4> SwerveDrive::ResetModuleOffsets(
    const std::array<frc::Rotation2d, 4> &targetOffsets)
{
    // This method is used during calibration to automatically calculate offsets
    // Process: Point all wheels straight, call this method, and it calculates
    // what offsets are needed to make the encoders read the target angles
    
    std::array<Rotation2d, 4> offsets;
    for (size_t i = 0; i < offsets.size(); ++i)
    {
        auto &module = GetModule(i);
        auto &cancoder = module.GetEncoder();
        
        // Get current wheel angle as reported by the encoder
        const auto &currentAngle = module.GetCurrentState().angle;
        
        // Calculate how far off we are from the target angle
        const auto &delta = currentAngle - targetOffsets[i];
        
        // Read current encoder configuration 
        configs::CANcoderConfiguration config;
        cancoder.GetConfigurator().Refresh(config);
        const auto &currentOffset = config.MagnetSensor.MagnetOffset;
        
        // Calculate new offset = old offset + error
        auto newOffset = currentOffset + delta.Degrees();
        
        // Keep offset in range [-180°, 180°] to avoid wrap-around issues
        newOffset = (degree_t)frc::InputModulus(((degree_t)newOffset).value(),
                                                -180.0, 180.0);
        
        // Apply the new offset
        config.MagnetSensor.MagnetOffset = newOffset;
        cancoder.GetConfigurator().Apply(config);
        
        // Save the offset value to return to caller
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
        // Field-centric driving: "forward" always means away from alliance wall
        return ApplyRequest(
            // This is a lambda (anonymous function) that captures joystick inputs
            // The [&, xAxis = std::move(xAxis), ...] syntax:
            // - & = capture 'this' object by reference 
            // - xAxis = std::move(xAxis) = capture and move the function objects
            // - std::move prevents expensive copying of function objects
            [&, xAxis = std::move(xAxis), yAxis = std::move(yAxis),
             rotationAxis = std::move(rotationAxis)]() -> auto &&
            {
                // Create field-centric drive request with joystick values scaled to max speeds
                return fieldCentricRequest
                    .WithVelocityX(xAxis() * maxTranslationSpeed)      // Sideways movement
                    .WithVelocityY(yAxis() * maxTranslationSpeed)      // Forward/back movement  
                    .WithRotationalRate(rotationAxis() * maxRotationSpeed); // Spinning
            });
    }
    else
    {
        // Robot-centric driving: "forward" means whatever direction robot is facing
        return ApplyRequest(
            [&, xAxis = std::move(xAxis), yAxis = std::move(yAxis),
             rotationAxis = std::move(rotationAxis)]() -> auto &&
            {
                return robotRelativeRequest
                    .WithVelocityX(xAxis() * maxTranslationSpeed)      // Sideways (relative to robot)
                    .WithVelocityY(yAxis() * maxTranslationSpeed)      // Forward/back (relative to robot)
                    .WithRotationalRate(rotationAxis() * maxRotationSpeed); // Spinning
            });
    }
}

void SwerveDrive::Log(const nfr::LogContext &log) const
{
    // Log robot position and orientation on the field
    log["pose"] << GetState().Pose;
    
    // Log current robot velocity (how fast it's moving in each direction)
    log["speeds"] << GetState().Speeds;
    
    // Calculate and log overall speed magnitude 
    // This gives a single number for "how fast is the robot moving overall?"
    auto vx = GetState().Speeds.vx;  // Velocity in X direction
    auto vy = GetState().Speeds.vy;  // Velocity in Y direction
    auto speed = math::sqrt(vx * vx + vy * vy);  // Pythagorean theorem: total speed
    log["speed"] << speed;
}