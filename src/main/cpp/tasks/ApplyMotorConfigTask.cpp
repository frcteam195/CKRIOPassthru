#include "tasks/ApplyMotorConfigTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <map>
#include <vector>
#include "DataStructures.hpp"
#include "MotorManager.hpp"
#include "ctre/Phoenix.h"
#include "NetworkManager.hpp"
#include "utils/PhoenixHelper.hpp"

ApplyMotorConfigTask::ApplyMotorConfigTask() : Task(THREAD_RATE_MS)
{
    NetworkManager::getInstance().joinGroup(MOTOR_CONFIG_MESSAGE_GROUP.c_str());
}

void ApplyMotorConfigTask::run(uint32_t timeSinceLastUpdateMs)
{
    //TODO: Improve memory efficiency
    std::vector<uint8_t> buf;
    if (NetworkManager::getInstance().getMessage(MOTOR_CONFIG_MESSAGE_GROUP, buf))
    {
        ck::MotorConfiguration motorsUpdate;
        motorsUpdate.ParseFromArray(&buf[0], buf.size());

        //https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.util.message_differencer#MessageDifferencer.Reporter
        //Maybe look into reportdifferencesto function
        if (!google::protobuf::util::MessageDifferencer::Equivalent(motorsUpdate, mPrevMotorsMsg))
        {
            for (ck::MotorConfiguration_Motor const& m : motorsUpdate.motors())
            {
                MotorManager::getInstance().registerMotor(m.id(), (MotorType)m.controller_type());
                //TODO: Implement per command differential set only if value is changed
                ck::MotorConfiguration_Motor_MotorControllerConfiguration config = m.motor_configuration();
                MotorManager::getInstance().onMotor(m.id(), [&] (uint16_t id, BaseMotorController* mCtrl, MotorType mType)
                {
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kP(0, config.kp(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kI(0, config.ki(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kD(0, config.kd(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kF(0, config.kf(), ck::kCANTimeoutMs); }, id);
                    
                });
            }
        }

        mPrevMotorsMsg = motorsUpdate;
    }
}