#pragma once
#include "utils/Singleton.hpp"
#include "failover/utils/FailoverSubsystem.hpp"
#include "frc/Joystick.h"
#include "MotorManager.hpp"

class DrivetrainFailover : public Singleton<DrivetrainFailover>, public FailoverSubsystem
{
    friend Singleton;
protected:
    void init() override;
    void uninit() override;
    void run() override;
    void registerMotors() override;

private:
    DrivetrainFailover() = default;
    ~DrivetrainFailover();

    frc::Joystick* mJoystick;

    static constexpr MotorType DRIVE_MOTOR_TYPE = MotorType::TALON_FX;
    static constexpr int LEFT_MOTOR_ID = 1;
    static constexpr int RIGHT_MOTOR_ID = 4;

    static constexpr int DRIVE_JOYSTICK_ID = 1;
    static constexpr double DRIVE_JOYSTICK_DEADBAND = 0.05;


};