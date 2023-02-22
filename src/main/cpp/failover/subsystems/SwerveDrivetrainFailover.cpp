#include "failover/subsystems/SwerveDrivetrainFailover.hpp"
#include "utils/PhoenixHelper.hpp"
#include "utils/CKMath.hpp"
#include "failover/FailoverMessageManager.hpp"
#include "math.h"

void SwerveDrivetrainFailover::init()
{
    //Initialize joysticks here so they don't exist until in failover mode
    mJoystick = new frc::Joystick(DRIVE_JOYSTICK_ID);

}

void SwerveDrivetrainFailover::uninit()
{
    if (mJoystick)
    {
        delete mJoystick;
        mJoystick = nullptr;
    }

    mFrontLeftDrive->set_output_value(0);
    mFrontRightDrive->set_output_value(0);
    mBackLeftDrive->set_output_value(0);
    mBackRightDrive->set_output_value(0);
}

void SwerveDrivetrainFailover::periodic()
{

}

double SwerveDrivetrainFailover::meters_per_second_to_native_units_drive_velocity(units::meters_per_second_t mps)
{
    units::meter_t wheel_diameter_m {DRIVE_WHEEL_DIAMETER};
    double rpm = mps.value() / (wheel_diameter_m.value() * M_PI) * 60.0;
    return rpm * DRIVE_GEAR_RATIO * 2048.0 / 600.0;
}

double SwerveDrivetrainFailover::degrees_to_native_units_steering_angle(frc::Rotation2d rotation)
{
    return (rotation.Degrees().value() / 360.0) * STEERING_GEAR_RATIO * 4096.0;
}

void SwerveDrivetrainFailover::run()
{
    //Motor Control code should be placed here
    double x = 0;
    double y = 0;
    double z = 0;
    //Check mJoystick is valid and connected
    if (mJoystick && mJoystick->IsConnected())
    {
        x = ck::math::normalizeWithDeadband(mJoystick->GetRawAxis(DRIVE_JOYSTICK_X_AXIS), DRIVE_JOYSTICK_DEADBAND);
        y = ck::math::normalizeWithDeadband(mJoystick->GetRawAxis(DRIVE_JOYSTICK_Y_AXIS), DRIVE_JOYSTICK_DEADBAND);
        z = ck::math::normalizeWithDeadband(mJoystick->GetRawAxis(DRIVE_JOYSTICK_Z_AXIS), DRIVE_JOYSTICK_DEADBAND);

        frc::ChassisSpeeds speeds = frc::ChassisSpeeds::FromFieldRelativeSpeeds(4_mps * x, 4_mps * y, 8_rad_per_s * z, frc::Rotation2d(/*robot current angle*/));
        auto [fl, fr, bl, br] = m_kinematics.ToSwerveModuleStates(speeds);
        auto flOptimized = frc::SwerveModuleState::Optimize(fl, units::radian_t(/*encoder distance*/));
        auto frOptimized = frc::SwerveModuleState::Optimize(fr, units::radian_t(/*encoder distance*/));
        auto blOptimized = frc::SwerveModuleState::Optimize(bl, units::radian_t(/*encoder distance*/));
        auto brOptimized = frc::SwerveModuleState::Optimize(br, units::radian_t(/*encoder distance*/));

        mFrontLeftDrive->set_output_value(meters_per_second_to_native_units_drive_velocity(flOptimized.speed));
        mFrontRightDrive->set_output_value(meters_per_second_to_native_units_drive_velocity(frOptimized.speed));
        mBackLeftDrive->set_output_value(meters_per_second_to_native_units_drive_velocity(blOptimized.speed));
        mBackRightDrive->set_output_value(meters_per_second_to_native_units_drive_velocity(brOptimized.speed));

        mFrontLeftSteering->set_output_value(degrees_to_native_units_steering_angle(flOptimized.angle));
        mFrontRightSteering->set_output_value(degrees_to_native_units_steering_angle(frOptimized.angle));
        mBackLeftSteering->set_output_value(degrees_to_native_units_steering_angle(blOptimized.angle));
        mBackRightSteering->set_output_value(degrees_to_native_units_steering_angle(brOptimized.angle));
    }
    else
    {
        mFrontLeftDrive->set_output_value(0);
        mFrontRightDrive->set_output_value(0);
        mBackLeftDrive->set_output_value(0);
        mBackRightDrive->set_output_value(0);
        return;
    }

}

SwerveDrivetrainFailover::SwerveDrivetrainFailover()
{
    SetDriveConfig(FRONT_LEFT_DRIVE_ID, mFrontLeftDriveConfig, mFrontLeftDrive);
    SetDriveConfig(FRONT_RIGHT_DRIVE_ID, mFrontRightDriveConfig, mFrontRightDrive);
    SetDriveConfig(BACK_LEFT_DRIVE_ID, mBackLeftDriveConfig, mBackLeftDrive);
    SetDriveConfig(BACK_RIGHT_DRIVE_ID, mBackRightDriveConfig, mBackRightDrive);

    SetSteeringConfig(FRONT_LEFT_STEERING_ID, FRONT_LEFT_STEERING_CANCODER_ID, mFrontLeftSteeringConfig, mFrontLeftSteering);
    SetSteeringConfig(FRONT_RIGHT_STEERING_ID, FRONT_RIGHT_STEERING_CANCODER_ID, mFrontRightSteeringConfig, mFrontRightSteering);
    SetSteeringConfig(BACK_LEFT_STEERING_ID, BACK_LEFT_STEERING_CANCODER_ID, mBackLeftSteeringConfig, mBackLeftSteering);
    SetSteeringConfig(BACK_RIGHT_STEERING_ID, BACK_RIGHT_STEERING_CANCODER_ID, mBackRightSteeringConfig, mBackRightSteering);
}

void SwerveDrivetrainFailover::SetDriveConfig(int id, ck::MotorConfiguration::Motor*& config_obj, ck::MotorControl::Motor*& control_obj)
{
    config_obj = FailoverMessageManager::getInstance().addMotorConfig();
    config_obj->set_id(id);
    config_obj->set_controller_mode(ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_FAST_MASTER);
    config_obj->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_None);
    config_obj->set_controller_type(ck::MotorConfiguration::Motor::ControllerType::MotorConfiguration_Motor_ControllerType_TALON_FX);
    config_obj->set_neutral_mode(ck::MotorConfiguration::Motor::NeutralMode::MotorConfiguration_Motor_NeutralMode_Brake);
    config_obj->set_voltage_compensation_saturation(12);
    config_obj->set_voltage_compensation_enabled(true);
    config_obj->set_kp(drive_velocity_kP);
    config_obj->set_ki(drive_velocity_kI);
    config_obj->set_kd(drive_velocity_kD);
    config_obj->set_kf(drive_velocity_kF);
    config_obj->set_open_loop_ramp(0.15);
    config_obj->set_forward_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    config_obj->set_forward_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    config_obj->set_reverse_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    config_obj->set_reverse_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    ck::MotorConfiguration::Motor::CurrentLimitConfiguration* mDriveCurrLim = new ck::MotorConfiguration::Motor::CurrentLimitConfiguration();
    mDriveCurrLim->set_enable(true);
    mDriveCurrLim->set_current_limit(55);
    mDriveCurrLim->set_trigger_threshold_current(0);
    mDriveCurrLim->set_trigger_threshold_time(0);
    config_obj->set_allocated_supply_current_limit_config(mDriveCurrLim);
    config_obj->set_can_network(ck::CANNetwork::RIO_CANIVORE);

    control_obj = FailoverMessageManager::getInstance().addMotorControl();
    control_obj->set_id(id);
    control_obj->set_arbitrary_feedforward(0);
    control_obj->set_controller_type(ck::MotorControl::Motor::ControllerType::MotorControl_Motor_ControllerType_TALON_FX);
    control_obj->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_Velocity);
    control_obj->set_output_value(0);
}

void SwerveDrivetrainFailover::SetSteeringConfig(int id, int cancoder_id, ck::MotorConfiguration::Motor*& config_obj, ck::MotorControl::Motor*& control_obj)
{
    config_obj = FailoverMessageManager::getInstance().addMotorConfig();
    config_obj->set_id(id);
    config_obj->set_controller_mode(ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_FAST_MASTER);
    config_obj->set_invert_type(ck::MotorConfiguration::Motor::InvertType::MotorConfiguration_Motor_InvertType_None);
    config_obj->set_controller_type(ck::MotorConfiguration::Motor::ControllerType::MotorConfiguration_Motor_ControllerType_TALON_FX);
    config_obj->set_neutral_mode(ck::MotorConfiguration::Motor::NeutralMode::MotorConfiguration_Motor_NeutralMode_Brake);
    config_obj->set_voltage_compensation_saturation(12);
    config_obj->set_voltage_compensation_enabled(true);
    config_obj->set_kp(steering_velocity_kP);
    config_obj->set_ki(steering_velocity_kI);
    config_obj->set_kd(steering_velocity_kD);
    config_obj->set_kf(steering_velocity_kF);
    config_obj->set_open_loop_ramp(0.15);
    config_obj->set_forward_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    config_obj->set_forward_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    config_obj->set_reverse_limit_switch_source(ck::MotorConfiguration::Motor::LimitSwitchSource::MotorConfiguration_Motor_LimitSwitchSource_Deactivated);
    config_obj->set_reverse_limit_switch_normal(ck::MotorConfiguration::Motor::LimitSwitchNormal::MotorConfiguration_Motor_LimitSwitchNormal_Disabled);
    ck::MotorConfiguration::Motor::CurrentLimitConfiguration* mDriveCurrLim = new ck::MotorConfiguration::Motor::CurrentLimitConfiguration();
    mDriveCurrLim->set_enable(true);
    mDriveCurrLim->set_current_limit(80);
    mDriveCurrLim->set_trigger_threshold_current(40);
    mDriveCurrLim->set_trigger_threshold_time(1);
    config_obj->set_allocated_supply_current_limit_config(mDriveCurrLim);
    config_obj->set_feedback_sensor_can_id(cancoder_id);
    config_obj->set_can_network(ck::CANNetwork::RIO_CANIVORE);

    control_obj = FailoverMessageManager::getInstance().addMotorControl();
    control_obj->set_id(id);
    control_obj->set_arbitrary_feedforward(0);
    control_obj->set_controller_type(ck::MotorControl::Motor::ControllerType::MotorControl_Motor_ControllerType_TALON_FX);
    control_obj->set_control_mode(ck::MotorControl::Motor::ControlMode::MotorControl_Motor_ControlMode_Position);
    control_obj->set_output_value(0);
}

SwerveDrivetrainFailover::~SwerveDrivetrainFailover()
{
    uninit();
}