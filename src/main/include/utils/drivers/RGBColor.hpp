#pragma once
#include <cstdint>
#include "IntellisenseDefs.h"
#include "LEDControl.pb.h"

struct RGBColor
{
    uint8_t white = 0;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    inline bool operator==(const RGBColor& other) const{
        if(other.white == white && other.red == red && other.green == green && other.blue == blue)
        {
            return true;
        }
        return false;
    };

    inline bool operator==(const ck::RGBWColor& other) const{
        if(other.w() == white && other.r() == red && other.g() == green && other.b() == blue)
        {
            return true;
        }
        return false;
    };

    inline RGBColor& operator=(const ck::RGBWColor& other){
        white = other.w();
        red = other.r();
        green = other.g();
        blue = other.b();
        return *this;
    };
};