#include "tasks/SendMotorValuesTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include "DataManager.hpp"

SendMotorValuesTask::SendMotorValuesTask() : Task(THREAD_RATE_MS) {}
SendMotorValuesTask::~SendMotorValuesTask()
{
    // for (std::pair<uint16_t, TalonFX *> p : DataManager::getInstance().motorObjectList)
    // {
    //     delete p.second;
    // }
}

void SendMotorValuesTask::run(uint32_t timeSinceLastUpdateMs)
{
    // std::map<uint16_t, TalonFX *> *motorList = &DataManager::getInstance().motorObjectList;
    // for (MotorData &m : DataManager::getInstance().motorData)
    // {
    //     std::map<uint16_t, TalonFX *>::iterator it = motorList->find(m.motorId);
    //     TalonFX *currMotor;
    //     if (it != motorList->end())
    //     {
    //         //Motor Exists
    //         currMotor = it->second;
    //     }
    //     else
    //     {
    //         currMotor = new TalonFX(m.motorId);
    //         motorList->emplace(m.motorId, currMotor);
    //     }
    //     currMotor->Set(m.ctrlMode, m.outputVal, DemandType::DemandType_ArbitraryFeedForward, m.arbFF);
    // }
}