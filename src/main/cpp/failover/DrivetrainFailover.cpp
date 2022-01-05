#include "failover/DrivetrainFailover.hpp"
#include "MotorManager.hpp"
#include "utils/PhoenixHelper.hpp"
#include "utils/CKMath.hpp"

DrivetrainFailover::DrivetrainFailover() : mJoystick(DRIVE_JOYSTICK_ID)
{

}
DrivetrainFailover::~DrivetrainFailover() {}

void DrivetrainFailover::init()
{
    //Motor Configuration code should be placed here
    MotorManager::getInstance().onMotor(LEFT_MOTOR_ID, [](uint16_t id, BaseMotorController* mCtrl, MotorType mType)
    {
        //Simple example config. See tasks/ApplyMotorConfigTask.cpp for more advanced examples
        mCtrl->ConfigVoltageCompSaturation(12);
        mCtrl->EnableVoltageCompensation(true);
    });

    MotorManager::getInstance().onMotor(RIGHT_MOTOR_ID, [](uint16_t id, BaseMotorController* mCtrl, MotorType mType)
    {
        //Simple example config. See tasks/ApplyMotorConfigTask.cpp for more advanced examples
        mCtrl->ConfigVoltageCompSaturation(12);
        mCtrl->EnableVoltageCompensation(true);
    });
}

void DrivetrainFailover::run()
{
    //Motor Control code should be placed here
    double x = ck::math::normalizeWithDeadband(mJoystick.GetX(), DRIVE_JOYSTICK_DEADBAND);
    double y = ck::math::normalizeWithDeadband(mJoystick.GetY(), DRIVE_JOYSTICK_DEADBAND);

    MotorManager::getInstance().onMotor(LEFT_MOTOR_ID, [x, y](uint16_t id, BaseMotorController* mCtrl, MotorType mType)
    {
        mCtrl->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, y + x);
    });
    MotorManager::getInstance().onMotor(LEFT_MOTOR_ID, [x, y](uint16_t id, BaseMotorController* mCtrl, MotorType mType)
    {
        mCtrl->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, y - x);
    });
}

void DrivetrainFailover::registerMotors()
{
    //Place all motors used here. This will be called automatically as needed in the SubsystemRun and SubsystemInit methods
    MotorManager::getInstance().registerMotor(LEFT_MOTOR_ID, MotorType::TALON_FX);
    MotorManager::getInstance().registerMotor(RIGHT_MOTOR_ID, MotorType::TALON_FX);
}