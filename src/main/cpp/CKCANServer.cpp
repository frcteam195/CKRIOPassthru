#include "CKCANServer.hpp"
#include <iostream>

CKCANServer::CKCANServer() : mThreadActive(true), mThread(&CKCANServer::runCANSendThread, this) {}

CKCANServer::~CKCANServer()
{
    mThreadActive = false;
    if (mThread.joinable())
    {
        mThread.join();
    }
}

bool CKCANServer::sendPacket(CKCANPacket& canPacket)
{
    bool lockAcquired = mPacketMutex.try_lock();
    if (lockAcquired)
    {
        mPacketList.push_back(canPacket);
        mPacketMutex.unlock();
    }
    return lockAcquired;
}

bool CKCANServer::sendPacket(CKCANDevice* canDev, uint8_t* data, int dataLength, int apiID)
{
    CKCANPacket canPacket = { canDev, apiID, std::vector<uint8_t>(data, data + dataLength) };
    return sendPacket(canPacket);
}

void CKCANServer::runCANSendThread()
{
    mRateControl.start();
    while (mThreadActive)
    {
        std::lock_guard<std::mutex> lock(mPacketMutex);
        if (mPacketList.size())
        {
            for (auto it = mPacketList.begin(); it != mPacketList.end(); )
            {
                CKCANPacket& canPacket = mPacketList.front();
                int status = canPacket.canDev->getCANDev().WritePacketNoError(&canPacket.data.front(), canPacket.data.size(), canPacket.apiID);
                //TODO: Consider removing only if status == 0
                it = mPacketList.erase(it);
                (void)status;
            }
        }
        mRateControl.doRateControl(CAN_SERVER_THREAD_RATE_MS);
    }
}