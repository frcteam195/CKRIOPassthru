#include "tasks/SendSolenoidDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include "SolenoidManager.hpp"
#include "NetworkManager.hpp"

SendSolenoidDataTask::SendSolenoidDataTask() : Task(THREAD_RATE_MS, TASK_NAME), mSolenoidStatusMsg()
{
    mSolenoidStatusBuf = malloc(SOLENOID_STATUS_MESSAGE_SIZE * sizeof(uint8_t));
    memset(mSolenoidStatusBuf, 0, SOLENOID_STATUS_MESSAGE_SIZE * sizeof(uint8_t));
}
SendSolenoidDataTask::~SendSolenoidDataTask()
{
    free(mSolenoidStatusBuf);
}

void SendSolenoidDataTask::run(uint32_t timeSinceLastUpdateMs)
{
    mTaskTimer.start();
    mSolenoidStatusMsg.Clear();

    SolenoidManager::getInstance().forEach([&] (uint16_t id, CKSolenoid* sCtrl, ck::SolenoidControl::Solenoid::SolenoidType sType)
    {
        ck::SolenoidStatus::Solenoid* s = mSolenoidStatusMsg.add_solenoids();
        s->set_id(id);
        s->set_solenoid_value((ck::SolenoidStatus::Solenoid::SolenoidValue)sCtrl->getValue());
    });

    if (mSolenoidStatusMsg.SerializeToArray(mSolenoidStatusBuf, mSolenoidStatusMsg.ByteSizeLong()))
    {
        NetworkManager::getInstance().sendMessage(SOLENOID_STATUS_MESSAGE_GROUP, mSolenoidStatusBuf, mSolenoidStatusMsg.ByteSizeLong());
    }
    else
    {
        std::cout << "Solenoid status message failed to serialize. Message probably too large or invalid." << std::endl;
    }
    mTaskTimer.reportElapsedTime();
}