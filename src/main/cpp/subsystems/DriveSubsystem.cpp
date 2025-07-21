// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystems/DriveSubsystem.h"

#include <frc2/command/Commands.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Timer.h>
#include <numbers>

#include "Constants.h"
#include "FieldConstants.h"

DriveSubsystem::DriveSubsystem() {
  SetName("DriveSubsystem");
  
  // Initialize field display
  frc::SmartDashboard::PutData("Field", &m_field);
  
  m_lastTime = frc::Timer::GetFPGATimestamp();
}

void DriveSubsystem::Periodic() {
  // Update simulated robot pose based on current chassis speeds
  auto currentTime = frc::Timer::GetFPGATimestamp();
  auto dt = currentTime - m_lastTime;
  m_lastTime = currentTime;
  
  if (dt > 0_s) {
    // Simple integration for simulation
    auto dx = m_chassisSpeeds.vx * dt;
    auto dy = m_chassisSpeeds.vy * dt;
    auto dtheta = m_chassisSpeeds.omega * dt;
    
    // Update pose
    auto currentPose = GetPose();
    auto newTranslation = currentPose.Translation() + 
                         frc::Translation2d{dx, dy}.RotateBy(currentPose.Rotation());
    auto newRotation = currentPose.Rotation() + frc::Rotation2d{dtheta};
    
    m_pose = frc::Pose2d{newTranslation, newRotation};
  }
  
  // Update field display
  m_field.SetRobotPose(m_pose);
  
  // Put pose data on dashboard
  frc::SmartDashboard::PutNumber("Robot X", m_pose.X().value());
  frc::SmartDashboard::PutNumber("Robot Y", m_pose.Y().value());  
  frc::SmartDashboard::PutNumber("Robot Rotation", m_pose.Rotation().Degrees().value());
}

frc2::CommandPtr DriveSubsystem::DriveByJoystick(
    std::function<double()> xSpeed,
    std::function<double()> ySpeed,
    std::function<double()> rotSpeed) {
  
  return frc2::cmd::Run([this, xSpeed, ySpeed, rotSpeed] {
    Drive(
      units::meters_per_second_t{xSpeed() * Constants::Drive::kMaxSpeed.value()},
      units::meters_per_second_t{ySpeed() * Constants::Drive::kMaxSpeed.value()},
      units::radians_per_second_t{rotSpeed() * Constants::Drive::kMaxAngularVelocity.value()}
    );
  }, {this});
}

frc2::CommandPtr DriveSubsystem::DriveToPose(const frc::Pose2d& targetPose) {
  return frc2::cmd::Run([this, targetPose] {
    // Simple proportional controller for autonomous driving
    auto currentPose = GetPose();
    auto error = targetPose - currentPose;
    
    // Distance and angle errors
    auto distanceError = error.Translation().Norm();
    auto angleError = (targetPose.Rotation() - currentPose.Rotation()).Radians();
    
    // Simple P control
    const double kP_trans = 2.0;
    const double kP_rot = 1.0;
    
    if (distanceError > 0.1_m || units::math::abs(angleError) > 0.1_rad) {
      auto xSpeed = kP_trans * error.X().value();
      auto ySpeed = kP_trans * error.Y().value();
      auto rotSpeed = kP_rot * angleError.value();
      
      // Limit speeds
      xSpeed = std::clamp(xSpeed, -2.0, 2.0);
      ySpeed = std::clamp(ySpeed, -2.0, 2.0);  
      rotSpeed = std::clamp(rotSpeed, -2.0, 2.0);
      
      Drive(units::meters_per_second_t{xSpeed}, 
            units::meters_per_second_t{ySpeed},
            units::radians_per_second_t{rotSpeed}, false);
    } else {
      Drive(0_mps, 0_mps, 0_rad_per_s);
    }
  }, {this});
}

frc2::CommandPtr DriveSubsystem::DriveToNearestReef() {
  return frc2::cmd::RunOnce([this] {
    // Find nearest reef position (simplified)
    auto currentPose = GetPose();
    frc::Pose2d nearestReef = FieldConstants::ReefPositions::kA; // Default
    
    // In a real implementation, this would calculate the actual nearest position
    // For now, just go to position A
    DriveToPose(nearestReef).Schedule();
  }, {});
}

frc2::CommandPtr DriveSubsystem::StrafeLeft(double speed) {
  return frc2::cmd::Run([this, speed] {
    Drive(0_mps, units::meters_per_second_t{speed}, 0_rad_per_s);
  }, {this});
}

frc2::CommandPtr DriveSubsystem::StrafeRight(double speed) {
  return frc2::cmd::Run([this, speed] {
    Drive(0_mps, units::meters_per_second_t{-speed}, 0_rad_per_s);
  }, {this});
}

frc2::CommandPtr DriveSubsystem::GoForward(double speed) {
  return frc2::cmd::Run([this, speed] {
    Drive(units::meters_per_second_t{speed}, 0_mps, 0_rad_per_s);
  }, {this});
}

frc2::CommandPtr DriveSubsystem::GoBackward(double speed) {
  return frc2::cmd::Run([this, speed] {
    Drive(units::meters_per_second_t{-speed}, 0_mps, 0_rad_per_s);
  }, {this});
}

frc2::CommandPtr DriveSubsystem::Stop() {
  return frc2::cmd::RunOnce([this] {
    Drive(0_mps, 0_mps, 0_rad_per_s);
  }, {this});
}

frc2::CommandPtr DriveSubsystem::ResetOrientation() {
  return frc2::cmd::RunOnce([this] {
    m_pose = frc::Pose2d{m_pose.Translation(), frc::Rotation2d{0_deg}};
  }, {});
}

void DriveSubsystem::ResetPose(const frc::Pose2d& pose) {
  m_pose = pose;
}

frc::Pose2d DriveSubsystem::GetPose() const {
  return m_pose;
}

void DriveSubsystem::AddVisionMeasurement(const frc::Pose2d& visionPose, units::second_t timestamp) {
  // In a real implementation, this would integrate vision measurements
  // with odometry using a Kalman filter or similar
  // For simulation, we can just log it
  frc::SmartDashboard::PutString("Vision Pose", 
    fmt::format("({:.2f}, {:.2f}, {:.1f}°)", 
                visionPose.X().value(), 
                visionPose.Y().value(), 
                visionPose.Rotation().Degrees().value()));
}

void DriveSubsystem::Drive(units::meters_per_second_t xSpeed,
                          units::meters_per_second_t ySpeed,
                          units::radians_per_second_t rotSpeed,
                          bool fieldRelative) {
  
  if (fieldRelative) {
    // Convert field relative speeds to robot relative
    m_chassisSpeeds = frc::ChassisSpeeds::FromFieldRelativeSpeeds(
      xSpeed, ySpeed, rotSpeed, m_pose.Rotation());
  } else {
    m_chassisSpeeds = frc::ChassisSpeeds{xSpeed, ySpeed, rotSpeed};
  }
  
  // In a real robot, this would command the motors
  // For simulation, we just store the speeds for periodic update
}