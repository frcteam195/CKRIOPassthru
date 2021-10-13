// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include <string>
#include <iostream>
#include "utils/RobotControlModeHelper.hpp"
#include "utils/TaskScheduler.hpp"
#include "utils/ThreadPriorityHelper.hpp"
#include "tasks/ReceiveMessagesTask.hpp"
#include "tasks/ApplyMotorValuesTask.hpp"
#include "tasks/ApplyMotorConfigTask.hpp"
#include "tasks/SendRobotDataTask.hpp"
#include "tasks/SendSensorDataTask.hpp"
#include "tasks/SendJoystickDataTask.hpp"
#include "tasks/ProcessHeartbeatTask.hpp"
#include "tasks/TaskTimingReporterTask.hpp"

class Robot : public frc::TimedRobot {
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
	ApplyMotorConfigTask applyMotorConfigTask;
	ApplyMotorValuesTask applyMotorValuesTask;
	SendRobotDataTask sendRobotDataTask;
	SendSensorDataTask sendSensorDataTask;
	SendJoystickDataTask sendJoystickDataTask;
	ProcessHeartbeatTask processHeartbeatTask;

#ifdef ENABLE_TASK_TIME_REPORTING
	TaskTimingReporterTask taskTimingReporterTask;
#endif

};
