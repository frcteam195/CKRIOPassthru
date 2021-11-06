#pragma once
#define ZMQ_BUILD_DRAFT_API

#include "utils/Singleton.hpp"
#include "zmq/zmq.hpp"
#include <string>
#include <map>
#include <vector>

class NetworkManager : public Singleton<NetworkManager>
{
    friend Singleton;
public:
    void joinGroup(const char *group);
    bool receiveMessagePump();
    bool getMessage(std::string group, std::vector<uint8_t> &bytes);
    bool sendMessage(std::string group, std::vector<uint8_t> &bytes);
    bool sendMessage(std::string group, void* bytes, int arrSize);

    void connectListener(std::string ip);

    void listStoredMessages();
private:
    NetworkManager();
    ~NetworkManager();
    bool sendMessage(std::string group, void* bytes, int arrSize, zmq::socket_t* zSock);
    zmq::context_t zmqCtx;
    std::vector<zmq::socket_t*> zmqSendSockVec;
    // zmq::socket_t zmqSendSock;
    zmq::socket_t zmqRecvSock;
    
    std::map<std::string, std::vector<uint8_t>> recvMsgMap;

    const std::string CK_CO_IP = "10.1.95.5";
    const std::string CK_CO2_IP = "10.1.95.6";
    const std::string CK_ROB_TEST_IP = "10.0.2.79";   //rob linux home
    const std::string CK_TODD_TEST_IP = "10.0.2.82";   //mtodd laptop home
    const std::string CK_ROB2_TEST_IP = "10.0.2.87";   //Rob linux vm home
    const std::string CK_SULI_TEST_IP = "10.0.2.108";   //Rob linux vm home
    const std::string CK_COMM_PORT = "5801";

    static constexpr int BUF_SIZE = 1500;
};