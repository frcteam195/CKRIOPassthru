#include "tasks/ApplyMotorConfigTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <map>
#include <vector>
#include "DataStructures.hpp"
#include "MotorManager.hpp"
#include "ctre/Phoenix.h"
#include "NetworkManager.hpp"
#include "utils/PhoenixHelper.hpp"

ApplyMotorConfigTask::ApplyMotorConfigTask() : Task(THREAD_RATE_MS, TASK_NAME)
{
    NetworkManager::getInstance().joinGroup(MOTOR_CONFIG_MESSAGE_GROUP.c_str());
}

void ApplyMotorConfigTask::run(uint32_t timeSinceLastUpdateMs)
{
    //TODO: Improve memory efficiency
    std::vector<uint8_t> buf;
    if (NetworkManager::getInstance().getMessage(MOTOR_CONFIG_MESSAGE_GROUP, buf))
    {
        ck::MotorConfiguration motorsUpdate;
        motorsUpdate.ParseFromArray(&buf[0], buf.size());

        //https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.util.message_differencer#MessageDifferencer.Reporter
        //Maybe look into reportdifferencesto function
        if (!google::protobuf::util::MessageDifferencer::Equivalent(motorsUpdate, mPrevMotorsMsg))
        {
            for (ck::MotorConfiguration_Motor const& m : motorsUpdate.motors())
            {
                MotorManager::getInstance().registerMotor(m.id(), (MotorType)m.controller_type());
                //TODO: Implement per command differential set only if value is changed
                ck::MotorConfiguration_Motor_MotorControllerConfiguration config = m.motor_configuration();
                MotorManager::getInstance().onMotor(m.id(), [&] (uint16_t id, BaseMotorController* mCtrl, MotorType mType)
                {
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kP(0, config.kp(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kI(0, config.ki(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kD(0, config.kd(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kF(0, config.kf(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_IntegralZone(0, config.izone(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigMaxIntegralAccumulator(0, config.max_i_accum(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigAllowableClosedloopError(0, config.allowed_closed_loop_error(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigClosedLoopPeakOutput(0, config.max_closed_loop_peak_output(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigMotionCruiseVelocity(config.motion_cruise_velocity(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigMotionAcceleration(config.motion_acceleration(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigMotionSCurveStrength(config.motion_s_curve_strength(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigForwardSoftLimitThreshold(config.forward_soft_limit(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigForwardSoftLimitEnable(config.forward_soft_limit_enable(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigReverseSoftLimitThreshold(config.reverse_soft_limit(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigReverseSoftLimitEnable(config.reverse_soft_limit_enable(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigSelectedFeedbackCoefficient(config.feedback_sensor_coefficient(), 0, ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigVoltageCompSaturation(config.voltage_compensation_saturation(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { mCtrl->EnableVoltageCompensation(config.voltage_compensation_enabled()); return mCtrl->GetLastError(); }, id);
                    ck::runTalonFunctionWithRetry([&]() { mCtrl->SetInverted((InvertType)config.invert_type()); return mCtrl->GetLastError(); }, id);
                    ck::runTalonFunctionWithRetry([&]() { mCtrl->SetSensorPhase(config.sensor_phase_inverted()); return mCtrl->GetLastError(); }, id);
                    ck::runTalonFunctionWithRetry([&]() { mCtrl->SetNeutralMode((NeutralMode)config.neutral_mode()); return mCtrl->GetLastError(); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigOpenloopRamp(config.open_loop_ramp(), ck::kCANTimeoutMs); }, id);
                    ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigClosedloopRamp(config.closed_loop_ramp(), ck::kCANTimeoutMs); }, id);
                    
                    const ck::MotorConfiguration_Motor_MotorControllerConfiguration_CurrentLimitConfiguration* supplyCurrLimConfig = &config.supply_current_limit_config();
                    
                    switch (mType)
                    {
                    case MotorType::TALON_FX:
                    {
                        const ck::MotorConfiguration_Motor_MotorControllerConfiguration_CurrentLimitConfiguration* statorCurrLimConfig = &config.stator_current_limit_config();
                        TalonFX* tfx = dynamic_cast<TalonFX*>(mCtrl);
                        ck::runTalonFunctionWithRetry([&]() { return tfx->ConfigSupplyCurrentLimit(SupplyCurrentLimitConfiguration(supplyCurrLimConfig->enable(), supplyCurrLimConfig->current_limit(), supplyCurrLimConfig->trigger_threshold_current(), supplyCurrLimConfig->trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
                        ck::runTalonFunctionWithRetry([&]() { return tfx->ConfigStatorCurrentLimit(StatorCurrentLimitConfiguration(statorCurrLimConfig->enable(), statorCurrLimConfig->current_limit(), statorCurrLimConfig->trigger_threshold_current(), statorCurrLimConfig->trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
                    }
                        break;
                    case MotorType::TALON_SRX:
                    {
                        TalonSRX* tsrx = dynamic_cast<TalonSRX*>(mCtrl);
                        ck::runTalonFunctionWithRetry([&]() { return tsrx->ConfigSupplyCurrentLimit(SupplyCurrentLimitConfiguration(supplyCurrLimConfig->enable(), supplyCurrLimConfig->current_limit(), supplyCurrLimConfig->trigger_threshold_current(), supplyCurrLimConfig->trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
                    }
                        break;
                    default:
                        break;
                    }

                    if (config.fast_master())
                    {

                    }
                    
                });
            }
        }

        mPrevMotorsMsg = motorsUpdate;
    }
    mTaskTimer.reportElapsedTime();
}