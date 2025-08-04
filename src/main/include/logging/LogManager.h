#pragma once

#include <span>
#include <string>

namespace nfr
{
    class ILogOutput
    {
    public:
        virtual void Log(const std::string& key, double value) = 0;
        virtual void Log(const std::string& key, long value) = 0;
        virtual void Log(const std::string& key, bool value) = 0;
        virtual void Log(const std::string& key, const std::string& value) = 0;
        virtual void Log(const std::string& key, std::span<double> values) = 0;
        virtual void Log(const std::string& key, std::span<long> values) = 0;
        virtual void Log(const std::string& key, std::span<bool> values) = 0;
        virtual void Log(const std::string& key,
                         std::span<std::string> values) = 0;
    };
}  // namespace nfr