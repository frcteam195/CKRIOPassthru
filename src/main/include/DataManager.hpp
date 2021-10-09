#pragma once

#include "DataStructures.hpp"
#include "utils/Singleton.hpp"
#include "ctre/Phoenix.h"
#include <vector>
#include <map>

enum class MotorType
{
    TALON_FX = 0,
    TALON_SRX = 1
};

class DataManager : public Singleton<DataManager>
{
    friend Singleton;
public:
    std::map<uint16_t, BaseMotorController*> motorObjectList;
    std::map<uint16_t, MotorType> motorTypeList;
private:
    DataManager();
};