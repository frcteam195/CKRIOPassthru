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

    void updateColor(uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& m);
    void updateAnimation(uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& m);

    void processLEDUpdate(const ck::LEDControl::LEDControlData& msg);

    bool fullUpdate(ck::LEDControl::LEDControlData& m);
    void initFieldDescriptors();
    void initUpdateFunctions();


    std::map<uint16_t, ck::LEDControl_LEDControlData_LEDControlMode> mCurrLEDCtrlMode;

    google::protobuf::FieldDescriptor* LED_TYPE_FD;
    google::protobuf::FieldDescriptor* VBAT_CONFIG_FD;
    google::protobuf::FieldDescriptor* VBAT_DUTY_CYCLE_FD;
    google::protobuf::FieldDescriptor* LED_CONTROL_MODE_FD;
    google::protobuf::FieldDescriptor* COLOR_FD;
    google::protobuf::FieldDescriptor* ANIMATION_FD;
};