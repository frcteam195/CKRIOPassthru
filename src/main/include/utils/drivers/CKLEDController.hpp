#pragma once

#include "frc/CAN.h"
#include <string>
#include "RGBColor.hpp"
#include "utils/drivers/CKCANDevice.hpp"

#define CK_LED_CAN_DEVICE_TYPE HAL_CAN_Dev_kIOBreakout
#define CK_LED_CAN_MANUFACTURER_TYPE HAL_CAN_Man_kTeamUse

class CKLEDController : public CKCANDevice
{
    enum class APIClass
    {
        IDLE = 0,
        OFF = 1,
        FIXED_ON = 2,
        BLINK = 3,
        COMM_LOSS = 4,
        COMM_RESTORED = 5,
        MORSE = 6,
        DRIVER_SIGNAL = 7,
        ENDGAME = 8
    };

    enum class APIIndex
    {
        DEFAULT = 0,
        SET_COLOR = 1,
        SET_BRIGHTNESS = 2,
        SET_BLINK = 3,
        SET_DEFAULT_STATE = 4,
        FLOAT_PIXEL = 5,
        ///////////////////////////////////////////
        //Support up to a 32 character sequenced morse string, 8 chars per packet
        MORSE_MESSAGE_1 = 1,
        MORSE_MESSAGE_2 = 2,
        MORSE_MESSAGE_3 = 3,
        MORSE_MESSAGE_4 = 4
        ///////////////////////////////////////////
    };
    
public:
    enum class LEDState
    {
        OFF,
        FIXED_ON,
        BLINK,
        MORSE
    };

    CKLEDController(int deviceID);
    ~CKLEDController();

    void setOn();
    void setOff();
    void setBlink();
    void setCommLoss();
    void setCommRestored();
    void setMorse(std::string msg);
    void setDriverSignal();
    void setEndgame();
    void setFloatPixel(RGBColor pixelColor, int pixelCount, int pixelRepeatSpacing = 0);
    void setColor(RGBColor rgbColor);
    void setBrightness(int brightness);
    void setDefaultState(LEDState defaultState);
    void configureBlink(int blinkCount, int blinkRateMs);

private:
    int mDeviceID;
    
    static constexpr int MAX_MORSE_STRING_SIZE = 32;
};