# include "ExternalControlManager.hpp"

ExternalControlManager::ExternalControlManager() {}
ExternalControlManager::~ExternalControlManager() {}

void ExternalControlManager::externalControlMsgReceived()
{
    //50 ticks per second
    mLastConnectionTime = 50;  //500ms timeout
}

bool ExternalControlManager::isExternalControlConnected()
{
    return mLastConnectionTime > 0;
}

//Planned to run at 20ms
void ExternalControlManager::processHeartbeat()
{
    //Prevent overflow. Less efficient, but guarantees no bound on runtime
    if (mLastConnectionTime > 0)
    {
        mLastConnectionTime--;
    }
}

bool isExternalControl()
{
    return ExternalControlManager::getInstance().isExternalControlConnected();
}