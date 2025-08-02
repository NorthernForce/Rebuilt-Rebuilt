#include "subsystems/apriltag/PhotonVisionCameraSimIO.h"

#include <frc/Timer.h>
#include <frc/apriltag/AprilTagFields.h>
#include <photon/PhotonCamera.h>
#include <units/frequency.h>

namespace nfr
{

PhotonVisionCameraSimIO::PhotonVisionCameraSimIO(
    const std::string& cameraName, const frc::Transform3d& cameraTransform,
    std::shared_ptr<photon::VisionSystemSim> visionSim)
    : m_cameraName(cameraName), m_cameraTransform(cameraTransform)
{
    // Create or use provided vision system simulation
    if (visionSim)
    {
        m_visionSim = visionSim;
    }
    else
    {
        m_visionSim = std::make_shared<photon::VisionSystemSim>("VisionSim");
        m_visionSim->AddAprilTags(frc::AprilTagFieldLayout::LoadField(
            frc::AprilTagField::k2024Crescendo));
    }

    // Create camera simulation with proper constructor
    photon::SimCameraProperties cameraProperties{};
    cameraProperties.SetCalibration(960, 720, frc::Rotation2d{90_deg});
    cameraProperties.SetCalibError(0.35, 0.10);
    cameraProperties.SetFPS(15_Hz);
    cameraProperties.SetAvgLatency(50_ms);
    cameraProperties.SetLatencyStdDev(15_ms);

    // Use new constructor approach
    m_cameraSim =
        std::shared_ptr<photon::PhotonCameraSim>(new photon::PhotonCameraSim(
            m_cameraName, cameraProperties, m_cameraTransform));

    // Add camera to vision system
    m_visionSim->AddCamera(m_cameraSim.get(), m_cameraTransform);
}

void PhotonVisionCameraSimIO::UpdateInputs(AprilTagCameraInputs& inputs)
{
    // Clear previous data
    inputs.robotPoses.clear();
    inputs.timestamps.clear();
    inputs.targetCount = 0;
    inputs.latency = 0.0;
    inputs.connected = m_connected;

    if (!inputs.connected)
    {
        return;
    }

    // Get simulated camera results
    photon::PhotonCamera camera{m_cameraName};
    auto results = camera.GetAllUnreadResults();

    for (const auto& result : results)
    {
        inputs.targetCount = result.GetTargets().size();
        inputs.latency = result.GetLatency().value();

        // For simulation, we can generate pose estimates directly from known
        // AprilTag positions
        if (result.HasTargets())
        {
            // Use the vision system's pose estimation
            // In a real implementation, this would use PhotonPoseEstimator
            // For simulation, we'll use the known robot pose with some noise

            // Add timestamp
            inputs.timestamps.push_back(result.GetTimestamp());

            // Note: In a real simulation, we would compute the pose estimate
            // from the detected targets. For now, we'll leave this empty
            // and let the actual robot pose be set via UpdateRobotPose
        }
    }

    // Copy simulated data
    inputs.robotPoses = m_robotPoses;
    if (inputs.timestamps.empty() && !inputs.robotPoses.empty())
    {
        // Fill timestamps if we have poses but no timestamps
        auto currentTime = frc::Timer::GetFPGATimestamp();
        for (size_t i = 0; i < inputs.robotPoses.size(); ++i)
        {
            inputs.timestamps.push_back(currentTime);
        }
    }
}

void PhotonVisionCameraSimIO::SetReferencePose(const frc::Pose3d& pose)
{
    // Update the robot pose in the vision simulation
    UpdateRobotPose(pose);
}

void PhotonVisionCameraSimIO::UpdateRobotPose(const frc::Pose3d& robotPose)
{
    if (m_visionSim)
    {
        m_visionSim->Update(robotPose);
    }

    // For simulation purposes, generate a pose estimate with some noise
    // This simulates what a real camera would detect
    if (m_targetCount > 0)  // Only if we "see" targets
    {
        // Add some noise to simulate real-world conditions
        auto noisyPose = robotPose.ToPose2d();
        // Note: In a production system, you'd add realistic noise here

        m_robotPoses.clear();
        m_robotPoses.push_back(noisyPose);
    }
    else
    {
        m_robotPoses.clear();
    }
}

void PhotonVisionCameraSimIO::Log(const nfr::LogContext& log) const
{
    log["camera_name"] << m_cameraName;
    log["simulation"] << true;
    log["connected"] << m_connected;

    // Transform information is not needed in regular logging

    // Log simulation-specific data
    log["sim/pose_count"] << static_cast<int>(m_robotPoses.size());
    log["sim/target_count"] << m_targetCount;
    log["sim/latency"] << m_latency;
}

std::string PhotonVisionCameraSimIO::GetCameraName() const
{
    return m_cameraName;
}

frc::Transform3d PhotonVisionCameraSimIO::GetCameraTransform() const
{
    return m_cameraTransform;
}

std::shared_ptr<photon::PhotonCameraSim> PhotonVisionCameraSimIO::GetCameraSim()
    const
{
    return m_cameraSim;
}

}  // namespace nfr