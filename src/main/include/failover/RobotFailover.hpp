#pragma once

#include "utils/drivers/CKLEDController.hpp"


class RobotFailover {
public:
	RobotFailover();

    void RobotFailoverInit();
	void RobotFailoverPeriodic();
	
	void AutonomousFailoverInit();
	void AutonomousFailoverPeriodic();

	void TeleopFailoverInit();
	void TeleopFailoverPeriodic();

	void DisabledFailoverInit();
	void DisabledFailoverPeriodic();

	void Reset();

private:
	CKLEDController* lCtrl;

};
