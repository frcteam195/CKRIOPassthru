#include "Robot.hpp"
#include "utils/GlobalConfig.hpp"
#include "utils/CKLogger.hpp"
#include <iostream>
#include "NavXManager.hpp"
#include "ExternalControlManager.hpp"
#include "frc/Errors.h"

Robot::Robot() : TimedRobot(20_ms) {}

void Robot::RobotInit()
{
	frc::LiveWindow::DisableAllTelemetry();

	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::DISABLED);

	ck::configureSystemPriority();
	ck::configureThreadPriority(98);

	//Register Tasks
	TaskScheduler::getInstance().scheduleTask(receiveMessagesTask);
	TaskScheduler::getInstance().scheduleTask(applyMotorConfigTask);
	TaskScheduler::getInstance().scheduleTask(applyMotorValuesTask);
	TaskScheduler::getInstance().scheduleTask(sendRobotDataTask);
	TaskScheduler::getInstance().scheduleTask(sendIMUDataTask);
	TaskScheduler::getInstance().scheduleTask(sendSensorDataTask);
	TaskScheduler::getInstance().scheduleTask(sendJoystickDataTask);
	TaskScheduler::getInstance().scheduleTask(processHeartbeatTask);

#ifdef ENABLE_TASK_TIME_REPORTING
	TaskScheduler::getInstance().scheduleTask(taskTimingReporterTask);
#endif 

	//Start Scheduler
	TaskScheduler::getInstance().start();

	std::cout << "Initialized successfully. Entering run..." << std::endl;

	NavXManager::getInstance().getNavX().zeroYaw();
}
void Robot::RobotPeriodic() {
	if (isExternalControl())
	{
		if (failoverActive)
		{
			if (frc::DriverStation::IsDSAttached())
			{
				FRC_ReportError(1, "{}", "ROS Connection Resumed");
			}
			//std::cout << "ROS Connection Resumed" << std::endl;
			robotFailover.Reset();
			failoverActive = false;
		}
	}
	else
	{
		if (!failoverActive)
		{
			if (frc::DriverStation::IsDSAttached())
			{
				FRC_ReportError(1, "{}", "Failover Control Activated");
			}
			//std::cout << "Failover Control Activated" << std::endl;
			robotFailover.RobotFailoverInit();
			failoverActive = true;
		}
		robotFailover.RobotFailoverPeriodic();
	}
	
}

void Robot::AutonomousInit()
{
	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::AUTONOMOUS);
	NavXManager::getInstance().getNavX().zeroYaw();
	if (!isExternalControl())
	{
		robotFailover.AutonomousFailoverInit();
	}
}
void Robot::AutonomousPeriodic()
{
	if (!isExternalControl())
	{
		robotFailover.AutonomousFailoverPeriodic();
	}
}

void Robot::TeleopInit()
{
	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::TELEOP);
	if (!isExternalControl())
	{
		robotFailover.TeleopFailoverInit();
	}
}
void Robot::TeleopPeriodic()
{
	if (!isExternalControl())
	{
		robotFailover.TeleopFailoverPeriodic();
	}
}

void Robot::DisabledInit()
{
	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::DISABLED);
	if (!isExternalControl())
	{
		robotFailover.DisabledFailoverInit();
	}
}
void Robot::DisabledPeriodic()
{
	if (!isExternalControl())
	{
		robotFailover.DisabledFailoverPeriodic();
	}
}

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
