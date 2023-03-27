#pragma once
#include "utils/Singleton.hpp"
#include "failover/utils/FailoverSubsystem.hpp"
#include "frc/Joystick.h"
#include "MotorManager.hpp"

#include "MotorControl.pb.h"
#include "MotorConfiguration.pb.h"
#include "SolenoidControl.pb.h"

#include <cstdint>
#include "frc/geometry/Translation2d.h"
#include "frc/kinematics/SwerveDriveKinematics.h"

class IntakeFailover : public Singleton<IntakeFailover>, public FailoverSubsystem
{
    friend Singleton;
protected:
    void init() override;
    void periodic() override;
    void uninit() override;
    void run() override;

private:
    IntakeFailover();
    ~IntakeFailover();

    void SetBaseConfig(int id, ck::MotorConfiguration::Motor*& config_obj, int current_limit = 40);
    void SetBaseControl(int id, ck::MotorControl::Motor*& control_obj);

    static constexpr int INTAKE_ID = 14;

    static constexpr int PINCH_SOLENOID_ID = 1;
    static constexpr int PINCH_SOLENOID_MODULE_ID = 1;

    ck::MotorConfiguration::Motor* mIntakeConfig = nullptr;

    ck::MotorControl::Motor* mIntake = nullptr;

    ck::SolenoidControl::Solenoid* mPinch = nullptr;
};