#pragma once
#include "utils/Singleton.hpp"
#include <mutex>
#include "MotorControl.pb.h"
#include "MotorConfiguration.pb.h"
#include "SolenoidControl.pb.h"
#include "LEDControl.pb.h"
#include "IMUConfig.pb.h"

class FailoverMessageManager : public Singleton<FailoverMessageManager>
{
    friend Singleton;
public:
    ck::MotorControl::Motor* addMotorControl();
    ck::MotorConfiguration::Motor* addMotorConfig();
    ck::SolenoidControl::Solenoid* addSolenoidControl();
    ck::IMUConfig::IMUConfigData* addIMUConfig();
    ck::LEDControl::LEDControlData* addLEDControl();
    void publishMessages();

private:
    std::recursive_mutex mLock;
    ck::MotorConfiguration mMotorConfiguration;
    ck::MotorControl mMotorControl;
    ck::SolenoidControl mSolenoidControl;
    ck::IMUConfig mIMUConfig;
    ck::LEDControl mLEDControl;

    static constexpr int BUF_SIZE = 16384;
    void* mBuff = nullptr;

    FailoverMessageManager();
    ~FailoverMessageManager();
};