#include "failover/subsystems/ArmFailover.hpp"
#include "utils/PhoenixHelper.hpp"
#include "utils/CKMath.hpp"
#include "failover/FailoverMessageManager.hpp"
#include "math.h"

void ArmFailover::init()
{

}

void ArmFailover::uninit()
{
    mLowerArmMaster->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    mLowerArmMaster->set_output_value(0);
    mUpperArmMaster->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    mUpperArmMaster->set_output_value(0);
    mWrist->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    mWrist->set_output_value(0);
}

void ArmFailover::periodic()
{

}

void ArmFailover::run()
{
    mLowerArmMaster->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    mLowerArmMaster->set_output_value(0);
    mUpperArmMaster->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    mUpperArmMaster->set_output_value(0);
    mWrist->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    mWrist->set_output_value(0);
}

ArmFailover::ArmFailover()
{
    SetBaseConfig(LOWER_ARM_MASTER_ID, mLowerArmMasterConfig);
    mLowerArmMasterConfig->set_feedback_sensor_can_id(LOWER_ARM_CANCODER_ID);
    mLowerArmMasterConfig->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_InvertMotorOutput);
    SetBaseControl(LOWER_ARM_MASTER_ID, mLowerArmMaster);
    ConfigPID(mLowerArmMasterConfig, lower_arm_pid);
    SetBaseConfig(LOWER_ARM_FOLLOWER_ID, mLowerArmFollowerConfig);
    mLowerArmFollowerConfig->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_OpposeMaster);
    SetBaseControl(LOWER_ARM_FOLLOWER_ID, mLowerArmFollower, LOWER_ARM_MASTER_ID);

    SetBaseConfig(UPPER_ARM_MASTER_ID, mUpperArmMasterConfig);
    mUpperArmMasterConfig->set_feedback_sensor_can_id(UPPER_ARM_CANCODER_ID);
    mUpperArmMasterConfig->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_InvertMotorOutput);
    SetBaseControl(UPPER_ARM_MASTER_ID, mUpperArmMaster);
    ConfigPID(mUpperArmMasterConfig, upper_arm_pid);
    SetBaseConfig(UPPER_ARM_FOLLOWER_ID, mUpperArmFollowerConfig);
    mUpperArmFollowerConfig->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_OpposeMaster);
    SetBaseControl(UPPER_ARM_FOLLOWER_ID, mUpperArmFollower, UPPER_ARM_MASTER_ID);

    SetBaseConfig(WRIST_ID, mWristConfig, 30);
    mWristConfig->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_InvertMotorOutput);
    SetBaseControl(WRIST_ID, mWrist);
    ConfigPID(mWristConfig, wrist_pid);
}

void ArmFailover::SetBaseConfig(int id, ck::MotorConfiguration::Motor*& config_obj, int current_limit)
{
    config_obj = FailoverMessageManager::getInstance().addMotorConfig();
    config_obj->set_id(id);
    config_obj->set_controller_mode(ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_MASTER);
    config_obj->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_None);
    config_obj->set_controller_type(ck::MotorConfiguration::Motor::ControllerType::MotorConfiguration_Motor_ControllerType_TALON_FX);
    config_obj->set_neutral_mode(ck::MotorConfiguration::Motor::NeutralMode::MotorConfiguration_Motor_NeutralMode_Brake);
    config_obj->set_voltage_compensation_saturation(12);
    config_obj->set_voltage_compensation_enabled(true);
    config_obj->set_open_loop_ramp(0);
    config_obj->set_forward_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    config_obj->set_forward_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    config_obj->set_reverse_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    config_obj->set_reverse_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    ck::MotorConfiguration::Motor::CurrentLimitConfiguration* mCurrLim = new ck::MotorConfiguration::Motor::CurrentLimitConfiguration();
    mCurrLim->set_enable(true);
    mCurrLim->set_current_limit(current_limit);
    mCurrLim->set_trigger_threshold_current(0);
    mCurrLim->set_trigger_threshold_time(0);
    config_obj->set_allocated_supply_current_limit_config(mCurrLim);
    config_obj->set_can_network(ck::CANNetwork::RIO_CANIVORE);
}

void ArmFailover::SetBaseControl(int id, ck::MotorControl::Motor*& control_obj)
{
    control_obj = FailoverMessageManager::getInstance().addMotorControl();
    control_obj->set_id(id);
    control_obj->set_arbitrary_feedforward(0);
    control_obj->set_controller_type(ck::MotorControl::Motor::ControllerType::MotorControl_Motor_ControllerType_TALON_FX);
    control_obj->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    control_obj->set_output_value(0);
}

void ArmFailover::SetBaseControl(int id, ck::MotorControl::Motor*& control_obj, int master_id)
{
    SetBaseControl(id, control_obj);
    control_obj->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_Follower);
    control_obj->set_output_value(master_id);
}

void ArmFailover::ConfigPID(ck::MotorConfiguration::Motor*& config_obj, ConfigDataSet& d)
{
    config_obj->set_kp(d.kP);
    config_obj->set_ki(d.kI);
    config_obj->set_kd(d.kD);
    config_obj->set_kf(d.kF);
    config_obj->set_motion_cruise_velocity(d.motion_cruise_vel);
    config_obj->set_motion_acceleration(d.motion_accel);
    config_obj->set_motion_s_curve_strength(d.motion_scurve);
    config_obj->set_forward_soft_limit_enable(d.forward_soft_limit_enabled);
    config_obj->set_forward_soft_limit(d.forward_soft_limit);
    config_obj->set_reverse_soft_limit_enable(d.reverse_soft_limit_enabled);
    config_obj->set_reverse_soft_limit(d.reverse_soft_limit);
}

ArmFailover::~ArmFailover()
{
    uninit();
}