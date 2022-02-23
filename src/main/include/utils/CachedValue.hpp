#pragma once
#include "TimeoutTimer.hpp"
#include <functional>
#include <mutex>

template <typename T>
class CachedValue
{
public:
    CachedValue(double updateTimeoutMs, const std::function<T(void)>& updateFunction) : mTimeoutTimer(updateTimeoutMs)
    {
        mUpdateFunction = updateFunction;
        mCachedValue = mUpdateFunction();
    }

    T getValue()
    {
        std::scoped_lock<std::recursive_mutex> lock(mCacheMutex);
        
        if (mTimeoutTimer.isTimedOut()) {
            mCachedValue = mUpdateFunction();
            mTimeoutTimer.reset();
        }

		return mCachedValue;
	}

private:
    TimeoutTimer mTimeoutTimer;
    T mCachedValue;
    std::function<T(void)> mUpdateFunction;
    std::recursive_mutex mCacheMutex;
};