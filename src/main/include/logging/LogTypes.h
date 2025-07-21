#include <logging/Logger.h>

#include <frc/geometry/Pose2d.h>

template<>
void nfr::Log(const nfr::LogContext& logContext, const units::meter_t& value) {
    logContext << value.value();
}

template<>
void nfr::Log(const nfr::LogContext& logContext, const units::degree_t& value) {
    logContext << value.value();
}

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::Rotation2d& value) {
    logContext << value.Degrees();
}

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::Pose2d& pose) {
    logContext["x"] << pose.X();
    logContext["y"] << pose.Y();
    logContext["rotation"] << pose.Rotation().Degrees();
}

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::Translation2d& translation) {
    logContext["x"] << translation.X();
    logContext["y"] << translation.Y();
}

#include <frc/geometry/Pose3d.h>

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::Rotation3d& value) {
    logContext["roll"] << value.X();
    logContext["pitch"] << value.Y();
    logContext["yaw"] << value.Z();
}

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::Translation3d& translation) {
    logContext["x"] << translation.X();
    logContext["y"] << translation.Y();
    logContext["z"] << translation.Z();
}

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::Pose3d& pose) {
    logContext["x"] << pose.X();
    logContext["y"] << pose.Y();
    logContext["z"] << pose.Z();
    logContext["rotation"] << pose.Rotation();
}

#include <frc/kinematics/ChassisSpeeds.h>

template<>
void nfr::Log(const nfr::LogContext& logContext, const units::meters_per_second_t& value) {
    logContext << value.value();
}

template<>
void nfr::Log(const nfr::LogContext& logContext, const units::radians_per_second_t& value) {
    logContext << value.value();
}

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::ChassisSpeeds& speeds) {
    logContext["vx"] << speeds.vx;
    logContext["vy"] << speeds.vy;
    logContext["omega"] << speeds.omega;
}

#include <frc/kinematics/SwerveModuleState.h>

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::SwerveModuleState& state) {
    logContext["angle"] << state.angle;
    logContext["speed"] << state.speed;
}

#include <frc/kinematics/SwerveModulePosition.h>

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::SwerveModulePosition& position) {
    logContext["angle"] << position.angle;
    logContext["distance"] << position.distance;
}

#include <frc/geometry/Transform2d.h>

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::Transform2d& transform) {
    logContext["translation"] << transform.Translation();
    logContext["rotation"] << transform.Rotation();
}

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::Twist2d& twist) {
    logContext["dx"] << twist.dx;
    logContext["dy"] << twist.dy;
    logContext["dtheta"] << twist.dtheta;
}

#include <frc/geometry/Transform3d.h>

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::Transform3d& transform) {
    logContext["translation"] << transform.Translation();
    logContext["rotation"] << transform.Rotation();
}

template<>
void nfr::Log(const nfr::LogContext& logContext, const frc::Twist3d& twist) {
    logContext["dx"] << twist.dx;
    logContext["dy"] << twist.dy;
    logContext["dz"] << twist.dz;
    logContext["droll"] << twist.rx;
    logContext["dpitch"] << twist.ry;
    logContext["dyaw"] << twist.rz;
}

#include <units/temperature.h>

template<>
void nfr::Log(const nfr::LogContext& logContext, const units::celsius_t& value) {
    logContext << value.value();
}

#include <units/voltage.h>

template<>
void nfr::Log(const nfr::LogContext& logContext, const units::volt_t& value) {
    logContext << value.value();
}

#include <units/current.h>

template<>
void nfr::Log(const nfr::LogContext& logContext, const units::ampere_t& value) {
    logContext << value.value();
}

#include <units/acceleration.h>

template<>
void nfr::Log(const nfr::LogContext& logContext, const units::meters_per_second_squared_t& value) {
    logContext << value.value();
}