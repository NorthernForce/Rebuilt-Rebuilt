// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/geometry/Pose2d.h>
#include <frc/geometry/Rotation2d.h>
#include <frc/geometry/Translation2d.h>
#include <units/acceleration.h>
#include <units/angle.h>
#include <units/angular_acceleration.h>
#include <units/angular_velocity.h>
#include <units/current.h>
#include <units/length.h>
#include <units/velocity.h>
#include <units/voltage.h>

namespace Constants {

// Drive Constants
namespace Drive {
  // PID Constants for PathPlanner
  constexpr double kPPDriveTP = 2.9;
  constexpr double kPPDriveTI = 0.0;
  constexpr double kPPDriveTD = 0.0;
  
  constexpr double kPPDriveRP = 5.0;
  constexpr double kPPDriveRI = 0.0;
  constexpr double kPPDriveRD = 0.0;
  
  // Drive radius and constraints
  constexpr units::length::inch_t kDriveRadius = 12.3645_in;
  constexpr units::velocity::meters_per_second_t kMaxSpeed = 4.5_mps;
  constexpr units::angular_velocity::radians_per_second_t kMaxAngularVelocity = 3.0_rad_per_s;
  
  // PathPlanner constraints
  constexpr units::velocity::meters_per_second_t kPPMaxVelocity = 3.5_mps;
  constexpr units::acceleration::meters_per_second_squared_t kPPMaxAcceleration = 3.0_mps_sq;
  constexpr units::angular_velocity::radians_per_second_t kPPMaxAngularVelocity = 3.0_rad_per_s;
  constexpr units::angular_acceleration::radians_per_second_squared_t kPPMaxAngularAcceleration = 3.0_rad_per_s_sq;
  
  // Close drive PID constants
  constexpr double kCloseDriveTP = 2.65;
  constexpr double kCloseDriveTI = 0.0;
  constexpr double kCloseDriveTD = 0.0;
  
  constexpr double kCloseDriveRP = 5.0;
  constexpr double kCloseDriveRI = 0.0;
  constexpr double kCloseDriveRD = 0.0;
  
  constexpr double kCloseDriveVP = 2.5;
  constexpr double kCloseDriveVI = 0.0;
  constexpr double kCloseDriveVD = 0.1;
  
  // Placing offset for reef scoring
  const frc::Translation2d kPlacingOffset{3.5_in, -10.75_in};
}

// Controller Constants
namespace Controller {
  constexpr int kDriverControllerPort = 0;
  constexpr int kOperatorControllerPort = 1;
  constexpr double kJoystickDeadband = 0.1;
}

// Manipulator Constants
namespace Manipulator {
  constexpr int kMotorId = 19;
  constexpr bool kMotorInverted = true;
  constexpr int kSensorId = 2;
  
  constexpr double kIntakeSpeed = 0.6;
  constexpr double kOuttakeSpeed = 0.89;
  constexpr double kSlowOuttakeSpeed = 0.4;
  constexpr double kPurgeSpeed = 0.2;
}

// Climber Constants
namespace Climber {
  constexpr int kMotorId = 17;
  constexpr bool kInverted = false;
  constexpr int kEncoderId = 23;
  
  constexpr units::angle::turn_t kLowerLimit = -0.03_tr;
  constexpr units::angle::turn_t kUpperLimit = 0.22_tr;
  constexpr units::angle::degree_t kSweetAngle = 70.0_deg;
  constexpr double kClimbSpeed = 1.0;
}

// Elevator Constants
namespace Elevator {
  constexpr double kHomingSpeed = 0.25;
  constexpr units::length::inch_t kTolerance = 0.5_in;
  
  // Inner Elevator
  namespace Inner {
    constexpr double kGearBoxRatio = 12.0;
    constexpr double kSprocketTeeth = 16.0;
    constexpr units::length::inch_t kSprocketPitch = 0.25_in;
    constexpr units::length::inch_t kSprocketCircumference = kSprocketPitch * kSprocketTeeth;
    
    // Motor control constants
    constexpr double kS = 0.017384;
    constexpr double kV = 28.59;
    constexpr double kA = 0.015;
    constexpr double kP = 18.0;
    constexpr double kI = 0.0;
    constexpr double kD = 0.0;
    constexpr double kG = 0.21;
    
    constexpr units::length::inch_t kLowerLimit = 0.0_in;
    constexpr units::length::inch_t kUpperLimit = 24.8_in;
  }
  
  // Outer Elevator
  namespace Outer {
    constexpr double kGearBoxRatio = 16.0;
    constexpr double kSprocketTeeth = 22.0;
    constexpr units::length::inch_t kSprocketPitch = 0.25_in;
    constexpr units::length::inch_t kSprocketCircumference = kSprocketPitch * kSprocketTeeth;
    
    // Motor control constants
    constexpr double kS = 0.052289;
    constexpr double kV = 19.868;
    constexpr double kA = 0.015;
    constexpr double kP = 18.0;
    constexpr double kI = 0.0;
    constexpr double kD = 0.0;
    constexpr double kG = 0.31;
    
    constexpr units::length::inch_t kLowerLimit = 0.0_in;
    constexpr units::length::inch_t kUpperLimit = 26.8_in;
  }
}

// Superstructure Goal Heights
enum class SuperstructureGoal {
  L1,
  L2,
  L3,
  L4,
  CORAL_STATION,
  START
};

// Algae Extractor Constants
namespace AlgaeExtractor {
  constexpr int kMotorId = 18;
  constexpr int kSensorId = 3;
  constexpr bool kInverted = false;
  constexpr double kGearRatio = 70.0;
  
  constexpr double kRemovingSpeed = 0.8;
  constexpr double kReturningSpeed = 0.2;
}

// LED Constants
namespace LED {
  constexpr int kCANId = 30;
  constexpr int kLEDCount = 72;
  constexpr double kLEDRate = 0.1;
}

// Vision Constants
namespace Vision {
  constexpr const char* kLimelightName = "limelight";
}

} // namespace Constants