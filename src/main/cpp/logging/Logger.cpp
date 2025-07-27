#include <logging/Logger.h>

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

namespace nfr
{
Logger logger;
}