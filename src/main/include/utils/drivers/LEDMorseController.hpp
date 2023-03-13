#pragma once

#include <ctre/Phoenix.h>
#include <utils/drivers/RGBColor.hpp>
#include <string>
#include "IntellisenseDefs.h"
#include "LEDControl.pb.h"

void update_morse_controller(CANdle* candle, int num_leds, const std::string& msg, const ck::RGBWColor& color);