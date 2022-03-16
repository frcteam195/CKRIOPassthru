#include "Robot.hpp"
#include "utils/GlobalConfig.hpp"
#include "utils/CKLogger.hpp"
#include <iostream>
#include "ExternalControlManager.hpp"
#include "frc/RobotController.h"
#include "CKCANServer.hpp"
#include "ctre/phoenix/unmanaged/Unmanaged.h"
#include "utils/CKErrors.hpp"
#include <thread>
#include <chrono>
#include "MotorManager.hpp"
#include "utils/PhoenixHelper.hpp"

static bool hasRobotInitialized = false;
void initIfNotInit()
{
	if (!hasRobotInitialized)
	{
		MotorManager::getInstance().forEach([] (uint16_t id, BaseTalon* mCtrl, MotorType motorType)
		{
			if (!ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetSelectedSensorPosition(0, 0, ck::kCANTimeoutMs); }, id))
			{
				ck::ReportError("Failed to zero motor: " + id);
			}
		});
		hasRobotInitialized = true;
	}
}

Robot::Robot() : TimedRobot(20_ms) {}

void Robot::RobotInit()
{
	//Restart CANivore to mitigate CPU spike bug. TODO: Needs testing
	ck::resetCANivore();
	ThreadRateControl trc;
	trc.start();
	trc.doRateControl(8000);	//Wait for CANivore reset and Phoenix init


	frc::RobotController::SetBrownoutVoltage(4.5_V);
	frc::LiveWindow::DisableAllTelemetry();
	ctre::phoenix::unmanaged::Unmanaged::LoadPhoenix();

	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::DISABLED);

	ck::configureSystemPriority();
	ck::configureThreadPriority(98);

	//Register Tasks
	TaskScheduler::getInstance().scheduleTask(receiveMessagesTask);
	TaskScheduler::getInstance().scheduleTask(applyIMUConfigTask);
	TaskScheduler::getInstance().scheduleTask(applyMotorValuesTask);
	TaskScheduler::getInstance().scheduleTask(applyMotorConfigTask);
	TaskScheduler::getInstance().scheduleTask(applySolenoidValuesTask);
	// TaskScheduler::getInstance().scheduleTask(sendLEDOutputTask);
	TaskScheduler::getInstance().scheduleTask(sendRobotDataTask);
	TaskScheduler::getInstance().scheduleTask(sendIMUDataTask);
	TaskScheduler::getInstance().scheduleTask(sendMotorDataTask);
	// TaskScheduler::getInstance().scheduleTask(sendSolenoidDataTask);
	TaskScheduler::getInstance().scheduleTask(sendJoystickDataTask);
	TaskScheduler::getInstance().scheduleTask(processHeartbeatTask);

#ifdef ENABLE_TASK_TIME_REPORTING
	TaskScheduler::getInstance().scheduleTask(taskTimingReporterTask);
#endif 

	//Initialize the CAN Server
	// std::cout << "Starting CK CAN Server..." << std::endl;
	// CKCANServer::getInstance();
	// std::cout << "CK CAN Server started!" << std::endl;

	//Start Scheduler
	std::cout << "Starting Task Scheduler..." << std::endl;
	TaskScheduler::getInstance().start();
	std::cout << "Task Scheduler started!" << std::endl;

	std::cout << "Initialized successfully. Entering run..." << std::endl;

}
void Robot::RobotPeriodic() {

	RobotControlModeHelper::getInstance().setDSAttached(frc::DriverStation::IsDSAttached());

	if (isExternalControl())
	{
		if (failoverActive)
		{
			if (RobotControlModeHelper::getInstance().isDSAttached())
			{
				ck::ReportWarning("[rosdiag] ROS Connection Resumed");
			}
			robotFailover.Reset();
			failoverActive = false;
		}
		else
		{
			if (!initialStartupROSCompleted && RobotControlModeHelper::getInstance().isDSAttached() && !failoverActive)
			{
				ck::ReportWarning("[rosdiag] ROS Connection Resumed");
				initialStartupROSCompleted = true;
			}
		}
	}
	else
	{
		if (!failoverActive)
		{
			if (RobotControlModeHelper::getInstance().isDSAttached())
			{
				ck::ReportWarning("[rosdiag] Failover Control Activated");
			}
			robotFailover.RobotFailoverInit();
			failoverActive = true;
		}
		robotFailover.RobotFailoverPeriodic();
	}
	
}

void Robot::AutonomousInit()
{
	initIfNotInit();

	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::AUTONOMOUS);
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
	initIfNotInit();

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
