# LimeLight and PhotonVision AprilTag Detection Subsystem

This subsystem provides AprilTag detection and pose estimation using both PhotonVision cameras and LimeLight devices.

## Features

- **Multi-camera support**: Supports 2 PhotonVision cameras and 2 LimeLight devices
- **Pose estimation**: Uses AprilTag detection for robot localization
- **Vision integration**: Automatically provides vision measurements to the SwerveDrive pose estimator
- **Fallback strategies**: Multiple cameras provide redundancy for pose estimation
- **Recent estimate tracking**: Monitors freshness of pose estimates

## Hardware Setup

### PhotonVision Cameras
- **Front Left Camera**: Named "front_left_camera" in PhotonVision interface
- **Center Camera**: Named "center_camera" in PhotonVision interface

### LimeLight Devices  
- **Front Right LimeLight**: Named "limelight-fl" 
- **Center Back LimeLight**: Named "limelight-ctr"

## Configuration

### Camera Positions
Camera transforms are defined in `src/main/cpp/constants/Constants.cpp`. Update these values to match your robot's camera mounting positions:

```cpp
// Example: Front left camera 30cm forward, 20cm left, 50cm up from robot center
const frc::Transform3d CameraConstants::kFrontLeftCameraTransform{
    frc::Translation3d{0.3_m, 0.2_m, 0.5_m},  
    frc::Rotation3d{0_deg, -15_deg, 0_deg}     // Slight downward tilt
};
```

### Camera Names
Camera names can be modified in `src/main/include/constants/Constants.h`:

```cpp
static constexpr const char* kFrontLeftCameraName = "front_left_camera";
static constexpr const char* kCenterCameraName = "center_camera";
static constexpr const char* kLimelightFLName = "limelight-fl";
static constexpr const char* kLimelightCenterName = "limelight-ctr";
```

## Usage

The subsystem is automatically integrated with the robot:

1. **Automatic Integration**: The `Localizer` is instantiated in `RobotContainer` and runs automatically
2. **Vision Updates**: Pose estimates are automatically fed to the SwerveDrive pose estimator
3. **Reference Updates**: The subsystem receives the current robot pose for improved accuracy

### Accessing Pose Data

```cpp
// Get all estimated poses from current cycle
const auto& poses = localizer.GetEstimatedPoses();

// Get poses from specific cameras
const auto& frontLeftPoses = localizer.GetFrontLeftPoses();
const auto& centerPoses = localizer.GetCenterPoses();
const auto& limelightFLPoses = localizer.GetFrontRightPoses();
const auto& limelightCenterPoses = localizer.GetCenterBackPoses();

// Check for recent estimates
bool hasRecent = localizer.HasHadRecentEstimate();
units::second_t timeSinceLastEstimate = localizer.GetTimeSinceLastEstimate();
```

## Dependencies

This subsystem requires the following vendor dependencies (automatically included):

- **PhotonVision**: `vendordeps/photonlib.json`
- **LimeLight**: `vendordeps/LimelightLib.json`

## Field Layout

The subsystem uses the 2025 Reefscape field layout for AprilTag positions. This is automatically loaded from WPILib.

## Coordinate System

- All poses are in the WPILib blue alliance coordinate system
- X-axis: Forward (towards opposing alliance)
- Y-axis: Left (when facing opposing alliance)  
- Rotation: Counter-clockwise positive

## Troubleshooting

1. **No pose estimates**: Check camera connections and AprilTag visibility
2. **Inaccurate poses**: Verify camera transform calibration in Constants.cpp
3. **Build errors**: Ensure vendor dependencies are properly installed
4. **Network issues**: Check PhotonVision and LimeLight device network configuration