#include "subsystems/VisionSubsystem.h"
#include <frc/smartdashboard/SmartDashboard.h>

VisionSubsystem::VisionSubsystem() {
    SetName("VisionSubsystem");
    
    // Initialize NetworkTables
    m_visionTable = nt::NetworkTableInstance::GetDefault().GetTable("Vision");
    m_limelightTable = nt::NetworkTableInstance::GetDefault().GetTable("limelight");
    
    // Set initial pipeline
    SetPipeline(kAprilTagPipeline);
    
    // Configure camera
    SetLEDMode(false); // Start with LEDs off
    
    // Initialize telemetry
    UpdateTelemetry();
}

void VisionSubsystem::Periodic() {
    UpdateVision();
    UpdateTelemetry();
}

void VisionSubsystem::UpdateVision() {
    if (!m_visionEnabled) return;
    
    // Simplified vision processing using NetworkTables data
    // This would normally interface with PhotonVision or Limelight
    
    if (GetCurrentPipeline() == kAprilTagPipeline) {
        // Process AprilTag data from NetworkTables
        double tv = m_limelightTable->GetNumber("tv", 0.0);
        double tx = m_limelightTable->GetNumber("tx", 0.0);
        double ty = m_limelightTable->GetNumber("ty", 0.0);
        double ta = m_limelightTable->GetNumber("ta", 0.0);
        double tid = m_limelightTable->GetNumber("tid", -1.0);
        
        m_lastTarget = VisionTarget{};
        if (tv > 0.5) {
            m_lastTarget.id = static_cast<int>(tid);
            m_lastTarget.yaw = units::degree_t{tx};
            m_lastTarget.pitch = units::degree_t{ty};
            m_lastTarget.area = ta;
            m_lastTarget.distance = 2.0; // Simplified distance calculation
            m_lastTarget.isValid = true;
        }
    } else if (GetCurrentPipeline() == kGamePiecePipeline) {
        // Process game piece data
        double tx = m_limelightTable->GetNumber("tx", 0.0);
        double ty = m_limelightTable->GetNumber("ty", 0.0);
        double ta = m_limelightTable->GetNumber("ta", 0.0);
        double tv = m_limelightTable->GetNumber("tv", 0.0);
        
        m_lastGamePiece.centerX = tx;
        m_lastGamePiece.centerY = ty;
        m_lastGamePiece.area = ta;
        m_lastGamePiece.yaw = units::degree_t{tx};
        m_lastGamePiece.isDetected = tv > 0.5;
    }
}

std::optional<VisionSubsystem::VisionTarget> VisionSubsystem::GetBestTarget() {
    if (m_lastTarget.isValid) {
        return m_lastTarget;
    }
    return std::nullopt;
}

bool VisionSubsystem::HasTargets() {
    return m_lastTarget.isValid;
}

VisionSubsystem::GamePieceTarget VisionSubsystem::GetGamePieceTarget() {
    return m_lastGamePiece;
}

bool VisionSubsystem::IsGamePieceDetected() {
    return m_lastGamePiece.isDetected;
}

void VisionSubsystem::SetLEDMode(bool on) {
    if (on) {
        m_limelightTable->PutNumber("ledMode", 3); // Force on
    } else {
        m_limelightTable->PutNumber("ledMode", 1); // Force off
    }
}

void VisionSubsystem::SetPipeline(int pipeline) {
    m_currentPipeline = std::clamp(pipeline, 0, 9);
    m_limelightTable->PutNumber("pipeline", m_currentPipeline);
}

void VisionSubsystem::SetVisionEnabled(bool enabled) {
    m_visionEnabled = enabled;
}

void VisionSubsystem::UpdateTelemetry() {
    // Update SmartDashboard
    frc::SmartDashboard::PutBoolean("Vision/Enabled", m_visionEnabled);
    frc::SmartDashboard::PutNumber("Vision/Pipeline", GetCurrentPipeline());
    frc::SmartDashboard::PutBoolean("Vision/HasTargets", HasTargets());
    frc::SmartDashboard::PutBoolean("Vision/GamePieceDetected", IsGamePieceDetected());
    
    if (HasTargets()) {
        frc::SmartDashboard::PutNumber("Vision/TargetID", m_lastTarget.id);
        frc::SmartDashboard::PutNumber("Vision/TargetDistance", m_lastTarget.distance);
        frc::SmartDashboard::PutNumber("Vision/TargetYaw", m_lastTarget.yaw.value());
        frc::SmartDashboard::PutNumber("Vision/TargetArea", m_lastTarget.area);
    }
    
    if (IsGamePieceDetected()) {
        frc::SmartDashboard::PutNumber("Vision/GamePieceX", m_lastGamePiece.centerX);
        frc::SmartDashboard::PutNumber("Vision/GamePieceY", m_lastGamePiece.centerY);
        frc::SmartDashboard::PutNumber("Vision/GamePieceArea", m_lastGamePiece.area);
    }
    
    // Update NetworkTables for dashboard
    m_visionTable->PutBoolean("enabled", m_visionEnabled);
    m_visionTable->PutNumber("pipeline", GetCurrentPipeline());
    m_visionTable->PutBoolean("hasTargets", HasTargets());
    m_visionTable->PutBoolean("gamePieceDetected", IsGamePieceDetected());
    
    if (HasTargets()) {
        m_visionTable->PutNumber("targetId", m_lastTarget.id);
        m_visionTable->PutNumber("targetDistance", m_lastTarget.distance);
        m_visionTable->PutNumber("targetYaw", m_lastTarget.yaw.value());
        m_visionTable->PutNumber("targetPitch", m_lastTarget.pitch.value());
        m_visionTable->PutNumber("targetArea", m_lastTarget.area);
    }
    
    if (IsGamePieceDetected()) {
        m_visionTable->PutNumber("gamePieceX", m_lastGamePiece.centerX);
        m_visionTable->PutNumber("gamePieceY", m_lastGamePiece.centerY);
        m_visionTable->PutNumber("gamePieceArea", m_lastGamePiece.area);
        m_visionTable->PutNumber("gamePieceYaw", m_lastGamePiece.yaw.value());
    }
}