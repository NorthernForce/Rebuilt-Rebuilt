#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/AddressableLED.h>
#include <frc/util/Color.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <array>

/**
 * LEDSubsystem - Controls addressable LED strip for robot status indication
 * Provides visual feedback for game states, alliance colors, and robot status
 */
class LEDSubsystem : public frc2::SubsystemBase {
public:
    enum class LEDPattern {
        OFF,
        SOLID_ALLIANCE,
        RAINBOW,
        BREATHING_ALLIANCE,
        GAME_PIECE_DETECTED,
        AUTONOMOUS_MODE,
        CLIMBING_MODE,
        ERROR_STATE,
        CELEBRATION
    };

    LEDSubsystem();

    /**
     * Called periodically by the CommandScheduler
     */
    void Periodic() override;

    /**
     * Set LED pattern
     */
    void SetPattern(LEDPattern pattern);

    /**
     * Set alliance color (affects alliance-based patterns)
     */
    void SetAllianceColor(bool isRed);

    /**
     * Set custom solid color
     */
    void SetSolidColor(frc::Color color);

    /**
     * Set custom RGB color
     */
    void SetSolidColor(int r, int g, int b);

    /**
     * Turn off all LEDs
     */
    void TurnOff();

    /**
     * Set brightness (0.0 to 1.0)
     */
    void SetBrightness(double brightness);

    /**
     * Get current pattern
     */
    LEDPattern GetCurrentPattern() const { return m_currentPattern; }

private:
    // LED hardware
    frc::AddressableLED m_led;
    static constexpr int kLEDLength = 60; // Number of LEDs on strip
    std::array<frc::AddressableLED::LEDData, kLEDLength> m_ledBuffer;
    
    // State
    LEDPattern m_currentPattern{LEDPattern::OFF};
    bool m_isRedAlliance{false};
    double m_brightness{1.0};
    frc::Color m_customColor{frc::Color::kBlack};
    
    // Animation state
    int m_animationCounter{0};
    int m_rainbowFirstPixelHue{0};
    
    // Team colors
    static constexpr frc::Color kRedAlliance{255, 0, 0};
    static constexpr frc::Color kBlueAlliance{0, 0, 255};
    static constexpr frc::Color kTeam172Green{0, 255, 65};
    static constexpr frc::Color kGamePieceOrange{255, 165, 0};
    static constexpr frc::Color kAutonomousGold{255, 215, 0};
    
    // NetworkTables
    std::shared_ptr<nt::NetworkTable> m_ledTable;
    
    void UpdateLEDs();
    void ApplyPattern();
    void SetAllLEDs(frc::Color color);
    void ApplyRainbow();
    void ApplyBreathing(frc::Color baseColor);
    void ApplyBrightness();
    frc::Color GetAllianceColor();
    std::string PatternToString(LEDPattern pattern);
};