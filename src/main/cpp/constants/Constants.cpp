#include "constants/Constants.h"

namespace nfr
{
// Define camera transforms - these values should be adjusted based on actual robot configuration
const frc::Transform3d CameraConstants::kFrontLeftCameraTransform{
    frc::Translation3d{0.3_m, 0.2_m, 0.5_m},  // 30cm forward, 20cm left, 50cm up
    frc::Rotation3d{0_deg, -15_deg, 0_deg}     // Slight downward tilt
};

const frc::Transform3d CameraConstants::kCenterCameraTransform{
    frc::Translation3d{0.0_m, 0.0_m, 0.5_m},  // Center of robot, 50cm up  
    frc::Rotation3d{0_deg, -15_deg, 0_deg}     // Slight downward tilt
};
}  // namespace nfr