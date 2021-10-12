#include "tasks/SendSensorDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>
#include "MotorManager.hpp"
#include "NetworkManager.hpp"

SendSensorDataTask::SendSensorDataTask() : Task(THREAD_RATE_MS), mMotorStatusMsg()
{
    mMotorStatusBuf = malloc(MOTOR_STATUS_MESSAGE_SIZE * sizeof(uint8_t));
    memset(mMotorStatusBuf, 0, MOTOR_STATUS_MESSAGE_SIZE * sizeof(uint8_t));
}

void SendSensorDataTask::run(uint32_t timeSinceLastUpdateMs)
{
    mMotorStatusMsg.Clear();

    MotorManager::getInstance().forEach([&] (uint16_t id, BaseMotorController* mCtrl, MotorType mType)
    {
        ck::MotorStatus_Motor* m = mMotorStatusMsg.add_motors();
        m->set_id(id);
        m->set_sensor_position(mCtrl->GetSelectedSensorPosition());
        m->set_sensor_velocity(mCtrl->GetSelectedSensorVelocity());
        m->set_bus_voltage(mCtrl->GetBusVoltage());
        switch (mType)
        {
        case MotorType::TALON_FX:
        {
            TalonFX* tfx = dynamic_cast<TalonFX*>(mCtrl);
            m->set_bus_current(tfx->GetSupplyCurrent());
            m->set_stator_current(tfx->GetStatorCurrent());
        }
            break;
        case MotorType::TALON_SRX:
        {
            TalonSRX* tsrx = dynamic_cast<TalonSRX*>(mCtrl);
            m->set_bus_current(tsrx->GetSupplyCurrent());
            m->set_stator_current(tsrx->GetStatorCurrent());
        }
            break;
        default:
        {

        }
            break;
        }
    });

    // for (auto const& [key, val] : DataManager::getInstance().motorObjectList)
    // {
    //     ck::MotorStatus_Motor* m = mMotorStatusMsg.add_motors();
    //     m->set_id(key);
    //     m->set_sensor_position(val->GetSelectedSensorPosition());
    //     m->set_sensor_velocity(val->GetSelectedSensorVelocity());
    //     m->set_bus_voltage(val->GetBusVoltage());
    //     switch (DataManager::getInstance().motorTypeList[key])
    //     {
    //     case MotorType::TALON_FX:
    //     {
    //         TalonFX* tfx = reinterpret_cast<TalonFX*>(val);
    //         m->set_bus_current(tfx->GetSupplyCurrent());
    //         m->set_stator_current(tfx->GetStatorCurrent());
    //     }
    //         break;
    //     case MotorType::TALON_SRX:
    //     {
    //         TalonSRX* tsrx = reinterpret_cast<TalonSRX*>(val);
    //         m->set_bus_current(tsrx->GetSupplyCurrent());
    //         m->set_stator_current(tsrx->GetStatorCurrent());
    //     }
    //         break;
    //     default:
    //     {

    //     }
    //         break;
    //     }
    // }

    if (mMotorStatusMsg.SerializeToArray(mMotorStatusBuf, mMotorStatusMsg.ByteSizeLong()))
    {
        NetworkManager::getInstance().sendMessage(MOTOR_STATUS_MESSAGE_GROUP, mMotorStatusBuf, mMotorStatusMsg.ByteSizeLong());
    }
    else
    {
        std::cout << "Motor status message failed to serialize. Message probably too large or invalid." << std::endl;
    }
}