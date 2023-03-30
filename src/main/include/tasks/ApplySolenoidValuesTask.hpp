#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "Task.hpp"
#include <stdint.h>
#include "IntellisenseDefs.h"
#include "SolenoidControl.pb.h"
#include "google/protobuf/util/message_differencer.h"
#include "utils/RTTimer.hpp"
#include "frc/Compressor.h"

class ApplySolenoidValuesTask : public Task
{
public:
    inline const static std::string SOLENOID_CONTROL_MESSAGE_GROUP = "solenoidcontrol";
    inline const static std::string TASK_NAME = "ApplySolenoidValuesTask";

    ApplySolenoidValuesTask();
    ~ApplySolenoidValuesTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 20;
private:
    ck::SolenoidControl mPrevSolenoidMsg;
    RTTimer rtTimer;
    frc::Compressor* mCompressor;
    bool mCompressorState = false;
    static constexpr unsigned int kMandatoryUpdatePerioduS = 200000;
    static constexpr bool MANDATORY_UPDATE_ENABLED = true;
};