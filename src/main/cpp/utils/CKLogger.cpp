#include "utils/CKLogger.hpp"

namespace ck
{
    std::unordered_map<std::string, ck::LogLevel> messageMap;
    std::recursive_mutex messageMapLock;
    LogLevel currLogLevel = LogLevel::INFO;

    void log(std::string msg, LogLevel logLevel)
    {
#ifdef CONSOLE_REPORTING
        (void)CKLogRunner::getInstance();
        std::scoped_lock<std::recursive_mutex> lock(messageMapLock);
        messageMap[msg] = logLevel;
#endif
    }

    void set_log_level(LogLevel logLevel)
    {
#ifdef CONSOLE_REPORTING
        std::scoped_lock<std::recursive_mutex> lock(messageMapLock);
        currLogLevel = logLevel;
#endif
    }

    void _log_pump()
    {
#ifdef CONSOLE_REPORTING
        bool needFlush = false;
        {
            std::scoped_lock<std::recursive_mutex> lock(messageMapLock);
            if (messageMap.size() > 0)
            {
                needFlush = true;
                for (auto it = messageMap.cbegin(); it != messageMap.cend(); )
                {
                    if (it->second >= currLogLevel)
                    {
                        std::cout << it->first;
                    }
                    it = messageMap.erase(it);
                }
            }
        }

        if (needFlush)
        {
            std::cout.flush();
        }
#endif
    }

    CKLogRunner::CKLogRunner()
    {
#ifdef CONSOLE_REPORTING
        mThreadActive = true;
        mThread = std::thread(&CKLogRunner::runThread, this);
#endif
    }

    CKLogRunner::~CKLogRunner()
    {
#ifdef CONSOLE_REPORTING
        mThreadActive = false;
        if (mThread.joinable())
        {
            mThread.join();
        }
#endif
    }

    void CKLogRunner::runThread()
    {
#ifdef CONSOLE_REPORTING
        mRateControl.start();
        std::ios_base::sync_with_stdio(false);
        while (mThreadActive)
        {
            _log_pump();
            mRateControl.doRateControl(LOGGER_THREAD_RATE_MS);
        }
#endif
    }
}