#pragma once
#include "utils/Singleton.hpp"
#include <mutex>
#include "MotorControl.pb.h"
#include "MotorConfiguration.pb.h"

class FailoverMessageManager : public Singleton<FailoverMessageManager>
{
    friend Singleton;
public:
    ck::MotorControl::Motor* addMotorControl();
    ck::MotorConfiguration::Motor* addMotorConfig();
    void publishMessages();

private:
    std::recursive_mutex mLock;
    ck::MotorConfiguration mMotorConfiguration;
    ck::MotorControl mMotorControl;

    static constexpr int BUF_SIZE = 16384;
    void* mBuff = nullptr;

    FailoverMessageManager();
    ~FailoverMessageManager();
};