#pragma once

#include <units/angle.h>
#include <frc/Preferences.h>
#include <array>
#include "frc/MathUtil.h"
#include "frc/geometry/Rotation2d.h"

/**
* @brief Loads swerve module steering offsets from robot preferences
*
* Swerve modules need to know their absolute angular position to steer
* correctly. These offsets compensate for mechanical differences and are
* determined through calibration. They're stored in robot preferences so they
* persist between reboots.
*
* @return Array of 4 rotation offsets [FrontLeft, FrontRight, BackLeft,
* BackRight]
*/
inline std::array<frc::Rotation2d, 4> getModuleOffsets()
{
    return {
        (units::degree_t)frc::Preferences::GetDouble("FrontLeftOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("FrontRightOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("BackLeftOffset", 0.0),
        (units::degree_t)frc::Preferences::GetDouble("BackRightOffset", 0.0)};
}

/**
* @brief Saves swerve module steering offsets to robot preferences
*
* After calibrating the swerve modules, we save the offsets so they persist
* between robot reboots. This avoids having to recalibrate every time.
*
* @param offsets Array of 4 rotation offsets to save
*/
inline void SetModuleOffsets(const std::array<frc::Rotation2d, 4>& offsets)
{
    frc::Preferences::SetDouble("FrontLeftOffset",
                                offsets[0].Degrees().value());
    frc::Preferences::SetDouble("FrontRightOffset",
                                offsets[1].Degrees().value());
    frc::Preferences::SetDouble("BackLeftOffset", offsets[2].Degrees().value());
    frc::Preferences::SetDouble("BackRightOffset",
                                offsets[3].Degrees().value());
}

/**
 * @brief Processes joystick input to improve driving feel
 *
 * Raw joystick values can be noisy and sensitive. This function:
 * 1. Applies deadband - ignores small inputs near zero (reduces drift)
 * 2. Squares the input - gives finer control at low speeds, more aggressive at
 * high speeds
 *
 * The squaring technique is common in FRC - it makes precise movements easier
 * while still allowing full speed when needed.
 *
 * @param input Function that returns current joystick value (-1.0 to 1.0)
 * @return Processed function that returns cleaned-up joystick value
 */
inline std::function<double()> ProcessInput(std::function<double()> input)
{
    return [input = std::move(input)]() mutable
    {
        auto x = input();
        x = frc::ApplyDeadband(x, 0.10);  // Ignore inputs smaller than 10%
        x *= std::abs(x);  // Square the input for finer control at low speeds
        return x;
    };
}