#include "tasks/ApplyIMUConfigTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <map>
#include <vector>
#include "utils/PhoenixHelper.hpp"
#include "CKIMUManager.hpp"
#include "utils/drivers/CKPigeon2.hpp"
#include "NetworkManager.hpp"
#include <iostream>

ApplyIMUConfigTask::ApplyIMUConfigTask() : Task(THREAD_RATE_MS, TASK_NAME)
{
    NetworkManager::getInstance().joinGroup(IMU_CONFIG_MESSAGE_GROUP.c_str());
}

void ApplyIMUConfigTask::run(uint32_t timeSinceLastUpdateMs)
{
    mTaskTimer.start();
    //TODO: Improve memory efficiency
    std::vector<uint8_t> buf;
    if (NetworkManager::getInstance().getMessage(IMU_CONFIG_MESSAGE_GROUP, buf))
    {
        ck::IMUConfig imuUpdate;
        if (imuUpdate.ParseFromArray(&buf[0], buf.size()))
        {
            // std::cout << "Parsed IMU Config" << std::endl;

            for (auto imu_config_msg : imuUpdate.imu_config())
            {
                bool success = true;
                CKIMUManager::getInstance().registerIMU(imu_config_msg.id(), (IMUType)imu_config_msg.imu_type(), (CANInterface)imu_config_msg.can_network());
                if (!google::protobuf::util::MessageDifferencer::Equivalent(imu_config_msg, mPrevIMUMsg[imu_config_msg.id()]))
                {
                    std::cout << "IMU Mount Pose Config" << std::endl;
                    CKIMUManager::getInstance().onIMU(imu_config_msg.id(), [&](uint16_t id, CKIMU* imu, IMUType imu_type)
                    {
                        success &= imu->configMountPose((AxisDirection)imu_config_msg.mount_pose_axis_forward(), (AxisDirection)imu_config_msg.mount_pose_axis_up());
                        // success &= imu->reset();
                        switch (imu_type)
                        {
                            case IMUType::PIGEON2:
                            {
                                // CKPigeon2* ckpigeon2 = dynamic_cast<CKPigeon2*>(imu);
                                // Pigeon2 pigeon2 = ckpigeon2->getRawPigeon2();
                                break;
                            }
                            case IMUType::NAVX:
                            {
                                break;
                            }
                        }
                    });
                }

                if (success)
                {
                    mPrevIMUMsg[imu_config_msg.id()] = imu_config_msg;
                }
            }
        }
    }

    mTaskTimer.reportElapsedTime();
}