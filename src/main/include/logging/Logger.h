/**
 * @file Logger.h
 * @brief Custom logging system for FRC robot debugging and analysis
 *
 * This logging system helps us debug robot problems and analyze performance
 * by recording data during robot operation. The data can be viewed in real-time
 * or saved for later analysis.
 *
 * ## Why Custom Logging?
 * While WPILib has basic logging, this system provides:
 * - Structured data organization (nested keys like "drive/frontLeft/position")
 * - Multiple output targets (files, NetworkTables, etc.)
 * - Type-safe logging with automatic conversions
 * - Performance optimization for real-time operation
 *
 * ## Basic Usage:
 * ```cpp
 * nfr::logger["subsystem"] << value;           // Log a single value
 * nfr::logger["robot"]["position"] << pose;    // Nested logging
 * ```
 *
 * ## For Students:
 * Think of this like a super-powered `std::cout` that can organize data,
 * send it to multiple places, and handle complex robotics data types
 * automatically.
 */

#pragma once

#include <logging/LogManager.h>

#include <memory>
#include <span>
#include <sstream>
#include <streambuf>  // Include for std::streambuf
#include <string>
#include <vector>

namespace nfr
{
    class LogContext;

    // === TEMPLATE CONCEPTS FOR TYPE SAFETY ===
    // These concepts determine which types can be logged and how

    /**
     * @brief Concept for objects with a Log() method
     *
     * Many robot subsystems have a Log() method that records their state.
     * This concept identifies such objects so they can be logged automatically.
     *
     * Example: `drivetrain.Log(logContext)`
     */
    template <typename T>
    concept HasLogMethod = requires(T t, const LogContext& logContext) {
        { t.Log(logContext) } -> std::same_as<void>;
    };

    /**
     * @brief Concept for types that have a standalone Log() function
     *
     * Some types (like Pose2d) are logged via free functions rather than
     * methods. This concept identifies such types.
     *
     * Example: `Log(logContext, robotPose)`
     */
    template <typename T>
    concept ExistsLogMethodFor = requires(T t, const LogContext& logContext) {
        { Log(logContext, t) } -> std::same_as<void>;
    };

    /**
     * @brief Concept for pointer types that can be dereferenced and logged
     */
    template <typename T>
    concept ExistsPointerLogMethodFor =
        requires(T t, const LogContext& logContext) {
            { Log(logContext, *t) } -> std::same_as<void>;
        };

    /**
     * @brief Concept for smart pointers with Log() methods
     */
    template <typename T>
    concept HasPointerLogMethod = requires(T t, const LogContext& logContext) {
        { t->Log(logContext) } -> std::same_as<void>;
    };

    class Logger;
    /**
     * @brief Context for logging data with hierarchical keys
     *
     * LogContext represents a "location" in the log data structure where you
     * can write values. It manages the hierarchical key structure that
     * organizes logged data.
     *
     * ## Key Structure:
     * Keys are like file paths: "robot/drivetrain/frontLeft/speed"
     * This creates a nested structure that's easy to navigate in log viewers.
     *
     * ## Example Usage:
     * ```cpp
     * LogContext robotLog = logger["robot"];
     * robotLog["battery_voltage"] << 12.3;
     * robotLog["drivetrain"]["speed"] << 2.5;
     * ```
     *
     * ## Design Notes:
     * - Move-only type (can't be copied) for performance
     * - Supports many data types via operator<< overloads
     * - Uses template concepts to determine how to log different types
     */
    class LogContext
    {
    public:
        /**
         * @brief Constructor - creates a log context for a specific key path
         * @param key The hierarchical key (like "robot/drivetrain/speed")
         * @param logger Pointer to the logger that will handle the actual
         * output
         */
        LogContext(const std::string& key, Logger* logger);

        // Prevent copying (for performance - LogContext objects can be large)
        LogContext(const LogContext&) = delete;
        LogContext& operator=(const LogContext&) = delete;

        // Allow moving (efficient transfer of ownership)
        LogContext(LogContext&&) = default;
        LogContext& operator=(LogContext&&) = default;
        ~LogContext() = default;

        // === BASIC DATA TYPE LOGGING ===
        // These operators handle built-in types like numbers, strings, etc.

        /** @brief Log a decimal number */
        const LogContext& operator<<(double value) const;

        /** @brief Log an integer number */
        const LogContext& operator<<(long value) const;

        /** @brief Log an integer (converts to long internally) */
        const LogContext& operator<<(int value) const
        {
            return operator<<(static_cast<long>(value));
        }
        /** @brief Log a boolean value (true/false) */
        const LogContext& operator<<(bool value) const;

        /** @brief Log a string value */
        const LogContext& operator<<(const std::string& value) const;

        // === ARRAY LOGGING ===
        // These handle arrays/lists of values efficiently

        /** @brief Log an array of decimal numbers */
        const LogContext& operator<<(std::span<double> values) const;

        /** @brief Log an array of integers */
        const LogContext& operator<<(std::span<long> values) const;

        /** @brief Log an array of boolean values */
        const LogContext& operator<<(std::span<bool> values) const;

        /** @brief Log an array of strings */
        const LogContext& operator<<(std::span<std::string> values) const;

        // === TEMPLATE OPERATORS FOR CUSTOM TYPES ===
        // These use C++ concepts to determine how to log different object types

        /**
         * @brief Log objects that have a standalone Log() function
         *
         * For types like Pose2d where we define: `void Log(LogContext&, const
         * Pose2d&)`
         */
        template <typename T>
            requires ExistsLogMethodFor<T>
        const LogContext& operator<<(const T& value) const
        {
            Log(*this, value);  // Call the standalone Log() function
            return *this;
        }

        /**
         * @brief Log smart pointers that have Log() methods
         *
         * For smart pointers like std::unique_ptr<Subsystem> where the
         * pointed-to object has a Log() method. Safely handles null pointers.
         */
        template <typename T>
            requires HasPointerLogMethod<T>
        const LogContext& operator<<(T&& value) const
        {
            if (value)  // Check for null pointer
            {
                value->Log(*this);
            }
            return *this;
        }

        /**
         * @brief Log raw pointers to objects with standalone Log() functions
         *
         * Safely handles null pointers.
         */
        template <typename T>
            requires ExistsPointerLogMethodFor<T>
        const LogContext& operator<<(const T* value) const
        {
            if (value)  // Check for null pointer
            {
                Log(*this, *value);
            }
            return *this;
        }

        /**
         * @brief Log objects that have their own Log() method
         *
         * For objects like subsystems that implement: `void Log(const
         * LogContext&) const`
         */
        template <typename T>
            requires HasLogMethod<T>
        const LogContext& operator<<(const T& value) const
        {
            value.Log(*this);  // Call the object's Log() method
            return *this;
        }

        /**
         * @brief Create a nested log context with a sub-key
         *
         * This operator creates hierarchical log structures. If the current
         * context has key "robot", then `context["drivetrain"]` creates a new
         * context with key "robot/drivetrain".
         *
         * @param newKey The sub-key to append
         * @return New LogContext for the nested key
         */
        LogContext operator[](std::string_view newKey) const
        {
            return LogContext{key + "/" + std::string(newKey), logger};
        }

    private:
        std::string key;
        Logger* logger;
    };

    // New custom streambuf to duplicate output
    class TeeStreamBuf : public std::streambuf
    {
    public:
        // Constructor takes the primary buffer (for original destination)
        // and a secondary stringstream (for logging)
        TeeStreamBuf(std::streambuf* primaryBuf,
                     std::shared_ptr<std::stringstream> logStream)
            : primary_buf_(primaryBuf), log_stream_(logStream)
        {
        }

    protected:
        int_type overflow(int_type c) override
        {
            if (c == traits_type::eof())
            {
                return traits_type::eof();
            }

            // Write to the primary buffer (original destination)
            if (primary_buf_->sputc(c) == traits_type::eof())
            {
                return traits_type::eof();
            }

            // Write to the logging stringstream
            log_stream_->put(c);

            return c;
        }

        int sync() override
        {
            int ret = 0;
            if (primary_buf_->pubsync() == -1)
            {
                ret = -1;
            }
            // No explicit sync needed for stringstream as it buffers
            // internally.
            return ret;
        }

    private:
        std::streambuf* primary_buf_;  // Original streambuf (e.g., std::cout's
                                       // original buffer)
        std::shared_ptr<std::stringstream>
            log_stream_;  // Stringstream for logger
    };

    class Logger
    {
    public:
        Logger();
        // Delete copy constructor and assignment operator
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        void Log(const std::string& key, double value);
        void Log(const std::string& key, long value);
        void Log(const std::string& key, bool value);
        void Log(const std::string& key, const std::string& value);
        void Log(const std::string& key, std::span<double> values);
        void Log(const std::string& key, std::span<long> values);
        void Log(const std::string& key, std::span<bool> values);
        void Log(const std::string& key, std::span<std::string> values);
        void AddOutput(std::shared_ptr<ILogOutput> output);
        LogContext operator[](std::string_view key)
        {
            return LogContext{std::string(key), this};
        }
        void Flush();

    private:
        std::vector<std::shared_ptr<ILogOutput>> outputs;

        // Store original streambufs
        std::streambuf* original_cout_buf_;
        std::streambuf* original_cerr_buf_;

        // Stringstreams to capture cout/cerr for logging
        std::shared_ptr<std::stringstream> cout_log_stream_;
        std::shared_ptr<std::stringstream> cerr_log_stream_;

        // Our tee streambufs
        TeeStreamBuf cout_tee_buf_;
        TeeStreamBuf cerr_tee_buf_;
    };

    extern Logger logger;  // Global logger instance
}  // namespace nfr