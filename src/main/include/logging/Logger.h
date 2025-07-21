#pragma once

#include <logging/LogManager.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

namespace nfr
{
    class LogContext;
    template<typename T>
    concept HasLogMethod = requires(T t, const LogContext& logContext)
    {
        { t.Log(logContext) } -> std::same_as<void>;
    };
    template<typename T>
    void Log(const LogContext& logContext, const T& t)
    {
        static_assert(HasLogMethod<T>, "Type T does not have a Log method that matches the required signature.");
    }
    template<typename T>
    concept ExistsLogMethodFor = requires(T t, const LogContext& logContext)
    {
        { Log<T>(logContext, t) } -> std::same_as<void>;
    } && !std::same_as<T, double> && !std::same_as<T, int> && !std::same_as<T, long> && !std::same_as<T, bool> && !std::same_as<T, std::string>
        && !std::same_as<T, std::span<double>> && !std::same_as<T, std::span<long>> && !std::same_as<T, std::span<bool>> && !std::same_as<T, std::span<std::string>>;
    class Logger;
    class LogContext
    {
    public:
        LogContext(const std::string& key, Logger* logger);
        LogContext(const LogContext&) = delete;
        LogContext& operator=(const LogContext&) = delete;
        LogContext(LogContext&&) = default;
        LogContext& operator=(LogContext&&) = default;
        ~LogContext() = default;
        const LogContext& operator<<(double value) const;
        const LogContext& operator<<(long value) const;
        const LogContext& operator<<(int value) const
        {
            return operator<<(static_cast<long>(value));
        }
        const LogContext& operator<<(bool value) const;
        const LogContext& operator<<(const std::string& value) const;
        const LogContext& operator<<(std::span<double> values) const;
        const LogContext& operator<<(std::span<long> values) const;
        const LogContext& operator<<(std::span<bool> values) const;
        const LogContext& operator<<(std::span<std::string> values) const;
        template<typename T>
        requires ExistsLogMethodFor<T>
        const LogContext& operator<<(const T& value) const
        {
            Log<T>(*this, value);
            return *this;
        }
        template<typename T>
        requires ExistsLogMethodFor<T>
        const LogContext& operator<<(T&& value) const {
            Log<T>(*this, std::forward<T>(value));
            return std::move(*this);
        }
        LogContext operator[](std::string_view newKey) const
        {
            return LogContext{key + "/" + std::string(newKey), logger};
        }
    private:
        std::string key;
        Logger* logger;
    };
    class Logger
    {
    public:
        Logger() = default;
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
        template<typename T>
        requires ExistsLogMethodFor<T>
        void Log(const std::string& key, const T& value);
        void AddOutput(std::shared_ptr<ILogOutput> output);
        LogContext operator[](std::string_view key)
        {
            return LogContext{std::string(key), this};
        }
    private:
        std::vector<std::shared_ptr<ILogOutput>> outputs;
    };

    extern Logger logger; // Global logger instance
}