#include "subsystems/dashboard/Dashboard.h"

#include <wpinet/WebServer.h>

#include <iostream>

namespace nfr
{
Dashboard::Dashboard(std::filesystem::path distPath, int port) : port(port)
{
    try
    {
        wpi::WebServer::GetInstance().Start(port, distPath.string());
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to start web server: " << e.what() << std::endl;
        throw;
    }
}
Dashboard::~Dashboard()
{
    try
    {
        wpi::WebServer::GetInstance().Stop(port);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to stop web server: " << e.what() << std::endl;
    }
}
}  // namespace nfr