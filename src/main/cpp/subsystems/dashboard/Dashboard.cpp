#include "subsystems/dashboard/Dashboard.h"

#include <wpinet/WebServer.h>

namespace nfr
{
Dashboard::Dashboard(std::filesystem::path distPath, int port) : port(port)
{
    wpi::WebServer::GetInstance().Start(port, distPath.string());
}
Dashboard::~Dashboard()
{
    wpi::WebServer::GetInstance().Stop(port);
}
}  // namespace nfr