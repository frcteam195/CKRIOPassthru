#pragma once
#include "frc/DriverStation.h"
#include "utils/Singleton.hpp"
#include "utils/ThreadRateControl.hpp"
#include <atomic>
#include <thread>

class DSConnectionMonitor : public Singleton<DSConnectionMonitor>
{
    friend Singleton;
public:
    bool isConnected();
    void updateConnectionStatus();

private:
    DSConnectionMonitor();
    ~DSConnectionMonitor();
    std::atomic_bool hasConnection {false};
};