#pragma once

#include <optional>
#include <vector>
#include <string>
#include <cmath>
#include <networktables/NetworkTableInstance.h>
#include <networktables/DoubleArrayTopic.h>
#include <networktables/DoubleTopic.h>
#include <networktables/StringTopic.h>
#include <frc2/command/SubsystemBase.h>

class Vision : public frc2::SubsystemBase {
private:
    nt::DoubleSubscriber xOffsetSubscriber;
    nt::DoubleArraySubscriber postsSubscriber;
    nt::StringPublisher cameraPathPublisher;
    nt::StringPublisher rpub;
    double xOffset;
    std::vector<double> posts;
    bool connected;
    std::string deviceName;

public:
    Vision(const std::string& deviceName, const std::string& tableName);

    void Periodic() override;

    std::optional<double> GetXOffset() const;

    double GetRawXOffset() const;

    std::vector<double> GetRawPosts() const;

    bool IsConnected() const;

    bool IsAligned() const;
};