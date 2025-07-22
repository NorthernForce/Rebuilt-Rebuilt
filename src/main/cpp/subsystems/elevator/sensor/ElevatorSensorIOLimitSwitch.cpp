#include <subsystems/elevator/ElevatorSensor.h>

ElevatorSensorIOLimitSwitch::ElevatorSensorIOLimitSwitch(int channel) {
  m_digitalInput = make_shared<DigitalInput>(channel);
}

bool ElevatorSensorIOLimitSwitch::IsAtLimit() { return m_digitalInput->Get(); }