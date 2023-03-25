#pragma once
#include "utils/Singleton.hpp"
#include "failover/utils/FailoverSubsystem.hpp"
#include "frc/Joystick.h"
#include "MotorManager.hpp"

#include "MotorControl.pb.h"
#include "MotorConfiguration.pb.h"
#include <cstdint>
#include "frc/geometry/Translation2d.h"
#include "frc/kinematics/SwerveDriveKinematics.h"

struct ConfigDataSet
{
    double kP = 0.0;
    double kI = 0.0;
    double kD = 0.0;
    double kF = 0.0;
    double motion_cruise_vel = 0.0;
    double motion_accel = 0.0;
    double motion_scurve = 0.0;

    bool forward_soft_limit_enabled = false;
    double forward_soft_limit = 0.0;
    bool reverse_soft_limit_enabled = false;
    double reverse_soft_limit = 0.0;
};


class ArmFailover : public Singleton<ArmFailover>, public FailoverSubsystem
{
    friend Singleton;
protected:
    void init() override;
    void periodic() override;
    void uninit() override;
    void run() override;

private:
    ArmFailover();
    ~ArmFailover();

    void SetBaseConfig(int id, ck::MotorConfiguration::Motor*& config_obj, int current_limit = 40);
    void SetBaseControl(int id, ck::MotorControl::Motor*& control_obj);
    void SetBaseControl(int id, ck::MotorControl::Motor*& control_obj, int master_id);
    void ConfigPID(ck::MotorConfiguration::Motor*& config_obj, ConfigDataSet& d);

    static constexpr int LOWER_ARM_MASTER_ID = 9;
    static constexpr int LOWER_ARM_FOLLOWER_ID = 10;
    static constexpr int UPPER_ARM_MASTER_ID = 11;
    static constexpr int UPPER_ARM_FOLLOWER_ID = 12;
    static constexpr int WRIST_ID = 13;

    static constexpr int LOWER_ARM_CANCODER_ID = 29;
    static constexpr int UPPER_ARM_CANCODER_ID = 31;

    ConfigDataSet lower_arm_pid {0.524, 0.0, 0.33, 0.242, 3600, 3700, 3, true, 8958, true, -13371};
    ConfigDataSet upper_arm_pid {1.7, 0.0, 2.5, 3.91463871, 200, 450, 1, true, -573, true, -3604};
    ConfigDataSet wrist_pid {0.027, 0.0, 0.037, 0.0449122, 19500, 44000, 3, true, 232492, true, -30930};

    ck::MotorConfiguration::Motor* mLowerArmMasterConfig = nullptr;
    ck::MotorConfiguration::Motor* mLowerArmFollowerConfig = nullptr;
    ck::MotorConfiguration::Motor* mUpperArmMasterConfig = nullptr;
    ck::MotorConfiguration::Motor* mUpperArmFollowerConfig = nullptr;
    ck::MotorConfiguration::Motor* mWristConfig = nullptr;

    ck::MotorControl::Motor* mLowerArmMaster = nullptr;
    ck::MotorControl::Motor* mLowerArmFollower = nullptr;
    ck::MotorControl::Motor* mUpperArmMaster = nullptr;
    ck::MotorControl::Motor* mUpperArmFollower = nullptr;
    ck::MotorControl::Motor* mWrist = nullptr;
};