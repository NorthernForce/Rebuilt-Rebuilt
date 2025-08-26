#pragma once
#include <frc/DigitalInput.h>
#include <frc/RobotController.h>

#include <memory>

/**
 * @brief An abstract class to read data from a sensor such as a limit switch or
 * a beam break This class has no actual functionality
 */
class ElevatorSensorIO
{
public:
    ElevatorSensorIO() = default;
    virtual ~ElevatorSensorIO() = default;
    virtual bool IsAtLimit() const = 0;
};

/**
 * @brief A class to read data from a limit switch sensor
 */
class ElevatorSensorIOLimitSwitch : public ElevatorSensorIO
{
public:
    /**
     * @param channel The DIO port that the sensor is connected to
     */
    ElevatorSensorIOLimitSwitch(int channel);

    /**
     * @return true if the sensor is triggered, meaning the elevator is at the
     * hard limit, and false if not
     */
    bool IsAtLimit() const override;

private:
    std::shared_ptr<frc::DigitalInput> m_digitalInput;
};