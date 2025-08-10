#pragma once

#include <frc2/command/SubsystemBase.h>

#include <array>
#include <ctre/phoenix6/CANdle.hpp>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "logging/Logger.h"
#include "subsystems/LEDState.h"
#include "subsystems/LEDStates.h"
#include "subsystems/LEDStateEnum.h"

namespace nfr
{

    /**
     * @brief Represents an LED animation configuration
     */
    struct LEDAnimation
    {
        // Animation type (used to determine which animation to apply)
        enum class AnimationType
        {
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
        double brightness = 1.0;                    // 0.0-1.0 brightness scalar
        double speed = 1.0;                         // animation speed
        std::array<uint8_t, 3> color1 = {0, 0, 0};  // RGB
        std::array<uint8_t, 3> color2 = {0, 0, 0};  // RGB
        int startIndex = 0;
        int numLEDs = 8;

        // for custom animations
        std::function<void(ctre::phoenix6::hardware::CANdle&)>
            customAnimationFunction = nullptr;
    };

    /**
     * @brief A generic state-based LED subsystem using the CTRE CANdle lib
     */
    class LEDSubsystem : public frc2::SubsystemBase
    {
    public:
        /**
         * @brief Construct a new LED subsystem
         *
         * @param canID CAN ID of the CANdle device
         * @param canBus CAN bus name (optional)
         * @param numLEDs Number of LEDs connected to the CANdle
         */
        LEDSubsystem(int canID, const std::string& canBus = "",
                     int numLEDs = 8);

        /**
         * @brief Periodic function called by the command scheduler
         */
        void Periodic() override;

        /**
         * @brief Set the current LED state
         *
         * @param state Shared pointer to an LED state object
         */
        void SetState(LEDStatePtr state);

        /**
         * @brief Set the current LED state using the legacy enum
         * 
         * @param stateEnum Legacy enum state value
         */
        void SetStateEnum(LEDStateEnum stateEnum);

        /**
         * @brief Get the current LED state name
         *
         * @return std::string The name of the current state
         */
        std::string GetStateName() const;

        /**
         * @brief Get the current LED state object
         * 
         * @return LEDStatePtr Shared pointer to the current LED state
         */
        LEDStatePtr GetState() const;

        /**
         * @brief Configure an animation for a specific LED state
         *
         * @param state The LED state to configure
         * @param animation Animation configuration
         */
        void ConfigureStateAnimation(LEDStatePtr state,
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
        void SetStateColor(LEDStatePtr state, uint8_t r, uint8_t g, uint8_t b,
                           uint8_t startIndex = 0, uint8_t count = 8);

        /**
         * @brief Create and register a rainbow animation state
         *
         * @param name State name
         * @param brightness brightness (0-1)
         * @param speed animation speed (0-1)
         * @return LEDStatePtr Shared pointer to the created state
         */
        LEDStatePtr CreateRainbowState(const std::string& name, double brightness = 1.0,
                                       double speed = 1.0);

        /**
         * @brief Create and register a color fade animation state
         *
         * @param name State name
         * @param r1 first color red (0-255)
         * @param g1 first color green (0-255)
         * @param b1 first color blue (0-255)
         * @param r2 second color red (0-255)
         * @param g2 second color green (0-255)
         * @param b2 second color blue (0-255)
         * @param speed animation speed (0-1)
         * @return LEDStatePtr Shared pointer to the created state
         */
        LEDStatePtr CreateFadeState(const std::string& name, uint8_t r1, uint8_t g1, uint8_t b1,
                                    uint8_t r2, uint8_t g2, uint8_t b2, double speed = 1.0);

        /**
         * @brief Create and register a strobe animation state
         *
         * @param name State name
         * @param r red (0-255)
         * @param g green (0-255)
         * @param b blue (0-255)
         * @param speed animation speed (0-1)
         * @return LEDStatePtr Shared pointer to the created state
         */
        LEDStatePtr CreateStrobeState(const std::string& name, uint8_t r, uint8_t g, uint8_t b,
                                      double speed = 1.0);

        /**
         * @brief Create and register a solid color state
         *
         * @param name State name
         * @param r red (0-255)
         * @param g green (0-255)
         * @param b blue (0-255)
         * @param brightness brightness (0-1)
         * @return LEDStatePtr Shared pointer to the created state
         */
        LEDStatePtr CreateColorState(const std::string& name, uint8_t r, uint8_t g, uint8_t b,
                                     double brightness = 1.0);

        /**
         * @brief Create and register a custom animation state
         *
         * @param name State name
         * @param animationFunction function to call to animate the LEDs
         * @return LEDStatePtr Shared pointer to the created state
         */
        LEDStatePtr CreateCustomState(
            const std::string& name,
            std::function<void(ctre::phoenix6::hardware::CANdle&)> animationFunction);

        /**
         * @brief Set the global brightness for all animations
         *
         * @param brightness Brightness value (0.0-1.0, where 1.0 is full brightness)
         */
        void SetBrightness(double brightness);

        /**
         * @brief Turn off all LEDs
         */
        void TurnOff();

        /**
         * @brief Log subsystem data to the logging system
         * 
         * @param log The logging context
         */
        void Log(const nfr::LogContext& log) const;

    private:
        ctre::phoenix6::hardware::CANdle m_candle;
        int m_numLEDs;
        bool m_initialized;
        double m_brightness;

        LEDStatePtr m_currentState;

        // Map of registered states by name
        std::unordered_map<std::string, LEDStatePtr> m_states;

        // Map for backward compatibility with enum-based states
        std::unordered_map<LEDStateEnum, std::string> m_enumToStateName;

        /**
         * @brief Initialize default states
         */
        void InitializeDefaultStates();

        /**
         * @brief Register a state with the subsystem
         * 
         * @param name State name
         * @param state State object
         * @return LEDStatePtr The registered state
         */
        LEDStatePtr RegisterState(const std::string& name, LEDStatePtr state);
    };

    // LEDStateEnum is now defined in LEDStateEnum.h

}  // namespace nfr