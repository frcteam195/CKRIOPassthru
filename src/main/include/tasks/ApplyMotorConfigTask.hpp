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

    ApplyMotorConfigTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 5;
private:
    ck::MotorConfiguration mPrevMotorsMsg;
};