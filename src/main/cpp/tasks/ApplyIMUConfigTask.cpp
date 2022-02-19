#include "tasks/ApplyIMUConfigTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <map>
#include <vector>
#include "utils/PhoenixHelper.hpp"
#include "CKIMUManager.hpp"
#include "NetworkManager.hpp"
#include <iostream>

ApplyIMUConfigTask::ApplyIMUConfigTask() : Task(THREAD_RATE_MS, TASK_NAME)
{
    NetworkManager::getInstance().joinGroup(IMU_CONFIG_MESSAGE_GROUP.c_str());
}

void ApplyIMUConfigTask::run(uint32_t timeSinceLastUpdateMs)
{
    //TODO: Improve memory efficiency
    std::vector<uint8_t> buf;
    if (NetworkManager::getInstance().getMessage(IMU_CONFIG_MESSAGE_GROUP, buf))
    {
        ck::IMUConfig imuUpdate;
        imuUpdate.ParseFromArray(&buf[0], buf.size());

        for (auto imu_config_msg : imuUpdate.imu_config())
        {
            CKIMUManager::getInstance().registerIMU(imu_config_msg.id(), (IMUType)imu_config_msg.imu_type());
            if (mPrevIMUMsg.count(imu_config_msg.id()))
            {
                if (!google::protobuf::util::MessageDifferencer::Equivalent(imu_config_msg, mPrevIMUMsg[imu_config_msg.id()]))
                {
                    CKIMUManager::getInstance().onIMU(imu_config_msg.id(), [&](uint16_t id, CKIMU* imu, IMUType imu_type)
                    {
                        imu->configMountPose((AxisDirection)imu_config_msg.mount_pose_axis_forward(), (AxisDirection)imu_config_msg.mount_pose_axis_up());
                    });
                }
            }
            mPrevIMUMsg[imu_config_msg.id()] = imu_config_msg;
        }
    }

    mTaskTimer.reportElapsedTime();
}