#pragma once

#include <units/angular_velocity.h>
#include <units/angular_acceleration.h>
#include <units/angular_jerk.h>
#include <units/length.h>
#include <units/mass.h>

struct ElevatorConstants
    {
        double kS;
        double kV;
        double kA;
        double kP;
        double kI;
        double kD;
        double kG;
        units::angular_velocity::turns_per_second_t kCruiseVelocity;
        units::turns_per_second_squared_t kAcceleration;
        units::turns_per_second_cubed_t kJerk;
        units::meter_t kSprocketCircumference;
        double kGearRatio;
        bool kInverted;
        units::meter_t kLowerLimit;
        units::meter_t kUpperLimit;
        units::kilogram_t kMass;
    };