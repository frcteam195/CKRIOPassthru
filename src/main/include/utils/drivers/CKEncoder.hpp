#pragma once

enum class EncoderSensorSource
{
    RELATIVE = 0,
    ABSOLUTE = 1
};

class CKEncoder
{
public:
    virtual ~CKEncoder() {};
    virtual double getAbsolutePosition() = 0;
    virtual double getRelativePosition() = 0;
    virtual double getPosition() = 0;
    virtual double getVelocity() = 0;
    virtual bool isFaulted() = 0;
    virtual bool configSensorSource(EncoderSensorSource sensor_source) = 0;
    virtual bool reset() = 0;

protected:
    EncoderSensorSource m_sensor_source {EncoderSensorSource::RELATIVE};
};