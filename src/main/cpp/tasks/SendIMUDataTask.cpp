#include "tasks/SendIMUDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include "NetworkManager.hpp"
#include <vector>
#include <iostream>

SendIMUDataTask::SendIMUDataTask() : Task(THREAD_RATE_MS, TASK_NAME), mIMUData()
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

void SendIMUDataTask::doSendIMUUpdate(double yaw, double pitch, double roll)
{
    mIMUData.Clear();
    ck::IMUData_IMUSensorData *imuSensorData = mIMUData.add_imu_sensor();
    imuSensorData->set_yaw(yaw);
    imuSensorData->set_pitch(pitch);
    imuSensorData->set_roll(roll);
    if (mIMUData.SerializeToArray(mIMUDataBuf, mIMUData.ByteSizeLong()))
    {
        NetworkManager::getInstance().sendMessage(IMU_DATA_MESSAGE_GROUP, mIMUDataBuf, mIMUData.ByteSizeLong());
    }
    else
    {
        std::cout << "IMU data message failed to serialize. Message probably too large or invalid." << std::endl;
    }
}

void SendIMUDataTask::sendIMUDataMessage()
{
    if (mNavX.isPresent())
    {
        if (mNavX.hasUpdated())
        {
            doSendIMUUpdate(mNavX.getFusedHeading(), mNavX.getPitch(), mNavX.getRoll());
        }
    }
    else 
    {
        doSendIMUUpdate(0, 0, 0);
    }
}