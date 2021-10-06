#include "NetworkManager.hpp"
#include <iostream>

NetworkManager::NetworkManager() : zmqCtx(), zmqSendSock(zmqCtx, zmq::socket_type::radio), zmqRecvSock(zmqCtx, zmq::socket_type::dish), recvMsgMap()
{
    zmqRecvSock.bind(std::string("udp://*:") + CK_COMM_PORT);
    zmqSendSock.connect(std::string("udp://") + CK_CO_IP + std::string(":") + CK_COMM_PORT);
}

void NetworkManager::joinGroup(const char *group)
{
    zmqRecvSock.join(group);
}

bool NetworkManager::receiveMessagePump()
{
    try {
        const uint8_t bytes[BUF_SIZE] = {0};
        zmq::message_t msg(bytes, BUF_SIZE);
        zmq::recv_result_t retVal = zmqRecvSock.recv(msg, zmq::recv_flags::dontwait);
        if (retVal.has_value() && retVal.value_or(0) > 0)
        {
            std::vector<uint8_t> vec(bytes, bytes + msg.size());
            std::string msgGroup = msg.group();
            recvMsgMap[msgGroup] = vec;
            return true;
        }
        return false;
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

bool NetworkManager::sendMessage(std::string group, std::vector<uint8_t> &bytes)
{
    return sendMessage(group, &bytes[0], bytes.size());
}

bool NetworkManager::sendMessage(std::string group, uint8_t bytes[], int arrSize)
{
    try {
        zmq::message_t msg(bytes, arrSize);
        msg.set_group(group.c_str());
        zmq::send_result_t retVal = zmqSendSock.send(msg, zmq::send_flags::dontwait);
        return retVal.has_value() && retVal.value_or(0) > 0;
    }
    catch (std::exception &e) {
        std::cout << "Error sending message: " << e.what() << std::endl;
        return false;
    }
}