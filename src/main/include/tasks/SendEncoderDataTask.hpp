#pragma once

#include <vector>
#include "Task.hpp"
#include <stdint.h>
#include "utils/RTTimer.hpp"
#include "IntellisenseDefs.h"
#include "EncoderData.pb.h"
#include <string>

class SendEncoderDataTask : public Task
{
public:
    inline const static std::string ENCODER_DATA_MESSAGE_GROUP = "encoderdata";
    inline const static std::string TASK_NAME = "SendEncoderDataTask";

    SendEncoderDataTask();
    ~SendEncoderDataTask();
    RTTimer rtTimer;
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 10;
private:
    ck::EncoderData mEncoderData;
    const size_t ENCODER_DATA_MESSAGE_SIZE = 1500;
    void* mEncoderDataBuf;
};