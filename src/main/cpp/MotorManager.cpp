#include "MotorManager.hpp"
#include "MotorConfigManager.hpp"
#include "utils/PhoenixHelper.hpp"
#include "utils/ProtobufTypeHelper.hpp"

MotorManager::MotorManager() {}
MotorManager::~MotorManager()
{
    for (auto const& [key, val] : mRegisteredMotorList)
    {
        (void)key;  //Remove unused warning
        delete val;
    }
}

void MotorManager::forEach(std::function<void(uint16_t, BaseTalon*, MotorType)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(motorLock);
    for (auto const& [key, val] : mRegisteredMotorList)
    {
        func(key, val, mRegisteredMotorTypeList[key]);
    }
}

void MotorManager::onMotor(uint16_t id, std::function<void(uint16_t, BaseTalon*, MotorType)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(motorLock);
    if (mRegisteredMotorList.count(id))
    {
        func(id, mRegisteredMotorList[id], mRegisteredMotorTypeList[id]);
    }
}

void MotorManager::onMotor(const google::protobuf::Message& msg, std::function<void(uint16_t, BaseTalon*, MotorType, const ck::MotorConfiguration::Motor&)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(motorLock);
    ck::MotorConfiguration::Motor m;
    if (getTypedMessage(msg, m))
    {
        if (mRegisteredMotorList.count(m.id()))
        {
            func((uint16_t)m.id(), mRegisteredMotorList[m.id()], mRegisteredMotorTypeList[m.id()], m);
        }
    }
}

void MotorManager::registerMotor(uint16_t id, MotorType motorType, CANInterface canInterface)
{
    std::scoped_lock<std::recursive_mutex> lock(motorLock);
    if (id < 64)
    {
        if (!mRegisteredMotorList.count(id))
        {
            std::string canNetwork = ck::getCANInterfaceName(canInterface);
            switch (motorType)
            {
            case MotorType::TALON_FX:
            {
                mRegisteredMotorList[id] = new TalonFX(id, canNetwork);
            }
                break;
            case MotorType::TALON_SRX:
            {
                mRegisteredMotorList[id] = new TalonSRX(id);
            }
                break;
            default:
            {
                std::cout << "Motor " << id << " does not have a valid type " << (int)motorType << " on bus " << canNetwork << std::endl;
                return;
            }
                break;
            }
            std::cout << "Motor " << id << " created with type " << (int)motorType << " on bus " << canNetwork << std::endl;
            mRegisteredMotorTypeList[id] = motorType;
        }
        mRegisteredMotorHeartbeatList[id] = kMaxHeartbeatTicks;
    }
}

void MotorManager::deleteMotor(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(motorLock);
    if (mRegisteredMotorList.count(id))
    {
        if (mRegisteredMotorList[id])
        {
            delete mRegisteredMotorList[id];
        }
        mRegisteredMotorList.erase(id);
    }
    if (mRegisteredMotorTypeList.count(id))
    {
        mRegisteredMotorTypeList.erase(id);
    }
    MotorConfigManager::getInstance().deleteMotor(id);
}

void MotorManager::processHeartbeat()
{
    std::scoped_lock<std::recursive_mutex> lock(motorLock);
    for (auto it = mRegisteredMotorHeartbeatList.cbegin(); it != mRegisteredMotorHeartbeatList.cend(); )
    {
        mRegisteredMotorHeartbeatList[it->first]--;
        if (mRegisteredMotorHeartbeatList[it->first] <= 0)
        {
            uint16_t currMotor = it->first;
            // std::cout << std::endl << "Deleting motor, id: " << currMotor << std::endl;
            deleteMotor(it->first);
            it = mRegisteredMotorHeartbeatList.erase(it);
            std::cout << "Motor deleted, id: " << currMotor << std::endl;
        }
        else
        {
            ++it;
        }
    }
}

bool MotorManager::motorExists(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(motorLock);
    return mRegisteredMotorList.count(id);
}

BaseTalon* MotorManager::getMotor(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(motorLock);
    if (mRegisteredMotorList.count(id))
    {
        return mRegisteredMotorList[id];
    }
    else
    {
        return nullptr;
    }
}
