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
#include "utils/ProtobufTypeHelper.hpp"

#include "utils/drivers/LEDMorseController.hpp"

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
        if (candlesUpdate.ParseFromArray(&buf[0], buf.size()))
        {
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
    }
    mTaskTimer.reportElapsedTime();
}

void ApplyLEDControlTask::clearAllAnimations(uint16_t id)
{
    CANdleManager::getInstance().onCANdle(id, [&] (uint16_t id, ctre::phoenix::led::CANdle* mCtrl)
    {
        for (auto an : CANdleManager::getInstance().getPrevCANdlesConfigMsg()[id].animation())
        {
            ck::runPhoenixFunctionWithRetry([mCtrl, an]() { return mCtrl->ClearAnimation(an.slot()); }, id);
        }
    });
}

void ApplyLEDControlTask::updateColor(const ck::LEDControl::LEDControlData& m)
{
    clearAllAnimations(m.id());
    CANdleManager::getInstance().onCANdle(m.id(), [&] (uint16_t id, ctre::phoenix::led::CANdle* mCtrl)
    {
        const ck::LEDControl_LEDColor& c = m.color();
        ck::runPhoenixFunctionWithRetry([mCtrl, c]() { return mCtrl->SetLEDs(c.rgbw_color().r(), c.rgbw_color().g(), c.rgbw_color().b(), c.rgbw_color().w(), c.start_index(), c.num_leds()); }, id);
    });
}

ctre::phoenix::led::Animation* ApplyLEDControlTask::animationLookup(const ck::LEDAnimation& a)
{
    switch(a.animation_type())
    {
        case ck::LEDAnimation_AnimationType_ColorFlow:
        {
            return new ctre::phoenix::led::ColorFlowAnimation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), (ctre::phoenix::led::ColorFlowAnimation::Direction)a.direction(), a.offset());
        }
        case ck::LEDAnimation_AnimationType_Fire:
        {
            return new ctre::phoenix::led::FireAnimation(a.brightness(), a.speed(), a.num_led(), 1.0, 1.0, a.direction(), a.offset());
        }
        case ck::LEDAnimation_AnimationType_Larson:
        {
            //TODO: Fix bounce mode in protobuf API
            return new ctre::phoenix::led::LarsonAnimation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), (ctre::phoenix::led::LarsonAnimation::BounceMode)a.direction(), 7, a.offset());
        }
        case ck::LEDAnimation_AnimationType_Rainbow:
        {
            return new ctre::phoenix::led::RainbowAnimation(a.brightness(), a.speed(), a.num_led(), a.direction(), a.offset());
        }
        case ck::LEDAnimation_AnimationType_RGBFade:
        {
            return new ctre::phoenix::led::RgbFadeAnimation(a.brightness(), a.speed(), a.num_led(), a.offset());
        }
        case ck::LEDAnimation_AnimationType_SingleFade:
        {
            return new ctre::phoenix::led::SingleFadeAnimation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), a.offset());
        }
        case ck::LEDAnimation_AnimationType_Twinkle:
        {
            return new ctre::phoenix::led::TwinkleAnimation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), ctre::phoenix::led::TwinkleAnimation::TwinklePercent::Percent30, a.offset());
        }
        case ck::LEDAnimation_AnimationType_TwinkleOff:
        {
            return new ctre::phoenix::led::TwinkleOffAnimation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), ctre::phoenix::led::TwinkleOffAnimation::TwinkleOffPercent::Percent30, a.offset());
        }
        case ck::LEDAnimation_AnimationType_Strobe:
        {
            return new ctre::phoenix::led::StrobeAnimation(a.color().r(), a.color().g(), a.color().b(), a.color().w(), a.speed(), a.num_led(), a.offset());
        }
        default:
        {
            break;
        }
    }
    return NULL;
}

void ApplyLEDControlTask::updateAnimation(const ck::LEDControl::LEDControlData& m)
{
    clearAllAnimations(m.id());
    CANdleManager::getInstance().onCANdle(m.id(), [&] (uint16_t id, ctre::phoenix::led::CANdle* mCtrl)
    {
        for (ck::LEDAnimation a : m.animation())
        {
            if (a.animation_type() == ck::LEDAnimation::AnimationType::LEDAnimation_AnimationType_Morse)
            {
                try
                {
                    // update_morse_controller(mCtrl, a.num_led(), a.morse_message(), a.color());
                }
                catch (std::exception& e)
                {
                    //Just here to prevent code issues until we can test more
                }
            }
            else
            {
                ctre::phoenix::led::Animation* ctre_animation = animationLookup(a);
                if (ctre_animation != NULL)
                {
                    ck::runPhoenixFunctionWithRetry([mCtrl, ctre_animation, a]() { return mCtrl->Animate(*ctre_animation, a.slot()); }, id);
                    delete ctre_animation;
                }
            }
        }
    });
}

bool ApplyLEDControlTask::fullUpdate(ck::LEDControl::LEDControlData& m)
{
    ck::log("Full CANdle Config Start\n");
    CANdleManager::getInstance().onCANdle(m.id(), [&] (uint16_t id, ctre::phoenix::led::CANdle* mCtrl)
    {
        ck::runPhoenixFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigLEDType((ctre::phoenix::led::LEDStripType)m.led_type(), ck::kCANTimeoutMs); }, id);
        ck::runPhoenixFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigVBatOutput((VBatOutputMode)m.vbat_config(), ck::kCANTimeoutMs); }, id);
        ck::runPhoenixFunctionWithRetry([mCtrl, m]() { return mCtrl->ModulateVBatOutput(m.vbat_duty_cycle()); }, id);

        switch(m.led_control_mode())
        {
            case ck::LEDControl_LEDControlData_LEDControlMode_Static:
            {
                updateColor(m);
                break;
            }
            case ck::LEDControl_LEDControlData_LEDControlMode_Animated:
            {
                updateAnimation(m);
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

void ApplyLEDControlTask::processLEDUpdate(const google::protobuf::Message &msg)
{
    ck::LEDControl::LEDControlData m;
    if (getTypedMessage(msg, m))
    {
        if (m.led_control_mode() == ck::LEDControl_LEDControlData_LEDControlMode_Static)
        {
            updateColor(m);
        }
        else if (m.led_control_mode() == ck::LEDControl_LEDControlData_LEDControlMode_Animated)
        {
            updateAnimation(m);
        }
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
        processLEDUpdate(msg);
    });

    mDiffReporter.RegisterUpdateFunction(COLOR_FD, [&](const google::protobuf::Message &msg)
    {
        processLEDUpdate(msg);
    });

    mDiffReporter.RegisterUpdateFunction(ANIMATION_FD, [&](const google::protobuf::Message &msg)
    {
        processLEDUpdate(msg);
    });
}