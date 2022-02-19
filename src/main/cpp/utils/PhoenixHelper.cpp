#include "utils/PhoenixHelper.hpp"

namespace ck
{
    bool runPhoenixFunctionWithRetry(std::function<ErrorCode()> func, int id)
    {
        ErrorCode eCode;
		int retryCounter = 0;
		do {
			eCode = func();
		} while(eCode != ErrorCode::OK && retryCounter++ < kPhoenixRetryCount);

		if (retryCounter >= kPhoenixRetryCount || eCode != ErrorCode::OK)
        {
            if (id >= 0)
            {
			    std::cout << "Failed to set parameter on Talon ID " << id << " !!!!!!" << std::endl;
            }
            return false;
        }

		return true;
    }
}