#pragma once

#include <string>
#include <vector>
#include "Task.hpp"
#include "IntellisenseDefs.h"
#include "SolenoidStatus.pb.h"

class SendSolenoidDataTask : public Task
{
public:
    inline const static std::string SOLENOID_STATUS_MESSAGE_GROUP = "solenoidstatus";
    inline const static std::string TASK_NAME = "SendSolenoidDataTask";

    SendSolenoidDataTask();
    ~SendSolenoidDataTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 100;
private:
    ck::SolenoidStatus mSolenoidStatusMsg;
    const size_t SOLENOID_STATUS_MESSAGE_SIZE = 1500;
    void* mSolenoidStatusBuf;
};