// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include "utils/TaskScheduler.hpp"
#include "tasks/ReceiveMessagesTask.hpp"
#include "tasks/SendMotorValuesTask.hpp"
#include "tasks/SendMotorConfigTask.hpp"
#include "tasks/SendOutboundDataTask.hpp"
#include "tasks/ReadSensorDataTask.hpp"

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
	SendMotorValuesTask sendMotorValuesTask;
	SendMotorConfigTask sendMotorConfigTask;
	SendOutboundDataTask sendOutboundDataTask;
	ReadSensorDataTask readSensorDataTask;
};
