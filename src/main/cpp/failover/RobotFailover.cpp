#include "failover/RobotFailover.hpp"
#include "failover/DrivetrainFailover.hpp"

void RobotFailover::RobotFailoverInit()
{
    DrivetrainFailover::getInstance().SubsystemInit();
    lCtrl = new CKLEDController(1);
    lCtrl->setColor(RGBColor{0, 128, 0, 255});
    lCtrl->setBrightness(255);
    lCtrl->configureBlink(10, 500);
    lCtrl->setCommLoss();
    // lCtrl->setDefaultState(CKLEDController::LEDState::FIXED_ON);
    // lCtrl->setCommRestored();
    // lCtrl->setBlink();
    // lCtrl->setMorse("SOS");
    // lCtrl->setOn();
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