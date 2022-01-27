#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "Task.hpp"
#include <stdint.h>
#include "IntellisenseDefs.h"
#include "MotorConfiguration.pb.h"
#include "google/protobuf/util/message_differencer.h"
#include "utils/DifferentialUpdateReporter.hpp"

class ApplyMotorConfigTask : public Task
{
public:
    inline const static std::string MOTOR_CONFIG_MESSAGE_GROUP = "motorconfig";
    inline const static std::string TASK_NAME = "ApplyMotorConfigTask";

    ApplyMotorConfigTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 100;
private:
    DifferentialUpdateReporter mDiffReporter;
    google::protobuf::util::MessageDifferencer mDiff;

    void fullUpdate(ck::MotorConfiguration &motorMsg);
    void initFieldDescriptors();
    void initUpdateFunctions();

    struct StatusFrameConfig
    {
        int CONTROL_FRAME_PERIOD_MS;
		int STATUS_FRAME_GENERAL_1_MS;
		int STATUS_FRAME_FEEDBACK0_2_MS;
    };
    static constexpr StatusFrameConfig FAST_MASTER_CONFIG = {5, 5, 20};
    static constexpr StatusFrameConfig NORMAL_MASTER_CONFIG = {10, 10, 20};
    static constexpr StatusFrameConfig SLAVE_CONFIG = {255, 255, 255};

    google::protobuf::FieldDescriptor* ID_FD;
    google::protobuf::FieldDescriptor* CONTROLLER_TYPE_FD;
    google::protobuf::FieldDescriptor* CONTROLLER_MODE_FD;
    google::protobuf::FieldDescriptor* KP_FD;
    google::protobuf::FieldDescriptor* KI_FD;
    google::protobuf::FieldDescriptor* KD_FD;
    google::protobuf::FieldDescriptor* KF_FD;
    google::protobuf::FieldDescriptor* IZONE_FD;
    google::protobuf::FieldDescriptor* MAX_I_ACCUM_FD;
    google::protobuf::FieldDescriptor* ALLOWED_CLOSED_LOOP_ERROR_FD;
    google::protobuf::FieldDescriptor* MAX_CLOSED_LOOP_PEAK_OUTPUT_FD;
    google::protobuf::FieldDescriptor* MOTION_CRUISE_VELOCITY_FD;
    google::protobuf::FieldDescriptor* MOTION_ACCELERATION_FD;
    google::protobuf::FieldDescriptor* MOTION_S_CURVE_STRENGTH_FD;
    google::protobuf::FieldDescriptor* FORWARD_SOFT_LIMIT_FD;
    google::protobuf::FieldDescriptor* FORWARD_SOFT_LIMIT_ENABLE_FD;
    google::protobuf::FieldDescriptor* REVERSE_SOFT_LIMIT_FD;
    google::protobuf::FieldDescriptor* REVERSE_SOFT_LIMIT_ENABLE_FD;
    google::protobuf::FieldDescriptor* FEEDBACK_SENSOR_COEFFICIENT_FD;
    google::protobuf::FieldDescriptor* VOLTAGE_COMPENSATION_SATURATION_FD;
    google::protobuf::FieldDescriptor* VOLTAGE_COMPENSATION_ENABLED_FD;
    google::protobuf::FieldDescriptor* INVERT_TYPE_FD;
    google::protobuf::FieldDescriptor* SENSOR_PHASE_INVERTED_FD;
    google::protobuf::FieldDescriptor* NEUTRAL_MODE_FD;
    google::protobuf::FieldDescriptor* OPEN_LOOP_RAMP_FD;
    google::protobuf::FieldDescriptor* CLOSED_LOOP_RAMP_FD;
    google::protobuf::FieldDescriptor* SUPPLY_CURRENT_LIMIT_CONFIG_FD;
    google::protobuf::FieldDescriptor* STATOR_CURRENT_LIMIT_CONFIG_FD;

};