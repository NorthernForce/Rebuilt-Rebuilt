#pragma once

#include <frc/apriltag/AprilTagFieldLayout.h>
#include <photon/PhotonCamera.h>
#include <photon/PhotonPoseEstimator.h>

#include "AprilTagCameraIO.h"

namespace nfr
{

/**
 * PhotonVision implementation of AprilTagCameraIO
 *
 * This implementation uses PhotonVision cameras for AprilTag detection
 * and pose estimation with multi-tag PnP on the coprocessor.
 */
class PhotonVisionCameraIO : public AprilTagCameraIO
{
  private:
    photon::PhotonCamera m_camera;
    photon::PhotonPoseEstimator m_poseEstimator;
    std::string m_cameraName;
    frc::Transform3d m_cameraTransform;

  public:
    /**
     * Constructor for PhotonVision camera
     * @param cameraName Name of the PhotonVision camera
     * @param cameraTransform Transform from robot center to camera
     */
    PhotonVisionCameraIO(const std::string& cameraName,
                         const frc::Transform3d& cameraTransform);

    void UpdateInputs(AprilTagCameraInputs& inputs) override;
    void SetReferencePose(const frc::Pose3d& pose) override;
    void Log(const nfr::LogContext& log) const override;
    std::string GetCameraName() const override;
    frc::Transform3d GetCameraTransform() const override;
};

}  // namespace nfr