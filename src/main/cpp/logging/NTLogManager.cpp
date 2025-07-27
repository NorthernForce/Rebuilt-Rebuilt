#include <logging/NTLogManager.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

using namespace nfr;
using namespace std;
using namespace nt;

NTLogManager::NTLogManager(const string& tableName)
    : table(NetworkTableInstance::GetDefault().GetTable(tableName))
{
    if (!table)
    {
        throw runtime_error("Failed to create NetworkTable for logging.");
    }
}

void NTLogManager::Log(const string& key, double value)
{
    if (!topics.contains(key))
    {
        topics[key] = table->GetDoubleTopic(key).Publish();
    }
    auto& topic = topics[key];
    std::visit(
        [&](auto& pub)
        {
            using T = std::decay_t<decltype(pub)>;
            if constexpr (std::is_same_v<T, DoublePublisher>)
            {
                pub.Set(value);
            }
            else
            {
                throw runtime_error("Log entry type mismatch for key: " + key +
                                    ". Expected double, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string& key, long value)
{
    if (!topics.contains(key))
    {
        topics[key] = table->GetIntegerTopic(key).Publish();
    }
    auto& topic = topics[key];
    std::visit(
        [&](auto& pub)
        {
            using T = std::decay_t<decltype(pub)>;
            if constexpr (std::is_same_v<T, IntegerPublisher>)
            {
                pub.Set(value);
            }
            else
            {
                throw runtime_error("Log entry type mismatch for key: " + key +
                                    ". Expected integer, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string& key, bool value)
{
    if (!topics.contains(key))
    {
        topics[key] = table->GetBooleanTopic(key).Publish();
    }
    auto& topic = topics[key];
    std::visit(
        [&](auto& pub)
        {
            using T = std::decay_t<decltype(pub)>;
            if constexpr (std::is_same_v<T, BooleanPublisher>)
            {
                pub.Set(value);
            }
            else
            {
                throw runtime_error("Log entry type mismatch for key: " + key +
                                    ". Expected boolean, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string& key, const string& value)
{
    if (!topics.contains(key))
    {
        topics[key] = table->GetStringTopic(key).Publish();
    }
    auto& topic = topics[key];
    std::visit(
        [&](auto& pub)
        {
            using T = std::decay_t<decltype(pub)>;
            if constexpr (std::is_same_v<T, StringPublisher>)
            {
                pub.Set(value);
            }
            else
            {
                throw runtime_error("Log entry type mismatch for key: " + key +
                                    ". Expected string, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string& key, std::span<double> values)
{
    if (!topics.contains(key))
    {
        topics[key] = table->GetDoubleArrayTopic(key).Publish();
    }
    auto& topic = topics[key];
    std::visit(
        [&](auto& pub)
        {
            using T = std::decay_t<decltype(pub)>;
            if constexpr (std::is_same_v<T, DoubleArrayPublisher>)
            {
                pub.Set(values);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + key +
                    ". Expected double array, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string& key, std::span<long> values)
{
    if (!topics.contains(key))
    {
        topics[key] = table->GetIntegerArrayTopic(key).Publish();
    }
    auto& topic = topics[key];
    std::visit(
        [&](auto& pub)
        {
            using T = std::decay_t<decltype(pub)>;
            if constexpr (std::is_same_v<T, IntegerArrayPublisher>)
            {
                // Convert std::span<long> to std::span<int64_t>
                std::vector<int64_t> intValues(values.begin(), values.end());
                pub.Set(intValues);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + key +
                    ". Expected integer array, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string& key, std::span<bool> values)
{
    if (!topics.contains(key))
    {
        topics[key] = table->GetBooleanArrayTopic(key).Publish();
    }
    auto& topic = topics[key];
    std::visit(
        [&](auto& pub)
        {
            using T = std::decay_t<decltype(pub)>;
            if constexpr (std::is_same_v<T, BooleanArrayPublisher>)
            {
                // Convert std::span<bool> to std::vector<int>
                std::vector<int> boolValues(values.begin(), values.end());
                pub.Set(boolValues);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + key +
                    ". Expected boolean array, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string& key, std::span<std::string> values)
{
    if (!topics.contains(key))
    {
        topics[key] = table->GetStringArrayTopic(key).Publish();
    }
    auto& topic = topics[key];
    std::visit(
        [&](auto& pub)
        {
            using T = std::decay_t<decltype(pub)>;
            if constexpr (std::is_same_v<T, StringArrayPublisher>)
            {
                pub.Set(values);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + key +
                    ". Expected string array, got different type.");
            }
        },
        topic);
}