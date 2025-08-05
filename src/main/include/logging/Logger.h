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