#include "failover/FailoverMessageManager.hpp"
#include "NetworkManager.hpp"
#include <cstdint>

FailoverMessageManager::FailoverMessageManager()
{
    mBuff = malloc(BUF_SIZE * sizeof(uint8_t));
    memset(mBuff, 0, BUF_SIZE * sizeof(uint8_t));
}

FailoverMessageManager::~FailoverMessageManager()
{
    free(mBuff);
}

ck::MotorControl::Motor* FailoverMessageManager::addMotorControl()
{
    std::scoped_lock<std::recursive_mutex> lock(mLock);
    return mMotorControl.add_motors();
}

ck::MotorConfiguration::Motor* FailoverMessageManager::addMotorConfig()
{
    std::scoped_lock<std::recursive_mutex> lock(mLock);
    return mMotorConfiguration.add_motors();
}

ck::SolenoidControl::Solenoid* FailoverMessageManager::addSolenoidControl()
{
    std::scoped_lock<std::recursive_mutex> lock(mLock);
    return mSolenoidControl.add_solenoids();
}

ck::IMUConfig::IMUConfigData* FailoverMessageManager::addIMUConfig()
{
    std::scoped_lock<std::recursive_mutex> lock(mLock);
    return mIMUConfig.add_imu_config();
}

ck::LEDControl::LEDControlData* FailoverMessageManager::addLEDControl()
{
    std::scoped_lock<std::recursive_mutex> lock(mLock);
    return mLEDControl.add_led_control();
}

void FailoverMessageManager::publishMessages()
{
    //Called at roughly 20ms
    std::scoped_lock<std::recursive_mutex> lock(mLock);
    static uint32_t mLoopCounter = 0;

    if (mMotorControl.SerializeToArray(mBuff, BUF_SIZE))
    {
        std::vector<uint8_t> buf(mMotorControl.ByteSizeLong(), 0);
        memcpy(&buf[0], mBuff, mMotorControl.ByteSizeLong());
        NetworkManager::getInstance().placeFailoverMessage("motorcontrol", buf);
    }

    if (mSolenoidControl.SerializeToArray(mBuff, BUF_SIZE))
    {
        std::vector<uint8_t> buf(mSolenoidControl.ByteSizeLong(), 0);
        memcpy(&buf[0], mBuff, mSolenoidControl.ByteSizeLong());
        NetworkManager::getInstance().placeFailoverMessage("solenoidcontrol", buf);
    }

    //Should be roughly 500ms
    if (mLoopCounter++ % 25 == 0)
    {

        if (mMotorConfiguration.SerializeToArray(mBuff, BUF_SIZE))
        {
            std::vector<uint8_t> buf(mMotorConfiguration.ByteSizeLong(), 0);
            memcpy(&buf[0], mBuff, mMotorConfiguration.ByteSizeLong());
            NetworkManager::getInstance().placeFailoverMessage("motorconfig", buf);
        }

        if (mLEDControl.SerializeToArray(mBuff, BUF_SIZE))
        {
            std::vector<uint8_t> buf(mLEDControl.ByteSizeLong(), 0);
            memcpy(&buf[0], mBuff, mLEDControl.ByteSizeLong());
            NetworkManager::getInstance().placeFailoverMessage("ledcontrol", buf);
        }

        // if (mIMUConfig.SerializeToArray(mBuff, BUF_SIZE))
        // {
        //     std::vector<uint8_t> buf(mIMUConfig.ByteSizeLong(), 0);
        //     memcpy(&buf[0], mBuff, mIMUConfig.ByteSizeLong());
        //     NetworkManager::getInstance().placeFailoverMessage("imuconfig", buf);
        // }
    }
}