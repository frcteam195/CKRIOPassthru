#include "failover/RobotFailover.hpp"
#include "failover/subsystems/SwerveDrivetrainFailover.hpp"
#include "failover/subsystems/ArmFailover.hpp"
#include "failover/subsystems/IntakeFailover.hpp"
#include "failover/subsystems/LEDFailover.hpp"
#include "failover/FailoverMessageManager.hpp"

void RobotFailover::RobotFailoverInit()
{
    SwerveDrivetrainFailover::getInstance().SubsystemInit();
    ArmFailover::getInstance().SubsystemInit();
    IntakeFailover::getInstance().SubsystemInit();
    LEDFailover::getInstance().SubsystemInit();
}

void RobotFailover::RobotFailoverPeriodic()
{
    //Make sure this is called to keep motors registered
    FailoverMessageManager::getInstance().publishMessages();

    SwerveDrivetrainFailover::getInstance().SubsystemPeriodic();
    ArmFailover::getInstance().SubsystemPeriodic();
    IntakeFailover::getInstance().SubsystemPeriodic();
    LEDFailover::getInstance().SubsystemPeriodic();
}

void RobotFailover::AutonomousFailoverInit() {}
void RobotFailover::AutonomousFailoverPeriodic() {}

void RobotFailover::TeleopFailoverInit()
{

}

void RobotFailover::TeleopFailoverPeriodic()
{
    SwerveDrivetrainFailover::getInstance().SubsystemRun();
    ArmFailover::getInstance().SubsystemRun();
    IntakeFailover::getInstance().SubsystemRun();
    LEDFailover::getInstance().SubsystemRun();
}

void RobotFailover::DisabledFailoverInit() {}
void RobotFailover::DisabledFailoverPeriodic() {}


void RobotFailover::Reset()
{
    SwerveDrivetrainFailover::getInstance().SubsystemReset();
    ArmFailover::getInstance().SubsystemReset();
    IntakeFailover::getInstance().SubsystemReset();
    LEDFailover::getInstance().SubsystemReset();
}


RobotFailover::RobotFailover()
{
    SwerveDrivetrainFailover::getInstance();
    ArmFailover::getInstance();
    IntakeFailover::getInstance();
    LEDFailover::getInstance();
}