#include <logging/Logger.h>

#include <iostream>
#include <memory>
#include <sstream>

using namespace nfr;
using namespace std;

LogContext::LogContext(const string_view& key, Logger* logger)
    : key(key), logger(logger)
{
}

const LogContext& LogContext::operator<<(double value) const
{
    logger->Log(key, value);
    return *this;
}

const LogContext& LogContext::operator<<(long value) const
{
    logger->Log(key, value);
    return *this;
}

const LogContext& LogContext::operator<<(bool value) const
{
    logger->Log(key, value);
    return *this;
}

const LogContext& LogContext::operator<<(const string_view& value) const
{
    logger->Log(key, value);
    return *this;
}

const LogContext& LogContext::operator<<(span<double> values) const
{
    logger->Log(key, values);
    return *this;
}

const LogContext& LogContext::operator<<(span<long> values) const
{
    logger->Log(key, values);
    return *this;
}

const LogContext& LogContext::operator<<(span<bool> values) const
{
    logger->Log(key, values);
    return *this;
}

const LogContext& LogContext::operator<<(span<string_view> values) const
{
    logger->Log(key, values);
    return *this;
}

Logger::Logger()
    // Initialize shared stringstreams for logging
    : original_cout_buf_(std::cout.rdbuf()),
      original_cerr_buf_(std::cerr.rdbuf()),
      // Save original streambufs
      cout_log_stream_(std::make_shared<std::stringstream>()),
      cerr_log_stream_(std::make_shared<std::stringstream>()),
      // Initialize tee streambufs with original and log streams
      cout_tee_buf_(original_cout_buf_, cout_log_stream_),
      cerr_tee_buf_(original_cerr_buf_, cerr_log_stream_)

{
    // Set cout and cerr to use our tee streambufs
    std::cout.rdbuf(&cout_tee_buf_);
    std::cerr.rdbuf(&cerr_tee_buf_);
}

void Logger::Log(const string_view& key, double value)
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

void Logger::Log(const string_view& key, long value)
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

void Logger::Log(const string_view& key, bool value)
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

void Logger::Log(const string_view& key, const string_view& value)
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

void Logger::Log(const string_view& key, span<double> values)
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

void Logger::Log(const string_view& key, span<long> values)
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

void Logger::Log(const string_view& key, span<bool> values)
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

void Logger::Log(const string_view& key, span<string_view> values)
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

void Logger::EnableNTLogging(const string_view& tableName)
{
    if (!nt_log_manager_)
    {
        nt_log_manager_ = std::make_unique<NTLogManager>(tableName);
    }
}

void Logger::EnableWPILogging()
{
    if (!wpi_log_manager_)
    {
        wpi_log_manager_ = std::make_unique<WPILogManager>();
    }
}

void Logger::Flush()
{
    std::string cout_log = cout_log_stream_->str();
    std::string cerr_log = cerr_log_stream_->str();
    cout_log_stream_->str("");  // Clear the stringstream
    cerr_log_stream_->str("");  // Clear the stringstream
    Log("cout", cout_log);
    Log("cerr", cerr_log);
}

namespace nfr
{
    Logger logger;
}