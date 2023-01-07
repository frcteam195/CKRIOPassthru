#pragma once

#include "ctre/Phoenix.h"
#include "CKEncoder.hpp"
#include <string>
#include <atomic>

class CKCANCoder : public CKEncoder
{
public:
    CKCANCoder(int deviceNumber = 0, std::string const &canbus = "");
    double getAbsolutePosition() override;
    double getRelativePosition() override;
    double getPosition() override;
    double getVelocity() override;
    bool isFaulted() override;
    bool configSensorSource(EncoderSensorSource sensor_source) override;
    bool configInitializationStrategy(InitializationStrategy initialization_strategy) override;
    bool reset() override;
    CANCoder& getRawCANCoder();

private:
    CANCoder mCANCoder;
};