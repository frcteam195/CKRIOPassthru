// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.hpp"
#include "utils/ThreadPriorityHelper.hpp"
#include <string>
#include <iostream>

Robot::Robot() : TimedRobot(20_ms) {}

void Robot::RobotInit()
{
	ck::configureSystemPriority();
	ck::configureThreadPriority(98);

	//Register Tasks
	// TaskScheduler::getInstance().scheduleTask(receiveMessagesTask);
	// TaskScheduler::getInstance().scheduleTask(sendMotorValuesTask);
	// TaskScheduler::getInstance().scheduleTask(sendMotorConfigTask);
	TaskScheduler::getInstance().scheduleTask(readSensorDataTask);
	TaskScheduler::getInstance().scheduleTask(sendOutboundDataTask);

	//Start Scheduler
	TaskScheduler::getInstance().start();
}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {}

void Robot::DisabledInit() {}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main()
{
	return frc::StartRobot<Robot>();
}
#endif
