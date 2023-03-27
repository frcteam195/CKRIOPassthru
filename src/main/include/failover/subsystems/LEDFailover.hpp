#pragma once
#include "utils/Singleton.hpp"
#include "failover/utils/FailoverSubsystem.hpp"
#include "CANdleManager.hpp"

#include "LEDControl.pb.h"
#include <cstdint>

class LEDFailover : public Singleton<LEDFailover>, public FailoverSubsystem
{
    friend Singleton;
protected:
    void init() override;
    void periodic() override;
    void uninit() override;
    void run() override;

private:
    LEDFailover();
    ~LEDFailover();

    static constexpr int NUM_LEDS = 50;

    ck::LEDControl::LEDControlData* mLED = nullptr;
};