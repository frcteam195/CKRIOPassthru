#define EXTERNAL_PHOENIX_CONTROL

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
#include "CKIMUManager.hpp"
#include "utils/PhoenixHelper.hpp"
#include <hal/DriverStation.h>
#include <hal/DriverStationTypes.h>
#include "RobotDataHelper.hpp"
#include "utils/CKSendable.hpp"
#include "utils/AutoStartPositionAnglePair.hpp"

#include "networktables/NetworkTableInstance.h"

Robot::Robot() : TimedRobot(20_ms), dh()
{
	// //Restart CANivore to mitigate CPU spike bug. TODO: Needs testing
	ck::resetCANivore();

#ifndef EXTERNAL_PHOENIX_CONTROL
	ThreadRateControl trc;
	trc.start();
	trc.doRateControl(2000);	//Wait for CANivore reset and Phoenix init
#endif

	dh.set_cleanup(false);
	dh.set_generate_core_dump(false);
	dh.set_color_output(false);
	dh.set_cut_common_path_root(true);
	dh.set_cut_relative_paths(true);
	dh.set_frames_count(10);
}

void Robot::RobotInit()
{	
	frc::RobotController::SetBrownoutVoltage(4.5_V);
	frc::LiveWindow::DisableAllTelemetry();

#ifdef EXTERNAL_PHOENIX_CONTROL
	ctre::phoenix::unmanaged::Unmanaged::SetPhoenixDiagnosticsStartTime(-1);
#endif

	ctre::phoenix::unmanaged::Unmanaged::LoadPhoenix();

	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::DISABLED);

	ck::configureSystemPriority();
	ck::configureThreadPriority(98);

	//Register Tasks
	TaskScheduler::getInstance().scheduleTask(receiveMessagesTask);

#ifndef EXTERNAL_PHOENIX_CONTROL
	// TaskScheduler::getInstance().scheduleTask(applyEncoderConfigTask);
	TaskScheduler::getInstance().scheduleTask(applyIMUConfigTask);
	TaskScheduler::getInstance().scheduleTask(applyMotorValuesTask);
	TaskScheduler::getInstance().scheduleTask(applyMotorConfigTask);
	// TaskScheduler::getInstance().scheduleTask(sendEncoderDataTask);
	TaskScheduler::getInstance().scheduleTask(sendIMUDataTask);
	TaskScheduler::getInstance().scheduleTask(sendMotorDataTask);
	TaskScheduler::getInstance().scheduleTask(applyLEDControlTask);
#endif

	
	// TaskScheduler::getInstance().scheduleTask(sendSolenoidDataTask);
	TaskScheduler::getInstance().scheduleTask(applySolenoidValuesTask);
	TaskScheduler::getInstance().scheduleTask(sendRobotDataTask);
	TaskScheduler::getInstance().scheduleTask(sendJoystickDataTask);
	TaskScheduler::getInstance().scheduleTask(processHeartbeatTask);
	TaskScheduler::getInstance().scheduleTask(updateDSConnectionStatusTask);

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
				ck::ReportError("[rosdiag] ROS Connection Resumed");
			}
			robotFailover.Reset();
			failoverActive = false;
		}
		else
		{
			if (!initialStartupROSCompleted && RobotControlModeHelper::getInstance().isDSAttached() && !failoverActive)
			{
				ck::ReportError("[rosdiag] ROS Connection Resumed");
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
				ck::ReportError("[rosdiag] Failover Control Activated");
			}
			robotFailover.RobotFailoverInit();
			failoverActive = true;
		}
		robotFailover.RobotFailoverPeriodic();
	}
	
}

void Robot::AutonomousInit()
{
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
