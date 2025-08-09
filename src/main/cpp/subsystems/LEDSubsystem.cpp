#include "subsystems/LEDSubsystem.h"

#include <frc/smartdashboard/SmartDashboard.h>

using namespace nfr;

LEDSubsystem::LEDSubsystem(int canID, const std::string& canBus, int numLEDs)
    : m_candle(canID, canBus), m_numLEDs(numLEDs)
{
    // config the CANdle
    ctre::phoenix6::configs::CANdleConfiguration config;

    // set the LED strip type
    config.LEDStripType = ctre::phoenix6::hardware::LEDStripType::GRB;

    // set the brightness to 100%
    config.BrightnessScalar = 1.0;

    // apply the configuration
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
    animation.animationType = ctre::phoenix6::controls::AnimationType::SetAll;
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
    animation.animationType =
        ctre::phoenix6::controls::AnimationType::RainbowAnimation;
    animation.brightness = static_cast<uint8_t>(brightness * 255);
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
    animation.animationType =
        ctre::phoenix6::controls::AnimationType::ColorFadeAnimation;

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
    animation.animationType =
        ctre::phoenix6::controls::AnimationType::StrobeAnimation;
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
    animation.animationType =
        ctre::phoenix6::controls::AnimationType::SingleFadeAnimation;
    animation.color1 = {r, g, b};

    animation.speed = speed;
    animation.startIndex = startIndex;

    animation.numLEDs = count;

    m_stateAnimations[state] = animation;
}

void LEDSubsystem::SetStateCustomAnimation(
    LEDState state, std::function<void(ctre::phoenix6::CANdle&)> animationFunc)
{
    LEDAnimation animation;
    animation.customAnimationFunction = animationFunc;

    m_stateAnimations[state] = animation;
}

void LEDSubsystem::SetBrightness(double brightness)
{
    m_brightness = std::max(0.0, std::min(brightness, 1.0));
    m_candle.GetConfigurator().Apply(
        ctre::phoenix6::configs::CANdleConfiguration().WithBrightnessScalar(
            m_brightness));
}

void LEDSubsystem::TurnOff()
{
    // clear LEDs
    m_candle.SetLEDs(0, 0, 0, 0, 0, m_numLEDs);
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

    // off state - all LEDs off
    SetStateColor(LEDState::OFF, 0, 0, 0);
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
            case ctre::phoenix6::controls::AnimationType::None:
                // do nothing
                break;

            case ctre::phoenix6::controls::AnimationType::SetAll:
                m_candle.SetLEDs(animation.color1[0], animation.color1[1],
                                 animation.color1[2], 0, animation.startIndex,
                                 animation.numLEDs);
                break;

            case ctre::phoenix6::controls::AnimationType::RainbowAnimation:
                m_candle.Animate(ctre::phoenix6::controls::RainbowAnimation(
                    animation.brightness, animation.speed, animation.numLEDs,
                    animation.startIndex));
                break;

            case ctre::phoenix6::controls::AnimationType::ColorFadeAnimation:
                m_candle.Animate(
                    ctre::phoenix6::controls::TwoColorFadeAnimation(
                        animation.color1[0], animation.color1[1],
                        animation.color1[2], animation.color2[0],
                        animation.color2[1], animation.color2[2],
                        animation.speed, animation.numLEDs,
                        animation.startIndex));
                break;

            case ctre::phoenix6::controls::AnimationType::SingleFadeAnimation:
                m_candle.Animate(ctre::phoenix6::controls::SingleFadeAnimation(
                    animation.color1[0], animation.color1[1],
                    animation.color1[2], animation.speed, animation.numLEDs,
                    animation.startIndex));
                break;
                // super clean code blocks
            case ctre::phoenix6::controls::AnimationType::StrobeAnimation:
                m_candle.Animate(ctre::phoenix6::controls::StrobeAnimation(
                    animation.color1[0], animation.color1[1],
                    animation.color1[2], animation.speed, animation.numLEDs,
                    animation.startIndex));
                break;

            case ctre::phoenix6::controls::AnimationType::TwinkleAnimation:
            case ctre::phoenix6::controls::AnimationType::TwinkleOffAnimation:
            case ctre::phoenix6::controls::AnimationType::FireAnimation:
            case ctre::phoenix6::controls::AnimationType::LarsonAnimation:
                // these animations require additional parameters that i don't
                // got set up right now for simplicity it will just default to
                // SetAll animation for now
                m_candle.SetLEDs(animation.color1[0], animation.color1[1],
                                 animation.color1[2], 0, animation.startIndex,
                                 animation.numLEDs);
                break;
        }
    }
    else
    {
        // if no animation is configured for this state justturn off the LEDs
        // and dont use them
        TurnOff();
    }
}
