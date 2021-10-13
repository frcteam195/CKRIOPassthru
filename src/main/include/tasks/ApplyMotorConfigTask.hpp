#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "Task.hpp"
#include <stdint.h>
#include "IntellisenseDefs.h"
#include "MotorConfiguration.pb.h"
#include "google/protobuf/util/message_differencer.h"

class ApplyMotorConfigTask : public Task
{
public:
    inline const static std::string MOTOR_CONFIG_MESSAGE_GROUP = "motorconfig";
    inline const static std::string TASK_NAME = "ApplyMotorConfigTask";

    ApplyMotorConfigTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 100;
private:
    ck::MotorConfiguration mPrevMotorsMsg;

    struct StatusFrameConfig
    {
        int CONTROL_FRAME_PERIOD_MS;
		int STATUS_FRAME_GENERAL_1_MS;
		int STATUS_FRAME_FEEDBACK0_2_MS;
    };
    static constexpr StatusFrameConfig FAST_MASTER_CONFIG = {5, 5, 20};
    static constexpr StatusFrameConfig NORMAL_MASTER_CONFIG = {10, 10, 20};
    static constexpr StatusFrameConfig SLAVE_CONFIG = {10, 100, 100};
};