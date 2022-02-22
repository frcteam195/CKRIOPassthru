#pragma once

#include "ctre/Phoenix.h"
#include <functional>
#include <iostream>
#include "utils/drivers/CANInterface.hpp"

namespace ck
{
    static constexpr int kPhoenixRetryCount = 3;
    static constexpr int kCANTimeoutMs = 30;

    bool runPhoenixFunctionWithRetry(std::function<ErrorCode()> func, int id = -1);
    std::string getCANInterfaceName(CANInterface canInterface);
}
