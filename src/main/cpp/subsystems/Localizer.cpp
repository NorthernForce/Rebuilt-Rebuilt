#include "subsystems/Localizer.h"
#include "subsystems/apriltag/PhotonVisionCameraIO.h"
#include "subsystems/apriltag/LimeLightCameraIO.h"
#include "subsystems/apriltag/PhotonVisionCameraSimIO.h"
#include "constants/Constants.h"
#include <frc/RobotBase.h>

namespace nfr
{

Localizer::Localizer()
{
    // Start the timer - we haven't had any estimates yet
    m_timeSinceLastEstimate.Start();
    
    // Initialize cameras based on robot mode
    InitializeCameras();
}

void Localizer::InitializeCameras()
{
    if (frc::RobotBase::IsReal())
    {
        // Real robot - create PhotonVision and LimeLight cameras
        
        // PhotonVision cameras
        auto frontLeftIO = std::make_unique<PhotonVisionCameraIO>(
            CameraConstants::kFrontLeftCameraName,
            CameraConstants::kFrontLeftCameraTransform);
        m_cameras.push_back(std::make_unique<AprilTagCamera>(
            std::move(frontLeftIO), "FrontLeft"));
            
        auto centerIO = std::make_unique<PhotonVisionCameraIO>(
            CameraConstants::kCenterCameraName,
            CameraConstants::kCenterCameraTransform);
        m_cameras.push_back(std::make_unique<AprilTagCamera>(
            std::move(centerIO), "Center"));
        
        // LimeLight cameras
        auto frontRightIO = std::make_unique<LimeLightCameraIO>(
            CameraConstants::kFrontRightCameraName,
            CameraConstants::kFrontRightCameraTransform);
        m_cameras.push_back(std::make_unique<AprilTagCamera>(
            std::move(frontRightIO), "FrontRight"));
            
        auto centerBackIO = std::make_unique<LimeLightCameraIO>(
            CameraConstants::kCenterBackCameraName,
            CameraConstants::kCenterBackCameraTransform);
        m_cameras.push_back(std::make_unique<AprilTagCamera>(
            std::move(centerBackIO), "CenterBack"));
    }
    else
    {
        // Simulation - create PhotonVision simulation cameras
        auto frontLeftSimIO = std::make_unique<PhotonVisionCameraSimIO>(
            CameraConstants::kFrontLeftCameraName,
            CameraConstants::kFrontLeftCameraTransform);
        m_cameras.push_back(std::make_unique<AprilTagCamera>(
            std::move(frontLeftSimIO), "FrontLeft-Sim"));
            
        auto centerSimIO = std::make_unique<PhotonVisionCameraSimIO>(
            CameraConstants::kCenterCameraName,
            CameraConstants::kCenterCameraTransform);
        m_cameras.push_back(std::make_unique<AprilTagCamera>(
            std::move(centerSimIO), "Center-Sim"));
    }
}

void Localizer::UpdateWithReferencePose(const frc::Pose2d& pose)
{
    // Convert Pose2d to Pose3d for camera reference
    frc::Pose3d pose3d{pose.X(), pose.Y(), 0_m, 
                       frc::Rotation3d{0_deg, 0_deg, pose.Rotation().Radians()}};
    
    // Update all cameras with reference pose
    for (auto& camera : m_cameras)
    {
        camera->SetReferencePose(pose3d);
    }
}

void Localizer::Periodic()
{
    // Clear previous estimates
    m_estimatedPoses.clear();
    
    // Update all cameras and collect poses
    bool hadAnyEstimates = false;
    
    for (auto& camera : m_cameras)
    {
        camera->Periodic();
        
        const auto& poses = camera->GetRobotPoses();
        const auto& timestamps = camera->GetTimestamps();
        
        // Add poses with camera identification
        for (size_t i = 0; i < poses.size() && i < timestamps.size(); ++i)
        {
            m_estimatedPoses.emplace_back(poses[i], timestamps[i], camera->GetName());
            hadAnyEstimates = true;
        }
    }
    
    // Restart timer if we got any estimates
    if (hadAnyEstimates)
    {
        m_timeSinceLastEstimate.Restart();
    }
}

const std::vector<EstimatedPose>& Localizer::GetEstimatedPoses() const
{
    return m_estimatedPoses;
}

std::vector<frc::Pose2d> Localizer::GetPosesFromCamera(const std::string& cameraName) const
{
    std::vector<frc::Pose2d> poses;
    
    for (const auto& estimate : m_estimatedPoses)
    {
        if (estimate.cameraName == cameraName)
        {
            poses.push_back(estimate.pose);
        }
    }
    
    return poses;
}

const std::vector<std::unique_ptr<AprilTagCamera>>& Localizer::GetCameras() const
{
    return m_cameras;
}

units::second_t Localizer::GetTimeSinceLastEstimate() const
{
    return m_timeSinceLastEstimate.Get();
}

bool Localizer::HasHadRecentEstimate() const
{
    return !m_timeSinceLastEstimate.HasElapsed(VisionConstants::kEstimateTimeout);
}

int Localizer::GetConnectedCameraCount() const
{
    int count = 0;
    for (const auto& camera : m_cameras)
    {
        if (camera->IsConnected())
        {
            count++;
        }
    }
    return count;
}

int Localizer::GetTotalTargetCount() const
{
    int total = 0;
    for (const auto& camera : m_cameras)
    {
        total += camera->GetTargetCount();
    }
    return total;
}

void Localizer::Log(const nfr::LogContext& log) const
{
    log["connected_cameras"] << GetConnectedCameraCount();
    log["total_cameras"] << static_cast<int>(m_cameras.size());
    log["total_targets"] << GetTotalTargetCount();
    log["total_poses"] << static_cast<int>(m_estimatedPoses.size());
    log["time_since_estimate"] << GetTimeSinceLastEstimate().value();
    log["has_recent_estimate"] << HasHadRecentEstimate();
    
    // Log each camera
    for (size_t i = 0; i < m_cameras.size(); ++i)
    {
        const auto& camera = m_cameras[i];
        camera->Log(log["cameras"][camera->GetName()]);
    }
    
    // Log recent poses summary
    if (!m_estimatedPoses.empty())
    {
        const auto& latestPose = m_estimatedPoses.back();
        log["latest_pose/x"] << latestPose.pose.X().value();
        log["latest_pose/y"] << latestPose.pose.Y().value();
        log["latest_pose/rotation"] << latestPose.pose.Rotation().Degrees().value();
        log["latest_pose/camera"] << latestPose.cameraName;
        log["latest_pose/timestamp"] << latestPose.timestamp.value();
    }
}

}  // namespace nfr