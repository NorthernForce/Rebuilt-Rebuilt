#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/geometry/Pose2d.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <optional>
#include <vector>

/**
 * AprilTagsSubsystem - Dedicated AprilTag pose estimation system
 * Simplified version without PhotonVision dependencies for initial implementation
 */
class AprilTagsSubsystem : public frc2::SubsystemBase {
public:
    struct PoseEstimate {
        frc::Pose2d pose;
        units::second_t timestamp{0_s};
        double ambiguity{0.0};
        std::vector<int> tagIds;
        bool isValid{false};
    };

    AprilTagsSubsystem();

    /**
     * Called periodically by the CommandScheduler
     */
    void Periodic() override;

    /**
     * Get latest robot pose estimate from AprilTags
     */
    std::optional<PoseEstimate> GetRobotPose();

    /**
     * Check if AprilTags are currently visible
     */
    bool HasValidPose();

    /**
     * Get number of AprilTags currently visible
     */
    int GetVisibleTagCount();

    /**
     * Get list of visible tag IDs
     */
    std::vector<int> GetVisibleTagIds();

    /**
     * Set reference pose for pose estimation (from odometry)
     */
    void SetReferencePose(const frc::Pose2d& pose);

    /**
     * Enable/disable pose estimation
     */
    void SetEstimationEnabled(bool enabled);

    /**
     * Check if estimation is enabled
     */
    bool IsEstimationEnabled() const { return m_estimationEnabled; }

private:
    // State
    bool m_estimationEnabled{true};
    PoseEstimate m_lastPoseEstimate;
    std::vector<int> m_visibleTags;
    frc::Pose2d m_referencePose;
    
    // NetworkTables
    std::shared_ptr<nt::NetworkTable> m_aprilTagTable;
    std::shared_ptr<nt::NetworkTable> m_limelightTable;
    
    void UpdatePoseEstimation();
    void UpdateTelemetry();
};