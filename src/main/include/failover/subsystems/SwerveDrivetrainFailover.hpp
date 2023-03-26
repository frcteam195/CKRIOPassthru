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


class SwerveDrivetrainFailover : public Singleton<SwerveDrivetrainFailover>, public FailoverSubsystem
{
    friend Singleton;
protected:
    void init() override;
    void periodic() override;
    void uninit() override;
    void run() override;

private:
    SwerveDrivetrainFailover();
    ~SwerveDrivetrainFailover();

    void SetDriveConfig(int id, ck::MotorConfiguration::Motor*& config_obj, ck::MotorControl::Motor*& control_obj);
    void SetSteeringConfig(int id, int cancoder_id, ck::MotorConfiguration::Motor*& config_obj, ck::MotorControl::Motor*& control_obj);

    frc::Joystick* mJoystick;

    static constexpr int FRONT_LEFT_DRIVE_ID = 1;
    static constexpr int FRONT_RIGHT_DRIVE_ID = 2;
    static constexpr int BACK_LEFT_DRIVE_ID = 3;
    static constexpr int BACK_RIGHT_DRIVE_ID = 4;
    static constexpr int FRONT_LEFT_STEERING_ID = 5;
    static constexpr int FRONT_RIGHT_STEERING_ID = 6;
    static constexpr int BACK_LEFT_STEERING_ID = 7;
    static constexpr int BACK_RIGHT_STEERING_ID = 8;

    static constexpr int FRONT_LEFT_STEERING_CANCODER_ID = 25;
    static constexpr int FRONT_RIGHT_STEERING_CANCODER_ID = 26;
    static constexpr int BACK_LEFT_STEERING_CANCODER_ID = 27;
    static constexpr int BACK_RIGHT_STEERING_CANCODER_ID = 28;

    static constexpr int DRIVE_JOYSTICK_ID = 0;
    static constexpr double DRIVE_JOYSTICK_DEADBAND = 0.1;
    static constexpr int DRIVE_JOYSTICK_X_AXIS = 0;
    static constexpr int DRIVE_JOYSTICK_Y_AXIS = 1;
    static constexpr int DRIVE_JOYSTICK_Z_AXIS = 3;

    frc::Translation2d m_frontLeftLocation{9.375_in, -9.375_in};
    frc::Translation2d m_frontRightLocation{9.375_in, 9.375_in};
    frc::Translation2d m_backLeftLocation{-9.375_in, -9.375_in};
    frc::Translation2d m_backRightLocation{-9.375_in, 9.375_in};

    units::inch_t DRIVE_WHEEL_DIAMETER = 3.9_in;
    double DRIVE_GEAR_RATIO = 6.75;
    double STEERING_GEAR_RATIO = 1.0;

    double drive_velocity_kP = 0.07;
    double drive_velocity_kI = 0.0;
    double drive_velocity_kD = 0.43;
    double drive_velocity_kF = 0.0469;

    double steering_velocity_kP = 2.1;
    double steering_velocity_kI = 0.0;
    double steering_velocity_kD = 3.0;
    double steering_velocity_kF = 0.0;

    double meters_per_second_to_native_units_drive_velocity(units::meters_per_second_t mps);
    double degrees_to_native_units_steering_angle(frc::Rotation2d rotation);

    frc::SwerveDriveKinematics<4> m_kinematics{ m_frontLeftLocation, m_frontRightLocation, m_backLeftLocation, m_backRightLocation };

    ck::MotorConfiguration::Motor* mFrontLeftDriveConfig = nullptr;
    ck::MotorConfiguration::Motor* mFrontRightDriveConfig = nullptr;
    ck::MotorConfiguration::Motor* mBackLeftDriveConfig = nullptr;
    ck::MotorConfiguration::Motor* mBackRightDriveConfig = nullptr;
    ck::MotorConfiguration::Motor* mFrontLeftSteeringConfig = nullptr;
    ck::MotorConfiguration::Motor* mFrontRightSteeringConfig = nullptr;
    ck::MotorConfiguration::Motor* mBackLeftSteeringConfig = nullptr;
    ck::MotorConfiguration::Motor* mBackRightSteeringConfig = nullptr;

    ck::MotorControl::Motor* mFrontLeftDrive = nullptr;
    ck::MotorControl::Motor* mFrontRightDrive = nullptr;
    ck::MotorControl::Motor* mBackLeftDrive = nullptr;
    ck::MotorControl::Motor* mBackRightDrive = nullptr;
    ck::MotorControl::Motor* mFrontLeftSteering = nullptr;
    ck::MotorControl::Motor* mFrontRightSteering = nullptr;
    ck::MotorControl::Motor* mBackLeftSteering = nullptr;
    ck::MotorControl::Motor* mBackRightSteering = nullptr;

    double mSteeringAngles[4] = {0.0, 0.0, 0.0, 0.0};
};