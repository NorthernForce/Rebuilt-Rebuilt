#pragma once

#include "AprilTagCameraIO.h"
#include <photon/simulation/PhotonCameraSim.h>
#include <photon/simulation/VisionSystemSim.h>
#include <frc/apriltag/AprilTagFieldLayout.h>

namespace nfr
{

/**
 * PhotonVision simulation implementation of AprilTagCameraIO
 * 
 * This implementation provides simulation support for PhotonVision cameras,
 * allowing for testing and development without physical hardware.
 */
class PhotonVisionCameraSimIO : public AprilTagCameraIO
{
private:
    std::string m_cameraName;
    frc::Transform3d m_cameraTransform;
    std::shared_ptr<photon::PhotonCameraSim> m_cameraSim;
    std::shared_ptr<photon::VisionSystemSim> m_visionSim;
    
    // Simulated inputs
    bool m_connected = true;
    std::vector<frc::Pose2d> m_robotPoses;
    std::vector<units::second_t> m_timestamps;
    int m_targetCount = 0;
    double m_latency = 0.0;
    
public:
    /**
     * Constructor for PhotonVision camera simulation
     * @param cameraName Name of the simulated PhotonVision camera
     * @param cameraTransform Transform from robot center to camera
     * @param visionSim Shared vision system simulation
     */
    PhotonVisionCameraSimIO(const std::string& cameraName, 
                           const frc::Transform3d& cameraTransform,
                           std::shared_ptr<photon::VisionSystemSim> visionSim = nullptr);
    
    void UpdateInputs(AprilTagCameraInputs& inputs) override;
    void SetReferencePose(const frc::Pose3d& pose) override;
    void Log(const nfr::LogContext& log) const override;
    std::string GetCameraName() const override;
    frc::Transform3d GetCameraTransform() const override;
    
    /**
     * Update the robot pose in simulation
     * @param robotPose Current robot pose for simulation
     */
    void UpdateRobotPose(const frc::Pose3d& robotPose);
    
    /**
     * Get the camera simulation for advanced configuration
     * @return Shared pointer to camera simulation
     */
    std::shared_ptr<photon::PhotonCameraSim> GetCameraSim() const;
};

}  // namespace nfr