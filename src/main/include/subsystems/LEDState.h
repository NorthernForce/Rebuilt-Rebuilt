#pragma once

#include <ctre/phoenix6/CANdle.hpp>
#include <memory>
#include <string>

namespace nfr
{
    /**
     * @brief Base class for LED states
     *
     * This class defines the interface for all LED states. Each state must
     * implement the Animate method to apply its animation to the CANdle, and
     * the GetStateName method to return a human-readable name for the state.
     */
    class LEDState
    {
    public:
        virtual ~LEDState() = default;

        /**
         * @brief Apply this state's animation to the CANdle
         *
         * @param candle The CANdle to animate
         */
        virtual void Animate(ctre::phoenix6::hardware::CANdle& candle) = 0;

        /**
         * @brief Get the name of this state
         *
         * @return std::string The name of the state
         */
        virtual std::string GetStateName() const = 0;
    };

    // Smart pointer type for LED states
    using LEDStatePtr = std::shared_ptr<LEDState>;
}  // namespace nfr
