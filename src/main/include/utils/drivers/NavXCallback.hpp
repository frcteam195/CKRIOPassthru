#pragma once

#include <AHRS.h>
#include "CKNavX.hpp"

class CKNavX;

class NavXCallback : public ITimestampedDataSubscriber {
public:
    NavXCallback(CKNavX *navX);
    void timestampedDataReceived( long system_timestamp, long sensor_timestamp, AHRSProtocol::AHRSUpdateBase& sensor_data, void * context ) override;

private:
    CKNavX *mNavX;
};