#pragma once

#include <frc2/command/SubsystemBase.h>

#include <array>
#include <ctre/phoenix6/CANdle.hpp>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace nfr
{

    /**
     * @brief Represents different LED states the robot can be in
     */
    enum class LEDState
    {
        OFF,         // LEDs off (no really)
        DEFAULT,     // default state (typically idle)
        ENABLED,     // robot enabled
        DISABLED,    // robot disabled
        AUTONOMOUS,  // in autonomous mode
        TELEOP,      // in teleop mode
        ALIGNMENT,   // alignment mode (if we do vision stuff ig this would be
                     // cool)
        ERROR,       // error state
        WARNING,     // warning state
        SUCCESS,     // success state (like loaded a piece or scored? idk)
        CUSTOM1,     // just
        CUSTOM2,     // in
        CUSTOM3      // case
    };

    /**
     * @brief Represents an LED animation configuration
     */
    struct LEDAnimation {
        // Animation type (used to determine which animation to apply)
        enum class AnimationType {
            None,
            SetAll,
            RainbowAnimation,
            ColorFadeAnimation,
            SingleFadeAnimation,
            StrobeAnimation,
            TwinkleAnimation,
            TwinkleOffAnimation,
            FireAnimation,
            LarsonAnimation
        };
        
        // animation parameters
        AnimationType animationType = AnimationType::None;
        int animationSlot = 0;
        double brightness = 1.0;  // 0.0-1.0 brightness scalar
        double speed = 1.0;       // animation speed
        std::array<uint8_t, 3> color1 = {0, 0, 0}; // RGB
        std::array<uint8_t, 3> color2 = {0, 0, 0}; // RGB
        int startIndex = 0;
        int numLEDs = 8;

        // for custom animations
        std::function<void(ctre::phoenix6::hardware::CANdle&)> customAnimationFunction = nullptr;
    };

    /**
     * @brief A generic state-based LED subsystem using the CTRE CANdle lib
     */
    class LEDSubsystem : public frc2::SubsystemBase {
    public:
        /**
         * @brief Construct a new LED subsystem
         * 
         * @param canID CAN ID of the CANdle device
         * @param canBus CAN bus name (optional ig)
         * @param numLEDs Number of LEDs connected to the CANdle
         */
        LEDSubsystem(int canID, const std::string& canBus = "",
                     int numLEDs = 8);

        /**
         * @brief Periodic update function -> (called by the command scheduler)
         */
        void Periodic() override;

        /**
         * @brief Set the current LED state
         *
         * @param state New LED state
         */
        void SetState(LEDState state);

        /**
         * @brief Get the current LED state
         *
         * @return Current LED state
         */
        LEDState GetState() const;

        /**
         * @brief Configure an animation for a specific LED state
         *
         * @param state The LED state to configure
         * @param animation Animation configuration
         */
        void ConfigureStateAnimation(LEDState state,
                                     const LEDAnimation& animation);

        /**
         * @brief Set solid color for a specific state
         *
         * @param state the LED state
         * @param r red (0-255)
         * @param g green (0-255)
         * @param b blue (0-255)
         * @param startIndex first LED to set
         * @param count number of LEDs to set
         */
        void SetStateColor(LEDState state, uint8_t r, uint8_t g, uint8_t b,
                           uint8_t startIndex = 0, uint8_t count = 8);

        /**
         * @brief Set a rainbow animation for a specific state (could look cool)
         *
         * @param state the LED state
         * @param brightness brightness (0-1)
         * @param speed animation speed
         * @param startIndex first LED to set
         * @param count number of LEDs to set
         */
        void SetStateRainbow(LEDState state, double brightness = 1.0,
                             uint32_t speed = 1, uint8_t startIndex = 0,
                             uint8_t count = 8);

        /**
         * @brief Set a color fade animation for a specific state (for like a
         * loading animation of some sort)
         *
         * @param state the LED state
         * @param r1 first color red (0-255)
         * @param g1 first color green (0-255)
         * @param b1 first color blue (0-255)
         * @param r2 second color red (0-255)
         * @param g2 second color green (0-255)
         * @param b2 second color blue (0-255)
         * @param speed animation speed
         * @param startIndex first LED to set
         * @param count number of LEDs to set
         */
        void SetStateColorFade(LEDState state, uint8_t r1, uint8_t g1,
                               uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2,
                               uint32_t speed = 1, uint8_t startIndex = 0,
                               uint8_t count = 8);

        /**
         * @brief Set a strobe animation for a specific state
         *
         * @param state The LED state
         * @param r Red (0-255)
         * @param g Green (0-255)
         * @param b Blue (0-255)
         * @param speed Animation speed
         * @param startIndex First LED to set
         * @param count Number of LEDs to set
         */
        void SetStateStrobe(LEDState state, uint8_t r, uint8_t g, uint8_t b,
                            uint32_t speed = 1, uint8_t startIndex = 0,
                            uint8_t count = 8);

        /**
         * @brief Set a chase animation for a specific state
         *
         * @param state The LED state
         * @param r Red (0-255)
         * @param g Green (0-255)
         * @param b Blue (0-255)
         * @param speed Animation speed
         * @param startIndex First LED to set
         * @param count Number of LEDs to set
         */
        void SetStateChase(LEDState state, uint8_t r, uint8_t g, uint8_t b,
                           uint32_t speed = 1, uint8_t startIndex = 0,
                           uint8_t count = 8);

        /**
         * @brief Set a custom animation function for a specific state
         *
         * @param state The LED state
         * @param animationFunc Custom animation function that will be called in
         * Periodic
         */
        void SetStateCustomAnimation(
            LEDState state,
            std::function<void(ctre::phoenix6::hardware::CANdle&)> animationFunc);

        /**
         * @brief Set the global brightness for all animations
         *
         * @param brightness Brightness value (0-1) (decimals like %s)
         */
        void SetBrightness(double brightness);

        /**
         * @brief Turn off all LEDs (useful frrr)
         */
        void TurnOff();

    private:
        ctre::phoenix6::hardware::CANdle m_candle;
        int m_numLEDs;
        LEDState m_currentState = LEDState::DEFAULT;
        double m_brightness = 1.0;
        bool m_initialized = false;

        // map of state to animation config
        std::unordered_map<LEDState, LEDAnimation> m_stateAnimations;

        // init default animations for each state
        void InitializeDefaultAnimations();

        // apply the current animation based on state
        void ApplyCurrentAnimation();
    };

}  // namespace nfr