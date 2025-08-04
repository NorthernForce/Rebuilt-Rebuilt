#pragma once

#include <tuning/Tunable.h>
#include <units/angular_velocity.h>
#include <units/frequency.h>
#include <units/velocity.h>

namespace nfr
{

/**
 * @brief Tunable constants for the drive subsystem that can be adjusted via
 * NetworkTables
 *
 * These constants demonstrate how to use the tuning system for runtime
 * parameter adjustment. Users can modify these values through NetworkTables
 * clients like ShuffleBoard or custom dashboards.
 */
class TunableDriveConstants
{
  public:
    // Tunable drive parameters - these can be adjusted at runtime via
    // NetworkTables
    static inline TunableDouble kUpdateRateHz =
        TunableDouble("Drive/UpdateRateHz", 200.0);
    static inline Tunable<units::meters_per_second_t> kMaxTranslationSpeed =
        Tunable<units::meters_per_second_t>("Drive/MaxTranslationSpeed",
                                            3.0_mps);
    static inline Tunable<units::radians_per_second_t> kMaxRotationSpeed =
        Tunable<units::radians_per_second_t>("Drive/MaxRotationSpeed",
                                             10.0_rad_per_s);

    // PID tuning constants
    static inline TunableDouble kTranslationP =
        TunableDouble("Drive/Translation/P", 0.5);
    static inline TunableDouble kTranslationI =
        TunableDouble("Drive/Translation/I", 0.0);
    static inline TunableDouble kTranslationD =
        TunableDouble("Drive/Translation/D", 0.0);

    static inline TunableDouble kRotationP =
        TunableDouble("Drive/Rotation/P", 0.1);
    static inline TunableDouble kRotationI =
        TunableDouble("Drive/Rotation/I", 0.0);
    static inline TunableDouble kRotationD =
        TunableDouble("Drive/Rotation/D", 0.0);

    // Odometry and vision standard deviations
    static inline TunableDouble kOdometryStdDevX =
        TunableDouble("Drive/OdometryStdDev/X", 0.01);
    static inline TunableDouble kOdometryStdDevY =
        TunableDouble("Drive/OdometryStdDev/Y", 0.01);
    static inline TunableDouble kOdometryStdDevTheta =
        TunableDouble("Drive/OdometryStdDev/Theta", 0.01);

    static inline TunableDouble kVisionStdDevX =
        TunableDouble("Drive/VisionStdDev/X", 0.1);
    static inline TunableDouble kVisionStdDevY =
        TunableDouble("Drive/VisionStdDev/Y", 0.1);
    static inline TunableDouble kVisionStdDevTheta =
        TunableDouble("Drive/VisionStdDev/Theta", 9999999.0);

    /**
     * @brief Update all tunable constants from NetworkTables
     *
     * Call this method periodically (e.g., in Robot::RobotPeriodic()) to keep
     * the constants synchronized with NetworkTables values.
     */
    static void UpdateFromNetworkTables()
    {
        kUpdateRateHz.Update();
        kMaxTranslationSpeed.Update();
        kMaxRotationSpeed.Update();
        kTranslationP.Update();
        kTranslationI.Update();
        kTranslationD.Update();
        kRotationP.Update();
        kRotationI.Update();
        kRotationD.Update();
        kOdometryStdDevX.Update();
        kOdometryStdDevY.Update();
        kOdometryStdDevTheta.Update();
        kVisionStdDevX.Update();
        kVisionStdDevY.Update();
        kVisionStdDevTheta.Update();
    }

    /**
     * @brief Get odometry standard deviation as array for WPILib usage
     */
    static std::array<double, 3> GetOdometryStandardDeviation()
    {
        return {kOdometryStdDevX, kOdometryStdDevY, kOdometryStdDevTheta};
    }

    /**
     * @brief Get vision standard deviation as array for WPILib usage
     */
    static std::array<double, 3> GetVisionStandardDeviation()
    {
        return {kVisionStdDevX, kVisionStdDevY, kVisionStdDevTheta};
    }
};

/**
 * @brief Example showing how to use tunables in other subsystems
 */
class TunableExampleConstants
{
  public:
    // Example motor control parameters
    static inline TunableDouble kMotorSpeed =
        TunableDouble("Example/MotorSpeed", 0.5);
    static inline TunableBool kMotorEnabled =
        TunableBool("Example/MotorEnabled", true);
    static inline TunableInt kEncoderCPR =
        TunableInt("Example/EncoderCPR", 2048);

    // Example with units
    static inline Tunable<units::meter_t> kArmLength =
        Tunable<units::meter_t>("Example/ArmLength", 0.8_m);
    static inline Tunable<units::degree_t> kArmAngle =
        Tunable<units::degree_t>("Example/ArmAngle", 45.0_deg);

    static void UpdateFromNetworkTables()
    {
        kMotorSpeed.Update();
        kMotorEnabled.Update();
        kEncoderCPR.Update();
        kArmLength.Update();
        kArmAngle.Update();
    }
};

}  // namespace nfr