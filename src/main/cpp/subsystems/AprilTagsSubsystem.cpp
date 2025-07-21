#include "subsystems/AprilTagsSubsystem.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Timer.h>

AprilTagsSubsystem::AprilTagsSubsystem() {
    SetName("AprilTagsSubsystem");
    
    // Initialize NetworkTables
    m_aprilTagTable = nt::NetworkTableInstance::GetDefault().GetTable("AprilTags");
    m_limelightTable = nt::NetworkTableInstance::GetDefault().GetTable("limelight");
    
    // Initialize telemetry
    UpdateTelemetry();
}

void AprilTagsSubsystem::Periodic() {
    UpdatePoseEstimation();
    UpdateTelemetry();
}

void AprilTagsSubsystem::UpdatePoseEstimation() {
    if (!m_estimationEnabled) return;
    
    // Simplified pose estimation using NetworkTables
    // This would normally use PhotonVision's pose estimator
    
    double tv = m_limelightTable->GetNumber("tv", 0.0);
    double tid = m_limelightTable->GetNumber("tid", -1.0);
    
    m_lastPoseEstimate = PoseEstimate{}; // Reset
    m_visibleTags.clear();
    
    if (tv > 0.5 && tid >= 0) {
        // Simple pose estimation based on tag detection
        // This is a placeholder - real implementation would use field layout
        
        m_visibleTags.push_back(static_cast<int>(tid));
        
        // Create a simple pose estimate (placeholder)
        m_lastPoseEstimate.pose = m_referencePose; // Use reference pose for now
        m_lastPoseEstimate.timestamp = frc::Timer::GetFPGATimestamp();
        m_lastPoseEstimate.ambiguity = 0.1; // Low ambiguity
        m_lastPoseEstimate.tagIds = m_visibleTags;
        m_lastPoseEstimate.isValid = true;
    }
}

std::optional<AprilTagsSubsystem::PoseEstimate> AprilTagsSubsystem::GetRobotPose() {
    if (m_lastPoseEstimate.isValid) {
        return m_lastPoseEstimate;
    }
    return std::nullopt;
}

bool AprilTagsSubsystem::HasValidPose() {
    return m_lastPoseEstimate.isValid;
}

int AprilTagsSubsystem::GetVisibleTagCount() {
    return m_visibleTags.size();
}

std::vector<int> AprilTagsSubsystem::GetVisibleTagIds() {
    return m_visibleTags;
}

void AprilTagsSubsystem::SetReferencePose(const frc::Pose2d& pose) {
    m_referencePose = pose;
}

void AprilTagsSubsystem::SetEstimationEnabled(bool enabled) {
    m_estimationEnabled = enabled;
}

void AprilTagsSubsystem::UpdateTelemetry() {
    // Update SmartDashboard
    frc::SmartDashboard::PutBoolean("AprilTags/EstimationEnabled", m_estimationEnabled);
    frc::SmartDashboard::PutBoolean("AprilTags/HasValidPose", HasValidPose());
    frc::SmartDashboard::PutNumber("AprilTags/VisibleTagCount", GetVisibleTagCount());
    
    if (HasValidPose()) {
        frc::SmartDashboard::PutNumber("AprilTags/PoseX", m_lastPoseEstimate.pose.X().value());
        frc::SmartDashboard::PutNumber("AprilTags/PoseY", m_lastPoseEstimate.pose.Y().value());
        frc::SmartDashboard::PutNumber("AprilTags/PoseRotation", m_lastPoseEstimate.pose.Rotation().Degrees().value());
        frc::SmartDashboard::PutNumber("AprilTags/PoseAmbiguity", m_lastPoseEstimate.ambiguity);
        frc::SmartDashboard::PutNumber("AprilTags/PoseTimestamp", m_lastPoseEstimate.timestamp.value());
        
        // Put visible tag IDs as string
        std::string tagIdStr;
        for (size_t i = 0; i < m_visibleTags.size(); ++i) {
            if (i > 0) tagIdStr += ",";
            tagIdStr += std::to_string(m_visibleTags[i]);
        }
        frc::SmartDashboard::PutString("AprilTags/VisibleTagIds", tagIdStr);
    }
    
    // Update NetworkTables for dashboard
    m_aprilTagTable->PutBoolean("estimationEnabled", m_estimationEnabled);
    m_aprilTagTable->PutBoolean("hasValidPose", HasValidPose());
    m_aprilTagTable->PutNumber("visibleTagCount", GetVisibleTagCount());
    
    if (HasValidPose()) {
        m_aprilTagTable->PutNumber("poseX", m_lastPoseEstimate.pose.X().value());
        m_aprilTagTable->PutNumber("poseY", m_lastPoseEstimate.pose.Y().value());
        m_aprilTagTable->PutNumber("poseRotation", m_lastPoseEstimate.pose.Rotation().Degrees().value());
        m_aprilTagTable->PutNumber("poseAmbiguity", m_lastPoseEstimate.ambiguity);
        m_aprilTagTable->PutNumber("poseTimestamp", m_lastPoseEstimate.timestamp.value());
        
        // Put visible tag IDs as array
        std::vector<double> tagIds;
        for (int id : m_visibleTags) {
            tagIds.push_back(static_cast<double>(id));
        }
        m_aprilTagTable->PutNumberArray("visibleTagIds", tagIds);
    }
}