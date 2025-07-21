#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/Command.h>
#include <frc/simulation/PWMSim.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/motorcontrol/PWMSparkMax.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <units/length.h>
#include <functional>

/**
 * SuperstructureSubsystem - Controls multi-stage elevator system
 * Provides movement to 4 scoring levels (L1-L4) for coral placement
 */
class SuperstructureSubsystem : public frc2::SubsystemBase {
public:
    enum class ScoringLevel {
        L1, // Ground level
        L2, // Low reef
        L3, // Mid reef  
        L4, // High reef
        HOME, // Starting position
        CORAL_STATION // Intake position
    };

    struct SuperstructureState {
        units::meter_t innerElevatorHeight{0_m};
        units::meter_t outerElevatorHeight{0_m};
        
        SuperstructureState() = default;
        SuperstructureState(units::meter_t inner, units::meter_t outer) 
            : innerElevatorHeight(inner), outerElevatorHeight(outer) {}
    };

    SuperstructureSubsystem();

    /**
     * Called periodically by the CommandScheduler
     */
    void Periodic() override;

    /**
     * Move to specified scoring level
     */
    void MoveToLevel(ScoringLevel level);

    /**
     * Set manual speed for inner elevator
     */
    void SetInnerElevatorSpeed(double speed);

    /**
     * Set manual speed for outer elevator
     */
    void SetOuterElevatorSpeed(double speed);

    /**
     * Stop all elevator movement
     */
    void Stop();

    /**
     * Get current superstructure state
     */
    SuperstructureState GetState();

    /**
     * Check if at target height
     */
    bool IsAtTargetHeight(ScoringLevel level);

    /**
     * Check if at specified heights with tolerance
     */
    bool IsAtHeight(units::meter_t innerHeight, units::meter_t outerHeight);

    /**
     * Get current scoring level based on position
     */
    ScoringLevel GetCurrentLevel();

    /**
     * Reset elevators to home position
     */
    void ResetToHome();

    /**
     * Check if elevators are homed
     */
    bool IsHomed();

    // Command factory methods
    frc2::CommandPtr GetMoveToL1Command();
    frc2::CommandPtr GetMoveToL2Command();
    frc2::CommandPtr GetMoveToL3Command();
    frc2::CommandPtr GetMoveToL4Command();
    frc2::CommandPtr GetMoveToHomeCommand();
    frc2::CommandPtr GetMoveToCoralStationCommand();
    frc2::CommandPtr GetHoldAtLevelCommand(ScoringLevel level);
    frc2::CommandPtr GetManualControlCommand(std::function<double()> innerSupplier, 
                                           std::function<double()> outerSupplier);

private:
    // Motor controllers
    frc::PWMSparkMax m_innerElevatorMotor;
    frc::PWMSparkMax m_outerElevatorMotor;
    
    // Simulation
    frc::sim::PWMSim m_innerElevatorMotorSim;
    frc::sim::PWMSim m_outerElevatorMotorSim;
    double m_simInnerPosition{0.0};  // Position in meters
    double m_simOuterPosition{0.0};  // Position in meters
    
    // Target positions for each level (in meters)
    static constexpr double kL1InnerHeight = 0.0;
    static constexpr double kL1OuterHeight = 0.0;
    static constexpr double kL2InnerHeight = 0.3;
    static constexpr double kL2OuterHeight = 0.2;
    static constexpr double kL3InnerHeight = 0.6;
    static constexpr double kL3OuterHeight = 0.5;
    static constexpr double kL4InnerHeight = 1.0;
    static constexpr double kL4OuterHeight = 0.8;
    static constexpr double kHomeInnerHeight = 0.0;
    static constexpr double kHomeOuterHeight = 0.0;
    static constexpr double kCoralStationInnerHeight = 0.1;
    static constexpr double kCoralStationOuterHeight = 0.0;
    
    // Constants
    static constexpr double kElevatorSpeed = 0.7;
    static constexpr double kMaxInnerHeight = 1.2;
    static constexpr double kMaxOuterHeight = 1.0;
    static constexpr double kMinHeight = 0.0;
    static constexpr double kHeightTolerance = 0.05; // 5cm tolerance
    
    // Current target
    ScoringLevel m_targetLevel{ScoringLevel::HOME};
    bool m_isHomed{false};
    
    // NetworkTables
    std::shared_ptr<nt::NetworkTable> m_superstructureTable;
    
    SuperstructureState GetTargetStateForLevel(ScoringLevel level);
    void UpdateSimulation();
    void UpdateTelemetry();
    std::string LevelToString(ScoringLevel level);
};