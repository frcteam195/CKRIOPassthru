#include "tasks/SendEncoderDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include "NetworkManager.hpp"
#include <vector>
#include <iostream>
#include "google/protobuf/util/json_util.h"
#include "utils/GlobalConfig.hpp"
#include <string>
#include "CKEncoderManager.hpp"

SendEncoderDataTask::SendEncoderDataTask() : Task(THREAD_RATE_MS, TASK_NAME), mEncoderData()
{
    mEncoderDataBuf = malloc(ENCODER_DATA_MESSAGE_SIZE * sizeof(uint8_t));
    memset(mEncoderDataBuf, 0, ENCODER_DATA_MESSAGE_SIZE * sizeof(uint8_t));
}

SendEncoderDataTask::~SendEncoderDataTask()
{
    free(mEncoderDataBuf);
}

void SendEncoderDataTask::run(uint32_t timeSinceLastUpdateMs)
{
    mTaskTimer.start();
// #ifdef CONSOLE_REPORTING
//     static int count = 0;
// #endif

    mEncoderData.Clear();

    CKEncoderManager::getInstance().forEach([&](uint16_t id, CKEncoder* encoder, EncoderType encoderType)
    {
        ck::EncoderData_EncoderSensorData *encoderSensorData = mEncoderData.add_encoder_sensor();
        encoderSensorData->set_id(id);
        encoderSensorData->set_sensor_position(encoder->getPosition());
        encoderSensorData->set_sensor_velocity(encoder->getVelocity());
        encoderSensorData->set_is_faulted(encoder->isFaulted());
    });

    if (mEncoderData.SerializeToArray(mEncoderDataBuf, mEncoderData.ByteSizeLong()))
    {
        NetworkManager::getInstance().sendMessage(ENCODER_DATA_MESSAGE_GROUP, mEncoderDataBuf, mEncoderData.ByteSizeLong());
    }
    else
    {
        std::cout << "Encoder data message failed to serialize. Message probably too large or invalid." << std::endl;
    }

// #ifdef CONSOLE_REPORTING
//     if (count++ % 500 == 0)
//     {
//         std::string s;
//         google::protobuf::util::MessageToJsonString(mIMUData, &s);
//         std::cout << s << std::endl;
//     }
// #endif
    mTaskTimer.reportElapsedTime();
}