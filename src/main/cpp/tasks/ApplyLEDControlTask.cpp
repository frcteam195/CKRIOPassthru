#include "tasks/ApplyLEDControlTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <map>
#include <vector>
#include "DataStructures.hpp"
#include "CANdleManager.hpp"
#include "ctre/Phoenix.h"
#include "NetworkManager.hpp"
#include "utils/PhoenixHelper.hpp"
#include <iostream>
#include "utils/CKLogger.hpp"

#include "ctre/phoenix/led/ColorFlowAnimation.h"
#include "ctre/phoenix/led/FireAnimation.h"
#include "ctre/phoenix/led/LarsonAnimation.h"
#include "ctre/phoenix/led/RainbowAnimation.h"
#include "ctre/phoenix/led/RgbFadeAnimation.h"
#include "ctre/phoenix/led/SingleFadeAnimation.h"
#include "ctre/phoenix/led/StrobeAnimation.h"
#include "ctre/phoenix/led/TwinkleAnimation.h"
#include "ctre/phoenix/led/TwinkleOffAnimation.h"

using namespace ctre::phoenix::led;


// #define DEBUG

ApplyLEDControlTask::ApplyLEDControlTask() : Task(THREAD_RATE_MS, TASK_NAME)
{
    NetworkManager::getInstance().joinGroup(LED_CONTROL_MESSAGE_GROUP.c_str());
    initFieldDescriptors();
    initUpdateFunctions();
    mDiff.set_report_matches(false);
    mDiff.set_report_moves(false);
    mDiff.ReportDifferencesTo(&mDiffReporter);
}

void ApplyLEDControlTask::run(uint32_t timeSinceLastUpdateMs)
{
    mTaskTimer.start();
    //TODO: Improve memory efficiency
    std::vector<uint8_t> buf;
    if (NetworkManager::getInstance().getMessage(LED_CONTROL_MESSAGE_GROUP, buf))
    {
        ck::LEDControl candlesUpdate;
        candlesUpdate.ParseFromArray(&buf[0], buf.size());
        if (CANdleManager::getInstance().try_lock())
        {
            CANdleManager::getInstance().setCANdlesConfigMsg(candlesUpdate);
            std::map<uint16_t, ck::LEDControl::LEDControlData>& mPrevCANdlesControlMsgMap = CANdleManager::getInstance().getPrevCANdlesConfigMsg();
            for (ck::LEDControl::LEDControlData m : candlesUpdate.led_control())
            {
                CANdleManager::getInstance().registerCANdle(m.id(), (CANInterface)m.can_network());

                bool updateSuccessful = true;
                if (CANdleManager::getInstance().candleExists(m.id()))
                {
                    if ((size_t)candlesUpdate.led_control().size() == mPrevCANdlesControlMsgMap.size())
                    {
                        mDiff.Compare(mPrevCANdlesControlMsgMap[m.id()], m);  //Make sure the current update is message2 for our implementation
                    }
                    else
                    {
                        updateSuccessful = fullUpdate(m);
                    }

                    if (updateSuccessful)
                    {
                        CANdleManager::getInstance().setPrevCANdleConfigMsg(m.id(), m);
                    }
                }
            }
            CANdleManager::getInstance().unlock();
        }
    }
    mTaskTimer.reportElapsedTime();
}

void ApplyLEDControlTask::updateColor(uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& m)
{
    ck::LEDControl_LEDColor c = m.color();
    mCtrl->SetLEDs(c.rgbw_color().r(), c.rgbw_color().g(), c.rgbw_color().b(), c.rgbw_color().w(), c.start_index(), c.num_leds());
}

void ApplyLEDControlTask::updateAnimation(uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& m)
{
    for (ck::LEDAnimation a : m.animation())
    {
        switch(a.animation_type())
        {
            case ck::LEDAnimation_AnimationType_ColorFlow:
            {
                ctre::phoenix::led::ColorFlowAnimation animation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), (ctre::phoenix::led::ColorFlowAnimation::Direction)a.direction(), a.offset());
                mCtrl->Animate(animation, a.slot());
                break;
            }
            case ck::LEDAnimation_AnimationType_Fire:
            {
                ctre::phoenix::led::FireAnimation animation(a.brightness(), a.speed(), a.num_led(), 0.5, 0.5, a.direction(), a.offset());
                mCtrl->Animate(animation, a.slot());
                break;
            }
            case ck::LEDAnimation_AnimationType_Larson:
            {
                //TODO: Fix bounce mode in protobuf API
                ctre::phoenix::led::LarsonAnimation animation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), (ctre::phoenix::led::LarsonAnimation::BounceMode)a.direction(), a.offset());
                mCtrl->Animate(animation, a.slot());
                break;
            }
            case ck::LEDAnimation_AnimationType_Rainbow:
            {
                ctre::phoenix::led::RainbowAnimation animation(a.brightness(), a.speed(), a.num_led(), a.direction(), a.offset());
                mCtrl->Animate(animation, a.slot());
                break;
            }
            case ck::LEDAnimation_AnimationType_RGBFade:
            {
                ctre::phoenix::led::RgbFadeAnimation animation(a.brightness(), a.speed(), a.num_led(), a.offset());
                mCtrl->Animate(animation, a.slot());
                break;
            }
            case ck::LEDAnimation_AnimationType_SingleFade:
            {
                ctre::phoenix::led::SingleFadeAnimation animation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), a.offset());
                mCtrl->Animate(animation, a.slot());
                break;
            }
            case ck::LEDAnimation_AnimationType_Twinkle:
            {
                ctre::phoenix::led::TwinkleAnimation animation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), ctre::phoenix::led::TwinkleAnimation::TwinklePercent::Percent76, a.offset());
                mCtrl->Animate(animation, a.slot());
                break;
            }
            case ck::LEDAnimation_AnimationType_TwinkleOff:
            {
                ctre::phoenix::led::TwinkleOffAnimation animation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), ctre::phoenix::led::TwinkleOffAnimation::TwinkleOffPercent::Percent76, a.offset());
                mCtrl->Animate(animation, a.slot());
                break;
            }
            case ck::LEDAnimation_AnimationType_Strobe:
            {
                std::cout << "Stobe Animation" << std::endl;
                ctre::phoenix::led::StrobeAnimation animation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), a.offset());
                mCtrl->Animate(animation, a.slot());
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

bool ApplyLEDControlTask::fullUpdate(ck::LEDControl::LEDControlData& m)
{
    ck::log("Full CANdle Config Start\n");
    CANdleManager::getInstance().onCANdle(m.id(), [&] (uint16_t id, ctre::phoenix::led::CANdle* mCtrl)
    {
        ck::runPhoenixFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigLEDType((ctre::phoenix::led::LEDStripType)m.led_type(), ck::kCANTimeoutMs); }, id);
        ck::runPhoenixFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigVBatOutput((VBatOutputMode)m.vbat_config(), ck::kCANTimeoutMs); }, id);
        ck::runPhoenixFunctionWithRetry([mCtrl, m]() { return mCtrl->ModulateVBatOutput(m.vbat_duty_cycle()); }, id);
        mCurrLEDCtrlMode[m.id()] = m.led_control_mode();

        switch(m.led_control_mode())
        {
            case ck::LEDControl_LEDControlData_LEDControlMode_Static:
            {
                updateColor(id, mCtrl, m);
                break;
            }
            case ck::LEDControl_LEDControlData_LEDControlMode_Animated:
            {
                updateAnimation(id, mCtrl, m);
                break;
            }
            default:
            {
                break;
            }
        };
    });
    return true;
}

void ApplyLEDControlTask::processLEDUpdate(ck::LEDControl::LEDControlData msg)
{
    if (msg.led_control_mode() == ck::LEDControl_LEDControlData_LEDControlMode_Static)
    {
        updateColor(msg.id(), CANdleManager::getInstance().getCANdle(msg.id()), msg);
    }
    else if (msg.led_control_mode() == ck::LEDControl_LEDControlData_LEDControlMode_Animated)
    {
        updateAnimation(msg.id(), CANdleManager::getInstance().getCANdle(msg.id()), msg);
    }
}

void ApplyLEDControlTask::initFieldDescriptors()
{
    LED_TYPE_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(3);
    VBAT_CONFIG_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(4);
    VBAT_DUTY_CYCLE_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(5);
    LED_CONTROL_MODE_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(6);
    COLOR_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(7);
    ANIMATION_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(8);
}

void ApplyLEDControlTask::initUpdateFunctions()
{
    mDiffReporter.RegisterUpdateFunction(LED_TYPE_FD, [](const google::protobuf::Message &msg)
    {
        CANdleManager::getInstance().onCANdle(msg,
            [](uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& d)
            { ck::runPhoenixFunctionWithRetry([mCtrl, d]() { return mCtrl->ConfigLEDType((LEDStripType)d.led_type(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(VBAT_CONFIG_FD, [](const google::protobuf::Message &msg)
    {
        CANdleManager::getInstance().onCANdle(msg,
            [](uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& d)
            { ck::runPhoenixFunctionWithRetry([mCtrl, d]() { return mCtrl->ConfigVBatOutput((VBatOutputMode)d.vbat_config(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(VBAT_DUTY_CYCLE_FD, [](const google::protobuf::Message &msg)
    {
        CANdleManager::getInstance().onCANdle(msg,
            [](uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& d)
            { ck::runPhoenixFunctionWithRetry([mCtrl, d]() { return mCtrl->ModulateVBatOutput(d.vbat_duty_cycle()); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(LED_CONTROL_MODE_FD, [&](const google::protobuf::Message &msg)
    {
        const ck::LEDControl::LEDControlData& m = (const ck::LEDControl::LEDControlData&)msg;
        mCurrLEDCtrlMode[m.id()] = m.led_control_mode();
        processLEDUpdate(m);
    });

    mDiffReporter.RegisterUpdateFunction(COLOR_FD, [&](const google::protobuf::Message &msg)
    {
        const ck::LEDControl::LEDControlData& m = (const ck::LEDControl::LEDControlData&)msg;
        mCurrLEDCtrlMode[m.id()] = m.led_control_mode();
        processLEDUpdate(m);
    });

    mDiffReporter.RegisterUpdateFunction(ANIMATION_FD, [&](const google::protobuf::Message &msg)
    {
        const ck::LEDControl::LEDControlData& m = (const ck::LEDControl::LEDControlData&)msg;
        mCurrLEDCtrlMode[m.id()] = m.led_control_mode();
        processLEDUpdate(m);
    });
}