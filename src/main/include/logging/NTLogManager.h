#pragma once

#include <logging/LogManager.h>
#include <networktables/BooleanArrayTopic.h>
#include <networktables/BooleanTopic.h>
#include <networktables/DoubleArrayTopic.h>
#include <networktables/DoubleTopic.h>
#include <networktables/IntegerArrayTopic.h>
#include <networktables/IntegerTopic.h>
#include <networktables/StringArrayTopic.h>
#include <networktables/StringTopic.h>

#include <memory>
#include <unordered_map>
#include <variant>

namespace nt
{
    class NetworkTable;
}

namespace nfr
{
    /**
     * @brief A logging manager that writes logs to a file.
     */
    class NTLogManager : public ILogOutput
    {
      public:
        NTLogManager(const std::string &tableName = "logs");
        /**
         * @brief Logs a double value to a file.
         * @param key The key/name for the log entry.
         * @param value The double value to log.
         */
        void Log(const std::string &key, double value) override;

        /**
         * @brief Logs a long integer value to a file.
         * @param key The key/name for the log entry.
         * @param value The long integer value to log.
         */
        void Log(const std::string &key, long value) override;

        /**
         * @brief Logs a boolean value to a file.
         * @param key The key/name for the log entry.
         * @param value The boolean value to log.
         */
        void Log(const std::string &key, bool value) override;

        /**
         * @brief Logs a string value to a file.
         * @param key The key/name for the log entry.
         * @param value The string value to log.
         */
        void Log(const std::string &key, const std::string &value) override;

        /**
         * @brief Logs a span of double values to a file.
         * @param key The key/name for the log entry.
         * @param values The span of double values to log.
         */
        void Log(const std::string &key, std::span<double> values) override;

        /**
         * @brief Logs a span of long integer values to a file.
         * @param key The key/name for the log entry.
         * @param values The span of long integer values to log.
         */
        void Log(const std::string &key, std::span<long> values) override;

        /**
         * @brief Logs a span of boolean values to a file.
         * @param key The key/name for the log entry.
         * @param values The span of boolean values to log.
         */
        void Log(const std::string &key, std::span<bool> values) override;

        /**
         * @brief Logs a span of string values to a file.
         * @param key The key/name for the log entry.
         * @param values The span of string values to log.
         */
        void Log(const std::string &key,
                 std::span<std::string> values) override;

      private:
        std::shared_ptr<nt::NetworkTable> table;
        std::unordered_map<
            std::string,
            std::variant<nt::DoublePublisher, nt::IntegerPublisher,
                         nt::BooleanPublisher, nt::StringPublisher,
                         nt::DoubleArrayPublisher, nt::IntegerArrayPublisher,
                         nt::BooleanArrayPublisher, nt::StringArrayPublisher>>
            topics;
    };
}  // namespace nfr