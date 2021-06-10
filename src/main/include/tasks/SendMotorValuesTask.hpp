#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "ITask.hpp"

class SendMotorValuesTask : public ITask
{
public:
    SendMotorValuesTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
private:
    
};