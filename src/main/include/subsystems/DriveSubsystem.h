// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/CommandPtr.h>
#include <frc/geometry/Pose2d.h>
#include <frc/geometry/Rotation2d.h>
#include <frc/kinematics/ChassisSpeeds.h>
#include <frc/smartdashboard/Field2d.h>
#include <units/velocity.h>
#include <units/angular_velocity.h>
#include <functional>

/**
 * Basic drivetrain subsystem for autonomous movement and teleop control.
 * This is a simplified implementation - a full swerve drive would have
 * more complex motor controllers and sensors.
 */
class DriveSubsystem : public frc2::SubsystemBase {
 public:
  DriveSubsystem();

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;

  /**
   * Drive using joystick inputs (for teleop)
   */
  frc2::CommandPtr DriveByJoystick(
    std::function<double()> xSpeed,
    std::function<double()> ySpeed, 
    std::function<double()> rotSpeed
  );

  /**
   * Drive to a specific pose autonomously
   */
  frc2::CommandPtr DriveToPose(const frc::Pose2d& targetPose);
  
  /**
   * Drive to the nearest reef position
   */
  frc2::CommandPtr DriveToNearestReef();
  
  /**
   * Strafe left at given speed
   */
  frc2::CommandPtr StrafeLeft(double speed);
  
  /**
   * Strafe right at given speed  
   */
  frc2::CommandPtr StrafeRight(double speed);
  
  /**
   * Move forward at given speed
   */
  frc2::CommandPtr GoForward(double speed);
  
  /**
   * Move backward at given speed
   */
  frc2::CommandPtr GoBackward(double speed);
  
  /**
   * Stop the robot
   */
  frc2::CommandPtr Stop();

  /**
   * Reset the robot's orientation
   */
  frc2::CommandPtr ResetOrientation();
  
  /**
   * Reset the robot's pose to a given position
   */
  void ResetPose(const frc::Pose2d& pose);

  /**
   * Get the current robot pose
   */
  frc::Pose2d GetPose() const;

  /**
   * Add a vision measurement for pose estimation
   */
  void AddVisionMeasurement(const frc::Pose2d& visionPose, units::second_t timestamp);

 private:
  void Drive(units::meters_per_second_t xSpeed, 
             units::meters_per_second_t ySpeed,
             units::radians_per_second_t rotSpeed,
             bool fieldRelative = true);

  // Simulated robot pose
  frc::Pose2d m_pose{0_m, 0_m, frc::Rotation2d{0_deg}};
  
  // Field display for dashboard
  frc::Field2d m_field;
  
  // Current speeds for simulation
  frc::ChassisSpeeds m_chassisSpeeds;
  
  // Simulation time tracking
  units::second_t m_lastTime{0_s};
};