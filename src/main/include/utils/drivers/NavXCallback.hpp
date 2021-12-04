#pragma once

#include <AHRS.h>
#include "NavX.hpp"

class NavX;

class NavXCallback : public ITimestampedDataSubscriber {
public:
    NavXCallback(NavX *navX);
    void timestampedDataReceived( long system_timestamp, long sensor_timestamp, AHRSProtocol::AHRSUpdateBase& sensor_data, void * context ) override;

private:
    NavX *mNavX;
};