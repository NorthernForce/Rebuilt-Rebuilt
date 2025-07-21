#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/geometry/Pose2d.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <units/length.h>
#include <units/angle.h>
#include <optional>

/**
 * VisionSubsystem - Handles vision processing for robot localization and targeting
 * Simplified version without PhotonVision dependencies for initial implementation
 */
class VisionSubsystem : public frc2::SubsystemBase {
public:
    struct VisionTarget {
        int id{-1};
        double distance{0.0};
        units::degree_t yaw{0_deg};
        units::degree_t pitch{0_deg};
        double area{0.0};
        bool isValid{false};
    };

    struct GamePieceTarget {
        double centerX{0.0};
        double centerY{0.0};
        double area{0.0};
        units::degree_t yaw{0_deg};
        bool isDetected{false};
    };

    VisionSubsystem();

    /**
     * Called periodically by the CommandScheduler
     */
    void Periodic() override;

    /**
     * Get the best AprilTag target currently visible
     */
    std::optional<VisionTarget> GetBestTarget();

    /**
     * Check if any AprilTag is visible
     */
    bool HasTargets();

    /**
     * Get detected game piece information
     */
    GamePieceTarget GetGamePieceTarget();

    /**
     * Check if a game piece is detected
     */
    bool IsGamePieceDetected();

    /**
     * Set camera LED mode
     */
    void SetLEDMode(bool on);

    /**
     * Set vision processing pipeline
     * @param pipeline Pipeline index (0-9)
     */
    void SetPipeline(int pipeline);

    /**
     * Get current pipeline index
     */
    int GetCurrentPipeline() { return m_currentPipeline; }

    /**
     * Enable/disable vision processing
     */
    void SetVisionEnabled(bool enabled);

    /**
     * Check if vision system is enabled
     */
    bool IsVisionEnabled() const { return m_visionEnabled; }

private:
    // State
    bool m_visionEnabled{true};
    int m_currentPipeline{0};
    VisionTarget m_lastTarget;
    GamePieceTarget m_lastGamePiece;
    
    // Pipeline constants
    static constexpr int kAprilTagPipeline = 0;
    static constexpr int kGamePiecePipeline = 1;
    
    // NetworkTables
    std::shared_ptr<nt::NetworkTable> m_visionTable;
    std::shared_ptr<nt::NetworkTable> m_limelightTable;
    
    void UpdateVision();
    void UpdateTelemetry();
};