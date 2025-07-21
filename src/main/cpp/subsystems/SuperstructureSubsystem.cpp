#include "subsystems/SuperstructureSubsystem.h"
#include <frc2/command/Commands.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/WaitUntilCommand.h>

SuperstructureSubsystem::SuperstructureSubsystem() 
    : m_innerElevatorMotor(11), // PWM channel 11 for inner elevator
      m_outerElevatorMotor(12), // PWM channel 12 for outer elevator
      m_innerElevatorMotorSim(m_innerElevatorMotor.GetChannel()),
      m_outerElevatorMotorSim(m_outerElevatorMotor.GetChannel()) {
    
    SetName("SuperstructureSubsystem");
    
    // Configure motors
    m_innerElevatorMotor.SetInverted(false);
    m_outerElevatorMotor.SetInverted(false);
    
    // Initialize NetworkTables
    m_superstructureTable = nt::NetworkTableInstance::GetDefault().GetTable("Superstructure");
    
    // Start at home position
    ResetToHome();
    
    // Initialize telemetry
    UpdateTelemetry();
}

void SuperstructureSubsystem::Periodic() {
    UpdateSimulation();
    UpdateTelemetry();
}

void SuperstructureSubsystem::MoveToLevel(ScoringLevel level) {
    m_targetLevel = level;
    SuperstructureState targetState = GetTargetStateForLevel(level);
    
    // Calculate required movements
    double innerError = targetState.innerElevatorHeight.value() - m_simInnerPosition;
    double outerError = targetState.outerElevatorHeight.value() - m_simOuterPosition;
    
    // Set motor speeds based on error (simple proportional control)
    double innerSpeed = std::clamp(innerError * 2.0, -kElevatorSpeed, kElevatorSpeed);
    double outerSpeed = std::clamp(outerError * 2.0, -kElevatorSpeed, kElevatorSpeed);
    
    // Apply deadband
    if (std::abs(innerError) < kHeightTolerance) innerSpeed = 0.0;
    if (std::abs(outerError) < kHeightTolerance) outerSpeed = 0.0;
    
    SetInnerElevatorSpeed(innerSpeed);
    SetOuterElevatorSpeed(outerSpeed);
}

void SuperstructureSubsystem::SetInnerElevatorSpeed(double speed) {
    // Clamp speed and check limits
    speed = std::clamp(speed, -1.0, 1.0);
    
    if (speed > 0 && m_simInnerPosition >= kMaxInnerHeight) speed = 0.0;
    if (speed < 0 && m_simInnerPosition <= kMinHeight) speed = 0.0;
    
    m_innerElevatorMotor.Set(speed);
}

void SuperstructureSubsystem::SetOuterElevatorSpeed(double speed) {
    // Clamp speed and check limits
    speed = std::clamp(speed, -1.0, 1.0);
    
    if (speed > 0 && m_simOuterPosition >= kMaxOuterHeight) speed = 0.0;
    if (speed < 0 && m_simOuterPosition <= kMinHeight) speed = 0.0;
    
    m_outerElevatorMotor.Set(speed);
}

void SuperstructureSubsystem::Stop() {
    m_innerElevatorMotor.Set(0.0);
    m_outerElevatorMotor.Set(0.0);
}

SuperstructureSubsystem::SuperstructureState SuperstructureSubsystem::GetState() {
    return SuperstructureState{
        units::meter_t{m_simInnerPosition},
        units::meter_t{m_simOuterPosition}
    };
}

bool SuperstructureSubsystem::IsAtTargetHeight(ScoringLevel level) {
    SuperstructureState targetState = GetTargetStateForLevel(level);
    return IsAtHeight(targetState.innerElevatorHeight, targetState.outerElevatorHeight);
}

bool SuperstructureSubsystem::IsAtHeight(units::meter_t innerHeight, units::meter_t outerHeight) {
    return std::abs(m_simInnerPosition - innerHeight.value()) < kHeightTolerance &&
           std::abs(m_simOuterPosition - outerHeight.value()) < kHeightTolerance;
}

SuperstructureSubsystem::ScoringLevel SuperstructureSubsystem::GetCurrentLevel() {
    // Determine current level based on position
    if (IsAtTargetHeight(ScoringLevel::L1)) return ScoringLevel::L1;
    if (IsAtTargetHeight(ScoringLevel::L2)) return ScoringLevel::L2;
    if (IsAtTargetHeight(ScoringLevel::L3)) return ScoringLevel::L3;
    if (IsAtTargetHeight(ScoringLevel::L4)) return ScoringLevel::L4;
    if (IsAtTargetHeight(ScoringLevel::HOME)) return ScoringLevel::HOME;
    if (IsAtTargetHeight(ScoringLevel::CORAL_STATION)) return ScoringLevel::CORAL_STATION;
    
    return ScoringLevel::HOME; // Default
}

void SuperstructureSubsystem::ResetToHome() {
    m_simInnerPosition = kHomeInnerHeight;
    m_simOuterPosition = kHomeOuterHeight;
    m_targetLevel = ScoringLevel::HOME;
    m_isHomed = true;
    Stop();
}

bool SuperstructureSubsystem::IsHomed() {
    return m_isHomed && IsAtTargetHeight(ScoringLevel::HOME);
}

SuperstructureSubsystem::SuperstructureState SuperstructureSubsystem::GetTargetStateForLevel(ScoringLevel level) {
    switch (level) {
        case ScoringLevel::L1:
            return SuperstructureState{units::meter_t{kL1InnerHeight}, units::meter_t{kL1OuterHeight}};
        case ScoringLevel::L2:
            return SuperstructureState{units::meter_t{kL2InnerHeight}, units::meter_t{kL2OuterHeight}};
        case ScoringLevel::L3:
            return SuperstructureState{units::meter_t{kL3InnerHeight}, units::meter_t{kL3OuterHeight}};
        case ScoringLevel::L4:
            return SuperstructureState{units::meter_t{kL4InnerHeight}, units::meter_t{kL4OuterHeight}};
        case ScoringLevel::CORAL_STATION:
            return SuperstructureState{units::meter_t{kCoralStationInnerHeight}, units::meter_t{kCoralStationOuterHeight}};
        case ScoringLevel::HOME:
        default:
            return SuperstructureState{units::meter_t{kHomeInnerHeight}, units::meter_t{kHomeOuterHeight}};
    }
}

// Command factory methods
frc2::CommandPtr SuperstructureSubsystem::GetMoveToL1Command() {
    return frc2::cmd::Run([this] { MoveToLevel(ScoringLevel::L1); }, {this})
           .Until([this] { return IsAtTargetHeight(ScoringLevel::L1); })
           .AndThen([this] { Stop(); })
           .WithName("MoveToL1");
}

frc2::CommandPtr SuperstructureSubsystem::GetMoveToL2Command() {
    return frc2::cmd::Run([this] { MoveToLevel(ScoringLevel::L2); }, {this})
           .Until([this] { return IsAtTargetHeight(ScoringLevel::L2); })
           .AndThen([this] { Stop(); })
           .WithName("MoveToL2");
}

frc2::CommandPtr SuperstructureSubsystem::GetMoveToL3Command() {
    return frc2::cmd::Run([this] { MoveToLevel(ScoringLevel::L3); }, {this})
           .Until([this] { return IsAtTargetHeight(ScoringLevel::L3); })
           .AndThen([this] { Stop(); })
           .WithName("MoveToL3");
}

frc2::CommandPtr SuperstructureSubsystem::GetMoveToL4Command() {
    return frc2::cmd::Run([this] { MoveToLevel(ScoringLevel::L4); }, {this})
           .Until([this] { return IsAtTargetHeight(ScoringLevel::L4); })
           .AndThen([this] { Stop(); })
           .WithName("MoveToL4");
}

frc2::CommandPtr SuperstructureSubsystem::GetMoveToHomeCommand() {
    return frc2::cmd::Run([this] { MoveToLevel(ScoringLevel::HOME); }, {this})
           .Until([this] { return IsAtTargetHeight(ScoringLevel::HOME); })
           .AndThen([this] { Stop(); })
           .WithName("MoveToHome");
}

frc2::CommandPtr SuperstructureSubsystem::GetMoveToCoralStationCommand() {
    return frc2::cmd::Run([this] { MoveToLevel(ScoringLevel::CORAL_STATION); }, {this})
           .Until([this] { return IsAtTargetHeight(ScoringLevel::CORAL_STATION); })
           .AndThen([this] { Stop(); })
           .WithName("MoveToCoralStation");
}

frc2::CommandPtr SuperstructureSubsystem::GetHoldAtLevelCommand(ScoringLevel level) {
    return frc2::cmd::Run([this, level] { MoveToLevel(level); }, {this})
           .WithName("HoldAt" + LevelToString(level));
}

frc2::CommandPtr SuperstructureSubsystem::GetManualControlCommand(std::function<double()> innerSupplier, 
                                                                std::function<double()> outerSupplier) {
    return frc2::cmd::Run([this, innerSupplier, outerSupplier] {
        SetInnerElevatorSpeed(innerSupplier() * kElevatorSpeed);
        SetOuterElevatorSpeed(outerSupplier() * kElevatorSpeed);
    }, {this}).WithName("ManualElevatorControl");
}

void SuperstructureSubsystem::UpdateSimulation() {
    // Update positions based on motor outputs
    double innerOutput = m_innerElevatorMotorSim.GetSpeed();
    double outerOutput = m_outerElevatorMotorSim.GetSpeed();
    
    if (std::abs(innerOutput) > 0.1) {
        m_simInnerPosition += innerOutput * 0.02; // 2cm per 20ms at full speed
        m_simInnerPosition = std::clamp(m_simInnerPosition, kMinHeight, kMaxInnerHeight);
    }
    
    if (std::abs(outerOutput) > 0.1) {
        m_simOuterPosition += outerOutput * 0.02; // 2cm per 20ms at full speed
        m_simOuterPosition = std::clamp(m_simOuterPosition, kMinHeight, kMaxOuterHeight);
    }
}

void SuperstructureSubsystem::UpdateTelemetry() {
    SuperstructureState currentState = GetState();
    ScoringLevel currentLevel = GetCurrentLevel();
    
    // Update SmartDashboard
    frc::SmartDashboard::PutNumber("Superstructure/InnerHeight", currentState.innerElevatorHeight.value());
    frc::SmartDashboard::PutNumber("Superstructure/OuterHeight", currentState.outerElevatorHeight.value());
    frc::SmartDashboard::PutString("Superstructure/CurrentLevel", LevelToString(currentLevel));
    frc::SmartDashboard::PutString("Superstructure/TargetLevel", LevelToString(m_targetLevel));
    frc::SmartDashboard::PutBoolean("Superstructure/IsHomed", IsHomed());
    frc::SmartDashboard::PutBoolean("Superstructure/AtTarget", IsAtTargetHeight(m_targetLevel));
    
    // Update NetworkTables for dashboard
    m_superstructureTable->PutNumber("innerHeight", currentState.innerElevatorHeight.value());
    m_superstructureTable->PutNumber("outerHeight", currentState.outerElevatorHeight.value());
    m_superstructureTable->PutString("currentLevel", LevelToString(currentLevel));
    m_superstructureTable->PutString("targetLevel", LevelToString(m_targetLevel));
    m_superstructureTable->PutBoolean("isHomed", IsHomed());
    m_superstructureTable->PutBoolean("atTarget", IsAtTargetHeight(m_targetLevel));
}

std::string SuperstructureSubsystem::LevelToString(ScoringLevel level) {
    switch (level) {
        case ScoringLevel::L1: return "L1";
        case ScoringLevel::L2: return "L2";
        case ScoringLevel::L3: return "L3";
        case ScoringLevel::L4: return "L4";
        case ScoringLevel::HOME: return "HOME";
        case ScoringLevel::CORAL_STATION: return "CORAL_STATION";
        default: return "UNKNOWN";
    }
}