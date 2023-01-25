#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "Task.hpp"
#include <stdint.h>
#include <map>
#include "IntellisenseDefs.h"
#include "LEDControl.pb.h"
#include "google/protobuf/util/message_differencer.h"
#include "utils/RTTimer.hpp"
#include "utils/RobotControlModeHelper.hpp"
#include "utils/DifferentialUpdateReporter.hpp"

class ApplyLEDControlTask : public Task
{
public:
    inline const static std::string LED_CONTROL_MESSAGE_GROUP = "ledcontrol";
    inline const static std::string TASK_NAME = "ApplyLEDControlTask";

    ApplyLEDControlTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 100;
private:
    DifferentialUpdateReporter mDiffReporter;
    google::protobuf::util::MessageDifferencer mDiff;

    void clearAllAnimations(uint16_t id);
    void updateColor(const ck::LEDControl::LEDControlData& m);
    void updateAnimation(const ck::LEDControl::LEDControlData& m);

    void processLEDUpdate(const google::protobuf::Message &msg);

    bool fullUpdate(ck::LEDControl::LEDControlData& m);
    void initFieldDescriptors();
    void initUpdateFunctions();

    ctre::phoenix::led::Animation* animationLookup(const ck::LEDAnimation& a);

    google::protobuf::FieldDescriptor* LED_TYPE_FD;
    google::protobuf::FieldDescriptor* VBAT_CONFIG_FD;
    google::protobuf::FieldDescriptor* VBAT_DUTY_CYCLE_FD;
    google::protobuf::FieldDescriptor* LED_CONTROL_MODE_FD;
    google::protobuf::FieldDescriptor* COLOR_FD;
    google::protobuf::FieldDescriptor* ANIMATION_FD;
};