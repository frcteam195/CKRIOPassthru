#pragma once

#include "Task.hpp"
#include "frc/DriverStation.h"
#include <vector>
#include "frc/Joystick.h"
#include "IntellisenseDefs.h"
#include "RobotJoysticks.pb.h"

#define MAX_NUM_JOYSTICKS 6

class SendJoystickDataTask : public Task
{
public:
    inline const static std::string JOYSTICK_MESSAGE_GROUP = "joystickstatus";
    inline const static std::string TASK_NAME = "SendJoystickDataTask";

    SendJoystickDataTask();
    ~SendJoystickDataTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 20;
private:
    const frc::DriverStation* ds = &frc::DriverStation::GetInstance();
    bool mJoystickConnected[MAX_NUM_JOYSTICKS] = {false};
    std::vector<int> mConnectedJoysticks;
    ck::RobotJoystickStatus robotJoystickStatusMsg;
    const size_t JOYSTICK_STATUS_MESSAGE_SIZE = 1500;
    void* mJoystickStatusBuf;
};