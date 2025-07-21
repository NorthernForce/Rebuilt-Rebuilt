#include "subsystems/LEDSubsystem.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/util/Color.h>
#include <cmath>

LEDSubsystem::LEDSubsystem() 
    : m_led(0) { // PWM channel 0 for LED data
    
    SetName("LEDSubsystem");
    
    // Set LED strip length and start output
    m_led.SetLength(kLEDLength);
    m_led.SetData(m_ledBuffer);
    m_led.Start();
    
    // Initialize NetworkTables
    m_ledTable = nt::NetworkTableInstance::GetDefault().GetTable("LEDs");
    
    // Start with team color
    SetPattern(LEDPattern::SOLID_ALLIANCE);
    SetAllianceColor(false); // Default to blue alliance
}

void LEDSubsystem::Periodic() {
    UpdateLEDs();
    
    // Update SmartDashboard
    frc::SmartDashboard::PutString("LEDs/Pattern", PatternToString(m_currentPattern));
    frc::SmartDashboard::PutString("LEDs/Alliance", m_isRedAlliance ? "Red" : "Blue");
    frc::SmartDashboard::PutNumber("LEDs/Brightness", m_brightness);
    
    // Update NetworkTables for dashboard
    m_ledTable->PutString("pattern", PatternToString(m_currentPattern));
    m_ledTable->PutString("alliance", m_isRedAlliance ? "Red" : "Blue");
    m_ledTable->PutNumber("brightness", m_brightness);
}

void LEDSubsystem::SetPattern(LEDPattern pattern) {
    m_currentPattern = pattern;
    m_animationCounter = 0; // Reset animation state
}

void LEDSubsystem::SetAllianceColor(bool isRed) {
    m_isRedAlliance = isRed;
}

void LEDSubsystem::SetSolidColor(frc::Color color) {
    m_customColor = color;
    SetAllLEDs(color);
}

void LEDSubsystem::SetSolidColor(int r, int g, int b) {
    SetSolidColor(frc::Color{r, g, b});
}

void LEDSubsystem::TurnOff() {
    SetPattern(LEDPattern::OFF);
}

void LEDSubsystem::SetBrightness(double brightness) {
    m_brightness = std::clamp(brightness, 0.0, 1.0);
}

void LEDSubsystem::UpdateLEDs() {
    ApplyPattern();
    ApplyBrightness();
    m_led.SetData(m_ledBuffer);
    m_animationCounter++;
}

void LEDSubsystem::ApplyPattern() {
    switch (m_currentPattern) {
        case LEDPattern::OFF:
            SetAllLEDs(frc::Color::kBlack);
            break;
            
        case LEDPattern::SOLID_ALLIANCE:
            SetAllLEDs(GetAllianceColor());
            break;
            
        case LEDPattern::RAINBOW:
            ApplyRainbow();
            break;
            
        case LEDPattern::BREATHING_ALLIANCE:
            ApplyBreathing(GetAllianceColor());
            break;
            
        case LEDPattern::GAME_PIECE_DETECTED:
            ApplyBreathing(kGamePieceOrange);
            break;
            
        case LEDPattern::AUTONOMOUS_MODE:
            ApplyBreathing(kAutonomousGold);
            break;
            
        case LEDPattern::CLIMBING_MODE:
            // Alternating alliance color and team green
            for (int i = 0; i < kLEDLength; i++) {
                frc::Color color = ((i + m_animationCounter / 10) % 2 == 0) ? 
                                  GetAllianceColor() : kTeam172Green;
                m_ledBuffer[i].SetRGB(color.red * 255, color.green * 255, color.blue * 255);
            }
            break;
            
        case LEDPattern::ERROR_STATE:
            // Fast blinking red
            if ((m_animationCounter / 5) % 2 == 0) {
                SetAllLEDs(frc::Color::kRed);
            } else {
                SetAllLEDs(frc::Color::kBlack);
            }
            break;
            
        case LEDPattern::CELEBRATION:
            // Fast rainbow chase
            ApplyRainbow();
            m_rainbowFirstPixelHue += 5; // Faster animation
            break;
    }
}

void LEDSubsystem::SetAllLEDs(frc::Color color) {
    for (int i = 0; i < kLEDLength; i++) {
        m_ledBuffer[i].SetRGB(color.red * 255, color.green * 255, color.blue * 255);
    }
}

void LEDSubsystem::ApplyRainbow() {
    for (int i = 0; i < kLEDLength; i++) {
        // Calculate hue for this LED
        int hue = (m_rainbowFirstPixelHue + (i * 180 / kLEDLength)) % 180;
        
        // Convert HSV to RGB (simplified)
        double h = hue / 180.0 * 360.0;
        double s = 1.0;
        double v = 1.0;
        
        double c = v * s;
        double x = c * (1 - std::abs(std::fmod(h / 60.0, 2) - 1));
        double m = v - c;
        
        double r, g, b;
        if (h < 60) { r = c; g = x; b = 0; }
        else if (h < 120) { r = x; g = c; b = 0; }
        else if (h < 180) { r = 0; g = c; b = x; }
        else if (h < 240) { r = 0; g = x; b = c; }
        else if (h < 300) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }
        
        m_ledBuffer[i].SetRGB((r + m) * 255, (g + m) * 255, (b + m) * 255);
    }
    
    // Update hue for next frame
    m_rainbowFirstPixelHue = (m_rainbowFirstPixelHue + 3) % 180;
}

void LEDSubsystem::ApplyBreathing(frc::Color baseColor) {
    // Create breathing effect using sine wave
    double breathingFactor = (std::sin(m_animationCounter * 0.1) + 1) / 2; // 0.0 to 1.0
    breathingFactor = 0.3 + breathingFactor * 0.7; // Keep minimum brightness at 30%
    
    frc::Color breathingColor{
        baseColor.red * breathingFactor,
        baseColor.green * breathingFactor,
        baseColor.blue * breathingFactor
    };
    
    SetAllLEDs(breathingColor);
}

void LEDSubsystem::ApplyBrightness() {
    for (int i = 0; i < kLEDLength; i++) {
        m_ledBuffer[i].SetRGB(
            m_ledBuffer[i].r * m_brightness,
            m_ledBuffer[i].g * m_brightness,
            m_ledBuffer[i].b * m_brightness
        );
    }
}

frc::Color LEDSubsystem::GetAllianceColor() {
    return m_isRedAlliance ? kRedAlliance : kBlueAlliance;
}

std::string LEDSubsystem::PatternToString(LEDPattern pattern) {
    switch (pattern) {
        case LEDPattern::OFF: return "OFF";
        case LEDPattern::SOLID_ALLIANCE: return "SOLID_ALLIANCE";
        case LEDPattern::RAINBOW: return "RAINBOW";
        case LEDPattern::BREATHING_ALLIANCE: return "BREATHING_ALLIANCE";
        case LEDPattern::GAME_PIECE_DETECTED: return "GAME_PIECE_DETECTED";
        case LEDPattern::AUTONOMOUS_MODE: return "AUTONOMOUS_MODE";
        case LEDPattern::CLIMBING_MODE: return "CLIMBING_MODE";
        case LEDPattern::ERROR_STATE: return "ERROR_STATE";
        case LEDPattern::CELEBRATION: return "CELEBRATION";
        default: return "UNKNOWN";
    }
}