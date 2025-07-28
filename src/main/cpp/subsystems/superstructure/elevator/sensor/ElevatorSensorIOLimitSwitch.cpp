#include <subsystems/superstructure/elevator/ElevatorSensor.h>

using namespace std;
using namespace units;
using namespace frc;

ElevatorSensorIOLimitSwitch::ElevatorSensorIOLimitSwitch(int channel)
{
    m_digitalInput = std::make_shared<frc::DigitalInput>(channel);
}

bool ElevatorSensorIOLimitSwitch::IsAtLimit()
{
    return m_digitalInput->Get();
}