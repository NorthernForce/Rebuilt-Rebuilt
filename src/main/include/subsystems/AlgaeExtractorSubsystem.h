#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/Command.h>
#include <frc/simulation/PWMSim.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/motorcontrol/PWMSparkMax.h>
#include <frc/DigitalInput.h>
#include <frc/simulation/DIOSim.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

/**
 * AlgaeExtractorSubsystem - Controls algae extraction mechanism
 * Provides extraction and return functionality for algae removal from reef
 */
class AlgaeExtractorSubsystem : public frc2::SubsystemBase {
public:
    AlgaeExtractorSubsystem();

    /**
     * Called periodically by the CommandScheduler
     */
    void Periodic() override;

    /**
     * Extract algae by extending the extractor arm
     */
    void ExtractAlgae();

    /**
     * Return the extractor arm to home position
     */
    void ReturnExtractor();

    /**
     * Stop extractor movement
     */
    void Stop();

    /**
     * Set extractor speed manually
     * @param speed Motor speed (-1.0 to 1.0)
     */
    void SetSpeed(double speed);

    /**
     * Check if extractor has reached the top (limit switch)
     * @return True if limit switch is pressed
     */
    bool HasReachedTop();

    /**
     * Check if extractor is at home position
     * @return True if at home position
     */
    bool IsAtHome();

    /**
     * Get current extractor position
     * @return Position in rotations (simulated)
     */
    double GetPosition();

    /**
     * Reset extractor position to home
     */
    void ResetPosition();

    /**
     * Get command to extract algae
     */
    frc2::CommandPtr GetExtractCommand();

    /**
     * Get command to return extractor to home
     */
    frc2::CommandPtr GetReturnCommand();

    /**
     * Get command for full algae extraction cycle
     */
    frc2::CommandPtr GetFullExtractionCycle();

private:
    // Motor and sensor hardware
    frc::PWMSparkMax m_extractorMotor;
    frc::DigitalInput m_limitSwitch;
    
    // Simulation
    frc::sim::PWMSim m_extractorMotorSim;
    frc::sim::DIOSim m_limitSwitchSim;
    double m_simPosition{0.0};
    
    // Constants
    static constexpr double kExtractSpeed = 0.6;
    static constexpr double kReturnSpeed = 0.4;
    static constexpr double kMaxPosition = 50.0; // Max extension in rotations
    static constexpr double kMinPosition = 0.0;  // Home position
    static constexpr double kPositionTolerance = 1.0;
    static constexpr double kTopPosition = 48.0; // Position where limit switch triggers
    
    // NetworkTables
    std::shared_ptr<nt::NetworkTable> m_extractorTable;
    
    void UpdateSimulation();
    void UpdateTelemetry();
};