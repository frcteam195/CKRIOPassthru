#pragma once

#include <vector>
#include "Task.hpp"
#include "IntellisenseDefs.h"
#include "MotorStatus.pb.h"

class SendSensorDataTask : public Task
{
public:
    inline const static std::string MOTOR_STATUS_MESSAGE_GROUP = "motorstatus";
    inline const static std::string TASK_NAME = "SendSensorDataTask";

    SendSensorDataTask();
    ~SendSensorDataTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 10;
private:
    ck::MotorStatus mMotorStatusMsg;
    const size_t MOTOR_STATUS_MESSAGE_SIZE = 1500;
    void* mMotorStatusBuf;
};