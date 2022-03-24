#pragma once
#include "utils/Singleton.hpp"
#include "failover/utils/FailoverSubsystem.hpp"
#include "frc/Joystick.h"
#include "MotorManager.hpp"

#include "MotorControl.pb.h"
#include "MotorConfiguration.pb.h"
#include <cstdint>
#include "failover/utils/drive_helper.hpp"
#include "failover/utils/ValueRamper.hpp"

class DrivetrainFailover : public Singleton<DrivetrainFailover>, public FailoverSubsystem
{
    friend Singleton;
protected:
    void init() override;
    void periodic() override;
    void uninit() override;
    void run() override;

private:
    DrivetrainFailover();
    ~DrivetrainFailover();

    frc::Joystick* mJoystick;

    static constexpr int LEFT_MASTER_MOTOR_ID = 1;
    static constexpr int LEFT_FOLLOWER_MOTOR_ID = 2;
    static constexpr int RIGHT_MASTER_MOTOR_ID = 4;
    static constexpr int RIGHT_FOLLOWER_MOTOR_ID = 5;

    static constexpr int DRIVE_JOYSTICK_ID = 0;
    static constexpr double DRIVE_JOYSTICK_DEADBAND = 0.12;
    static constexpr int DRIVE_JOYSTICK_X_AXIS = 0;
    static constexpr int DRIVE_JOYSTICK_Y_AXIS = 1;
    static constexpr int DRIVE_JOYSTICK_QUICK_TURN_BUTTON = 7;
    static constexpr int DRIVE_JOYSTICK_BRAKE_MODE_BUTTON = 8;

    static constexpr double DRIVE_ACCEL_RAMP = 0.5;
    static constexpr double DRIVE_DECEL_RAMP = 0.2;
    static constexpr double DRIVE_ZERO_VAL = 0;
    static constexpr double DRIVE_MAX_VAL = 1;


    ck::MotorConfiguration::Motor* mLeftMasterConfig = nullptr;
    ck::MotorConfiguration::Motor* mLeftFollowerConfig = nullptr;
    ck::MotorConfiguration::Motor* mRightMasterConfig = nullptr;
    ck::MotorConfiguration::Motor* mRightFollowerConfig = nullptr;

    ck::MotorControl::Motor* mLeftMaster = nullptr;
    ck::MotorControl::Motor* mLeftFollower = nullptr;
    ck::MotorControl::Motor* mRightMaster = nullptr;
    ck::MotorControl::Motor* mRightFollower = nullptr;
    
    DriveHelper mDriveHelper;
    ValueRamper mLeftValueRamper;
    ValueRamper mRightValueRamper;
};