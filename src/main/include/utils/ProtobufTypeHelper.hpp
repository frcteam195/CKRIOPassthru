#pragma once
#include <google/protobuf/message.h>

template<typename T>
bool getTypedMessage(const google::protobuf::Message& msg, T& t_msg)
{
    try {
        t_msg = dynamic_cast<const T&>(msg);
    }
    catch (std::exception& ex)
    {
        return false;
    }
    return true;
}