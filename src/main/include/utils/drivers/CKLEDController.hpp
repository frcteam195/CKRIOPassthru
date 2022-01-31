#pragma once

#include "frc/CAN.h"
#include <string>
#include "RGBColor.hpp"
#include "utils/drivers/CKCANDevice.hpp"

#define CK_LED_CAN_DEVICE_TYPE HAL_CAN_Dev_kIOBreakout
#define CK_LED_CAN_MANUFACTURER_TYPE HAL_CAN_Man_kTeamUse

class CKLEDController : public CKCANDevice
{
    enum class APIClass : int
    {
        IDLE = 0,
        OFF = 1,
        FIXED_ON = 2,
        BLINK = 3,
        COMM_LOSS = 4,
        COMM_RESTORED = 5,
        MORSE = 6,
        DRIVER_SIGNAL = 7,
        ENDGAME =8
    };

    enum class APIIndex : int
    {
        DEFAULT = 0,
        SET_COLOR = 1,
        SET_BLINK = 2,
        FLOAT_PIXEL = 3,
        ///////////////////////////////////////////
        //Support up to a 32 character sequenced morse string, 8 chars per packet
        MORSE_MESSAGE_1 = 1,
        MORSE_MESSAGE_2 = 2,
        MORSE_MESSAGE_3 = 3,
        MORSE_MESSAGE_4 = 4
        ///////////////////////////////////////////
    };

public:
    CKLEDController(int deviceID);
    ~CKLEDController();

    void setLEDsOn();
    void setLEDsOff();
    void setLEDsBlink();
    void setLEDsCommLoss();
    void setLEDsCommRestored();
    void setLEDsMorse(std::string msg);
    void setDriverSignal();
    void setEndgame();
    void setFloatPixel(RGBColor pixelColor, int pixelCount, int pixelRepeatSpacing = 0);
    void setColor(RGBColor rgbColor);
    void configureBlink(int blinkRate, int blinkRateMs);

private:
    int mDeviceID;
    
    static constexpr int MAX_MORSE_STRING_SIZE = 32;
};