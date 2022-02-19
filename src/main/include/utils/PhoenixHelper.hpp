#pragma once

#include "ctre/Phoenix.h"
#include <functional>
#include <iostream>

namespace ck
{
    static constexpr int kPhoenixRetryCount = 3;
    static constexpr int kCANTimeoutMs = 30;

    bool runPhoenixFunctionWithRetry(std::function<ErrorCode()> func, int id = -1);
}
