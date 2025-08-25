#include "subsystems/apriltag/AprilTagCamera.h"

#include "constants/Constants.h"

namespace nfr
{

    AprilTagCamera::AprilTagCamera(std::unique_ptr<AprilTagCameraIO> io,
                                   const std::string& name)
        : m_io(std::move(io)), m_name(name)
    {
        m_timeSinceLastEstimate.Start();
    }

    void AprilTagCamera::Periodic()
    {
        // Update inputs from the IO layer
        m_io->UpdateInputs(m_inputs);

        // Restart timer if we got any valid poses
        if (!m_inputs.robotPoses.empty())
        {
            m_timeSinceLastEstimate.Restart();
        }
    }

    void AprilTagCamera::SetReferencePose(const frc::Pose3d& pose)
    {
        m_io->SetReferencePose(pose);
    }

    const std::vector<frc::Pose2d>& AprilTagCamera::GetRobotPoses() const
    {
        return m_inputs.robotPoses;
    }

    const std::vector<units::second_t>& AprilTagCamera::GetTimestamps() const
    {
        return m_inputs.timestamps;
    }

    bool AprilTagCamera::IsConnected() const
    {
        return m_inputs.connected;
    }

    int AprilTagCamera::GetTargetCount() const
    {
        return m_inputs.targetCount;
    }

    double AprilTagCamera::GetLatency() const
    {
        return m_inputs.latency;
    }

    units::second_t AprilTagCamera::GetTimeSinceLastEstimate() const
    {
        return m_timeSinceLastEstimate.Get();
    }

    bool AprilTagCamera::HasRecentEstimate() const
    {
        return !m_timeSinceLastEstimate.HasElapsed(
            VisionConstants::kEstimateTimeout);
    }

    const std::string& AprilTagCamera::GetName() const
    {
        return m_name;
    }

    void AprilTagCamera::Log(const nfr::LogContext& log) const
    {
        log["connected"] << m_inputs.connected;
        log["target_count"] << m_inputs.targetCount;
        log["latency"] << m_inputs.latency;
        log["pose_count"] << static_cast<int>(m_inputs.robotPoses.size());
        log["time_since_estimate"] << GetTimeSinceLastEstimate().value();
        log["has_recent_estimate"] << HasRecentEstimate();

        // Log poses if available
        if (!m_inputs.robotPoses.empty())
        {
            // Log the most recent pose
            const auto& latestPose = m_inputs.robotPoses.back();
            log["latest_pose/x"] << latestPose.X().value();
            log["latest_pose/y"] << latestPose.Y().value();
            log["latest_pose/rotation"]
                << latestPose.Rotation().Degrees().value();
        }

        // Delegate to IO-specific logging
        m_io->Log(log["io"]);
    }

}  // namespace nfr