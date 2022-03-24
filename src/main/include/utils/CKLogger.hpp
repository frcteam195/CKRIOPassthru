#pragma once

#include "utils/Singleton.hpp"
#include <mutex>
#include <unordered_map>
#include <iostream>
#include <atomic>
#include "GlobalConfig.hpp"

namespace ck
{
    enum class LogLevel : int
    {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3
    };

    void log(std::string msg, LogLevel logLevel = LogLevel::INFO);
    void set_log_level(LogLevel logLevel);

    void _log_pump();

    class CKLogRunner : public Singleton<CKLogRunner>
    {
        friend CKLogRunner;
    public:
        
    private:
        static constexpr int LOGGER_THREAD_RATE_MS = 1000;

        std::thread mThread;
        std::atomic<bool> mThreadActive;
        ThreadRateControl mRateControl;
        void runThread();
        CKLogRunner();
        ~CKLogRunner();
    };
};