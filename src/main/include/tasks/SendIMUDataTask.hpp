#pragma once

#include <vector>
#include "Task.hpp"
#include <stdint.h>
#include "utils/RTTimer.hpp"
#include "IntellisenseDefs.h"
#include "IMUData.pb.h"
#include <string>
#include "utils/drivers/NavX.hpp"

class SendIMUDataTask : public Task
{
public:
    inline const static std::string IMU_DATA_MESSAGE_GROUP = "imudata";
    inline const static std::string TASK_NAME = "SendIMUDataTask";

    SendIMUDataTask();
    ~SendIMUDataTask();
    RTTimer rtTimer;
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 2;
private:
    ck::IMUData mIMUData;
    const size_t IMU_DATA_MESSAGE_SIZE = 1500;
    void* mIMUDataBuf;
    void sendIMUDataMessage();
    void doSendIMUUpdate(double yaw, double pitch, double roll);
    NavX mNavX;
};