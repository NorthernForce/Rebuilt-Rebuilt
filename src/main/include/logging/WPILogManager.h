#pragma once

#include <logging/LogManager.h>
#include <wpi/DataLog.h>

#include <unordered_map>
#include <variant>

namespace nfr
{
    class WPILogManager : public ILogOutput
    {
      public:
        WPILogManager();
        void Log(const std::string& key, double value) override;
        void Log(const std::string& key, long value) override;
        void Log(const std::string& key, bool value) override;
        void Log(const std::string& key, const std::string& value) override;
        void Log(const std::string& key, std::span<double> values) override;
        void Log(const std::string& key, std::span<long> values) override;
        void Log(const std::string& key, std::span<bool> values) override;
        void Log(const std::string& key,
                 std::span<std::string> values) override;

      private:
        wpi::log::DataLog& logRef;
        std::unordered_map<
            std::string,
            std::variant<
                wpi::log::DoubleLogEntry, wpi::log::BooleanLogEntry,
                wpi::log::IntegerLogEntry, wpi::log::StringLogEntry,
                wpi::log::DoubleArrayLogEntry, wpi::log::BooleanArrayLogEntry,
                wpi::log::IntegerArrayLogEntry, wpi::log::StringArrayLogEntry>>
            entries;
    };
}  // namespace nfr