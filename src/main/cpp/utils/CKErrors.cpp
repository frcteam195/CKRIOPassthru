#include "utils/CKErrors.hpp"
#include <frc/Errors.h>
#include <hal/Errors.h>

namespace ck
{

    bool ReportError(std::string msg, bool printCallStack)
    {
        if (printCallStack)
        {
            FRC_ReportError(-111, "{}", msg);
        }
        else
        {
            return HAL_SendError(1, -111, 0, msg.c_str(), "", "", 1) == HAL_SUCCESS;
        }
        return true;
    }

    bool ReportWarning(std::string msg, bool printCallStack)
    {
        if (printCallStack)
        {
            FRC_ReportError(16, "{}", msg);
        }
        else
        {
            return HAL_SendError(0, 16, 0, msg.c_str(), "", "", 1) == HAL_SUCCESS;
        }
        return true;
    }

}