#pragma once

class ITask
{
    virtual void run(unsigned int timeSinceLastUpdateMs) = 0;
};