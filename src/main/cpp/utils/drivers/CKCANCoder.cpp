#include "utils/drivers/CKCANCoder.hpp"
#include "utils/PhoenixHelper.hpp"
#include "utils/CKMath.hpp"
#include "utils/CKLogger.hpp"

CKCANCoder::CKCANCoder(int deviceNumber, std::string const &canbus)
:mCANCoder(deviceNumber, canbus)
{

}

double CKCANCoder::getAbsolutePosition()
{
    return mCANCoder.GetAbsolutePosition();
}

double CKCANCoder::getRelativePosition()
{
    return mCANCoder.GetPosition();
}

double CKCANCoder::getPosition()
{
    switch (CKEncoder::m_sensor_source)
    {
        case EncoderSensorSource::RELATIVE:
        {
            return getRelativePosition();
        }
        case EncoderSensorSource::ABSOLUTE:
        {
            return getAbsolutePosition();
        }
    }
    return 0;
}

double CKCANCoder::getVelocity()
{
    return mCANCoder.GetVelocity();
}

bool CKCANCoder::isFaulted()
{
    return mCANCoder.GetMagnetFieldStrength() == MagnetFieldStrength::BadRange_RedLED
        || mCANCoder.GetMagnetFieldStrength() == MagnetFieldStrength::Invalid_Unknown;
}

bool CKCANCoder::configSensorSource(EncoderSensorSource sensor_source)
{
    CKEncoder::m_sensor_source = sensor_source;
    return true;
}

bool CKCANCoder::configInitializationStrategy(InitializationStrategy initialization_strategy)
{
    return ck::runPhoenixFunctionWithRetry([&]() { return mCANCoder.ConfigSensorInitializationStrategy((SensorInitializationStrategy)initialization_strategy, ck::kCANTimeoutMs); });
}

bool CKCANCoder::reset()
{
    return ck::runPhoenixFunctionWithRetry([&]() { return mCANCoder.SetPosition(0, ck::kCANTimeoutMs); });
}

CANCoder& CKCANCoder::getRawCANCoder()
{
    return mCANCoder;
}
