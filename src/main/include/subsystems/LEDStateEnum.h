#pragma once

namespace nfr
{
    /**
     * @brief Legacy enum for LED states (kept for backward compatibility)
     */
    enum class LEDStateEnum
    {
        OFF,
        DEFAULT,
        ENABLED,
        DISABLED,
        AUTONOMOUS,
        TELEOP,
        ALIGNMENT,
        ERROR,
        WARNING,
        SUCCESS,
        CUSTOM1,
        CUSTOM2,
        CUSTOM3
    };
}  // namespace nfr
