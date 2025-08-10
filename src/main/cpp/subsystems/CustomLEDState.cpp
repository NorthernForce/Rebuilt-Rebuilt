#include "subsystems/CustomLEDState.h"

using namespace nfr;

CustomLEDState::CustomLEDState(std::string name, std::function<void(ctre::phoenix6::hardware::CANdle&)> animationFunction)
    : m_name(std::move(name)), m_animationFunction(std::move(animationFunction))
{
}

void CustomLEDState::Animate(ctre::phoenix6::hardware::CANdle& candle)
{
    if (m_animationFunction)
    {
        m_animationFunction(candle);
    }
}
