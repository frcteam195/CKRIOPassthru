#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "Task.hpp"
#include <stdint.h>
#include <map>
#include "IntellisenseDefs.h"
#include "EncoderConfig.pb.h"
#include "google/protobuf/util/message_differencer.h"

class ApplyEncoderConfigTask : public Task
{
public:
    inline const static std::string ENCODER_CONFIG_MESSAGE_GROUP = "encoderconfig";
    inline const static std::string TASK_NAME = "ApplyEncoderConfigTask";

    ApplyEncoderConfigTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 100;

private:
    std::map<uint16_t, ck::EncoderConfig::EncoderConfigData> mPrevEncoderMsg;
};