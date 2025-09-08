#pragma once

#include <wpi/DataLog.h>

#include <memory>
#include <unordered_map>
#include <variant>

#include "wpi/struct/Struct.h"

namespace nfr
{
    class WPILogManager
    {
    public:
        WPILogManager();
        void Log(const std::string_view& key, double value);
        void Log(const std::string_view& key, long value);
        void Log(const std::string_view& key, bool value);
        void Log(const std::string_view& key, const std::string_view& value);
        void Log(const std::string_view& key, std::span<double> values);
        void Log(const std::string_view& key, std::span<long> values);
        void Log(const std::string_view& key, std::span<bool> values);
        void Log(const std::string_view& key,
                 std::span<std::string_view> values);
        template <typename T, typename... I>
            requires wpi::StructSerializable<T, I...>
        void Log(const std::string_view& key, const T& value)
        {
            if (!structEntries.contains(std::string(key)))
            {
                structEntries[std::string(key)] =
                    std::make_shared<wpi::log::StructLogEntry<T, I...>>(logRef,
                                                                        key);
            }
            auto entry = structEntries[std::string(key)].get();
            auto structEntry = (wpi::log::StructLogEntry<T, I...>*)entry;
            structEntry->Append(value);
        }
        template <typename T, typename... I>
            requires wpi::StructSerializable<T, I...>
        void Log(const std::string_view& key, std::span<T> values)
        {
            if (!structEntries.contains(std::string(key)))
            {
                structEntries[std::string(key)] =
                    std::make_shared<wpi::log::StructArrayLogEntry<T, I...>>(
                        logRef, key);
            }
            auto entry = structEntries[std::string(key)].get();
            auto structArrayEntry =
                (wpi::log::StructArrayLogEntry<T, I...>*)entry;
            structArrayEntry->Append(values);
        }

    private:
        wpi::log::DataLog& logRef;
        std::unordered_map<
            std::string,
            std::variant<
                wpi::log::DoubleLogEntry, wpi::log::BooleanLogEntry,
                wpi::log::IntegerLogEntry, wpi::log::StringLogEntry,
                wpi::log::DoubleArrayLogEntry, wpi::log::BooleanArrayLogEntry,
                wpi::log::IntegerArrayLogEntry, wpi::log::StringArrayLogEntry>>
            entries;
        std::unordered_map<std::string, std::shared_ptr<wpi::log::DataLogEntry>>
            structEntries;
    };
}  // namespace nfr