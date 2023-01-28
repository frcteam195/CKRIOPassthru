#include "utils/DSConnectionMonitor.hpp"
#include <functional>

DSConnectionMonitor::DSConnectionMonitor() {}

DSConnectionMonitor::~DSConnectionMonitor() {}

bool DSConnectionMonitor::isConnected()
{
    return hasConnection;
}

void DSConnectionMonitor::updateConnectionStatus()
{
    hasConnection = frc::DriverStation::IsDSAttached();
}