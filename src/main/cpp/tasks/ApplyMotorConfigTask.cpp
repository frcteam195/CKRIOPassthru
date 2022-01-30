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
#include "MotorConfigManager.hpp"
#include <iostream>

ApplyMotorConfigTask::ApplyMotorConfigTask() : Task(THREAD_RATE_MS, TASK_NAME)
{
    NetworkManager::getInstance().joinGroup(MOTOR_CONFIG_MESSAGE_GROUP.c_str());
    initFieldDescriptors();
    initUpdateFunctions();
    mDiff.set_report_matches(false);
    mDiff.set_report_moves(false);
    mDiff.ReportDifferencesTo(&mDiffReporter);
}

void ApplyMotorConfigTask::run(uint32_t timeSinceLastUpdateMs)
{
    //TODO: Improve memory efficiency
    std::vector<uint8_t> buf;
    if (NetworkManager::getInstance().getMessage(MOTOR_CONFIG_MESSAGE_GROUP, buf))
    {
        ck::MotorConfiguration motorsUpdate;
        motorsUpdate.ParseFromArray(&buf[0], buf.size());
        if (MotorConfigManager::getInstance().try_lock())
        {
            ck::MotorConfiguration& mPrevMotorsConfigMsg = MotorConfigManager::getInstance().getPrevMotorsConfigMsg();
            if (!google::protobuf::util::MessageDifferencer::Equivalent(motorsUpdate, mPrevMotorsConfigMsg))
            {
                if (motorsUpdate.motors().size() == mPrevMotorsConfigMsg.motors().size())
                {
                    for (int i = 0; i < motorsUpdate.motors().size(); i++)
                    {
                        const ck::MotorConfiguration::Motor &m = motorsUpdate.motors()[i];
                        const ck::MotorConfiguration::Motor &mPrev = mPrevMotorsConfigMsg.motors()[i];

                        if (m.id() != mPrev.id())
                        {
                            //Make sure all motors are fully initiallized if array size stays the same but IDs change
                            fullUpdate(motorsUpdate);
                            break;
                        }

                        //If we're here motor should be already registered? Needs testing
                        //MotorManager::getInstance().registerMotor(m.id(), (MotorType)m.controller_type());
                        mDiff.Compare(mPrev, m);  //Make sure the current update is message2 for our implementation
                    }
                }
                else
                {
                    fullUpdate(motorsUpdate);
                }
            }

            //https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.util.message_differencer#MessageDifferencer.Reporter
            //Maybe look into reportdifferencesto function
            
            /*
            if (!google::protobuf::util::MessageDifferencer::Equivalent(motorsUpdate, mPrevMotorsMsg))
            {
                fullUpdate(motorsUpdate);
            }
            */

            MotorConfigManager::getInstance().setPrevMotorsConfigMsg(motorsUpdate);
            MotorConfigManager::getInstance().unlock();
        }
    }
    mTaskTimer.reportElapsedTime();
}

void ApplyMotorConfigTask::fullUpdate(ck::MotorConfiguration &motorMsg)
{
    for (ck::MotorConfiguration_Motor const& m : motorMsg.motors())
    {
        MotorManager::getInstance().registerMotor(m.id(), (MotorType)m.controller_type());
        //TODO: Implement per command differential set only if value is changed
        MotorManager::getInstance().onMotor(m.id(), [&] (uint16_t id, BaseMotorController* mCtrl, MotorType mType)
        {
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigFactoryDefault(); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ClearStickyFaults(); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kP(0, m.kp(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kI(0, m.ki(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kD(0, m.kd(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_kF(0, m.kf(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->Config_IntegralZone(0, m.izone(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigMaxIntegralAccumulator(0, m.max_i_accum(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigAllowableClosedloopError(0, m.allowed_closed_loop_error(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigClosedLoopPeakOutput(0, m.max_closed_loop_peak_output() < 0.01 ? 1 : m.max_closed_loop_peak_output(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigMotionCruiseVelocity(m.motion_cruise_velocity(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigMotionAcceleration(m.motion_acceleration(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigMotionSCurveStrength(m.motion_s_curve_strength(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigForwardSoftLimitThreshold(m.forward_soft_limit(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigForwardSoftLimitEnable(m.forward_soft_limit_enable(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigReverseSoftLimitThreshold(m.reverse_soft_limit(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigReverseSoftLimitEnable(m.reverse_soft_limit_enable(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigSelectedFeedbackCoefficient(m.feedback_sensor_coefficient(), 0, ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigVoltageCompSaturation(m.voltage_compensation_saturation(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { mCtrl->EnableVoltageCompensation(m.voltage_compensation_enabled()); return mCtrl->GetLastError(); }, id);
            ck::runTalonFunctionWithRetry([&]() { mCtrl->SetInverted((InvertType)m.invert_type()); return mCtrl->GetLastError(); }, id);
            ck::runTalonFunctionWithRetry([&]() { mCtrl->SetSensorPhase(m.sensor_phase_inverted()); return mCtrl->GetLastError(); }, id);
            ck::runTalonFunctionWithRetry([&]() { mCtrl->SetNeutralMode((NeutralMode)m.neutral_mode()); return mCtrl->GetLastError(); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigOpenloopRamp(m.open_loop_ramp(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigClosedloopRamp(m.closed_loop_ramp(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigForwardLimitSwitchSource((ctre::phoenix::motorcontrol::LimitSwitchSource)m.forward_limit_switch_source(), (ctre::phoenix::motorcontrol::LimitSwitchNormal)m.forward_limit_switch_normal(), ck::kCANTimeoutMs); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->ConfigReverseLimitSwitchSource((ctre::phoenix::motorcontrol::LimitSwitchSource)m.reverse_limit_switch_source(), (ctre::phoenix::motorcontrol::LimitSwitchNormal)m.reverse_limit_switch_normal(), ck::kCANTimeoutMs); }, id);
            const ck::MotorConfiguration_Motor_CurrentLimitConfiguration &supplyCurrLimConfig = m.supply_current_limit_config();
            
            switch (mType)
            {
            case MotorType::TALON_FX:
            {
                const ck::MotorConfiguration_Motor_CurrentLimitConfiguration &statorCurrLimConfig = m.stator_current_limit_config();
                TalonFX* tfx = dynamic_cast<TalonFX*>(mCtrl);
                ck::runTalonFunctionWithRetry([&]() { return tfx->ConfigSupplyCurrentLimit(SupplyCurrentLimitConfiguration(supplyCurrLimConfig.enable(), supplyCurrLimConfig.current_limit(), supplyCurrLimConfig.trigger_threshold_current(), supplyCurrLimConfig.trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
                ck::runTalonFunctionWithRetry([&]() { return tfx->ConfigStatorCurrentLimit(StatorCurrentLimitConfiguration(statorCurrLimConfig.enable(), statorCurrLimConfig.current_limit(), statorCurrLimConfig.trigger_threshold_current(), statorCurrLimConfig.trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
            }
                break;
            case MotorType::TALON_SRX:
            {
                TalonSRX* tsrx = dynamic_cast<TalonSRX*>(mCtrl);
                ck::runTalonFunctionWithRetry([&]() { return tsrx->ConfigSupplyCurrentLimit(SupplyCurrentLimitConfiguration(supplyCurrLimConfig.enable(), supplyCurrLimConfig.current_limit(), supplyCurrLimConfig.trigger_threshold_current(), supplyCurrLimConfig.trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
            }
                break;
            default:
                break;
            }

            switch (m.controller_mode())
            {
            case ck::MotorConfiguration_Motor_ControllerMode::MotorConfiguration_Motor_ControllerMode_FAST_MASTER:
            {
                ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetControlFramePeriod(ControlFrame::Control_3_General, FAST_MASTER_CONFIG.CONTROL_FRAME_PERIOD_MS); }, id);
                ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_1_General_, FAST_MASTER_CONFIG.STATUS_FRAME_GENERAL_1_MS, ck::kCANTimeoutMs); }, id);
                ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_2_Feedback0_, FAST_MASTER_CONFIG.STATUS_FRAME_FEEDBACK0_2_MS, ck::kCANTimeoutMs); }, id);
            }
                break;
            case ck::MotorConfiguration_Motor_ControllerMode::MotorConfiguration_Motor_ControllerMode_SLAVE:
            {
                ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetControlFramePeriod(ControlFrame::Control_3_General, SLAVE_CONFIG.CONTROL_FRAME_PERIOD_MS); }, id);
                ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_1_General_, SLAVE_CONFIG.STATUS_FRAME_GENERAL_1_MS, ck::kCANTimeoutMs); }, id);
                ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_2_Feedback0_, SLAVE_CONFIG.STATUS_FRAME_FEEDBACK0_2_MS, ck::kCANTimeoutMs); }, id);
            }
                break;
            case ck::MotorConfiguration_Motor_ControllerMode::MotorConfiguration_Motor_ControllerMode_MASTER:
            default:
            {
                ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetControlFramePeriod(ControlFrame::Control_3_General, NORMAL_MASTER_CONFIG.CONTROL_FRAME_PERIOD_MS); }, id);
                ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_1_General_, NORMAL_MASTER_CONFIG.STATUS_FRAME_GENERAL_1_MS, ck::kCANTimeoutMs); }, id);
                ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_2_Feedback0_, NORMAL_MASTER_CONFIG.STATUS_FRAME_FEEDBACK0_2_MS, ck::kCANTimeoutMs); }, id);
            }
                break;
            }
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_4_AinTempVbat_, 255); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_6_Misc_, 255); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_7_CommStatus_, 255); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_10_MotionMagic_, 255); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_12_Feedback1_, 255); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_13_Base_PIDF0_, 255); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_14_Turn_PIDF1_, 255); }, id);
            ck::runTalonFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_17_Targets1_, 255); }, id);
        });
    }
}

void ApplyMotorConfigTask::initFieldDescriptors()
{
    ID_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(1);
    CONTROLLER_TYPE_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(2);
    CONTROLLER_MODE_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(3);
    KP_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(4);
    KI_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(5);
    KD_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(6);
    KF_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(7);
    IZONE_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(8);
    MAX_I_ACCUM_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(9);
    ALLOWED_CLOSED_LOOP_ERROR_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(10);
    MAX_CLOSED_LOOP_PEAK_OUTPUT_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(11);
    MOTION_CRUISE_VELOCITY_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(12);
    MOTION_ACCELERATION_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(13);
    MOTION_S_CURVE_STRENGTH_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(14);
    FORWARD_SOFT_LIMIT_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(15);
    FORWARD_SOFT_LIMIT_ENABLE_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(16);
    REVERSE_SOFT_LIMIT_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(17);
    REVERSE_SOFT_LIMIT_ENABLE_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(18);
    FEEDBACK_SENSOR_COEFFICIENT_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(19);
    VOLTAGE_COMPENSATION_SATURATION_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(20);
    VOLTAGE_COMPENSATION_ENABLED_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(21);
    INVERT_TYPE_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(22);
    SENSOR_PHASE_INVERTED_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(23);
    NEUTRAL_MODE_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(24);
    OPEN_LOOP_RAMP_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(25);
    CLOSED_LOOP_RAMP_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(26);
    SUPPLY_CURRENT_LIMIT_CONFIG_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(27);
    STATOR_CURRENT_LIMIT_CONFIG_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(28);
    FORWARD_LIMIT_SWITCH_SOURCE_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(29);
    FORWARD_LIMIT_SWITCH_NORMAL_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(30);
    REVERSE_LIMIT_SWITCH_SOURCE_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(31);
    REVERSE_LIMIT_SWITCH_NORMAL_FD = (google::protobuf::FieldDescriptor*)ck::MotorConfiguration::Motor::GetDescriptor()->FindFieldByNumber(32);
}

void ApplyMotorConfigTask::initUpdateFunctions()
{
    mDiffReporter.RegisterUpdateFunction(KP_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->Config_kP(0, m.kp(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(KI_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->Config_kI(0, m.ki(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(KD_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->Config_kD(0, m.kd(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(KF_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->Config_kF(0, m.kf(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(IZONE_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->Config_IntegralZone(0, m.izone(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(MAX_I_ACCUM_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigMaxIntegralAccumulator(0, m.max_i_accum(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(ALLOWED_CLOSED_LOOP_ERROR_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigAllowableClosedloopError(0, m.allowed_closed_loop_error(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(MAX_CLOSED_LOOP_PEAK_OUTPUT_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigClosedLoopPeakOutput(0, m.max_closed_loop_peak_output(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(MOTION_CRUISE_VELOCITY_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigMotionCruiseVelocity(m.motion_cruise_velocity(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(MOTION_ACCELERATION_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigMotionAcceleration(m.motion_acceleration(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(MOTION_S_CURVE_STRENGTH_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigMotionSCurveStrength(m.motion_s_curve_strength(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(FORWARD_SOFT_LIMIT_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigForwardSoftLimitThreshold(m.forward_soft_limit(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(FORWARD_SOFT_LIMIT_ENABLE_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigForwardSoftLimitEnable(m.forward_soft_limit_enable(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(REVERSE_SOFT_LIMIT_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigReverseSoftLimitThreshold(m.reverse_soft_limit(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(REVERSE_SOFT_LIMIT_ENABLE_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigReverseSoftLimitEnable(m.reverse_soft_limit_enable(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(FEEDBACK_SENSOR_COEFFICIENT_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigSelectedFeedbackCoefficient(m.feedback_sensor_coefficient(), 0, ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(VOLTAGE_COMPENSATION_SATURATION_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigVoltageCompSaturation(m.voltage_compensation_saturation(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(VOLTAGE_COMPENSATION_ENABLED_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { mCtrl->EnableVoltageCompensation(m.voltage_compensation_enabled()); return mCtrl->GetLastError(); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(INVERT_TYPE_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { mCtrl->SetInverted((InvertType)m.invert_type()); return mCtrl->GetLastError(); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(SENSOR_PHASE_INVERTED_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { mCtrl->SetSensorPhase(m.sensor_phase_inverted()); return mCtrl->GetLastError(); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(NEUTRAL_MODE_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { mCtrl->SetNeutralMode((NeutralMode)m.neutral_mode()); return mCtrl->GetLastError(); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(OPEN_LOOP_RAMP_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigOpenloopRamp(m.open_loop_ramp(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(CLOSED_LOOP_RAMP_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigClosedloopRamp(m.closed_loop_ramp(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(SUPPLY_CURRENT_LIMIT_CONFIG_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            {
                const ck::MotorConfiguration_Motor_CurrentLimitConfiguration &supplyCurrLimConfig = m.supply_current_limit_config();
                switch (mType)
                {
                case MotorType::TALON_FX:
                {
                    TalonFX* tfx = dynamic_cast<TalonFX*>(mCtrl);
                    ck::runTalonFunctionWithRetry([mCtrl, supplyCurrLimConfig, tfx]() { return tfx->ConfigSupplyCurrentLimit(SupplyCurrentLimitConfiguration(supplyCurrLimConfig.enable(), supplyCurrLimConfig.current_limit(), supplyCurrLimConfig.trigger_threshold_current(), supplyCurrLimConfig.trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
                }
                    break;
                case MotorType::TALON_SRX:
                {
                    TalonSRX* tsrx = dynamic_cast<TalonSRX*>(mCtrl);
                    ck::runTalonFunctionWithRetry([mCtrl, supplyCurrLimConfig, tsrx]() { return tsrx->ConfigSupplyCurrentLimit(SupplyCurrentLimitConfiguration(supplyCurrLimConfig.enable(), supplyCurrLimConfig.current_limit(), supplyCurrLimConfig.trigger_threshold_current(), supplyCurrLimConfig.trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
                }
                    break;
                default:
                    break;
                }
            });
    });

    mDiffReporter.RegisterUpdateFunction(STATOR_CURRENT_LIMIT_CONFIG_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { 
                const ck::MotorConfiguration_Motor_CurrentLimitConfiguration &statorCurrLimConfig = m.stator_current_limit_config();
                switch (mType)
                {
                case MotorType::TALON_FX:
                {
                    TalonFX* tfx = dynamic_cast<TalonFX*>(mCtrl);
                    ck::runTalonFunctionWithRetry([mCtrl, statorCurrLimConfig, tfx]() { return tfx->ConfigStatorCurrentLimit(StatorCurrentLimitConfiguration(statorCurrLimConfig.enable(), statorCurrLimConfig.current_limit(), statorCurrLimConfig.trigger_threshold_current(), statorCurrLimConfig.trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
                }
                    break;
                default:
                    break;
                }
            });
    });

    mDiffReporter.RegisterUpdateFunction(FORWARD_LIMIT_SWITCH_SOURCE_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigForwardLimitSwitchSource((ctre::phoenix::motorcontrol::LimitSwitchSource)m.forward_limit_switch_source(), (ctre::phoenix::motorcontrol::LimitSwitchNormal)m.forward_limit_switch_normal(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(FORWARD_LIMIT_SWITCH_NORMAL_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigForwardLimitSwitchSource((ctre::phoenix::motorcontrol::LimitSwitchSource)m.forward_limit_switch_source(), (ctre::phoenix::motorcontrol::LimitSwitchNormal)m.forward_limit_switch_normal(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(REVERSE_LIMIT_SWITCH_SOURCE_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigReverseLimitSwitchSource((ctre::phoenix::motorcontrol::LimitSwitchSource)m.reverse_limit_switch_source(), (ctre::phoenix::motorcontrol::LimitSwitchNormal)m.reverse_limit_switch_normal(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(REVERSE_LIMIT_SWITCH_NORMAL_FD, [](const google::protobuf::Message &msg)
    {
        MotorManager::getInstance().onMotor(msg,
            [](uint16_t id, BaseMotorController* mCtrl, MotorType mType, const ck::MotorConfiguration::Motor& m)
            { ck::runTalonFunctionWithRetry([mCtrl, m]() { return mCtrl->ConfigReverseLimitSwitchSource((ctre::phoenix::motorcontrol::LimitSwitchSource)m.reverse_limit_switch_source(), (ctre::phoenix::motorcontrol::LimitSwitchNormal)m.reverse_limit_switch_normal(), ck::kCANTimeoutMs); }, id); });
    });
}