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
    /**
     * @brief Advanced swerve drivetrain subsystem for FRC robots
     *
     * ## What is Swerve Drive?
     * Swerve drive is a type of drivetrain where each wheel can:
     * 1. **Rotate** (spin to make the robot move)
     * 2. **Steer** (turn to point in any direction)
     *
     * This gives incredible maneuverability - the robot can move in any
     * direction instantly without having to turn first, and can even spin while
     * moving sideways!
     *
     * ## Key Features:
     * - **Field-centric driving**: "Forward" always means away from your
     * alliance wall
     * - **Autonomous path following**: Robot can follow pre-planned routes
     * precisely
     * - **System identification**: Automatic tuning of control parameters
     * - **Advanced odometry**: Tracks robot position very accurately
     * - **Vision integration**: Can use camera data to improve position
     * estimates
     *
     * ## Inheritance Chain:
     * This class inherits from both:
     * - `SubsystemBase`: Integrates with WPILib's command system
     * - `SwerveDrivetrain`: Provides low-level swerve drive control from CTRE
     * Phoenix
     *
     * @note This uses CTRE Phoenix 6 hardware (TalonFX motors, CANcoder
     * encoders)
     */
    class SwerveDrive : public frc2::SubsystemBase,
                        public ctre::phoenix6::swerve::SwerveDrivetrain<
                            ctre::phoenix6::hardware::TalonFX,
                            ctre::phoenix6::hardware::TalonFX,
                            ctre::phoenix6::hardware::CANcoder>
    {
    private:
        // === SIMULATION CONSTANTS ===
        /** @brief How often to update simulation physics (200 Hz = every 5ms)
         */
        static constexpr units::second_t kSimLoopPeriod = 5_ms;

        /** @brief Handles periodic simulation updates */
        std::unique_ptr<frc::Notifier> simNotifier;

        /** @brief Tracks time for simulation physics calculations */
        units::second_t lastSimTime;

        // === FIELD ORIENTATION CONSTANTS ===
        /** @brief Blue alliance perspective: 0° is away from blue alliance wall
         */
        static constexpr frc::Rotation2d kBlueAlliancePerspectiveRotation{
            0_deg};

        /** @brief Red alliance perspective: 180° is away from red alliance wall
         */
        static constexpr frc::Rotation2d kRedAlliancePerspectiveRotation{
            180_deg};

        // === SYSTEM IDENTIFICATION (SYSID) SETUP ===
        // SysId helps automatically tune PID controllers by characterizing
        // robot response

        /** @brief SysId request for measuring translation (drive) motor
         * response */
        ctre::phoenix6::swerve::requests::SysIdSwerveTranslation
            translationCharacterization;

        /** @brief SysId request for measuring steer motor response */
        ctre::phoenix6::swerve::requests::SysIdSwerveSteerGains
            steerCharacterization;

        /** @brief SysId request for measuring robot rotation response */
        ctre::phoenix6::swerve::requests::SysIdSwerveRotation
            rotationCharacterization;
        /**
         * @brief SysId routine for characterizing translation (drive) motors
         *
         * This systematically applies different voltages to the drive motors
         * and measures the robot's response to automatically calculate optimal
         * PID gains. The configuration specifies:
         * - Maximum voltage to apply (4V for safety)
         * - How to log the test progress
         */
        frc2::sysid::SysIdRoutine sysIdRoutineTranslation{
            frc2::sysid::Config{
                std::nullopt,  // Use default ramp rate (1 volt/second)
                4_V,           // Maximum voltage for safety
                std::nullopt,  // Use default timeout (10 seconds)
                [](frc::sysid::State state)
                {
                    // Log the test state so we can see progress in Phoenix
                    // Tuner
                    ctre::phoenix6::SignalLogger::WriteString(
                        "SysIdTranslation_State",
                        frc::sysid::SysIdRoutineLog::StateEnumToString(state));
                }},
            frc2::sysid::Mechanism{
                // Function that applies voltage to the drive motors
                [this](units::volt_t output)
                { SetControl(translationCharacterization.WithVolts(output)); },
                {},  // No additional log data needed
                this}};

        /**
         * @brief SysId routine for characterizing steer motors
         *
         * Similar to translation SysId, but tests the steering motors that
         * rotate each wheel to point in different directions.
         */
        frc2::sysid::SysIdRoutine sysIdRoutineSteerGains{
            frc2::sysid::Config{
                std::nullopt,  // Use default ramp rate
                4_V,           // Maximum voltage for safety
                std::nullopt,  // Use default timeout
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

        /**
         * @brief SysId routine for characterizing robot rotation
         *
         * Tests how the robot responds when all wheels work together to spin
         * the robot in place. This helps tune rotation PID controllers.
         */
        frc2::sysid::SysIdRoutine sysIdRoutineRotation{
            frc2::sysid::Config{
                std::nullopt,  // Use default ramp rate
                4_V,           // Maximum voltage for safety
                std::nullopt,  // Use default timeout
                [](frc::sysid::State state)
                {
                    ctre::phoenix6::SignalLogger::WriteString(
                        "SysIdRotation_State",
                        frc::sysid::SysIdRoutineLog::StateEnumToString(state));
                }},
            frc2::sysid::Mechanism{
                [this](units::volt_t output)
                {
                    // Convert voltage to rotational rate (1V = 1 rad/s for this
                    // test)
                    SetControl(rotationCharacterization.WithRotationalRate(
                        output * (1_rad_per_s / 1_V)));
                    // Log the commanded rotation rate for analysis
                    ctre::phoenix6::SignalLogger::WriteValue(
                        "Rotation_Rate", output * (1_rad_per_s / 1_V));
                },
                {},
                this}};
        // === AUTONOMOUS PATH FOLLOWING SETUP ===

        /**
         * @brief Request for PathPlanner autonomous path following
         *
         * PathPlanner is a tool for creating autonomous paths. This request
         * applies robot-relative speeds (forward/sideways/rotation) to follow
         * the planned path precisely.
         */
        ctre::phoenix6::swerve::requests::ApplyRobotSpeeds pathplannerFollower =
            ctre::phoenix6::swerve::requests::ApplyRobotSpeeds();

        /**
         * @brief Setup for Choreo path following (alternative to PathPlanner)
         *
         * Choreo is another autonomous path planning tool. This struct contains
         * everything needed to follow Choreo-generated paths.
         */
        struct
        {
            /** @brief Field-relative speed request for following Choreo paths
             */
            ctre::phoenix6::swerve::requests::ApplyFieldSpeeds follower =
                ctre::phoenix6::swerve::requests::ApplyFieldSpeeds()
                    .WithForwardPerspective(
                        ctre::phoenix6::swerve::requests::
                            ForwardPerspectiveValue::BlueAlliance);

            /** @brief PID controller for X-direction path following */
            std::unique_ptr<frc::PIDController> xController;

            /** @brief PID controller for Y-direction path following */
            std::unique_ptr<frc::PIDController> yController;

            /** @brief PID controller for rotation during path following */
            std::unique_ptr<frc::PIDController> headingController;
        } choreo;

        // === PRIVATE HELPER METHODS ===

        /**
         * @brief Configures PathPlanner for autonomous path following
         * @param translationPID PID gains for X/Y movement
         * @param rotationPID PID gains for rotation
         */
        void ConfigurePathplanner(pathplanner::PIDConstants translationPID,
                                  pathplanner::PIDConstants rotationPID);

        /**
         * @brief Configures Choreo for autonomous path following
         * @param translationPID PID gains for X/Y movement
         * @param rotationPID PID gains for rotation
         */
        void ConfigureChoreo(pathplanner::PIDConstants translationPID,
                             pathplanner::PIDConstants rotationPID);

        /** @brief Starts the simulation thread (only runs in simulation) */
        void StartSimThread();

        // === MANUAL DRIVE REQUESTS ===

        /**
         * @brief Field-centric driving request for manual control
         *
         * Field-centric means "forward" is always away from our alliance wall,
         * regardless of which direction the robot is facing. Much easier for
         * drivers!
         */
        ctre::phoenix6::swerve::requests::FieldCentric fieldCentricRequest =
            ctre::phoenix6::swerve::requests::FieldCentric()
                .WithForwardPerspective(
                    ctre::phoenix6::swerve::requests::ForwardPerspectiveValue::
                        OperatorPerspective);

        /**
         * @brief Robot-centric driving request for manual control
         *
         * Robot-centric means "forward" is whatever direction the robot is
         * currently facing. Harder for drivers but sometimes useful for precise
         * positioning.
         */
        ctre::phoenix6::swerve::requests::RobotCentric robotRelativeRequest =
            ctre::phoenix6::swerve::requests::RobotCentric();

        // === SPEED LIMITS ===

        /** @brief Maximum speed for translation (m/s) */
        units::meters_per_second_t maxTranslationSpeed;

        /** @brief Maximum speed for rotation (rad/s) */
        units::radians_per_second_t maxRotationSpeed;

    public:
        /**
         * @brief Type alias for swerve module hardware configuration
         *
         * This defines what hardware each swerve module uses:
         * - TalonFX for drive motor configuration
         * - TalonFX for steer motor configuration
         * - CANcoder for absolute position encoder configuration
         */
        using SwerveModuleConstants =
            ctre::phoenix6::swerve::SwerveModuleConstants<
                ctre::phoenix6::configs::TalonFXConfiguration,
                ctre::phoenix6::configs::TalonFXConfiguration,
                ctre::phoenix6::configs::CANcoderConfiguration>;

        /**
         * @brief Constructor - creates and configures the swerve drivetrain
         *
         * This sets up everything needed for swerve drive operation:
         * - Hardware configuration for all motors and encoders
         * - Autonomous path following capabilities
         * - Simulation support for testing code without robot hardware
         *
         * @param drivetrainConstants Basic drivetrain parameters (wheelbase,
         * track width, etc.)
         * @param updateRate How often to recalculate motor commands (Hz)
         * @param odometryStandardDeviation Trust level for wheel-based position
         * tracking
         * @param visionStandardDeviation Trust level for camera-based position
         * tracking
         * @param translationPID PID gains for autonomous X/Y movement
         * @param rotationPID PID gains for autonomous rotation
         * @param maxTranslationSpeed Speed limit for safety (m/s)
         * @param maxRotationSpeed Rotation speed limit for safety (rad/s)
         * @param frontLeftConstants Hardware configuration for front-left
         * module
         * @param frontRightConstants Hardware configuration for front-right
         * module
         * @param backLeftConstants Hardware configuration for back-left module
         * @param backRightConstants Hardware configuration for back-right
         * module
         */
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

        // === SYSTEM IDENTIFICATION (SYSID) METHODS ===
        // These help automatically tune PID controllers

        /** @brief Gets SysId routine for drive motor characterization */
        frc2::sysid::SysIdRoutine &GetSysIdTranslation()
        {
            return sysIdRoutineTranslation;
        }

        /** @brief Gets SysId routine for steer motor characterization */
        frc2::sysid::SysIdRoutine &GetSysIdSteerGains()
        {
            return sysIdRoutineSteerGains;
        }

        /** @brief Gets SysId routine for robot rotation characterization */
        frc2::sysid::SysIdRoutine &GetSysIdRotation()
        {
            return sysIdRoutineRotation;
        }

        /**
         * @brief Creates a command to run SysId characterization
         *
         * This command will systematically test the drivetrain to automatically
         * determine optimal PID controller gains. Run this in test mode only!
         *
         * @return Command that runs the SysId routine
         */
        frc2::CommandPtr GetSysIdRoutine();
        // === ADVANCED TEMPLATE METHODS ===
        // These use C++ templates to create flexible command factories

        /**
         * @brief Creates command from a function that returns a reference to a
         * request
         *
         * ## Template Explanation for Students:
         * This is a "template method" - it can work with many different types.
         * The `template <typename RequestSupplier>` means this method can
         * accept any function that returns a swerve drive request.
         *
         * ## What the `requires` clause does:
         * This is a C++20 "concept" that enforces rules:
         * - `RequestSupplier` must be a function that returns a reference (&)
         * - The returned object must work with `SetControl()`
         *
         * @param request Function that returns a reference to a drive request
         * @return Command that applies the request when run
         */
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
         * @brief Creates command from a function that returns a value (not
         * reference)
         *
         * This is similar to the above template but for functions that return
         * actual objects rather than references. The C++ compiler automatically
         * chooses which version to use based on what you pass in.
         *
         * ## Why Two Versions?
         * Sometimes you want to return a reference to an existing request
         * object, other times you want to create a new request object each
         * time. Both patterns are useful, so we support both.
         *
         * @param request Function that returns a drive request by value
         * @return Command that applies the request when run
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
        // === STANDARD SUBSYSTEM METHODS ===

        /**
         * @brief Called automatically every 20ms by the command scheduler
         *
         * This is where periodic updates happen - things that need to run
         * continuously regardless of what commands are active.
         */
        void Periodic() override;

        /**
         * @brief Adds vision-based position measurement to improve odometry
         *
         * Cameras can provide accurate position estimates by recognizing field
         * features (AprilTags, etc.). This method lets us incorporate that data
         * to improve our position tracking beyond just wheel encoders.
         *
         * @param pose Robot position as measured by vision system
         * @param timestamp When this measurement was taken
         */
        void AddVisionMeasurement(frc::Pose2d pose,
                                  units::second_t timestamp) override;

        // === AUTONOMOUS PATH FOLLOWING ===

        /**
         * @brief Follows a single sample from a Choreo trajectory
         *
         * Choreo generates smooth trajectories as a series of "samples" - each
         * specifying where the robot should be at a specific time. This method
         * commands the drivetrain to reach the target pose in that sample.
         *
         * @param sample Target pose, velocity, and timing from Choreo
         * trajectory
         */
        void FollowTrajectory(const choreo::SwerveSample &sample);

        // === SWERVE MODULE CALIBRATION ===

        /**
         * @brief Sets the steering angle offsets for all swerve modules
         *
         * Swerve modules need to know their absolute angular position. Due to
         * mechanical tolerances, each module has a different "zero" position.
         * These offsets compensate for those differences.
         *
         * @param offsets Array of 4 angle offsets [FL, FR, BL, BR]
         */
        void SetModuleOffsets(const std::array<frc::Rotation2d, 4> &offsets);

        /**
         * @brief Resets swerve module offsets and returns the calculated
         * corrections
         *
         * This is used during calibration. Point all wheels straight forward,
         * then call this method to calculate and apply the necessary offsets.
         *
         * @param targetOffsets Desired offset angles (usually all zeros)
         * @return Calculated offset corrections that were applied
         */
        std::array<frc::Rotation2d, 4> ResetModuleOffsets(
            const std::array<frc::Rotation2d, 4> &targetOffsets);

        // === MANUAL DRIVING COMMANDS ===

        /**
         * @brief Creates command for joystick-controlled driving
         *
         * This is the main driving command that reads joystick inputs and
         * translates them into robot movement. It's designed to be used as
         * the default command for the drivetrain.
         *
         * @param xAxis Function returning X-axis input (-1.0 to 1.0, positive =
         * right)
         * @param yAxis Function returning Y-axis input (-1.0 to 1.0, positive =
         * forward)
         * @param rotationAxis Function returning rotation input (-1.0 to 1.0,
         * positive = counterclockwise)
         * @param fieldRelative If true, uses field-centric control; if false,
         * robot-centric
         * @return Command that handles joystick driving
         */
        frc2::CommandPtr DriveByJoystick(std::function<double()> xAxis,
                                         std::function<double()> yAxis,
                                         std::function<double()> rotationAxis,
                                         bool fieldRelative = true);

        /**
         * @brief Logs current drivetrain state for debugging and analysis
         *
         * Records important data like:
         * - Current robot position and orientation
         * - Wheel speeds and angles
         * - Motor currents and temperatures
         * - Any error conditions
         *
         * @param log Logging context to write data to
         */
        void Log(const nfr::LogContext &log) const;
    };
}  // namespace nfr