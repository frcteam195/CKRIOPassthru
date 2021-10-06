#pragma once
#include "DataStructures.hpp"
#include "utils/Singleton.hpp"
#include "zmq/zmq.hpp"
#include "ctre/Phoenix.h"
#include <vector>
#include <map>

class DataManager : public Singleton<DataManager>
{
    friend Singleton;
public:
    std::vector<MotorData> motorData;
    std::vector<MotorConfigData> motorConfigData;
    std::map<uint16_t, TalonFX*> motorObjectList;
private:
    DataManager();
    zmq::context_t zmqCtx;
    zmq::socket_t zmqSendSock;
    zmq::socket_t zmqRecvSock;
};