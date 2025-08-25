#include "subsystems/RalphLEDStates.h"

#include <units/frequency.h>

#include <ctre/phoenix6/controls/LarsonAnimation.hpp>
#include <ctre/phoenix6/controls/RainbowAnimation.hpp>
#include <ctre/phoenix6/controls/StrobeAnimation.hpp>

namespace nfr
{
    namespace ralph
    {

        // AlignmentState implementation
        AlignmentState::AlignmentState(double speed) : m_speed(speed)
        {
        }

        std::string AlignmentState::GetStateName() const
        {
            return "ALIGNMENT";
        }

        void AlignmentState::Animate(ctre::phoenix6::hardware::CANdle& candle)
        {
            // Ralph's alignment state uses a blue larson (chase) animation
            ctre::phoenix6::controls::LarsonAnimation larsonAnimation(
                0, 8);  // Start at LED 0, cover 8 LEDs
            larsonAnimation.Color.Red = 0;
            larsonAnimation.Color.Green = 0;
            larsonAnimation.Color.Blue = 255;
            larsonAnimation.FrameRate = units::frequency::hertz_t(m_speed);
            larsonAnimation.Size = 7;
            candle.SetControl(larsonAnimation);
        }

        // AutonomousState implementation
        AutonomousState::AutonomousState(double speed) : m_speed(speed)
        {
        }

        std::string AutonomousState::GetStateName() const
        {
            return "AUTONOMOUS";
        }

        void AutonomousState::Animate(ctre::phoenix6::hardware::CANdle& candle)
        {
            // Ralph's autonomous state uses a green strobe animation
            ctre::phoenix6::controls::StrobeAnimation strobeAnimation(
                0, 8);  // Start at LED 0, cover 8 LEDs
            strobeAnimation.Color.Red = 0;
            strobeAnimation.Color.Green = 255;
            strobeAnimation.Color.Blue = 0;
            strobeAnimation.FrameRate = units::frequency::hertz_t(m_speed);
            candle.SetControl(strobeAnimation);
        }

        // TeleopState implementation
        TeleopState::TeleopState(double speed) : m_speed(speed)
        {
        }

        std::string TeleopState::GetStateName() const
        {
            return "TELEOP";
        }

        void TeleopState::Animate(ctre::phoenix6::hardware::CANdle& candle)
        {
            // Ralph's teleop state uses a rainbow animation
            ctre::phoenix6::controls::RainbowAnimation rainbowAnimation(
                0, 8);  // Start at LED 0, cover 8 LEDs
            rainbowAnimation.FrameRate = units::frequency::hertz_t(m_speed);
            candle.SetControl(rainbowAnimation);
        }

        // Factory methods
        LEDStatePtr RalphLEDStateFactory::CreateAlignmentState(double speed)
        {
            return std::make_shared<AlignmentState>(speed);
        }

        LEDStatePtr RalphLEDStateFactory::CreateAutonomousState(double speed)
        {
            return std::make_shared<AutonomousState>(speed);
        }

        LEDStatePtr RalphLEDStateFactory::CreateTeleopState(double speed)
        {
            return std::make_shared<TeleopState>(speed);
        }

    }  // namespace ralph
}  // namespace nfr
