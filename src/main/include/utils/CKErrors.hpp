#pragma once

#include <hal/DriverStation.h>
#include <string>

namespace ck
{
    bool ReportError(std::string msg, bool printCallStack = false);
    bool ReportWarning(std::string msg, bool printCallStack = false);
}