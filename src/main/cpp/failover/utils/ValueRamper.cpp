#include "failover/utils/ValueRamper.hpp"

ValueRamper::ValueRamper(double accelRampRate, double decelRampRate, double zeroValue, double maxValue)
: mAccelRampRate(accelRampRate), mDecelRampRate(decelRampRate), mZeroValue(zeroValue), mMaxValue(maxValue)
{}

double ValueRamper::calculateOutput(double currValue)
{
    double timeNow = frc::Timer::GetFPGATimestamp().value();
    if (mPrevTime != 0)
    {
        double dt = timeNow - mPrevTime;
        double accelStep = (mMaxValue - mZeroValue) * dt / mAccelRampRate;
        double decelStep = (mMaxValue - mZeroValue) * dt / mDecelRampRate;
        mPrevTime = timeNow;

        if (currValue > mPrevValue && currValue >= 0)
        {
            //accelerating
            mPrevValue += accelStep;
            mPrevValue = std::min(mPrevValue, currValue);
            return mPrevValue;
        }
        else if(currValue < mPrevValue && currValue >= 0)
        {
            //decelerating
            mPrevValue -= decelStep;
            mPrevValue = std::max(mPrevValue, currValue);
            return mPrevValue;
        }
        else if (currValue < mPrevValue && currValue <= 0)
        {
            //accelerating
            mPrevValue -= accelStep;
            mPrevValue = std::max(mPrevValue, currValue);
            return mPrevValue;
        }
        else if (currValue > mPrevValue && currValue <= 0)
        {
            //decelerating
            mPrevValue += decelStep;
            mPrevValue = std::min(mPrevValue, currValue);
            return mPrevValue;
        }
        else
        {
            mPrevValue = currValue;
            return currValue;
        }
    }
    mPrevTime = timeNow;
    return 0;
}