/**
 * @file LogTypes.h
 * @brief Template specializations for logging common FRC data types
 * 
 * This file contains Log() function specializations that teach our logging
 * system how to record common FRC data types like poses, speeds, units, etc.
 * 
 * ## How This Works:
 * When you write `log["pose"] << robotPose`, the logging system needs to know
 * how to convert a Pose2d object into loggable data. These functions provide
 * that conversion for many common types.
 * 
 * ## For Students:
 * You don't usually need to modify this file - it's already set up to handle
 * most data types you'll encounter in FRC programming.
 */

#include <frc/geometry/Pose2d.h>
#include <frc/geometry/Pose3d.h>
#include <frc/geometry/Transform2d.h>
#include <frc/geometry/Transform3d.h>
#include <frc/kinematics/ChassisSpeeds.h>
#include <frc/kinematics/SwerveModulePosition.h>
#include <frc/kinematics/SwerveModuleState.h>
#include <logging/Logger.h>
#include <units/acceleration.h>
#include <units/current.h>
#include <units/frequency.h>
#include <units/temperature.h>
#include <units/voltage.h>

namespace nfr
{
    // === UNITS SYSTEM LOGGING ===
    // These concepts and functions handle WPILib's type-safe units system
    
    /**
     * @brief Concept that identifies length units (meters, inches, feet, etc.)
     * 
     * C++20 concepts are like "type requirements" - this one ensures we only
     * accept types that represent length measurements.
     */
    template <typename T>
    concept IsConvertableToMeter = units::traits::is_length_unit_v<T>;

    /**
     * @brief Logs any length unit by converting to meters
     * 
     * This function handles logging of distances - whether they're in meters,
     * inches, feet, etc., they all get logged as meter values for consistency.
     */
    template <IsConvertableToMeter T>
    inline void Log(const LogContext &logContext, const T &value)
    {
        auto meters = static_cast<units::meter_t>(value);
        logContext << meters.value();
    }

    /**
     * @brief Concept that identifies angle units (degrees, radians, rotations, etc.)
     */
    template <typename T>
    concept IsConvertableToDegree = units::traits::is_angle_unit_v<T>;

    /**
     * @brief Logs any angle unit by converting to degrees
     * 
     * Angles get logged as degrees since they're more intuitive for humans
     * to read than radians (90° vs 1.57 rad).
     */
    template <IsConvertableToDegree T>
    inline void Log(const LogContext &logContext, const T &value)
    {
        auto degrees = static_cast<units::degree_t>(value);
        logContext << degrees.value();
    }

    // === GEOMETRY TYPES ===
    // Functions for logging robot positions, orientations, and movements
    
    /** @brief Logs a 2D rotation as degrees */
    inline void Log(const LogContext &logContext, const frc::Rotation2d &value)
    {
        logContext << value.Degrees();
    }

    /** @brief Logs a 2D pose (position + orientation) with named fields */
    inline void Log(const LogContext &logContext, const frc::Pose2d &pose)
    {
        logContext["x"] << pose.X();          // X position on field
        logContext["y"] << pose.Y();          // Y position on field  
        logContext["rotation"] << pose.Rotation().Degrees(); // Which way robot is facing
    }

    inline void Log(const LogContext &logContext,
                    const frc::Translation2d &translation)
    {
        logContext["x"] << translation.X();
        logContext["y"] << translation.Y();
    }

    inline void Log(const LogContext &logContext, const frc::Rotation3d &value)
    {
        logContext["roll"] << value.X();
        logContext["pitch"] << value.Y();
        logContext["yaw"] << value.Z();
    }

    inline void Log(const LogContext &logContext,
                    const frc::Translation3d &translation)
    {
        logContext["x"] << translation.X();
        logContext["y"] << translation.Y();
        logContext["z"] << translation.Z();
    }

    inline void Log(const LogContext &logContext, const frc::Pose3d &pose)
    {
        logContext["x"] << pose.X();
        logContext["y"] << pose.Y();
        logContext["z"] << pose.Z();
        logContext["rotation"] << pose.Rotation();
    }

    template <typename T>
    concept IsConvertableToMetersPerSecond =
        units::traits::is_velocity_unit_v<T>;

    template <IsConvertableToMetersPerSecond T>
    inline void Log(const LogContext &logContext, const T &value)
    {
        auto metersPerSecond = static_cast<units::meters_per_second_t>(value);
        logContext << metersPerSecond.value();
    }

    template <typename T>
    concept IsConvertableToRadiansPerSecond =
        units::traits::is_angular_velocity_unit_v<T>;

    template <IsConvertableToRadiansPerSecond T>
    inline void Log(const LogContext &logContext, const T &value)
    {
        auto radiansPerSecond = static_cast<units::radians_per_second_t>(value);
        logContext << radiansPerSecond.value();
    }

    inline void Log(const LogContext &logContext,
                    const frc::ChassisSpeeds &speeds)
    {
        logContext["vx"] << speeds.vx;
        logContext["vy"] << speeds.vy;
        logContext["omega"] << speeds.omega;
    }

    inline void Log(const LogContext &logContext,
                    const frc::SwerveModuleState &state)
    {
        logContext["angle"] << state.angle;
        logContext["speed"] << state.speed;
    }

    inline void Log(const LogContext &logContext,
                    const frc::SwerveModulePosition &position)
    {
        logContext["angle"] << position.angle;
        logContext["distance"] << position.distance;
    }

    inline void Log(const LogContext &logContext,
                    const frc::Transform2d &transform)
    {
        logContext["translation"] << transform.Translation();
        logContext["rotation"] << transform.Rotation();
    }

    inline void Log(const LogContext &logContext, const frc::Twist2d &twist)
    {
        logContext["dx"] << twist.dx;
        logContext["dy"] << twist.dy;
        logContext["dtheta"] << twist.dtheta;
    }

    inline void Log(const LogContext &logContext,
                    const frc::Transform3d &transform)
    {
        logContext["translation"] << transform.Translation();
        logContext["rotation"] << transform.Rotation();
    }

    inline void Log(const LogContext &logContext, const frc::Twist3d &twist)
    {
        logContext["dx"] << twist.dx;
        logContext["dy"] << twist.dy;
        logContext["dz"] << twist.dz;
        logContext["droll"] << twist.rx;
        logContext["dpitch"] << twist.ry;
        logContext["dyaw"] << twist.rz;
    }

    template <typename T>
    concept IsConvertableToCelsius = units::traits::is_temperature_unit_v<T>;

    template <IsConvertableToCelsius T>
    inline void Log(const LogContext &logContext, const T &value)
    {
        auto celsius = static_cast<units::celsius_t>(value);
        logContext << celsius.value();
    }

    template <typename T>
    concept IsConvertableToVolt = units::traits::is_voltage_unit_v<T>;

    template <IsConvertableToVolt T>
    inline void Log(const LogContext &logContext, const T &value)
    {
        auto volts = static_cast<units::volt_t>(value);
        logContext << volts.value();
    }

    template <typename T>
    concept IsConvertableToAmpere = units::traits::is_current_unit_v<T>;

    template <IsConvertableToAmpere T>
    inline void Log(const LogContext &logContext, const T &value)
    {
        auto amperes = static_cast<units::ampere_t>(value);
        logContext << amperes.value();
    }

    template <typename T>
    concept IsConvertableToMetersPerSecondSquared =
        units::traits::is_acceleration_unit_v<T>;

    template <IsConvertableToMetersPerSecondSquared T>
    inline void Log(const LogContext &logContext, const T &value)
    {
        auto metersPerSecondSquared =
            static_cast<units::meters_per_second_squared_t>(value);
        logContext << metersPerSecondSquared.value();
    }

    template <typename T>
    concept IsConvertableToSeconds = units::traits::is_time_unit_v<T>;
    template <IsConvertableToSeconds T>
    inline void Log(const LogContext &logContext, const T &value)
    {
        auto seconds = static_cast<units::second_t>(value);
        logContext << seconds.value();
    }

    template <typename T>
    concept IsConvertableToFrequency = units::traits::is_frequency_unit_v<T>;
    template <IsConvertableToFrequency T>
    inline void Log(const LogContext &logContext, const T &value)
    {
        auto hertz = static_cast<units::hertz_t>(value);
        logContext << hertz.value();
    }
}  // namespace nfr