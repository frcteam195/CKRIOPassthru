#include "tasks/SendJoystickDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <hal/DriverStation.h>
#include <hal/DriverStationTypes.h>
#include "NetworkManager.hpp"

SendJoystickDataTask::SendJoystickDataTask() : Task(THREAD_RATE_MS, TASK_NAME)
{
    mJoystickStatusBuf = malloc(JOYSTICK_STATUS_MESSAGE_SIZE * sizeof(uint8_t));
    memset(mJoystickStatusBuf, 0, JOYSTICK_STATUS_MESSAGE_SIZE * sizeof(uint8_t));
}
SendJoystickDataTask::~SendJoystickDataTask()
{
    free(mJoystickStatusBuf);
}


void SendJoystickDataTask::run(uint32_t timeSinceLastUpdateMs)
{  
    robotJoystickStatusMsg.Clear();
    for (int i = 0; i < MAX_NUM_JOYSTICKS; i++) {
        if(ds->IsJoystickConnected(i)) {
            ck::RobotJoystickStatus_Joystick* protoJoystick = robotJoystickStatusMsg.add_joysticks();

            HAL_JoystickAxes rawAxes;
            HAL_GetJoystickAxes(i, &rawAxes);
            for (int j = 0; j < rawAxes.count; j++) {
                protoJoystick->add_axes(rawAxes.axes[j]);
            }

            HAL_JoystickButtons rawButtons;
            HAL_GetJoystickButtons(i, &rawButtons);
            for (int j = 0; j < rawButtons.count; j++) {
                protoJoystick->set_buttons(rawButtons.buttons);
            }

            HAL_JoystickPOVs rawPovs;
            HAL_GetJoystickPOVs(i, &rawPovs);
            for (int j = 0; j < rawPovs.count; j++) {
                protoJoystick->add_povs(rawPovs.povs[j]);
            }
        }
    }

    if (robotJoystickStatusMsg.SerializeToArray(mJoystickStatusBuf, robotJoystickStatusMsg.ByteSizeLong()))
    {
        NetworkManager::getInstance().sendMessage(JOYSTICK_MESSAGE_GROUP, mJoystickStatusBuf, robotJoystickStatusMsg.ByteSizeLong());
    }
    else
    {
        std::cout << "Joystick status message failed to serialize. Message probably too large or invalid." << std::endl;
    }
    mTaskTimer.reportElapsedTime();
}