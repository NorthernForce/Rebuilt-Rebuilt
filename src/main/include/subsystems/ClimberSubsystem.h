#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/Command.h>
#include <frc/simulation/PWMSim.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/motorcontrol/PWMSparkMax.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

/**
 * ClimberSubsystem - Controls robot climbing mechanism for end game
 * Provides extend and retract functionality for climbing bars
 */
class ClimberSubsystem : public frc2::SubsystemBase {
public:
    ClimberSubsystem();

    /**
     * Called periodically by the CommandScheduler
     */
    void Periodic() override;

    /**
     * Extend the climber arms
     */
    void Extend();

    /**
     * Retract the climber arms  
     */
    void Retract();

    /**
     * Stop climber movement
     */
    void Stop();

    /**
     * Set climber speed manually
     * @param speed Motor speed (-1.0 to 1.0)
     */
    void SetSpeed(double speed);

    /**
     * Get current climber position
     * @return Position in rotations (simulated)
     */
    double GetPosition();

    /**
     * Check if climber is fully extended
     * @return True if at maximum extension
     */
    bool IsFullyExtended();

    /**
     * Check if climber is fully retracted
     * @return True if at minimum position
     */
    bool IsFullyRetracted();

    /**
     * Reset climber position (for simulation)
     */
    void ResetPosition();

    /**
     * Get command to extend climber
     */
    frc2::CommandPtr GetExtendCommand();

    /**
     * Get command to retract climber  
     */
    frc2::CommandPtr GetRetractCommand();

private:
    // Motor control
    frc::PWMSparkMax m_climbMotor;
    
    // Simulation
    frc::sim::PWMSim m_climbMotorSim;
    double m_simPosition{0.0};
    
    // Constants
    static constexpr double kClimbSpeed = 0.8;
    static constexpr double kMaxPosition = 100.0; // Max extension in rotations
    static constexpr double kMinPosition = 0.0;   // Fully retracted
    static constexpr double kPositionTolerance = 2.0;
    
    // NetworkTables
    std::shared_ptr<nt::NetworkTable> m_climbTable;
    
    void UpdateSimulation();
    void UpdateTelemetry();
};