#pragma once

#include <filesystem>

#include "frc/Filesystem.h"
#include "frc2/command/SubsystemBase.h"
namespace nfr
{
    /** Mostly empty for now pending further implementation by @Daboss-1 */
    class Dashboard : public frc2::SubsystemBase
    {
    public:
        Dashboard(
            std::filesystem::path distPath =
                std::filesystem::path(frc::filesystem::GetDeployDirectory()) /
                "dist",
            int port = 5800);
        ~Dashboard();

    private:
        int port;
    };
}  // namespace nfr