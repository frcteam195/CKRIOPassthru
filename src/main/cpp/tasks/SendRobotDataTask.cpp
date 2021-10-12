#include "tasks/SendRobotDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include "utils/RobotControlModeHelper.hpp"
#include "NetworkManager.hpp"
#include <vector>
#include <iostream>

SendRobotDataTask::SendRobotDataTask() : Task(THREAD_RATE_MS, TASK_NAME), mRobotStatus()
{
    mRobotStatusBuf = malloc(ROBOT_STATUS_MESSAGE_SIZE * sizeof(uint8_t));
    memset(mRobotStatusBuf, 0, ROBOT_STATUS_MESSAGE_SIZE * sizeof(uint8_t));
}

SendRobotDataTask::~SendRobotDataTask()
{
    free(mRobotStatusBuf);
}


void SendRobotDataTask::run(uint32_t timeSinceLastUpdateMs)
{
    sendRobotStatusMessage();
    mTaskTimer.reportElapsedTime();
}

void SendRobotDataTask::sendRobotStatusMessage()
{
    mRobotStatus.Clear();
    mRobotStatus.set_robot_state((ck::RobotStatus_RobotState) RobotControlModeHelper::getInstance().getControlMode());
    mRobotStatus.set_alliance(ds->GetAlliance() == frc::DriverStation::Alliance::kRed ? ck::RobotStatus_Alliance::RobotStatus_Alliance_RED : ck::RobotStatus_Alliance::RobotStatus_Alliance_BLUE);
    mRobotStatus.set_match_time(ds->GetMatchTime());
    mRobotStatus.set_game_data(ds->GetGameSpecificMessage());
    if (mRobotStatus.SerializeToArray(mRobotStatusBuf, mRobotStatus.ByteSizeLong()))
    {
        NetworkManager::getInstance().sendMessage(ROBOT_STATUS_MESSAGE_GROUP, mRobotStatusBuf, mRobotStatus.ByteSizeLong());
    }
    else
    {
        std::cout << "Robot status message failed to serialize. Message probably too large or invalid." << std::endl;
    }
}