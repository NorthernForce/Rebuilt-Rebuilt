#pragma once

#include "AprilTagCameraIO.h"
#include <memory>
#include <string>
#include <frc/Timer.h>

namespace nfr
{

/**
 * High-level AprilTag camera wrapper that manages IO implementations
 * 
 * This class provides a clean interface for AprilTag pose estimation
 * while abstracting the underlying camera technology (PhotonVision, LimeLight, etc.)
 */
class AprilTagCamera
{
private:
    std::unique_ptr<AprilTagCameraIO> m_io;
    AprilTagCameraInputs m_inputs;
    std::string m_name;
    frc::Timer m_timeSinceLastEstimate;
    
public:
    /**
     * Constructor for AprilTagCamera
     * @param io The IO implementation to use (PhotonVision, LimeLight, etc.)
     * @param name Human-readable name for this camera
     */
    AprilTagCamera(std::unique_ptr<AprilTagCameraIO> io, const std::string& name);
    
    /**
     * Update camera data - should be called periodically
     */
    void Periodic();
    
    /**
     * Set reference pose for improved accuracy
     * @param pose Current robot pose from odometry
     */
    void SetReferencePose(const frc::Pose3d& pose);
    
    /**
     * Get the latest robot poses from this camera
     * @return Vector of estimated robot poses
     */
    const std::vector<frc::Pose2d>& GetRobotPoses() const;
    
    /**
     * Get timestamps corresponding to the robot poses
     * @return Vector of timestamps for each pose
     */
    const std::vector<units::second_t>& GetTimestamps() const;
    
    /**
     * Check if camera is connected and functioning
     * @return True if camera is connected
     */
    bool IsConnected() const;
    
    /**
     * Get number of detected targets
     * @return Number of AprilTags currently visible
     */
    int GetTargetCount() const;
    
    /**
     * Get camera latency
     * @return Latency in seconds
     */
    double GetLatency() const;
    
    /**
     * Get time since last valid estimate
     * @return Time since last estimate
     */
    units::second_t GetTimeSinceLastEstimate() const;
    
    /**
     * Check if we've had a recent estimate
     * @return True if recent estimate available (within timeout)
     */
    bool HasRecentEstimate() const;
    
    /**
     * Get camera name
     * @return Camera name string
     */
    const std::string& GetName() const;
    
    /**
     * Log camera data
     * @param log Logging context
     */
    void Log(const nfr::LogContext& log) const;
};

}  // namespace nfr