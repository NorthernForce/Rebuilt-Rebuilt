#pragma once

#include <memory>
#include <span>
#include <sstream>
#include <streambuf>  // Include for std::streambuf
#include <string>

#include "logging/NTLogManager.h"
#include "logging/WPILogManager.h"
#include "units/base.h"
#include "wpi/struct/Struct.h"

namespace nfr
{
    class LogContext;
    // template <typename T>
    // void Log(const LogContext& logContext, const T& t);
    template <typename T>
    concept HasLogMethod = requires(T t, const LogContext& logContext) {
        { t.Log(logContext) } -> std::same_as<void>;
    };
    template <typename T>
    concept ExistsLogMethodFor = requires(T t, const LogContext& logContext) {
        { Log(logContext, t) } -> std::same_as<void>;
    };

    template <typename T>
    concept ExistsPointerLogMethodFor =
        requires(T t, const LogContext& logContext) {
            { Log(logContext, *t) } -> std::same_as<void>;
        };
    template <typename T>
    concept HasPointerLogMethod = requires(T t, const LogContext& logContext) {
        { t->Log(logContext) } -> std::same_as<void>;
    };
    class Logger;
    class LogContext
    {
    public:
        LogContext(const std::string_view& key, Logger* logger);
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
        const LogContext& operator<<(const std::string_view& value) const;
        const LogContext& operator<<(std::span<double> values) const;
        const LogContext& operator<<(std::span<long> values) const;
        const LogContext& operator<<(std::span<bool> values) const;
        const LogContext& operator<<(std::span<std::string_view> values) const;
        template <typename T>
            requires ExistsLogMethodFor<T>
        const LogContext& operator<<(const T& value) const
        {
            Log(*this, value);
            return *this;
        }
        template <typename T>
            requires HasPointerLogMethod<T>
        const LogContext& operator<<(T&& value) const
        {
            if (value)
            {
                value->Log(*this);
            }
            return *this;
        }
        template <typename T>
            requires ExistsPointerLogMethodFor<T>
        const LogContext& operator<<(const T* value) const
        {
            if (value)
            {
                Log(*this, *value);
            }
            return *this;
        }
        template <typename T>
            requires HasLogMethod<T>
        const LogContext& operator<<(const T& value) const
        {
            value.Log(*this);
            return *this;
        }
        LogContext operator[](std::string_view newKey) const
        {
            return LogContext{key + "/" + std::string(newKey), logger};
        }

        Logger* GetLogger() const
        {
            return logger;
        }

        std::string GetKey() const
        {
            return key;
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
            if (wpi_log_manager_)
            {
                wpi_log_manager_->Log(key, value);
            }
            if (nt_log_manager_)
            {
                nt_log_manager_->Log(key, value);
            }
        }
        template <typename T, typename... I>
            requires wpi::StructSerializable<T, I...>
        void Log(const std::string_view& key, std::span<T> values)
        {
            if (wpi_log_manager_)
            {
                wpi_log_manager_->Log(key, values);
            }
            if (nt_log_manager_)
            {
                nt_log_manager_->Log(key, values);
            }
        }
        void EnableNTLogging(const std::string_view& tableName = "logs");
        void EnableWPILogging();
        LogContext operator[](std::string_view key)
        {
            return LogContext{std::string(key), this};
        }
        void Flush();

    private:
        std::unique_ptr<NTLogManager> nt_log_manager_{nullptr};
        std::unique_ptr<WPILogManager> wpi_log_manager_{nullptr};

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

    template <typename T>
        requires wpi::StructSerializable<T>
    inline const LogContext& operator<<(const LogContext& logContext,
                                        const T& t)
    {
        logContext.GetLogger()->Log(logContext.GetKey(), t);
        return logContext;
    }

    template <typename T>
        requires wpi::StructSerializable<T>
    inline const LogContext& operator<<(const LogContext& logContext,
                                        std::span<T> values)
    {
        logContext.GetLogger()->Log(logContext.GetKey(), values);
        return logContext;
    }

    template <typename T>
        requires units::traits::is_unit_t_v<T>
    inline const LogContext& operator<<(const LogContext& logContext,
                                        const T& t)
    {
        logContext.GetLogger()->Log(logContext.GetKey(),
                                    static_cast<double>(t));
        return logContext;
    }

    template <typename T>
        requires units::traits::is_unit_t_v<T>
    inline const LogContext& operator<<(const LogContext& logContext,
                                        std::span<T> values)
    {
        std::vector<double> double_values;
        double_values.reserve(values.size());
        for (const auto& v : values)
        {
            double_values.push_back(static_cast<double>(v));
        }
        logContext.GetLogger()->Log(logContext.GetKey(),
                                    std::span<double>{double_values});
        return logContext;
    }

    extern Logger logger;  // Global logger instance
}  // namespace nfr