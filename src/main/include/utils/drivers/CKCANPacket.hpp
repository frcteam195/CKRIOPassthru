#pragma once
#include <cstdint>
#include <vector>
#include "CKCANDevice.hpp"

struct CKCANPacket
{
    CKCANDevice* canDev;
    int apiID;
    std::vector<uint8_t> data;
};