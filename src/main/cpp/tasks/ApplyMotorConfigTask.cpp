#include "tasks/ApplyMotorConfigTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <map>
#include <vector>
#include "DataStructures.hpp"
#include "MotorManager.hpp"
#include "ctre/Phoenix.h"
#include "NetworkManager.hpp"

ApplyMotorConfigTask::ApplyMotorConfigTask() : Task(THREAD_RATE_MS) {}

void ApplyMotorConfigTask::run(uint32_t timeSinceLastUpdateMs)
{
    //TODO: Improve memory efficiency
    std::vector<uint8_t> buf;
    NetworkManager::getInstance().getMessage(MOTOR_CONFIG_MESSAGE_GROUP, buf);

    ck::MotorConfiguration motorsUpdate;
    motorsUpdate.ParseFromArray(&buf[0], buf.size());

    if (!google::protobuf::util::MessageDifferencer::Equivalent(motorsUpdate, mPrevMotorsMsg))
    {
        for (ck::MotorConfiguration_Motor const& m : motorsUpdate.motors())
        {
            ck::MotorConfiguration_Motor_MotorControllerConfiguration config = m.motor_configuration();
            MotorManager::getInstance().forEach([&] (uint16_t id, BaseMotorController* mCtrl, MotorType mType)
            {
                int kTimeoutMs = 10;
                mCtrl->Config_kP(0, config.kp(), kTimeoutMs);
            });
        }
    }

    mPrevMotorsMsg = motorsUpdate;


    // std::map<uint16_t, TalonFX *> *motorList = &DataManager::getInstance().motorObjectList;
    // for (MotorConfigData &m : DataManager::getInstance().motorConfigData)
    // {
    //     std::map<uint16_t, TalonFX *>::iterator it = motorList->find(m.motorId);
    //     TalonFX *currMotor;
    //     if (it != motorList->end())
    //     {
    //         //Motor Exists
    //         currMotor = it->second;
    //         //TODO: Do differential compare and Set parameters here if changed
    //         //TODO: Make check/resend function for timeout error
    //         currMotor->Config_kP(0, m.kP, 10);
    //     }
    // }
}