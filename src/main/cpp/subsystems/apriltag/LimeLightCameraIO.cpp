#include "subsystems/apriltag/LimeLightCameraIO.h"

namespace nfr
{

LimeLightCameraIO::LimeLightCameraIO(const std::string& cameraName,
                                     const frc::Transform3d& cameraTransform)
    : m_cameraName(cameraName), m_cameraTransform(cameraTransform)
{
}

void LimeLightCameraIO::UpdateInputs(AprilTagCameraInputs& inputs)
{
    // Clear previous data
    inputs.robotPoses.clear();
    inputs.timestamps.clear();
    inputs.targetCount = 0;
    inputs.latency = 0.0;

    // Get pose estimate from LimeLight using MegaTag2
    auto estimate =
        LimelightHelpers::getBotPoseEstimate_wpiBlue_MegaTag2(m_cameraName);

    // Check if we have valid data
    inputs.connected = estimate.tagCount > 0;
    inputs.targetCount = estimate.tagCount;
    inputs.latency = estimate.latency / 1000.0;  // Convert ms to seconds

    if (inputs.connected && estimate.tagCount > 0)
    {
        inputs.robotPoses.push_back(estimate.pose);
        inputs.timestamps.push_back(units::second_t{estimate.timestampSeconds});
    }
}

void LimeLightCameraIO::SetReferencePose(const frc::Pose3d& pose)
{
    // Update robot orientation for improved MegaTag2 accuracy
    LimelightHelpers::SetRobotOrientation(
        m_cameraName,
        pose.Rotation().Z().convert<units::degree>().value(),  // Yaw in degrees
        0, 0, 0, 0, 0  // Other orientation values (pitch, roll, etc.)
    );
}

void LimeLightCameraIO::Log(const nfr::LogContext& log) const
{
    log["camera_name"] << m_cameraName;

    // Log camera transform
    const auto& translation = m_cameraTransform.Translation();
    const auto& rotation = m_cameraTransform.Rotation();
    log["transform/x"] << translation.X().value();
    log["transform/y"] << translation.Y().value();
    log["transform/z"] << translation.Z().value();
    log["transform/roll"] << rotation.X().value();
    log["transform/pitch"] << rotation.Y().value();
    log["transform/yaw"] << rotation.Z().value();

    // Log current LimeLight data
    log["tv"] << LimelightHelpers::getTV(m_cameraName);
    log["ta"] << LimelightHelpers::getTA(m_cameraName);
    log["tx"] << LimelightHelpers::getTX(m_cameraName);
    log["ty"] << LimelightHelpers::getTY(m_cameraName);
    log["pipeline_latency"]
        << LimelightHelpers::getLatency_Pipeline(m_cameraName);
    log["capture_latency"] << LimelightHelpers::getLatency_Capture(
        m_cameraName);

    // Log pose estimate data
    auto estimate =
        LimelightHelpers::getBotPoseEstimate_wpiBlue_MegaTag2(m_cameraName);
    log["pose_estimate/tag_count"] << estimate.tagCount;
    log["pose_estimate/tag_span"] << estimate.tagSpan;
    log["pose_estimate/avg_dist"] << estimate.avgTagDist;
    log["pose_estimate/avg_area"] << estimate.avgTagArea;
}

std::string LimeLightCameraIO::GetCameraName() const
{
    return m_cameraName;
}

frc::Transform3d LimeLightCameraIO::GetCameraTransform() const
{
    return m_cameraTransform;
}

}  // namespace nfr