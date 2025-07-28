#include "subsystems/apriltag/PhotonVisionCameraIO.h"
#include <frc/apriltag/AprilTagFields.h>

namespace nfr
{

PhotonVisionCameraIO::PhotonVisionCameraIO(const std::string& cameraName, const frc::Transform3d& cameraTransform)
    : m_camera(cameraName),
      m_poseEstimator(
          frc::LoadAprilTagLayoutField(frc::AprilTagField::k2024Crescendo),
          photon::PoseStrategy::MULTI_TAG_PNP_ON_COPROCESSOR,
          cameraTransform),
      m_cameraName(cameraName),
      m_cameraTransform(cameraTransform)
{
    // Configure fallback strategy for improved accuracy
    m_poseEstimator.SetMultiTagFallbackStrategy(
        photon::PoseStrategy::CLOSEST_TO_REFERENCE_POSE);
}

void PhotonVisionCameraIO::UpdateInputs(AprilTagCameraInputs& inputs)
{
    // Clear previous data
    inputs.robotPoses.clear();
    inputs.timestamps.clear();
    inputs.targetCount = 0;
    inputs.latency = 0.0;
    inputs.connected = m_camera.IsConnected();
    
    if (!inputs.connected)
    {
        return;
    }
    
    // Process all unread results
    auto results = m_camera.GetAllUnreadResults();
    for (const auto& result : results)
    {
        inputs.targetCount = result.GetTargets().size();
        inputs.latency = result.GetLatency().value();
        
        // Attempt pose estimation
        auto estimate = m_poseEstimator.Update(result);
        if (estimate.has_value())
        {
            inputs.robotPoses.push_back(estimate.value().estimatedPose.ToPose2d());
            inputs.timestamps.push_back(estimate.value().timestamp);
        }
    }
}

void PhotonVisionCameraIO::SetReferencePose(const frc::Pose3d& pose)
{
    m_poseEstimator.SetReferencePose(pose);
}

void PhotonVisionCameraIO::Log(const nfr::LogContext& log) const
{
    log["camera_name"] << m_cameraName;
    log["is_connected"] << m_camera.IsConnected();
    
    // Log camera transform
    const auto& translation = m_cameraTransform.Translation();
    const auto& rotation = m_cameraTransform.Rotation();
    log["transform/x"] << translation.X().value();
    log["transform/y"] << translation.Y().value();
    log["transform/z"] << translation.Z().value();
    log["transform/roll"] << rotation.X().value();
    log["transform/pitch"] << rotation.Y().value();
    log["transform/yaw"] << rotation.Z().value();
    
    // Log latest result if available
    auto result = m_camera.GetLatestResult();
    if (result.HasTargets())
    {
        log["latest_result/target_count"] << static_cast<int>(result.GetTargets().size());
        log["latest_result/latency"] << result.GetLatency().value();
        log["latest_result/timestamp"] << result.GetTimestamp().value();
    }
}

std::string PhotonVisionCameraIO::GetCameraName() const
{
    return m_cameraName;
}

frc::Transform3d PhotonVisionCameraIO::GetCameraTransform() const
{
    return m_cameraTransform;
}

}  // namespace nfr