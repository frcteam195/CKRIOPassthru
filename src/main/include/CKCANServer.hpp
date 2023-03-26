#pragma once
#include "utils/Singleton.hpp"
#include "utils/drivers/CKCANDevice.hpp"
#include "utils/drivers/CKCANPacket.hpp"
#include "utils/ThreadRateControl.hpp"
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>

class CKCANServer : public Singleton<CKCANServer>
{
    friend Singleton;
public:
    bool sendPacket(CKCANPacket& canPacket);
    bool sendPacket(CKCANDevice* canDev, uint8_t* data, int dataLength, int apiID);

private:
    CKCANServer();
    ~CKCANServer();

    void runCANSendThread();
    std::vector<CKCANPacket> mPacketList;
    std::mutex mPacketMutex;
    std::atomic_bool mThreadActive;

    ThreadRateControl mRateControl;

    static constexpr int CAN_SERVER_THREAD_RATE_MS = 10;
    
    std::thread mThread;
};