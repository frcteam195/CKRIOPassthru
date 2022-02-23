#include "tasks/SendMotorDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include "MotorManager.hpp"
#include "NetworkManager.hpp"
#include "MotorConfigManager.hpp"

SendMotorDataTask::SendMotorDataTask() : Task(THREAD_RATE_MS, TASK_NAME), mMotorStatusMsg()
{
    mMotorStatusBuf = malloc(MOTOR_STATUS_MESSAGE_SIZE * sizeof(uint8_t));
    memset(mMotorStatusBuf, 0, MOTOR_STATUS_MESSAGE_SIZE * sizeof(uint8_t));
}
SendMotorDataTask::~SendMotorDataTask()
{
    free(mMotorStatusBuf);
}

void SendMotorDataTask::run(uint32_t timeSinceLastUpdateMs)
{
    mTaskTimer.start();
    mMotorStatusMsg.Clear();

    MotorManager::getInstance().forEach([&](uint16_t id, BaseTalon *mCtrl, MotorType mType)
                                        {
        //ck::MotorConfiguration::Motor::ControllerMode mCtrlMode = MotorConfigManager::getInstance().getControllerMode(id);
        
        // if (mCtrlMode == ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_FAST_MASTER
        //     || mCtrlMode == ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_MASTER)
        // {
            ck::MotorStatus_Motor* m = mMotorStatusMsg.add_motors();
            m->set_id(id);
            m->set_sensor_position(mCtrl->GetSelectedSensorPosition());
            m->set_sensor_velocity(mCtrl->GetSelectedSensorVelocity());
            ControlMode cm = mCtrl->GetControlMode();
            m->set_control_mode((ck::MotorStatus::Motor::ControlMode)cm);
            switch (cm)
            {
                case ControlMode::PercentOutput:
                {
                    m->set_commanded_output(mCtrl->GetMotorOutputPercent());
                    break;
                }
                case ControlMode::MotionProfile:
                case ControlMode::MotionMagic:
                case ControlMode::MotionProfileArc:
                {
                    m->set_active_trajectory_arbff(mCtrl->GetActiveTrajectoryArbFeedFwd());
                    m->set_active_trajectory_position(mCtrl->GetActiveTrajectoryPosition());
                    m->set_active_trajectory_velocity(mCtrl->GetActiveTrajectoryVelocity());
                    [[fallthrough]]; //Fall through on the above cases so we also set closed loop data
                }
                case ControlMode::Position:
                case ControlMode::Velocity:
                case ControlMode::Current:
                {
                    m->set_commanded_output(mCtrl->GetClosedLoopTarget());
                    m->set_raw_closed_loop_error(mCtrl->GetClosedLoopError());
                    m->set_raw_integral_accum(mCtrl->GetIntegralAccumulator());
                    m->set_raw_error_derivative(mCtrl->GetErrorDerivative());
                    break;
                }
                case ControlMode::Follower:
                case ControlMode::MusicTone:
                case ControlMode::Disabled:
                {
                    break;
                }
            }
            m->set_raw_output_percent(mCtrl->GetMotorOutputPercent());
            m->set_bus_voltage(mCtrl->GetBusVoltage());
            m->set_bus_current(mCtrl->GetSupplyCurrent());
            m->set_stator_current(mCtrl->GetStatorCurrent());
            
            switch (mType)
            {
            case MotorType::TALON_FX:
            {
                TalonFX* tfx = dynamic_cast<TalonFX*>(mCtrl);
                m->set_forward_limit_closed(tfx->GetSensorCollection().IsFwdLimitSwitchClosed());
                m->set_reverse_limit_closed(tfx->GetSensorCollection().IsRevLimitSwitchClosed());
            }
                break;
            case MotorType::TALON_SRX:
            {
                TalonSRX* tsrx = dynamic_cast<TalonSRX*>(mCtrl);
                m->set_forward_limit_closed(tsrx->GetSensorCollection().IsFwdLimitSwitchClosed());
                m->set_reverse_limit_closed(tsrx->GetSensorCollection().IsRevLimitSwitchClosed());
            }
                break;
            default:
            {

            }
                break;
            }
        //}
        });

    if (mMotorStatusMsg.SerializeToArray(mMotorStatusBuf, mMotorStatusMsg.ByteSizeLong()))
    {
        NetworkManager::getInstance().sendMessage(MOTOR_STATUS_MESSAGE_GROUP, mMotorStatusBuf, mMotorStatusMsg.ByteSizeLong());
    }
    else
    {
        std::cout << "Motor status message failed to serialize. Message probably too large or invalid." << std::endl;
    }
    mTaskTimer.reportElapsedTime();
}