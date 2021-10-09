#include "tasks/ApplyMotorConfigTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <map>
#include <vector>
#include "DataStructures.hpp"
#include "DataManager.hpp"
#include "ctre/Phoenix.h"

ApplyMotorConfigTask::ApplyMotorConfigTask() : Task(THREAD_RATE_MS) {}

void ApplyMotorConfigTask::run(uint32_t timeSinceLastUpdateMs)
{
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