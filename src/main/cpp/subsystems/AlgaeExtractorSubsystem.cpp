#include "subsystems/AlgaeExtractorSubsystem.h"
#include <frc2/command/Commands.h>
#include <frc2/command/WaitUntilCommand.h>
#include <frc/smartdashboard/SmartDashboard.h>

AlgaeExtractorSubsystem::AlgaeExtractorSubsystem() 
    : m_extractorMotor(10), // PWM channel 10 for extractor motor
      m_limitSwitch(0),     // DIO channel 0 for limit switch
      m_extractorMotorSim(m_extractorMotor.GetChannel()),
      m_limitSwitchSim(m_limitSwitch.GetChannel()) {
    
    SetName("AlgaeExtractorSubsystem");
    
    // Configure motor
    m_extractorMotor.SetInverted(false);
    
    // Initialize NetworkTables
    m_extractorTable = nt::NetworkTableInstance::GetDefault().GetTable("AlgaeExtractor");
    
    // Initialize telemetry
    UpdateTelemetry();
}

void AlgaeExtractorSubsystem::Periodic() {
    UpdateSimulation();
    UpdateTelemetry();
}

void AlgaeExtractorSubsystem::ExtractAlgae() {
    if (!HasReachedTop()) {
        m_extractorMotor.Set(kExtractSpeed);
    } else {
        Stop();
    }
}

void AlgaeExtractorSubsystem::ReturnExtractor() {
    if (!IsAtHome()) {
        m_extractorMotor.Set(-kReturnSpeed);
    } else {
        Stop();
    }
}

void AlgaeExtractorSubsystem::Stop() {
    m_extractorMotor.Set(0.0);
}

void AlgaeExtractorSubsystem::SetSpeed(double speed) {
    // Clamp speed to valid range
    speed = std::clamp(speed, -1.0, 1.0);
    
    // Prevent movement beyond limits
    if (speed > 0 && HasReachedTop()) {
        speed = 0.0;
    } else if (speed < 0 && IsAtHome()) {
        speed = 0.0;
    }
    
    m_extractorMotor.Set(speed);
}

bool AlgaeExtractorSubsystem::HasReachedTop() {
    // Limit switch returns false when pressed (normally closed)
    return !m_limitSwitch.Get() || (GetPosition() >= kTopPosition);
}

bool AlgaeExtractorSubsystem::IsAtHome() {
    return GetPosition() <= (kMinPosition + kPositionTolerance);
}

double AlgaeExtractorSubsystem::GetPosition() {
    return m_simPosition; // In real implementation, would read encoder
}

void AlgaeExtractorSubsystem::ResetPosition() {
    m_simPosition = kMinPosition;
}

frc2::CommandPtr AlgaeExtractorSubsystem::GetExtractCommand() {
    return frc2::cmd::Run([this] { ExtractAlgae(); }, {this})
           .Until([this] { return HasReachedTop(); })
           .AndThen([this] { Stop(); })
           .WithName("ExtractAlgae");
}

frc2::CommandPtr AlgaeExtractorSubsystem::GetReturnCommand() {
    return frc2::cmd::Run([this] { ReturnExtractor(); }, {this})
           .Until([this] { return IsAtHome(); })
           .AndThen([this] { Stop(); })
           .WithName("ReturnExtractor");
}

frc2::CommandPtr AlgaeExtractorSubsystem::GetFullExtractionCycle() {
    return GetExtractCommand()
           .AndThen(frc2::cmd::Wait(0.5_s)) // Brief pause at top
           .AndThen(GetReturnCommand())
           .WithName("FullAlgaeExtraction");
}

void AlgaeExtractorSubsystem::UpdateSimulation() {
    // Simulate extractor position based on motor output
    double motorOutput = m_extractorMotorSim.GetSpeed();
    
    if (std::abs(motorOutput) > 0.1) {
        // Update position based on motor speed
        m_simPosition += motorOutput * 1.5; // 1.5 rotations per second at full speed
        
        // Clamp to valid range
        m_simPosition = std::clamp(m_simPosition, kMinPosition, kMaxPosition);
    }
    
    // Simulate limit switch
    bool limitPressed = m_simPosition >= kTopPosition;
    m_limitSwitchSim.SetValue(!limitPressed); // Invert because switch is normally closed
}

void AlgaeExtractorSubsystem::UpdateTelemetry() {
    // Update SmartDashboard
    frc::SmartDashboard::PutNumber("AlgaeExtractor/Position", GetPosition());
    frc::SmartDashboard::PutNumber("AlgaeExtractor/MotorOutput", m_extractorMotor.Get());
    frc::SmartDashboard::PutBoolean("AlgaeExtractor/HasReachedTop", HasReachedTop());
    frc::SmartDashboard::PutBoolean("AlgaeExtractor/IsAtHome", IsAtHome());
    frc::SmartDashboard::PutBoolean("AlgaeExtractor/LimitSwitch", !m_limitSwitch.Get());
    
    // Update NetworkTables for dashboard
    m_extractorTable->PutNumber("position", GetPosition());
    m_extractorTable->PutNumber("motorOutput", m_extractorMotor.Get());
    m_extractorTable->PutBoolean("hasReachedTop", HasReachedTop());
    m_extractorTable->PutBoolean("isAtHome", IsAtHome());
    m_extractorTable->PutBoolean("limitSwitch", !m_limitSwitch.Get());
    m_extractorTable->PutNumber("maxPosition", kMaxPosition);
    m_extractorTable->PutNumber("minPosition", kMinPosition);
}