#include "LEDManager.hpp"
#include "utils/RobotControlModeHelper.hpp"

LEDManager::LEDManager() : mLEDController(CK_LED_CAN_DEVICE_ID)
{
    mLEDController.setCommLoss();
    mRequestedState = LEDMode::COMM_LOSS;
    mRGBColor = DEFAULT_LED_COLOR;
}
LEDManager::~LEDManager() {}

void LEDManager::sendUpdate()
{
    if (mRequestedState != LEDMode::COMM_LOSS && mRequestedState != LEDMode::COMM_RESTORED)
    {
        mLEDController.setColor(mRGBColor);
    }
    
    switch (DEFAULT_LED_MODE)
    {
        case LEDMode::FADE:
        {
            mLEDController.setDefaultState(CKLEDController::LEDState::FADE);
            break;
        }
        case LEDMode::FIXED_ON:
        {
            mLEDController.setDefaultState(CKLEDController::LEDState::FIXED_ON);
            break;
        }
        case LEDMode::BLINK:
        {
            mLEDController.setDefaultState(CKLEDController::LEDState::BLINK);
            break;
        }
        default:
        {
            mLEDController.setDefaultState(CKLEDController::LEDState::FIXED_ON);
            break;
        }
    }

    bool mDSAttachedState = RobotControlModeHelper::getInstance().isDSAttached();
    if (mDSAttachedState == mPrevDSAttachedState)
    {
        //Normal Operation

        switch (mRequestedState)
        {
            case LEDMode::OFF:
            {
                mLEDController.setOff();
                break;
            }
            case LEDMode::FIXED_ON:
            {
                mLEDController.setOn();
                break;
            }
            case LEDMode::BLINK:
            {
                mLEDController.setBlink();
                break;
            }
            case LEDMode::MORSE:
            {
                //mLEDController.setMorse();
                break;
            }
            case LEDMode::COMM_LOSS:
            {
                mLEDController.setCommLoss();
                break;
            }
            case LEDMode::COMM_RESTORED:
            {
                mLEDController.setCommRestored();
                mRequestedState = DEFAULT_LED_MODE;
                break;
            }
            case LEDMode::FADE:
            {
                mLEDController.setFade();
                break;
            }
            case LEDMode::NO_CHANGE:
            {
                break;
            }
        }
    }
    else if (mDSAttachedState && mPrevDSAttachedState != mDSAttachedState)
    {
        //Just Reconnected
        setRequestedState(LEDMode::COMM_RESTORED);
    }
    else if (!mDSAttachedState && mPrevDSAttachedState != mDSAttachedState)
    {
        //Just Disconnected
        setRequestedState(LEDMode::COMM_LOSS);
    }
    
    mPrevDSAttachedState = mDSAttachedState;
}

void LEDManager::setRequestedState(LEDMode requestedState)
{
    mRequestedState = requestedState;
}

void LEDManager::setColor(RGBColor rgbColor)
{
    std::scoped_lock<std::recursive_mutex> lock(mLEDLock);
    mRGBColor = rgbColor;
}
