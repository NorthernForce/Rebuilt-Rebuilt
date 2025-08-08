#pragma once
#include <frc/DigitalInput.h>
#include <frc/RobotController.h>

#include <memory>

class ElevatorSensorIO
{
public:
    ElevatorSensorIO() = default;
    virtual ~ElevatorSensorIO() = default;
    virtual bool IsAtLimit() const = 0;
};

class ElevatorSensorIOLimitSwitch : public ElevatorSensorIO
{
public:
    ElevatorSensorIOLimitSwitch(int channel);
    bool IsAtLimit() const override;

private:
    std::shared_ptr<frc::DigitalInput> m_digitalInput;
};