#include "subsystems/Localizer.h"
#include "constants/Constants.h"
#include <frc/apriltag/AprilTagFields.h>
#include <units/time.h>

namespace nfr
{

Localizer::Localizer()
    : m_frontLeftCamera(CameraConstants::kFrontLeftCameraName),
      m_centerCamera(CameraConstants::kCenterCameraName),
      m_frontLeftPoseEstimator(
          frc::LoadAprilTagLayoutField(frc::AprilTagField::k2025Reefscape),
          photon::PoseStrategy::MULTI_TAG_PNP_ON_COPROCESSOR,
          CameraConstants::kFrontLeftCameraTransform),
      m_centerPoseEstimator(
          frc::LoadAprilTagLayoutField(frc::AprilTagField::k2025Reefscape),
          photon::PoseStrategy::MULTI_TAG_PNP_ON_COPROCESSOR,
          CameraConstants::kCenterCameraTransform),
      m_fieldLayout(frc::LoadAprilTagLayoutField(frc::AprilTagField::k2025Reefscape))
{
    // Configure fallback strategies for pose estimators
    m_frontLeftPoseEstimator.SetMultiTagFallbackStrategy(
        photon::PoseStrategy::CLOSEST_TO_REFERENCE_POSE);
    m_centerPoseEstimator.SetMultiTagFallbackStrategy(
        photon::PoseStrategy::CLOSEST_TO_REFERENCE_POSE);
}

void Localizer::UpdateWithReferencePose(const frc::Pose2d& pose)
{
    units::second_t timestamp = frc::Timer::GetFPGATimestamp();
    
    // Update LimeLight robot orientation
    limelightlib::LimelightHelpers::SetRobotOrientation_MegaTag2(
        CameraConstants::kLimelightFLName,
        pose.Rotation().Degrees().value(), 0, 0, 0, 0, 0);
        
    limelightlib::LimelightHelpers::SetRobotOrientation_MegaTag2(
        CameraConstants::kLimelightCenterName,
        pose.Rotation().Degrees().value(), 0, 0, 0, 0, 0);
    
    // Set reference poses for improved accuracy
    m_frontLeftPoseEstimator.SetReferencePose(pose);
    m_centerPoseEstimator.SetReferencePose(pose);
}

void Localizer::Periodic()
{
    // Clear previous estimates
    m_estimatedPoses.clear();
    m_frontLeftPoses.clear();
    m_centerPoses.clear();
    m_frontRightPoses.clear();
    m_centerBackPoses.clear();
    
    // Process LimeLight estimates
    auto frontRightEstimate = limelightlib::LimelightHelpers::getBotPoseEstimate_wpiBlue_MegaTag2(
        CameraConstants::kLimelightFLName);
    if (frontRightEstimate.tagCount > 0)
    {
        EstimatedPose pose{frontRightEstimate.pose, units::second_t{frontRightEstimate.timestampSeconds}};
        m_frontRightPoses.push_back(pose.pose);
        m_estimatedPoses.push_back(pose);
    }
    
    auto centerBackEstimate = limelightlib::LimelightHelpers::getBotPoseEstimate_wpiBlue_MegaTag2(
        CameraConstants::kLimelightCenterName);
    if (centerBackEstimate.tagCount > 0)
    {
        EstimatedPose pose{centerBackEstimate.pose, units::second_t{centerBackEstimate.timestampSeconds}};
        m_centerBackPoses.push_back(pose.pose);
        m_estimatedPoses.push_back(pose);
    }
    
    // Process PhotonVision front left camera results
    auto frontLeftResults = m_frontLeftCamera.GetAllUnreadResults();
    for (const auto& result : frontLeftResults)
    {
        auto estimate = m_frontLeftPoseEstimator.Update(result);
        if (estimate.has_value())
        {
            EstimatedPose pose{estimate.value().estimatedPose.ToPose2d(), 
                             estimate.value().timestamp};
            m_frontLeftPoses.push_back(pose.pose);
            m_estimatedPoses.push_back(pose);
        }
    }
    
    // Process PhotonVision center camera results
    auto centerResults = m_centerCamera.GetAllUnreadResults();
    for (const auto& result : centerResults)
    {
        auto estimate = m_centerPoseEstimator.Update(result);
        if (estimate.has_value())
        {
            EstimatedPose pose{estimate.value().estimatedPose.ToPose2d(), 
                             estimate.value().timestamp};
            m_centerPoses.push_back(pose.pose);
            m_estimatedPoses.push_back(pose);
        }
    }
    
    // Restart timer if we got any estimates
    if (!m_estimatedPoses.empty())
    {
        m_timeSinceLastEstimate.Restart();
    }
}

const std::vector<EstimatedPose>& Localizer::GetEstimatedPoses() const
{
    return m_estimatedPoses;
}

const std::vector<frc::Pose2d>& Localizer::GetFrontLeftPoses() const
{
    return m_frontLeftPoses;
}

const std::vector<frc::Pose2d>& Localizer::GetCenterPoses() const
{
    return m_centerPoses;
}

const std::vector<frc::Pose2d>& Localizer::GetFrontRightPoses() const
{
    return m_frontRightPoses;
}

const std::vector<frc::Pose2d>& Localizer::GetCenterBackPoses() const
{
    return m_centerBackPoses;
}

units::second_t Localizer::GetTimeSinceLastEstimate() const
{
    return m_timeSinceLastEstimate.Get();
}

bool Localizer::HasHadRecentEstimate() const
{
    return !m_timeSinceLastEstimate.HasElapsed(0.5_s);
}

}  // namespace nfr