#pragma once
#include <cstdint> 
#include "frc/CAN.h"

class CKCANDevice
{
public:
    CKCANDevice(int deviceID, int deviceType, int manufacturerType)
    : mDeviceID(deviceID), mDeviceType(deviceType), mManufacturerType(manufacturerType),
      mCANDev(deviceID & 0x3F, manufacturerType, deviceType)
    {

    };

    virtual uint32_t getDeviceIDBase()
    {
        return ((mDeviceType << 24) & 0x1F000000)
            | ((mManufacturerType << 16) & 0x00FF0000)
            | (mDeviceID & 0x0000003F);
    };

    virtual int getAPIID(int apiClass, int apiIndex)
    {
        return ((((int)apiClass) << 6) & 0x3F0) | (((int)apiIndex) & 0x00F);
    };

    virtual uint32_t getDeviceID(int apiID)
    {
        return getDeviceIDBase() | ((apiID << 6) & 0x000003C0);
    }

    frc::CAN& getCANDev()
    {
        return mCANDev;
    };

private:
    int mDeviceID;
    int mDeviceType;
    int mManufacturerType;

    frc::CAN mCANDev;
};