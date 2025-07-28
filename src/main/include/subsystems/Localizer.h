#pragma once

#include "apriltag/AprilTagCamera.h"
#include <frc2/command/SubsystemBase.h>
#include <frc/geometry/Pose2d.h>
#include <frc/Timer.h>
#include <logging/Logger.h>
#include <vector>
#include <memory>
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
    std::string cameraName;
    
    EstimatedPose(frc::Pose2d pose, units::second_t timestamp, const std::string& cameraName) 
        : pose(pose), timestamp(timestamp), cameraName(cameraName) {}
};

/**
 * AprilTag detection and pose estimation subsystem using multiple cameras
 * 
 * This subsystem integrates multiple cameras for robust robot localization:
 * - PhotonVision cameras for high-accuracy pose estimation
 * - LimeLight devices for additional pose data using MegaTag2
 * - Simulation support for testing and development
 * 
 * The subsystem automatically:
 * - Processes camera data each periodic cycle
 * - Provides pose estimates with timestamps and source camera info
 * - Integrates with SwerveDrive for improved odometry
 * - Tracks estimate freshness for validation
 * - Logs comprehensive data for analysis
 * 
 * Uses the IO pattern for clean separation of concerns and easy testing.
 */
class Localizer : public frc2::SubsystemBase
{
  private:
    // Camera instances using IO pattern
    std::vector<std::unique_ptr<AprilTagCamera>> m_cameras;
    
    // Storage for estimated poses from all cameras
    std::vector<EstimatedPose> m_estimatedPoses;
    
    // Timer to track time since last estimate
    frc::Timer m_timeSinceLastEstimate;
    
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
     * @return Vector of estimated poses with timestamps and camera info
     */
    const std::vector<EstimatedPose>& GetEstimatedPoses() const;
    
    /**
     * Get poses from a specific camera
     * @param cameraName Name of the camera to get poses from
     * @return Vector of poses from the specified camera
     */
    std::vector<frc::Pose2d> GetPosesFromCamera(const std::string& cameraName) const;
    
    /**
     * Get all cameras for direct access
     * @return Vector of camera pointers
     */
    const std::vector<std::unique_ptr<AprilTagCamera>>& GetCameras() const;
    
    /**
     * Get time since last pose estimate from any camera
     * @return Time since last estimate in seconds
     */
    units::second_t GetTimeSinceLastEstimate() const;
    
    /**
     * Check if we've had a recent pose estimate (within timeout)
     * @return True if recent estimate available
     */
    bool HasHadRecentEstimate() const;
    
    /**
     * Get number of connected cameras
     * @return Number of cameras currently connected
     */
    int GetConnectedCameraCount() const;
    
    /**
     * Get total number of visible targets across all cameras
     * @return Total target count
     */
    int GetTotalTargetCount() const;
    
    /**
     * Periodic function called by robot scheduler
     */
    void Periodic() override;
    
    /**
     * Log subsystem data
     * @param log Logging context
     */
    void Log(const nfr::LogContext& log) const;
    
  private:
    /**
     * Initialize cameras based on robot mode (real vs simulation)
     */
    void InitializeCameras();
};

}  // namespace nfr