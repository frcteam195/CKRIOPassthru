#include "tasks/ApplySolenoidValuesTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <map>
#include <vector>
#include "NetworkManager.hpp"
#include "SolenoidManager.hpp"
#include "utils/RobotControlModeHelper.hpp"

ApplySolenoidValuesTask::ApplySolenoidValuesTask() : Task(THREAD_RATE_MS, TASK_NAME)
{
    NetworkManager::getInstance().joinGroup(SOLENOID_CONTROL_MESSAGE_GROUP.c_str());
}

ApplySolenoidValuesTask::~ApplySolenoidValuesTask()
{
    if (mCompressor)
    {
        delete mCompressor;
    }
    mCompressor = nullptr;
}

void ApplySolenoidValuesTask::run(uint32_t timeSinceLastUpdateMs)
{
    mTaskTimer.start();
    //TODO: Improve memory efficiency
    std::vector<uint8_t> buf;
    if (NetworkManager::getInstance().getMessage(SOLENOID_CONTROL_MESSAGE_GROUP, buf))
    {
        ck::SolenoidControl solenoidUpdate;
        if (solenoidUpdate.ParseFromArray(&buf[0], buf.size()))
        {
            if (solenoidUpdate.solenoids_size() > 0)
            {
                if (mCompressor == nullptr)
                {
                    mCompressor = new frc::Compressor(0, frc::PneumaticsModuleType::CTREPCM);
                    mCompressor->Disable();
                    mCompressorState = false;
                }

                if (!solenoidUpdate.compressor_is_enabled_for_auto() && RobotControlModeHelper::getInstance().getControlMode() != CONTROL_MODE::TELEOP)
                {
                    if (mCompressorState)
                    {
                        mCompressor->Disable();
                        mCompressorState = false;
                    }
                }
                else
                {
                    if (!mCompressorState)
                    {
                        mCompressor->EnableDigital();
                        mCompressorState = true;
                    }
                }
            }

            //https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.util.message_differencer#MessageDifferencer.Reporter
            //Maybe look into reportdifferencesto function
            if (!google::protobuf::util::MessageDifferencer::Equivalent(solenoidUpdate, mPrevSolenoidMsg) || (rtTimer.hasElapseduS() > kMandatoryUpdatePerioduS && MANDATORY_UPDATE_ENABLED))
            {
                for (ck::SolenoidControl::Solenoid const& s : solenoidUpdate.solenoids())
                {
                    SolenoidManager::getInstance().registerSolenoid(s.module_type(), s.id(), s.solenoid_type());
                    SolenoidManager::getInstance().onSolenoid(s.id(), [&] (uint16_t id, CKSolenoid* sCtrl, ck::SolenoidControl::Solenoid::SolenoidType sType)
                    {
                        sCtrl->set(s.output_value());
                    });
                }
                rtTimer.start();
            }
        }
        mPrevSolenoidMsg = solenoidUpdate;
    }
    mTaskTimer.reportElapsedTime();
}