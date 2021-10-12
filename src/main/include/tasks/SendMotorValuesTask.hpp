#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "Task.hpp"
#include <stdint.h>
#include "IntellisenseDefs.h"
#include "MotorControl.pb.h"
#include "google/protobuf/util/message_differencer.h"

class SendMotorValuesTask : public Task
{
public:
    inline const static std::string MOTOR_CONTROL_MESSAGE_GROUP = "motorcontrol";


    SendMotorValuesTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 5;
private:
    ck::MotorControl mPrevMotorsMsg;
};