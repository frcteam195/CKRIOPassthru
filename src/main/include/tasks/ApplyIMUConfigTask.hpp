#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "Task.hpp"
#include <stdint.h>
#include <map>
#include "IntellisenseDefs.h"
#include "IMUConfig.pb.h"
#include "google/protobuf/util/message_differencer.h"

class ApplyIMUConfigTask : public Task
{
public:
    inline const static std::string IMU_CONFIG_MESSAGE_GROUP = "imuconfig";
    inline const static std::string TASK_NAME = "ApplyIMUConfigTask";

    ApplyIMUConfigTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 100;
private:
    std::map<uint16_t, ck::IMUConfig::IMUConfigData> mPrevIMUMsg;
};