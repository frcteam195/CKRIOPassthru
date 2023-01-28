#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "Task.hpp"
#include <stdint.h>
#include "utils/RTTimer.hpp"
#include "IntellisenseDefs.h"
#include "RobotStatus.pb.h"
#include <string>
#include "frc/DriverStation.h"

class UpdateDSConnectionStatusTask : public Task
{
public:
    inline const static std::string TASK_NAME = "UpdateDSConnectionStatusTask";

    UpdateDSConnectionStatusTask();
    ~UpdateDSConnectionStatusTask();
    RTTimer rtTimer;
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 1000;
};