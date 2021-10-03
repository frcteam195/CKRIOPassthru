// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.hpp"
#include <functional>
#include <pthread.h>
#include <iostream>
#include "sched.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

Robot::Robot() : TimedRobot(20_ms) {}

void Robot::RobotInit()
{
	//system("sh -c \"ps -o pid,args | grep FRC_Net | grep -v lvuser | grep -v grep | sed s:/.*$::g | sed s:' '::g | xargs -I {} chrt -a -p -f 99 {}\"");
	
	std::string netstrPID = s_exec("ps -o pid,args | grep FRC_Net | grep -v lvuser | grep -v grep | sed s:/.*$::g | sed s:' '::g");
	pid_t netPID = std::stoi(netstrPID);
	struct sched_param sp_set;
	sp_set.sched_priority = 99;
	int ret = sched_setscheduler(netPID, SCHED_FIFO, &sp_set);


	void* ctx = zmq_ctx_new();
	std::cout << "ZMQ Context Created!" << std::endl;
	zmq_ctx_destroy(ctx);
	int policy = sched_getscheduler(netPID);

	struct sched_param sp;
	ret = sched_getparam(netPID, &sp);
	std::cout << "rtprio: " << sp.sched_priority << std::endl;
	switch(policy) {
		case SCHED_OTHER: printf("SCHED_OTHER\n"); break;
		case SCHED_RR:   printf("SCHED_RR\n"); break;
		case SCHED_FIFO:  printf("SCHED_FIFO\n"); break;
		default:   printf("Unknown...\n");
	}


	///////////////////////////
	//Set Main Thread Priority
	//Must also set configuration for rtprio 99, priority -20, and nice -20 in /etc/security/limits.conf
	sched_param sch;
	sch.sched_priority = 98;
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

std::string Robot::s_exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

#ifndef RUNNING_FRC_TESTS
int main()
{
	return frc::StartRobot<Robot>();
}
#endif
