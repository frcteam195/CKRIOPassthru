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

    void listStoredMessages();
private:
    NetworkManager();
    zmq::context_t zmqCtx;
    zmq::socket_t zmqSendSock;
    zmq::socket_t zmqRecvSock;
    
    std::map<std::string, std::vector<uint8_t>> recvMsgMap;

    // const std::string CK_CO_IP = "10.1.95.5";
    // const std::string CK_CO_IP = "10.1.95.223";
    const std::string CK_CO_IP = "10.0.2.82";   //mtodd laptop home
    const std::string CK_COMM_PORT = "5801";

    static constexpr int BUF_SIZE = 1500;
};