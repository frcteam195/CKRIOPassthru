#pragma once
#include "utils/Singleton.hpp"
#include "failover/utils/FailoverSubsystem.hpp"
#include "frc/Joystick.h"
#include "MotorManager.hpp"

#include "MotorControl.pb.h"
#include "MotorConfiguration.pb.h"
#include <cstdint>

class DrivetrainFailover : public Singleton<DrivetrainFailover>, public FailoverSubsystem
{
    friend Singleton;
protected:
    void init() override;
    void periodic() override;
    void uninit() override;
    void run() override;
    void registerMotors() override;

private:
    DrivetrainFailover();
    ~DrivetrainFailover();

    frc::Joystick* mJoystick;

    static constexpr MotorType DRIVE_MOTOR_TYPE = MotorType::TALON_FX;
    static constexpr int LEFT_MOTOR_ID = 1;
    static constexpr int RIGHT_MOTOR_ID = 4;

    static constexpr int DRIVE_JOYSTICK_ID = 0;
    static constexpr double DRIVE_JOYSTICK_DEADBAND = 0.05;

    ck::MotorConfiguration mMotorConfiguration;
    ck::MotorConfiguration::Motor* mLeftMasterConfig = nullptr;
    ck::MotorConfiguration::Motor* mLeftFollowerConfig = nullptr;
    ck::MotorConfiguration::Motor* mRightMasterConfig = nullptr;
    ck::MotorConfiguration::Motor* mRightFollowerConfig = nullptr;

    ck::MotorControl mMotorControl;
    ck::MotorControl::Motor* mLeftMaster = nullptr;
    ck::MotorControl::Motor* mLeftFollower = nullptr;
    ck::MotorControl::Motor* mRightMaster = nullptr;
    ck::MotorControl::Motor* mRightFollower = nullptr;

    static constexpr int BUF_SIZE = 1500;
    void* mBuff = nullptr;
};