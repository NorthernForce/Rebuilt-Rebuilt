#include <logging/NTLogManager.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/StructArrayTopic.h>
#include <networktables/StructTopic.h>

#include <string_view>
#include <unordered_map>

using namespace nfr;
using namespace std;
using namespace nt;

NTLogManager::NTLogManager(const string_view& tableName, const string_view& metadataTableName)
    : table(NetworkTableInstance::GetDefault().GetTable(tableName)),
        metadataTable(
            NetworkTableInstance::GetDefault().GetTable(metadataTableName))
{
    if (!table)
    {
        throw runtime_error("Failed to create NetworkTable for logging.");
    }
}

void NTLogManager::Log(const string_view& key, double value)
{
    if (key.starts_with("Metadata/"))
    {
        metadataTable->GetDoubleTopic(key.substr(9)).Publish().Set(value);
        return;
    }
    if (!topics.contains(string(key)))
    {
        topics[string(key)] = table->GetDoubleTopic(key).Publish();
    }
    auto& topic = topics[string(key)];
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
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected double, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string_view& key, long value)
{
    if (key.starts_with("Metadata/"))
    {
        metadataTable->GetEntry(key.substr(9)).SetInteger(value);
        return;
    }
    if (!topics.contains(string(key)))
    {
        topics[string(key)] = table->GetIntegerTopic(key).Publish();
    }
    auto& topic = topics[string(key)];
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
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected integer, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string_view& key, bool value)
{
    if (key.starts_with("Metadata/"))
    {
        metadataTable->GetEntry(key.substr(9)).SetBoolean(value);
        return;
    }
    if (!topics.contains(string(key)))
    {
        topics[string(key)] = table->GetBooleanTopic(key).Publish();
    }
    auto& topic = topics[string(key)];
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
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected boolean, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string_view& key, const string_view& value)
{
    if (key.starts_with("Metadata/"))
    {
        metadataTable->GetEntry(key.substr(9)).SetString(value);
        return;
    }
    if (!topics.contains(string(key)))
    {
        topics[string(key)] = table->GetStringTopic(key).Publish();
    }
    auto& topic = topics[string(key)];
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
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected string, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string_view& key, std::span<double> values)
{
    if (!topics.contains(string(key)))
    {
        topics[string(key)] = table->GetDoubleArrayTopic(key).Publish();
    }
    auto& topic = topics[string(key)];
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
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected double array, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string_view& key, std::span<long> values)
{
    if (!topics.contains(string(key)))
    {
        topics[string(key)] = table->GetIntegerArrayTopic(key).Publish();
    }
    auto& topic = topics[string(key)];
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
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected integer array, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string_view& key, std::span<bool> values)
{
    if (!topics.contains(string(key)))
    {
        topics[string(key)] = table->GetBooleanArrayTopic(key).Publish();
    }
    auto& topic = topics[string(key)];
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
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected boolean array, got different type.");
            }
        },
        topic);
}

void NTLogManager::Log(const string_view& key,
                       std::span<std::string_view> values)
{
    if (!topics.contains(string(key)))
    {
        topics[string(key)] = table->GetStringArrayTopic(key).Publish();
    }
    auto& topic = topics[string(key)];
    std::visit(
        [&](auto& pub)
        {
            using T = std::decay_t<decltype(pub)>;
            if constexpr (std::is_same_v<T, StringArrayPublisher>)
            {
                // Convert std::span<std::string_view> to
                // std::vector<std::string>
                std::vector<std::string> stringValues;
                stringValues.reserve(values.size());
                for (const auto& val : values)
                {
                    stringValues.emplace_back(val);
                }
                pub.Set(stringValues);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected string array, got different type.");
            }
        },
        topic);
}