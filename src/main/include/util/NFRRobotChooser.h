#pragma once

#include <util/NFRRobotContainer.h>

using RobotSupplier = std::function<std::unique_ptr<NFRRobotContainer>()>;

class NFRRobotChooser
{
public:
    NFRRobotChooser(RobotSupplier defaultRobot,
                    std::map<std::string, RobotSupplier> otherRobots);
    std::unique_ptr<NFRRobotContainer> GetNFRRobotContainer();

protected:
    RobotSupplier m_defaultRobot;
    std::map<std::string, RobotSupplier> m_otherRobots;
};