#pragma once
#include <functional>
#include <pthread.h>
#include <iostream>
#include "sched.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

namespace ck
{
    bool configureSystemPriority();
    bool configureThreadPriority(int priority);
    std::string s_exec(const char* cmd);
}