#include "failover/DrivetrainFailover.hpp"
#include "utils/PhoenixHelper.hpp"
#include "utils/CKMath.hpp"
#include "NetworkManager.hpp"

void DrivetrainFailover::init()
{
    //Initialize joysticks here so they don't exist until in failover mode
    mJoystick = new frc::Joystick(DRIVE_JOYSTICK_ID);

}

void DrivetrainFailover::uninit()
{
    if (mJoystick)
    {
        delete mJoystick;
        mJoystick = nullptr;
    }
}

void DrivetrainFailover::periodic()
{

}

void DrivetrainFailover::run()
{
    //Motor Control code should be placed here
    double x = 0;
    double y = 0;
    //Check mJoystick is valid and connected
    if (mJoystick && mJoystick->IsConnected())
    {
        x = ck::math::normalizeWithDeadband(mJoystick->GetRawAxis(4), DRIVE_JOYSTICK_DEADBAND);
        y = -ck::math::normalizeWithDeadband(mJoystick->GetRawAxis(1), DRIVE_JOYSTICK_DEADBAND);
    }

    mLeftMaster->set_output_value(std::max(std::min(y + x, 1.0), -1.0));
    mRightMaster->set_output_value(std::max(std::min(y - x, 1.0), -1.0));
}

void DrivetrainFailover::registerMotors()
{
    static uint32_t mLoopCounter = 0;
    //Place all motors used here. This will be called automatically as needed in the SubsystemRun and SubsystemInit methods
    if (mMotorControl.SerializeToArray(mBuff, BUF_SIZE))
    {
        std::vector<uint8_t> buf(BUF_SIZE, 0);
        memcpy(&buf[0], mBuff, BUF_SIZE);
        NetworkManager::getInstance().placeFailoverMessage("motorcontrol", buf);
    }

    if (mLoopCounter++ % 50 == 0)
    {
        if (mMotorConfiguration.SerializeToArray(mBuff, BUF_SIZE))
        {
            std::vector<uint8_t> buf(BUF_SIZE, 0);
            memcpy(&buf[0], mBuff, BUF_SIZE);
            NetworkManager::getInstance().placeFailoverMessage("motorconfig", buf);
        }
    }
}

DrivetrainFailover::DrivetrainFailover()
{
    mBuff = malloc(BUF_SIZE * sizeof(uint8_t));
    memset(mBuff, 0, BUF_SIZE * sizeof(uint8_t));

    //Left Side
    mLeftMasterConfig = mMotorConfiguration.add_motors();
    mLeftMasterConfig->set_id(1);
    mLeftMasterConfig->set_controller_mode(ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_FAST_MASTER);
    mLeftMasterConfig->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_None);
    mLeftMasterConfig->set_controller_type(ck::MotorConfiguration::Motor::ControllerType::MotorConfiguration_Motor_ControllerType_TALON_FX);
    mLeftMasterConfig->set_neutral_mode(ck::MotorConfiguration::Motor::NeutralMode::MotorConfiguration_Motor_NeutralMode_Coast);
    mLeftMasterConfig->set_voltage_compensation_saturation(12);
    mLeftMasterConfig->set_voltage_compensation_enabled(true);
    mLeftMasterConfig->set_open_loop_ramp(0.15);
    mLeftMasterConfig->set_forward_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    mLeftMasterConfig->set_forward_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    mLeftMasterConfig->set_reverse_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    mLeftMasterConfig->set_reverse_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    ck::MotorConfiguration::Motor::CurrentLimitConfiguration* mLeftMasterCurrLim = new ck::MotorConfiguration::Motor::CurrentLimitConfiguration();
    mLeftMasterCurrLim->set_enable(true);
    mLeftMasterCurrLim->set_current_limit(40);
    mLeftMasterCurrLim->set_trigger_threshold_current(0);
    mLeftMasterCurrLim->set_trigger_threshold_time(0);
    mLeftMasterConfig->set_allocated_supply_current_limit_config(mLeftMasterCurrLim);
    mLeftMasterConfig->set_can_network(ck::CANNetwork::RIO_CANIVORE);

    mLeftMaster = mMotorControl.add_motors();
    mLeftMaster->set_id(1);
    mLeftMaster->set_arbitrary_feedforward(0);
    mLeftMaster->set_controller_type(ck::MotorControl::Motor::ControllerType::MotorControl_Motor_ControllerType_TALON_FX);
    mLeftMaster->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    mLeftMaster->set_output_value(0);
    
    mLeftFollowerConfig = mMotorConfiguration.add_motors();
    mLeftFollowerConfig->set_id(2);
    mLeftFollowerConfig->set_controller_mode(ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_SLAVE);
    mLeftFollowerConfig->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_None);
    mLeftFollowerConfig->set_controller_type(ck::MotorConfiguration::Motor::ControllerType::MotorConfiguration_Motor_ControllerType_TALON_FX);
    mLeftFollowerConfig->set_neutral_mode(ck::MotorConfiguration::Motor::NeutralMode::MotorConfiguration_Motor_NeutralMode_Coast);
    mLeftFollowerConfig->set_voltage_compensation_saturation(12);
    mLeftFollowerConfig->set_voltage_compensation_enabled(true);
    mLeftFollowerConfig->set_open_loop_ramp(0.15);
    mLeftFollowerConfig->set_forward_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    mLeftFollowerConfig->set_forward_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    mLeftFollowerConfig->set_reverse_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    mLeftFollowerConfig->set_reverse_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    ck::MotorConfiguration::Motor::CurrentLimitConfiguration* mLeftFollowerCurrLim = new ck::MotorConfiguration::Motor::CurrentLimitConfiguration();
    mLeftFollowerCurrLim->set_enable(true);
    mLeftFollowerCurrLim->set_current_limit(40);
    mLeftFollowerCurrLim->set_trigger_threshold_current(0);
    mLeftFollowerCurrLim->set_trigger_threshold_time(0);
    mLeftFollowerConfig->set_allocated_supply_current_limit_config(mLeftFollowerCurrLim);
    mLeftFollowerConfig->set_can_network(ck::CANNetwork::RIO_CANIVORE);

    mLeftFollower = mMotorControl.add_motors();
    mLeftFollower->set_id(2);
    mLeftFollower->set_arbitrary_feedforward(0);
    mLeftFollower->set_controller_type(ck::MotorControl::Motor::ControllerType::MotorControl_Motor_ControllerType_TALON_FX);
    mLeftFollower->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_Follower);
    mLeftFollower->set_output_value(1); //Master ID

    //Right Side
    mRightMasterConfig = mMotorConfiguration.add_motors();
    mRightMasterConfig->set_id(4);
    mRightMasterConfig->set_controller_mode(ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_FAST_MASTER);
    mRightMasterConfig->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_None);
    mRightMasterConfig->set_controller_type(ck::MotorConfiguration::Motor::ControllerType::MotorConfiguration_Motor_ControllerType_TALON_FX);
    mRightMasterConfig->set_neutral_mode(ck::MotorConfiguration::Motor::NeutralMode::MotorConfiguration_Motor_NeutralMode_Coast);
    mRightMasterConfig->set_voltage_compensation_saturation(12);
    mRightMasterConfig->set_voltage_compensation_enabled(true);
    mRightMasterConfig->set_open_loop_ramp(0.15);
    mRightMasterConfig->set_forward_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    mRightMasterConfig->set_forward_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    mRightMasterConfig->set_reverse_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    mRightMasterConfig->set_reverse_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    ck::MotorConfiguration::Motor::CurrentLimitConfiguration* mRightMasterCurrLim = new ck::MotorConfiguration::Motor::CurrentLimitConfiguration();
    mRightMasterCurrLim->set_enable(true);
    mRightMasterCurrLim->set_current_limit(40);
    mRightMasterCurrLim->set_trigger_threshold_current(0);
    mRightMasterCurrLim->set_trigger_threshold_time(0);
    mRightMasterConfig->set_allocated_supply_current_limit_config(mRightMasterCurrLim);
    mRightMasterConfig->set_can_network(ck::CANNetwork::RIO_CANIVORE);

    mRightMaster = mMotorControl.add_motors();
    mRightMaster->set_id(4);
    mRightMaster->set_arbitrary_feedforward(0);
    mRightMaster->set_controller_type(ck::MotorControl::Motor::ControllerType::MotorControl_Motor_ControllerType_TALON_FX);
    mRightMaster->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    mRightMaster->set_output_value(0);
    
    mRightFollowerConfig = mMotorConfiguration.add_motors();
    mRightFollowerConfig->set_id(5);
    mRightFollowerConfig->set_controller_mode(ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_SLAVE);
    mRightFollowerConfig->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_None);
    mRightFollowerConfig->set_controller_type(ck::MotorConfiguration::Motor::ControllerType::MotorConfiguration_Motor_ControllerType_TALON_FX);
    mRightFollowerConfig->set_neutral_mode(ck::MotorConfiguration::Motor::NeutralMode::MotorConfiguration_Motor_NeutralMode_Coast);
    mRightFollowerConfig->set_voltage_compensation_saturation(12);
    mRightFollowerConfig->set_voltage_compensation_enabled(true);
    mRightFollowerConfig->set_open_loop_ramp(0.15);
    mRightFollowerConfig->set_forward_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    mRightFollowerConfig->set_forward_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    mRightFollowerConfig->set_reverse_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    mRightFollowerConfig->set_reverse_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    ck::MotorConfiguration::Motor::CurrentLimitConfiguration* mRightFollowerCurrLim = new ck::MotorConfiguration::Motor::CurrentLimitConfiguration();
    mRightFollowerCurrLim->set_enable(true);
    mRightFollowerCurrLim->set_current_limit(40);
    mRightFollowerCurrLim->set_trigger_threshold_current(0);
    mRightFollowerCurrLim->set_trigger_threshold_time(0);
    mRightFollowerConfig->set_allocated_supply_current_limit_config(mRightFollowerCurrLim);
    mRightFollowerConfig->set_can_network(ck::CANNetwork::RIO_CANIVORE);

    mRightFollower = mMotorControl.add_motors();
    mRightFollower->set_id(5);
    mRightFollower->set_arbitrary_feedforward(0);
    mRightFollower->set_controller_type(ck::MotorControl::Motor::ControllerType::MotorControl_Motor_ControllerType_TALON_FX);
    mRightFollower->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_Follower);
    mRightFollower->set_output_value(4); //Master ID
}

DrivetrainFailover::~DrivetrainFailover()
{
    uninit();
    free(mBuff);
}