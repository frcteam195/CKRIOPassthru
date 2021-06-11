// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.hpp"
#include <functional>
#include <pthread.h>
#include <iostream>

Robot::Robot() : TimedRobot(20_ms) {}

void Robot::RobotInit()
{
	///////////////////////////
	//Set Main Thread Priority
	//Must also set configuration for rtprio 99, priority -20, and nice -20 in /etc/security/limits.conf
	sched_param sch;
	sch.sched_priority = 99;
	if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch) != 0)
	{
		std::cout << "Error setting main thread priority!" << std::endl;
	}
	///////////////////////////


	//Register Tasks
	TaskScheduler::getInstance().scheduleTask(sendMotorValuesTask, 5);
	TaskScheduler::getInstance().scheduleTask(sendMotorConfigTask, 100);
	TaskScheduler::getInstance().scheduleTask(readSensorDataTask, 10);
	TaskScheduler::getInstance().scheduleTask(sendOutboundDataTask, 10);

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
