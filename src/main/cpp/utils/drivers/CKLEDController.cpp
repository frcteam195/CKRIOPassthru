#include "utils/drivers/CKLEDController.hpp"
#include <cstring>
#include <algorithm>
#include <cctype>
#include "CKCANServer.hpp"
#include "utils/drivers/CKCANPacket.hpp"
#include <vector>
#include "utils/CKErrors.hpp"

CKLEDController::CKLEDController(int deviceID)
: CKCANDevice(deviceID, CK_LED_CAN_DEVICE_TYPE, CK_LED_CAN_MANUFACTURER_TYPE), mDeviceID(deviceID & 0x3F)
{}

CKLEDController::~CKLEDController() {}

void CKLEDController::setOn()
{
    std::vector<uint8_t> buf = { 0 };
    CKCANPacket canPacket = {this, getAPIID((int)APIClass::FIXED_ON, (int)APIIndex::DEFAULT), buf};
    CKCANServer::getInstance().sendPacket(canPacket);
}

void CKLEDController::setOff()
{
    std::vector<uint8_t> buf = { 0 };
    CKCANPacket canPacket = {this, getAPIID((int)APIClass::OFF, (int)APIIndex::DEFAULT), buf};
    CKCANServer::getInstance().sendPacket(canPacket);
}

void CKLEDController::setBlink()
{
    std::vector<uint8_t> buf = { 0 };
    CKCANPacket canPacket = {this, getAPIID((int)APIClass::BLINK, (int)APIIndex::DEFAULT), buf};
    CKCANServer::getInstance().sendPacket(canPacket);
}

void CKLEDController::setCommLoss()
{
    std::vector<uint8_t> buf = { 0 };
    CKCANPacket canPacket = {this, getAPIID((int)APIClass::COMM_LOSS, (int)APIIndex::DEFAULT), buf};
    CKCANServer::getInstance().sendPacket(canPacket);
}

void CKLEDController::setCommRestored()
{
    std::vector<uint8_t> buf = { 0 };
    CKCANPacket canPacket = {this, getAPIID((int)APIClass::COMM_RESTORED, (int)APIIndex::DEFAULT), buf};
    CKCANServer::getInstance().sendPacket(canPacket);
}

void CKLEDController::setMorse(std::string msg)
{
    //Check if string is empty or whitespace and return
    if (msg.empty() || std::all_of(msg.begin(), msg.end(), [](char c){return std::isspace(c);}))
    {
        ck::ReportError("Morse code message is empty or whitespace!");
        return;
    }

    if (msg.length() > MAX_MORSE_STRING_SIZE)
    {
        ck::ReportError("Morse code message size is too long!");
        return;
    }

    int msgCounter = 0;
    while (msg.length() > 0 && msgCounter < 4)
    {
        int msgLength = msg.length() >= 8 ? 8 : msg.length();
        std::vector<uint8_t> buf;
        for (int i = 0; i < msgLength; i++)
        {
            buf.push_back(msg.front());
            msg.erase(0,1);
        }
        CKCANPacket canPacket = {this, getAPIID((int)APIClass::MORSE, (int)(APIIndex)(++msgCounter)), buf};
        CKCANServer::getInstance().sendPacket(canPacket);
    }
}

void CKLEDController::setFade()
{
    std::vector<uint8_t> buf = { 0 };
    CKCANPacket canPacket = {this, getAPIID((int)APIClass::FADE, (int)APIIndex::DEFAULT), buf};
    CKCANServer::getInstance().sendPacket(canPacket);
}

void CKLEDController::setFloatPixel(RGBColor pixelColor, int pixelCount, int pixelRepeatSpacing)
{
    std::vector<uint8_t> buf = {pixelColor.white, pixelColor.red, pixelColor.green, pixelColor.blue,
        (uint8_t)(pixelCount & 0xFF), (uint8_t)((pixelCount >> 8) & 0xFF),
        (uint8_t)(pixelRepeatSpacing & 0xFF), (uint8_t)((pixelRepeatSpacing >> 8) & 0xFF)};
    CKCANPacket canPacket = {this, getAPIID((int)APIClass::FIXED_ON, (int)APIIndex::FLOAT_PIXEL), buf};
    CKCANServer::getInstance().sendPacket(canPacket);
}

void CKLEDController::setColor(RGBColor rgbColor)
{
    std::vector<uint8_t> buf = {rgbColor.white, rgbColor.red, rgbColor.green, rgbColor.blue};
    CKCANPacket canPacket = {this, getAPIID((int)APIClass::IDLE, (int)APIIndex::SET_COLOR), buf};
    CKCANServer::getInstance().sendPacket(canPacket);
}

void CKLEDController::setBrightness(int brightness)
{
    std::vector<uint8_t> buf = {(uint8_t) brightness};
    CKCANPacket canPacket = {this, getAPIID((int)APIClass::IDLE, (int)APIIndex::SET_BRIGHTNESS), buf};
    CKCANServer::getInstance().sendPacket(canPacket);
}

void CKLEDController::setDefaultState(LEDState defaultState)
{
    std::vector<uint8_t> buf = {(uint8_t) defaultState};
    CKCANPacket canPacket = {this, getAPIID((int)APIClass::IDLE, (int)APIIndex::SET_DEFAULT_STATE), buf};
    CKCANServer::getInstance().sendPacket(canPacket);
}

void CKLEDController::configureBlink(int blinkCount, int blinkRateMs)
{
    //Support blink_rate as a uint16_t, max value 65535 milliseconds, blink count as a uint8
    std::vector<uint8_t> buf = {(uint8_t)(blinkRateMs & 0xFF), (uint8_t)((blinkRateMs >> 8) & 0xFF), (uint8_t)(blinkCount & 0xFF)};
    CKCANPacket canPacket = {this, getAPIID((int)APIClass::IDLE, (int)APIIndex::SET_BLINK), buf};
    CKCANServer::getInstance().sendPacket(canPacket);
}