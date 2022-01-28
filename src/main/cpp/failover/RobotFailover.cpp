#include "failover/RobotFailover.hpp"
#include "failover/DrivetrainFailover.hpp"


void RobotFailover::RobotFailoverInit()
{
    DrivetrainFailover::getInstance().SubsystemInit();
}

void RobotFailover::RobotFailoverPeriodic()
{
    //Make sure this is called to keep motors registered
    DrivetrainFailover::getInstance().SubsystemPeriodic();
}

void RobotFailover::AutonomousFailoverInit() {}
void RobotFailover::AutonomousFailoverPeriodic() {}

void RobotFailover::TeleopFailoverInit()
{

}

void RobotFailover::TeleopFailoverPeriodic()
{
    DrivetrainFailover::getInstance().SubsystemRun();
}

void RobotFailover::DisabledFailoverInit() {}
void RobotFailover::DisabledFailoverPeriodic() {}


void RobotFailover::Reset()
{
    DrivetrainFailover::getInstance().SubsystemReset();
}


RobotFailover::RobotFailover() {}