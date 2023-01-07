#include "tasks/ApplyEncoderConfigTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <map>
#include <vector>
#include "utils/PhoenixHelper.hpp"
#include "CKEncoderManager.hpp"
#include "utils/drivers/CKPigeon2.hpp"
#include "NetworkManager.hpp"
#include "utils/drivers/CKCANCoder.hpp"
#include <iostream>

ApplyEncoderConfigTask::ApplyEncoderConfigTask() : Task(THREAD_RATE_MS, TASK_NAME)
{
    NetworkManager::getInstance().joinGroup(ENCODER_CONFIG_MESSAGE_GROUP.c_str());
}

void ApplyEncoderConfigTask::run(uint32_t timeSinceLastUpdateMs)
{
    mTaskTimer.start();
    //TODO: Improve memory efficiency
    std::vector<uint8_t> buf;
    if (NetworkManager::getInstance().getMessage(ENCODER_CONFIG_MESSAGE_GROUP, buf))
    {
        ck::EncoderConfig encoderUpdate;
        encoderUpdate.ParseFromArray(&buf[0], buf.size());

        // std::cout << "Parsed IMU Config" << std::endl;

        for (auto encoder_config_msg : encoderUpdate.encoder_config())
        {
            bool success = true;
            CKEncoderManager::getInstance().registerEncoder(encoder_config_msg.id(), (EncoderType)encoder_config_msg.encoder_type(), (CANInterface)encoder_config_msg.can_network());
            if (!google::protobuf::util::MessageDifferencer::Equivalent(encoder_config_msg, mPrevEncoderMsg[encoder_config_msg.id()]))
            {
                std::cout << "Encoder Init Config" << std::endl;
                CKEncoderManager::getInstance().onEncoder(encoder_config_msg.id(), [&](uint16_t id, CKEncoder* encoder, EncoderType encoder_type)
                {
                    success &= encoder->configSensorSource((EncoderSensorSource)encoder_config_msg.sensor_source());
                    success &= encoder->configInitializationStrategy((InitializationStrategy) encoder_config_msg.initialization_strategy());
                    // success &= encoder->reset();
                    switch (encoder_type)
                    {
                        case EncoderType::CANCoder:
                        {
                            CKCANCoder* ckcancoder = dynamic_cast<CKCANCoder*>(encoder);
                            CANCoder cancoder = ckcancoder->getRawCANCoder();
                            cancoder.ConfigMagnetOffset(encoder_config_msg.magnetic_offset());
                            break;
                        }
                    }
                });
            }

            if (success)
            {
                mPrevEncoderMsg[encoder_config_msg.id()] = encoder_config_msg;
            }
        }
    }

    mTaskTimer.reportElapsedTime();
}