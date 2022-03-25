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
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "RobotDataHelper.hpp"

static frc::SendableChooser<int> autoChooser;
static std::string autoMsg = "AutoSelection";
static bool hasRobotInitialized = false;

static constexpr int ROBOT_PLACED_FINAL_JOYSTICK_HAL_ID = 3;
static constexpr int ROBOT_PLACED_FINAL_BUTTON_HAL_ID = 13;
static bool robotFinalButtonPressed = false;
static bool prevRobotFinalButtonPressed = false;

void performInit()
{
	CKIMUManager::getInstance().forEach([] (uint16_t id, CKIMU* imu, IMUType imuType)
	{
		if (!imu->reset())
		{
			ck::ReportError("Failed to zero IMU: " + id);
		}
	});

	MotorManager::getInstance().forEach([] (uint16_t id, BaseTalon* mCtrl, MotorType motorType)
	{
		if (!ck::runPhoenixFunctionWithRetry([&]() { return mCtrl->SetSelectedSensorPosition(0, 0, ck::kCANTimeoutMs); }, id))
		{
			ck::ReportError("Failed to zero motor: " + id);
		}
	});
}

void initIfNotInit()
{
	if (!hasRobotInitialized)
	{
		performInit();
		hasRobotInitialized = true;
	}
}

Robot::Robot() : TimedRobot(20_ms) {}

void Robot::RobotInit()
{
	// dh.set_generate_core_dump(true);
	
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

	autoChooser.SetDefaultOption("Auto1", 0);
	autoChooser.AddOption("Auto2", 1);
	autoChooser.AddOption("Auto3", 2);
	frc::SmartDashboard::PutData(&autoChooser);
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
	RobotDataHelper::getInstance().setSelectedAuto(autoChooser.GetSelected());

	static int debounceCounter = 0;
	static uint32_t cycleCounter = 0;
	if (!isExternalControl())
	{
		robotFailover.DisabledFailoverPeriodic();
	}

	if (!hasRobotInitialized && cycleCounter++ % 50 == 0)
	{
		performInit();
	}

	if(debounceCounter <= 1 && !hasRobotInitialized && frc::DriverStation::IsJoystickConnected(ROBOT_PLACED_FINAL_JOYSTICK_HAL_ID)) {
		HAL_JoystickButtons rawButtons;
		HAL_GetJoystickButtons(ROBOT_PLACED_FINAL_JOYSTICK_HAL_ID, &rawButtons);
		robotFinalButtonPressed = rawButtons.buttons & (1 << ROBOT_PLACED_FINAL_BUTTON_HAL_ID);
		if (robotFinalButtonPressed && prevRobotFinalButtonPressed != robotFinalButtonPressed)
		{
			debounceCounter++;
		}
		prevRobotFinalButtonPressed = robotFinalButtonPressed;
	}

	if (debounceCounter > 1 && !hasRobotInitialized)
	{
		ck::ReportWarning("Robot Final Position Recorded");
		performInit();
		hasRobotInitialized = true;
		frc::SmartDashboard::PutBoolean("FinalPositionRecorded", true);
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
