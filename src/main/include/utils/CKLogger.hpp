#pragma once

#include "Singleton.hpp"
#include <mutex>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <iostream>


class CKLogger
{
public:
    template<class T>
    const CKLogger& operator<<(const T& v)
    {
        std::scoped_lock<std::mutex> lock(listLock);
        std::string s = toString(v);
        if (!msgList.count(s))
        {
            msgList[s] = 0;
            mStream << v << std::endl;
        }
        return *this;
    }

    const CKLogger& operator<<(std::ostream& (*F)(std::ostream&)) const
    {
        // std::scoped_lock<std::mutex> lock(listLock);
        return *this;
    }

    void messagePump()
    {
        std::scoped_lock<std::mutex> lock(listLock);
        std::cout << mStream.rdbuf();
        msgList.clear();
    }

    CKLogger() {}
    ~CKLogger() {}
private:
    std::mutex listLock;
    std::unordered_map<std::string, int> msgList;
    std::stringstream mStream;

    template<typename T>
    std::string toString(const T& x)
    {
        std::ostringstream os;
        os << x;
        return os.str();
    }
};

extern CKLogger ckLogger;