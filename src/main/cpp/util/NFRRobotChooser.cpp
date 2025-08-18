#include "util/NFRRobotChooser.h"

#include <iostream>
#include <fstream>

using namespace std;

NFRRobotChooser::NFRRobotChooser(shared_ptr<NFRRobotContainer> defaultRobot, map<string, shared_ptr<NFRRobotContainer>> otherRobots, string robotNamePath)
{
    m_defaultRobot = defaultRobot;
    m_otherRobots = otherRobots;
    m_robotNamePath = robotNamePath;
}

NFRRobotChooser::NFRRobotChooser(shared_ptr<NFRRobotContainer> defaultRobot, map<string, shared_ptr<NFRRobotContainer>> otherRobots)
: NFRRobotChooser(defaultRobot, otherRobots, "/home/admin/robot_settings.txt")
{
}

bool EqualsIgnoreCase(string first, string second)
{
    transform(first.begin(), first.end(), first.begin(), ::tolower);
    transform(second.begin(), second.end(), second.begin(), ::tolower);
    return first == second;
}

shared_ptr<NFRRobotContainer> NFRRobotChooser::GetNFRRobotContainer()
{
    string text;
    ifstream file(m_robotNamePath);
    if(file.is_open())
    {
        string robotName;
        getline(file, robotName);
        for (auto robot : m_otherRobots)
        {
            if (EqualsIgnoreCase(robot.first, robotName))
            {
                return robot.second;
            }
        }
    } else {
        cout << "Could not find file";
    }
    return m_defaultRobot;
}