// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "utils/ckrobot/IterativeRobotBase.hpp"

#include <fmt/format.h>
#include <hal/DriverStation.h>
#include <networktables/NetworkTableInstance.h>

#include "frc/DSControlWord.h"
#include "frc/Errors.h"
#include "frc/livewindow/LiveWindow.h"
#include "frc/shuffleboard/Shuffleboard.h"
#include "frc/smartdashboard/SmartDashboard.h"
#include "utils/CKErrors.hpp"
#include <sstream>

using namespace frc;
namespace ck
{
	IterativeRobotBase::IterativeRobotBase(double period)
		: IterativeRobotBase(units::second_t(period)) {}

	IterativeRobotBase::IterativeRobotBase(units::second_t period)
		: m_period(period) {}

	void IterativeRobotBase::RobotInit() {}

	void IterativeRobotBase::SimulationInit() {}

	void IterativeRobotBase::DisabledInit() {}

	void IterativeRobotBase::AutonomousInit() {}

	void IterativeRobotBase::TeleopInit() {}

	void IterativeRobotBase::TestInit() {}

	void IterativeRobotBase::RobotPeriodic()
	{
		static bool firstRun = true;
		if (firstRun)
		{
			fmt::print("Default {}() method... Override me!\n", __FUNCTION__);
			firstRun = false;
		}
	}

	void IterativeRobotBase::SimulationPeriodic()
	{
		static bool firstRun = true;
		if (firstRun)
		{
			fmt::print("Default {}() method... Override me!\n", __FUNCTION__);
			firstRun = false;
		}
	}

	void IterativeRobotBase::DisabledPeriodic()
	{
		static bool firstRun = true;
		if (firstRun)
		{
			fmt::print("Default {}() method... Override me!\n", __FUNCTION__);
			firstRun = false;
		}
	}

	void IterativeRobotBase::AutonomousPeriodic()
	{
		static bool firstRun = true;
		if (firstRun)
		{
			fmt::print("Default {}() method... Override me!\n", __FUNCTION__);
			firstRun = false;
		}
	}

	void IterativeRobotBase::TeleopPeriodic()
	{
		static bool firstRun = true;
		if (firstRun)
		{
			fmt::print("Default {}() method... Override me!\n", __FUNCTION__);
			firstRun = false;
		}
	}

	void IterativeRobotBase::TestPeriodic()
	{
		static bool firstRun = true;
		if (firstRun)
		{
			fmt::print("Default {}() method... Override me!\n", __FUNCTION__);
			firstRun = false;
		}
	}

	void IterativeRobotBase::DisabledExit() {}

	void IterativeRobotBase::AutonomousExit() {}

	void IterativeRobotBase::TeleopExit() {}

	void IterativeRobotBase::TestExit() {}

	void IterativeRobotBase::SetNetworkTablesFlushEnabled(bool enabled)
	{
		m_ntFlushEnabled = enabled;
	}

	units::second_t IterativeRobotBase::GetPeriod() const
	{
		return m_period;
	}

	void IterativeRobotBase::LoopFunc()
	{
		// Get current mode
		DSControlWord word;
		Mode mode = Mode::kNone;
		if (word.IsDisabled())
		{
			mode = Mode::kDisabled;
		}
		else if (word.IsAutonomous())
		{
			mode = Mode::kAutonomous;
		}
		else if (word.IsTeleop())
		{
			mode = Mode::kTeleop;
		}
		else if (word.IsTest())
		{
			mode = Mode::kTest;
		}

		// If mode changed, call mode exit and entry functions
		if (m_lastMode != mode)
		{
			// Call last mode's exit function
			if (m_lastMode == Mode::kDisabled)
			{
				DisabledExit();
			}
			else if (m_lastMode == Mode::kAutonomous)
			{
				AutonomousExit();
			}
			else if (m_lastMode == Mode::kTeleop)
			{
				TeleopExit();
			}
			else if (m_lastMode == Mode::kTest)
			{
				LiveWindow::SetEnabled(false);
				Shuffleboard::DisableActuatorWidgets();
				TestExit();
			}

			// Call current mode's entry function
			if (mode == Mode::kDisabled)
			{
				DisabledInit();
			}
			else if (mode == Mode::kAutonomous)
			{
				AutonomousInit();
			}
			else if (mode == Mode::kTeleop)
			{
				TeleopInit();
			}
			else if (mode == Mode::kTest)
			{
				// LiveWindow::SetEnabled(true);
				// Shuffleboard::EnableActuatorWidgets();
				TestInit();
			}

			m_lastMode = mode;
		}

		// Call the appropriate function depending upon the current robot mode
		if (mode == Mode::kDisabled)
		{
			HAL_ObserveUserProgramDisabled();
			DisabledPeriodic();
		}
		else if (mode == Mode::kAutonomous)
		{
			HAL_ObserveUserProgramAutonomous();
			AutonomousPeriodic();
		}
		else if (mode == Mode::kTeleop)
		{
			HAL_ObserveUserProgramTeleop();
			TeleopPeriodic();
		}
		else if (mode == Mode::kTest)
		{
			HAL_ObserveUserProgramTest();
			TestPeriodic();
		}

		RobotPeriodic();

		//   SmartDashboard::UpdateValues();
		//   m_watchdog.AddEpoch("SmartDashboard::UpdateValues()");
		//   LiveWindow::UpdateValues();
		//   m_watchdog.AddEpoch("LiveWindow::UpdateValues()");
		//   Shuffleboard::Update();
		//   m_watchdog.AddEpoch("Shuffleboard::Update()");

		if constexpr (IsSimulation())
		{
			HAL_SimPeriodicBefore();
			SimulationPeriodic();
			HAL_SimPeriodicAfter();
		}

		// Flush NetworkTables
		if (m_ntFlushEnabled)
		{
			nt::NetworkTableInstance::GetDefault().Flush();
		}
	}
}