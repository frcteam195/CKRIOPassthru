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
			    std::cout << "Failed to set parameter on device ID " << id << " !!!!!!" << std::endl;
            }
            return false;
        }

		return true;
    }

    std::string getCANInterfaceName(CANInterface canInterface)
    {
        std::string canString = "rio";
        switch (canInterface)
        {
            case CANInterface::RIO_INTERNAL_CAN:
            {
                canString = "rio";
                break;
            }
            case CANInterface::RIO_CANIVORE:
            {
                canString = "canivore1";
                break;
            }
            case CANInterface::COPROCESSOR_CAN:
            {
                canString = "rio";
                break;
            }
        }
        return canString;
    }
}