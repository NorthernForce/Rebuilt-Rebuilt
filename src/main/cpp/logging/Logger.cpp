#include <logging/Logger.h>

#include <iostream>
#include <memory>
#include <sstream>

using namespace nfr;
using namespace std;

LogContext::LogContext(const string& key, Logger* logger)
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

const LogContext& LogContext::operator<<(const string& value) const
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

const LogContext& LogContext::operator<<(span<string> values) const
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

void Logger::Log(const string& key, double value)
{
    for (auto& output : outputs)
    {
        output->Log(key, value);
    }
}

void Logger::Log(const string& key, long value)
{
    for (auto& output : outputs)
    {
        output->Log(key, value);
    }
}

void Logger::Log(const string& key, bool value)
{
    for (auto& output : outputs)
    {
        output->Log(key, value);
    }
}

void Logger::Log(const string& key, const string& value)
{
    for (auto& output : outputs)
    {
        output->Log(key, value);
    }
}

void Logger::Log(const string& key, span<double> values)
{
    for (auto& output : outputs)
    {
        output->Log(key, values);
    }
}

void Logger::Log(const string& key, span<long> values)
{
    for (auto& output : outputs)
    {
        output->Log(key, values);
    }
}

void Logger::Log(const string& key, span<bool> values)
{
    for (auto& output : outputs)
    {
        output->Log(key, values);
    }
}

void Logger::Log(const string& key, span<string> values)
{
    for (auto& output : outputs)
    {
        output->Log(key, values);
    }
}

void Logger::AddOutput(shared_ptr<ILogOutput> output)
{
    outputs.push_back(std::move(output));
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