#pragma once

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

private:
};
