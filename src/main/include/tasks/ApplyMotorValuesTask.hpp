#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "Task.hpp"
#include <stdint.h>
#include "IntellisenseDefs.h"
#include "MotorControl.pb.h"
#include "google/protobuf/util/message_differencer.h"
#include "utils/RTTimer.hpp"
#include "utils/RobotControlModeHelper.hpp"

class ApplyMotorValuesTask : public Task
{
public:
    inline const static std::string MOTOR_CONTROL_MESSAGE_GROUP = "motorcontrol";
    inline const static std::string TASK_NAME = "ApplyMotorValuesTask";

    ApplyMotorValuesTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 5;
private:
    ck::MotorControl mPrevMotorsMsg;
    RTTimer rtTimer;

    static constexpr unsigned int kMandatoryUpdatePerioduS = 200000;
    static constexpr bool MANDATORY_UPDATE_ENABLED = true;
};