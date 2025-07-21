// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <array>
#include <units/length.h>
#include <frc/geometry/Pose2d.h>
#include <frc/geometry/Rotation2d.h>
#include <frc/geometry/Translation2d.h>
#include <frc/DriverStation.h>

namespace FieldConstants {

// Field dimensions
constexpr units::length::meter_t kFieldLength = 17.55_m;
constexpr units::length::meter_t kFieldWidth = 8.05_m;

// Reef center position
const frc::Translation2d kReefCenter{4.489323_m, 4.0259_m};

// Reef rotations (in degrees)
constexpr units::angle::degree_t kABRotation = 0.0_deg;
constexpr units::angle::degree_t kCDRotation = 60.0_deg;
constexpr units::angle::degree_t kEFRotation = 120.0_deg;
constexpr units::angle::degree_t kGHRotation = 180.0_deg;
constexpr units::angle::degree_t kIJRotation = 240.0_deg;
constexpr units::angle::degree_t kKLRotation = 300.0_deg;

// Reef positions (Blue alliance relative)
namespace ReefPositions {
  // Coral scoring positions
  const frc::Pose2d kA{3.15_m, 4.18_m, frc::Rotation2d{kABRotation}};
  const frc::Pose2d kB{3.15_m, 3.85_m, frc::Rotation2d{kABRotation}};
  const frc::Pose2d kC{3.69_m, 2.96_m, frc::Rotation2d{kCDRotation}};
  const frc::Pose2d kD{3.94_m, 2.79_m, frc::Rotation2d{kCDRotation}};
  const frc::Pose2d kE{5.01_m, 2.81_m, frc::Rotation2d{kEFRotation}};
  const frc::Pose2d kF{5.29_m, 2.96_m, frc::Rotation2d{kEFRotation}};
  const frc::Pose2d kG{5.85_m, 3.87_m, frc::Rotation2d{kGHRotation}};
  const frc::Pose2d kH{5.85_m, 4.18_m, frc::Rotation2d{kGHRotation}};
  const frc::Pose2d kI{5.29_m, 5.12_m, frc::Rotation2d{kIJRotation}};
  const frc::Pose2d kJ{5.01_m, 5.29_m, frc::Rotation2d{kIJRotation}};
  const frc::Pose2d kK{3.95_m, 5.29_m, frc::Rotation2d{kKLRotation}};
  const frc::Pose2d kL{3.65_m, 5.12_m, frc::Rotation2d{kKLRotation}};
  
  // Algae positions
  const frc::Pose2d kABAlgae{3.15_m, 4.02_m, frc::Rotation2d{kABRotation}};
  const frc::Pose2d kCDAlgae{3.79_m, 2.86_m, frc::Rotation2d{kCDRotation}};
  const frc::Pose2d kEFAlgae{5.14_m, 2.87_m, frc::Rotation2d{kEFRotation}};
  const frc::Pose2d kGHAlgae{5.85_m, 4.02_m, frc::Rotation2d{kGHRotation}};
  const frc::Pose2d kIJAlgae{5.14_m, 5.19_m, frc::Rotation2d{kIJRotation}};
  const frc::Pose2d kKLAlgae{3.82_m, 5.19_m, frc::Rotation2d{kKLRotation}};
}

// Coral station positions (Blue alliance relative)
namespace CoralStations {
  const frc::Pose2d kLeft{1.18_m, 7.07_m, frc::Rotation2d{307.5_deg}};
  const frc::Pose2d kRight{1.11_m, 1.00_m, frc::Rotation2d{52.5_deg}};
}

// Processor station position (Blue alliance relative)
namespace ProcessorStation {
  const frc::Pose2d kPosition{6.34_m, 0.44_m, frc::Rotation2d{90.0_deg}};
}

// Reef side structure
struct ReefSide {
  frc::Pose2d left;
  frc::Pose2d right;
  frc::Pose2d center;
  frc::Pose2d trough;
  
  ReefSide(frc::Pose2d l, frc::Pose2d r, frc::Pose2d c, frc::Pose2d t)
    : left(l), right(r), center(c), trough(t) {}
};

// All reef sides
namespace ReefSides {
  extern const ReefSide kABSide;
  extern const ReefSide kCDSide;
  extern const ReefSide kEFSide;
  extern const ReefSide kGHSide;
  extern const ReefSide kIJSide;
  extern const ReefSide kKLSide;
  
  extern const std::array<ReefSide, 6> kAllSides;
}

// Helper functions for alliance conversion
/**
 * Convert a pose from blue alliance to current alliance
 */
frc::Pose2d ConvertPoseByAlliance(const frc::Pose2d& pose, 
                                  frc::DriverStation::Alliance alliance = frc::DriverStation::GetAlliance().value_or(frc::DriverStation::Alliance::kBlue));

/**
 * Convert a translation from blue alliance to current alliance
 */
frc::Translation2d ConvertTranslationByAlliance(const frc::Translation2d& translation,
                                                frc::DriverStation::Alliance alliance = frc::DriverStation::GetAlliance().value_or(frc::DriverStation::Alliance::kBlue));

/**
 * Convert a reef side from blue alliance to current alliance
 */
ReefSide ConvertReefSideByAlliance(const ReefSide& side,
                                   frc::DriverStation::Alliance alliance = frc::DriverStation::GetAlliance().value_or(frc::DriverStation::Alliance::kBlue));

/**
 * Apply an offset to a pose relative to the pose's rotation
 */
frc::Pose2d ApplyOffset(const frc::Pose2d& pose, const frc::Translation2d& offset);

/**
 * Get the field rotation for alliance-specific transforms
 */
frc::Rotation2d GetFieldRotation(frc::DriverStation::Alliance alliance);

} // namespace FieldConstants