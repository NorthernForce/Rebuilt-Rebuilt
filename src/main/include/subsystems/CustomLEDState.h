#pragma once

#include <functional>

#include "subsystems/LEDState.h"

namespace nfr
{
    /**
     * @brief Custom LED state that uses a function to animate the LEDs
     */
    class CustomLEDState : public LEDState
    {
    public:
        /**
         * @brief Construct a new Custom LED State
         *
         * @param name State name
         * @param animationFunction Function to call to animate the LEDs
         */
        CustomLEDState(std::string name,
                       std::function<void(ctre::phoenix6::hardware::CANdle&)>
                           animationFunction);

        void Animate(ctre::phoenix6::hardware::CANdle& candle) override;
        std::string GetStateName() const override
        {
            return m_name;
        }

    private:
        std::string m_name;
        std::function<void(ctre::phoenix6::hardware::CANdle&)>
            m_animationFunction;
    };
}  // namespace nfr
