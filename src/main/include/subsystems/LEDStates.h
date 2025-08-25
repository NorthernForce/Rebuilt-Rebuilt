#pragma once

#include <ctre/phoenix6/controls/ColorFlowAnimation.hpp>
#include <ctre/phoenix6/controls/LarsonAnimation.hpp>
#include <ctre/phoenix6/controls/RainbowAnimation.hpp>
#include <ctre/phoenix6/controls/SingleFadeAnimation.hpp>
#include <ctre/phoenix6/controls/SolidColor.hpp>
#include <ctre/phoenix6/controls/StrobeAnimation.hpp>

#include "subsystems/LEDState.h"

namespace nfr
{
    /**
     * @brief LED state that turns off all LEDs
     */
    class OffState : public LEDState
    {
    public:
        void Animate(ctre::phoenix6::hardware::CANdle& candle) override;
        std::string GetStateName() const override
        {
            return "OFF";
        }
    };

    /**
     * @brief Base class for LED states that display a solid color
     */
    class SolidColorState : public LEDState
    {
    public:
        /**
         * @brief Construct a new Solid Color State
         *
         * @param r Red component (0-255)
         * @param g Green component (0-255)
         * @param b Blue component (0-255)
         * @param brightness Brightness (0.0-1.0)
         * @param name State name
         */
        SolidColorState(uint8_t r, uint8_t g, uint8_t b,
                        double brightness = 1.0,
                        std::string name = "SOLID_COLOR");

        void Animate(ctre::phoenix6::hardware::CANdle& candle) override;
        std::string GetStateName() const override
        {
            return m_name;
        }

    protected:
        uint8_t m_r;
        uint8_t m_g;
        uint8_t m_b;
        double m_brightness;
        std::string m_name;
    };

    /**
     * @brief Default state (typically blue)
     */
    class DefaultState : public SolidColorState
    {
    public:
        DefaultState() : SolidColorState(0, 0, 255, 1.0, "DEFAULT")
        {
        }
    };

    /**
     * @brief Enabled state (typically green)
     */
    class EnabledState : public SolidColorState
    {
    public:
        EnabledState() : SolidColorState(0, 255, 0, 1.0, "ENABLED")
        {
        }
    };

    /**
     * @brief Disabled state (typically red)
     */
    class DisabledState : public SolidColorState
    {
    public:
        DisabledState() : SolidColorState(255, 0, 0, 1.0, "DISABLED")
        {
        }
    };

    /**
     * @brief Autonomous state (typically yellow)
     */
    class AutonomousState : public SolidColorState
    {
    public:
        AutonomousState() : SolidColorState(255, 255, 0, 1.0, "AUTONOMOUS")
        {
        }
    };

    /**
     * @brief Teleop state (typically purple)
     */
    class TeleopState : public SolidColorState
    {
    public:
        TeleopState() : SolidColorState(128, 0, 128, 1.0, "TELEOP")
        {
        }
    };

    /**
     * @brief Error state (typically flashing red)
     */
    class ErrorState : public LEDState
    {
    public:
        ErrorState(double speed = 0.5);
        void Animate(ctre::phoenix6::hardware::CANdle& candle) override;
        std::string GetStateName() const override
        {
            return "ERROR";
        }

    private:
        double m_speed;
    };

    /**
     * @brief Warning state (typically flashing yellow)
     */
    class WarningState : public LEDState
    {
    public:
        WarningState(double speed = 0.5);
        void Animate(ctre::phoenix6::hardware::CANdle& candle) override;
        std::string GetStateName() const override
        {
            return "WARNING";
        }

    private:
        double m_speed;
    };

    /**
     * @brief Success state (typically rainbow)
     */
    class SuccessState : public LEDState
    {
    public:
        SuccessState(double speed = 1.0);
        void Animate(ctre::phoenix6::hardware::CANdle& candle) override;
        std::string GetStateName() const override
        {
            return "SUCCESS";
        }

    private:
        double m_speed;
    };

    /**
     * @brief Alignment state (typically chase animation)
     */
    class AlignmentState : public LEDState
    {
    public:
        AlignmentState(double speed = 0.5);
        void Animate(ctre::phoenix6::hardware::CANdle& candle) override;
        std::string GetStateName() const override
        {
            return "ALIGNMENT";
        }

    private:
        double m_speed;
    };

    /**
     * @brief Factory for creating common LED states
     */
    class LEDStateFactory
    {
    public:
        static LEDStatePtr CreateOffState();
        static LEDStatePtr CreateDefaultState();
        static LEDStatePtr CreateEnabledState();
        static LEDStatePtr CreateDisabledState();
        static LEDStatePtr CreateAutonomousState();
        static LEDStatePtr CreateTeleopState();
        static LEDStatePtr CreateErrorState(double speed = 0.5);
        static LEDStatePtr CreateWarningState(double speed = 0.5);
        static LEDStatePtr CreateSuccessState(double speed = 1.0);
        static LEDStatePtr CreateAlignmentState(double speed = 0.5);
        static LEDStatePtr CreateSolidColorState(
            uint8_t r, uint8_t g, uint8_t b, double brightness = 1.0,
            const std::string& name = "CUSTOM");
    };
}  // namespace nfr
