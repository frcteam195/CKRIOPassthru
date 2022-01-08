#include "tasks/SendIMUDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include "NetworkManager.hpp"
#include <vector>
#include <iostream>
#include "google/protobuf/util/json_util.h"
#include "utils/GlobalConfig.hpp"
#include <string>
#include "NavXManager.hpp"

SendIMUDataTask::SendIMUDataTask() : Task(THREAD_RATE_MS, TASK_NAME), mIMUData(), mNavX(NavXManager::getInstance().getNavX())
{
    mIMUDataBuf = malloc(IMU_DATA_MESSAGE_SIZE * sizeof(uint8_t));
    memset(mIMUDataBuf, 0, IMU_DATA_MESSAGE_SIZE * sizeof(uint8_t));
}

SendIMUDataTask::~SendIMUDataTask()
{
    free(mIMUDataBuf);
}


void SendIMUDataTask::run(uint32_t timeSinceLastUpdateMs)
{
    sendIMUDataMessage();
    mTaskTimer.reportElapsedTime();
}

void SendIMUDataTask::doSendIMUUpdate(double yaw, double pitch, double roll, double yawrate)
{
#ifdef CONSOLE_REPORTING
    static int count = 0;
#endif

    mIMUData.Clear();
    ck::IMUData_IMUSensorData *imuSensorData = mIMUData.add_imu_sensor();
    imuSensorData->set_yaw(yaw);
    imuSensorData->set_pitch(pitch);
    imuSensorData->set_roll(roll);
    imuSensorData->set_yawrate(yawrate);
    if (mIMUData.SerializeToArray(mIMUDataBuf, mIMUData.ByteSizeLong()))
    {
        NetworkManager::getInstance().sendMessage(IMU_DATA_MESSAGE_GROUP, mIMUDataBuf, mIMUData.ByteSizeLong());
    }
    else
    {
        std::cout << "IMU data message failed to serialize. Message probably too large or invalid." << std::endl;
    }

#ifdef CONSOLE_REPORTING
    if (count++ % 500 == 0)
    {
        std::string s;
        google::protobuf::util::MessageToJsonString(mIMUData, &s);
        std::cout << s << std::endl;
    }
#endif
}

void SendIMUDataTask::sendIMUDataMessage()
{
    if (mNavX.isPresent())
    {
        if (mNavX.hasUpdated())
        {
            doSendIMUUpdate(mNavX.getYaw(), mNavX.getPitch(), mNavX.getRoll(), mNavX.getYawRateRadPerSec());
        }
    }
    else 
    {
        doSendIMUUpdate(0, 0, 0, 0);
    }
}