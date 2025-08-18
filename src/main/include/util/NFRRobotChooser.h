#pragma once

#include <util/NFRRobotContainer.h>

class NFRRobotChooser {
    public:
        NFRRobotChooser(std::shared_ptr<NFRRobotContainer> defaultRobot, std::map<std::string, std::shared_ptr<NFRRobotContainer>> otherRobots, std::string robotNamePath);
        NFRRobotChooser(std::shared_ptr<NFRRobotContainer> defaultRobot, std::map<std::string, std::shared_ptr<NFRRobotContainer>> otherRobots);
        std::shared_ptr<NFRRobotContainer> GetNFRRobotContainer();
    protected:
        std::shared_ptr<NFRRobotContainer> m_defaultRobot;
        std::map<std::string, std::shared_ptr<NFRRobotContainer>> m_otherRobots;
        std::string m_robotNamePath;
};