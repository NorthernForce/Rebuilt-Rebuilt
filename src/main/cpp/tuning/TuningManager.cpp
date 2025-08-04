#include <tuning/TuningManager.h>

namespace nfr
{

TuningManager& TuningManager::GetInstance()
{
    static TuningManager instance;
    return instance;
}

void TuningManager::RegisterTunable(std::function<void()> updateFunction)
{
    updateFunctions_.push_back(updateFunction);
}

void TuningManager::UpdateAll()
{
    for (auto& updateFunction : updateFunctions_)
    {
        updateFunction();
    }
}

void TuningManager::Clear()
{
    updateFunctions_.clear();
}

} // namespace nfr