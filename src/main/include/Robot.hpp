// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <utils/ckrobot/TimedRobot.hpp>
#include <string>
#include <iostream>
#include "utils/RobotControlModeHelper.hpp"
#include "utils/TaskScheduler.hpp"
#include "utils/SystemHelper.hpp"
#include "tasks/ReceiveMessagesTask.hpp"
#include "tasks/ApplySolenoidValuesTask.hpp"
#include "tasks/ApplyEncoderConfigTask.hpp"
#include "tasks/ApplyLEDControlTask.hpp"
#include "tasks/ApplyIMUConfigTask.hpp"
#include "tasks/ApplyMotorValuesTask.hpp"
#include "tasks/ApplyMotorConfigTask.hpp"
#include "tasks/SendRobotDataTask.hpp"
#include "tasks/SendIMUDataTask.hpp"
#include "tasks/SendEncoderDataTask.hpp"
#include "tasks/SendMotorDataTask.hpp"
#include "tasks/SendSolenoidDataTask.hpp"
#include "tasks/SendJoystickDataTask.hpp"
#include "tasks/ProcessHeartbeatTask.hpp"
#include "tasks/TaskTimingReporterTask.hpp"
#include "tasks/UpdateDSConnectionStatusTask.hpp"
#include "frc/livewindow/LiveWindow.h"
#include "failover/RobotFailover.hpp"

#include "utils/death_handler.h"

class Robot : public ck::TimedRobot {
public:
	Robot();

	void RobotInit() override;
	void RobotPeriodic() override;
	
	void AutonomousInit() override;
	void AutonomousPeriodic() override;

	void TeleopInit() override;
	void TeleopPeriodic() override;

	void DisabledInit() override;
	void DisabledPeriodic() override;

	void TestInit() override;
	void TestPeriodic() override;

private:
	//Declare tasks as member variables in Robot cpp so they never go out of scope
	ReceiveMessagesTask receiveMessagesTask;
	ApplyIMUConfigTask applyIMUConfigTask;
	ApplyEncoderConfigTask applyEncoderConfigTask;
	ApplyLEDControlTask applyLEDControlTask;
	ApplyMotorConfigTask applyMotorConfigTask;
	ApplyMotorValuesTask applyMotorValuesTask;
	ApplySolenoidValuesTask applySolenoidValuesTask;
	SendRobotDataTask sendRobotDataTask;
	SendIMUDataTask sendIMUDataTask;
	SendEncoderDataTask sendEncoderDataTask;
	SendMotorDataTask sendMotorDataTask;
	SendSolenoidDataTask sendSolenoidDataTask;
	SendJoystickDataTask sendJoystickDataTask;
	ProcessHeartbeatTask processHeartbeatTask;
	UpdateDSConnectionStatusTask updateDSConnectionStatusTask;
 
	RobotFailover robotFailover;
	bool failoverActive = false;
	bool initialStartupROSCompleted = false;

	Debug::DeathHandler dh;

#ifdef ENABLE_TASK_TIME_REPORTING
	TaskTimingReporterTask taskTimingReporterTask;
#endif

};
