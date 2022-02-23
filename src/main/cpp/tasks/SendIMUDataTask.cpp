#include "tasks/SendIMUDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include "NetworkManager.hpp"
#include <vector>
#include <iostream>
#include "google/protobuf/util/json_util.h"
#include "utils/GlobalConfig.hpp"
#include <string>
#include "CKIMUManager.hpp"

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
    mTaskTimer.start();
#ifdef CONSOLE_REPORTING
    static int count = 0;
#endif

    mIMUData.Clear();

    CKIMUManager::getInstance().forEach([&](uint16_t id, CKIMU* imu, IMUType imuType)
    {
        ck::IMUData_IMUSensorData *imuSensorData = mIMUData.add_imu_sensor();
        imuSensorData->set_id(id);
        double quaternion[4];
        if (imu->getQuaternion(quaternion))
        {
            imuSensorData->set_w(quaternion[0]);
            imuSensorData->set_x(quaternion[1]);
            imuSensorData->set_y(quaternion[2]);
            imuSensorData->set_z(quaternion[3]);
        }
        else
        {
            imuSensorData->set_w(0);
            imuSensorData->set_x(0);
            imuSensorData->set_y(0);
            imuSensorData->set_z(0);
        }
    });

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
    mTaskTimer.reportElapsedTime();
}