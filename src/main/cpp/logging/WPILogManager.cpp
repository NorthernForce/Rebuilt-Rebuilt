#include <frc/DataLogManager.h>
#include <frc/DriverStation.h>
#include <logging/WPILogManager.h>

#include <string_view>
#include <unordered_map>

using namespace nfr;
using namespace wpi::log;
using namespace std;
using namespace frc;

WPILogManager::WPILogManager() : logRef(frc::DataLogManager::GetLog())
{
    DriverStation::StartDataLog(logRef);
}

void WPILogManager::Log(const string_view& key, double value)
{
    if (!entries.contains(string(key)))
    {
        entries[string(key)] = DoubleLogEntry(logRef, key);
    }
    std::visit(
        [&](auto& entry)
        {
            using T = std::decay_t<decltype(entry)>;
            if constexpr (std::is_same_v<T, DoubleLogEntry>)
            {
                entry.Append(value);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected double, got different type.");
            }
        },
        entries[string(key)]);
}

void WPILogManager::Log(const string_view& key, long value)
{
    if (!entries.contains(string(key)))
    {
        entries[string(key)] = IntegerLogEntry(logRef, key);
    }
    std::visit(
        [&](auto& entry)
        {
            using T = std::decay_t<decltype(entry)>;
            if constexpr (std::is_same_v<T, IntegerLogEntry>)
            {
                entry.Append(value);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected integer, got different type.");
            }
        },
        entries[string(key)]);
}

void WPILogManager::Log(const string_view& key, bool value)
{
    if (!entries.contains(string(key)))
    {
        entries[string(key)] = BooleanLogEntry(logRef, key);
    }
    std::visit(
        [&](auto& entry)
        {
            using T = std::decay_t<decltype(entry)>;
            if constexpr (std::is_same_v<T, BooleanLogEntry>)
            {
                entry.Append(value);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected boolean, got different type.");
            }
        },
        entries[string(key)]);
}

void WPILogManager::Log(const string_view& key, const string_view& value)
{
    if (!entries.contains(string(key)))
    {
        entries[string(key)] = StringLogEntry(logRef, key);
    }
    std::visit(
        [&](auto& entry)
        {
            using T = std::decay_t<decltype(entry)>;
            if constexpr (std::is_same_v<T, StringLogEntry>)
            {
                entry.Append(value);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected string, got different type.");
            }
        },
        entries[string(key)]);
}

void WPILogManager::Log(const string_view& key, std::span<double> values)
{
    if (!entries.contains(string(key)))
    {
        entries[string(key)] = DoubleArrayLogEntry(logRef, key);
    }
    std::visit(
        [&](auto& entry)
        {
            using T = std::decay_t<decltype(entry)>;
            if constexpr (std::is_same_v<T, DoubleArrayLogEntry>)
            {
                entry.Append(values);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected double array, got different type.");
            }
        },
        entries[string(key)]);
}

void WPILogManager::Log(const string_view& key, std::span<long> values)
{
    if (!entries.contains(string(key)))
    {
        entries[string(key)] = IntegerArrayLogEntry(logRef, key);
    }
    std::visit(
        [&](auto& entry)
        {
            using T = std::decay_t<decltype(entry)>;
            if constexpr (std::is_same_v<T, IntegerArrayLogEntry>)
            {
                std::vector<int64_t> valuesVec(values.begin(), values.end());
                entry.Append(valuesVec);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected integer array, got different type.");
            }
        },
        entries[string(key)]);
}

void WPILogManager::Log(const string_view& key, std::span<bool> values)
{
    if (!entries.contains(string(key)))
    {
        entries[string(key)] = BooleanArrayLogEntry(logRef, key);
    }
    std::visit(
        [&](auto& entry)
        {
            using T = std::decay_t<decltype(entry)>;
            if constexpr (std::is_same_v<T, BooleanArrayLogEntry>)
            {
                entry.Append(values);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected boolean array, got different type.");
            }
        },
        entries[string(key)]);
}

void WPILogManager::Log(const string_view& key,
                        std::span<std::string_view> values)
{
    if (!entries.contains(string(key)))
    {
        entries[string(key)] = StringArrayLogEntry(logRef, key);
    }
    std::visit(
        [&](auto& entry)
        {
            using T = std::decay_t<decltype(entry)>;
            if constexpr (std::is_same_v<T, StringArrayLogEntry>)
            {
                entry.Append(values);
            }
            else
            {
                throw runtime_error(
                    "Log entry type mismatch for key: " + string(key) +
                    ". Expected string array, got different type.");
            }
        },
        entries[string(key)]);
}