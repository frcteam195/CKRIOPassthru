#include "tasks/ApplyLEDControlTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include <map>
#include <vector>
#include "DataStructures.hpp"
#include "CANdleManager.hpp"
#include "ctre/Phoenix.h"
#include "NetworkManager.hpp"
#include "utils/PhoenixHelper.hpp"
#include <iostream>
#include "utils/CKLogger.hpp"

using namespace ctre::phoenix::led;


// #define DEBUG

ApplyLEDControlTask::ApplyLEDControlTask() : Task(THREAD_RATE_MS, TASK_NAME)
{
    NetworkManager::getInstance().joinGroup(LED_CONTROL_MESSAGE_GROUP.c_str());
    initFieldDescriptors();
    initUpdateFunctions();
    mDiff.set_report_matches(false);
    mDiff.set_report_moves(false);
    mDiff.ReportDifferencesTo(&mDiffReporter);
}

void ApplyLEDControlTask::run(uint32_t timeSinceLastUpdateMs)
{
    mTaskTimer.start();
    // TODO: Improve memory efficiency
    // std::vector<uint8_t> buf;
    // if (NetworkManager::getInstance().getMessage(LED_CONTROL_MESSAGE_GROUP, buf))
    // {
    //     ck::LEDControl motorsUpdate;
    //     motorsUpdate.ParseFromArray(&buf[0], buf.size());
    //     if (CANdleManager::getInstance().try_lock())
    //     {
    //         CANdleManager::getInstance().setCANdlesConfigMsg(motorsUpdate);
    //         std::map<uint16_t, ck::LEDControl::LEDControlData>& mPrevMotorsConfigMsgMap = CANdleManager::getInstance().getPrevMotorsConfigMsg();
    //         for (ck::LEDControl::LEDControlData m : motorsUpdate.motors())
    //         {
    //             bool updateSuccessful = true;
    //             if (CANdleManager::getInstance().motorExists(m.id()))
    //             {
    //                 if ((size_t)motorsUpdate.motors().size() == mPrevMotorsConfigMsgMap.size())
    //                 {
    //                     mDiff.Compare(mPrevMotorsConfigMsgMap[m.id()], m);  //Make sure the current update is message2 for our implementation
    //                 }
    //                 else
    //                 {
    //                     updateSuccessful = fullUpdate(m);
    //                 }

    //                 if (updateSuccessful)
    //                 {
    //                     CANdleManager::getInstance().setPrevMotorConfigMsg(m.id(), m);
    //                 }
    //             }
    //         }
    //         CANdleManager::getInstance().unlock();
    //     }
    // }
    mTaskTimer.reportElapsedTime();
}

bool ApplyLEDControlTask::fullUpdate(ck::LEDControl::LEDControlData& d)
{
    // ck::log("Full Config Start\n");
    // bool isMaster = m.controller_mode() != ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_SLAVE;

    // //MotorManager::getInstance().registerMotor(m.id(), (MotorType)m.controller_type());
    // //TODO: Implement per command differential set only if value is changed
    // MotorManager::getInstance().onMotor(m.id(), [&] (uint16_t id, BaseTalon* mCtrl, MotorType mType)
    // {
    //     // ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigFactoryDefault(); }, id);
    //     ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ClearStickyFaults(); }, id);
    //     ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigVoltageCompSaturation(m.voltage_compensation_saturation(), ck::kCANTimeoutMs); }, id);
    //     ck::runPhoenixFunctionWithRetry([&]() { mCtrl->EnableVoltageCompensation(m.voltage_compensation_enabled()); return mCtrl->GetLastError(); }, id);
    //     ck::runPhoenixFunctionWithRetry([&]() { mCtrl->SetInverted((InvertType)m.invert_type()); return mCtrl->GetLastError(); }, id);
    //     // ck::runPhoenixFunctionWithRetry([&]() { mCtrl->SetSensorPhase(m.sensor_phase_inverted()); return mCtrl->GetLastError(); }, id);
    //     ck::runPhoenixFunctionWithRetry([&]() { mCtrl->SetNeutralMode((NeutralMode)m.neutral_mode()); return mCtrl->GetLastError(); }, id);
    //     ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigOpenloopRamp(m.open_loop_ramp(), ck::kCANTimeoutMs); }, id);
    //     ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigClosedloopRamp(m.closed_loop_ramp(), ck::kCANTimeoutMs); }, id);
    //     ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigPeakOutputForward(m.peak_output_forward() < 0.04 ? 1.0 : m.peak_output_forward(), ck::kCANTimeoutMs); }, id);
    //     ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigPeakOutputReverse(m.peak_output_reverse() > -0.04 ? -1.0 : m.peak_output_reverse(), ck::kCANTimeoutMs); }, id);
    //     ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigForwardLimitSwitchSource((ctre::phoenix::motorcontrol::LimitSwitchSource)m.forward_limit_switch_source(), (ctre::phoenix::motorcontrol::LimitSwitchNormal)m.forward_limit_switch_normal(), ck::kCANTimeoutMs); }, id);
    //     ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigReverseLimitSwitchSource((ctre::phoenix::motorcontrol::LimitSwitchSource)m.reverse_limit_switch_source(), (ctre::phoenix::motorcontrol::LimitSwitchNormal)m.reverse_limit_switch_normal(), ck::kCANTimeoutMs); }, id);
        
    //     if (isMaster)
    //     {
    //         ck::runPhoenixFunctionWithRetry([&]() {
    //             if (m.feedback_sensor_can_id() > 0)
    //             {
    //                 bool success = true;
    //                 // CKEncoder* ckEncoder = CKEncoderManager::getInstance().getEncoder(m.feedback_sensor_can_id());
    //                 // CKCANCoder* ckCANCoder = dynamic_cast<CKCANCoder*>(ckEncoder);
    //                 // CANCoder canCoder = ckCANCoder->getRawCANCoder();
    //                 // success &= mCtrl->ConfigRemoteFeedbackFilter(canCoder, 0, ck::kCANTimeoutMs) == ErrorCode::OK;
    //                 std::cout << "Setting remote filter for motor ID: " << m.id() << std::endl;
    //                 success &= mCtrl->ConfigRemoteFeedbackFilter(m.feedback_sensor_can_id(), RemoteSensorSource::RemoteSensorSource_CANCoder, 0, ck::kCANTimeoutMs) == ErrorCode::OK;
    //                 success &= mCtrl->ConfigSelectedFeedbackSensor(RemoteFeedbackDevice::RemoteSensor0) == ErrorCode::OK;
    //                 return success ? ErrorCode::OK : ErrorCode::GeneralError;
    //             }
    //             else
    //             {
    //                 return mCtrl->ConfigSelectedFeedbackSensor(FeedbackDevice::IntegratedSensor);
    //             }
    //         }, id);

    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->Config_kP(0, m.kp(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->Config_kI(0, m.ki(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->Config_kD(0, m.kd(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->Config_kF(0, m.kf(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->Config_IntegralZone(0, m.izone(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigMaxIntegralAccumulator(0, m.max_i_accum(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigAllowableClosedloopError(0, m.allowed_closed_loop_error(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigClosedLoopPeakOutput(0, m.max_closed_loop_peak_output() < 0.01 ? 1 : m.max_closed_loop_peak_output(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigMotionCruiseVelocity(m.motion_cruise_velocity(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigMotionAcceleration(m.motion_acceleration(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigMotionSCurveStrength(m.motion_s_curve_strength(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigForwardSoftLimitThreshold(m.forward_soft_limit(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigForwardSoftLimitEnable(m.forward_soft_limit_enable(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigReverseSoftLimitThreshold(m.reverse_soft_limit(), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigReverseSoftLimitEnable(m.reverse_soft_limit_enable(), ck::kCANTimeoutMs); }, id);
    //         //ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->ConfigSelectedFeedbackCoefficient(m.feedback_sensor_coefficient(), 0, ck::kCANTimeoutMs); }, id);
    //     }

    //     const ck::MotorConfiguration_Motor_CurrentLimitConfiguration &supplyCurrLimConfig = m.supply_current_limit_config();
        
    //     switch (mType)
    //     {
    //     case MotorType::TALON_FX:
    //     {
    //         const ck::MotorConfiguration_Motor_CurrentLimitConfiguration &statorCurrLimConfig = m.stator_current_limit_config();
    //         TalonFX* tfx = dynamic_cast<TalonFX*>(mCtrl);
    //         ck::runPhoenixFunctionWithRetry([&]() { return tfx->ConfigSupplyCurrentLimit(SupplyCurrentLimitConfiguration(supplyCurrLimConfig.enable(), supplyCurrLimConfig.current_limit(), supplyCurrLimConfig.trigger_threshold_current(), supplyCurrLimConfig.trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return tfx->ConfigStatorCurrentLimit(StatorCurrentLimitConfiguration(statorCurrLimConfig.enable(), statorCurrLimConfig.current_limit(), statorCurrLimConfig.trigger_threshold_current(), statorCurrLimConfig.trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
    //     }
    //         break;
    //     case MotorType::TALON_SRX:
    //     {
    //         TalonSRX* tsrx = dynamic_cast<TalonSRX*>(mCtrl);
    //         ck::runPhoenixFunctionWithRetry([&]() { return tsrx->ConfigSupplyCurrentLimit(SupplyCurrentLimitConfiguration(supplyCurrLimConfig.enable(), supplyCurrLimConfig.current_limit(), supplyCurrLimConfig.trigger_threshold_current(), supplyCurrLimConfig.trigger_threshold_time()), ck::kCANTimeoutMs); }, id);
    //     }
    //         break;
    //     default:
    //         break;
    //     }

    //     switch (m.controller_mode())
    //     {
    //     case ck::MotorConfiguration_Motor_ControllerMode::MotorConfiguration_Motor_ControllerMode_FAST_MASTER:
    //     {
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetControlFramePeriod(ControlFrame::Control_3_General, FAST_MASTER_CONFIG.CONTROL_FRAME_PERIOD_MS); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_1_General_, FAST_MASTER_CONFIG.STATUS_FRAME_GENERAL_1_MS, ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_2_Feedback0_, FAST_MASTER_CONFIG.STATUS_FRAME_FEEDBACK0_2_MS, ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_4_AinTempVbat_, FAST_MASTER_CONFIG.STATUS_FRAME_AINTTEMPVBAT, ck::kCANTimeoutMs); }, id);
    //     }
    //         break;
    //     case ck::MotorConfiguration_Motor_ControllerMode::MotorConfiguration_Motor_ControllerMode_SLAVE:
    //     {
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetControlFramePeriod(ControlFrame::Control_3_General, SLAVE_CONFIG.CONTROL_FRAME_PERIOD_MS); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_1_General_, SLAVE_CONFIG.STATUS_FRAME_GENERAL_1_MS, ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_2_Feedback0_, SLAVE_CONFIG.STATUS_FRAME_FEEDBACK0_2_MS, ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_4_AinTempVbat_, SLAVE_CONFIG.STATUS_FRAME_AINTTEMPVBAT, ck::kCANTimeoutMs); }, id);
    //     }
    //         break;
    //     case ck::MotorConfiguration_Motor_ControllerMode::MotorConfiguration_Motor_ControllerMode_MASTER:
    //     default:
    //     {
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetControlFramePeriod(ControlFrame::Control_3_General, NORMAL_MASTER_CONFIG.CONTROL_FRAME_PERIOD_MS); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_1_General_, NORMAL_MASTER_CONFIG.STATUS_FRAME_GENERAL_1_MS, ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_2_Feedback0_, NORMAL_MASTER_CONFIG.STATUS_FRAME_FEEDBACK0_2_MS, ck::kCANTimeoutMs); }, id);
    //         ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetStatusFramePeriod(StatusFrame::Status_4_AinTempVbat_, NORMAL_MASTER_CONFIG.STATUS_FRAME_AINTTEMPVBAT, ck::kCANTimeoutMs); }, id);
    //     }
    //         break;
    //     }

    // });
    return true;
}

void ApplyLEDControlTask::initFieldDescriptors()
{
    LED_TYPE_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(3);
    VBAT_CONFIG_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(4);
    VBAT_DUTY_CYCLE_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(5);
    LED_CONTROL_MODE_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(6);
    COLOR_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(7);
    ANIMATION_FD = (google::protobuf::FieldDescriptor*)ck::LEDControl::LEDControlData::GetDescriptor()->FindFieldByNumber(8);
}

void updateColor(uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& d)
{
    ck::RGBWColor c = d.color();
    // mCtrl->SetLEDs(c.r(), c.g(), c.b(), c.w());
}

void ApplyLEDControlTask::initUpdateFunctions()
{
    mDiffReporter.RegisterUpdateFunction(LED_TYPE_FD, [](const google::protobuf::Message &msg)
    {
        CANdleManager::getInstance().onCANdle(msg,
            [](uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& d)
            { ck::runPhoenixFunctionWithRetry([mCtrl, d]() { return mCtrl->ConfigLEDType((LEDStripType)d.led_type(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(VBAT_CONFIG_FD, [](const google::protobuf::Message &msg)
    {
        CANdleManager::getInstance().onCANdle(msg,
            [](uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& d)
            { ck::runPhoenixFunctionWithRetry([mCtrl, d]() { return mCtrl->ConfigVBatOutput((VBatOutputMode)d.vbat_config(), ck::kCANTimeoutMs); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(VBAT_DUTY_CYCLE_FD, [](const google::protobuf::Message &msg)
    {
        CANdleManager::getInstance().onCANdle(msg,
            [](uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& d)
            { ck::runPhoenixFunctionWithRetry([mCtrl, d]() { return mCtrl->ModulateVBatOutput(d.vbat_duty_cycle()); }, id); });
    });

    mDiffReporter.RegisterUpdateFunction(LED_CONTROL_MODE_FD, [](const google::protobuf::Message &msg)
    {
        CANdleManager::getInstance().onCANdle(msg,
            [](uint16_t id, ctre::phoenix::led::CANdle* mCtrl, const ck::LEDControl::LEDControlData& d)
            { ck::runPhoenixFunctionWithRetry([mCtrl, d]() { return mCtrl->ConfigVBatOutput((VBatOutputMode)d.vbat_config(), ck::kCANTimeoutMs); }, id); });
    });
}