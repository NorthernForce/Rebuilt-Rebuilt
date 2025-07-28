#pragma once

#include <photon/PhotonCamera.h>
#include <photon/PhotonPoseEstimator.h>
#include <frc2/command/SubsystemBase.h>
#include <frc/geometry/Pose2d.h>
#include <frc/Timer.h>
#include <frc/apriltag/AprilTagFieldLayout.h>
#include <limelightlib-wpicpp/LimelightHelpers.h>
#include <vector>
#include <units/time.h>

namespace nfr
{

/**
 * Estimated pose with timestamp
 */
struct EstimatedPose
{
    frc::Pose2d pose;
    units::second_t timestamp;
    
    EstimatedPose(frc::Pose2d pose, units::second_t timestamp) 
        : pose(pose), timestamp(timestamp) {}
};

/**
 * AprilTag detection and pose estimation subsystem using PhotonVision and LimeLight
 * 
 * This subsystem integrates multiple cameras for robust robot localization:
 * - 2 PhotonVision cameras for high-accuracy pose estimation
 * - 2 LimeLight devices for additional pose data using MegaTag2
 * 
 * The subsystem automatically:
 * - Processes camera data each periodic cycle
 * - Provides pose estimates with timestamps
 * - Integrates with SwerveDrive for improved odometry
 * - Tracks estimate freshness for validation
 * 
 * Camera positions must be configured in Constants.cpp to match physical mounting.
 */
class Localizer : public frc2::SubsystemBase
{
  private:
    // PhotonVision cameras
    photon::PhotonCamera m_frontLeftCamera;
    photon::PhotonCamera m_centerCamera;
    
    // Pose estimators for PhotonVision cameras
    photon::PhotonPoseEstimator m_frontLeftPoseEstimator;
    photon::PhotonPoseEstimator m_centerPoseEstimator;
    
    // Storage for estimated poses from different cameras
    std::vector<EstimatedPose> m_estimatedPoses;
    std::vector<frc::Pose2d> m_frontLeftPoses;
    std::vector<frc::Pose2d> m_centerPoses;
    std::vector<frc::Pose2d> m_frontRightPoses;  // LimeLight FL
    std::vector<frc::Pose2d> m_centerBackPoses;  // LimeLight Center
    
    // Timer to track time since last estimate
    frc::Timer m_timeSinceLastEstimate;
    
    // Field layout for AprilTag positions
    frc::AprilTagFieldLayout m_fieldLayout;
    
  public:
    /**
     * Constructor for Localizer
     */
    Localizer();
    
    /**
     * Update pose estimators with reference pose for improved accuracy
     * @param pose Reference pose from odometry
     */
    void UpdateWithReferencePose(const frc::Pose2d& pose);
    
    /**
     * Get all estimated poses from the current periodic cycle
     * @return Vector of estimated poses with timestamps
     */
    const std::vector<EstimatedPose>& GetEstimatedPoses() const;
    
    /**
     * Get poses from front left PhotonVision camera
     * @return Vector of poses from front left camera
     */
    const std::vector<frc::Pose2d>& GetFrontLeftPoses() const;
    
    /**
     * Get poses from center PhotonVision camera  
     * @return Vector of poses from center camera
     */
    const std::vector<frc::Pose2d>& GetCenterPoses() const;
    
    /**
     * Get poses from front right LimeLight camera
     * @return Vector of poses from front right LimeLight
     */
    const std::vector<frc::Pose2d>& GetFrontRightPoses() const;
    
    /**
     * Get poses from center back LimeLight camera
     * @return Vector of poses from center back LimeLight 
     */
    const std::vector<frc::Pose2d>& GetCenterBackPoses() const;
    
    /**
     * Get time since last pose estimate
     * @return Time since last estimate in seconds
     */
    units::second_t GetTimeSinceLastEstimate() const;
    
    /**
     * Check if we've had a recent pose estimate (within 0.5 seconds)
     * @return True if recent estimate available
     */
    bool HasHadRecentEstimate() const;
    
    /**
     * Periodic function called by robot scheduler
     */
    void Periodic() override;
};

}  // namespace nfr