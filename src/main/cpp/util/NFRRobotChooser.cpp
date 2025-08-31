#include "util/NFRRobotChooser.h"

#include <iostream>
#include <memory>

#include "frc/RobotController.h"
#include "util/NFRRobotContainer.h"

using namespace std;

NFRRobotChooser::NFRRobotChooser(RobotSupplier defaultRobot,
                                 map<string, RobotSupplier> otherRobots)
{
    m_defaultRobot = defaultRobot;
    m_otherRobots = otherRobots;
}

bool EqualsIgnoreCase(string first, string second)
{
    if (first.size() != second.size())
    {
        return false;
    }
    return equal(first.begin(), first.end(), second.begin(),
                 [](char a, char b) { return tolower(a) == tolower(b); });
}

std::unique_ptr<NFRRobotContainer> NFRRobotChooser::GetNFRRobotContainer()
{
    string roborioSerial = frc::RobotController::GetSerialNumber();
    for (auto const &entry : m_otherRobots)
    {
        if (EqualsIgnoreCase(entry.first, roborioSerial))
        {
            cout << "Creating robot container for robot with serial "
                 << roborioSerial << endl;
            return entry.second();
        }
    }
    cout << "Creating default robot container" << endl;
    return m_defaultRobot();
}