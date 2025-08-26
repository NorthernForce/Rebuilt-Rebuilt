#pragma once

#include <networktables/BooleanArrayTopic.h>
#include <networktables/BooleanTopic.h>
#include <networktables/DoubleArrayTopic.h>
#include <networktables/DoubleTopic.h>
#include <networktables/IntegerArrayTopic.h>
#include <networktables/IntegerTopic.h>
#include <networktables/StringArrayTopic.h>
#include <networktables/StringTopic.h>
#include <networktables/StructArrayTopic.h>
#include <networktables/StructTopic.h>

#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

#include "networktables/Topic.h"
#include "wpi/struct/Struct.h"

namespace nt
{
    class NetworkTable;
}

namespace nfr
{
    /**
     * @brief A logging manager that writes logs to a file.
     */
    class NTLogManager
    {
    public:
        NTLogManager(const std::string_view &tableName = "logs");
        /**
         * @brief Logs a double value to a file.
         * @param key The key/name for the log entry.
         * @param value The double value to log.
         */
        void Log(const std::string_view &key, double value);

        /**
         * @brief Logs a long integer value to a file.
         * @param key The key/name for the log entry.
         * @param value The long integer value to log.
         */
        void Log(const std::string_view &key, long value);

        /**
         * @brief Logs a boolean value to a file.
         * @param key The key/name for the log entry.
         * @param value The boolean value to log.
         */
        void Log(const std::string_view &key, bool value);

        /**
         * @brief Logs a string value to a file.
         * @param key The key/name for the log entry.
         * @param value The string value to log.
         */
        void Log(const std::string_view &key, const std::string_view &value);

        /**
         * @brief Logs a span of double values to a file.
         * @param key The key/name for the log entry.
         * @param values The span of double values to log.
         */
        void Log(const std::string_view &key, std::span<double> values);

        /**
         * @brief Logs a span of long integer values to a file.
         * @param key The key/name for the log entry.
         * @param values The span of long integer values to log.
         */
        void Log(const std::string_view &key, std::span<long> values);

        /**
         * @brief Logs a span of boolean values to a file.
         * @param key The key/name for the log entry.
         * @param values The span of boolean values to log.
         */
        void Log(const std::string_view &key, std::span<bool> values);

        /**
         * @brief Logs a span of string values to a file.
         * @param key The key/name for the log entry.
         * @param values The span of string values to log.
         */
        void Log(const std::string_view &key,
                 std::span<std::string_view> values);

        template <typename T, typename... I>
            requires wpi::StructSerializable<T, I...>
        void Log(const std::string_view &key, const T &value)
        {
            if (!structEntries.contains(std::string(key)))
            {
                structEntries[std::string(key)] =
                    std::make_shared<nt::StructPublisher<T, I...>>(
                        table->GetStructTopic<T, I...>(key).Publish());
            }
            auto topic = structEntries[std::string(key)].get();
            auto structTopic = (nt::StructPublisher<T, I...> *)topic;
            structTopic->Set(value);
        }

        template <typename T, typename... I>
            requires wpi::StructSerializable<T, I...>
        void Log(const std::string_view &key, std::span<T> values)
        {
            if (!structEntries.contains(std::string(key)))
            {
                structEntries[std::string(key)] =
                    std::make_shared<nt::StructArrayPublisher<T, I...>>(
                        table->GetStructArrayTopic<T, I...>(key).Publish());
            }
            auto topic = structEntries[std::string(key)].get();
            auto structArrayTopic = (nt::StructArrayPublisher<T, I...> *)topic;
            structArrayTopic->Set(values);
        }

    private:
        std::shared_ptr<nt::NetworkTable> table;
        std::unordered_map<
            std::string,
            std::variant<nt::DoublePublisher, nt::IntegerPublisher,
                         nt::BooleanPublisher, nt::StringPublisher,
                         nt::DoubleArrayPublisher, nt::IntegerArrayPublisher,
                         nt::BooleanArrayPublisher, nt::StringArrayPublisher>>
            topics;
        std::unordered_map<std::string, std::shared_ptr<nt::Publisher>>
            structEntries;
    };
}  // namespace nfr