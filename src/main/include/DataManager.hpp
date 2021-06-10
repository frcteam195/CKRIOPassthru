#pragma once
#include "DataStructures.hpp"
#include "utils/Singleton.hpp"

class DataManager : public Singleton<DataManager>
{
    friend Singleton;
public:
    std::vector<MotorData> motorData;
    std::vector<MotorConfigData> motorConfigData;
private:
    DataManager();
};