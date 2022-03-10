#include "NetworkManager.hpp"
#include <iostream>
#include "ExternalControlManager.hpp"
#include <frc/Timer.h>

NetworkManager::NetworkManager() : zmqCtx(), zmqSendSockVec(), zmqRecvSock(zmqCtx, zmq::socket_type::dish), recvMsgMap()
{
    zmqRecvSock.bind(std::string("udp://*:") + CK_COMM_PORT);
    connectListener(CK_CO_IP);
    // connectListener(CK_CO2_IP);
    // connectListener(CK_CO3_IP);
    // connectListener(CK_ROB_TEST_IP);
    // connectListener(CK_ROB2_TEST_IP);
    // connectListener(CK_ROB3_TEST_IP);
    // connectListener(CK_TODD_TEST_IP);
    // connectListener(CK_SULI_TEST_IP);
}

NetworkManager::~NetworkManager()
{
    for (zmq::socket_t* zSock : zmqSendSockVec)
    {
        delete zSock;
    }
}

void NetworkManager::connectListener(std::string ip)
{
    zmq::socket_t* zSock = new zmq::socket_t(zmqCtx, zmq::socket_type::radio);
    zSock->connect(std::string("udp://") + ip + std::string(":") + CK_COMM_PORT);
    zmqSendSockVec.push_back(zSock);
}

void NetworkManager::joinGroup(const char *group)
{
    zmqRecvSock.join(group);
}

bool NetworkManager::receiveMessagePump()
{
    try {
        zmq::message_t msg(BUF_SIZE);
        zmq::recv_result_t retVal = zmqRecvSock.recv(msg, zmq::recv_flags::dontwait);
        bool readSuccessful = retVal.has_value() && (retVal.value() != EAGAIN);
        while (retVal.has_value() && (retVal.value() != EAGAIN))
        {
            std::vector<uint8_t> buf(msg.size(), 0);
            memcpy(&buf[0], msg.data(), msg.size());
            std::string msgGroup = msg.group();
            //TODO: verify that this vector is moved into map properly and not copied
            recvMsgMap[msgGroup] = std::move(buf);
            ExternalControlManager::getInstance().externalControlMsgReceived();

            //Clear any buffered msgs
            retVal = zmqRecvSock.recv(msg, zmq::recv_flags::dontwait);
        }

        return readSuccessful;
    }
    catch (std::exception &e) {
        std::cout << "Error receiving message: " << e.what() << std::endl;
        return false;
    }
}

bool NetworkManager::getMessage(std::string group, std::vector<uint8_t> &bytes)
{
    if (recvMsgMap.count(group))
    {
        bytes = recvMsgMap[group];
        recvMsgMap.erase(group);
        return true;
    }
    return false;
}

void NetworkManager::listStoredMessages()
{
    for (auto const& [key, val] : recvMsgMap)
    {
        (void)val;
        std::cout << "Group: " << key << std::endl;
    }
}

bool NetworkManager::sendMessage(std::string group, std::vector<uint8_t> &bytes)
{
    return sendMessage(group, &bytes[0], bytes.size());
}

bool NetworkManager::sendMessage(std::string group, void* bytes, int arrSize)
{
    bool retVal = true;
    for (zmq::socket_t* zSock : zmqSendSockVec)
    {
        retVal &= sendMessage(group, bytes, arrSize, zSock);
    }
    return retVal;
}

bool NetworkManager::sendMessage(std::string group, void* bytes, int arrSize, zmq::socket_t* zSock)
{
    try {
        zmq::message_t msg(bytes, arrSize);
        msg.set_group(group.c_str());
        zmq::send_result_t retVal = zSock->send(msg, zmq::send_flags::dontwait);
        return retVal.has_value() && retVal.value_or(0) > 0;
    }
    catch (std::exception &e) {
        std::cout << "Error sending message: " << e.what() << std::endl;
        return false;
    }
}