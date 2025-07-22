#pragma once
#include <frc/DigitalInput.h>
#include <frc/RobotController.h>
#include <memory>

using namespace std;
using namespace units;
using namespace frc;

class ElevatorSensorIO {
public:
  ElevatorSensorIO() = default;
  virtual ~ElevatorSensorIO();
  virtual bool IsAtLimit() = 0;
};

class ElevatorSensorIOLimitSwitch : public ElevatorSensorIO {
public:
  ElevatorSensorIOLimitSwitch(int channel);
  bool IsAtLimit() override;

private:
  shared_ptr<DigitalInput> m_digitalInput;
};