#include "subsystems/LEDStates.h"

using namespace nfr;

void OffState::Animate(ctre::phoenix6::hardware::CANdle& candle)
{
    // Turn off all LEDs by setting them to black
    ctre::phoenix6::controls::SolidColor solidColor(0, 8); // Start at LED 0, cover 8 LEDs
    // Black color is default (all 0)
    candle.SetControl(solidColor);
}

SolidColorState::SolidColorState(uint8_t r, uint8_t g, uint8_t b, double brightness, std::string name)
    : m_r(r), m_g(g), m_b(b), m_brightness(brightness), m_name(std::move(name))
{
}

void SolidColorState::Animate(ctre::phoenix6::hardware::CANdle& candle)
{
    ctre::phoenix6::controls::SolidColor solidColor(0, 8); // Start at LED 0, cover 8 LEDs
    solidColor.Color.Red = m_r;
    solidColor.Color.Green = m_g;
    solidColor.Color.Blue = m_b;
    // Brightness is set at the CANdle level, not per animation
    candle.SetControl(solidColor);
}

ErrorState::ErrorState(double speed)
    : m_speed(speed)
{
}

void ErrorState::Animate(ctre::phoenix6::hardware::CANdle& candle)
{
    // Error state uses a red strobe animation
    ctre::phoenix6::controls::StrobeAnimation strobeAnimation(0, 8); // Start at LED 0, cover 8 LEDs
    strobeAnimation.Color.Red = 255;
    strobeAnimation.Color.Green = 0;
    strobeAnimation.Color.Blue = 0;
    strobeAnimation.FrameRate = units::frequency::hertz_t(m_speed);
    candle.SetControl(strobeAnimation);
}

WarningState::WarningState(double speed)
    : m_speed(speed)
{
}

void WarningState::Animate(ctre::phoenix6::hardware::CANdle& candle)
{
    // Warning state uses a yellow strobe animation
    ctre::phoenix6::controls::StrobeAnimation strobeAnimation(0, 8); // Start at LED 0, cover 8 LEDs
    strobeAnimation.Color.Red = 255;
    strobeAnimation.Color.Green = 255;
    strobeAnimation.Color.Blue = 0;
    strobeAnimation.FrameRate = units::frequency::hertz_t(m_speed);
    candle.SetControl(strobeAnimation);
}

SuccessState::SuccessState(double speed)
    : m_speed(speed)
{
}

void SuccessState::Animate(ctre::phoenix6::hardware::CANdle& candle)
{
    // Success state uses a rainbow animation
    ctre::phoenix6::controls::RainbowAnimation rainbowAnimation(0, 8); // Start at LED 0, cover 8 LEDs
    rainbowAnimation.FrameRate = units::frequency::hertz_t(m_speed);
    // Brightness is set at the CANdle level, not per animation
    candle.SetControl(rainbowAnimation);
}

AlignmentState::AlignmentState(double speed)
    : m_speed(speed)
{
}

void AlignmentState::Animate(ctre::phoenix6::hardware::CANdle& candle)
{
    // Alignment state uses a larson (chase) animation
    ctre::phoenix6::controls::LarsonAnimation larsonAnimation(0, 8); // Start at LED 0, cover 8 LEDs
    larsonAnimation.Color.Red = 0;
    larsonAnimation.Color.Green = 0;
    larsonAnimation.Color.Blue = 255;
    larsonAnimation.FrameRate = units::frequency::hertz_t(m_speed);
    larsonAnimation.Size = 7;
    candle.SetControl(larsonAnimation);
}

// Factory methods
LEDStatePtr LEDStateFactory::CreateOffState()
{
    return std::make_shared<OffState>();
}

LEDStatePtr LEDStateFactory::CreateDefaultState()
{
    return std::make_shared<DefaultState>();
}

LEDStatePtr LEDStateFactory::CreateEnabledState()
{
    return std::make_shared<EnabledState>();
}

LEDStatePtr LEDStateFactory::CreateDisabledState()
{
    return std::make_shared<DisabledState>();
}

LEDStatePtr LEDStateFactory::CreateAutonomousState()
{
    return std::make_shared<AutonomousState>();
}

LEDStatePtr LEDStateFactory::CreateTeleopState()
{
    return std::make_shared<TeleopState>();
}

LEDStatePtr LEDStateFactory::CreateErrorState(double speed)
{
    return std::make_shared<ErrorState>(speed);
}

LEDStatePtr LEDStateFactory::CreateWarningState(double speed)
{
    return std::make_shared<WarningState>(speed);
}

LEDStatePtr LEDStateFactory::CreateSuccessState(double speed)
{
    return std::make_shared<SuccessState>(speed);
}

LEDStatePtr LEDStateFactory::CreateAlignmentState(double speed)
{
    return std::make_shared<AlignmentState>(speed);
}

LEDStatePtr LEDStateFactory::CreateSolidColorState(uint8_t r, uint8_t g, uint8_t b, double brightness, const std::string& name)
{
    return std::make_shared<SolidColorState>(r, g, b, brightness, name);
}
