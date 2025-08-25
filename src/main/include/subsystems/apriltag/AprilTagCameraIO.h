#pragma once

#include <frc/geometry/Pose2d.h>
#include <frc/geometry/Pose3d.h>
#include <frc/geometry/Transform3d.h>
#include <logging/Logger.h>
#include <units/time.h>

#include <optional>
#include <vector>

namespace nfr
{

    /**
     * Inputs from camera hardware/simulation
     */
    struct AprilTagCameraInputs
    {
        bool connected = false;
        std::vector<frc::Pose2d> robotPoses;
        std::vector<units::second_t> timestamps;
        int targetCount = 0;
        double latency = 0.0;  // seconds
    };

    /**
     * Interface for AprilTag camera implementations
     *
     * This interface abstracts the different camera types (PhotonVision,
     * LimeLight) and provides a common interface for pose estimation and
     * logging.
     */
    class AprilTagCameraIO
    {
    public:
        virtual ~AprilTagCameraIO() = default;

        /**
         * Update inputs from the camera
         * @param inputs Structure to populate with current camera data
         */
        virtual void UpdateInputs(AprilTagCameraInputs& inputs) = 0;

        /**
         * Set the robot's reference pose for improved accuracy
         * @param pose Current robot pose from odometry
         */
        virtual void SetReferencePose(const frc::Pose3d& pose) = 0;

        /**
         * Log camera-specific data
         * @param log Logging context
         */
        virtual void Log(const nfr::LogContext& log) const = 0;

        /**
         * Get the camera's name for identification
         * @return Camera name string
         */
        virtual std::string GetCameraName() const = 0;

        /**
         * Get the camera's transform relative to robot center
         * @return Transform from robot center to camera
         */
        virtual frc::Transform3d GetCameraTransform() const = 0;
    };

}  // namespace nfr