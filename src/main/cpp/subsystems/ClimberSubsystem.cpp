#include "subsystems/ClimberSubsystem.h"
#include <frc2/command/Commands.h>
#include <frc/smartdashboard/SmartDashboard.h>

ClimberSubsystem::ClimberSubsystem() 
    : m_climbMotor(9), // PWM channel 9 for climber motor
      m_climbMotorSim(m_climbMotor.GetChannel()) {
    
    SetName("ClimberSubsystem");
    
    // Configure motor
    m_climbMotor.SetInverted(false);
    
    // Initialize NetworkTables
    m_climbTable = nt::NetworkTableInstance::GetDefault().GetTable("Climber");
    
    // Initialize telemetry
    UpdateTelemetry();
}

void ClimberSubsystem::Periodic() {
    UpdateSimulation();
    UpdateTelemetry();
}

void ClimberSubsystem::Extend() {
    if (!IsFullyExtended()) {
        m_climbMotor.Set(kClimbSpeed);
    } else {
        Stop();
    }
}

void ClimberSubsystem::Retract() {
    if (!IsFullyRetracted()) {
        m_climbMotor.Set(-kClimbSpeed);
    } else {
        Stop();
    }
}

void ClimberSubsystem::Stop() {
    m_climbMotor.Set(0.0);
}

void ClimberSubsystem::SetSpeed(double speed) {
    // Clamp speed to valid range
    speed = std::clamp(speed, -1.0, 1.0);
    
    // Prevent movement beyond limits
    if (speed > 0 && IsFullyExtended()) {
        speed = 0.0;
    } else if (speed < 0 && IsFullyRetracted()) {
        speed = 0.0;
    }
    
    m_climbMotor.Set(speed);
}

double ClimberSubsystem::GetPosition() {
    return m_simPosition; // In real implementation, would read encoder
}

bool ClimberSubsystem::IsFullyExtended() {
    return GetPosition() >= (kMaxPosition - kPositionTolerance);
}

bool ClimberSubsystem::IsFullyRetracted() {
    return GetPosition() <= (kMinPosition + kPositionTolerance);
}

void ClimberSubsystem::ResetPosition() {
    m_simPosition = kMinPosition;
}

frc2::CommandPtr ClimberSubsystem::GetExtendCommand() {
    return frc2::cmd::Run([this] { Extend(); }, {this})
           .WithName("ExtendClimber");
}

frc2::CommandPtr ClimberSubsystem::GetRetractCommand() {
    return frc2::cmd::Run([this] { Retract(); }, {this})
           .WithName("RetractClimber");
}

void ClimberSubsystem::UpdateSimulation() {
    // Simulate climber position based on motor output
    double motorOutput = m_climbMotorSim.GetSpeed();
    
    if (std::abs(motorOutput) > 0.1) {
        // Update position based on motor speed (simplified simulation)
        m_simPosition += motorOutput * 2.0; // 2 rotations per second at full speed
        
        // Clamp to valid range
        m_simPosition = std::clamp(m_simPosition, kMinPosition, kMaxPosition);
    }
}

void ClimberSubsystem::UpdateTelemetry() {
    // Update SmartDashboard
    frc::SmartDashboard::PutNumber("Climber/Position", GetPosition());
    frc::SmartDashboard::PutNumber("Climber/MotorOutput", m_climbMotor.Get());
    frc::SmartDashboard::PutBoolean("Climber/FullyExtended", IsFullyExtended());
    frc::SmartDashboard::PutBoolean("Climber/FullyRetracted", IsFullyRetracted());
    
    // Update NetworkTables for dashboard
    m_climbTable->PutNumber("position", GetPosition());
    m_climbTable->PutNumber("motorOutput", m_climbMotor.Get());
    m_climbTable->PutBoolean("fullyExtended", IsFullyExtended());
    m_climbTable->PutBoolean("fullyRetracted", IsFullyRetracted());
    m_climbTable->PutNumber("maxPosition", kMaxPosition);
    m_climbTable->PutNumber("minPosition", kMinPosition);
}