#include "subsystems/LEDSubsystem.h"

#include <frc/smartdashboard/SmartDashboard.h>

using namespace nfr;

LEDSubsystem::LEDSubsystem(int canID, const std::string& canBus, int numLEDs)
    : m_candle(canID, canBus), m_numLEDs(numLEDs)
{
    // config the CANdle
    ctre::phoenix6::configs::CANdleConfiguration config;

    // Apply the configuration
    // Note: The LED strip type and brightness are now configured differently
    // in Phoenix 6 API, but we'll keep the default settings for now
    m_candle.GetConfigurator().Apply(config);

    // initialize default animations for each state
    InitializeDefaultAnimations();

    // set initial state to OFF
    SetState(LEDState::OFF);

    m_initialized = true;
}

void LEDSubsystem::Periodic()
{
    // apply the animation for the current state
    if (m_initialized)
    {
        ApplyCurrentAnimation();
    }
}

void LEDSubsystem::SetState(LEDState state)
{
    m_currentState = state;

    // log state
    switch (state)
    {
        case LEDState::OFF:
            frc::SmartDashboard::PutString("LED State", "OFF");
            break;
        case LEDState::DEFAULT:
            frc::SmartDashboard::PutString("LED State", "DEFAULT");
            break;
        case LEDState::ENABLED:
            frc::SmartDashboard::PutString("LED State", "ENABLED");
            break;
        case LEDState::DISABLED:
            frc::SmartDashboard::PutString("LED State", "DISABLED");
            break;
        case LEDState::AUTONOMOUS:
            frc::SmartDashboard::PutString("LED State", "AUTONOMOUS");
            break;
        case LEDState::TELEOP:
            frc::SmartDashboard::PutString("LED State", "TELEOP");
            break;
        case LEDState::ALIGNMENT:
            frc::SmartDashboard::PutString("LED State", "ALIGNMENT");
            break;
        case LEDState::ERROR:
            frc::SmartDashboard::PutString("LED State", "ERROR");
            break;
        case LEDState::WARNING:
            frc::SmartDashboard::PutString("LED State", "WARNING");
            break;
        case LEDState::SUCCESS:
            frc::SmartDashboard::PutString("LED State", "SUCCESS");
            break;
        case LEDState::CUSTOM1:
            frc::SmartDashboard::PutString("LED State", "CUSTOM1");
            break;
        case LEDState::CUSTOM2:
            frc::SmartDashboard::PutString("LED State", "CUSTOM2");
            break;
        case LEDState::CUSTOM3:
            frc::SmartDashboard::PutString("LED State", "CUSTOM3");
            break;
    }
}

LEDState LEDSubsystem::GetState() const
{
    return m_currentState;
}

void LEDSubsystem::ConfigureStateAnimation(LEDState state,
                                           const LEDAnimation& animation)
{
    m_stateAnimations[state] = animation;
}

void LEDSubsystem::SetStateColor(LEDState state, uint8_t r, uint8_t g,
                                 uint8_t b, uint8_t startIndex, uint8_t count)
{
    LEDAnimation animation;
    animation.animationType = LEDAnimation::AnimationType::SetAll;
    animation.color1 = {r, g, b};
    animation.startIndex = startIndex;
    animation.numLEDs = count;

    m_stateAnimations[state] = animation;
}

void LEDSubsystem::SetStateRainbow(LEDState state, double brightness,
                                   uint32_t speed, uint8_t startIndex,
                                   uint8_t count)
{
    LEDAnimation animation;
    animation.animationType = LEDAnimation::AnimationType::RainbowAnimation;
    animation.brightness = brightness;
    animation.speed = speed;
    animation.startIndex = startIndex;
    animation.numLEDs = count;

    m_stateAnimations[state] = animation;
}

void LEDSubsystem::SetStateColorFade(LEDState state, uint8_t r1, uint8_t g1,
                                     uint8_t b1, uint8_t r2, uint8_t g2,
                                     uint8_t b2, uint32_t speed,
                                     uint8_t startIndex, uint8_t count)
{
    LEDAnimation animation;
    animation.animationType = LEDAnimation::AnimationType::ColorFadeAnimation;

    animation.color1 = {r1, g1, b1};
    animation.color2 = {r2, g2, b2};

    animation.speed = speed;
    animation.startIndex = startIndex;
    animation.numLEDs = count;

    m_stateAnimations[state] = animation;
}

void LEDSubsystem::SetStateStrobe(LEDState state, uint8_t r, uint8_t g,
                                  uint8_t b, uint32_t speed, uint8_t startIndex,
                                  uint8_t count)
{
    LEDAnimation animation;
    animation.animationType = LEDAnimation::AnimationType::StrobeAnimation;
    animation.color1 = {r, g, b};
    animation.speed = speed;
    animation.startIndex = startIndex;
    animation.numLEDs = count;

    m_stateAnimations[state] = animation;
}

void LEDSubsystem::SetStateChase(LEDState state, uint8_t r, uint8_t g,
                                 uint8_t b, uint32_t speed, uint8_t startIndex,
                                 uint8_t count)
{
    LEDAnimation animation;
    animation.animationType = LEDAnimation::AnimationType::SingleFadeAnimation;
    animation.color1 = {r, g, b};
    animation.speed = speed;
    animation.startIndex = startIndex;
    animation.numLEDs = count;

    m_stateAnimations[state] = animation;
}

void LEDSubsystem::SetStateCustomAnimation(
    LEDState state, std::function<void(ctre::phoenix6::hardware::CANdle&)> animationFunc)
{
    LEDAnimation animation;
    animation.customAnimationFunction = animationFunc;

    m_stateAnimations[state] = animation;
}

void LEDSubsystem::SetBrightness(double brightness)
{
    m_brightness = brightness;

    // Apply brightness directly to animations instead of using configuration
    // The brightness will be applied in ApplyCurrentAnimation
}

void LEDSubsystem::TurnOff()
{
    // Use SolidColor control to turn off all LEDs
    ctre::phoenix6::controls::SolidColor solidColor(0, m_numLEDs - 1);
    solidColor.Color.Red = 0;
    solidColor.Color.Green = 0;
    solidColor.Color.Blue = 0;
    solidColor.Color.White = 0;
    m_candle.SetControl(solidColor);
}

void LEDSubsystem::InitializeDefaultAnimations()
{
    // default state -blue
    SetStateColor(LEDState::DEFAULT, 0, 0, 255);

    // enabled state -green
    SetStateColor(LEDState::ENABLED, 0, 255, 0);

    // disabled state - red
    SetStateColor(LEDState::DISABLED, 255, 0, 0);

    // autonomous mode - yellow
    SetStateColor(LEDState::AUTONOMOUS, 255, 255, 0);

    // Teleop mode - blue
    SetStateColor(LEDState::TELEOP, 0, 0, 255);

    // alignment mode - strobe white
    SetStateStrobe(LEDState::ALIGNMENT, 255, 255, 255, 1);

    // error state - fast strobe red
    SetStateStrobe(LEDState::ERROR, 255, 0, 0, 3);

    // warning state - slow strobe orange
    SetStateStrobe(LEDState::WARNING, 255, 165, 0, 1);

    // success state - rainbow just cuz
    SetStateRainbow(LEDState::SUCCESS, 1.0, 2);

    // we can change these later for stuff if we gotta go something game
    // specific
    SetStateColor(LEDState::CUSTOM1, 255, 0, 255);    // purple default
    SetStateColor(LEDState::CUSTOM2, 255, 255, 255);  // white default
    SetStateColor(LEDState::CUSTOM3, 0, 255, 255);    // cyan default
}

void LEDSubsystem::ApplyCurrentAnimation()
{
    // check if has a configured animation for the current state
    auto animationIt = m_stateAnimations.find(m_currentState);

    if (animationIt != m_stateAnimations.end())
    {
        const LEDAnimation& animation = animationIt->second;

        // if has a custom animation function, should prob call it
        if (animation.customAnimationFunction)
        {
            animation.customAnimationFunction(m_candle);
            return;
        }

        // otherwise, use the builtin animation types
        switch (animation.animationType)
        {
            case LEDAnimation::AnimationType::None:
                // do nothing
                break;

            case LEDAnimation::AnimationType::SetAll:
                {
                    ctre::phoenix6::controls::SolidColor solidColor(animation.startIndex, animation.startIndex + animation.numLEDs - 1);
                    solidColor.Color.Red = animation.color1[0];
                    solidColor.Color.Green = animation.color1[1];
                    solidColor.Color.Blue = animation.color1[2];
                    solidColor.Color.White = 0;
                    m_candle.SetControl(solidColor);
                }
                break;

            case LEDAnimation::AnimationType::RainbowAnimation:
                {
                    ctre::phoenix6::controls::RainbowAnimation rainbowConfig(animation.startIndex, animation.startIndex + animation.numLEDs - 1);
                    // Set animation properties
                    rainbowConfig.FrameRate = units::frequency::hertz_t(animation.speed);
                    // Apply the animation using SetControl
                    m_candle.SetControl(rainbowConfig);
                }
                break;

            case LEDAnimation::AnimationType::ColorFadeAnimation:
                {
                    // Use ColorFlow animation instead since TwoColorFadeAnimation doesn't exist
                    ctre::phoenix6::controls::ColorFlowAnimation fadeConfig(animation.startIndex, animation.startIndex + animation.numLEDs - 1);
                    fadeConfig.Color.Red = animation.color1[0];
                    fadeConfig.Color.Green = animation.color1[1];
                    fadeConfig.Color.Blue = animation.color1[2];
                    fadeConfig.FrameRate = units::frequency::hertz_t(animation.speed);
                    // Apply the animation using SetControl
                    m_candle.SetControl(fadeConfig);
                }
                break;

            case LEDAnimation::AnimationType::SingleFadeAnimation:
                {
                    ctre::phoenix6::controls::SingleFadeAnimation singleFadeConfig(animation.startIndex, animation.startIndex + animation.numLEDs - 1);
                    singleFadeConfig.Color.Red = animation.color1[0];
                    singleFadeConfig.Color.Green = animation.color1[1];
                    singleFadeConfig.Color.Blue = animation.color1[2];
                    singleFadeConfig.FrameRate = units::frequency::hertz_t(animation.speed);
                    // Apply the animation using SetControl
                    m_candle.SetControl(singleFadeConfig);
                }
                break;
                
            case LEDAnimation::AnimationType::StrobeAnimation:
                {
                    ctre::phoenix6::controls::StrobeAnimation strobeConfig(animation.startIndex, animation.startIndex + animation.numLEDs - 1);
                    strobeConfig.Color.Red = animation.color1[0];
                    strobeConfig.Color.Green = animation.color1[1];
                    strobeConfig.Color.Blue = animation.color1[2];
                    strobeConfig.FrameRate = units::frequency::hertz_t(animation.speed);
                    // Apply the animation using SetControl
                    m_candle.SetControl(strobeConfig);
                }
                break;

            case LEDAnimation::AnimationType::TwinkleAnimation:
            case LEDAnimation::AnimationType::TwinkleOffAnimation:
            case LEDAnimation::AnimationType::FireAnimation:
            case LEDAnimation::AnimationType::LarsonAnimation:
                // these animations require additional parameters that aren't
                // set up right now, so for simplicity it will just default to
                // SetAll animation for now
                {
                    ctre::phoenix6::controls::SolidColor solidColor(animation.startIndex, animation.startIndex + animation.numLEDs - 1);
                    solidColor.Color.Red = animation.color1[0];
                    solidColor.Color.Green = animation.color1[1];
                    solidColor.Color.Blue = animation.color1[2];
                    solidColor.Color.White = 0;
                    m_candle.SetControl(solidColor);
                }
                break;
        }
    }
    else
    {
        // if no animation is configured for this state just turn off the LEDs
        TurnOff();
    }
}
