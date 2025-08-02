#pragma once

#include "AprilTagCameraIO.h"
#include "limelight/LimelightHelpers.h"

namespace nfr
{

/**
 * LimeLight implementation of AprilTagCameraIO
 *
 * This implementation uses LimeLight cameras with MegaTag2 for AprilTag
 * detection and pose estimation.
 */
class LimeLightCameraIO : public AprilTagCameraIO
{
  private:
    std::string m_cameraName;
    frc::Transform3d m_cameraTransform;

  public:
    /**
     * Constructor for LimeLight camera
     * @param cameraName Name of the LimeLight camera
     * @param cameraTransform Transform from robot center to camera
     */
    LimeLightCameraIO(const std::string& cameraName,
                      const frc::Transform3d& cameraTransform);

    void UpdateInputs(AprilTagCameraInputs& inputs) override;
    void SetReferencePose(const frc::Pose3d& pose) override;
    void Log(const nfr::LogContext& log) const override;
    std::string GetCameraName() const override;
    frc::Transform3d GetCameraTransform() const override;
};

}  // namespace nfr