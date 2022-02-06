#pragma once
#include "utils/Singleton.hpp"
#include <mutex>
#include <atomic>

class ExternalControlManager : public Singleton<ExternalControlManager>
{
    friend Singleton;
public:
    void externalControlMsgReceived();
    bool isExternalControlConnected();
    void processHeartbeat();
private:
    ExternalControlManager();
    ~ExternalControlManager();

    std::atomic<int> mLastConnectionTime = 0;
};

extern bool isExternalControl();