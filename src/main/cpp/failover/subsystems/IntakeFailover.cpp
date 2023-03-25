#include "failover/subsystems/IntakeFailover.hpp"
#include "utils/PhoenixHelper.hpp"
#include "utils/CKMath.hpp"
#include "failover/FailoverMessageManager.hpp"
#include "math.h"

void IntakeFailover::init()
{

}

void IntakeFailover::uninit()
{
    mIntake->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    mIntake->set_output_value(0);
}

void IntakeFailover::periodic()
{

}

void IntakeFailover::run()
{
    mIntake->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    mIntake->set_output_value(0);
}

IntakeFailover::IntakeFailover()
{
    SetBaseConfig(INTAKE_ID, mIntakeConfig);
    SetBaseControl(INTAKE_ID, mIntake);
}

void IntakeFailover::SetBaseConfig(int id, ck::MotorConfiguration::Motor*& config_obj, int current_limit)
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

void IntakeFailover::SetBaseControl(int id, ck::MotorControl::Motor*& control_obj)
{
    control_obj = FailoverMessageManager::getInstance().addMotorControl();
    control_obj->set_id(id);
    control_obj->set_arbitrary_feedforward(0);
    control_obj->set_controller_type(ck::MotorControl::Motor::ControllerType::MotorControl_Motor_ControllerType_TALON_FX);
    control_obj->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_PercentOutput);
    control_obj->set_output_value(0);
}

IntakeFailover::~IntakeFailover()
{
    uninit();
}