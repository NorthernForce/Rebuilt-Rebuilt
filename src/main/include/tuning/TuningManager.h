#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace nfr
{

/**
 * @brief Manages all tunable values and provides periodic updates
 */
class TuningManager
{
public:
    /**
     * @brief Get the singleton instance
     */
    static TuningManager& GetInstance();

    /**
     * @brief Register a tunable for periodic updates
     */
    void RegisterTunable(std::function<void()> updateFunction);

    /**
     * @brief Update all registered tunables (call this periodically)
     */
    void UpdateAll();

    /**
     * @brief Clear all registered tunables
     */
    void Clear();

private:
    TuningManager() = default;
    ~TuningManager() = default;
    TuningManager(const TuningManager&) = delete;
    TuningManager& operator=(const TuningManager&) = delete;

    std::vector<std::function<void()>> updateFunctions_;
};

} // namespace nfr