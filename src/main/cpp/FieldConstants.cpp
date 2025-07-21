// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "FieldConstants.h"

using namespace units;

namespace FieldConstants {

// Define the reef sides
namespace ReefSides {
  const ReefSide kABSide{
    ReefPositions::kA, 
    ReefPositions::kB, 
    ReefPositions::kABAlgae, 
    frc::Pose2d{3.531_m, 5.203_m, frc::Rotation2d{kABRotation + 90.0_deg}}
  };
  
  const ReefSide kCDSide{
    ReefPositions::kC, 
    ReefPositions::kD, 
    ReefPositions::kCDAlgae, 
    kABSide.trough.RotateBy(frc::Rotation2d{60.0_deg})
  };
  
  const ReefSide kEFSide{
    ReefPositions::kE, 
    ReefPositions::kF, 
    ReefPositions::kEFAlgae, 
    kABSide.trough.RotateBy(frc::Rotation2d{120.0_deg})
  };
  
  const ReefSide kGHSide{
    ReefPositions::kG, 
    ReefPositions::kH, 
    ReefPositions::kGHAlgae, 
    kABSide.trough.RotateBy(frc::Rotation2d{180.0_deg})
  };
  
  const ReefSide kIJSide{
    ReefPositions::kI, 
    ReefPositions::kJ, 
    ReefPositions::kIJAlgae, 
    kABSide.trough.RotateBy(frc::Rotation2d{240.0_deg})
  };
  
  const ReefSide kKLSide{
    ReefPositions::kK, 
    ReefPositions::kL, 
    ReefPositions::kKLAlgae, 
    kABSide.trough.RotateBy(frc::Rotation2d{300.0_deg})
  };
  
  const std::array<ReefSide, 6> kAllSides = {
    kABSide, kCDSide, kEFSide, kGHSide, kIJSide, kKLSide
  };
}

frc::Pose2d ConvertPoseByAlliance(const frc::Pose2d& pose, frc::DriverStation::Alliance alliance) {
  if (alliance == frc::DriverStation::Alliance::kBlue) {
    return pose;
  } else {
    return frc::Pose2d{
      kFieldLength.value() - pose.X().value(),
      kFieldWidth.value() - pose.Y().value(),
      pose.Rotation() + frc::Rotation2d{180.0_deg}
    };
  }
}

frc::Translation2d ConvertTranslationByAlliance(const frc::Translation2d& translation, frc::DriverStation::Alliance alliance) {
  if (alliance == frc::DriverStation::Alliance::kBlue) {
    return translation;
  } else {
    return frc::Translation2d{
      kFieldLength.value() - translation.X().value(),
      kFieldWidth.value() - translation.Y().value()
    };
  }
}

ReefSide ConvertReefSideByAlliance(const ReefSide& side, frc::DriverStation::Alliance alliance) {
  if (alliance == frc::DriverStation::Alliance::kBlue) {
    return side;
  } else {
    return ReefSide{
      ConvertPoseByAlliance(side.left, alliance),
      ConvertPoseByAlliance(side.right, alliance),
      ConvertPoseByAlliance(side.center, alliance),
      ConvertPoseByAlliance(side.trough, alliance)
    };
  }
}

frc::Pose2d ApplyOffset(const frc::Pose2d& pose, const frc::Translation2d& offset) {
  frc::Translation2d rotatedOffset = offset.RotateBy(pose.Rotation());
  return frc::Pose2d{
    pose.Translation() + rotatedOffset,
    pose.Rotation()
  };
}

frc::Rotation2d GetFieldRotation(frc::DriverStation::Alliance alliance) {
  if (alliance == frc::DriverStation::Alliance::kBlue) {
    return frc::Rotation2d{0.0_deg};
  } else {
    return frc::Rotation2d{180.0_deg};
  }
}

} // namespace FieldConstants