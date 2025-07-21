#include <logging/Logger.h>

#include <frc/geometry/Pose2d.h>
#include <type_traits>
#include <units/acceleration.h>
#include <units/voltage.h>
#include <frc/geometry/Pose3d.h>
#include <frc/kinematics/ChassisSpeeds.h>
#include <frc/kinematics/SwerveModulePosition.h>
#include <frc/kinematics/SwerveModuleState.h>
#include <frc/geometry/Transform2d.h>
#include <frc/geometry/Transform3d.h>
#include <units/temperature.h>
#include <units/voltage.h>
#include <units/current.h>
#include <units/acceleration.h>

namespace nfr
{
    template <typename T>
    concept IsConvertableToMeter = units::traits::is_length_unit_v<T>;

    template <IsConvertableToMeter T>
    void Log(const LogContext &logContext, const T &value)
    {
        auto meters = static_cast<units::meter_t>(value);
        logContext << meters.value();
    }

    template <typename T>
    concept IsConvertableToDegree = units::traits::is_angle_unit_v<T>;

    template <IsConvertableToDegree T>
    void Log(const LogContext &logContext, const T &value)
    {
        auto degrees = static_cast<units::degree_t>(value);
        logContext << degrees.value();
    }

    template <>
    void Log(const LogContext &logContext, const frc::Rotation2d &value)
    {
        logContext << value.Degrees();
    }

    template <>
    void Log(const LogContext &logContext, const frc::Pose2d &pose)
    {
        logContext["x"] << pose.X();
        logContext["y"] << pose.Y();
        logContext["rotation"] << pose.Rotation().Degrees();
    }

    template <>
    void Log(const LogContext &logContext, const frc::Translation2d &translation)
    {
        logContext["x"] << translation.X();
        logContext["y"] << translation.Y();
    }

    template <>
    void Log(const LogContext &logContext, const frc::Rotation3d &value)
    {
        logContext["roll"] << value.X();
        logContext["pitch"] << value.Y();
        logContext["yaw"] << value.Z();
    }

    template <>
    void Log(const LogContext &logContext, const frc::Translation3d &translation)
    {
        logContext["x"] << translation.X();
        logContext["y"] << translation.Y();
        logContext["z"] << translation.Z();
    }

    template <>
    void Log(const LogContext &logContext, const frc::Pose3d &pose)
    {
        logContext["x"] << pose.X();
        logContext["y"] << pose.Y();
        logContext["z"] << pose.Z();
        logContext["rotation"] << pose.Rotation();
    }


    template <typename T>
    concept IsConvertableToMetersPerSecond = units::traits::is_velocity_unit_v<T>;

    template <IsConvertableToMetersPerSecond T>
    void Log(const LogContext &logContext, const T &value)
    {
        auto metersPerSecond = static_cast<units::meters_per_second_t>(value);
        logContext << metersPerSecond.value();
    }

    template <typename T>
    concept IsConvertableToRadiansPerSecond = units::traits::is_angular_velocity_unit_v<T>;

    template <IsConvertableToRadiansPerSecond T>
    void Log(const LogContext &logContext, const T &value)
    {
        auto radiansPerSecond = static_cast<units::radians_per_second_t>(value);
        logContext << radiansPerSecond.value();
    }

    template <>
    void Log(const LogContext &logContext, const frc::ChassisSpeeds &speeds)
    {
        logContext["vx"] << speeds.vx;
        logContext["vy"] << speeds.vy;
        logContext["omega"] << speeds.omega;
    }


    template <>
    void Log(const LogContext &logContext, const frc::SwerveModuleState &state)
    {
        logContext["angle"] << state.angle;
        logContext["speed"] << state.speed;
    }


    template <>
    void Log(const LogContext &logContext, const frc::SwerveModulePosition &position)
    {
        logContext["angle"] << position.angle;
        logContext["distance"] << position.distance;
    }


    template <>
    void Log(const LogContext &logContext, const frc::Transform2d &transform)
    {
        logContext["translation"] << transform.Translation();
        logContext["rotation"] << transform.Rotation();
    }

    template <>
    void Log(const LogContext &logContext, const frc::Twist2d &twist)
    {
        logContext["dx"] << twist.dx;
        logContext["dy"] << twist.dy;
        logContext["dtheta"] << twist.dtheta;
    }


    template <>
    void Log(const LogContext &logContext, const frc::Transform3d &transform)
    {
        logContext["translation"] << transform.Translation();
        logContext["rotation"] << transform.Rotation();
    }

    template <>
    void Log(const LogContext &logContext, const frc::Twist3d &twist)
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
    void Log(const LogContext &logContext, const T &value)
    {
        auto celsius = static_cast<units::celsius_t>(value);
        logContext << celsius.value();
    }


    template <typename T>
    concept IsConvertableToVolt = units::traits::is_voltage_unit_v<T>;

    template <IsConvertableToVolt T>
    void Log(const LogContext &logContext, const units::volt_t &value)
    {
        auto volts = static_cast<units::volt_t>(value);
        logContext << volts.value();
    }


    template <typename T>
    concept IsConvertableToAmpere = units::traits::is_current_unit_v<T>;

    template <IsConvertableToAmpere T>
    void Log(const LogContext &logContext, const units::ampere_t &value)
    {
        auto amperes = static_cast<units::ampere_t>(value);
        logContext << amperes.value();
    }


    template <typename T>
    concept IsConvertableToMetersPerSecondSquared = units::traits::is_acceleration_unit_v<T>;

    template <IsConvertableToMetersPerSecondSquared T>
    void Log(const LogContext &logContext, const units::meters_per_second_squared_t &value)
    {
        auto metersPerSecondSquared = static_cast<units::meters_per_second_squared_t>(value);
        logContext << metersPerSecondSquared.value();
    }
}