#pragma once

#include <frc/DataLogManager.h>
#include <wpi/DataLog.h>
#include <frc/DriverStation.h>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <networktables/NetworkTableInstance.h>
#include <networktables/Topic.h>
#include <networktables/DoubleArrayTopic.h>
#include <networktables/StringArrayTopic.h>
#include <networktables/BooleanArrayTopic.h>
#include <networktables/IntegerArrayTopic.h>
#include <networktables/DoubleTopic.h>
#include <networktables/StringTopic.h>
#include <networktables/BooleanTopic.h>
#include <networktables/IntegerTopic.h>
#include <rfl.hpp>     // Include reflect-cpp library
#include <type_traits> // For std::is_same_v, std::remove_cvref_t

namespace nfr
{
    /**
     * @brief Abstract base class for logging managers.
     * Defines the interface for different logging backends (e.g., WPILog, NetworkTables).
     */
    class BaseLogManager
    {
    public:
        /**
         * @brief Logs a double value.
         * @param key The key/name for the log entry.
         * @param value The double value to log.
         */
        virtual void log(const std::string& key, double value) = 0;

        /**
         * @brief Logs a vector of double values.
         * @param key The key/name for the log entry.
         * @param values The vector of double values to log.
         */
        virtual void log(const std::string& key, std::vector<double> values) = 0;

        /**
         * @brief Logs a string value.
         * @param key The key/name for the log entry.
         * @param value The string value to log.
         */
        virtual void log(const std::string& key, std::string value) = 0;

        /**
         * @brief Logs a vector of string values.
         * @param key The key/name for the log entry.
         * @param values The vector of string values to log.
         */
        virtual void log(const std::string& key, std::vector<std::string> values) = 0;

        /**
         * @brief Logs a boolean value.
         * @param key The key/name for the log entry.
         * @param value The boolean value to log.
         */
        virtual void log(const std::string& key, bool value) = 0;

        /**
         * @brief Logs a vector of boolean values.
         * @param key The key/name for the log entry.
         * @param values The vector of boolean values to log.
         */
        virtual void log(const std::string& key, std::vector<bool> values) = 0;

        /**
         * @brief Logs a 64-bit integer value.
         * @param key The key/name for the log entry.
         * @param value The int64_t value to log.
         */
        virtual void log(const std::string& key, int64_t value) = 0;

        /**
         * @brief Logs a vector of 64-bit integer values.
         * @param key The key/name for the log entry.
         * @param values The vector of int64_t values to log.
         */
        virtual void log(const std::string& key, std::vector<int64_t> values) = 0;
    };

    /**
     * @brief Log manager implementation using WPILog (FRC DataLogManager).
     * This class handles logging data to the WPILog system, which is typically
     * used for on-robot data recording.
     */
    class WPILogManager : public BaseLogManager
    {
    private:
        wpi::log::DataLog &logRef;
        std::map<std::string, wpi::log::DoubleLogEntry> doubleEntries;
        std::map<std::string, wpi::log::DoubleArrayLogEntry> doubleArrayEntries;
        std::map<std::string, wpi::log::StringLogEntry> stringEntries;
        std::map<std::string, wpi::log::StringArrayLogEntry> stringArrayEntries;
        std::map<std::string, wpi::log::BooleanLogEntry> booleanEntries;
        std::map<std::string, wpi::log::BooleanArrayLogEntry> booleanArrayEntries;
        std::map<std::string, wpi::log::IntegerLogEntry> integerEntries;
        std::map<std::string, wpi::log::IntegerArrayLogEntry> integerArrayEntries;

    public:
        /**
         * @brief Constructs a WPILogManager.
         * Initializes with the global WPILog DataLog instance.
         */
        WPILogManager() : logRef(frc::DataLogManager::GetLog()) {}

        /**
         * @brief Logs a double value to WPILog.
         * @param key The key/name for the log entry.
         * @param value The double value to log.
         */
        void log(const std::string& key, double value) override
        {
            auto &entry = doubleEntries[key];
            if (!entry)
            {
                entry = wpi::log::DoubleLogEntry(logRef, key);
            }
            entry.Append(value);
        }

        /**
         * @brief Logs a vector of double values to WPILog.
         * @param key The key/name for the log entry.
         * @param values The vector of double values to log.
         */
        void log(const std::string& key, const std::vector<double>& values) override
        {
            auto &entry = doubleArrayEntries[key];
            if (!entry)
            {
                entry = wpi::log::DoubleArrayLogEntry(logRef, key);
            }
            entry.Append(values);
        }

        /**
         * @brief Logs a string value to WPILog.
         * @param key The key/name for the log entry.
         * @param value The string value to log.
         */
        void log(std::string key, std::string value) override
        {
            auto &entry = stringEntries[key];
            if (!entry)
            {
                entry = wpi::log::StringLogEntry(logRef, key);
            }
            entry.Append(value);
        }

        /**
         * @brief Logs a vector of string values to WPILog.
         * @param key The key/name for the log entry.
         * @param values The vector of string values to log.
         */
        void log(std::string key, std::vector<std::string> values) override
        {
            auto &entry = stringArrayEntries[key];
            if (!entry)
            {
                entry = wpi::log::StringArrayLogEntry(logRef, key);
            }
            entry.Append(values);
        }

        /**
         * @brief Logs a boolean value to WPILog.
         * @param key The key/name for the log entry.
         * @param value The boolean value to log.
         */
        void log(std::string key, bool value) override
        {
            auto &entry = booleanEntries[key];
            if (!entry)
            {
                entry = wpi::log::BooleanLogEntry(logRef, key);
            }
            entry.Append(value);
        }

        /**
         * @brief Logs a vector of boolean values to WPILog.
         * Converts `std::vector<bool>` to `std::vector<int>` for compatibility.
         * @param key The key/name for the log entry.
         * @param values The vector of boolean values to log.
         */
        void log(std::string key, std::vector<bool> values) override
        {
            auto &entry = booleanArrayEntries[key];
            if (!entry)
            {
                entry = wpi::log::BooleanArrayLogEntry(logRef, key);
            }
            // Convert std::vector<bool> to std::vector<int> for logging
            std::vector<int> boolValues(values.begin(), values.end());
            entry.Append(boolValues);
        }

        /**
         * @brief Logs a 64-bit integer value to WPILog.
         * @param key The key/name for the log entry.
         * @param value The int64_t value to log.
         */
        void log(std::string key, int64_t value) override
        {
            auto &entry = integerEntries[key];
            if (!entry)
            {
                entry = wpi::log::IntegerLogEntry(logRef, key);
            }
            entry.Append(value);
        }

        /**
         * @brief Logs a vector of 64-bit integer values to WPILog.
         * @param key The key/name for the log entry.
         * @param values The vector of int64_t values to log.
         */
        void log(std::string key, std::vector<int64_t> values) override
        {
            auto &entry = integerArrayEntries[key];
            if (!entry)
            {
                entry = wpi::log::IntegerArrayLogEntry(logRef, key);
            }
            entry.Append(values);
        }

        /**
         * @brief Destructor for WPILogManager.
         */
        virtual ~WPILogManager() = default;
    };

    /**
     * @brief Log manager implementation using NetworkTables.
     * This class handles logging data to NetworkTables, allowing real-time
     * viewing and interaction with robot data.
     */
    class NTLogManager : public BaseLogManager
    {
    private:
        std::shared_ptr<nt::NetworkTable> table;
        std::map<std::string, std::shared_ptr<nt::Publisher>> entries;

    public:
        /**
         * @brief Constructs an NTLogManager.
         * Initializes a NetworkTable for logging under the "Logging" subtable.
         * @throws std::runtime_error if the NetworkTable cannot be created.
         */
        NTLogManager()
        {
            table = nt::NetworkTableInstance::GetDefault().GetTable("Logging");
            if (!table)
            {
                throw std::runtime_error("Failed to create NetworkTable for logging.");
            }
        }

        /**
         * @brief Logs a double value to NetworkTables.
         * @param key The key/name for the log entry.
         * @param value The double value to log.
         * @throws std::runtime_error if there's a type mismatch for an existing key.
         */
        void log(const std::string& key, double value) override
        {
            auto &entry = entries[key];
            if (!entry)
            {
                entry = std::make_shared<nt::DoublePublisher>(table->GetDoubleTopic(key).Publish());
            }
            if (auto doubleEntry = std::dynamic_pointer_cast<nt::DoublePublisher>(entry))
            {
                doubleEntry->Set(value);
            }
            else
            {
                throw std::runtime_error("Log entry type mismatch for key: " + key +
                                         ". Expected double, got different type.");
            }
        }

        /**
         * @brief Logs a vector of double values to NetworkTables.
         * @param key The key/name for the log entry.
         * @param values The vector of double values to log.
         * @throws std::runtime_error if there's a type mismatch for an existing key.
         */
        void log(std::string key, std::vector<double> values) override
        {
            auto &entry = entries[key];
            if (!entry)
            {
                entry = std::make_shared<nt::DoubleArrayPublisher>(table->GetDoubleArrayTopic(key).Publish());
            }
            if (auto arrayEntry = std::dynamic_pointer_cast<nt::DoubleArrayPublisher>(entry))
            {
                arrayEntry->Set(values);
            }
            else
            {
                throw std::runtime_error("Log entry type mismatch for key: " + key +
                                         ". Expected double array, got different type.");
            }
        }

        /**
         * @brief Logs a string value to NetworkTables.
         * @param key The key/name for the log entry.
         * @param value The string value to log.
         * @throws std::runtime_error if there's a type mismatch for an existing key.
         */
        void log(std::string key, std::string value) override
        {
            auto &entry = entries[key];
            if (!entry)
            {
                entry = std::make_shared<nt::StringPublisher>(table->GetStringTopic(key).Publish());
            }
            if (auto stringEntry = std::dynamic_pointer_cast<nt::StringPublisher>(entry))
            {
                stringEntry->Set(value);
            }
            else
            {
                throw std::runtime_error("Log entry type mismatch for key: " + key +
                                         ". Expected string, got different type.");
            }
        }

        /**
         * @brief Logs a vector of string values to NetworkTables.
         * @param key The key/name for the log entry.
         * @param values The vector of string values to log.
         * @throws std::runtime_error if there's a type mismatch for an existing key.
         */
        void log(std::string key, std::vector<std::string> values) override
        {
            auto &entry = entries[key];
            if (!entry)
            {
                entry = std::make_shared<nt::StringArrayPublisher>(table->GetStringArrayTopic(key).Publish());
            }
            if (auto arrayEntry = std::dynamic_pointer_cast<nt::StringArrayPublisher>(entry))
            {
                arrayEntry->Set(values);
            }
            else
            {
                throw std::runtime_error("Log entry type mismatch for key: " + key +
                                         ". Expected string array, got different type.");
            }
        }

        /**
         * @brief Logs a boolean value to NetworkTables.
         * @param key The key/name for the log entry.
         * @param value The boolean value to log.
         * @throws std::runtime_error if there's a type mismatch for an existing key.
         */
        void log(std::string key, bool value) override
        {
            auto &entry = entries[key];
            if (!entry)
            {
                entry = std::make_shared<nt::BooleanPublisher>(table->GetBooleanTopic(key).Publish());
            }
            if (auto boolEntry = std::dynamic_pointer_cast<nt::BooleanPublisher>(entry))
            {
                boolEntry->Set(value);
            }
            else
            {
                throw std::runtime_error("Log entry type mismatch for key: " + key +
                                         ". Expected boolean, got different type.");
            }
        }

        /**
         * @brief Logs a vector of boolean values to NetworkTables.
         * Converts `std::vector<bool>` to `std::vector<int>` for compatibility.
         * @param key The key/name for the log entry.
         * @param values The vector of boolean values to log.
         * @throws std::runtime_error if there's a type mismatch for an existing key.
         */
        void log(std::string key, std::vector<bool> values) override
        {
            auto &entry = entries[key];
            if (!entry)
            {
                entry = std::make_shared<nt::BooleanArrayPublisher>(table->GetBooleanArrayTopic(key).Publish());
            }
            if (auto arrayEntry = std::dynamic_pointer_cast<nt::BooleanArrayPublisher>(entry))
            {
                std::vector<int> boolValues(values.begin(), values.end());
                arrayEntry->Set(boolValues);
            }
            else
            {
                throw std::runtime_error("Log entry type mismatch for key: " + key +
                                         ". Expected boolean array, got different type.");
            }
        }

        /**
         * @brief Logs a 64-bit integer value to NetworkTables.
         * @param key The key/name for the log entry.
         * @param value The int64_t value to log.
         * @throws std::runtime_error if there's a type mismatch for an existing key.
         */
        void log(std::string key, int64_t value) override
        {
            auto &entry = entries[key];
            if (!entry)
            {
                entry = std::make_shared<nt::IntegerPublisher>(table->GetIntegerTopic(key).Publish());
            }
            if (auto intEntry = std::dynamic_pointer_cast<nt::IntegerPublisher>(entry))
            {
                intEntry->Set(value);
            }
            else
            {
                throw std::runtime_error("Log entry type mismatch for key: " + key +
                                         ". Expected integer, got different type.");
            }
        }

        /**
         * @brief Logs a vector of 64-bit integer values to NetworkTables.
         * @param key The key/name for the log entry.
         * @param values The vector of int64_t values to log.
         * @throws std::runtime_error if there's a type mismatch for an existing key.
         */
        void log(std::string key, std::vector<int64_t> values) override
        {
            auto &entry = entries[key];
            if (!entry)
            {
                entry = std::make_shared<nt::IntegerArrayPublisher>(table->GetIntegerArrayTopic(key).Publish());
            }
            if (auto arrayEntry = std::dynamic_pointer_cast<nt::IntegerArrayPublisher>(entry))
            {
                arrayEntry->Set(values);
            }
            else
            {
                throw std::runtime_error("Log entry type mismatch for key: " + key +
                                         ". Expected integer array, got different type.");
            }
        }
    };
    class LoggerContext;
    template <typename T>
    class CustomLoggerFor
    {
    public:
        void log(const T &t, LoggerContext&& context)
        {
            // This function should be specialized for each type T that needs custom logging.
            // It can use rfl::to_view to reflect over the members of T and log them.
            // For now, we will throw an error if this is called without specialization.
            throw std::runtime_error("Custom logging not implemented for this type.");
        }
    };
    // A trait class to detect if CustomLoggerFor<T> has a 'log' member
    // with the expected signature.
    template <typename T, typename = void>
    struct has_custom_logger_impl : std::false_type {};

    template <typename T>
    struct has_custom_logger_impl<T, std::void_t<decltype(std::declval<CustomLoggerFor<T>>().log(std::declval<const T&>(), std::declval<LoggerContext&>()))>>
        : std::true_type {};

    // A more user-friendly alias for the trait
    template <typename T>
    inline constexpr bool has_custom_logger_v = has_custom_logger_impl<T>::value;

    template<typename T>
    struct NotLog
    {
    };
    // A trait to check if a type has a not log implementation
    template <typename T, typename = void>
    struct is_not_log_implemented : std::false_type {};
    template <typename T>
    struct is_not_log_implemented<T, std::void_t<decltype(std::declval<T>().NotLog)>> : std::true_type {};

    // A more user-friendly alias for the trait
    template <typename T>
    inline constexpr bool is_not_log_implemented_v = is_not_log_implemented<T>::value;

    /**
     * @brief A composite logger that can manage multiple logging backends.
     * It dispatches log calls to all registered BaseLogManager instances.
     */
    class Logger
    {
    private:
        std::vector<std::shared_ptr<BaseLogManager>> logManagers;

    public:
        /**
         * @brief Constructs a Logger with an initial set of log managers.
         * @param managers A vector of shared pointers to BaseLogManager instances.
         */
        Logger(std::vector<std::shared_ptr<BaseLogManager>> managers)
            : logManagers(std::move(managers)) {}

        /**
         * @brief Adds a new log manager to the logger.
         * @param manager A shared pointer to the BaseLogManager to add.
         */
        void addLogManager(std::shared_ptr<BaseLogManager> manager)
        {
            logManagers.push_back(manager);
        }

        /**
         * @brief Logs a double value through all registered managers.
         * @param key The key/name for the log entry.
         * @param value The double value to log.
         */
        void log(const std::string& key, double value)
        {
            for (const auto &manager : logManagers)
            {
                manager->log(key, value);
            }
        }

        /**
         * @brief Logs a vector of double values through all registered managers.
         * @param key The key/name for the log entry.
         * @param values The vector of double values to log.
         */
        void log(std::string key, std::vector<double> values)
        {
            for (const auto &manager : logManagers)
            {
                manager->log(key, values);
            }
        }

        /**
         * @brief Logs a string value through all registered managers.
         * @param key The key/name for the log entry.
         * @param value The string value to log.
         */
        void log(std::string key, std::string value)
        {
            for (const auto &manager : logManagers)
            {
                manager->log(key, value);
            }
        }

        /**
         * @brief Logs a vector of string values through all registered managers.
         * @param key The key/name for the log entry.
         * @param values The vector of string values to log.
         */
        void log(std::string key, std::vector<std::string> values)
        {
            for (const auto &manager : logManagers)
            {
                manager->log(key, values);
            }
        }

        /**
         * @brief Logs a boolean value through all registered managers.
         * @param key The key/name for the log entry.
         * @param value The boolean value to log.
         */
        void log(std::string key, bool value)
        {
            for (const auto &manager : logManagers)
            {
                manager->log(key, value);
            }
        }

        /**
         * @brief Logs a vector of boolean values through all registered managers.
         * @param key The key/name for the log entry.
         * @param values The vector of boolean values to log.
         */
        void log(std::string key, std::vector<bool> values)
        {
            for (const auto &manager : logManagers)
            {
                manager->log(key, values);
            }
        }

        /**
         * @brief Logs a 64-bit integer value through all registered managers.
         * @param key The key/name for the log entry.
         * @param value The int64_t value to log.
         */
        void log(std::string key, int64_t value)
        {
            for (const auto &manager : logManagers)
            {
                manager->log(key, value);
            }
        }

        /**
         * @brief Logs a vector of 64-bit integer values through all registered managers.
         * @param key The key/name for the log entry.
         * @param values The vector of int64_t values to log.
         */
        void log(std::string key, std::vector<int64_t> values)
        {
            for (const auto &manager : logManagers)
            {
                manager->log(key, values);
            }
        }

        /**
         * @brief Logs a struct/class 's' by reflecting its members.
         * Uses rfl::to_view and rfl::apply to iterate over members and log them
         * individually. Handles type casting for 'int' to 'int64_t'.
         * @tparam T The type of the struct/class to log.
         * @param key The base key/name for the log entries.
         * @param s The struct/class instance to log.
         */
        template <class T>
        void log(const std::string& key, const T &s)
        {
            if constexpr (is_not_log_implemented_v<T>)
            {
                // If a NotLog implementation is provided, skip logging this type
                return;
            }
            if constexpr (has_custom_logger_v<T>)
            {
                // If a custom logger is implemented for this type, use it
                CustomLoggerFor<T> customLogger;
                customLogger.log(s, LoggerContext(*this, key));
                return;
            }
            const auto view = rfl::to_view(s);
            view.apply([&](const auto &field)
                       {
                for (const auto &manager : logManagers)
                {
                    using MemberValueType = std::remove_cvref_t<decltype(*field.value())>;
                    if constexpr (is_not_log_implemented_v<MemberValueType>) {
                        // If a NotLog implementation is provided, skip logging this field
                        return;
                    }
                    else if constexpr (std::is_same_v<MemberValueType, int>) {
                        // Cast int to int64_t to resolve ambiguity
                        manager->log(key + "/" + std::string(field.name()), static_cast<int64_t>(*field.value()));
                    } else if constexpr (std::is_same_v<MemberValueType, double>) {
                        manager->log(key + "/" + std::string(field.name()), *field.value());
                    } else if constexpr (std::is_same_v<MemberValueType, bool>) {
                        manager->log(key + "/" + std::string(field.name()), *field.value());
                    } else if constexpr (std::is_same_v<MemberValueType, int64_t>) {
                        manager->log(key + "/" + std::string(field.name()), *field.value());
                    } else if constexpr (std::is_same_v<MemberValueType, std::string>) {
                        manager->log(key + "/" + std::string(field.name()), *field.value());
                    }
                    // Add checks for vector types if they are expected to be reflected
                    else if constexpr (std::is_same_v<MemberValueType, std::vector<double>>) {
                        manager->log(key + "/" + std::string(field.name()), *field.value());
                    } else if constexpr (std::is_same_v<MemberValueType, std::vector<std::string>>) {
                        manager->log(key + "/" + std::string(field.name()), *field.value());
                    } else if constexpr (std::is_same_v<MemberValueType, std::vector<bool>>) {
                        manager->log(key + "/" + std::string(field.name()), *field.value());
                    } else if constexpr (std::is_same_v<MemberValueType, std::vector<int64_t>>) {
                        manager->log(key + "/" + std::string(field.name()), *field.value());
                    }
                    else if constexpr (has_custom_logger_v<MemberValueType>) {
                        // If a custom logger is implemented for this type, use it
                        CustomLoggerFor<MemberValueType> customLogger;
                        customLogger.log(*field.value(), LoggerContext(*this, key + "/" + std::string(field.name())));
                    }
                    else 
                    {
                        // If no specific logging is implemented, log the value as a string
                        manager->log(key + "/" + std::string(field.name()), std::to_string(*field.value()));
                    }
                } });
        }

        /**
         * @brief Destructor for Logger.
         */
        ~Logger() = default;

        /**
         * @brief Provides a context for logging, allowing chained operations.
         * Example: `logger["mySubsystem"]["myValue"] << 10.0;`
         */
        class LoggerContext
        {
        private:
            Logger &logger;
            std::string keyName;

        public:
            /**
             * @brief Constructs a LoggerContext.
             * @param logger The Logger instance to use for logging.
             * @param keyName The base key name for this context.
             */
            LoggerContext(Logger &logger, const std::string &keyName)
                : logger(logger), keyName(keyName) {}

            /**
             * @brief Logs a double value using the current context's key.
             * @param value The double value to log.
             * @return A const reference to this LoggerContext for chaining.
             */
            const LoggerContext &operator<<(double value) const
            {
                logger.log(keyName, value);
                return *this;
            }

            /**
             * @brief Logs a vector of double values using the current context's key.
             * @param values The vector of double values to log.
             * @return A const reference to this LoggerContext for chaining.
             */
            const LoggerContext &operator<<(const std::vector<double> &values) const
            {
                logger.log(keyName, values);
                return *this;
            }

            /**
             * @brief Logs a string value using the current context's key.
             * @param value The string value to log.
             * @return A const reference to this LoggerContext for chaining.
             */
            const LoggerContext &operator<<(const std::string &value) const
            {
                logger.log(keyName, value);
                return *this;
            }

            /**
             * @brief Logs a vector of string values using the current context's key.
             * @param values The vector of string values to log.
             * @return A const reference to this LoggerContext for chaining.
             */
            const LoggerContext &operator<<(const std::vector<std::string> &values) const
            {
                logger.log(keyName, values);
                return *this;
            }

            /**
             * @brief Logs a boolean value using the current context's key.
             * @param value The boolean value to log.
             * @return A const reference to this LoggerContext for chaining.
             */
            const LoggerContext &operator<<(bool value) const
            {
                logger.log(keyName, value);
                return *this;
            }

            /**
             * @brief Logs a vector of boolean values using the current context's key.
             * @param values The vector of boolean values to log.
             * @return A const reference to this LoggerContext for chaining.
             */
            const LoggerContext &operator<<(const std::vector<bool> &values) const
            {
                logger.log(keyName, values);
                return *this;
            }

            /**
             * @brief Logs a 64-bit integer value using the current context's key.
             * @param value The int64_t value to log.
             * @return A const reference to this LoggerContext for chaining.
             */
            const LoggerContext &operator<<(int64_t value) const
            {
                logger.log(keyName, value);
                return *this;
            }

            /**
             * @brief Logs a vector of 64-bit integer values using the current context's key.
             * @param values The vector of int64_t values to log.
             * @return A const reference to this LoggerContext for chaining.
             */
            const LoggerContext &operator<<(const std::vector<int64_t> &values) const
            {
                logger.log(keyName, values);
                return *this;
            }

            /**
             * @brief Logs a custom type T using the current context's key.
             * This overload allows logging of structs/classes that are reflectable by rfl.
             * @tparam T The type of the value to log.
             * @param value The value of type T to log.
             * @return A const reference to this LoggerContext for chaining.
             */
            template <typename T>
            const LoggerContext &operator<<(const T &value) const
            {
                logger.log(keyName, value);
                return *this;
            }

            /**
             * @brief Creates a new LoggerContext with a sub-key, allowing hierarchical logging.
             * Example: `logger["parent"]["child"] << 1.0;`
             * @param subKey The sub-key to append to the current context's key.
             * @return A new LoggerContext object.
             */
            LoggerContext operator[](const std::string &subKey) const
            {
                return LoggerContext(logger, keyName + "/" + subKey);
            }
        };

        /**
         * @brief Creates a new LoggerContext for a given base key.
         * Example: `logger["mySubsystem"] << 5.0;`
         * @param keyName The base key name for the new context.
         * @return A new LoggerContext object.
         */
        LoggerContext operator[](const std::string &keyName)
        {
            return LoggerContext(*this, keyName);
        }
    };

}
