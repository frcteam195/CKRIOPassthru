#include "Robot.hpp"

Robot::Robot() : TimedRobot(20_ms) {}

void Robot::RobotInit()
{
	frc::LiveWindow::GetInstance()->DisableAllTelemetry();

	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::DISABLED);

	ck::configureSystemPriority();
	ck::configureThreadPriority(98);

	//Register Tasks
	TaskScheduler::getInstance().scheduleTask(receiveMessagesTask);
	TaskScheduler::getInstance().scheduleTask(applyMotorConfigTask);
	TaskScheduler::getInstance().scheduleTask(applyMotorValuesTask);
	TaskScheduler::getInstance().scheduleTask(sendRobotDataTask);
	TaskScheduler::getInstance().scheduleTask(sendSensorDataTask);
	TaskScheduler::getInstance().scheduleTask(sendJoystickDataTask);
	TaskScheduler::getInstance().scheduleTask(processHeartbeatTask);

#ifdef ENABLE_TASK_TIME_REPORTING
	TaskScheduler::getInstance().scheduleTask(taskTimingReporterTask);
#endif 

	//Start Scheduler
	TaskScheduler::getInstance().start();
}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit()
{
	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::AUTONOMOUS);
}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit()
{
	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::TELEOP);
}
void Robot::TeleopPeriodic() {}

void Robot::DisabledInit()
{
	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::DISABLED);

}
void Robot::DisabledPeriodic() {}

void Robot::TestInit()
{
	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::TEST);
}
void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main()
{
	return frc::StartRobot<Robot>();
}
#endif
