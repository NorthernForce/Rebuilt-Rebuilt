#pragma once

#include <ctre/phoenix6/CANdle.hpp>
#include <memory>

#include "subsystems/LEDState.h"

namespace nfr
{

    /**
     * @brief LED states specific to the Ralph robot
     */
    namespace ralph
    {

        /**
         * @brief Ralph-specific LED state for alignment
         */
        class AlignmentState : public LEDState
        {
        public:
            /**
             * @brief Construct a new Alignment State object
             *
             * @param speed Animation speed (0.0-1.0)
             */
            explicit AlignmentState(double speed = 1.0);

            /**
             * @brief Get the name of this state
             *
             * @return std::string State name
             */
            std::string GetStateName() const override;

            /**
             * @brief Animate the LEDs for this state
             *
             * @param candle CANdle device to animate
             */
            void Animate(ctre::phoenix6::hardware::CANdle& candle) override;

        private:
            double m_speed;
        };

        /**
         * @brief Ralph-specific LED state for autonomous mode
         */
        class AutonomousState : public LEDState
        {
        public:
            /**
             * @brief Construct a new Autonomous State object
             *
             * @param speed Animation speed (0.0-1.0)
             */
            explicit AutonomousState(double speed = 1.0);

            /**
             * @brief Get the name of this state
             *
             * @return std::string State name
             */
            std::string GetStateName() const override;

            /**
             * @brief Animate the LEDs for this state
             *
             * @param candle CANdle device to animate
             */
            void Animate(ctre::phoenix6::hardware::CANdle& candle) override;

        private:
            double m_speed;
        };

        /**
         * @brief Ralph-specific LED state for teleop mode
         */
        class TeleopState : public LEDState
        {
        public:
            /**
             * @brief Construct a new Teleop State object
             *
             * @param speed Animation speed (0.0-1.0)
             */
            explicit TeleopState(double speed = 1.0);

            /**
             * @brief Get the name of this state
             *
             * @return std::string State name
             */
            std::string GetStateName() const override;

            /**
             * @brief Animate the LEDs for this state
             *
             * @param candle CANdle device to animate
             */
            void Animate(ctre::phoenix6::hardware::CANdle& candle) override;

        private:
            double m_speed;
        };

        /**
         * @brief Factory for creating Ralph-specific LED states
         */
        class RalphLEDStateFactory
        {
        public:
            /**
             * @brief Create an alignment state for Ralph
             *
             * @param speed Animation speed (0.0-1.0)
             * @return LEDStatePtr Shared pointer to the created state
             */
            static LEDStatePtr CreateAlignmentState(double speed = 1.0);

            /**
             * @brief Create an autonomous state for Ralph
             *
             * @param speed Animation speed (0.0-1.0)
             * @return LEDStatePtr Shared pointer to the created state
             */
            static LEDStatePtr CreateAutonomousState(double speed = 1.0);

            /**
             * @brief Create a teleop state for Ralph
             *
             * @param speed Animation speed (0.0-1.0)
             * @return LEDStatePtr Shared pointer to the created state
             */
            static LEDStatePtr CreateTeleopState(double speed = 1.0);
        };

    }  // namespace ralph

}  // namespace nfr
