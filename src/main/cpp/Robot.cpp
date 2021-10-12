// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.hpp"
#include "utils/ThreadPriorityHelper.hpp"
#include <string>
#include <iostream>
#include "utils/RobotControlModeHelper.hpp"

Robot::Robot() : TimedRobot(20_ms) {}

void Robot::RobotInit()
{
	RobotControlModeHelper::getInstance().setControlMode(CONTROL_MODE::DISABLED);

	ck::configureSystemPriority();
	ck::configureThreadPriority(98);

	//Register Tasks
	TaskScheduler::getInstance().scheduleTask(sendMotorValuesTask);
	TaskScheduler::getInstance().scheduleTask(applyMotorConfigTask);
	TaskScheduler::getInstance().scheduleTask(receiveMessagesTask);
	TaskScheduler::getInstance().scheduleTask(sendSensorDataTask);
	TaskScheduler::getInstance().scheduleTask(sendRobotDataTask);
	TaskScheduler::getInstance().scheduleTask(joystickManagerTask);

	//Start Scheduler
	TaskScheduler::getInstance().start();
}
void Robot::RobotPeriodic() {
	if (testCounter++ % 50 == 0) {
		std::cout << "Test" << std::endl;
	}
}

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
