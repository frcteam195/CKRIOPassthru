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

class SendRobotDataTask : public Task
{
public:
    inline const static std::string ROBOT_STATUS_MESSAGE_GROUP = "robotstatus";
    inline const static std::string TASK_NAME = "SendRobotDataTask";

    SendRobotDataTask();
    ~SendRobotDataTask();
    RTTimer rtTimer;
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 10;
private:
    ck::RobotStatus mRobotStatus;
    const size_t ROBOT_STATUS_MESSAGE_SIZE = 1500;
    void* mRobotStatusBuf;
    void sendRobotStatusMessage();
};