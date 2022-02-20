#pragma once

#include "utils/Singleton.hpp"
#include "utils/drivers/CKLEDController.hpp"
#include "utils/drivers/RGBColor.hpp"
#include <atomic>
#include <mutex>

#define CK_LED_CAN_DEVICE_ID 1

class LEDManager : public Singleton<LEDManager>
{
    friend Singleton;
public:
    enum class LEDMode : int
    {
        OFF,
        FIXED_ON,
        BLINK,
        MORSE,
        COMM_LOSS,
        COMM_RESTORED,
        FADE,
        NO_CHANGE
    };

    void sendUpdate();
    void setRequestedState(LEDMode requestedState);
    void setColor(RGBColor rgbColor);
private:

    CKLEDController mLEDController;
    std::atomic<LEDMode> mRequestedState;
    RGBColor mRGBColor;

    std::recursive_mutex mLEDLock;

    bool mPrevDSAttachedState = false;

    static constexpr LEDMode DEFAULT_LED_MODE { LEDMode::FADE };
    static constexpr RGBColor DEFAULT_LED_COLOR { 0, 128, 0, 255 };
    
    LEDManager();
    ~LEDManager();
};