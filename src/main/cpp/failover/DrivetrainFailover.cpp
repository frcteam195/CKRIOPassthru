#include "failover/DrivetrainFailover.hpp"
#include "utils/PhoenixHelper.hpp"
#include "utils/CKMath.hpp"

void DrivetrainFailover::init()
{
    //Initialize joysticks here so they don't exist until in failover mode
    mJoystick = new frc::Joystick(DRIVE_JOYSTICK_ID);

    SupplyCurrentLimitConfiguration supplyCurrLim {true, 60, 0, 0};
    //Motor Configuration code should be placed here
    MotorManager::getInstance().onMotor(LEFT_MOTOR_ID, [&](uint16_t id, BaseTalon* mCtrl, MotorType mType)
    {
        //Simple example config. See tasks/ApplyMotorConfigTask.cpp for more advanced examples
        mCtrl->ConfigVoltageCompSaturation(12);
        mCtrl->EnableVoltageCompensation(true);
        mCtrl->ConfigSupplyCurrentLimit(supplyCurrLim);
    });

    MotorManager::getInstance().onMotor(RIGHT_MOTOR_ID, [&](uint16_t id, BaseTalon* mCtrl, MotorType mType)
    {
        //Simple example config. See tasks/ApplyMotorConfigTask.cpp for more advanced examples
        mCtrl->ConfigVoltageCompSaturation(12);
        mCtrl->EnableVoltageCompensation(true);
        mCtrl->SetInverted(ctre::phoenix::motorcontrol::InvertType::InvertMotorOutput);
        mCtrl->ConfigSupplyCurrentLimit(supplyCurrLim);
    });
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
        x = ck::math::normalizeWithDeadband(mJoystick->GetX(), DRIVE_JOYSTICK_DEADBAND);
        y = -ck::math::normalizeWithDeadband(mJoystick->GetY(), DRIVE_JOYSTICK_DEADBAND);
    }

    MotorManager::getInstance().onMotor(LEFT_MOTOR_ID, [x, y](uint16_t id, BaseTalon* mCtrl, MotorType mType)
    {
        mCtrl->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, std::max(std::min(y + x, 1.0), -1.0));
    });
    MotorManager::getInstance().onMotor(RIGHT_MOTOR_ID, [x, y](uint16_t id, BaseTalon* mCtrl, MotorType mType)
    {
        mCtrl->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, std::max(std::min(y - x, 1.0), -1.0));
    });
}

void DrivetrainFailover::registerMotors()
{
    //Place all motors used here. This will be called automatically as needed in the SubsystemRun and SubsystemInit methods
    // MotorManager::getInstance().registerMotor(LEFT_MOTOR_ID, DRIVE_MOTOR_TYPE);
    // MotorManager::getInstance().registerMotor(RIGHT_MOTOR_ID, DRIVE_MOTOR_TYPE);
}

DrivetrainFailover::~DrivetrainFailover()
{
    uninit();
}