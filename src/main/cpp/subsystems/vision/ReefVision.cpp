#include <optional>
#include <vector>
#include <string>
#include <cmath>
#include <networktables/NetworkTableInstance.h>
#include <networktables/DoubleArrayTopic.h>
#include <networktables/DoubleTopic.h>
#include <networktables/StringTopic.h>
#include <frc2/command/SubsystemBase.h>
#include "subsystems/vision/ReefVision.h"

Vision::Vision(const std::string& deviceName, const std::string& tableName)
    : xOffset(NAN), connected(false), deviceName(deviceName) {
    auto table = nt::NetworkTableInstance::GetDefault().GetTable(tableName);
    postsSubscriber = table->GetDoubleArrayTopic("Posts").Subscribe({});
    xOffsetSubscriber = table->GetDoubleTopic("CandidateMetersX").Subscribe(NAN);
    cameraPathPublisher = table->GetStringTopic("CameraPathRio").Publish();
    rpub = table->GetStringTopic("CameraPathRadio").Publish();
}

void Vision::Periodic() {
    xOffset = xOffsetSubscriber.Get();
    posts = postsSubscriber.Get();
    connected = false;

    for (const auto& connection : nt::NetworkTableInstance::GetDefault().GetConnections()) {
        if (connection.remote_id.find(deviceName) == 0) {
            connected = true;
            break;
        }
    }

    rpub.Set("10.1.72.36:1181");
    cameraPathPublisher.Set("172.22.11.2:1186");
}

std::optional<double> Vision::GetXOffset() const {
    return std::isnan(xOffset) ? std::nullopt : std::optional<double>(xOffset);
}

double Vision::GetRawXOffset() const {
    return xOffset;
}

std::vector<double> Vision::GetRawPosts() const {
    return posts;
}

bool Vision::IsConnected() const {
    return connected;
}

bool Vision::IsAligned() const {
    return std::abs(xOffset) < 0.05;
}
