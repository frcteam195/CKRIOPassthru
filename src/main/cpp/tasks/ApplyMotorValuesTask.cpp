#include "tasks/ApplyMotorValuesTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <map>
#include <iostream>
#include <vector>
#include "DataStructures.hpp"
#include "MotorManager.hpp"
#include "ctre/Phoenix.h"
#include "NetworkManager.hpp"
#include "utils/PhoenixHelper.hpp"

ApplyMotorValuesTask::ApplyMotorValuesTask() : Task(THREAD_RATE_MS, TASK_NAME)
{
    NetworkManager::getInstance().joinGroup(MOTOR_CONTROL_MESSAGE_GROUP.c_str());
}

void ApplyMotorValuesTask::run(uint32_t timeSinceLastUpdateMs)
{
    //TODO: Improve memory efficiency
    std::vector<uint8_t> buf;
    if (NetworkManager::getInstance().getMessage(MOTOR_CONTROL_MESSAGE_GROUP, buf))
    {
        ck::MotorControl motorsUpdate;
        motorsUpdate.ParseFromArray(&buf[0], buf.size());

        //https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.util.message_differencer#MessageDifferencer.Reporter
        //Maybe look into reportdifferencesto function
        if (!google::protobuf::util::MessageDifferencer::Equivalent(motorsUpdate, mPrevMotorsMsg) || (rtTimer.hasElapseduS() > kMandatoryUpdatePerioduS && MANDATORY_UPDATE_ENABLED))
        {
            for (ck::MotorControl_Motor const& m : motorsUpdate.motors())
            {
                MotorManager::getInstance().registerMotor(m.id(), (MotorType)m.controller_type());
                MotorManager::getInstance().onMotor(m.id(), [&] (uint16_t id, BaseMotorController* mCtrl, MotorType mType)
                {
                    //TODO: Implement per motor differential set.
                    if (m.control_mode() != ck::MotorControl_Motor_ControlMode::MotorControl_Motor_ControlMode_Follower)
                    {
                        mCtrl->Set((ControlMode)m.control_mode(), m.output_value(), DemandType::DemandType_ArbitraryFeedForward, m.arbitrary_feedforward());
                    }
                    else
                    {
                        BaseMotorController* motorMaster = MotorManager::getInstance().getMotor_unsafe(m.output_value());
                        if (motorMaster)
                        {
                            mCtrl->Follow(*motorMaster);
                        }
                    }
                });
            }
            rtTimer.start();
        }

        mPrevMotorsMsg = motorsUpdate;
    }
    mTaskTimer.reportElapsedTime();
}